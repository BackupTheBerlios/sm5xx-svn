/*
 * mfd/sm5xx/sm5xx_ci_regs.h
 *
 * DESCRIPTION:
 *  Definitions of the Command List Interpreter registers
 *
 * AUTHOR:
 *  Andrey Volkov
 *
 * COPYRIGHT:
 *  2005, Varma Electronics Oy
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

#ifndef __SM5XX_CI_REGS_H__
#define __SM5XX_CI_REGS_H__

#define CONFIG_SM5XX_FORCE_SLAVE	1

#define CMD_INTPR_CTRL                                  0x000018
#define CMD_INTPR_CTRL_STATUS                           31:31
#define CMD_INTPR_CTRL_STATUS_STOPPED                   0
#define CMD_INTPR_CTRL_STATUS_RUNNING                   1
#define CMD_INTPR_CTRL_EXT                              27:27
#define CMD_INTPR_CTRL_EXT_LOCAL                        0
#define CMD_INTPR_CTRL_EXT_EXTERNAL                     1
#define CMD_INTPR_CTRL_ADDRESS                          26:0

#define CMD_INTPR_CONDITIONS                            0x00001C

#define CMD_INTPR_RETURN                                0x000020
#define CMD_INTPR_RETURN_EXT                            27:27
#define CMD_INTPR_RETURN_EXT_LOCAL                      0
#define CMD_INTPR_RETURN_EXT_EXTERNAL                   1
#define CMD_INTPR_RETURN_CS                             26:26
#define CMD_INTPR_RETURN_CS_0                           0
#define CMD_INTPR_RETURN_CS_1                           1
#define CMD_INTPR_RETURN_ADDRESS                        25:0

#define CMD_INTPR_STATUS                                0x000024
#define CMD_INTPR_STATUS_2D_MEMORY_FIFO                 20:20
#define CMD_INTPR_STATUS_2D_MEMORY_FIFO_NOT_EMPTY       0
#define CMD_INTPR_STATUS_2D_MEMORY_FIFO_EMPTY           1
#define CMD_INTPR_STATUS_COMMAND_FIFO                   19:19
#define CMD_INTPR_STATUS_COMMAND_FIFO_NOT_EMPTY         0
#define CMD_INTPR_STATUS_COMMAND_FIFO_EMPTY             1
#define CMD_INTPR_STATUS_CSC_STATUS                     18:18
#define CMD_INTPR_STATUS_CSC_STATUS_IDLE                0
#define CMD_INTPR_STATUS_CSC_STATUS_BUSY                1
#define CMD_INTPR_STATUS_MEMORY_DMA                     17:17
#define CMD_INTPR_STATUS_MEMORY_DMA_IDLE                0
#define CMD_INTPR_STATUS_MEMORY_DMA_BUSY                1
#define CMD_INTPR_STATUS_CRT_STATUS                     16:16
#define CMD_INTPR_STATUS_CRT_STATUS_CURRENT             0
#define CMD_INTPR_STATUS_CRT_STATUS_PENDING             1
#define CMD_INTPR_STATUS_CURRENT_FIELD                  15:15
#define CMD_INTPR_STATUS_CURRENT_FIELD_ODD              0
#define CMD_INTPR_STATUS_CURRENT_FIELD_EVEN             1
#define CMD_INTPR_STATUS_VIDEO_STATUS                   14:14
#define CMD_INTPR_STATUS_VIDEO_STATUS_CURRENT           0
#define CMD_INTPR_STATUS_VIDEO_STATUS_PENDING           1
#define CMD_INTPR_STATUS_PANEL_STATUS                   13:13
#define CMD_INTPR_STATUS_PANEL_STATUS_CURRENT           0
#define CMD_INTPR_STATUS_PANEL_STATUS_PENDING           1
#define CMD_INTPR_STATUS_CRT_SYNC                       12:12
#define CMD_INTPR_STATUS_CRT_SYNC_INACTIVE              0
#define CMD_INTPR_STATUS_CRT_SYNC_ACTIVE                1
#define CMD_INTPR_STATUS_PANEL_SYNC                     11:11
#define CMD_INTPR_STATUS_PANEL_SYNC_INACTIVE            0
#define CMD_INTPR_STATUS_PANEL_SYNC_ACTIVE              1
#define CMD_INTPR_STATUS_2D_SETUP                       2:2
#define CMD_INTPR_STATUS_2D_SETUP_IDLE                  0
#define CMD_INTPR_STATUS_2D_SETUP_BUSY                  1
#define CMD_INTPR_STATUS_2D_FIFO                        1:1
#define CMD_INTPR_STATUS_2D_FIFO_NOT_EMPTY              0
#define CMD_INTPR_STATUS_2D_FIFO_EMPTY                  1
#define CMD_INTPR_STATUS_2D_ENGINE                      0:0
#define CMD_INTPR_STATUS_2D_ENGINE_IDLE                 0
#define CMD_INTPR_STATUS_2D_ENGINE_BUSY                 1

/*#if (defined(CONFIG_SM5XX_MMIO_BUSMASTER) || \
	 defined(CONFIG_SM5XX_PCI)) && !defined(CONFIG_SM5XX_FORCE_SLAVE)
*/
#if 0
#define	 CI_MEMORY_TYPE		FINIT(CMD_INTPR_CTRL,EXT,EXTERNAL)
#else
#define	 CI_MEMORY_TYPE		FINIT(CMD_INTPR_CTRL,EXT,LOCAL)
#endif

#endif /* __SM5XX_CI_REGS_H__ */

