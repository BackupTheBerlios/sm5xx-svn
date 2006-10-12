/*
 * mfd/sm5xx/sm5xx_gpio_regs.h
 *
 * DESCRIPTION:
 *  Definitions of the GPIO/I2C subsystems registers
 *
 * AUTHOR:
 *  Andrey Volkov
 *
 * COPYRIGHT:
 *  2005-2006, Varma Electronics Oy
 *
 *  Based on voyager.h v0.03 which is
 *     Copyright (C) 2004 Silicon Motion, Inc.
 *     Ge Wang, gewang@siliconmotion.com
 *
 * LICENCE:
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive
 *  for more details.
 *
 */
#ifndef __SM5XX_GPIO_REGS_H__
#define __SM5XX_GPIO_REGS_H__

// GPIO Control
#define GPIO_DATA_LOW						0x010000
#define GPIO_DATA_HIGH						0x010004
#define GPIO_DIR_LOW						0x010008
#define GPIO_DIR_HIGH						0x01000C
#define GPIO_INT_SETUP						0x010010
#define GPIO_INT_STATUS						0x010014
#define GPIO_INT_RESET						0x010014

#endif /* __SM5XX_GPIO_REGS_H__ */

