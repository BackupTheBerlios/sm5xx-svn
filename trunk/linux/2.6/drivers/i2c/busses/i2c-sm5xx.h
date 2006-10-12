/*
 * drivers/i2c/busses/i2c-sm5xx.h
 *
 * DESCRIPTION:
 *  Definitions of SM5xx I2C onboard adapter.
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
 */

#ifndef __SM501_I2C_H__
#define __SM501_I2C_H__

#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/i2c-id.h>
#include <linux/i2c-algo-bit.h>
#include <mfd/sm5xx/sm5xx_gpio_regs.h>

struct sm5xx_i2c_chan {
	struct i2c_adapter adapter;
	struct i2c_algo_bit_data algo;
};

/*
  This functions will implemented when ERRATA AA-12
  (Cannot program hardware I2C) will be fixed.

  void sm5xx_create_i2c_bus(struct sm5xx_par *par);
  void sm5xx_delete_i2c_bus(struct sm5xx_par *par);
*/
int sm5xx_probe_i2c_connector(struct sm5xx_dev *i2c_dev, u8 **out_edid);

#endif /* __SM501_I2C_H__ */

