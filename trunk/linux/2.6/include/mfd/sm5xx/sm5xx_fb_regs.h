/*
 * mfd/sm5xx/sm5xx_fb_regs.h
 *
 * DESCRIPTION:
 *  Definitions of the DISPLAY (panel/CRT/cursor...) subsystem registers
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
#ifndef __SM5XX_FB_REGS_H__
#define __SM5XX_FB_REGS_H__

/* Panel Graphics Control */

#define PANEL_DISPLAY_CTRL                              0x080000
#define PANEL_DISPLAY_CTRL_FPEN                         27:27
#define PANEL_DISPLAY_CTRL_FPEN_LOW                     0
#define PANEL_DISPLAY_CTRL_FPEN_HIGH                    1
#define PANEL_DISPLAY_CTRL_VBIASEN                      26:26
#define PANEL_DISPLAY_CTRL_VBIASEN_LOW                  0
#define PANEL_DISPLAY_CTRL_VBIASEN_HIGH                 1
#define PANEL_DISPLAY_CTRL_DATA                         25:25
#define PANEL_DISPLAY_CTRL_DATA_DISABLE                 0
#define PANEL_DISPLAY_CTRL_DATA_ENABLE                  1
#define PANEL_DISPLAY_CTRL_FPVDDEN                      24:24
#define PANEL_DISPLAY_CTRL_FPVDDEN_LOW                  0
#define PANEL_DISPLAY_CTRL_FPVDDEN_HIGH                 1
#define PANEL_DISPLAY_CTRL_PATTERN                      23:23
#define PANEL_DISPLAY_CTRL_PATTERN_4                    0
#define PANEL_DISPLAY_CTRL_PATTERN_8                    1
#define PANEL_DISPLAY_CTRL_TFT                          22:21
#define PANEL_DISPLAY_CTRL_TFT_24                       0
#define PANEL_DISPLAY_CTRL_TFT_9                        1
#define PANEL_DISPLAY_CTRL_TFT_12                       2
#define PANEL_DISPLAY_CTRL_DITHER                       20:20
#define PANEL_DISPLAY_CTRL_DITHER_DISABLE               0
#define PANEL_DISPLAY_CTRL_DITHER_ENABLE                1
#define PANEL_DISPLAY_CTRL_LCD                          19:18
#define PANEL_DISPLAY_CTRL_LCD_TFT                      0
#define PANEL_DISPLAY_CTRL_LCD_STN_8                    2
#define PANEL_DISPLAY_CTRL_LCD_STN_12                   3
#define PANEL_DISPLAY_CTRL_FIFO                         17:16
#define PANEL_DISPLAY_CTRL_FIFO_1                       0
#define PANEL_DISPLAY_CTRL_FIFO_3                       1
#define PANEL_DISPLAY_CTRL_FIFO_7                       2
#define PANEL_DISPLAY_CTRL_FIFO_11                      3
#define PANEL_DISPLAY_CTRL_CLOCK_PHASE                  14:14
#define PANEL_DISPLAY_CTRL_CLOCK_PHASE_ACTIVE_HIGH      0
#define PANEL_DISPLAY_CTRL_CLOCK_PHASE_ACTIVE_LOW       1
#define PANEL_DISPLAY_CTRL_VSYNC_PHASE                  13:13
#define PANEL_DISPLAY_CTRL_VSYNC_PHASE_ACTIVE_HIGH      0
#define PANEL_DISPLAY_CTRL_VSYNC_PHASE_ACTIVE_LOW       1
#define PANEL_DISPLAY_CTRL_HSYNC_PHASE                  12:12
#define PANEL_DISPLAY_CTRL_HSYNC_PHASE_ACTIVE_HIGH      0
#define PANEL_DISPLAY_CTRL_HSYNC_PHASE_ACTIVE_LOW       1
#define PANEL_DISPLAY_CTRL_COLOR_KEY                    9:9
#define PANEL_DISPLAY_CTRL_COLOR_KEY_DISABLE            0
#define PANEL_DISPLAY_CTRL_COLOR_KEY_ENABLE             1
#define PANEL_DISPLAY_CTRL_TIMING                       8:8
#define PANEL_DISPLAY_CTRL_TIMING_DISABLE               0
#define PANEL_DISPLAY_CTRL_TIMING_ENABLE                1
#define PANEL_DISPLAY_CTRL_VERTICAL_PAN_DIR             7:7
#define PANEL_DISPLAY_CTRL_VERTICAL_PAN_DIR_DOWN        0
#define PANEL_DISPLAY_CTRL_VERTICAL_PAN_DIR_UP          1
#define PANEL_DISPLAY_CTRL_VERTICAL_PAN                 6:6
#define PANEL_DISPLAY_CTRL_VERTICAL_PAN_DISABLE         0
#define PANEL_DISPLAY_CTRL_VERTICAL_PAN_ENABLE          1
#define PANEL_DISPLAY_CTRL_HORIZONTAL_PAN_DIR           5:5
#define PANEL_DISPLAY_CTRL_HORIZONTAL_PAN_DIR_RIGHT     0
#define PANEL_DISPLAY_CTRL_HORIZONTAL_PAN_DIR_LEFT      1
#define PANEL_DISPLAY_CTRL_HORIZONTAL_PAN               4:4
#define PANEL_DISPLAY_CTRL_HORIZONTAL_PAN_DISABLE       0
#define PANEL_DISPLAY_CTRL_HORIZONTAL_PAN_ENABLE        1
#define PANEL_DISPLAY_CTRL_GAMMA                        3:3
#define PANEL_DISPLAY_CTRL_GAMMA_DISABLE                0
#define PANEL_DISPLAY_CTRL_GAMMA_ENABLE                 1
#define PANEL_DISPLAY_CTRL_PLANE                        2:2
#define PANEL_DISPLAY_CTRL_PLANE_DISABLE                0
#define PANEL_DISPLAY_CTRL_PLANE_ENABLE                 1
#define PANEL_DISPLAY_CTRL_FORMAT                       1:0
#define PANEL_DISPLAY_CTRL_FORMAT_8                     0
#define PANEL_DISPLAY_CTRL_FORMAT_16                    1
#define PANEL_DISPLAY_CTRL_FORMAT_32                    2

#define PANEL_PAN_CTRL                                  0x080004
#define PANEL_PAN_CTRL_VERTICAL_PAN                     31:24
#define PANEL_PAN_CTRL_VERTICAL_VSYNC                   21:16
#define PANEL_PAN_CTRL_HORIZONTAL_PAN                   15:8
#define PANEL_PAN_CTRL_HORIZONTAL_VSYNC                 5:0

#define PANEL_COLOR_KEY                                 0x080008
#define PANEL_COLOR_KEY_MASK                            31:16
#define PANEL_COLOR_KEY_VALUE                           15:0

#define PANEL_FB_ADDRESS                                0x08000C

#define PANEL_FB_WIDTH                                  0x080010
#define PANEL_FB_WIDTH_WIDTH                            29:16
#define PANEL_FB_WIDTH_OFFSET                           13:0

#define PANEL_WINDOW_WIDTH                              0x080014
#define PANEL_WINDOW_WIDTH_WIDTH                        27:16
#define PANEL_WINDOW_WIDTH_X                            11:0

#define PANEL_WINDOW_HEIGHT                             0x080018
#define PANEL_WINDOW_HEIGHT_HEIGHT                      27:16
#define PANEL_WINDOW_HEIGHT_Y                           11:0

#define PANEL_PLANE_TL                                  0x08001C
#define PANEL_PLANE_TL_TOP                              26:16
#define PANEL_PLANE_TL_LEFT                             10:0

#define PANEL_PLANE_BR                                  0x080020
#define PANEL_PLANE_BR_BOTTOM                           26:16
#define PANEL_PLANE_BR_RIGHT                            10:0

#define PANEL_HORIZONTAL_TOTAL                          0x080024
#define PANEL_HORIZONTAL_TOTAL_TOTAL                    27:16
#define PANEL_HORIZONTAL_TOTAL_DISPLAY_END              11:0

#define PANEL_HORIZONTAL_SYNC                           0x080028
#define PANEL_HORIZONTAL_SYNC_WIDTH                     23:16
#define PANEL_HORIZONTAL_SYNC_START                     11:0

#define PANEL_VERTICAL_TOTAL                            0x08002C
#define PANEL_VERTICAL_TOTAL_TOTAL                      26:16
#define PANEL_VERTICAL_TOTAL_DISPLAY_END                10:0

#define PANEL_VERTICAL_SYNC                             0x080030
#define PANEL_VERTICAL_SYNC_HEIGHT                      21:16
#define PANEL_VERTICAL_SYNC_START                       10:0

#define PANEL_CURRENT_LINE                              0x080034
#define PANEL_CURRENT_LINE_LINE                         10:0

#define NUM_PANEL_REGS	((PANEL_CURRENT_LINE-PANEL_DISPLAY_CTRL)/sizeof(u32))

/* Video Control */
/* FIXME: All VIDEO_xxx should be moved to sm5xx_video_regs.h */

#define VIDEO_DISPLAY_CTRL                              0x080040
#define VIDEO_DISPLAY_CTRL_FIFO                         17:16
#define VIDEO_DISPLAY_CTRL_FIFO_1                       0
#define VIDEO_DISPLAY_CTRL_FIFO_3                       1
#define VIDEO_DISPLAY_CTRL_FIFO_7                       2
#define VIDEO_DISPLAY_CTRL_FIFO_11                      3
#define VIDEO_DISPLAY_CTRL_BUFFER                       15:15
#define VIDEO_DISPLAY_CTRL_BUFFER_0                     0
#define VIDEO_DISPLAY_CTRL_BUFFER_1                     1
#define VIDEO_DISPLAY_CTRL_CAPTURE                      14:14
#define VIDEO_DISPLAY_CTRL_CAPTURE_DISABLE              0
#define VIDEO_DISPLAY_CTRL_CAPTURE_ENABLE               1
#define VIDEO_DISPLAY_CTRL_DOUBLE_BUFFER                13:13
#define VIDEO_DISPLAY_CTRL_DOUBLE_BUFFER_DISABLE        0
#define VIDEO_DISPLAY_CTRL_DOUBLE_BUFFER_ENABLE         1
#define VIDEO_DISPLAY_CTRL_BYTE_SWAP                    12:12
#define VIDEO_DISPLAY_CTRL_BYTE_SWAP_DISABLE            0
#define VIDEO_DISPLAY_CTRL_BYTE_SWAP_ENABLE             1
#define VIDEO_DISPLAY_CTRL_VERTICAL_SCALE               11:11
#define VIDEO_DISPLAY_CTRL_VERTICAL_SCALE_NORMAL        0
#define VIDEO_DISPLAY_CTRL_VERTICAL_SCALE_HALF          1
#define VIDEO_DISPLAY_CTRL_HORIZONTAL_SCALE             10:10
#define VIDEO_DISPLAY_CTRL_HORIZONTAL_SCALE_NORMAL      0
#define VIDEO_DISPLAY_CTRL_HORIZONTAL_SCALE_HALF        1
#define VIDEO_DISPLAY_CTRL_VERTICAL_MODE                9:9
#define VIDEO_DISPLAY_CTRL_VERTICAL_MODE_REPLICATE      0
#define VIDEO_DISPLAY_CTRL_VERTICAL_MODE_INTERPOLATE    1
#define VIDEO_DISPLAY_CTRL_HORIZONTAL_MODE              8:8
#define VIDEO_DISPLAY_CTRL_HORIZONTAL_MODE_REPLICATE    0
#define VIDEO_DISPLAY_CTRL_HORIZONTAL_MODE_INTERPOLATE  1
#define VIDEO_DISPLAY_CTRL_PIXEL                        7:4
#define VIDEO_DISPLAY_CTRL_GAMMA                        3:3
#define VIDEO_DISPLAY_CTRL_GAMMA_DISABLE                0
#define VIDEO_DISPLAY_CTRL_GAMMA_ENABLE                 1
#define VIDEO_DISPLAY_CTRL_PLANE                        2:2
#define VIDEO_DISPLAY_CTRL_PLANE_DISABLE                0
#define VIDEO_DISPLAY_CTRL_PLANE_ENABLE                 1
#define VIDEO_DISPLAY_CTRL_FORMAT                       1:0
#define VIDEO_DISPLAY_CTRL_FORMAT_8                     0
#define VIDEO_DISPLAY_CTRL_FORMAT_16                    1
#define VIDEO_DISPLAY_CTRL_FORMAT_32                    2
#define VIDEO_DISPLAY_CTRL_FORMAT_YUV                   3

#define VIDEO_FB_0_ADDRESS                              0x080044

#define VIDEO_FB_WIDTH                                  0x080048
#define VIDEO_FB_WIDTH_WIDTH                            29:16
#define VIDEO_FB_WIDTH_OFFSET                           13:0

#define VIDEO_FB_0_LAST_ADDRESS                         0x08004C
#define VIDEO_FB_0_LAST_ADDRESS_EXT                     27:27
#define VIDEO_FB_0_LAST_ADDRESS_EXT_LOCAL               0
#define VIDEO_FB_0_LAST_ADDRESS_EXT_EXTERNAL            1
#define VIDEO_FB_0_LAST_ADDRESS_CS                      26:26
#define VIDEO_FB_0_LAST_ADDRESS_CS_0                    0
#define VIDEO_FB_0_LAST_ADDRESS_CS_1                    1
#define VIDEO_FB_0_LAST_ADDRESS_ADDRESS                 25:0

#define VIDEO_PLANE_TL                                  0x080050
#define VIDEO_PLANE_TL_TOP                              26:16
#define VIDEO_PLANE_TL_LEFT                             13:0

#define VIDEO_PLANE_BR                                  0x080054
#define VIDEO_PLANE_BR_BOTTOM                           26:16
#define VIDEO_PLANE_BR_RIGHT                            13:0

#define VIDEO_SCALE                                     0x080058
#define VIDEO_SCALE_VERTICAL_MODE                       31:31
#define VIDEO_SCALE_VERTICAL_MODE_EXPAND                0
#define VIDEO_SCALE_VERTICAL_MODE_SHRINK                1
#define VIDEO_SCALE_VERTICAL_SCALE                      27:16
#define VIDEO_SCALE_HORIZONTAL_MODE                     15:15
#define VIDEO_SCALE_HORIZONTAL_MODE_EXPAND              0
#define VIDEO_SCALE_HORIZONTAL_MODE_SHRINK              1
#define VIDEO_SCALE_HORIZONTAL_SCALE                    11:0

#define VIDEO_INITIAL_SCALE                             0x08005C
#define VIDEO_INITIAL_SCALE_FB_1                        27:16
#define VIDEO_INITIAL_SCALE_FB_0                        11:0

#define VIDEO_YUV_CONSTANTS                             0x080060
#define VIDEO_YUV_CONSTANTS_Y                           31:24
#define VIDEO_YUV_CONSTANTS_R                           23:16
#define VIDEO_YUV_CONSTANTS_G                           15:8
#define VIDEO_YUV_CONSTANTS_B                           7:0

#define VIDEO_FB_1_ADDRESS                              0x080064
#define VIDEO_FB_1_ADDRESS_STATUS                       31:31
#define VIDEO_FB_1_ADDRESS_STATUS_CURRENT               0
#define VIDEO_FB_1_ADDRESS_STATUS_PENDING               1
#define VIDEO_FB_1_ADDRESS_EXT                          27:27
#define VIDEO_FB_1_ADDRESS_EXT_LOCAL                    0
#define VIDEO_FB_1_ADDRESS_EXT_EXTERNAL                 1
#define VIDEO_FB_1_ADDRESS_CS                           26:26
#define VIDEO_FB_1_ADDRESS_CS_0                         0
#define VIDEO_FB_1_ADDRESS_CS_1                         1
#define VIDEO_FB_1_ADDRESS_ADDRESS                      25:0

#define VIDEO_FB_1_LAST_ADDRESS                         0x080068
#define VIDEO_FB_1_LAST_ADDRESS_EXT                     27:27
#define VIDEO_FB_1_LAST_ADDRESS_EXT_LOCAL               0
#define VIDEO_FB_1_LAST_ADDRESS_EXT_EXTERNAL            1
#define VIDEO_FB_1_LAST_ADDRESS_CS                      26:26
#define VIDEO_FB_1_LAST_ADDRESS_CS_0                    0
#define VIDEO_FB_1_LAST_ADDRESS_CS_1                    1
#define VIDEO_FB_1_LAST_ADDRESS_ADDRESS                 25:0

/* Video Alpha Control */

#define VIDEO_ALPHA_DISPLAY_CTRL                        0x080080
#define VIDEO_ALPHA_DISPLAY_CTRL_SELECT                 28:28
#define VIDEO_ALPHA_DISPLAY_CTRL_SELECT_PER_PIXEL       0
#define VIDEO_ALPHA_DISPLAY_CTRL_SELECT_ALPHA           1
#define VIDEO_ALPHA_DISPLAY_CTRL_ALPHA                  27:24
#define VIDEO_ALPHA_DISPLAY_CTRL_FIFO                   17:16
#define VIDEO_ALPHA_DISPLAY_CTRL_FIFO_1                 0
#define VIDEO_ALPHA_DISPLAY_CTRL_FIFO_3                 1
#define VIDEO_ALPHA_DISPLAY_CTRL_FIFO_7                 2
#define VIDEO_ALPHA_DISPLAY_CTRL_FIFO_11                3
#define VIDEO_ALPHA_DISPLAY_CTRL_VERT_SCALE             11:11
#define VIDEO_ALPHA_DISPLAY_CTRL_VERT_SCALE_NORMAL      0
#define VIDEO_ALPHA_DISPLAY_CTRL_VERT_SCALE_HALF        1
#define VIDEO_ALPHA_DISPLAY_CTRL_HORZ_SCALE             10:10
#define VIDEO_ALPHA_DISPLAY_CTRL_HORZ_SCALE_NORMAL      0
#define VIDEO_ALPHA_DISPLAY_CTRL_HORZ_SCALE_HALF        1
#define VIDEO_ALPHA_DISPLAY_CTRL_VERT_MODE              9:9
#define VIDEO_ALPHA_DISPLAY_CTRL_VERT_MODE_REPLICATE    0
#define VIDEO_ALPHA_DISPLAY_CTRL_VERT_MODE_INTERPOLATE  1
#define VIDEO_ALPHA_DISPLAY_CTRL_HORZ_MODE              8:8
#define VIDEO_ALPHA_DISPLAY_CTRL_HORZ_MODE_REPLICATE    0
#define VIDEO_ALPHA_DISPLAY_CTRL_HORZ_MODE_INTERPOLATE  1
#define VIDEO_ALPHA_DISPLAY_CTRL_PIXEL                  7:4
#define VIDEO_ALPHA_DISPLAY_CTRL_CHROMA_KEY             3:3
#define VIDEO_ALPHA_DISPLAY_CTRL_CHROMA_KEY_DISABLE     0
#define VIDEO_ALPHA_DISPLAY_CTRL_CHROMA_KEY_ENABLE      1
#define VIDEO_ALPHA_DISPLAY_CTRL_PLANE                  2:2
#define VIDEO_ALPHA_DISPLAY_CTRL_PLANE_DISABLE          0
#define VIDEO_ALPHA_DISPLAY_CTRL_PLANE_ENABLE           1
#define VIDEO_ALPHA_DISPLAY_CTRL_FORMAT                 1:0
#define VIDEO_ALPHA_DISPLAY_CTRL_FORMAT_8               0
#define VIDEO_ALPHA_DISPLAY_CTRL_FORMAT_16              1
#define VIDEO_ALPHA_DISPLAY_CTRL_FORMAT_ALPHA_4_4       2
#define VIDEO_ALPHA_DISPLAY_CTRL_FORMAT_ALPHA_4_4_4_4   3

#define VIDEO_ALPHA_FB_ADDRESS                          0x080084

#define VIDEO_ALPHA_FB_WIDTH                            0x080088
#define VIDEO_ALPHA_FB_WIDTH_WIDTH                      29:16
#define VIDEO_ALPHA_FB_WIDTH_OFFSET                     13:0

#define VIDEO_ALPHA_FB_LAST_ADDRESS                     0x08008C
#define VIDEO_ALPHA_FB_LAST_ADDRESS_EXT                 27:27
#define VIDEO_ALPHA_FB_LAST_ADDRESS_EXT_LOCAL           0
#define VIDEO_ALPHA_FB_LAST_ADDRESS_EXT_EXTERNAL        1
#define VIDEO_ALPHA_FB_LAST_ADDRESS_CS                  26:26
#define VIDEO_ALPHA_FB_LAST_ADDRESS_CS_0                0
#define VIDEO_ALPHA_FB_LAST_ADDRESS_CS_1                1
#define VIDEO_ALPHA_FB_LAST_ADDRESS_ADDRESS             25:0

#define VIDEO_ALPHA_PLANE_TL                            0x080090
#define VIDEO_ALPHA_PLANE_TL_TOP                        26:16
#define VIDEO_ALPHA_PLANE_TL_LEFT                       10:0

#define VIDEO_ALPHA_PLANE_BR                            0x080094
#define VIDEO_ALPHA_PLANE_BR_BOTTOM                     26:16
#define VIDEO_ALPHA_PLANE_BR_RIGHT                      10:0

#define VIDEO_ALPHA_SCALE                               0x080098
#define VIDEO_ALPHA_SCALE_VERTICAL_MODE                 31:31
#define VIDEO_ALPHA_SCALE_VERTICAL_MODE_EXPAND          0
#define VIDEO_ALPHA_SCALE_VERTICAL_MODE_SHRINK          1
#define VIDEO_ALPHA_SCALE_VERTICAL_SCALE                27:16
#define VIDEO_ALPHA_SCALE_HORIZONTAL_MODE               15:15
#define VIDEO_ALPHA_SCALE_HORIZONTAL_MODE_EXPAND        0
#define VIDEO_ALPHA_SCALE_HORIZONTAL_MODE_SHRINK        1
#define VIDEO_ALPHA_SCALE_HORIZONTAL_SCALE              11:0

#define VIDEO_ALPHA_INITIAL_SCALE                       0x08009C
#define VIDEO_ALPHA_INITIAL_SCALE_FB                    11:0

#define VIDEO_ALPHA_CHROMA_KEY                          0x0800A0
#define VIDEO_ALPHA_CHROMA_KEY_MASK                     31:16
#define VIDEO_ALPHA_CHROMA_KEY_VALUE                    15:0

/* Panel Cursor Control */

#define PANEL_HWC_ADDRESS                               0x0800F0
#define PANEL_HWC_LOCATION                              0x0800F4
#define PANEL_HWC_COLOR_12                              0x0800F8
#define PANEL_HWC_COLOR_3                               0x0800FC

/* Alpha Control */

#define ALPHA_DISPLAY_CTRL                              0x080100
#define ALPHA_DISPLAY_CTRL_SELECT                       28:28
#define ALPHA_DISPLAY_CTRL_SELECT_PER_PIXEL             0
#define ALPHA_DISPLAY_CTRL_SELECT_ALPHA                 1
#define ALPHA_DISPLAY_CTRL_ALPHA                        27:24
#define ALPHA_DISPLAY_CTRL_FIFO                         17:16
#define ALPHA_DISPLAY_CTRL_FIFO_1                       0
#define ALPHA_DISPLAY_CTRL_FIFO_3                       1
#define ALPHA_DISPLAY_CTRL_FIFO_7                       2
#define ALPHA_DISPLAY_CTRL_FIFO_11                      3
#define ALPHA_DISPLAY_CTRL_PIXEL                        7:4
#define ALPHA_DISPLAY_CTRL_CHROMA_KEY                   3:3
#define ALPHA_DISPLAY_CTRL_CHROMA_KEY_DISABLE           0
#define ALPHA_DISPLAY_CTRL_CHROMA_KEY_ENABLE            1
#define ALPHA_DISPLAY_CTRL_PLANE                        2:2
#define ALPHA_DISPLAY_CTRL_PLANE_DISABLE                0
#define ALPHA_DISPLAY_CTRL_PLANE_ENABLE                 1
#define ALPHA_DISPLAY_CTRL_FORMAT                       1:0
#define ALPHA_DISPLAY_CTRL_FORMAT_16                    1
#define ALPHA_DISPLAY_CTRL_FORMAT_ALPHA_4_4             2
#define ALPHA_DISPLAY_CTRL_FORMAT_ALPHA_4_4_4_4         3

#define ALPHA_FB_ADDRESS                                0x080104

#define ALPHA_FB_WIDTH                                  0x080108
#define ALPHA_FB_WIDTH_WIDTH                            29:16
#define ALPHA_FB_WIDTH_OFFSET                           13:0

#define ALPHA_PLANE_TL                                  0x08010C
#define ALPHA_PLANE_TL_TOP                              26:16
#define ALPHA_PLANE_TL_LEFT                             10:0

#define ALPHA_PLANE_BR                                  0x080110
#define ALPHA_PLANE_BR_BOTTOM                           26:16
#define ALPHA_PLANE_BR_RIGHT                            10:0

#define ALPHA_CHROMA_KEY                                0x080114
#define ALPHA_CHROMA_KEY_MASK                           31:16
#define ALPHA_CHROMA_KEY_VALUE                          15:0

/* CRT Graphics Control */

#define CRT_DISPLAY_CTRL                                0x080200
#define CRT_DISPLAY_CTRL_FIFO                           17:16
#define CRT_DISPLAY_CTRL_FIFO_1                         0
#define CRT_DISPLAY_CTRL_FIFO_3                         1
#define CRT_DISPLAY_CTRL_FIFO_7                         2
#define CRT_DISPLAY_CTRL_FIFO_11                        3
#define CRT_DISPLAY_CTRL_TV_PHASE                       15:15
#define CRT_DISPLAY_CTRL_TV_PHASE_ACTIVE_HIGH           0
#define CRT_DISPLAY_CTRL_TV_PHASE_ACTIVE_LOW            1
#define CRT_DISPLAY_CTRL_CLOCK_PHASE                    14:14
#define CRT_DISPLAY_CTRL_CLOCK_PHASE_ACTIVE_HIGH        0
#define CRT_DISPLAY_CTRL_CLOCK_PHASE_ACTIVE_LOW         1
#define CRT_DISPLAY_CTRL_VSYNC_PHASE                    13:13
#define CRT_DISPLAY_CTRL_VSYNC_PHASE_ACTIVE_HIGH        0
#define CRT_DISPLAY_CTRL_VSYNC_PHASE_ACTIVE_LOW         1
#define CRT_DISPLAY_CTRL_HSYNC_PHASE                    12:12
#define CRT_DISPLAY_CTRL_HSYNC_PHASE_ACTIVE_HIGH        0
#define CRT_DISPLAY_CTRL_HSYNC_PHASE_ACTIVE_LOW         1
#define CRT_DISPLAY_CTRL_BLANK                          10:10
#define CRT_DISPLAY_CTRL_BLANK_OFF                      0
#define CRT_DISPLAY_CTRL_BLANK_ON                       1
#define CRT_DISPLAY_CTRL_SELECT                         9:9
#define CRT_DISPLAY_CTRL_SELECT_PANEL                   0
#define CRT_DISPLAY_CTRL_SELECT_CRT                     1
#define CRT_DISPLAY_CTRL_TIMING                         8:8
#define CRT_DISPLAY_CTRL_TIMING_DISABLE                 0
#define CRT_DISPLAY_CTRL_TIMING_ENABLE                  1
#define CRT_DISPLAY_CTRL_PIXEL                          7:4
#define CRT_DISPLAY_CTRL_GAMMA                          3:3
#define CRT_DISPLAY_CTRL_GAMMA_DISABLE                  0
#define CRT_DISPLAY_CTRL_GAMMA_ENABLE                   1
#define CRT_DISPLAY_CTRL_PLANE                          2:2
#define CRT_DISPLAY_CTRL_PLANE_DISABLE                  0
#define CRT_DISPLAY_CTRL_PLANE_ENABLE                   1
#define CRT_DISPLAY_CTRL_FORMAT                         1:0
#define CRT_DISPLAY_CTRL_FORMAT_8                       0
#define CRT_DISPLAY_CTRL_FORMAT_16                      1
#define CRT_DISPLAY_CTRL_FORMAT_32                      2

#define CRT_FB_ADDRESS                                  0x080204

#define CRT_FB_WIDTH                                    0x080208
#define CRT_FB_WIDTH_WIDTH                              29:16
#define CRT_FB_WIDTH_OFFSET                             13:0

#define CRT_HORIZONTAL_TOTAL                            0x08020C
#define CRT_HORIZONTAL_TOTAL_TOTAL                      27:16
#define CRT_HORIZONTAL_TOTAL_DISPLAY_END                11:0

#define CRT_HORIZONTAL_SYNC                             0x080210
#define CRT_HORIZONTAL_SYNC_WIDTH                       23:16
#define CRT_HORIZONTAL_SYNC_START                       11:0

#define CRT_VERTICAL_TOTAL                              0x080214
#define CRT_VERTICAL_TOTAL_TOTAL                        26:16
#define CRT_VERTICAL_TOTAL_DISPLAY_END                  10:0

#define CRT_VERTICAL_SYNC                               0x080218
#define CRT_VERTICAL_SYNC_HEIGHT                        21:16
#define CRT_VERTICAL_SYNC_START                         10:0

#define CRT_SIGNATURE_ANALYZER                          0x08021C
#define CRT_SIGNATURE_ANALYZER_STATUS                   31:16
#define CRT_SIGNATURE_ANALYZER_ENABLE                   3:3
#define CRT_SIGNATURE_ANALYZER_ENABLE_DISABLE           0
#define CRT_SIGNATURE_ANALYZER_ENABLE_ENABLE            1
#define CRT_SIGNATURE_ANALYZER_RESET                    2:2
#define CRT_SIGNATURE_ANALYZER_RESET_NORMAL             0
#define CRT_SIGNATURE_ANALYZER_RESET_RESET              1
#define CRT_SIGNATURE_ANALYZER_SOURCE                   1:0
#define CRT_SIGNATURE_ANALYZER_SOURCE_RED               0
#define CRT_SIGNATURE_ANALYZER_SOURCE_GREEN             1
#define CRT_SIGNATURE_ANALYZER_SOURCE_BLUE              2

#define CRT_CURRENT_LINE                                0x080220
#define CRT_CURRENT_LINE_LINE                           10:0

#define CRT_MONITOR_DETECT                              0x080224
#define CRT_MONITOR_DETECT_ENABLE                       24:24
#define CRT_MONITOR_DETECT_ENABLE_DISABLE               0
#define CRT_MONITOR_DETECT_ENABLE_ENABLE                1
#define CRT_MONITOR_DETECT_RED                          23:16
#define CRT_MONITOR_DETECT_GREEN                        15:8
#define CRT_MONITOR_DETECT_BLUE                         7:0

#define NUM_CRT_REGS	((CRT_MONITOR_DETECT-CRT_DISPLAY_CTRL)/sizeof(u32))

/* CRT Cursor Control */

#define CRT_HWC_ADDRESS 							0x080230
#define CRT_HWC_LOCATION							0x080234
#define CRT_HWC_COLOR_12							0x080238
#define CRT_HWC_COLOR_3 							0x08023C

/* Common for Panel/CRT/... defs */

#define FB_ADDRESS_STATUS                         31:31
#define FB_ADDRESS_STATUS_CURRENT                 0
#define FB_ADDRESS_STATUS_PENDING                 1
#define FB_ADDRESS_EXT                            27:27
#define FB_ADDRESS_EXT_LOCAL                      0
#define FB_ADDRESS_EXT_EXTERNAL                   1
#define FB_ADDRESS_CS                             26:26
#define FB_ADDRESS_CS_0                           0
#define FB_ADDRESS_CS_1                           1
#define FB_ADDRESS_ADDRESS                        25:0

#define HWC_ADDRESS				    				0x0
#define HWC_ADDRESS_ENABLE                          31:31
#define HWC_ADDRESS_ENABLE_DISABLE                  0
#define HWC_ADDRESS_ENABLE_ENABLE                   1
#define HWC_ADDRESS_EXT                             27:27
#define HWC_ADDRESS_EXT_LOCAL                       0
#define HWC_ADDRESS_EXT_EXTERNAL                    1
#define HWC_ADDRESS_CS                              26:26
#define HWC_ADDRESS_CS_0                            0
#define HWC_ADDRESS_CS_1                            1
#define HWC_ADDRESS_ADDRESS                         25:0

#define HWC_LOCATION		    		    0x4
#define HWC_LOCATION_TOP                            27:27
#define HWC_LOCATION_TOP_INSIDE                     0
#define HWC_LOCATION_TOP_OUTSIDE                    1
#define HWC_LOCATION_Y                              26:16
#define HWC_LOCATION_LEFT                           11:11
#define HWC_LOCATION_LEFT_INSIDE                    0
#define HWC_LOCATION_LEFT_OUTSIDE                   1
#define HWC_LOCATION_X                              10:0

#define HWC_COLOR_12		    		    0x8
#define HWC_COLOR_12_2_RGB565                       31:16
#define HWC_COLOR_12_1_RGB565                       15:0

#define HWC_COLOR_3                                 0xC
#define HWC_COLOR_3_RGB565                          15:0

#define PANEL_PALETTE_RAM                           0x080400
#define VIDEO_PALETTE_RAM                           0x080800
#define CRT_PALETTE_RAM                             0x080C00

/* Next two pallettes are 16 bits per entry each */
#define VIDEO_ALPHA_PALETTE_RAM                     0x0800A4
#define ALPHA_PALETTE_RAM                           0x080118

// Power constants to use with setDPMS function.
typedef enum _DPMS_t
{
	DPMS_ON,
	DPMS_STANDBY,
	DPMS_SUSPEND,
	DPMS_OFF
}
DPMS_t;

#endif /* __SM5XX_FB_REGS_H__ */

