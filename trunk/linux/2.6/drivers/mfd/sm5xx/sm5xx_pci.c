/*
 * drivers/mfd/sm5xx/sm5xx_pci.c
 *
 * DESCRIPTION:
 *  PCI device driver.
 *
 * AUTHOR:
 * Andrey Volkov <avolkov@varma-el.com>
 *
 * COPYRIGHT:
 * 2005-2006, Varma Electronics Oy
 *
 * LICENCE:
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file COPYING in the main directory of this archive
 * for more details.
 *
 */


#include <linux/config.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <linux/circ_buf.h>

#include <mfd/sm5xx/sm5xx.h>

#define sm5xx_read32(regindex)	_sm5xx_read32(regindex, bus)
#define sm5xx_write32(regindex,val)	_sm5xx_write32(regindex, val, bus)

#define DRIVER_NAME	"sm5xx-pci"

void sm5xx_setup_clock_regs(struct sm5xx_bus *bus)
{
	u32 reg;
	u32 normal_reg,low_power_reg;
	u32 power_ctl;
	/* Set SM501 normal operation timings:
		P2XCLK =48 MHz - PANEL clock,
		V2XCLK =48 MHz - CRT clock,
		MCLK =144MHz - local memory,
		M1XCLK =96 MHZ - core
	*/
	if (bus->pm_normal_gate_reg == POWER_MODE0_GATE) {
		normal_reg = POWER_MODE0_CLOCK;
		low_power_reg = POWER_MODE1_CLOCK;
	}
	else {
		normal_reg = POWER_MODE1_CLOCK;
		low_power_reg = POWER_MODE0_CLOCK;
	}

/*	power_ctl = sm5xx_read32(POWER_MODE_CTRL);

	if (FIELD_GET(power_ctl,POWER_MODE_CTRL,MODE) == POWER_MODE_CTRL_MODE_MODE0) {
		power_ctl = FIELD_SET(power_ctl,POWER_MODE_CTRL,MODE,MODE1);
		normal_reg = POWER_MODE1_CLOCK;
		low_power_reg = POWER_MODE0_CLOCK;
	}
	else
	{
		power_ctl = FIELD_SET(power_ctl,POWER_MODE_CTRL,MODE,MODE0);
		normal_reg = POWER_MODE0_CLOCK;
		low_power_reg = POWER_MODE1_CLOCK;
	}
*/
	reg = FINIT(POWER_MODE_CLOCK,P2XCLK_SELECT,288) 	|
		 FINIT(POWER_MODE_CLOCK,P2XCLK_DIVIDER,3) 	|
		 FINIT(POWER_MODE_CLOCK,P2XCLK_SHIFT,1)		|
		 FINIT(POWER_MODE_CLOCK,V2XCLK_SELECT,288) 	|
		 FINIT(POWER_MODE_CLOCK,V2XCLK_DIVIDER,3) 	|
		 FINIT(POWER_MODE_CLOCK,V2XCLK_SHIFT,1)		|

		 FINIT(POWER_MODE_CLOCK,MCLK_SELECT,288) 	|
		 FINIT(POWER_MODE_CLOCK,MCLK_DIVIDER,3) 		|
		 FINIT(POWER_MODE_CLOCK,MCLK_SHIFT,0)		|

		 FINIT(POWER_MODE_CLOCK,M1XCLK_SELECT,288) 	|
		 FINIT(POWER_MODE_CLOCK,M1XCLK_DIVIDER,1) 	|
		 FINIT(POWER_MODE_CLOCK,M1XCLK_SHIFT,1);
	sm5xx_write32(normal_reg, reg);
	msleep(16);
/* TODO.
	sm5xx_write32(POWER_MODE_CTRL, power_ctl);
*/
	/* Set lowpower operation timings:
		P2XCLK =24 MHz - PANEL clock,
		V2XCLK =24 MHz - CRT clock,
		MCLK =72 MHz - local memory,
		M1XCLK =48 MHZ - core
	*/
	reg = FINIT(POWER_MODE_CLOCK,P2XCLK_SELECT,288) 	|
		 FINIT(POWER_MODE_CLOCK,P2XCLK_DIVIDER,3) 	|
		 FINIT(POWER_MODE_CLOCK,P2XCLK_SHIFT,1)		|
		 FINIT(POWER_MODE_CLOCK,V2XCLK_SELECT,288) 	|
		 FINIT(POWER_MODE_CLOCK,V2XCLK_DIVIDER,3) 	|
		 FINIT(POWER_MODE_CLOCK,V2XCLK_SHIFT,1)		|
		 FINIT(POWER_MODE_CLOCK,MCLK_SELECT,288) 	|
		 FINIT(POWER_MODE_CLOCK,MCLK_DIVIDER,3) 		|
		 FINIT(POWER_MODE_CLOCK,MCLK_SHIFT,0)		|
		 FINIT(POWER_MODE_CLOCK,M1XCLK_SELECT,288) 	|
		 FINIT(POWER_MODE_CLOCK,M1XCLK_DIVIDER,1) 	|
		 FINIT(POWER_MODE_CLOCK,M1XCLK_SHIFT,1);

	sm5xx_write32(low_power_reg, reg);
	msleep(16);

	reg = FINIT(MISC_TIMING,EX,OFF)				|
		 FINIT(MISC_TIMING,SDRAM1_SELECT,288) 	|
		 FINIT(MISC_TIMING,SDRAM1_DIVIDER,3) 	|
		 FINIT(MISC_TIMING,SDRAM1_SHIFT,0)		|
		 FINIT(MISC_TIMING,SDRAM0_SELECT,288) 	|
		 FINIT(MISC_TIMING,SDRAM0_DIVIDER,3) 	|
		 FINIT(MISC_TIMING,SDRAM0_SHIFT,0)		|
		 FINIT(MISC_TIMING,ACPI,DISABLE)			|
		 FINIT(MISC_TIMING,USB_HOST_MODE,NORMAL)	|
		 FINIT(MISC_TIMING,DIVIDER,336) 			|
		 FINIT(MISC_TIMING,READ_DELAY,OFF);

	sm5xx_write32(MISC_TIMING,reg);
	msleep(16);
}

static int __devinit
sm5xx_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
	int err;
	struct sm5xx_bus *bus;

	err = pci_enable_device(pdev);
	if (err)
		return err;

	err = pci_set_dma_mask(pdev, (1<<26)-1);
	if (err) {
		printk(KERN_ERR "32-bit PCI DMA addresses not supported\n");
		goto out;
	}

	bus = sm5xx_bus_alloc();
	if (!bus) {
		err = -ENOMEM;
		goto out;
	}
	bus->bus_dev = &pdev->dev;

	bus->reg_addr = pci_resource_start(pdev, 1);
	bus->regs_size = pci_resource_len(pdev, 1);
	err = pci_request_region(pdev, 1, DRIVER_NAME);
	if ( err<0 ) {
		printk (KERN_ERR DRIVER_NAME " cannot reserve MMIO region, aborting\n");
		goto err_free_bus;
	}

	bus->regbase = pci_iomap(pdev, 1, bus->regs_size);
	if (bus->regbase == NULL) {
		printk (KERN_ERR DRIVER_NAME " cannot remap MMIO, aborting\n");
		err = -EIO;
		goto release_mmio;
	}

	sm5xx_write32(SYSTEM_CTRL, sm5xx_read32(SYSTEM_CTRL) |
							FINIT(SYSTEM_CTRL,PCI_MASTER,START) |
							FINIT(SYSTEM_CTRL,PCI_RETRY,ENABLE) |
							FINIT(SYSTEM_CTRL,PCI_BURST,ENABLE) |
							FINIT(SYSTEM_CTRL,PCI_BURST_READ,ENABLE) |
							FINIT(SYSTEM_CTRL,PCI_SUBSYS_LOCK,ENABLE) |
							FINIT(SYSTEM_CTRL,LATENCY_TIMER,ENABLE)
							);
	pci_set_master(pdev);

	bus->mem_addr = pci_resource_start(pdev, 0);
	bus->mem_limit = pci_resource_len(pdev, 0);

	err = pci_request_region(pdev, 0, DRIVER_NAME);
	if ( err <0 ) {
		printk (KERN_ERR DRIVER_NAME " cannot reserve FRAME BUFFER region, aborting\n");
		goto unmap_mmio;
	}

	bus->mem = pci_iomap(pdev, 0, bus->mem_limit);
	if (bus->mem == NULL) {
		printk (KERN_ERR DRIVER_NAME " cannot remap FRAME BUFFER, aborting\n");
		err = -EIO;
		goto release_mem;
	}
	memset_io(bus->mem,0,bus->mem_limit);
	bus->irq = pdev->irq;

	pci_set_drvdata (pdev, bus);

	if ((err = sm5xx_bus_init(bus)) == 0)
	 	return 0;

	iounmap(bus->mem);
release_mem:
	pci_release_region(pdev, 0);
unmap_mmio:
	iounmap(bus->regbase);
release_mmio:
	pci_release_region(pdev, 1);
err_free_bus:
	sm5xx_bus_free(bus);
out:
	pci_disable_device(pdev);
	return err;
}

static void __devexit sm5xx_remove(struct pci_dev *dev)
{
}

static struct pci_device_id sm5xx_pci_table[] = {
	{ 0x126f, 0x501, PCI_ANY_ID, PCI_ANY_ID, 0x380, 0, 0 },
	{ 0, }
}

MODULE_DEVICE_TABLE(pci,sm5xx_pci_table);

static struct pci_driver sm5xx_device_driver = {
	.name		= "SM501 (VoyagerGX)",
	.probe		= sm5xx_probe,
	.remove		= __devexit_p(sm5xx_remove),
/*
	.suspend	= sm5xx_suspend,
	.resume		= sm5xx_resume,
*/
	.id_table	= sm5xx_pci_table
};


int __init sm5xx_init(void)
{
	int rtn = bus_register(&sm5xx_bus_type);
	if (!rtn)
		rtn = pci_register_driver(&sm5xx_device_driver);
	return rtn;
}

void __exit sm5xx_exit(void)
{
	pci_unregister_driver(&sm5xx_device_driver);
	bus_unregister(&sm5xx_bus_type);
}

#ifndef MODULE
 subsys_initcall(sm5xx_init);
#else
 module_init(sm5xx_init);
 module_exit(sm5xx_exit);
#endif

MODULE_AUTHOR("Andrey Volkov");
MODULE_DESCRIPTION("SM5xx PCI device driver");
MODULE_LICENSE("GPL");
