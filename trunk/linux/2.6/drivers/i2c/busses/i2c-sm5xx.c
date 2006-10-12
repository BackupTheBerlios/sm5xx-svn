/*
 * drivers/i2c/busses/i2c-sm5xx.c
 *
 * DESCRIPTION:
 *    I2C bus driver for SM5xx.
 *
 * AUTHOR:
 *  Andrey Volkov <avolkov@varma-el.com>
 *
 * COPYRIGHT:
 *  2005-2006, Varma Electronics Oy
 *
 * LICENCE:
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive
 *  for more details.
 *
 */

#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/circ_buf.h>

#include <asm/io.h>

#include <mfd/sm5xx/sm5xx.h>
#include <video/edid.h>
#include "i2c-sm5xx.h"
#include "../../video/edid.h"

/* delays */
#define CYCLE_DELAY	40
#define TIMEOUT		(HZ / 2)

#define sm5xx_read32(regindex)	_sm5xx_read32(regindex, bus)
#define sm5xx_write32(regindex,val)	_sm5xx_write32(regindex, (val), bus)
#define DRIVER_NAME	"i2c-sm5xx"

/* bit locations in the register */
#undef I2C_SCL_PIN
#undef I2C_SDA_PIN
#undef I2C_SCL_DIR_OUT
#undef I2C_SDA_DIR_OUT

#define I2C_SCL_PIN		(1<<14)
#define I2C_SCL_DIR_OUT	(1<<14)
#define I2C_SDA_PIN		(1<<15)
#define I2C_SDA_DIR_OUT	(1<<15)

static void sm5xx_gpio_setscl(void *data, int val)
{
	struct sm5xx_bus *bus = (struct sm5xx_bus *)data;

	volatile u32 d = sm5xx_read32(GPIO_DIR_HIGH);
	if(val)
		d &= ~I2C_SCL_DIR_OUT;		 /* clock pin now IN */
	else   {
		volatile u32 r = sm5xx_read32(GPIO_DATA_HIGH);
		r &= (~I2C_SCL_PIN);		 	/* clock pin now LOW */
		sm5xx_write32(GPIO_DATA_HIGH,r);
		d |= I2C_SCL_DIR_OUT;		 	/* clock pin now OUT */
	}
	sm5xx_write32(GPIO_DIR_HIGH,d);
}

static int sm5xx_gpio_getscl(void *data)
{
	struct sm5xx_bus *bus = (struct sm5xx_bus *)data;
	return ((sm5xx_read32(GPIO_DATA_HIGH)&I2C_SCL_PIN) != 0);
}

/*----------------------*/
static void sm5xx_gpio_setsda(void *data, int val)
{
	struct sm5xx_bus *bus = (struct sm5xx_bus *)data;

	volatile u32 d = sm5xx_read32(GPIO_DIR_HIGH);
	if(val)
		d &= ~I2C_SDA_DIR_OUT;		 /* sda pin now IN */
	else   {
		volatile u32 r = sm5xx_read32(GPIO_DATA_HIGH);
		r &= (~I2C_SDA_PIN);		 	/* sda pin now LOW */
		sm5xx_write32(GPIO_DATA_HIGH,r);
		d |= I2C_SDA_DIR_OUT;		 	/* sda pin now OUT */
	}
	sm5xx_write32(GPIO_DIR_HIGH,d);
}

static int sm5xx_gpio_getsda(void *data)
{
	struct sm5xx_bus *bus = (struct sm5xx_bus *)data;
	return ((sm5xx_read32(GPIO_DATA_HIGH) & I2C_SDA_PIN) !=0 );
}

int sm5xx_probe_i2c_connector(struct sm5xx_dev *dev, u8 **out_edid)
{
   struct sm5xx_i2c_chan *chan = dev_get_drvdata(&dev->dev);
   u8 start = 0x0;
   struct i2c_msg msgs[] = {
           {
            .addr = 0x50,
            .len = 1,
            .buf = &start,
            }, {
                .addr = 0x50,
                .flags = I2C_M_RD,
                .len = EDID_LENGTH,
                },
   };
   u8 *buf;

   buf = kmalloc(EDID_LENGTH, GFP_KERNEL);
   if (!buf) {
           printk(KERN_ERR "Out of memory!\n");
           return -ENOMEM;
   }
   msgs[1].buf = buf;

   *out_edid = NULL;
   if ( i2c_transfer(&chan->adapter, msgs, 2) == 2) {
           *out_edid = buf;
           return 0;
   }
   printk(KERN_WARNING "Unable to read EDID block.\n");
   kfree(buf);
   return -EIO;
}

int sm5xx_i2c_probe(struct sm5xx_dev *dev)
{
	struct sm5xx_bus *bus = to_sm5xx_bus( dev );
	struct sm5xx_i2c_chan *chan = NULL;
	int ret = 0;

	chan = kmalloc(sizeof(*chan),GFP_KERNEL);
	if(!chan)
		return -ENOMEM;
	memset(chan,0,sizeof(*chan));

	strncpy(chan->adapter.name, DRIVER_NAME, I2C_NAME_SIZE);
	chan->adapter.dev.parent = &dev->dev;
	chan->adapter.owner = THIS_MODULE;
	chan->adapter.id = I2C_HW_B_SM501;

	chan->algo.setscl	= sm5xx_gpio_setscl;
	chan->algo.getscl	= sm5xx_gpio_getscl;
	chan->algo.setsda	= sm5xx_gpio_setsda;
	chan->algo.getsda	= sm5xx_gpio_getsda;
	chan->algo.udelay	= CYCLE_DELAY;
	chan->algo.mdelay	= CYCLE_DELAY;
	chan->algo.timeout	= TIMEOUT;
	chan->algo.data		= bus;
	chan->adapter.algo_data  = &chan->algo;

	dev_set_drvdata(&dev->dev,chan);

	sm5xx_enable_subsystem(bus,
						   FINIT(POWER_MODE_GATE,GPIO_PWM_I2C,ENABLE),
						   FINIT(POWER_MODE_GATE,GPIO_PWM_I2C,ENABLE));

	/* Raise SCL and SDA */
	sm5xx_write32(GPIO_MUX_HIGH, sm5xx_read32(GPIO_MUX_HIGH) & ~(I2C_SCL_PIN | I2C_SDA_PIN) );
	sm5xx_write32(GPIO_DATA_HIGH,sm5xx_read32(GPIO_DATA_HIGH) | (I2C_SCL_PIN | I2C_SDA_PIN) );
	sm5xx_write32(GPIO_DIR_HIGH,
				  sm5xx_read32(GPIO_DIR_HIGH) & (~(I2C_SDA_DIR_OUT | I2C_SCL_DIR_OUT)) );
	udelay(20);

	ret = i2c_bit_add_bus(&chan->adapter);
	if(ret==0)
		printk(DRIVER_NAME " probe for SM5xx i2c done\n");
	else
	   dev_set_drvdata(&dev->dev, 0);

	return ret;

}

void sm5xx_i2c_remove(struct sm5xx_dev *dev)
{
	struct sm5xx_i2c_chan *chan = dev_get_drvdata(&dev->dev);
	dev_set_drvdata(&dev->dev, 0);
	i2c_bit_del_bus(&chan->adapter);
	kfree(chan);
}

static struct sm5xx_driver sm5xx_i2c_driver = {
	.drv  = {
		.name	= DRIVER_NAME,
		.bus	= &sm5xx_bus_type,
	},
	.devid		= SM5XX_DEVID_I2C,
	.probe		= sm5xx_i2c_probe,
	.remove		= __devexit_p(sm5xx_i2c_remove),
/*	.suspend	= sm5xx_i2c_suspend,
	.resume		= sm5xx_i2c_resume,
*/
};

int __init sm5xx_i2c_init(void)
{
	return sm5xx_driver_register(&sm5xx_i2c_driver);
}

void __exit sm5xx_i2c_exit(void)
{
	sm5xx_driver_unregister(&sm5xx_i2c_driver);
}

module_init(sm5xx_i2c_init);
module_exit(sm5xx_i2c_exit);

MODULE_AUTHOR("Andrey Volkov");
MODULE_DESCRIPTION("SM5xx I2C driver");
MODULE_LICENSE("GPL");

EXPORT_SYMBOL(sm5xx_probe_i2c_connector);
