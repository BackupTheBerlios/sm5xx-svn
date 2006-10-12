/*
 *  drivers/video/sm5xx/sm5xx_fb.h
 *
 * DESCRIPTION:
 *	Silicon Motion SM5XX Framebuffer definitions
 *
 * AUTHOR:
 *  Andrey Volkov <avolkov@varma-el.com>
 *
 * COPYRIGHT:
 *  2005-2006, Varma Electronics Oy
 *
 *  Based on:
 *  Copyright (C) 2004 Robert Whaley, Applied Data Systems
 *  Copyright (C) 2004 Silicon Motion, Inc.
 *   John Huang, johuang@siliconmotion.com
 *   Ge Wang, gewang@siliconmotion.com
 *
 * LICENCE:
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive
 *  for more details.
 *
*/

#ifndef __SM5XX_FB_H__
#define __SM5XX_FB_H__

#include <linux/list.h>

#define RGB565_R_MASK		0xF8  /* Mask for red color */
#define RGB565_G_MASK		0xFC  /* Mask for green color */
#define RGB565_B_MASK		0xF8  /* Mask for blue color */

#define RGB565_R_SHIFT      8     /* Number of bits to shift for red color */
#define RGB565_G_SHIFT      3     /* Number of bits to shift for green color */
#define RGB565_B_SHIFT      3     /* Number of bits to shift for blue color */

#define RGB16(r, g, b) \
( \
    (unsigned short) ((((r) & RGB565_R_MASK) << RGB565_R_SHIFT) | \
                      (((g) & RGB565_G_MASK) << RGB565_G_SHIFT) | \
                      (((b) & RGB565_B_MASK) >> RGB565_B_SHIFT)) \
)

#define CNVT_TOHW(val,width) ((((val)<<(width))+0x7FFF-(val))>>16)

/* Display type constants to use with setMode function and others. */
typedef enum _display
{
	PANEL = 0,
	CRT = 1,
	DYSPLAY_NUM
} display_t;

/* Clock value structure. */
typedef struct clock_select
{
	ulong mclk;
	u32 divider;
	u32 shift;
	u32 pll2;
} clock_select_t;

/*
 * These are the actions for set_ctrlr_state
 */
enum {
	C_DISABLE,
	C_ENABLE,
	C_DISABLE_CLKCHANGE,
	C_ENABLE_CLKCHANGE,
	C_REENABLE,
	C_DISABLE_PM,
	C_ENABLE_PM,
	C_STARTUP,
};

/*
 *  Debug macros
 */
#if DEBUG
#  define DPRINTK(fmt, args...)	printk("%s: " fmt, __FUNCTION__ , ## args)
#else
#  define DPRINTK(fmt, args...)
#endif

/*
 * Minimum X and Y resolutions
 */
#define MIN_XRES	64
#define MIN_YRES	64

#define SM5XX_SET_FB_ADDRESS _IOWR('F', 0x19, u_long)

/* sm5xx_fb.flags */
#define SM5XX_FORCE_BIOS_SETTINGS	(1<<0)

struct sm5xx_fb
{
	struct list_head 	list;
	u32					flags;
	u_int               fb_type;
	struct				fb_info	*fb;

	void   *			fb_virt;	/* virtual address of frame buffer */
	clock_select_t		curr_clock; /* filled in sm501fb_check_var */

	u32    				fb_addr_reg;
	u32    				palette_reg;
	u32					palette_size;
	u32					hw_cursor_reg_base; /* PANEL_HWC_ADDRESS or */
											/*	CRT_HWC_ADDRESS */
	volatile u32		state;
	volatile u32		task_state;
	struct semaphore	ctrlr_sem;
	wait_queue_head_t	ctrlr_wait;
	struct work_struct	task;

	int (*set_regs)(struct sm5xx_fb *par);

	dma_addr_t 			h_pixmap;
	u32 				pseudo_palette[16];

	wait_queue_head_t	vsync_wq;
	atomic_t			vsync_count;
};

/* in sm5xx_accel.c */
extern int sm5xx_fb_sync(struct fb_info *info);
extern int sm5xx_fb_accel_init(struct sm5xx_bus *bus);
extern void sm5xx_fb_accel_setup(struct fb_info *info);
extern int sm5xx_fb_cursor_init(struct fb_info *info);

#endif /*__SM5XX_FB_H__*/
