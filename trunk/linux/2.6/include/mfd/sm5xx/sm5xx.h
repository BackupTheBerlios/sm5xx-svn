/*
 *  mfd/sm5xx/sm5xx.h
 *
 * DESCRIPTION:
 *  Definitions chip core registers,
 *  helpers, global functions and variables
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

#ifndef __SM5XX_H__
#define __SM5XX_H__

#define SM5XX_ALIGN						0x10
#define SM5XX_MIN_ALLOC_SIZE 			SM5XX_ALIGN
#define SM5XX_ALIGN_MASK				(SM5XX_ALIGN-1)
#define SM5XX_NR_INTS          			32

#define SYSTEM_CTRL                                     0x000000
#define SYSTEM_CTRL_DPMS                                31:30
#define SYSTEM_CTRL_DPMS_VPHP                           0
#define SYSTEM_CTRL_DPMS_VPHN                           1
#define SYSTEM_CTRL_DPMS_VNHP                           2
#define SYSTEM_CTRL_DPMS_VNHN                           3
#define SYSTEM_CTRL_PCI_BURST                           29:29
#define SYSTEM_CTRL_PCI_BURST_DISABLE                   0
#define SYSTEM_CTRL_PCI_BURST_ENABLE                    1
#define SYSTEM_CTRL_CSC_STATUS                          28:28
#define SYSTEM_CTRL_CSC_STATUS_IDLE                     0
#define SYSTEM_CTRL_CSC_STATUS_BUSY                     1
#define SYSTEM_CTRL_PCI_MASTER                          25:25
#define SYSTEM_CTRL_PCI_MASTER_STOP                     0
#define SYSTEM_CTRL_PCI_MASTER_START                    1
#define SYSTEM_CTRL_LATENCY_TIMER                       24:24
#define SYSTEM_CTRL_LATENCY_TIMER_ENABLE                0
#define SYSTEM_CTRL_LATENCY_TIMER_DISABLE               1
#define SYSTEM_CTRL_PANEL_STATUS                        23:23
#define SYSTEM_CTRL_PANEL_STATUS_CURRENT                0
#define SYSTEM_CTRL_PANEL_STATUS_PENDING                1
#define SYSTEM_CTRL_VIDEO_STATUS                        22:22
#define SYSTEM_CTRL_VIDEO_STATUS_CURRENT                0
#define SYSTEM_CTRL_VIDEO_STATUS_PENDING                1
#define SYSTEM_CTRL_DE_FIFO                             20:20
#define SYSTEM_CTRL_DE_FIFO_NOT_EMPTY                   0
#define SYSTEM_CTRL_DE_FIFO_EMPTY                       1
#define SYSTEM_CTRL_DE_STATUS                           19:19
#define SYSTEM_CTRL_DE_STATUS_IDLE                      0
#define SYSTEM_CTRL_DE_STATUS_BUSY                      1
#define SYSTEM_CTRL_CRT_STATUS                          17:17
#define SYSTEM_CTRL_CRT_STATUS_CURRENT                  0
#define SYSTEM_CTRL_CRT_STATUS_PENDING                  1
#define SYSTEM_CTRL_ZVPORT                              16:16
#define SYSTEM_CTRL_ZVPORT_0                            0
#define SYSTEM_CTRL_ZVPORT_1                            1
#define SYSTEM_CTRL_PCI_BURST_READ                      15:15
#define SYSTEM_CTRL_PCI_BURST_READ_DISABLE              0
#define SYSTEM_CTRL_PCI_BURST_READ_ENABLE               1
#define SYSTEM_CTRL_DE_ABORT                            13:12
#define SYSTEM_CTRL_DE_ABORT_NORMAL                     0
#define SYSTEM_CTRL_DE_ABORT_2D_ABORT                   3
#define SYSTEM_CTRL_PCI_SUBSYS_LOCK                     11:11
#define SYSTEM_CTRL_PCI_SUBSYS_LOCK_DISABLE             0
#define SYSTEM_CTRL_PCI_SUBSYS_LOCK_ENABLE              1
#define SYSTEM_CTRL_PCI_RETRY                           7:7
#define SYSTEM_CTRL_PCI_RETRY_ENABLE                    0
#define SYSTEM_CTRL_PCI_RETRY_DISABLE                   1
#define SYSTEM_CTRL_PCI_CLOCK_RUN                       6:6
#define SYSTEM_CTRL_PCI_CLOCK_RUN_DISABLE               0
#define SYSTEM_CTRL_PCI_CLOCK_RUN_ENABLE                1
#define SYSTEM_CTRL_PCI_SLAVE_BURST_READ_SIZE           5:4
#define SYSTEM_CTRL_PCI_SLAVE_BURST_READ_SIZE_1         0
#define SYSTEM_CTRL_PCI_SLAVE_BURST_READ_SIZE_2         1
#define SYSTEM_CTRL_PCI_SLAVE_BURST_READ_SIZE_4         2
#define SYSTEM_CTRL_PCI_SLAVE_BURST_READ_SIZE_8         3
#define SYSTEM_CTRL_CRT_TRISTATE                        2:2
#define SYSTEM_CTRL_CRT_TRISTATE_DISABLE                0
#define SYSTEM_CTRL_CRT_TRISTATE_ENABLE                 1
#define SYSTEM_CTRL_INTMEM_TRISTATE                     1:1
#define SYSTEM_CTRL_INTMEM_TRISTATE_DISABLE             0
#define SYSTEM_CTRL_INTMEM_TRISTATE_ENABLE              1
#define SYSTEM_CTRL_PANEL_TRISTATE                      0:0
#define SYSTEM_CTRL_PANEL_TRISTATE_DISABLE              0
#define SYSTEM_CTRL_PANEL_TRISTATE_ENABLE               1

#define MISC_CTRL                                       0x000004
#define MISC_CTRL_PCI_PAD                               31:30
#define MISC_CTRL_PCI_PAD_24MA                          0
#define MISC_CTRL_PCI_PAD_12MA                          1
#define MISC_CTRL_PCI_PAD_8MA                           2
#define MISC_CTRL_48_SELECT                             29:28
#define MISC_CTRL_48_SELECT_CRYSTAL                     0
#define MISC_CTRL_48_SELECT_CPU_96                      2
#define MISC_CTRL_48_SELECT_CPU_48                      3
#define MISC_CTRL_UART1_SELECT                          27:27
#define MISC_CTRL_UART1_SELECT_UART                     0
#define MISC_CTRL_UART1_SELECT_SSP                      1
#define MISC_CTRL_8051_LATCH                            26:26
#define MISC_CTRL_8051_LATCH_DISABLE                    0
#define MISC_CTRL_8051_LATCH_ENABLE                     1
#define MISC_CTRL_FPDATA                                25:25
#define MISC_CTRL_FPDATA_18                             0
#define MISC_CTRL_FPDATA_24                             1
#define MISC_CTRL_CRYSTAL                               24:24
#define MISC_CTRL_CRYSTAL_24                            0
#define MISC_CTRL_CRYSTAL_12                            1
#define MISC_CTRL_DRAM_REFRESH                          22:21
#define MISC_CTRL_DRAM_REFRESH_8                        0
#define MISC_CTRL_DRAM_REFRESH_16                       1
#define MISC_CTRL_DRAM_REFRESH_32                       2
#define MISC_CTRL_DRAM_REFRESH_64                       3
#define MISC_CTRL_BUS_HOLD                              20:18
#define MISC_CTRL_BUS_HOLD_FIFO_EMPTY                   0
#define MISC_CTRL_BUS_HOLD_8                            1
#define MISC_CTRL_BUS_HOLD_16                           2
#define MISC_CTRL_BUS_HOLD_24                           3
#define MISC_CTRL_BUS_HOLD_32                           4
#define MISC_CTRL_HITACHI_READY                         17:17
#define MISC_CTRL_HITACHI_READY_NEGATIVE                0
#define MISC_CTRL_HITACHI_READY_POSITIVE                1
#define MISC_CTRL_INTERRUPT                             16:16
#define MISC_CTRL_INTERRUPT_NORMAL                      0
#define MISC_CTRL_INTERRUPT_INVERT                      1
#define MISC_CTRL_PLL_CLOCK_COUNT                       15:15
#define MISC_CTRL_PLL_CLOCK_COUNT_DISABLE               0
#define MISC_CTRL_PLL_CLOCK_COUNT_ENABLE                1
#define MISC_CTRL_DAC_BAND_GAP                          14:13
#define MISC_CTRL_DAC_POWER                             12:12
#define MISC_CTRL_DAC_POWER_ENABLE                      0
#define MISC_CTRL_DAC_POWER_DISABLE                     1
#define MISC_CTRL_USB_SLAVE_CONTROLLER                  11:11
#define MISC_CTRL_USB_SLAVE_CONTROLLER_CPU              0
#define MISC_CTRL_USB_SLAVE_CONTROLLER_8051             1
#define MISC_CTRL_BURST_LENGTH                          10:10
#define MISC_CTRL_BURST_LENGTH_8                        0
#define MISC_CTRL_BURST_LENGTH_1                        1
#define MISC_CTRL_USB_SELECT                            9:9
#define MISC_CTRL_USB_SELECT_MASTER                     0
#define MISC_CTRL_USB_SELECT_SLAVE                      1
#define MISC_CTRL_LOOPBACK                              8:8
#define MISC_CTRL_LOOPBACK_NORMAL                       0
#define MISC_CTRL_LOOPBACK_USB_HOST                     1
#define MISC_CTRL_CLOCK_DIVIDER_RESET                   7:7
#define MISC_CTRL_CLOCK_DIVIDER_RESET_ENABLE            0
#define MISC_CTRL_CLOCK_DIVIDER_RESET_DISABLE           1
#define MISC_CTRL_TEST_MODE                             6:5
#define MISC_CTRL_TEST_MODE_NORMAL                      0
#define MISC_CTRL_TEST_MODE_DEBUGGING                   1
#define MISC_CTRL_TEST_MODE_NAND                        2
#define MISC_CTRL_TEST_MODE_MEMORY                      3
#define MISC_CTRL_NEC_MMIO                              4:4
#define MISC_CTRL_NEC_MMIO_30                           0
#define MISC_CTRL_NEC_MMIO_62                           1
#define MISC_CTRL_CLOCK                                 3:3
#define MISC_CTRL_CLOCK_PLL                             0
#define MISC_CTRL_CLOCK_TEST                            1
#define MISC_CTRL_HOST_BUS                              2:0
#define MISC_CTRL_HOST_BUS_HITACHI                      0
#define MISC_CTRL_HOST_BUS_PCI                          1
#define MISC_CTRL_HOST_BUS_XSCALE                       2
#define MISC_CTRL_HOST_BUS_STRONGARM                    4
#define MISC_CTRL_HOST_BUS_NEC                          6

#define GPIO_MUX_LOW                                    0x000008
#define GPIO_MUX_LOW_31                                 31:31
#define GPIO_MUX_LOW_31_GPIO                            0
#define GPIO_MUX_LOW_31_PWM                             1
#define GPIO_MUX_LOW_30                                 30:30
#define GPIO_MUX_LOW_30_GPIO                            0
#define GPIO_MUX_LOW_30_PWM                             1
#define GPIO_MUX_LOW_29                                 29:29
#define GPIO_MUX_LOW_29_GPIO                            0
#define GPIO_MUX_LOW_29_PWM                             1
#define GPIO_MUX_LOW_28                                 28:28
#define GPIO_MUX_LOW_28_GPIO                            0
#define GPIO_MUX_LOW_28_AC97_I2S                        1
#define GPIO_MUX_LOW_27                                 27:27
#define GPIO_MUX_LOW_27_GPIO                            0
#define GPIO_MUX_LOW_27_AC97_I2S                        1
#define GPIO_MUX_LOW_26                                 26:26
#define GPIO_MUX_LOW_26_GPIO                            0
#define GPIO_MUX_LOW_26_AC97_I2S                        1
#define GPIO_MUX_LOW_25                                 25:25
#define GPIO_MUX_LOW_25_GPIO                            0
#define GPIO_MUX_LOW_25_AC97_I2S                        1
#define GPIO_MUX_LOW_24                                 24:24
#define GPIO_MUX_LOW_24_GPIO                            0
#define GPIO_MUX_LOW_24_AC97                            1
#define GPIO_MUX_LOW_23                                 23:23
#define GPIO_MUX_LOW_23_GPIO                            0
#define GPIO_MUX_LOW_23_ZVPORT                          1
#define GPIO_MUX_LOW_22                                 22:22
#define GPIO_MUX_LOW_22_GPIO                            0
#define GPIO_MUX_LOW_22_ZVPORT                          1
#define GPIO_MUX_LOW_21                                 21:21
#define GPIO_MUX_LOW_21_GPIO                            0
#define GPIO_MUX_LOW_21_ZVPORT                          1
#define GPIO_MUX_LOW_20                                 20:20
#define GPIO_MUX_LOW_20_GPIO                            0
#define GPIO_MUX_LOW_20_ZVPORT                          1
#define GPIO_MUX_LOW_19                                 19:19
#define GPIO_MUX_LOW_19_GPIO                            0
#define GPIO_MUX_LOW_19_ZVPORT                          1
#define GPIO_MUX_LOW_18                                 18:18
#define GPIO_MUX_LOW_18_GPIO                            0
#define GPIO_MUX_LOW_18_ZVPORT                          1
#define GPIO_MUX_LOW_17                                 17:17
#define GPIO_MUX_LOW_17_GPIO                            0
#define GPIO_MUX_LOW_17_ZVPORT                          1
#define GPIO_MUX_LOW_16                                 16:16
#define GPIO_MUX_LOW_16_GPIO                            0
#define GPIO_MUX_LOW_16_ZVPORT                          1
#define GPIO_MUX_LOW_15                                 15:15
#define GPIO_MUX_LOW_15_GPIO                            0
#define GPIO_MUX_LOW_15_8051                            1
#define GPIO_MUX_LOW_14                                 14:14
#define GPIO_MUX_LOW_14_GPIO                            0
#define GPIO_MUX_LOW_14_8051                            1
#define GPIO_MUX_LOW_13                                 13:13
#define GPIO_MUX_LOW_13_GPIO                            0
#define GPIO_MUX_LOW_13_8051                            1
#define GPIO_MUX_LOW_12                                 12:12
#define GPIO_MUX_LOW_12_GPIO                            0
#define GPIO_MUX_LOW_12_8051                            1
#define GPIO_MUX_LOW_11                                 11:11
#define GPIO_MUX_LOW_11_GPIO                            0
#define GPIO_MUX_LOW_11_8051                            1
#define GPIO_MUX_LOW_10                                 10:10
#define GPIO_MUX_LOW_10_GPIO                            0
#define GPIO_MUX_LOW_10_8051                            1
#define GPIO_MUX_LOW_9                                  9:9
#define GPIO_MUX_LOW_9_GPIO                             0
#define GPIO_MUX_LOW_9_8051                             1
#define GPIO_MUX_LOW_8                                  8:8
#define GPIO_MUX_LOW_8_GPIO                             0
#define GPIO_MUX_LOW_8_8051                             1
#define GPIO_MUX_LOW_7                                  7:7
#define GPIO_MUX_LOW_7_GPIO                             0
#define GPIO_MUX_LOW_7_8051                             1
#define GPIO_MUX_LOW_6                                  6:6
#define GPIO_MUX_LOW_6_GPIO                             0
#define GPIO_MUX_LOW_6_8051                             1
#define GPIO_MUX_LOW_5                                  5:5
#define GPIO_MUX_LOW_5_GPIO                             0
#define GPIO_MUX_LOW_5_8051                             1
#define GPIO_MUX_LOW_4                                  4:4
#define GPIO_MUX_LOW_4_GPIO                             0
#define GPIO_MUX_LOW_4_8051                             1
#define GPIO_MUX_LOW_3                                  3:3
#define GPIO_MUX_LOW_3_GPIO                             0
#define GPIO_MUX_LOW_3_8051                             1
#define GPIO_MUX_LOW_2                                  2:2
#define GPIO_MUX_LOW_2_GPIO                             0
#define GPIO_MUX_LOW_2_8051                             1
#define GPIO_MUX_LOW_1                                  1:1
#define GPIO_MUX_LOW_1_GPIO                             0
#define GPIO_MUX_LOW_1_8051                             1
#define GPIO_MUX_LOW_0                                  0:0
#define GPIO_MUX_LOW_0_GPIO                             0
#define GPIO_MUX_LOW_0_8051                             1

#define GPIO_MUX_HIGH                                   0x00000C
#define GPIO_MUX_HIGH_63                                31:31
#define GPIO_MUX_HIGH_63_GPIO                           0
#define GPIO_MUX_HIGH_63_CRT_ZVPORT_FPDATA              1
#define GPIO_MUX_HIGH_62                                30:30
#define GPIO_MUX_HIGH_62_GPIO                           0
#define GPIO_MUX_HIGH_62_CRT_ZVPORT_FPDATA              1
#define GPIO_MUX_HIGH_61                                29:29
#define GPIO_MUX_HIGH_61_GPIO                           0
#define GPIO_MUX_HIGH_61_CRT_ZVPORT_FPDATA              1
#define GPIO_MUX_HIGH_60                                28:28
#define GPIO_MUX_HIGH_60_GPIO                           0
#define GPIO_MUX_HIGH_60_CRT_ZVPORT_FPDATA              1
#define GPIO_MUX_HIGH_59                                27:27
#define GPIO_MUX_HIGH_59_GPIO                           0
#define GPIO_MUX_HIGH_59_CRT_ZVPORT_FPDATA              1
#define GPIO_MUX_HIGH_58                                26:26
#define GPIO_MUX_HIGH_58_GPIO                           0
#define GPIO_MUX_HIGH_58_CRT_ZVPORT_FPDATA              1
#define GPIO_MUX_HIGH_57                                25:25
#define GPIO_MUX_HIGH_57_GPIO                           0
#define GPIO_MUX_HIGH_57_CRT_ZVPORT                     1
#define GPIO_MUX_HIGH_56                                24:24
#define GPIO_MUX_HIGH_56_GPIO                           0
#define GPIO_MUX_HIGH_56_CRT_ZVPORT                     1
#define GPIO_MUX_HIGH_55                                23:23
#define GPIO_MUX_HIGH_55_GPIO                           0
#define GPIO_MUX_HIGH_55_CRT                            1
#define GPIO_MUX_HIGH_47                                15:15
#define GPIO_MUX_HIGH_47_GPIO                           0
#define GPIO_MUX_HIGH_47_I2C                            1
#define GPIO_MUX_HIGH_46                                14:14
#define GPIO_MUX_HIGH_46_GPIO                           0
#define GPIO_MUX_HIGH_46_I2C                            1
#define GPIO_MUX_HIGH_45                                13:13
#define GPIO_MUX_HIGH_45_GPIO                           0
#define GPIO_MUX_HIGH_45_SSP1                           1
#define GPIO_MUX_HIGH_44                                12:12
#define GPIO_MUX_HIGH_44_GPIO                           0
#define GPIO_MUX_HIGH_44_UART1_SSP1                     1
#define GPIO_MUX_HIGH_43                                11:11
#define GPIO_MUX_HIGH_43_GPIO                           0
#define GPIO_MUX_HIGH_43_UART1_SSP1                     1
#define GPIO_MUX_HIGH_42                                10:10
#define GPIO_MUX_HIGH_42_GPIO                           0
#define GPIO_MUX_HIGH_42_UART1_SSP1                     1
#define GPIO_MUX_HIGH_41                                9:9
#define GPIO_MUX_HIGH_41_GPIO                           0
#define GPIO_MUX_HIGH_41_UART1_SSP1                     1
#define GPIO_MUX_HIGH_40                                8:8
#define GPIO_MUX_HIGH_40_GPIO                           0
#define GPIO_MUX_HIGH_40_UART0                          1
#define GPIO_MUX_HIGH_39                                7:7
#define GPIO_MUX_HIGH_39_GPIO                           0
#define GPIO_MUX_HIGH_39_UART0                          1
#define GPIO_MUX_HIGH_38                                6:6
#define GPIO_MUX_HIGH_38_GPIO                           0
#define GPIO_MUX_HIGH_38_UART0                          1
#define GPIO_MUX_HIGH_37                                5:5
#define GPIO_MUX_HIGH_37_GPIO                           0
#define GPIO_MUX_HIGH_37_UART0                          1
#define GPIO_MUX_HIGH_36                                4:4
#define GPIO_MUX_HIGH_36_GPIO                           0
#define GPIO_MUX_HIGH_36_SSP0                           1
#define GPIO_MUX_HIGH_35                                3:3
#define GPIO_MUX_HIGH_35_GPIO                           0
#define GPIO_MUX_HIGH_35_SSP0                           1
#define GPIO_MUX_HIGH_34                                2:2
#define GPIO_MUX_HIGH_34_GPIO                           0
#define GPIO_MUX_HIGH_34_SSP0                           1
#define GPIO_MUX_HIGH_33                                1:1
#define GPIO_MUX_HIGH_33_GPIO                           0
#define GPIO_MUX_HIGH_33_SSP0                           1
#define GPIO_MUX_HIGH_32                                0:0
#define GPIO_MUX_HIGH_32_GPIO                           0
#define GPIO_MUX_HIGH_32_SSP0                           1

#define DRAM_CTRL                                       0x000010
#define DRAM_CTRL_EMBEDDED                              31:31
#define DRAM_CTRL_EMBEDDED_ENABLE                       0
#define DRAM_CTRL_EMBEDDED_DISABLE                      1
#define DRAM_CTRL_CPU_BURST                             30:28
#define DRAM_CTRL_CPU_BURST_1                           0
#define DRAM_CTRL_CPU_BURST_2                           1
#define DRAM_CTRL_CPU_BURST_4                           2
#define DRAM_CTRL_CPU_BURST_8                           3
#define DRAM_CTRL_CPU_CAS_LATENCY                       27:27
#define DRAM_CTRL_CPU_CAS_LATENCY_2                     0
#define DRAM_CTRL_CPU_CAS_LATENCY_3                     1
#define DRAM_CTRL_CPU_SIZE                              26:24
#define DRAM_CTRL_CPU_SIZE_2                            0
#define DRAM_CTRL_CPU_SIZE_4                            1
#define DRAM_CTRL_CPU_SIZE_64                           4
#define DRAM_CTRL_CPU_SIZE_32                           5
#define DRAM_CTRL_CPU_SIZE_16                           6
#define DRAM_CTRL_CPU_SIZE_8                            7
#define DRAM_CTRL_CPU_COLUMN_SIZE                       23:22
#define DRAM_CTRL_CPU_COLUMN_SIZE_1024                  0
#define DRAM_CTRL_CPU_COLUMN_SIZE_512                   2
#define DRAM_CTRL_CPU_COLUMN_SIZE_256                   3
#define DRAM_CTRL_CPU_ACTIVE_PRECHARGE                  21:21
#define DRAM_CTRL_CPU_ACTIVE_PRECHARGE_6                0
#define DRAM_CTRL_CPU_ACTIVE_PRECHARGE_7                1
#define DRAM_CTRL_CPU_RESET                             20:20
#define DRAM_CTRL_CPU_RESET_ENABLE                      0
#define DRAM_CTRL_CPU_RESET_DISABLE                     1
#define DRAM_CTRL_CPU_BANKS                             19:19
#define DRAM_CTRL_CPU_BANKS_2                           0
#define DRAM_CTRL_CPU_BANKS_4                           1
#define DRAM_CTRL_CPU_WRITE_PRECHARGE                   18:18
#define DRAM_CTRL_CPU_WRITE_PRECHARGE_2                 0
#define DRAM_CTRL_CPU_WRITE_PRECHARGE_1                 1
#define DRAM_CTRL_BLOCK_WRITE                           17:17
#define DRAM_CTRL_BLOCK_WRITE_DISABLE                   0
#define DRAM_CTRL_BLOCK_WRITE_ENABLE                    1
#define DRAM_CTRL_REFRESH_COMMAND                       16:16
#define DRAM_CTRL_REFRESH_COMMAND_10                    0
#define DRAM_CTRL_REFRESH_COMMAND_12                    1
#define DRAM_CTRL_SIZE                                  15:13
#define DRAM_CTRL_SIZE_4                                0
#define DRAM_CTRL_SIZE_8                                1
#define DRAM_CTRL_SIZE_16                               2
#define DRAM_CTRL_SIZE_32                               3
#define DRAM_CTRL_SIZE_64                               4
#define DRAM_CTRL_SIZE_2                                5
#define DRAM_CTRL_COLUMN_SIZE                           12:11
#define DRAM_CTRL_COLUMN_SIZE_256                       0
#define DRAM_CTRL_COLUMN_SIZE_512                       2
#define DRAM_CTRL_COLUMN_SIZE_1024                      3
#define DRAM_CTRL_BLOCK_WRITE_TIME                      10:10
#define DRAM_CTRL_BLOCK_WRITE_TIME_1                    0
#define DRAM_CTRL_BLOCK_WRITE_TIME_2                    1
#define DRAM_CTRL_BLOCK_WRITE_PRECHARGE                 9:9
#define DRAM_CTRL_BLOCK_WRITE_PRECHARGE_4               0
#define DRAM_CTRL_BLOCK_WRITE_PRECHARGE_1               1
#define DRAM_CTRL_ACTIVE_PRECHARGE                      8:8
#define DRAM_CTRL_ACTIVE_PRECHARGE_6                    0
#define DRAM_CTRL_ACTIVE_PRECHARGE_7                    1
#define DRAM_CTRL_RESET                                 7:7
#define DRAM_CTRL_RESET_ENABLE                          0
#define DRAM_CTRL_RESET_DISABLE                         1
#define DRAM_CTRL_REMAIN_ACTIVE                         6:6
#define DRAM_CTRL_REMAIN_ACTIVE_ENABLE                  0
#define DRAM_CTRL_REMAIN_ACTIVE_DISABLE                 1
#define DRAM_CTRL_BANKS                                 1:1
#define DRAM_CTRL_BANKS_2                               0
#define DRAM_CTRL_BANKS_4                               1
#define DRAM_CTRL_WRITE_PRECHARGE                       0:0
#define DRAM_CTRL_WRITE_PRECHARGE_2                     0
#define DRAM_CTRL_WRITE_PRECHARGE_1                     1

#define ARBITRATION_CTRL                                0x000014
#define ARBITRATION_CTRL_CPUMEM                         29:29
#define ARBITRATION_CTRL_CPUMEM_FIXED                   0
#define ARBITRATION_CTRL_CPUMEM_ROTATE                  1
#define ARBITRATION_CTRL_INTMEM                         28:28
#define ARBITRATION_CTRL_INTMEM_FIXED                   0
#define ARBITRATION_CTRL_INTMEM_ROTATE                  1
#define ARBITRATION_CTRL_USB                            27:24
#define ARBITRATION_CTRL_USB_OFF                        0
#define ARBITRATION_CTRL_USB_PRIORITY_1                 1
#define ARBITRATION_CTRL_USB_PRIORITY_2                 2
#define ARBITRATION_CTRL_USB_PRIORITY_3                 3
#define ARBITRATION_CTRL_USB_PRIORITY_4                 4
#define ARBITRATION_CTRL_USB_PRIORITY_5                 5
#define ARBITRATION_CTRL_USB_PRIORITY_6                 6
#define ARBITRATION_CTRL_USB_PRIORITY_7                 7
#define ARBITRATION_CTRL_PANEL                          23:20
#define ARBITRATION_CTRL_PANEL_OFF                      0
#define ARBITRATION_CTRL_PANEL_PRIORITY_1               1
#define ARBITRATION_CTRL_PANEL_PRIORITY_2               2
#define ARBITRATION_CTRL_PANEL_PRIORITY_3               3
#define ARBITRATION_CTRL_PANEL_PRIORITY_4               4
#define ARBITRATION_CTRL_PANEL_PRIORITY_5               5
#define ARBITRATION_CTRL_PANEL_PRIORITY_6               6
#define ARBITRATION_CTRL_PANEL_PRIORITY_7               7
#define ARBITRATION_CTRL_ZVPORT                         19:16
#define ARBITRATION_CTRL_ZVPORT_OFF                     0
#define ARBITRATION_CTRL_ZVPORT_PRIORITY_1              1
#define ARBITRATION_CTRL_ZVPORT_PRIORITY_2              2
#define ARBITRATION_CTRL_ZVPORT_PRIORITY_3              3
#define ARBITRATION_CTRL_ZVPORT_PRIORITY_4              4
#define ARBITRATION_CTRL_ZVPORT_PRIORITY_5              5
#define ARBITRATION_CTRL_ZVPORT_PRIORITY_6              6
#define ARBITRATION_CTRL_ZVPORT_PRIORITY_7              7
#define ARBITRATION_CTRL_CMD_INTPR                      15:12
#define ARBITRATION_CTRL_CMD_INTPR_OFF                  0
#define ARBITRATION_CTRL_CMD_INTPR_PRIORITY_1           1
#define ARBITRATION_CTRL_CMD_INTPR_PRIORITY_2           2
#define ARBITRATION_CTRL_CMD_INTPR_PRIORITY_3           3
#define ARBITRATION_CTRL_CMD_INTPR_PRIORITY_4           4
#define ARBITRATION_CTRL_CMD_INTPR_PRIORITY_5           5
#define ARBITRATION_CTRL_CMD_INTPR_PRIORITY_6           6
#define ARBITRATION_CTRL_CMD_INTPR_PRIORITY_7           7
#define ARBITRATION_CTRL_DMA                            11:8
#define ARBITRATION_CTRL_DMA_OFF                        0
#define ARBITRATION_CTRL_DMA_PRIORITY_1                 1
#define ARBITRATION_CTRL_DMA_PRIORITY_2                 2
#define ARBITRATION_CTRL_DMA_PRIORITY_3                 3
#define ARBITRATION_CTRL_DMA_PRIORITY_4                 4
#define ARBITRATION_CTRL_DMA_PRIORITY_5                 5
#define ARBITRATION_CTRL_DMA_PRIORITY_6                 6
#define ARBITRATION_CTRL_DMA_PRIORITY_7                 7
#define ARBITRATION_CTRL_VIDEO                          7:4
#define ARBITRATION_CTRL_VIDEO_OFF                      0
#define ARBITRATION_CTRL_VIDEO_PRIORITY_1               1
#define ARBITRATION_CTRL_VIDEO_PRIORITY_2               2
#define ARBITRATION_CTRL_VIDEO_PRIORITY_3               3
#define ARBITRATION_CTRL_VIDEO_PRIORITY_4               4
#define ARBITRATION_CTRL_VIDEO_PRIORITY_5               5
#define ARBITRATION_CTRL_VIDEO_PRIORITY_6               6
#define ARBITRATION_CTRL_VIDEO_PRIORITY_7               7
#define ARBITRATION_CTRL_CRT                            3:0
#define ARBITRATION_CTRL_CRT_OFF                        0
#define ARBITRATION_CTRL_CRT_PRIORITY_1                 1
#define ARBITRATION_CTRL_CRT_PRIORITY_2                 2
#define ARBITRATION_CTRL_CRT_PRIORITY_3                 3
#define ARBITRATION_CTRL_CRT_PRIORITY_4                 4
#define ARBITRATION_CTRL_CRT_PRIORITY_5                 5
#define ARBITRATION_CTRL_CRT_PRIORITY_6                 6
#define ARBITRATION_CTRL_CRT_PRIORITY_7                 7

#define RAW_INT_STATUS                                  0x000028
#define RAW_INT_STATUS_USB_SLAVE_PLUG_IN                5:5
#define RAW_INT_STATUS_USB_SLAVE_PLUG_IN_INACTIVE       0
#define RAW_INT_STATUS_USB_SLAVE_PLUG_IN_ACTIVE         1
#define RAW_INT_STATUS_USB_SLAVE_PLUG_IN_CLEAR          1
#define RAW_INT_STATUS_ZVPORT                           4:4
#define RAW_INT_STATUS_ZVPORT_INACTIVE                  0
#define RAW_INT_STATUS_ZVPORT_ACTIVE                    1
#define RAW_INT_STATUS_ZVPORT_CLEAR                     1
#define RAW_INT_STATUS_CRT_VSYNC                        3:3
#define RAW_INT_STATUS_CRT_VSYNC_INACTIVE               0
#define RAW_INT_STATUS_CRT_VSYNC_ACTIVE                 1
#define RAW_INT_STATUS_CRT_VSYNC_CLEAR                  1
#define RAW_INT_STATUS_USB_SLAVE                        2:2
#define RAW_INT_STATUS_USB_SLAVE_INACTIVE               0
#define RAW_INT_STATUS_USB_SLAVE_ACTIVE                 1
#define RAW_INT_STATUS_USB_SLAVE_CLEAR                  1
#define RAW_INT_STATUS_PANEL_VSYNC                      1:1
#define RAW_INT_STATUS_PANEL_VSYNC_INACTIVE             0
#define RAW_INT_STATUS_PANEL_VSYNC_ACTIVE               1
#define RAW_INT_STATUS_PANEL_VSYNC_CLEAR                1
#define RAW_INT_STATUS_CMD_INTPR                        0:0
#define RAW_INT_STATUS_CMD_INTPR_INACTIVE               0
#define RAW_INT_STATUS_CMD_INTPR_ACTIVE                 1
#define RAW_INT_STATUS_CMD_INTPR_CLEAR                  1

#define INT_STATUS                                      0x00002C
#define INT_STATUS_USB_SLAVE_PLUG_IN                    31:31
#define INT_STATUS_USB_SLAVE_PLUG_IN_INACTIVE           0
#define INT_STATUS_USB_SLAVE_PLUG_IN_ACTIVE             1
#define INT_STATUS_GPIO54                               30:30
#define INT_STATUS_GPIO54_INACTIVE                      0
#define INT_STATUS_GPIO54_ACTIVE                        1
#define INT_STATUS_GPIO53                               29:29
#define INT_STATUS_GPIO53_INACTIVE                      0
#define INT_STATUS_GPIO53_ACTIVE                        1
#define INT_STATUS_GPIO52                               28:28
#define INT_STATUS_GPIO52_INACTIVE                      0
#define INT_STATUS_GPIO52_ACTIVE                        1
#define INT_STATUS_GPIO51                               27:27
#define INT_STATUS_GPIO51_INACTIVE                      0
#define INT_STATUS_GPIO51_ACTIVE                        1
#define INT_STATUS_GPIO50                               26:26
#define INT_STATUS_GPIO50_INACTIVE                      0
#define INT_STATUS_GPIO50_ACTIVE                        1
#define INT_STATUS_GPIO49                               25:25
#define INT_STATUS_GPIO49_INACTIVE                      0
#define INT_STATUS_GPIO49_ACTIVE                        1
#define INT_STATUS_GPIO48                               24:24
#define INT_STATUS_GPIO48_INACTIVE                      0
#define INT_STATUS_GPIO48_ACTIVE                        1
#define INT_STATUS_I2C                                  23:23
#define INT_STATUS_I2C_INACTIVE                         0
#define INT_STATUS_I2C_ACTIVE                           1
#define INT_STATUS_PWM                                  22:22
#define INT_STATUS_PWM_INACTIVE                         0
#define INT_STATUS_PWM_ACTIVE                           1
#define INT_STATUS_DMA                                  20:20
#define INT_STATUS_DMA_INACTIVE                         0
#define INT_STATUS_DMA_ACTIVE                           1
#define INT_STATUS_PCI                                  19:19
#define INT_STATUS_PCI_INACTIVE                         0
#define INT_STATUS_PCI_ACTIVE                           1
#define INT_STATUS_I2S                                  18:18
#define INT_STATUS_I2S_INACTIVE                         0
#define INT_STATUS_I2S_ACTIVE                           1
#define INT_STATUS_AC97                                 17:17
#define INT_STATUS_AC97_INACTIVE                        0
#define INT_STATUS_AC97_ACTIVE                          1
#define INT_STATUS_USB_SLAVE                            16:16
#define INT_STATUS_USB_SLAVE_INACTIVE                   0
#define INT_STATUS_USB_SLAVE_ACTIVE                     1
#define INT_STATUS_UART1                                13:13
#define INT_STATUS_UART1_INACTIVE                       0
#define INT_STATUS_UART1_ACTIVE                         1
#define INT_STATUS_UART0                                12:12
#define INT_STATUS_UART0_INACTIVE                       0
#define INT_STATUS_UART0_ACTIVE                         1
#define INT_STATUS_CRT_VSYNC                            11:11
#define INT_STATUS_CRT_VSYNC_INACTIVE                   0
#define INT_STATUS_CRT_VSYNC_ACTIVE                     1
#define INT_STATUS_8051                                 10:10
#define INT_STATUS_8051_INACTIVE                        0
#define INT_STATUS_8051_ACTIVE                          1
#define INT_STATUS_SSP1                                 9:9
#define INT_STATUS_SSP1_INACTIVE                        0
#define INT_STATUS_SSP1_ACTIVE                          1
#define INT_STATUS_SSP0                                 8:8
#define INT_STATUS_SSP0_INACTIVE                        0
#define INT_STATUS_SSP0_ACTIVE                          1
#define INT_STATUS_USB_HOST                             6:6
#define INT_STATUS_USB_HOST_INACTIVE                    0
#define INT_STATUS_USB_HOST_ACTIVE                      1
#define INT_STATUS_2D                                   3:3
#define INT_STATUS_2D_INACTIVE                          0
#define INT_STATUS_2D_ACTIVE                            1
#define INT_STATUS_ZVPORT                               2:2
#define INT_STATUS_ZVPORT_INACTIVE                      0
#define INT_STATUS_ZVPORT_ACTIVE                        1
#define INT_STATUS_PANEL_VSYNC                          1:1
#define INT_STATUS_PANEL_VSYNC_INACTIVE                 0
#define INT_STATUS_PANEL_VSYNC_ACTIVE                   1
#define INT_STATUS_CMD_INTPR                            0:0
#define INT_STATUS_CMD_INTPR_INACTIVE                   0
#define INT_STATUS_CMD_INTPR_ACTIVE                     1

#define INT_MASK                                        0x000030
#define INT_MASK_USB_SLAVE_PLUG_IN                      31:31
#define INT_MASK_USB_SLAVE_PLUG_IN_DISABLE              0
#define INT_MASK_USB_SLAVE_PLUG_IN_ENABLE               1
#define INT_MASK_GPIO54                                 30:30
#define INT_MASK_GPIO54_DISABLE                         0
#define INT_MASK_GPIO54_ENABLE                          1
#define INT_MASK_GPIO53                                 29:29
#define INT_MASK_GPIO53_DISABLE                         0
#define INT_MASK_GPIO53_ENABLE                          1
#define INT_MASK_GPIO52                                 28:28
#define INT_MASK_GPIO52_DISABLE                         0
#define INT_MASK_GPIO52_ENABLE                          1
#define INT_MASK_GPIO51                                 27:27
#define INT_MASK_GPIO51_DISABLE                         0
#define INT_MASK_GPIO51_ENABLE                          1
#define INT_MASK_GPIO50                                 26:26
#define INT_MASK_GPIO50_DISABLE                         0
#define INT_MASK_GPIO50_ENABLE                          1
#define INT_MASK_GPIO49                                 25:25
#define INT_MASK_GPIO49_DISABLE                         0
#define INT_MASK_GPIO49_ENABLE                          1
#define INT_MASK_GPIO48                                 24:24
#define INT_MASK_GPIO48_DISABLE                         0
#define INT_MASK_GPIO48_ENABLE                          1
#define INT_MASK_I2C                                    23:23
#define INT_MASK_I2C_DISABLE                            0
#define INT_MASK_I2C_ENABLE                             1
#define INT_MASK_PWM                                    22:22
#define INT_MASK_PWM_DISABLE                            0
#define INT_MASK_PWM_ENABLE                             1
#define INT_MASK_DMA                                    20:20
#define INT_MASK_DMA_DISABLE                            0
#define INT_MASK_DMA_ENABLE                             1
#define INT_MASK_PCI                                    19:19
#define INT_MASK_PCI_DISABLE                            0
#define INT_MASK_PCI_ENABLE                             1
#define INT_MASK_I2S                                    18:18
#define INT_MASK_I2S_DISABLE                            0
#define INT_MASK_I2S_ENABLE                             1
#define INT_MASK_AC97                                   17:17
#define INT_MASK_AC97_DISABLE                           0
#define INT_MASK_AC97_ENABLE                            1
#define INT_MASK_USB_SLAVE                              16:16
#define INT_MASK_USB_SLAVE_DISABLE                      0
#define INT_MASK_USB_SLAVE_ENABLE                       1
#define INT_MASK_UART1                                  13:13
#define INT_MASK_UART1_DISABLE                          0
#define INT_MASK_UART1_ENABLE                           1
#define INT_MASK_UART0                                  12:12
#define INT_MASK_UART0_DISABLE                          0
#define INT_MASK_UART0_ENABLE                           1
#define INT_MASK_CRT_VSYNC                              11:11
#define INT_MASK_CRT_VSYNC_DISABLE                      0
#define INT_MASK_CRT_VSYNC_ENABLE                       1
#define INT_MASK_8051                                   10:10
#define INT_MASK_8051_DISABLE                           0
#define INT_MASK_8051_ENABLE                            1
#define INT_MASK_SSP1                                   9:9
#define INT_MASK_SSP1_DISABLE                           0
#define INT_MASK_SSP1_ENABLE                            1
#define INT_MASK_SSP0                                   8:8
#define INT_MASK_SSP0_DISABLE                           0
#define INT_MASK_SSP0_ENABLE                            1
#define INT_MASK_USB_HOST                               6:6
#define INT_MASK_USB_HOST_DISABLE                       0
#define INT_MASK_USB_HOST_ENABLE                        1
#define INT_MASK_2D                                     3:3
#define INT_MASK_2D_DISABLE                             0
#define INT_MASK_2D_ENABLE                              1
#define INT_MASK_ZVPORT                                 2:2
#define INT_MASK_ZVPORT_DISABLE                         0
#define INT_MASK_ZVPORT_ENABLE                          1
#define INT_MASK_PANEL_VSYNC                            1:1
#define INT_MASK_PANEL_VSYNC_DISABLE                    0
#define INT_MASK_PANEL_VSYNC_ENABLE                     1
#define INT_MASK_CMD_INTPR                              0:0
#define INT_MASK_CMD_INTPR_DISABLE                      0
#define INT_MASK_CMD_INTPR_ENABLE                       1

#define DEBUG_CTRL                                      0x000034
#define DEBUG_CTRL_MODULE                               7:5
#define DEBUG_CTRL_PARTITION                            4:0
#define DEBUG_CTRL_PARTITION_HIF                        0
#define DEBUG_CTRL_PARTITION_CPUMEM                     1
#define DEBUG_CTRL_PARTITION_PCI                        2
#define DEBUG_CTRL_PARTITION_CMD_INTPR                  3
#define DEBUG_CTRL_PARTITION_DISPLAY                    4
#define DEBUG_CTRL_PARTITION_ZVPORT                     5
#define DEBUG_CTRL_PARTITION_2D                         6
#define DEBUG_CTRL_PARTITION_MIF                        8
#define DEBUG_CTRL_PARTITION_USB_HOST                   10
#define DEBUG_CTRL_PARTITION_SSP0                       12
#define DEBUG_CTRL_PARTITION_SSP1                       13
#define DEBUG_CTRL_PARTITION_UART0                      19
#define DEBUG_CTRL_PARTITION_UART1                      20
#define DEBUG_CTRL_PARTITION_I2C                        21
#define DEBUG_CTRL_PARTITION_8051                       23
#define DEBUG_CTRL_PARTITION_AC97                       24
#define DEBUG_CTRL_PARTITION_I2S                        25
#define DEBUG_CTRL_PARTITION_INTMEM                     26
#define DEBUG_CTRL_PARTITION_DMA                        27
#define DEBUG_CTRL_PARTITION_SIMULATION                 28

#define CURRENT_POWER_GATE                              0x000038
#define CURRENT_POWER_GATE_AC97_I2S                     18:18
#define CURRENT_POWER_GATE_AC97_I2S_DISABLE             0
#define CURRENT_POWER_GATE_AC97_I2S_ENABLE              1
#define CURRENT_POWER_GATE_8051                         17:17
#define CURRENT_POWER_GATE_8051_DISABLE                 0
#define CURRENT_POWER_GATE_8051_ENABLE                  1
#define CURRENT_POWER_GATE_PLL                          16:16
#define CURRENT_POWER_GATE_PLL_DISABLE                  0
#define CURRENT_POWER_GATE_PLL_ENABLE                   1
#define CURRENT_POWER_GATE_OSCILLATOR                   15:15
#define CURRENT_POWER_GATE_OSCILLATOR_DISABLE           0
#define CURRENT_POWER_GATE_OSCILLATOR_ENABLE            1
#define CURRENT_POWER_GATE_PLL_RECOVERY                 14:13
#define CURRENT_POWER_GATE_PLL_RECOVERY_32              0
#define CURRENT_POWER_GATE_PLL_RECOVERY_64              1
#define CURRENT_POWER_GATE_PLL_RECOVERY_96              2
#define CURRENT_POWER_GATE_PLL_RECOVERY_128             3
#define CURRENT_POWER_GATE_USB_SLAVE                    12:12
#define CURRENT_POWER_GATE_USB_SLAVE_DISABLE            0
#define CURRENT_POWER_GATE_USB_SLAVE_ENABLE             1
#define CURRENT_POWER_GATE_USB_HOST                     11:11
#define CURRENT_POWER_GATE_USB_HOST_DISABLE             0
#define CURRENT_POWER_GATE_USB_HOST_ENABLE              1
#define CURRENT_POWER_GATE_SSP0_SSP1                    10:10
#define CURRENT_POWER_GATE_SSP0_SSP1_DISABLE            0
#define CURRENT_POWER_GATE_SSP0_SSP1_ENABLE             1
#define CURRENT_POWER_GATE_UART1                        8:8
#define CURRENT_POWER_GATE_UART1_DISABLE                0
#define CURRENT_POWER_GATE_UART1_ENABLE                 1
#define CURRENT_POWER_GATE_UART0                        7:7
#define CURRENT_POWER_GATE_UART0_DISABLE                0
#define CURRENT_POWER_GATE_UART0_ENABLE                 1
#define CURRENT_POWER_GATE_GPIO_PWM_I2C                 6:6
#define CURRENT_POWER_GATE_GPIO_PWM_I2C_DISABLE         0
#define CURRENT_POWER_GATE_GPIO_PWM_I2C_ENABLE          1
#define CURRENT_POWER_GATE_ZVPORT                       5:5
#define CURRENT_POWER_GATE_ZVPORT_DISABLE               0
#define CURRENT_POWER_GATE_ZVPORT_ENABLE                1
#define CURRENT_POWER_GATE_CSC                          4:4
#define CURRENT_POWER_GATE_CSC_DISABLE                  0
#define CURRENT_POWER_GATE_CSC_ENABLE                   1
#define CURRENT_POWER_GATE_2D                           3:3
#define CURRENT_POWER_GATE_2D_DISABLE                   0
#define CURRENT_POWER_GATE_2D_ENABLE                    1
#define CURRENT_POWER_GATE_DISPLAY                      2:2
#define CURRENT_POWER_GATE_DISPLAY_DISABLE              0
#define CURRENT_POWER_GATE_DISPLAY_ENABLE               1
#define CURRENT_POWER_GATE_INTMEM                       1:1
#define CURRENT_POWER_GATE_INTMEM_DISABLE               0
#define CURRENT_POWER_GATE_INTMEM_ENABLE                1
#define CURRENT_POWER_GATE_HOST                         0:0
#define CURRENT_POWER_GATE_HOST_DISABLE                 0
#define CURRENT_POWER_GATE_HOST_ENABLE                  1

#define CURRENT_POWER_CLOCK                             0x00003C
#define CURRENT_POWER_CLOCK_P2XCLK_SELECT               29:29
#define CURRENT_POWER_CLOCK_P2XCLK_SELECT_288           0
#define CURRENT_POWER_CLOCK_P2XCLK_SELECT_336           1
#define CURRENT_POWER_CLOCK_P2XCLK_DIVIDER              28:27
#define CURRENT_POWER_CLOCK_P2XCLK_DIVIDER_1            0
#define CURRENT_POWER_CLOCK_P2XCLK_DIVIDER_3            1
#define CURRENT_POWER_CLOCK_P2XCLK_DIVIDER_5            2
#define CURRENT_POWER_CLOCK_P2XCLK_SHIFT                26:24
#define CURRENT_POWER_CLOCK_P2XCLK_SHIFT_0              0
#define CURRENT_POWER_CLOCK_P2XCLK_SHIFT_1              1
#define CURRENT_POWER_CLOCK_P2XCLK_SHIFT_2              2
#define CURRENT_POWER_CLOCK_P2XCLK_SHIFT_3              3
#define CURRENT_POWER_CLOCK_P2XCLK_SHIFT_4              4
#define CURRENT_POWER_CLOCK_P2XCLK_SHIFT_5              5
#define CURRENT_POWER_CLOCK_P2XCLK_SHIFT_6              6
#define CURRENT_POWER_CLOCK_P2XCLK_SHIFT_7              7
#define CURRENT_POWER_CLOCK_V2XCLK_SELECT               20:20
#define CURRENT_POWER_CLOCK_V2XCLK_SELECT_288           0
#define CURRENT_POWER_CLOCK_V2XCLK_SELECT_336           1
#define CURRENT_POWER_CLOCK_V2XCLK_DIVIDER              19:19
#define CURRENT_POWER_CLOCK_V2XCLK_DIVIDER_1            0
#define CURRENT_POWER_CLOCK_V2XCLK_DIVIDER_3            1
#define CURRENT_POWER_CLOCK_V2XCLK_SHIFT                18:16
#define CURRENT_POWER_CLOCK_V2XCLK_SHIFT_0              0
#define CURRENT_POWER_CLOCK_V2XCLK_SHIFT_1              1
#define CURRENT_POWER_CLOCK_V2XCLK_SHIFT_2              2
#define CURRENT_POWER_CLOCK_V2XCLK_SHIFT_3              3
#define CURRENT_POWER_CLOCK_V2XCLK_SHIFT_4              4
#define CURRENT_POWER_CLOCK_V2XCLK_SHIFT_5              5
#define CURRENT_POWER_CLOCK_V2XCLK_SHIFT_6              6
#define CURRENT_POWER_CLOCK_V2XCLK_SHIFT_7              7
#define CURRENT_POWER_CLOCK_MCLK_SELECT                 12:12
#define CURRENT_POWER_CLOCK_MCLK_SELECT_288             0
#define CURRENT_POWER_CLOCK_MCLK_SELECT_336             1
#define CURRENT_POWER_CLOCK_MCLK_DIVIDER                11:11
#define CURRENT_POWER_CLOCK_MCLK_DIVIDER_1              0
#define CURRENT_POWER_CLOCK_MCLK_DIVIDER_3              1
#define CURRENT_POWER_CLOCK_MCLK_SHIFT                  10:8
#define CURRENT_POWER_CLOCK_MCLK_SHIFT_0                0
#define CURRENT_POWER_CLOCK_MCLK_SHIFT_1                1
#define CURRENT_POWER_CLOCK_MCLK_SHIFT_2                2
#define CURRENT_POWER_CLOCK_MCLK_SHIFT_3                3
#define CURRENT_POWER_CLOCK_MCLK_SHIFT_4                4
#define CURRENT_POWER_CLOCK_MCLK_SHIFT_5                5
#define CURRENT_POWER_CLOCK_MCLK_SHIFT_6                6
#define CURRENT_POWER_CLOCK_MCLK_SHIFT_7                7
#define CURRENT_POWER_CLOCK_M2XCLK_SELECT               4:4
#define CURRENT_POWER_CLOCK_M2XCLK_SELECT_288           0
#define CURRENT_POWER_CLOCK_M2XCLK_SELECT_336           1
#define CURRENT_POWER_CLOCK_M2XCLK_DIVIDER              3:3
#define CURRENT_POWER_CLOCK_M2XCLK_DIVIDER_1            0
#define CURRENT_POWER_CLOCK_M2XCLK_DIVIDER_3            1
#define CURRENT_POWER_CLOCK_M2XCLK_SHIFT                2:0
#define CURRENT_POWER_CLOCK_M2XCLK_SHIFT_0              0
#define CURRENT_POWER_CLOCK_M2XCLK_SHIFT_1              1
#define CURRENT_POWER_CLOCK_M2XCLK_SHIFT_2              2
#define CURRENT_POWER_CLOCK_M2XCLK_SHIFT_3              3
#define CURRENT_POWER_CLOCK_M2XCLK_SHIFT_4              4
#define CURRENT_POWER_CLOCK_M2XCLK_SHIFT_5              5
#define CURRENT_POWER_CLOCK_M2XCLK_SHIFT_6              6
#define CURRENT_POWER_CLOCK_M2XCLK_SHIFT_7              7

#define POWER_MODE0_GATE                                0x000040
#define POWER_MODE0_CLOCK                               0x000044
#define POWER_MODE1_GATE                                0x000048
#define POWER_MODE1_CLOCK                               0x00004C

#define POWER_MODE_GATE_AC97_I2S                       18:18
#define POWER_MODE_GATE_AC97_I2S_DISABLE               0
#define POWER_MODE_GATE_AC97_I2S_ENABLE                1
#define POWER_MODE_GATE_8051                           17:17
#define POWER_MODE_GATE_8051_DISABLE                   0
#define POWER_MODE_GATE_8051_ENABLE                    1
#define POWER_MODE_GATE_USB_SLAVE                      12:12
#define POWER_MODE_GATE_USB_SLAVE_DISABLE              0
#define POWER_MODE_GATE_USB_SLAVE_ENABLE               1
#define POWER_MODE_GATE_USB_HOST                       11:11
#define POWER_MODE_GATE_USB_HOST_DISABLE               0
#define POWER_MODE_GATE_USB_HOST_ENABLE                1
#define POWER_MODE_GATE_SSP0_SSP1                      10:10
#define POWER_MODE_GATE_SSP0_SSP1_DISABLE              0
#define POWER_MODE_GATE_SSP0_SSP1_ENABLE               1
#define POWER_MODE_GATE_UART1                          8:8
#define POWER_MODE_GATE_UART1_DISABLE                  0
#define POWER_MODE_GATE_UART1_ENABLE                   1
#define POWER_MODE_GATE_UART0                          7:7
#define POWER_MODE_GATE_UART0_DISABLE                  0
#define POWER_MODE_GATE_UART0_ENABLE                   1
#define POWER_MODE_GATE_GPIO_PWM_I2C                   6:6
#define POWER_MODE_GATE_GPIO_PWM_I2C_DISABLE           0
#define POWER_MODE_GATE_GPIO_PWM_I2C_ENABLE            1
#define POWER_MODE_GATE_ZVPORT                         5:5
#define POWER_MODE_GATE_ZVPORT_DISABLE                 0
#define POWER_MODE_GATE_ZVPORT_ENABLE                  1
#define POWER_MODE_GATE_CSC                            4:4
#define POWER_MODE_GATE_CSC_DISABLE                    0
#define POWER_MODE_GATE_CSC_ENABLE                     1
#define POWER_MODE_GATE_2D                             3:3
#define POWER_MODE_GATE_2D_DISABLE                     0
#define POWER_MODE_GATE_2D_ENABLE                      1
#define POWER_MODE_GATE_DISPLAY                        2:2
#define POWER_MODE_GATE_DISPLAY_DISABLE                0
#define POWER_MODE_GATE_DISPLAY_ENABLE                 1
#define POWER_MODE_GATE_INTMEM                         1:1
#define POWER_MODE_GATE_INTMEM_DISABLE                 0
#define POWER_MODE_GATE_INTMEM_ENABLE                  1
#define POWER_MODE_GATE_HOST                           0:0
#define POWER_MODE_GATE_HOST_DISABLE                   0
#define POWER_MODE_GATE_HOST_ENABLE                    1

#define POWER_MODE_CLOCK_P2XCLK_SELECT                 29:29
#define POWER_MODE_CLOCK_P2XCLK_SELECT_288             0
#define POWER_MODE_CLOCK_P2XCLK_SELECT_336             1
#define POWER_MODE_CLOCK_P2XCLK_DIVIDER                28:27
#define POWER_MODE_CLOCK_P2XCLK_DIVIDER_1              0
#define POWER_MODE_CLOCK_P2XCLK_DIVIDER_3              1
#define POWER_MODE_CLOCK_P2XCLK_DIVIDER_5              2
#define POWER_MODE_CLOCK_P2XCLK_SHIFT                  26:24
#define POWER_MODE_CLOCK_P2XCLK_SHIFT_0                0
#define POWER_MODE_CLOCK_P2XCLK_SHIFT_1                1
#define POWER_MODE_CLOCK_P2XCLK_SHIFT_2                2
#define POWER_MODE_CLOCK_P2XCLK_SHIFT_3                3
#define POWER_MODE_CLOCK_P2XCLK_SHIFT_4                4
#define POWER_MODE_CLOCK_P2XCLK_SHIFT_5                5
#define POWER_MODE_CLOCK_P2XCLK_SHIFT_6                6
#define POWER_MODE_CLOCK_P2XCLK_SHIFT_7                7
#define POWER_MODE_CLOCK_V2XCLK_SELECT                 20:20
#define POWER_MODE_CLOCK_V2XCLK_SELECT_288             0
#define POWER_MODE_CLOCK_V2XCLK_SELECT_336             1
#define POWER_MODE_CLOCK_V2XCLK_DIVIDER                19:19
#define POWER_MODE_CLOCK_V2XCLK_DIVIDER_1              0
#define POWER_MODE_CLOCK_V2XCLK_DIVIDER_3              1
#define POWER_MODE_CLOCK_V2XCLK_SHIFT                  18:16
#define POWER_MODE_CLOCK_V2XCLK_SHIFT_0                0
#define POWER_MODE_CLOCK_V2XCLK_SHIFT_1                1
#define POWER_MODE_CLOCK_V2XCLK_SHIFT_2                2
#define POWER_MODE_CLOCK_V2XCLK_SHIFT_3                3
#define POWER_MODE_CLOCK_V2XCLK_SHIFT_4                4
#define POWER_MODE_CLOCK_V2XCLK_SHIFT_5                5
#define POWER_MODE_CLOCK_V2XCLK_SHIFT_6                6
#define POWER_MODE_CLOCK_V2XCLK_SHIFT_7                7
#define POWER_MODE_CLOCK_MCLK_SELECT                   12:12
#define POWER_MODE_CLOCK_MCLK_SELECT_288               0
#define POWER_MODE_CLOCK_MCLK_SELECT_336               1
#define POWER_MODE_CLOCK_MCLK_DIVIDER                  11:11
#define POWER_MODE_CLOCK_MCLK_DIVIDER_1                0
#define POWER_MODE_CLOCK_MCLK_DIVIDER_3                1
#define POWER_MODE_CLOCK_MCLK_SHIFT                    10:8
#define POWER_MODE_CLOCK_MCLK_SHIFT_0                  0
#define POWER_MODE_CLOCK_MCLK_SHIFT_1                  1
#define POWER_MODE_CLOCK_MCLK_SHIFT_2                  2
#define POWER_MODE_CLOCK_MCLK_SHIFT_3                  3
#define POWER_MODE_CLOCK_MCLK_SHIFT_4                  4
#define POWER_MODE_CLOCK_MCLK_SHIFT_5                  5
#define POWER_MODE_CLOCK_MCLK_SHIFT_6                  6
#define POWER_MODE_CLOCK_MCLK_SHIFT_7                  7
#define POWER_MODE_CLOCK_M1XCLK_SELECT                 4:4
#define POWER_MODE_CLOCK_M1XCLK_SELECT_288             0
#define POWER_MODE_CLOCK_M1XCLK_SELECT_336             1
#define POWER_MODE_CLOCK_M1XCLK_DIVIDER                3:3
#define POWER_MODE_CLOCK_M1XCLK_DIVIDER_1              0
#define POWER_MODE_CLOCK_M1XCLK_DIVIDER_3              1
#define POWER_MODE_CLOCK_M1XCLK_SHIFT                  2:0
#define POWER_MODE_CLOCK_M1XCLK_SHIFT_0                0
#define POWER_MODE_CLOCK_M1XCLK_SHIFT_1                1
#define POWER_MODE_CLOCK_M1XCLK_SHIFT_2                2
#define POWER_MODE_CLOCK_M1XCLK_SHIFT_3                3
#define POWER_MODE_CLOCK_M1XCLK_SHIFT_4                4
#define POWER_MODE_CLOCK_M1XCLK_SHIFT_5                5
#define POWER_MODE_CLOCK_M1XCLK_SHIFT_6                6
#define POWER_MODE_CLOCK_M1XCLK_SHIFT_7                7

#define POWER_SLEEP_GATE                                0x000050
#define POWER_SLEEP_GATE_PLL_RECOVERY_CLOCK             22:19
#define POWER_SLEEP_GATE_PLL_RECOVERY_CLOCK_4096        0
#define POWER_SLEEP_GATE_PLL_RECOVERY_CLOCK_2048        1
#define POWER_SLEEP_GATE_PLL_RECOVERY_CLOCK_1024        2
#define POWER_SLEEP_GATE_PLL_RECOVERY_CLOCK_512         3
#define POWER_SLEEP_GATE_PLL_RECOVERY_CLOCK_256         4
#define POWER_SLEEP_GATE_PLL_RECOVERY_CLOCK_128         5
#define POWER_SLEEP_GATE_PLL_RECOVERY_CLOCK_64          6
#define POWER_SLEEP_GATE_PLL_RECOVERY_CLOCK_32          7
#define POWER_SLEEP_GATE_PLL_RECOVERY_CLOCK_16          8
#define POWER_SLEEP_GATE_PLL_RECOVERY_CLOCK_8           9
#define POWER_SLEEP_GATE_PLL_RECOVERY_CLOCK_4           10
#define POWER_SLEEP_GATE_PLL_RECOVERY_CLOCK_2           11
#define POWER_SLEEP_GATE_PLL_RECOVERY                   14:13
#define POWER_SLEEP_GATE_PLL_RECOVERY_32                0
#define POWER_SLEEP_GATE_PLL_RECOVERY_64                1
#define POWER_SLEEP_GATE_PLL_RECOVERY_96                2
#define POWER_SLEEP_GATE_PLL_RECOVERY_128               3

#define POWER_MODE_CTRL                                 0x000054
#define POWER_MODE_CTRL_SLEEP_STATUS                    2:2
#define POWER_MODE_CTRL_SLEEP_STATUS_INACTIVE           0
#define POWER_MODE_CTRL_SLEEP_STATUS_ACTIVE             1
#define POWER_MODE_CTRL_MODE                            1:0
#define POWER_MODE_CTRL_MODE_MODE0                      0
#define POWER_MODE_CTRL_MODE_MODE1                      1
#define POWER_MODE_CTRL_MODE_SLEEP                      2

#define PCI_MASTER_BASE                                 0x000058
#define PCI_MASTER_BASE_ADDRESS                         31:20

#define ENDIAN_CTRL                                     0x00005C
#define ENDIAN_CTRL_ENDIAN                              0:0
#define ENDIAN_CTRL_ENDIAN_LITTLE                       0
#define ENDIAN_CTRL_ENDIAN_BIG                          1

#define DEVICE_ID                                       0x000060
#define DEVICE_ID_DEVICE_ID                             31:16
#define DEVICE_ID_REVISION_ID                           7:0

#define PLL_CLOCK_COUNT                                 0x000064
#define PLL_CLOCK_COUNT_COUNTER                         15:0

#define MISC_TIMING                                		0x000068
#define MISC_TIMING_EX		         					31:28
#define MISC_TIMING_EX_OFF	         					0
#define MISC_TIMING_EX_16 	         					1
#define MISC_TIMING_EX_32 	         					2
#define MISC_TIMING_EX_48 	         					3
#define MISC_TIMING_EX_64 	         					4
#define MISC_TIMING_EX_80 	         					5
#define MISC_TIMING_EX_96 	         					6
#define MISC_TIMING_EX_112	         					7
#define MISC_TIMING_EX_128	         					8
#define MISC_TIMING_EX_144	         					9
#define MISC_TIMING_EX_160	         					10
#define MISC_TIMING_EX_176	         					11
#define MISC_TIMING_EX_192	         					12
#define MISC_TIMING_EX_208	         					13
#define MISC_TIMING_EX_224	         					14
#define MISC_TIMING_EX_240	         					15
#define MISC_TIMING_XC		         					25:24
#define MISC_TIMING_XC_PLL 	         					0
#define MISC_TIMING_XC_HCLK	         					1
#define MISC_TIMING_XC_GPIO         					2
#define MISC_TIMING_USB_OVERCURRENT    					21:21
#define MISC_TIMING_USB_OVERCURRENT_DISABLE				0
#define MISC_TIMING_USB_OVERCURRENT_ENABLE				1
#define MISC_TIMING_SDRAM1_SELECT                 		20:20
#define MISC_TIMING_SDRAM1_SELECT_288             		0
#define MISC_TIMING_SDRAM1_SELECT_336             		1
#define MISC_TIMING_SDRAM1_DIVIDER     					19:19
#define MISC_TIMING_SDRAM1_DIVIDER_1              		0
#define MISC_TIMING_SDRAM1_DIVIDER_3              		1
#define MISC_TIMING_SDRAM1_SHIFT  						18:16
#define MISC_TIMING_SDRAM1_SHIFT_0						0
#define MISC_TIMING_SDRAM1_SHIFT_1						1
#define MISC_TIMING_SDRAM1_SHIFT_2						2
#define MISC_TIMING_SDRAM1_SHIFT_3						3
#define MISC_TIMING_SDRAM1_SHIFT_4						4
#define MISC_TIMING_SDRAM1_SHIFT_5						5
#define MISC_TIMING_SDRAM1_SHIFT_6						6
#define MISC_TIMING_SDRAM1_SHIFT_7						7
#define MISC_TIMING_SDRAM0_SELECT                 		12:12
#define MISC_TIMING_SDRAM0_SELECT_288             		0
#define MISC_TIMING_SDRAM0_SELECT_336             		1
#define MISC_TIMING_SDRAM0_DIVIDER     					11:11
#define MISC_TIMING_SDRAM0_DIVIDER_1              		0
#define MISC_TIMING_SDRAM0_DIVIDER_3              		1
#define MISC_TIMING_SDRAM0_SHIFT  						10:8
#define MISC_TIMING_SDRAM0_SHIFT_0						0
#define MISC_TIMING_SDRAM0_SHIFT_1						1
#define MISC_TIMING_SDRAM0_SHIFT_2						2
#define MISC_TIMING_SDRAM0_SHIFT_3						3
#define MISC_TIMING_SDRAM0_SHIFT_4						4
#define MISC_TIMING_SDRAM0_SHIFT_5						5
#define MISC_TIMING_SDRAM0_SHIFT_6						6
#define MISC_TIMING_SDRAM0_SHIFT_7						7
#define MISC_TIMING_DBG		         					7:7
#define MISC_TIMING_DBG_CHECK_INPUT   					0
#define MISC_TIMING_DBG_CHECK_OUTPUT   					1
#define MISC_TIMING_ACPI	         					6:6
#define MISC_TIMING_ACPI_DISABLE	   					0
#define MISC_TIMING_ACPI_ENABLE 	  					1
#define MISC_TIMING_DIVIDER         					5:4
#define MISC_TIMING_DIVIDER_336     					0
#define MISC_TIMING_DIVIDER_288     					1
#define MISC_TIMING_DIVIDER_240     					2
#define MISC_TIMING_DIVIDER_192     					3
#define MISC_TIMING_USB_HOST_MODE                  		3:3
#define MISC_TIMING_USB_HOST_MODE_NORMAL           		0
#define MISC_TIMING_USB_HOST_MODE_SIMULATION           	1
#define MISC_TIMING_READ_DELAY                     		2:0
#define MISC_TIMING_READ_DELAY_OFF                 		0
#define MISC_TIMING_READ_DELAY_0_5NS					1
#define MISC_TIMING_READ_DELAY_1NS  					2
#define MISC_TIMING_READ_DELAY_1_5NS					3
#define MISC_TIMING_READ_DELAY_2NS  					4
#define MISC_TIMING_READ_DELAY_2_5NS					5

#define CURRENT_SDRAM_CLOCK                        		0x00006C
#define CURRENT_SDRAM_CLOCK_SELECT			            4:4
#define CURRENT_SDRAM_CLOCK_SELECT_288           		0
#define CURRENT_SDRAM_CLOCK_SELECT_336           		1
#define CURRENT_SDRAM_CLOCK_DIVIDER                		3:3
#define CURRENT_SDRAM_CLOCK_DIVIDER_1              		0
#define CURRENT_SDRAM_CLOCK_DIVIDER_3              		1
#define CURRENT_SDRAM_CLOCK_SHIFT                  		2:0
#define CURRENT_SDRAM_CLOCK_SHIFT_0                		0
#define CURRENT_SDRAM_CLOCK_SHIFT_1                		1
#define CURRENT_SDRAM_CLOCK_SHIFT_2                		2
#define CURRENT_SDRAM_CLOCK_SHIFT_3                		3
#define CURRENT_SDRAM_CLOCK_SHIFT_4                		4
#define CURRENT_SDRAM_CLOCK_SHIFT_5                		5
#define CURRENT_SDRAM_CLOCK_SHIFT_6                		6
#define CURRENT_SDRAM_CLOCK_SHIFT_7                		7

#define NUM_CONTROL_REGS	((CURRENT_SDRAM_CLOCK-SYSTEM_CTRL)/sizeof(u32))
/*///////////////////////////////////////////////////////////////////////////////
// Internal macros                                                            //
///////////////////////////////////////////////////////////////////////////////*/

#define _F_START(f)             (0 ? f)
#define _F_END(f)               (1 ? f)
#define _F_SIZE(f)              (1 + _F_END(f) - _F_START(f))
#define _F_MASK(f)              (((1 << _F_SIZE(f)) - 1) << _F_START(f))
#define _F_NORMALIZE(v, f)      (((v) & _F_MASK(f)) >> _F_START(f))
#define _F_DENORMALIZE(v, f)    (((v) << _F_START(f)) & _F_MASK(f))

/*///////////////////////////////////////////////////////////////////////////////
// Global macros                                                              //
///////////////////////////////////////////////////////////////////////////////*/

#define FGET(x, reg, field) \
( \
    _F_NORMALIZE((x), reg ## _ ## field) \
)

#define FSET(x, reg, field, value) \
( \
    (x & ~_F_MASK(reg ## _ ## field)) \
    | _F_DENORMALIZE(reg ## _ ## field ## _ ## value, reg ## _ ## field) \
)

#define FVAL(x, reg, field, value) \
( \
    (x & ~_F_MASK(reg ## _ ## field)) \
    | _F_DENORMALIZE(value, reg ## _ ## field) \
)

#define FCLEAR(reg, field) \
( \
    ~ _F_MASK(reg ## _ ## field) \
)

/*///////////////////////////////////////////////////////////////////////////////
// Field Macros                                                               //
///////////////////////////////////////////////////////////////////////////////*/

#define FIELD_START(field)              (0 ? field)
#define FIELD_END(field)                (1 ? field)
#define FIELD_SIZE(field)               (1 + FIELD_END(field) - FIELD_START(field))
#define FIELD_MASK(field)               (((1 << (FIELD_SIZE(field)-1)) | \
										((1 << (FIELD_SIZE(field)-1)) - 1)) << \
										FIELD_START(field))

#define FIELD_NORMALIZE(reg, field)     (((reg) & FIELD_MASK(field)) >> FIELD_START(field))
#define FIELD_DENORMALIZE(field, value) ((((u32)value) << FIELD_START(field)) & FIELD_MASK(field))

#define FINIT(reg, field, value)   		FIELD_DENORMALIZE(reg ## _ ## field, \
                                                          reg ## _ ## field ## _ ## value)
#define FINIT_VAL(reg, field, value)	(FIELD_DENORMALIZE(reg ## _ ## field, value))

#define FVAL_SET(x, r, f, v)       		x = x & ~FIELD_MASK(r ## _ ## f) \
                                            | FIELD_DENORMALIZE(r ## _ ## f, r ## _ ## f ## _ ## v)

#define FMASK(reg, field)      			FIELD_MASK(reg ## _ ## field)

#ifdef __KERNEL__

/*
* Private structure
*/

enum {
	SM5XX_DEVID_FB	= 0,
	SM5XX_DEVID_USB,
	SM5XX_DEVID_I2C,
	SM5XX_DEV_COUNT
};

struct sm5xx_mem_list;

struct sm5xx_bus;
typedef irqreturn_t (*sm5xx_handler_t)(struct sm5xx_bus *bus, u32, void *);

struct sm5xx_irqdesc {
	sm5xx_handler_t handler;
	u32  mask;
	void *data;
} ____cacheline_aligned;

struct sm5xx_bus {
	struct  list_head bus_list;
	struct  device  *bus_dev;	/* pci/platform device */
	int 	busnum;	  			/* Bus number (in order of reg) */
	int 	irq;

	u32 	reg_addr;   	    /* !!!Physical addr !!!*/
	void    __iomem *regbase;
	u32 	regs_size;

	u32 	mem_addr;   	    /* !!!Physical addr !!!*/
	void    __iomem *mem;
	u32		mem_limit;

	struct sm5xx_mem_list *all_free_mem;
	struct list_head mem_list_head;
	struct list_head free_list_head;

	u32 pm_normal_gate_reg;

	struct circ_buf ci_list;
	dma_addr_t h_ci_list;
	spinlock_t ci_list_lock;

	struct semaphore control_regs_lock;

	spinlock_t irq_lock;
	struct sm5xx_irqdesc irq_desc[SM5XX_NR_INTS];

	u32	saved_regs[50];
};
typedef struct sm5xx_bus sm5xx_bus_t;

struct sm5xx_memreq {
	u32 paddr;
	u32 size;
};

struct sm5xx_dev {
	struct device	dev;
	uint		devid;
	u64		dma_mask;
};

#define SM5XX_DEV(_d)	container_of((_d), struct sm5xx_dev, dev)
#define to_sm5xx_bus(sm5xxdev) ((struct sm5xx_bus *)dev_get_drvdata(((struct sm5xx_dev *)(sm5xxdev))->dev.parent))

#define sm5xx_get_drvdata(d)	dev_get_drvdata(&(d)->dev)
#define sm5xx_set_drvdata(d,p)	dev_set_drvdata(&(d)->dev, p)

struct sm5xx_driver {
	struct device_driver	drv;
	unsigned int		devid;
	int (*probe)(struct sm5xx_dev *);
	int (*remove)(struct sm5xx_dev *);
	int (*suspend)(struct sm5xx_dev *, pm_message_t);
	int (*resume)(struct sm5xx_dev *);
};

#define SM5XX_DRV(_d)	container_of((_d), struct sm5xx_driver, drv)
#define SM5XX_DRIVER_NAME(_sm5xxdev) ((_sm5xxdev)->dev.driver->name)

struct sm5xx_bus *sm5xx_bus_alloc(void);
void sm5xx_bus_free(struct sm5xx_bus *bus);
int sm5xx_request_irq(struct sm5xx_bus *bus, int irq_mask,
					  sm5xx_handler_t, void *data, int irq_enable);
int sm5xx_bus_init(struct sm5xx_bus *sm_bus);
int sm5xx_driver_register(struct sm5xx_driver *);
void sm5xx_driver_unregister(struct sm5xx_driver *);
void *sm5xx_malloc(struct sm5xx_bus *bus, u32 size, u32 align);
void sm5xx_free(struct sm5xx_bus *bus, void *vaddr);
struct sm5xx_dev *sm5xx_find_child(struct sm5xx_bus *bus, unsigned int child_id);

u32 sm5xx_mask_irq(struct sm5xx_bus *bus, u32 irq_mask);
void sm5xx_unmask_irq(struct sm5xx_bus *bus, u32 irq_mask);


/*
  By default: for normal system operation uses POWER_MODE0_xxx regs pair,
  and POWER_MODE1_xxx for suspend mode, But it may be changed by
  BIOS configuration or by module parameter (pm_suspend_reg)
 */
int sm5xx_enable_subsystem(struct sm5xx_bus *bus,
								u32 normal_gate_mask,
								u32 suspend_gate_mask);

void sm5xx_disable_subsystem(struct sm5xx_bus *bus, u32 gate_reg_mask);

#ifdef DEBUG
void sm5xx_dump_regs(struct sm5xx_bus *bus);
#endif

static inline u32 sm5xx_virt_to_offset(struct sm5xx_bus *bus, void *virt)
{
	return (virt - bus->mem);
}

static inline u32 sm5xx_virt_to_phys(struct sm5xx_bus *bus, void * virt)
{
	return (virt - bus->mem) + bus->mem_addr;
}

static inline u32 sm5xx_phys_to_offset(struct sm5xx_bus *bus, u32 phys)
{
	return (phys - bus->mem_addr);
}

static inline void *sm5xx_phys_to_virt(struct sm5xx_bus *bus, u32 phys)
{
	return (phys - bus->mem_addr) + bus->mem;
}

#ifndef DEBUG
static inline u32 _sm5xx_read32(int regindex, const struct sm5xx_bus *bus)
{
	return readl(bus->regbase + regindex);
}

static inline void _sm5xx_write32(int regindex, u32 val, const struct sm5xx_bus *bus)
{
	writel(val, bus->regbase + regindex);
}
#else
extern u32 _sm5xx_read32(int regindex, const struct sm5xx_bus *_bus);
extern void _sm5xx_write32(int regindex, u32 val, const struct sm5xx_bus *_bus);
#endif

#define sm5xx_mem_readb __raw_readb
#define sm5xx_mem_readw __raw_readw
#define sm5xx_mem_readl __raw_readl
#define sm5xx_mem_readq __raw_readq
#define sm5xx_mem_writeb __raw_writeb
#define sm5xx_mem_writew __raw_writew
#define sm5xx_mem_writel __raw_writel
#define sm5xx_mem_writeq __raw_writeq


static inline void sm5xx_acquire_control_lock(struct sm5xx_bus *bus)
{
	down(&bus->control_regs_lock);
}

static inline void sm5xx_release_control_lock(struct sm5xx_bus *bus)
{
	up(&bus->control_regs_lock);
}

extern struct bus_type sm5xx_bus_type;
#endif /* __KERNEL__ */
#endif /*__SM5XX_H__*/
