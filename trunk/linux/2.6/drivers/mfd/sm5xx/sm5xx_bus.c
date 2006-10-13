/*
 * drivers/mfd/sm5xx/sm5xx_bus.c
 *
 * DESCRIPTION:
 *   sm5xx bus driver.
 *
 * AUTHOR:
 *  Andrey Volkov <avolkov@varma-el.com>
 *
 * COPYRIGHT:
 *  2005-2006, Varma Electronics Oy
 *
 * Based on drivers by Robert Whaley and Jeff Lackey,
 * Applied Data Systems.
 *
 * LICENCE:
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive
 *  for more details.
 *
 */
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/fb.h>
#include <linux/circ_buf.h>
#include <linux/io.h>

#include <mfd/sm5xx/sm5xx.h>
#include <mfd/sm5xx/sm5xx_ci_regs.h>
#include <video/sm5xx_fb_regs.h>
#include "../../video/sm/sm5xx_fb.h"
#include "../../i2c/busses/i2c-sm5xx.h"

#define DRIVER_NAME	"sm5xx_bus"

#define SM5XX_ALL_INTS_MASK	\
(FINIT(INT_MASK,USB_SLAVE_PLUG_IN,ENABLE) | \
 FINIT(INT_MASK,GPIO54,ENABLE) | \
 FINIT(INT_MASK,GPIO53,ENABLE) | \
 FINIT(INT_MASK,GPIO52,ENABLE) | \
 FINIT(INT_MASK,GPIO51,ENABLE) | \
 FINIT(INT_MASK,GPIO50,ENABLE) | \
 FINIT(INT_MASK,GPIO49,ENABLE) | \
 FINIT(INT_MASK,GPIO48,ENABLE) | \
 FINIT(INT_MASK,I2C,ENABLE) | \
 FINIT(INT_MASK,PWM,ENABLE) | \
 FINIT(INT_MASK,DMA,ENABLE) | \
 FINIT(INT_MASK,PCI,ENABLE) | \
 FINIT(INT_MASK,I2S,ENABLE) | \
 FINIT(INT_MASK,AC97,ENABLE) | \
 FINIT(INT_MASK,USB_SLAVE,ENABLE) | \
 FINIT(INT_MASK,UART1,ENABLE) | \
 FINIT(INT_MASK,UART0,ENABLE) | \
 FINIT(INT_MASK,CRT_VSYNC,ENABLE) | \
 FINIT(INT_MASK,8051,ENABLE) | \
 FINIT(INT_MASK,SSP1,ENABLE) | \
 FINIT(INT_MASK,SSP0,ENABLE) | \
 FINIT(INT_MASK,USB_HOST,ENABLE) | \
 FINIT(INT_MASK,2D,ENABLE) | \
 FINIT(INT_MASK,ZVPORT,ENABLE) | \
 FINIT(INT_MASK,PANEL_VSYNC,ENABLE) | \
 FINIT(INT_MASK,CMD_INTPR,ENABLE))

void sm5xx_setup_clock_regs(struct sm5xx_bus *bus);

/* SM5XX_HEADER_SIZE must be aligned at least to SM5XX_ALIGN */

#define sm5xx_read32(regindex)	_sm5xx_read32(regindex, bus)
#define sm5xx_write32(regindex,val)	_sm5xx_write32(regindex, val, bus)

#if (defined(CONFIG_SM5XX_MMIO_BUSMASTER) || \
	 defined(CONFIG_SM5XX_PCI)) && !defined(CONFIG_SM5XX_FORCE_SLAVE)
#define SM5XX_CI_ZONE_SIZE	PAGE_SIZE
#else
#define SM5XX_CI_ZONE_SIZE	1024
#endif

#ifdef DEBUG
void sm5xx_dump_regs(struct sm5xx_bus *bus)
{
	int i;
	printk("\n--------- SM5XX REGISTERS DUMP ---------\n"
	       "Configuration Registers [0x00 - 0x6C]:");
	for (i = 0; i < 0x6F; i += 4) {
		if (i % 16 == 0)
			printk("\n %08X:", i);
		printk(" %08X", _sm5xx_read32(i, bus));
	}

	printk("\n\nCRT Graphics Control Register [0x080200 - 0x080224]:");
	for (i = 0x080200; i < 0x080227; i += 4) {
		if (i % 16 == 0)
			printk("\n %08X:", i);
		printk(" %08X", _sm5xx_read32(i, bus));
	}

	printk("\n\nCRT Cursor Control Register [0x080230 - 0x08023C]:");
	for (i = 0x080230; i < 0x08023F; i += 4) {
		if (i % 16 == 0)
			printk("\n %08X:", i);
		printk(" %08X", _sm5xx_read32(i, bus));
	}

	printk("\n\n2D Drawing Engine Register [0x100000 - 0x100050]:");
	for (i = 0x100000; i < 0x100053; i += 4) {
		if (i % 16 == 0)
			printk("\n %08X:", i);
		printk(" %08X", _sm5xx_read32(i, bus));
	}

	printk("\n--------------------------------\n");
}

u32 _sm5xx_read32(int regindex, const struct sm5xx_bus *_bus)
{
	return readl(_bus->regbase + regindex);
}

void _sm5xx_write32(int regindex, u32 val, const struct sm5xx_bus *_bus)
{
	writel(val, _bus->regbase + regindex);
}
#endif

struct sm5xx_mem_list {
	struct list_head list;
	unsigned long length;
	void *address;
};

static LIST_HEAD(sm5xx_bus_list);

void sm5xx_fix_cache_bug(struct sm5xx_bus *bus)
{
	volatile u32 ignore;

	ignore = sm5xx_read32(0);
	ignore = sm5xx_read32(0x00000100);
	ignore = sm5xx_read32(0x00100000);
	ignore = sm5xx_read32(0x00100100);
	ignore = sm5xx_read32(bus->mem_limit - 0x00100000);
	ignore = sm5xx_read32(bus->mem_limit - 0x00100100);
}

static struct sm5xx_mem_list *sm5xx_find_neighbor(struct sm5xx_bus *bus,
					  struct sm5xx_mem_list *neighbor)
{
	struct sm5xx_mem_list *lst;
	void *top = neighbor->address + neighbor->length;

	pr_debug(" %s: %p, length: %#lx\n", __FUNCTION__,
		neighbor->address, neighbor->length);

	list_for_each_entry(lst, &bus->free_list_head, list) {
		pr_debug(" %p %#lx\n", lst, lst->length);
		/* is the top aligned with the bottom of something in the free
		   list */
		if (lst->address == top)
			return lst;

		/* is the bottom alighed with the top of something in the free
		   list */
		if (lst->address + lst->length == neighbor->address)
			return lst;
	}

	pr_debug(" No match\n");

	return NULL;
}

/* free list is sorted by size (smallest first) */

static void sm5xx_insert_by_size(struct sm5xx_bus *bus,
				 struct sm5xx_mem_list *ins)
{
	struct sm5xx_mem_list *lst = NULL;

	pr_debug(" %s: %p, length: %ld, lst: %p\n", __FUNCTION__, ins,
		ins->length, lst);

	list_for_each_entry(lst, &bus->free_list_head, list) {
		pr_debug(" %ld %ld\n", lst->length, ins->length);
		if (lst->length >= ins->length)
			break;
	}

	list_add_tail(&ins->list, &lst->list);
}

static void sm5xx_reclaim(struct sm5xx_bus *bus, struct sm5xx_mem_list *reclaim)
{
	struct sm5xx_mem_list *lst;

	pr_debug(" %s: %p\n", __FUNCTION__, reclaim);

	while ((lst = sm5xx_find_neighbor(bus, reclaim))) {
		pr_debug(" neighbor: %p, length: %ld\n", reclaim, lst->length);

		list_del(&lst->list);

		if (lst->address < reclaim->address) {
			lst->length += reclaim->length;
			kfree(reclaim);
			reclaim = lst;
		} else {
			reclaim->length += lst->length;
			kfree(lst);
		}
	}

	sm5xx_insert_by_size(bus, reclaim);
}

void sm5xx_free(struct sm5xx_bus *bus, void *vaddr)
{
	struct sm5xx_mem_list *mem_lst;

	list_for_each_entry(mem_lst, &bus->mem_list_head, list) {
		if (mem_lst->address == vaddr) {
			list_del(&mem_lst->list);
			sm5xx_reclaim(bus, mem_lst);
			break;
		}
	}
}

void *sm5xx_malloc(struct sm5xx_bus *bus, u32 size, u32 align)
{
	struct sm5xx_mem_list *free_lst = NULL, *new_lst;
	unsigned long split_length;

	pr_debug(" %s: %x\n", __FUNCTION__, size);

	/* Check & correct min requirements */
	size = ALIGN(size, SM5XX_MIN_ALLOC_SIZE);
	if (align < SM5XX_MIN_ALLOC_SIZE)
		align = SM5XX_MIN_ALLOC_SIZE;

	BUG_ON(align & (align - 1));	/* must be power of 2 */

	list_for_each_entry(free_lst, &bus->free_list_head, list) {
		pr_debug("lst: %p, length: %lx\n", free_lst, free_lst->length);

		/* find one big enough */
		if (free_lst->length >= size) {
			split_length = free_lst->length - size;

			/* align the split */
			split_length &= ~(align - 1);

			pr_debug(" split_length: %ld\n", split_length);

			/* split block if large enough */
			if (split_length >= SM5XX_MIN_ALLOC_SIZE) {

				new_lst = kmalloc(sizeof(*new_lst), GFP_KERNEL);
				if (!new_lst)
					break;

				new_lst->length =
				    free_lst->length - split_length;
				new_lst->address =
				    free_lst->address + split_length;

				free_lst->length = split_length;

				list_del(&free_lst->list);
				sm5xx_reclaim(bus, free_lst);

			} else {
				list_del(&free_lst->list);
				new_lst = free_lst;
			}

			list_add(&new_lst->list, &bus->mem_list_head);

			pr_debug(" SM5XX allocated: %p length: %lx\n",
				new_lst->address, new_lst->length);

			return new_lst->address;
		}
	}

	printk(KERN_WARNING " SM5XX failed to allocate: %#x\n", size);
	return NULL;
}

static irqreturn_t sm5xx_ci_irq(struct sm5xx_bus *bus, u32 irq, void *dev_id)
{
	/*TODO: */
	sm5xx_write32(RAW_INT_STATUS, FINIT(RAW_INT_STATUS, CMD_INTPR, CLEAR));
	return IRQ_HANDLED;
}

/* XXX: All of this mess needs to be wired up as a chained IRQ chip.. */
static irqreturn_t sm5xx_irq_demux(int irq, void *dev_id)
{
	u32 pending, irq_mask;
	struct sm5xx_irqdesc *desc;
	struct sm5xx_bus *bus = dev_id;

	pending = sm5xx_read32(INT_STATUS);
	if (unlikely(pending == 0))
		return IRQ_NONE;

	do {
		for (desc = bus->irq_desc;
		     desc < &bus->irq_desc[SM5XX_NR_INTS] && desc->handler;
		     desc++) {
		     	irqreturn_t ret;
			irq_mask = pending & desc->mask;

			ret = desc->handler(bus, irq_mask, desc->data);

			/*
			 * Sorry, but irq MUST be handled,
			 * else you reduce performance essentially
			 */
			if (irq_mask && ret == IRQ_NONE)
				BUG();
		}
		pending = sm5xx_read32(INT_STATUS);
	} while (pending);

	return IRQ_HANDLED;
}

static inline u32 _sm5xx_mask_irq(struct sm5xx_bus *bus, u32 irq_mask)
{
	u32 ret = sm5xx_read32(INT_MASK);
	sm5xx_write32(INT_MASK, ret & ~irq_mask);
	return ret;
}

static inline void _sm5xx_unmask_irq(struct sm5xx_bus *bus, u32 irq_mask)
{
	sm5xx_write32(INT_MASK, sm5xx_read32(INT_MASK) | irq_mask);
}

u32 sm5xx_mask_irq(struct sm5xx_bus *bus, u32 irq_mask)
{
	u32 ret;
	spin_lock(&bus->irq_lock);
	ret = _sm5xx_mask_irq(bus, SM5XX_ALL_INTS_MASK);
	spin_unlock(&bus->irq_lock);
	return ret;
}

void sm5xx_unmask_irq(struct sm5xx_bus *bus, u32 irq_mask)
{
	spin_lock(&bus->irq_lock);
	_sm5xx_unmask_irq(bus, irq_mask);
	spin_unlock(&bus->irq_lock);
}

int sm5xx_request_irq(struct sm5xx_bus *bus, int irq_mask,
		      sm5xx_handler_t handler, void *data, int irq_enable)
{
	struct sm5xx_irqdesc *desc;
	int ret = -EINVAL;
	u32 old_irq_mask;

	spin_lock(&bus->irq_lock);
	old_irq_mask = _sm5xx_mask_irq(bus, SM5XX_ALL_INTS_MASK);
	for (desc = bus->irq_desc; desc < &bus->irq_desc[SM5XX_NR_INTS]; desc++) {
		if (desc->mask & irq_mask)
			break;
		if (!desc->handler) {
			desc->mask = irq_mask;
			desc->handler = handler;
			desc->data = data;
			if (irq_enable)
				old_irq_mask |= irq_mask;
			ret = 0;
			break;
		}
	}
	_sm5xx_unmask_irq(bus, old_irq_mask);
	spin_unlock(&bus->irq_lock);

	return ret;
}

int sm5xx_enable_subsystem(struct sm5xx_bus *bus, u32 normal_gate_mask,
			   u32 suspend_gate_mask)
{
	u32 reg, suspend_gate;

	suspend_gate =
	    (bus->pm_normal_gate_reg ==
	     POWER_MODE0_GATE) ? POWER_MODE1_GATE : POWER_MODE0_GATE;
	sm5xx_acquire_control_lock(bus);

	reg = sm5xx_read32(bus->pm_normal_gate_reg);
	reg |= normal_gate_mask;
	sm5xx_write32(bus->pm_normal_gate_reg, reg);

	if (suspend_gate_mask) {
		reg = sm5xx_read32(suspend_gate);
		reg |= suspend_gate_mask;
		sm5xx_write32(suspend_gate, reg);
	}
	sm5xx_release_control_lock(bus);
	return reg;
}

void sm5xx_disable_subsystem(struct sm5xx_bus *bus, u32 gate_reg_mask)
{
	u32 reg;
	gate_reg_mask &= ~(FINIT(POWER_MODE_GATE, HOST, ENABLE) |
			   FINIT(POWER_MODE_GATE, INTMEM, ENABLE));
	if (gate_reg_mask) {
		sm5xx_acquire_control_lock(bus);

		reg = sm5xx_read32(POWER_MODE0_GATE);
		reg &= ~gate_reg_mask;
		sm5xx_write32(POWER_MODE0_GATE, reg);

		reg = sm5xx_read32(POWER_MODE1_GATE);
		reg &= ~gate_reg_mask;
		sm5xx_write32(POWER_MODE1_GATE, reg);

		sm5xx_release_control_lock(bus);
	}
}

#ifdef CONFIG_PM
static void sm5xx_save_regs(void)
{
}

static void sm5xx_restore_regs(void)
{
}

static void sm5xx_power_up_chip(void)
{
}

static void sm5xx_power_down_chip(void)
{
}
#endif

/*
 *	SM501 "Register Access Bus."
 */
static int sm5xx_match(struct device *_dev, struct device_driver *_drv)
{
	struct sm5xx_dev *dev = SM5XX_DEV(_dev);
	struct sm5xx_driver *drv = SM5XX_DRV(_drv);

	return dev->devid == drv->devid;
}

static int sm5xx_bus_suspend(struct device *dev, pm_message_t state)
{
	struct sm5xx_dev *sadev = SM5XX_DEV(dev);
	struct sm5xx_driver *drv = SM5XX_DRV(dev->driver);
	int ret = 0;

	if (drv && drv->suspend)
		ret = drv->suspend(sadev, state);

	return ret;
}

static int sm5xx_bus_resume(struct device *dev)
{
	struct sm5xx_dev *sadev = SM5XX_DEV(dev);
	struct sm5xx_driver *drv = SM5XX_DRV(dev->driver);
	int ret = 0;

	if (drv && drv->resume)
		ret = drv->resume(sadev);

	return ret;
}

static int sm5xx_bus_probe(struct device *dev)
{
	struct sm5xx_dev *sadev = SM5XX_DEV(dev);
	struct sm5xx_driver *drv = SM5XX_DRV(dev->driver);
	int ret = -ENODEV;

	if (drv->probe)
		ret = drv->probe(sadev);

	return ret;
}

static int sm5xx_bus_remove(struct device *dev)
{
	struct sm5xx_dev *sadev = SM5XX_DEV(dev);
	struct sm5xx_driver *drv = SM5XX_DRV(dev->driver);
	int ret = 0;

	if (drv->remove)
		ret = drv->remove(sadev);

	return ret;
}

struct bus_type sm5xx_bus_type = {
	.name = "sm5xx-mem-bus",
	.match = sm5xx_match,
	.suspend = sm5xx_bus_suspend,
	.resume = sm5xx_bus_resume,
};

struct sm5xx_bus *sm5xx_bus_alloc(void)
{
	struct sm5xx_bus *bus;

	bus = kzalloc(sizeof(struct sm5xx_bus), GFP_KERNEL);
	if (unlikely(!bus))
		return ERR_PTR(-ENOMEM);

	INIT_LIST_HEAD(&bus->bus_list);
	return bus;
}

void sm5xx_bus_free(struct sm5xx_bus *bus)
{
	list_del(&bus->bus_list);
	kfree(bus);
}

static void sm5xx_reset_regs(struct sm5xx_bus *bus)
{
	sm5xx_write32(ARBITRATION_CTRL, 0x05146732);

	bus->pm_normal_gate_reg = POWER_MODE0_GATE;

	sm5xx_write32(POWER_MODE0_GATE,
		      FINIT(POWER_MODE_GATE, INTMEM, ENABLE) |
		      FINIT(POWER_MODE_GATE, HOST, ENABLE));

	sm5xx_write32(POWER_MODE1_GATE,
		      FINIT(POWER_MODE_GATE, INTMEM, ENABLE) |
		      FINIT(POWER_MODE_GATE, HOST, ENABLE));

	sm5xx_write32(POWER_SLEEP_GATE,
		      FINIT_VAL(POWER_SLEEP_GATE, PLL_RECOVERY_CLOCK, 4096));
	/* Defined in sm5xx_pci.c or in sm5xx_platform.c */
	sm5xx_setup_clock_regs(bus);
}

int sm5xx_get_bios_info(struct sm5xx_bus *bus,
			struct fb_var_screeninfo *var, display_t type)
{
	return 0;
}

int sm5xx_bus_init(struct sm5xx_bus *bus)
{
	int ret;

	INIT_LIST_HEAD(&bus->free_list_head);
	INIT_LIST_HEAD(&bus->mem_list_head);
	init_MUTEX(&bus->control_regs_lock);
	spin_lock_init(&bus->irq_lock);

	bus->all_free_mem = kmalloc(sizeof(*bus->all_free_mem), GFP_KERNEL);
	if (unlikely(!bus->all_free_mem))
		return -ENOMEM;

	bus->all_free_mem->address = bus->mem;
	bus->all_free_mem->length = bus->mem_limit;
	list_add(&bus->all_free_mem->list, &bus->free_list_head);

#if (defined(CONFIG_SM5XX_MMIO_BUSMASTER) || \
	 defined(CONFIG_SM5XX_PCI)) && !defined(CONFIG_SM5XX_FORCE_SLAVE)

	bus->ci_list.buf = dma_alloc_coherent(bus->bus_dev, SM5XX_CI_ZONE_SIZE,
					      &bus->h_ci_list,
					      GFP_KERNEL | GFP_DMA);
#else
	bus->ci_list.buf = sm5xx_malloc(bus, SM5XX_CI_ZONE_SIZE, SM5XX_ALIGN);
	if (likely(bus->ci_list.buf))
		bus->h_ci_list = sm5xx_virt_to_offset(bus, bus->ci_list.buf);
#endif
	if (unlikely(!bus->ci_list.buf))
		return -ENOMEM;
	bus->ci_list.head = bus->ci_list.tail = 0;

	if ((ret = request_irq(bus->irq, sm5xx_irq_demux,
			       IRQF_DISABLED, "sm5xx_irq_demux", bus)) < 0) {
		printk(KERN_ERR DRIVER_NAME " - failed to attach interrupt\n");
		return ret;
	}

	sm5xx_reset_regs(bus);
	list_add_tail(&bus->bus_list, &sm5xx_bus_list);

	ret = sm5xx_request_irq(bus, FINIT(INT_MASK, CMD_INTPR, ENABLE),
				sm5xx_ci_irq, NULL, 0);
	printk(DRIVER_NAME " - inited\n");
	return ret;
}

const char *sm5xx_dev_name[] = {
	"sm5xx-fb",
	"sm5xx-ohci",
	"sm5xx-i2c",
};

struct sm5xx_dev *sm5xx_find_child(struct sm5xx_bus *bus, unsigned int child_id)
{
	struct klist_iter it;
	struct sm5xx_dev *child_dev;
	struct klist_node *node;

	if (unlikely(!bus))
		return NULL;

	klist_iter_init_node(&bus->bus_dev->klist_children, &it, NULL);

	while ((node = klist_next(&it))) {
		child_dev =
		    SM5XX_DEV(container_of(node, struct device, knode_parent));
		if (child_dev->devid == child_id && get_device(&child_dev->dev))
			break;
	}
	klist_iter_exit(&it);
	return child_dev;
}

static int sm5xx_init_child(struct sm5xx_dev *sdev, struct device *dev,
			    unsigned int id)
{
	int rtn;

	memset(sdev, 0, sizeof(*sdev));
	strcpy(sdev->dev.bus_id, sm5xx_dev_name[id]);
	sdev->devid = id;
	sdev->dma_mask = *dev->dma_mask;
	sdev->dev.parent = dev;
	sdev->dev.bus = &sm5xx_bus_type;
	sdev->dev.dma_mask = &sdev->dma_mask;

	rtn = device_register(&sdev->dev);
	if (likely(!rtn))
		printk(KERN_INFO "registered %s device\n", sdev->dev.bus_id);

	return rtn;
}

static int compare_dev(struct device *dev, void *devid)
{
	return (SM5XX_DRV(dev->driver)->devid == (unsigned int)devid) ? 1 : 0;
}

int sm5xx_driver_register(struct sm5xx_driver *driver)
{
	int ret;
	struct sm5xx_dev *dev;
	struct list_head *tmp;

	if (driver->drv.bus != &sm5xx_bus_type)
		return -EINVAL;

	if (bus_for_each_dev(&sm5xx_bus_type, NULL, (void *)driver->devid,
			     compare_dev))
		return -EBUSY;

	list_for_each(tmp, &sm5xx_bus_list) {
		ret = -ENOMEM;
		dev = kmalloc(sizeof(*dev), GFP_KERNEL);
		if (likely(dev))
			ret =
			    sm5xx_init_child(dev,
					     list_entry(tmp, sm5xx_bus_t,
							bus_list)->bus_dev,
					     driver->devid);
		if (ret < 0)
			goto cleanup;
	}

	driver->drv.probe = sm5xx_bus_probe;
	driver->drv.remove = sm5xx_bus_remove;
	driver->drv.bus = &sm5xx_bus_type;
	ret = driver_register(&driver->drv);
	if (ret >= 0)
		return ret;

cleanup:
	kfree(dev);

	return ret;
}

void sm5xx_driver_unregister(struct sm5xx_driver *driver)
{
	driver_unregister(&driver->drv);
}

MODULE_AUTHOR("Andrey Volkov");
MODULE_DESCRIPTION("SM5xx family MFDs support");
MODULE_LICENSE("GPL");

EXPORT_SYMBOL_GPL(sm5xx_fix_cache_bug);
EXPORT_SYMBOL_GPL(sm5xx_bus_alloc);
EXPORT_SYMBOL_GPL(sm5xx_bus_free);
EXPORT_SYMBOL_GPL(sm5xx_bus_init);
EXPORT_SYMBOL_GPL(sm5xx_bus_type);
EXPORT_SYMBOL_GPL(sm5xx_driver_register);
EXPORT_SYMBOL_GPL(sm5xx_driver_unregister);
EXPORT_SYMBOL_GPL(sm5xx_free);
EXPORT_SYMBOL_GPL(sm5xx_malloc);
EXPORT_SYMBOL_GPL(sm5xx_find_child);
EXPORT_SYMBOL_GPL(sm5xx_enable_subsystem);
EXPORT_SYMBOL_GPL(sm5xx_disable_subsystem);
