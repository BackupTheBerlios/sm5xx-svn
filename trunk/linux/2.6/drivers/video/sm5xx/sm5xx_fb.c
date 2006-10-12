/*
 * drivers/video/sm5xx/sm5xx_fb.c
 *
 * DESCRIPTION:
 *  SM5xx kernel framebuffer driver
 *
 * AUTHOR:
 *  Andrey Volkov <avolkov@varma-el.com>
 *
 * COPYRIGHT:
 *  2005-2006, Varma Electronics Oy
 *
 *  Based on reference SiliconMotion driver v.0.01.
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

#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/interrupt.h>
#include <linux/fb.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/dma-mapping.h>
#include <linux/circ_buf.h>

#include <asm/io.h>
#include <asm/uaccess.h>

#include <mfd/sm5xx/sm5xx.h>
#include <mfd/sm5xx/sm5xx_ci_regs.h>
#include <mfd/sm5xx/sm5xx_fb_regs.h>
#include <mfd/sm5xx/sm5xx_fb.h>
#include "../../i2c/busses/i2c-sm5xx.h"

#define sm5xx_read32(regindex)	_sm5xx_read32((regindex), bus)
#define sm5xx_write32(regindex,val)	_sm5xx_write32((regindex), val, bus)

struct sm5xx_fb_devdata
{
	struct list_head fb_list;
};

#define arraysize(x)	(sizeof(x)/sizeof(*(x)))
#define to_smfb(fb_info_ptr)	((struct sm5xx_fb *)(fb_info_ptr->par));
#define to_fb(smfb_ptr)			(smfb_ptr->fb)

#define round_div(n, d) ((2*(n)+(d))/(2*(d)))

#define SM5XX_PANEL_NAME        "SM5xx Panel"
#define SM5XX_CRT_NAME          "SM5xx CRT"

static int noaccel __devinitdata;
static int hwcur __devinitdata = 1;
static int vram __devinitdata;
static int pll __devinitdata;
static int mclk __devinitdata;
static int xclk __devinitdata;
static int active_plane = PANEL /*CRT*/;
static int dual_head __devinitdata;
static int crt_clone __devinitdata;
static char *mode_option __devinitdata;

void (*sm5xx_fb_blank_helper)(int blank);
EXPORT_SYMBOL(sm5xx_fb_blank_helper);

/* default mode */
#if 0
//static char *mode_option = "640x480-8@60";
static struct fb_var_screeninfo __devinitdata sm5xx_fb_default_var = {
	.xres = 640,
	.yres = 480,
	.xres_virtual = 640,
	.yres_virtual = 960,

	.bits_per_pixel = 8,
	.red = {0, 8, 0},
	.green = {0, 8, 0},
	.blue = {0, 8, 0},
	.transp = {0, 0, 0},
	.height = -1,
	.width = -1,
	.pixclock = 40000,
	.left_margin = 42,
	.right_margin = 16,
	.hsync_len = 96,

	.upper_margin = 31,
	.lower_margin = 13,
	.vsync_len = 2,
	.vmode = FB_VMODE_NONINTERLACED
};
#else
//static char *mode_option = "1024x768-8@60";
static struct fb_var_screeninfo __devinitdata sm5xx_fb_default_var = {
	.xres = 1024,
	.yres = 768,
	.xres_virtual = 1024,
	.yres_virtual = 768,

	.bits_per_pixel = 8,
	.red = {0, 8, 0},
	.green = {0, 8, 0},
	.blue = {0, 8, 0},
	.transp = {0, 0, 0},
	.height = -1,
	.width = -1,
	.pixclock = 15384,
	.left_margin = 168,
	.right_margin = 8,
	.hsync_len = 144,

	.upper_margin = 29,
	.lower_margin = 3,
	.vsync_len = 6,
	.vmode = FB_VMODE_NONINTERLACED
};
#endif

static void set_ctrlr_state(struct sm5xx_fb *par, u_int state);

static void vsyncwait(struct sm5xx_fb *par, int counter)
{
  atomic_set(&par->vsync_count, counter);
  wait_event( par->vsync_wq, atomic_read(&par->vsync_count)==0);
}

static inline void sm5xx_fb_schedule_work(struct sm5xx_fb *par, u_int state)
{
	unsigned long flags;

	local_irq_save(flags);
	/*
	 * We need to handle two requests being made at the same time.
	 * There are two important cases:
	 *  1. When we are changing VT (C_REENABLE) while unblanking (C_ENABLE)
	 *     We must perform the unblanking, which will do our REENABLE for us.
	 *  2. When we are blanking, but immediately unblank before we have
	 *     blanked.  We do the "REENABLE" thing here as well, just to be sure.
	 */
	if (par->task_state == C_ENABLE && state == C_REENABLE)
		state = (u_int) -1;
	if (par->task_state == C_DISABLE && state == C_ENABLE)
		state = C_REENABLE;

	if (state != (u_int)-1) {
		par->task_state = state;
		schedule_work(&par->task);
	}
	local_irq_restore(flags);
}

static int sm5xx_fb_setcolreg(u_int regno, u_int red, u_int green, u_int blue,
				  u_int transp, struct fb_info *info)
{
	struct sm5xx_fb *par = to_smfb(info);
	struct sm5xx_bus *bus = to_sm5xx_bus( info->device );
	u32 val;

	/*
	 * If greyscale is true, then we convert the RGB value
	 * to greyscale no mater what visual we are using.
	 */
	if (info->var.grayscale) {
		/* gray = 0.30*R + 0.59*G + 0.11*B */
		red = green = blue = (19595 * red + 38470 * green +
					7471 * blue) >> 16;
	}

	switch (info->fix.visual) {
	case FB_VISUAL_TRUECOLOR:
		/*
		 * 16-bit or 32-bit True Colour.  We encode the RGB value
		 * according to the RGB bitfield information.
		 */
			if (regno >= 16)
				return -EINVAL;
			val  = CNVT_TOHW(red, info->var.red.length)<<info->var.red.offset;
			val |= CNVT_TOHW(green,info->var.green.length)<<
				   info->var.green.offset;
			val |= CNVT_TOHW(blue, info->var.blue.length)<<
				   info->var.blue.offset;
			((u32 *)info->pseudo_palette)[regno] = val;
		break;

	case FB_VISUAL_PSEUDOCOLOR:
		if (regno >= (1 << info->var.green.length))
			return -EINVAL;
		else {
			val  = ((red << 8) & 0xff0000);
			val |= ( green & 0x00ff00);
			val |= ((blue  >> 8) & 0x0000ff);
			sm5xx_write32(par->palette_reg + 4*regno,val);
		}
		break;
	default:
		return -EINVAL;
	}

	return 0;
}


/* Finds clock closest to the requested. And return it IN KHZ */
static ulong sm5xx_fb_find_clock(ulong pix_frq_khz, clock_select_t *clock,
								 display_t display)
{
	ulong mclk;
	int divider, shift;
	ulong best_diff = 999999;
	int max_divider = (display == PANEL) ? 5 : 3;

	pix_frq_khz *=2;

	mclk = 288000;
//	for (mclk = 192000UL; mclk <= 336000UL; mclk += 48000UL)
	{
		// For CRT, try dividers 1 and 3, for panel, try divider 5 as well.
		for (divider = 1; divider <= max_divider; divider += 2)
		{
			// Try all 8 shift values.
			for (shift = 0; shift < 8; shift++)
			{
				// Calculate difference with requested clock.
				long diff = labs(round_div(mclk, divider << shift)-pix_frq_khz);

				// If the difference is less than the current, use it.
				if (diff < best_diff)
				{
					// Store best difference.
					best_diff = diff;

					// Store clock values.
					clock->mclk = mclk;
					clock->divider = divider;
					clock->shift = shift;
					clock->pll2 =  (mclk != 288000);
				}
			}
		}
	}

	// Return best clock.
	pix_frq_khz = clock->mclk / (2*(clock->divider << clock->shift));
	return pix_frq_khz;
}

static irqreturn_t sm5xx_fb_crt_irq(struct sm5xx_bus *bus,
								u32 irq_mask, void *data, struct pt_regs *regs)
{
	struct sm5xx_fb *par = (struct sm5xx_fb *)data;

   if (atomic_read(&par->vsync_count)) {
	   if(atomic_dec_return(&par->vsync_count) == 0);
			wake_up_all(&par->vsync_wq);
   }
   sm5xx_write32(RAW_INT_STATUS, FINIT(RAW_INT_STATUS,CRT_VSYNC,CLEAR));
   return IRQ_HANDLED;
}


static irqreturn_t sm5xx_fb_panel_irq(struct sm5xx_bus *bus, u32 irq_mask,
									  void *data, struct pt_regs *regs)
{
	struct sm5xx_fb *par = (struct sm5xx_fb *)data;

   if (atomic_read(&par->vsync_count)) {
	   if(atomic_dec_return(&par->vsync_count) == 0);
			wake_up_all(&par->vsync_wq);
   }

   sm5xx_write32(RAW_INT_STATUS, FINIT(RAW_INT_STATUS,PANEL_VSYNC,CLEAR));
   return IRQ_HANDLED;
}

static int sm5xx_fb_check_var(struct fb_var_screeninfo *var,
							  struct fb_info *info)
{
	struct sm5xx_fb *par = to_smfb(info);
	int mode_valid = 0;
	int err = 0;

	var->transp.offset = 0;
	var->transp.length = 0;

	if (var->bits_per_pixel <= 8)
		var->bits_per_pixel = 8;
	else if (var->bits_per_pixel <= 16)
		var->bits_per_pixel = 16;
	else
		var->bits_per_pixel = 32;

	switch (var->bits_per_pixel) {
	case 8:
		var->red.offset = 0;
		var->red.length = 8;
		var->green.offset = 0;
		var->green.length = 8;
		var->blue.offset = 0;
		var->blue.length = 8;
		break;
	case 16:	   /* 16-bpp RGB 5:6:5 */
		var->red.length = 5;
		var->green.length = 6;
		var->blue.length = 5;

		var->blue.offset = 0;
		var->green.offset = 5;
		var->red.offset = 11;
		break;
	case 32:		/* RGB0888 */
		var->red.offset = 16;
//		var->red.offset = 0;
		var->red.length = 8;
		var->green.offset = 8;
		var->green.length = 8;
		var->blue.offset = 0;
//		var->blue.offset = 16;
		var->blue.length = 8;
		break;
	}
	var->transp.offset = 0;
	var->transp.length = 0;

	var->red.msb_right = 0;
	var->green.msb_right = 0;
	var->blue.msb_right = 0;
	var->transp.msb_right = 0;

	if (!info->monspecs.hfmax || !info->monspecs.vfmax ||
	    !info->monspecs.dclkmax || !fb_validate_mode(var, info))
		mode_valid = 1;

	/* calculate modeline if supported by monitor */
	if (!mode_valid && info->monspecs.gtf) {
		if (!fb_get_mode(FB_MAXTIMINGS, 0, var, info))
			mode_valid = 1;
	}

	if (!mode_valid) {
		struct fb_videomode *mode;

		mode = fb_find_best_mode(var, &info->modelist);
		if (mode) {
			fb_videomode_to_var(var, mode);
			mode_valid = 1;
		}
	}

	if (!mode_valid && info->monspecs.modedb_len)
		return -EINVAL;

	if (var->yres_virtual < var->yres)
		var->yres_virtual = var->yres;

	if (var->xres_virtual < var->xres)
		var->xres_virtual = var->xres;

	var->xres_virtual = (var->xres_virtual + SM5XX_ALIGN_MASK) &
						 ~SM5XX_ALIGN_MASK;

	ulong h_frq;
	ulong blank_width_old, blank_width_new;
	ulong frq;
	uint  htotal;

	blank_width_old = var->left_margin + var->right_margin + var->hsync_len;
	htotal = var->xres + blank_width_old;
//	vtotal = var->yres + var->upper_margin + var->lower_margin + var->vsync_len;

	h_frq = PICOS2KHZ(var->pixclock*htotal);

	frq = sm5xx_fb_find_clock( PICOS2KHZ(var->pixclock),
							   &par->curr_clock,par->fb_type);

	htotal = round_div(frq, h_frq);

	/*// Calculate the sync start and width based on the VESA percentages.*/
	blank_width_new = htotal - var->xres;

	var->pixclock = KHZ2PICOS(frq);

	var->right_margin = round_div( var->right_margin * blank_width_new,
								   blank_width_old);
	var->hsync_len = round_div( var->hsync_len * blank_width_new,
								blank_width_old);
	var->left_margin = 	blank_width_new - var->right_margin - var->hsync_len;

	return err;
}

/*
 * sm5xx_fb_set_par():
 *	Set the user defined part of the display for the specified console
 */
static int sm5xx_fb_set_par(struct fb_info *info)
{
	struct sm5xx_fb *par = to_smfb(info);
	struct fb_var_screeninfo *var = &info->var;

	if (var->bits_per_pixel == 8)
		info->fix.visual = FB_VISUAL_PSEUDOCOLOR;
	else
		info->fix.visual = FB_VISUAL_TRUECOLOR;

	info->fix.line_length = var->xres_virtual * var->bits_per_pixel / 8;
	par->palette_size = (var->bits_per_pixel == 8) ? 256 : var->bits_per_pixel;

	fb_set_cmap (&info->cmap, info);
	(par->set_regs)(par);

//	sm5xx_fb_schedule_work(par, C_REENABLE);

	return 0;
}

static int sm5xx_fb_pan_display(struct fb_var_screeninfo *var,
				struct fb_info *info)
{
	struct sm5xx_bus *bus = to_sm5xx_bus( info->device );
	struct sm5xx_fb *par = to_smfb(info);
	u32 offset, reg;

	offset = (var->yoffset * var->xres_virtual +
			 ((var->xoffset + SM5XX_ALIGN_MASK) & ~SM5XX_ALIGN_MASK)) *
		   	 ((var->bits_per_pixel+7) / 8);

	reg = FINIT(FB_ADDRESS, STATUS, PENDING) |
		  FINIT(FB_ADDRESS, EXT, LOCAL) |
		  FINIT_VAL(FB_ADDRESS, ADDRESS,
					 sm5xx_virt_to_offset(bus, par->fb->screen_base + offset));

	sm5xx_write32(par->fb_addr_reg, reg);
	if ( (var->activate & FB_ACTIVATE_VBL )) {
		sm5xx_unmask_irq(bus, FINIT(INT_MASK, PANEL_VSYNC, ENABLE));
		vsyncwait(par, 1);
		sm5xx_mask_irq(bus, FINIT(INT_MASK, PANEL_VSYNC, ENABLE));
	}
//	if ( (var->activate & FB_ACTIVATE_VBL ))
//		sm5xx_mask_irq(bus, FINIT(INT_MASK, PANEL_VSYNC, ENABLE));
	return 0;
}

static int sm5xx_fb_blank(int blank, struct fb_info *info)
{
	struct sm5xx_fb *par = to_smfb(info);

	DPRINTK("sm5xx_fb_blank: blank=%d\n", blank);

	switch (blank) {
	case VESA_POWERDOWN:
	case VESA_VSYNC_SUSPEND:
	case VESA_HSYNC_SUSPEND:
		sm5xx_fb_schedule_work(par, C_DISABLE);
		if (sm5xx_fb_blank_helper)
			sm5xx_fb_blank_helper(blank);
		break;

	case VESA_NO_BLANKING:
		if (sm5xx_fb_blank_helper)
			sm5xx_fb_blank_helper(blank);
		sm5xx_fb_schedule_work(par, C_ENABLE);
	}
	return 0;
}

static int sm5xx_fb_ioctl(struct inode *inode, struct file *file,
						  unsigned int cmd, unsigned long arg,
						  struct fb_info *info)
{
	struct sm5xx_memreq mreq;
	void *kaddr;
	struct sm5xx_bus *bus = to_sm5xx_bus( info->device );

	switch (cmd)
	{
	  case FBIO_ALLOC:
			copy_from_user(&mreq, (void *)arg, sizeof(mreq));
			kaddr = sm5xx_malloc(bus,mreq.size, SM5XX_ALIGN);

			if (kaddr == NULL) {
				return -ENOMEM;
			}
			else {
				mreq.paddr = sm5xx_virt_to_phys(bus,kaddr);
				copy_to_user((void *)arg, &mreq, sizeof(mreq));
			}
			break;

	  case FBIO_FREE:
	  		sm5xx_free(bus,sm5xx_phys_to_virt(bus,(unsigned long) arg));
			break;

	  default:
			return -EINVAL;
	}
	return 0;
}

static inline void __sm5xx_fb_backlight_power(struct sm5xx_fb *par, int on)
{
	DPRINTK("backlight o%s\n", on ? "n" : "ff");
/*
	if ((par->fb_type == PANEL) && hw.sm5xx_backlight_power)
		hw.sm5xx_backlight_power(on);
*/
}

static inline void __sm5xx_fb_lcd_power(struct sm5xx_fb *par, int on)
{
	DPRINTK("LCD power o%s\n", on ? "n" : "ff");
/*
	if ((par->fb_type == PANEL) && hw.sm5xx_lcd_power)
		hw.sm5xx_lcd_power(on);
*/
}

static void sm5xx_fb_enable_controller(struct sm5xx_fb *par)
{
	struct sm5xx_bus *bus = to_sm5xx_bus( par->fb->device );

	if (par->fb_type == PANEL) {
		u32 panel_ctrl = sm5xx_read32(PANEL_DISPLAY_CTRL);
		DPRINTK("Enabling LCD controller\n");

		panel_ctrl = FSET(panel_ctrl, PANEL_DISPLAY_CTRL, PLANE, ENABLE);
		sm5xx_write32(PANEL_DISPLAY_CTRL, panel_ctrl);
	} else  {
		u32 panel_ctrl = sm5xx_read32(CRT_DISPLAY_CTRL);
		DPRINTK("Enabling CRT controller\n");

		panel_ctrl = FSET(panel_ctrl, CRT_DISPLAY_CTRL, PLANE, ENABLE);
		panel_ctrl = FSET(panel_ctrl, CRT_DISPLAY_CTRL, BLANK, OFF);
		sm5xx_write32(CRT_DISPLAY_CTRL, panel_ctrl);
	}
}

static void sm5xx_fb_disable_controller(struct sm5xx_fb *par)
{
	struct sm5xx_bus *bus = to_sm5xx_bus( par->fb->device );
	u32 ctrl_reg = 0;
	DECLARE_WAITQUEUE(wait, current);

	add_wait_queue(&par->ctrlr_wait, &wait);
	set_current_state(TASK_UNINTERRUPTIBLE);

	if (par->fb_type == PANEL) {
		DPRINTK("Disabling LCD controller\n");
		ctrl_reg = sm5xx_read32(PANEL_DISPLAY_CTRL);
		ctrl_reg = FSET(ctrl_reg, PANEL_DISPLAY_CTRL, PLANE, DISABLE);
		sm5xx_write32(PANEL_DISPLAY_CTRL,ctrl_reg);
	} else {
		DPRINTK("Disabling CRT controller\n");
		ctrl_reg = sm5xx_read32(CRT_DISPLAY_CTRL);
		ctrl_reg = FSET(ctrl_reg, CRT_DISPLAY_CTRL, PLANE, DISABLE);
		ctrl_reg = FSET(ctrl_reg, CRT_DISPLAY_CTRL, BLANK, ON);
		sm5xx_write32(CRT_DISPLAY_CTRL,ctrl_reg);
	}

	schedule_timeout(20 * HZ / 1000);
	current->state = TASK_RUNNING;
	remove_wait_queue(&par->ctrlr_wait, &wait);
}

/*
 * This function must be called from task context only, since it will
 * sleep when disabling the LCD controller, or if we get two contending
 * processes trying to alter state.
 */
static void set_ctrlr_state(struct sm5xx_fb *par, u_int state)
{
	u_int old_state;

	down(&par->ctrlr_sem);

	old_state = par->state;

	switch (state) {
	case C_DISABLE_PM:
//		sm5xx_fb_save_fb_regs(par);
		/* fall through */
	case C_DISABLE:
		/*
		 * Disable controller
		 */
		if (old_state != C_DISABLE) {
			par->state = state;
			if (par->fb_type == PANEL) {
				__sm5xx_fb_backlight_power(par, 0);
				__sm5xx_fb_lcd_power(par, 0);
			}
			sm5xx_fb_disable_controller(par);
		}
		break;

	case C_REENABLE:
		/*
		 * Re-enable the controller only if it was already
		 * enabled.  This is so we reprogram the control
		 * registers.
		 */
		if (old_state == C_ENABLE) {
			sm5xx_fb_disable_controller(par);
			sm5xx_fb_enable_controller(par);
		}
		break;

	case C_ENABLE_PM:
		if (old_state != C_DISABLE_PM)
			break;
//		sm5xx_fb_restore_fb_regs(par);
		/* fall through */
	case C_ENABLE:
		/*
		 * Power up the LCD screen, enable controller, and
		 * turn on the backlight.
		 */
		if (old_state != C_ENABLE) {
			par->state = C_ENABLE;
			sm5xx_fb_enable_controller(par);
			if (par->fb_type == PANEL) {
				__sm5xx_fb_lcd_power(par, 1);
				__sm5xx_fb_backlight_power(par, 1);
			}
		}
		break;
	}
	up(&par->ctrlr_sem);
}

static void sm5xx_fb_task(void *dummy)
{
	struct sm5xx_fb *par = dummy;
	u32 state = xchg_u32(&par->task_state, (u32)-1);

	set_ctrlr_state(par, state);
}

static int sm5xx_fb_set_panel_regs(struct sm5xx_fb *par)
{
	struct sm5xx_bus *bus = to_sm5xx_bus( par->fb->device );
	struct fb_var_screeninfo *var = &par->fb->var;
	u32  htotal,vtotal, i;
	u32 pll;
	u32 reg;
	u32 misc_timing;

	sm5xx_write32(PANEL_DISPLAY_CTRL,
				  FSET(sm5xx_read32(PANEL_DISPLAY_CTRL),
				  	   PANEL_DISPLAY_CTRL,TIMING,DISABLE));

	misc_timing = sm5xx_read32(MISC_TIMING);
	reg = sm5xx_read32(CURRENT_POWER_CLOCK);

	pll = FGET(reg,CURRENT_POWER_CLOCK,P2XCLK_SELECT);

	reg &= FCLEAR(POWER_MODE_CLOCK, P2XCLK_SELECT) &
		   FCLEAR(POWER_MODE_CLOCK, P2XCLK_DIVIDER) &
		   FCLEAR(POWER_MODE_CLOCK, P2XCLK_SHIFT);

	if (par->curr_clock.pll2 != pll) {
		u32 plm = 7-par->curr_clock.mclk/48000;

	    misc_timing &= FCLEAR(MISC_TIMING, DIVIDER);
	    misc_timing |= FVAL(misc_timing, MISC_TIMING, DIVIDER, plm);
		sm5xx_write32(MISC_TIMING,misc_timing);

		reg |= FINIT_VAL(POWER_MODE_CLOCK, P2XCLK_SELECT, par->curr_clock.pll2);
		sm5xx_write32(POWER_MODE0_CLOCK,reg);
		mdelay(16);
	}
	reg |= FINIT_VAL(POWER_MODE_CLOCK, P2XCLK_DIVIDER,
					 par->curr_clock.divider >> 1) |
		   FINIT_VAL(POWER_MODE_CLOCK, P2XCLK_SHIFT, par->curr_clock.shift);
	sm5xx_write32(POWER_MODE0_CLOCK,reg);

	vtotal = var->yres + var->upper_margin + var->lower_margin + var->vsync_len;
	htotal = var->xres + var->left_margin + var->right_margin + var->hsync_len;

	sm5xx_write32(PANEL_PAN_CTRL, 0x00000000);
	sm5xx_write32(PANEL_COLOR_KEY, 0x00000000);

	sm5xx_write32(PANEL_PLANE_TL, 0x00000000);
	reg = FINIT_VAL(PANEL_PLANE_BR, BOTTOM, var->yres-1);
	reg |= FINIT_VAL(PANEL_PLANE_BR, RIGHT, var->xres-1);
	sm5xx_write32(PANEL_PLANE_BR, reg);

	i = var->xres*var->bits_per_pixel/8;
	reg = FINIT_VAL(PANEL_FB_WIDTH,WIDTH,i) |
		  FINIT_VAL(PANEL_FB_WIDTH,OFFSET,i);
	sm5xx_write32(PANEL_FB_WIDTH,reg);

	reg = FINIT_VAL(PANEL_WINDOW_WIDTH,WIDTH,var->xres);
	sm5xx_write32(PANEL_WINDOW_WIDTH,reg);

	reg = FINIT_VAL(PANEL_WINDOW_HEIGHT,HEIGHT,var->yres);
	sm5xx_write32(PANEL_WINDOW_HEIGHT,reg);

	reg = FINIT_VAL(PANEL_HORIZONTAL_TOTAL,TOTAL,htotal-1);
	reg |= FINIT_VAL(PANEL_HORIZONTAL_TOTAL,DISPLAY_END,var->xres-1);
	sm5xx_write32(PANEL_HORIZONTAL_TOTAL,reg);

	reg = FINIT_VAL(PANEL_HORIZONTAL_SYNC,WIDTH, var->hsync_len);
	reg |= FINIT_VAL(PANEL_HORIZONTAL_SYNC,START,var->xres+var->right_margin-1);
	sm5xx_write32(PANEL_HORIZONTAL_SYNC,reg);

	reg = FINIT_VAL(PANEL_VERTICAL_TOTAL, TOTAL, vtotal-1);
	reg |= FINIT_VAL(PANEL_VERTICAL_TOTAL, DISPLAY_END, var->yres-1);
	sm5xx_write32(PANEL_VERTICAL_TOTAL,reg);

	reg = FINIT_VAL(PANEL_VERTICAL_SYNC,HEIGHT, var->vsync_len);
	reg |= FINIT_VAL(PANEL_VERTICAL_SYNC,START,
					 var->yres + var->lower_margin-1);
	sm5xx_write32(PANEL_VERTICAL_SYNC,reg);

	reg = FINIT_VAL(PANEL_DISPLAY_CTRL,FORMAT,var->bits_per_pixel>>4);
/*	if(var->bits_per_pixel != 8)
		reg |= FINIT(PANEL_DISPLAY_CTRL,GAMMA,ENABLE);
*/
	if( !(var->sync & FB_SYNC_HOR_HIGH_ACT) )
		reg |= FINIT(PANEL_DISPLAY_CTRL, HSYNC_PHASE,ACTIVE_LOW);

	if( !(var->sync & FB_SYNC_VERT_HIGH_ACT) )
		reg |= FINIT(PANEL_DISPLAY_CTRL, VSYNC_PHASE,ACTIVE_LOW);

//Dirty HACK :||
	if(var->xres == 640)
		reg |= FINIT(PANEL_DISPLAY_CTRL, CLOCK_PHASE, ACTIVE_HIGH);
	else
		reg |= FINIT(PANEL_DISPLAY_CTRL, CLOCK_PHASE, ACTIVE_LOW);

	reg |= FINIT(PANEL_DISPLAY_CTRL, TIMING,ENABLE) |
		   FINIT(PANEL_DISPLAY_CTRL, PLANE, ENABLE) |
		   FINIT(PANEL_DISPLAY_CTRL, DITHER, ENABLE) |
		   FINIT(PANEL_DISPLAY_CTRL, TFT, 24) |
		   FINIT(PANEL_DISPLAY_CTRL, LCD, TFT) |
		   FINIT(PANEL_DISPLAY_CTRL, FPVDDEN, HIGH) |
		   FINIT(PANEL_DISPLAY_CTRL, FIFO,3);
	sm5xx_write32(PANEL_DISPLAY_CTRL,reg);

	sm5xx_unmask_irq(bus, FINIT(INT_MASK, PANEL_VSYNC, ENABLE));

	vsyncwait(par, 4);

	reg |= FINIT(PANEL_DISPLAY_CTRL, DATA, ENABLE);

	sm5xx_write32(PANEL_DISPLAY_CTRL,reg);
	vsyncwait(par, 4);

	reg |= FINIT(PANEL_DISPLAY_CTRL, VBIASEN, HIGH);
	sm5xx_write32(PANEL_DISPLAY_CTRL,reg);
	vsyncwait(par, 4);

	reg |= FINIT(PANEL_DISPLAY_CTRL, FPEN, HIGH);
	sm5xx_write32(PANEL_DISPLAY_CTRL,reg);
	vsyncwait(par, 4);

	sm5xx_mask_irq(bus, FINIT(INT_MASK, PANEL_VSYNC, ENABLE));
	return 0;
}

static int sm5xx_fb_set_crt_regs(struct sm5xx_fb *par)
{
	struct sm5xx_bus *bus = to_sm5xx_bus( par->fb->device );
	struct fb_var_screeninfo *var = &par->fb->var;
	u32  htotal,vtotal, i;
	u32 pll2;
	u32 reg;
	u32 misc_timing;

	sm5xx_write32(CRT_DISPLAY_CTRL,
				  FSET(sm5xx_read32(CRT_DISPLAY_CTRL),
				  	   CRT_DISPLAY_CTRL,TIMING,DISABLE));

	misc_timing = sm5xx_read32(MISC_TIMING);
	reg = sm5xx_read32(CURRENT_POWER_CLOCK);

	pll2 = FGET(reg,CURRENT_POWER_CLOCK,V2XCLK_SELECT);

	reg &= FCLEAR(CURRENT_POWER_CLOCK,V2XCLK_SELECT) &
		   FCLEAR(CURRENT_POWER_CLOCK,V2XCLK_DIVIDER) &
		   FCLEAR(CURRENT_POWER_CLOCK,V2XCLK_SHIFT);

	if (par->curr_clock.pll2 != pll2) {
		u32 plm = 7-par->curr_clock.mclk/48000;

	    misc_timing &= FCLEAR(MISC_TIMING, DIVIDER);
	    misc_timing |= FVAL(misc_timing, MISC_TIMING, DIVIDER, plm);
		sm5xx_write32(MISC_TIMING,misc_timing);

		reg |= FINIT_VAL(CURRENT_POWER_CLOCK, V2XCLK_SELECT,
						 par->curr_clock.pll2);
		sm5xx_write32(POWER_MODE0_CLOCK,reg);
		mdelay(16);
	}
	reg |= FINIT_VAL(CURRENT_POWER_CLOCK, V2XCLK_DIVIDER,
					 par->curr_clock.divider >> 1) |
		   FINIT_VAL(CURRENT_POWER_CLOCK, V2XCLK_SHIFT, par->curr_clock.shift);
	sm5xx_write32(POWER_MODE0_CLOCK,reg);

	vtotal = var->yres + var->upper_margin + var->lower_margin + var->vsync_len;
	htotal = var->xres + var->left_margin + var->right_margin + var->hsync_len;

	i = var->xres*var->bits_per_pixel/8;
	reg = FINIT_VAL(CRT_FB_WIDTH,WIDTH,i) | FINIT_VAL(CRT_FB_WIDTH,OFFSET,i);
	sm5xx_write32(CRT_FB_WIDTH,reg);

	reg = FINIT_VAL(CRT_HORIZONTAL_TOTAL,TOTAL,htotal-1);
	reg |= FINIT_VAL(CRT_HORIZONTAL_TOTAL,DISPLAY_END,var->xres-1);
	sm5xx_write32(CRT_HORIZONTAL_TOTAL,reg);

	reg = FINIT_VAL(CRT_HORIZONTAL_SYNC,WIDTH, var->hsync_len);
	reg |= FINIT_VAL(CRT_HORIZONTAL_SYNC,START,var->xres+var->right_margin-1);
	sm5xx_write32(CRT_HORIZONTAL_SYNC,reg);

	reg = FINIT_VAL(CRT_VERTICAL_TOTAL,TOTAL, vtotal-1);
	reg |= FINIT_VAL(CRT_VERTICAL_TOTAL,DISPLAY_END,var->yres-1);
	sm5xx_write32(CRT_VERTICAL_TOTAL,reg);

	reg = FINIT_VAL(CRT_VERTICAL_SYNC,HEIGHT, var->vsync_len);
	reg |= FINIT_VAL(CRT_VERTICAL_SYNC,START,var->yres + var->lower_margin-1);
	sm5xx_write32(CRT_VERTICAL_SYNC,reg);

	reg = FINIT_VAL(CRT_DISPLAY_CTRL,FORMAT,var->bits_per_pixel>>4);
/*	if(var->bits_per_pixel != 8)
		reg |= FINIT(CRT_DISPLAY_CTRL,GAMMA,ENABLE);
*/
	if( !(var->sync & FB_SYNC_HOR_HIGH_ACT) )
		reg |= FINIT(CRT_DISPLAY_CTRL,HSYNC_PHASE,ACTIVE_LOW);

	if( !(var->sync & FB_SYNC_VERT_HIGH_ACT) )
		reg |= FINIT(CRT_DISPLAY_CTRL,VSYNC_PHASE,ACTIVE_LOW);

	reg |= FINIT(CRT_DISPLAY_CTRL,SELECT,CRT) |
		   FINIT(CRT_DISPLAY_CTRL,TIMING,ENABLE) |
		   FINIT(CRT_DISPLAY_CTRL,CLOCK_PHASE,ACTIVE_LOW) |
		   FINIT(CRT_DISPLAY_CTRL,BLANK,ON) |
		   FINIT(CRT_DISPLAY_CTRL,PLANE, ENABLE) |
		   FINIT(CRT_DISPLAY_CTRL,FIFO,3);

	sm5xx_write32(CRT_DISPLAY_CTRL, reg);
	return 0;
}

/* Fake monspecs to fill in parnfo structure */
static struct fb_monspecs fake_monspecs __initdata = {
	/* Generic */
	.modedb		= NULL,
	.modedb_len	= 0,
	.hfmin		= 30000,
	.hfmax		= 70000,
	.vfmin		= 50,
	.vfmax		= 65,
	.dpms		= 0,
};

/*
 * Fill in fb_info
 */

static int __devinit sm5xx_fb_set_fbinfo(struct fb_info *info)
{
	struct fb_monspecs *specs = &info->monspecs;
	struct fb_videomode modedb;
	struct sm5xx_fb *par = info->par;
	struct sm5xx_bus *bus = to_sm5xx_bus( info->device );
	int ret;

	info->var = sm5xx_fb_default_var;
	par->state = C_DISABLE;

	fb_videomode_to_modelist(info->monspecs.modedb,
							 info->monspecs.modedb_len, &info->modelist);
	fb_var_to_videomode(&modedb, &info->var);

	if (specs->modedb != NULL) {
		/* get preferred timing */
		if (specs->misc & FB_MISC_1ST_DETAIL) {
			int i;
			for (i = 0; i < specs->modedb_len; i++) {
				if (specs->modedb[i].flag & FB_MODE_IS_FIRST) {
					modedb = specs->modedb[i];
					break;
				}
			}
		} else {
			/* otherwise, get first mode in database */
			modedb = specs->modedb[0];
		}

		fb_videomode_to_var(&info->var, &modedb);
		info->var.bits_per_pixel = 8;
	}

	if (mode_option)
		fb_find_mode(&info->var, info, mode_option,
					 specs->modedb, specs->modedb_len, &modedb, 8);

	info->fix.ypanstep	   = 1;
	info->fix.ywrapstep   = 0;

	info->fix.visual = (info->var.bits_per_pixel == 8) ?
						FB_VISUAL_PSEUDOCOLOR : FB_VISUAL_TRUECOLOR;
	info->pseudo_palette = par->pseudo_palette;
	fb_alloc_cmap(&info->cmap, 256, 0);

	fb_destroy_modedb(info->monspecs.modedb);
	info->monspecs.modedb = NULL;

	info->pixmap.addr = dma_alloc_coherent(info->device, 8 * 1024,
										  &par->h_pixmap, GFP_KERNEL | GFP_DMA);
	if (info->pixmap.addr == NULL)
		return -ENOMEM;

	info->pixmap.size = 8 * 1024;
	info->pixmap.scan_align = 4;
	info->pixmap.buf_align = 4;
	info->pixmap.access_align = 32;
	info->pixmap.flags = FB_PIXMAP_SYSTEM /*| FB_PIXMAP_SYNC*/;

	if (hwcur)
		sm5xx_fb_cursor_init(info);
	if(!noaccel)
		sm5xx_fb_accel_setup(info);

	info->var.accel_flags = (!noaccel);

	info->var.yres_virtual = 2*info->var.yres;


	ret = sm5xx_fb_check_var(&info->var, info);
	if (ret==0) {
		void * fb_address;
		void * old_addr;
		u32    size, reg;

/*		size = info->var.xres_virtual * info->var.yres_virtual * info->var.bits_per_pixel / 8;
*/
		size = 7<<20;
		fb_address = sm5xx_malloc(bus, size, PAGE_SIZE);
		if (!fb_address)
			return -ENOSPC;

		memset(fb_address, 2, size);

		/* virtual address */
		old_addr = info->screen_base;
		info->screen_base = fb_address;
		info->screen_size = size;
		/* physical address */
		info->fix.smem_start = sm5xx_virt_to_phys(bus, fb_address);
		info->fix.smem_len = size;
		info->fix.mmio_start = bus->reg_addr;
		info->fix.mmio_len = bus->regs_size;

		reg = FINIT(FB_ADDRESS, STATUS, PENDING) |
			  FINIT(FB_ADDRESS, EXT, LOCAL) |
			  FINIT_VAL(FB_ADDRESS, ADDRESS,
			  			sm5xx_virt_to_offset(bus, par->fb->screen_base));
		sm5xx_write32(par->fb_addr_reg, reg);

		if(old_addr)
			sm5xx_free(bus, old_addr);
	}

	return ret;
}

static char *sm5xx_names[] = {
   SM5XX_PANEL_NAME,
   SM5XX_CRT_NAME,
};

static struct fb_ops sm5xx_fb_ops = {
	.owner			= THIS_MODULE,
	.fb_check_var	= sm5xx_fb_check_var,
	.fb_set_par		= sm5xx_fb_set_par,
	.fb_setcolreg	= sm5xx_fb_setcolreg,
	.fb_fillrect	= cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_imageblit	= cfb_imageblit,
	.fb_pan_display = sm5xx_fb_pan_display,
	.fb_blank		= sm5xx_fb_blank,
	.fb_sync		= sm5xx_fb_sync,
	.fb_ioctl		= sm5xx_fb_ioctl,
};

static struct sm5xx_fb * __devinit sm5xx_fb_alloc_fb_info(struct sm5xx_dev *dev, char *name)
{
    struct sm5xx_fb *par;
	struct fb_info *fb;
	struct sm5xx_bus *bus = to_sm5xx_bus( dev );

	fb = framebuffer_alloc(sizeof(struct sm5xx_fb), &dev->dev);
    if (!fb)
        return NULL;

	par = fb->par;
	par->fb = fb;

	INIT_LIST_HEAD(&par->list);

    strncpy(fb->fix.id, name, sizeof(fb->fix.id)-1);

	fb->flags = FBINFO_DEFAULT |
			    FBINFO_HWACCEL_FILLRECT |
			    FBINFO_HWACCEL_COPYAREA |
			    FBINFO_HWACCEL_IMAGEBLIT;

	fb->fix.accel		= FB_ACCEL_SM5XX;
    fb->fix.type		= FB_TYPE_PACKED_PIXELS;
	fb->fix.mmio_start	= bus->reg_addr;
	fb->fix.mmio_len	= bus->regs_size;
    fb->fbops			= &sm5xx_fb_ops;
    fb->node			= -1;
	fb->monspecs		= fake_monspecs;

	init_waitqueue_head(&par->ctrlr_wait);
	INIT_WORK(&par->task, sm5xx_fb_task, par);
	init_MUTEX(&par->ctrlr_sem);

	init_waitqueue_head(&par->vsync_wq);

    return par;
}
static int sm5xx_setup_panel(struct sm5xx_fb *par)
{
	struct sm5xx_bus *bus = to_sm5xx_bus( par->fb->device );
	int ret = 0;
	u32 reg;

	par->fb_addr_reg = PANEL_FB_ADDRESS;
	par->palette_reg = PANEL_PALETTE_RAM;
	par->hw_cursor_reg_base = PANEL_HWC_ADDRESS;
	par->set_regs = sm5xx_fb_set_panel_regs;

	ret = sm5xx_request_irq(bus,
						    FINIT(INT_MASK, PANEL_VSYNC, ENABLE),
							sm5xx_fb_panel_irq, par, 0);
	if(ret)
		return ret;

	if ( sm5xx_get_bios_info(bus, &par->fb->var, par->fb_type) )
		par->flags |= SM5XX_FORCE_BIOS_SETTINGS;
	else  {
	 sm5xx_acquire_control_lock(bus);
		reg = sm5xx_read32(MISC_CTRL);
		reg = FSET(reg, MISC_CTRL, FPDATA, 18);
		sm5xx_write32(MISC_CTRL, reg);
	 sm5xx_release_control_lock(bus);
	}

	return ret;
}

static int sm5xx_setup_crt(struct sm5xx_fb *par)
{
	struct sm5xx_bus *bus = to_sm5xx_bus( par->fb->device );
	int ret;
	u32 reg;
#ifdef CONFIG_SM5XX_I2C
	struct sm5xx_dev *i2c_dev;
	u8 *edid = NULL;
#endif

	par->fb_addr_reg = CRT_FB_ADDRESS;
	par->palette_reg = CRT_PALETTE_RAM;
	par->hw_cursor_reg_base = CRT_HWC_ADDRESS;
	par->set_regs = sm5xx_fb_set_crt_regs;

	ret = sm5xx_request_irq(bus,
						    FINIT(INT_MASK, CRT_VSYNC, ENABLE),
							sm5xx_fb_crt_irq, par, 0);
	if(ret)
		return ret;

	if ( sm5xx_get_bios_info(&par->fb->var, par->fb_type) )
		par->flags |= SM5XX_FORCE_BIOS_SETTINGS;

   	sm5xx_acquire_control_lock(bus);
   		reg = sm5xx_read32(MISC_CTRL);
   		reg = FSET(reg, MISC_CTRL, DAC_POWER, ENABLE);
   		sm5xx_write32(MISC_CTRL, reg);
   	sm5xx_release_control_lock(bus);

	reg = FINIT(CRT_DISPLAY_CTRL,TIMING,ENABLE) |
		  FINIT(CRT_DISPLAY_CTRL,BLANK, ON)	  |
		  FINIT(CRT_DISPLAY_CTRL,SELECT, CRT);
	sm5xx_write32(CRT_DISPLAY_CTRL,reg);

#ifdef CONFIG_SM5XX_I2C
//	sm5xx_write32(CRT_MONITOR_DETECT, FINIT(CRT_MONITOR_DETECT,ENABLE,ENABLE));
//	reg = sm5xx_read32(CRT_MONITOR_DETECT);

	i2c_dev = sm5xx_find_child(bus, SM5XX_DEVID_I2C);
	if (i2c_dev) {
		sm5xx_probe_i2c_connector(i2c_dev, &edid);
		if (edid ) {
			printk(KERN_INFO "sm5xx-fb: EDID found from CRT\n");
			fb_parse_edid(edid, &par->fb->var);
			fb_edid_to_monspecs(edid, &par->fb->monspecs);
			kfree(edid);
		}
	}
#endif
	return 0;
}

static int (*sm5xx_display_setup[])(struct sm5xx_fb *par) = {
	sm5xx_setup_panel,
	sm5xx_setup_crt
};

int __init sm5xx_fb_probe(struct sm5xx_dev *dev)
{
	struct sm5xx_fb *par = NULL;
	struct sm5xx_bus *bus = to_sm5xx_bus( dev );
	struct sm5xx_fb_devdata *private;
	int ret;
	display_t cur_fb, fb_num;

	private = kmalloc(sizeof(struct sm5xx_fb_devdata),GFP_KERNEL);
	if(!private)
		return -ENOMEM;

	INIT_LIST_HEAD(&private->fb_list);
	sm5xx_set_drvdata(dev, private);

	sm5xx_write32(PANEL_DISPLAY_CTRL,0);
	sm5xx_write32(CRT_DISPLAY_CTRL,0);
	sm5xx_write32(ALPHA_DISPLAY_CTRL,0);
	sm5xx_write32(VIDEO_DISPLAY_CTRL,0);
	sm5xx_write32(VIDEO_ALPHA_DISPLAY_CTRL,0);

	sm5xx_write32(CRT_HWC_ADDRESS,0);
	sm5xx_write32(PANEL_HWC_ADDRESS,0);

	if (!noaccel && sm5xx_fb_accel_init(bus))
		noaccel = 1;

	sm5xx_enable_subsystem(bus,
						   FINIT(POWER_MODE_GATE, DISPLAY, ENABLE),
						   FINIT(POWER_MODE_GATE, DISPLAY, ENABLE));

	volatile int dbg=0;
	while(dbg)
		__asm("nop");

 	if (dual_head) {
		cur_fb = PANEL;
		fb_num = DYSPLAY_NUM-1;
 	}
	else  {
		cur_fb = active_plane;
		fb_num = active_plane;
	}

	for (; cur_fb <= fb_num; cur_fb++) 	{
		par = sm5xx_fb_alloc_fb_info(dev,sm5xx_names[cur_fb]);
		if (!par) {
			ret = -ENOMEM;
			goto failed;
		}
		par->fb_type = cur_fb;

		ret = sm5xx_display_setup[cur_fb](par);
		if (ret) {
			continue;
		}

		ret = sm5xx_fb_set_fbinfo(par->fb);
		if (ret) {
			continue;
		}

//		sm5xx_fb_save_fb_regs(par);

		set_ctrlr_state(par, C_ENABLE);

		ret = register_framebuffer(par->fb);
		if (ret) {
			continue;
		}
		list_add_tail(&par->list, &private->fb_list);
	}

	printk(KERN_INFO "fb%d: %s frame buffer device\n",
		    par->fb->node, par->fb->fix.id);

	return 0;

failed:
	printk(KERN_WARNING "%s: failed, error %d\n", __func__, ret);
	sm5xx_set_drvdata(dev, NULL);
	if (par)
		kfree(par);
	kfree(private);

	return ret;
}

#ifdef CONFIG_PM
/*
 * Power management hooks.  Note that we won't be called from IRQ context,
 * unlike the blank functions above, so we may sleep.
 */
static int sm5xx_fb_suspend(struct sm5xx_dev *dev, u32 state)
{
	struct sm5xx_fb *par = NULL;
	struct sm5xx_fb_devdata *private = sm5xx_get_drvdata(dev);
	struct list_head 	node;

	list_for_each( &node, &private->fb_list)  {
		set_ctrlr_state(list_entry(node, struct sm5xx_fb, list), C_DISABLE_PM);
	}
	return 0;
}

static int sm5xx_fb_resume(struct sm5xx_dev *dev)
{
	struct sm5xx_fb_devdata *private = sm5xx_get_drvdata(dev);
	struct sm5xx_fb *par = NULL;
	struct list_head 	node;

	list_for_each( &node, &private->fb_list)  {
		set_ctrlr_state(list_entry(node, struct sm5xx_fb, list), C_ENABLE_PM);
	}
	return 0;
}
#else
#define sm5xx_fb_suspend	NULL
#define sm5xx_fb_resume		NULL
#endif

#ifndef MODULE
static int __init sm5xx_fb_setup(char *options)
{
	char *this_opt;

	if (!options || !*options)
		return 0;

	while ((this_opt = strsep(&options, ",")) != NULL) {
		if (!strncmp(this_opt, "noaccel", 7)) {
			noaccel = 1;
		}
		else if (!strncmp(this_opt, "vram:", 5))
			vram = simple_strtoul(this_opt + 5, NULL, 0);
		else if (!strncmp(this_opt, "pll:", 4))
			pll = simple_strtoul(this_opt + 4, NULL, 0);
		else if (!strncmp(this_opt, "mclk:", 5))
			mclk = simple_strtoul(this_opt + 5, NULL, 0);
		else if (!strncmp(this_opt, "xclk:", 5))
			xclk = simple_strtoul(this_opt+5, NULL, 0);
		else if (!strncmp(this_opt, "active_plane:", 13))
			active_plane = simple_strtoul(this_opt+13, NULL, 0);
		else if (!strncmp(this_opt, "dual_head:", 12))
			dual_head = simple_strtoul(this_opt+12, NULL, 0);
		else if (!strncmp(this_opt, "crt_clone:", 12))
			crt_clone = simple_strtoul(this_opt+12, NULL, 0);
		else
			mode_option = this_opt;
	}
	return 0;
}
#endif  /*  !MODULE  */

static struct sm5xx_driver sm5xx_fb_driver = {
	.drv  = {
		.name	= "sm5xx-fb",
		.bus	= &sm5xx_bus_type,
	},
	.devid		= SM5XX_DEVID_FB,
	.probe		= sm5xx_fb_probe,
	.suspend	= sm5xx_fb_suspend,
	.resume		= sm5xx_fb_resume,
};

int __init sm5xx_fb_init (void)
{
#ifndef MODULE
    char *option = NULL;

    if (fb_get_options("sm5xx-fb", &option))
	return -ENODEV;
    sm5xx_fb_setup(option);
#endif

	return sm5xx_driver_register(&sm5xx_fb_driver);
}

void __exit sm5xx_fb_exit (void)
{
	sm5xx_driver_unregister(&sm5xx_fb_driver);
}

late_initcall(sm5xx_fb_init);  /* Because i2c must be inited
								  before sm5xx_fb_probe will be called*/
module_exit(sm5xx_fb_exit);

MODULE_AUTHOR("Andrey Volkov");
MODULE_DESCRIPTION("loadable framebuffer driver for SM5xx");
MODULE_LICENSE("GPL");

module_param(noaccel, bool, 0);
MODULE_PARM_DESC(noaccel, "bool: disable acceleration");
module_param(hwcur, int, 0);
MODULE_PARM_DESC(hwcur,
		 "Enables hardware cursor implementation. (0 or 1=enabled) "
		 "(default=0)");
module_param(vram, int, 0);
MODULE_PARM_DESC(vram, "Size of available memory in MB, KB or B "
						"(2,4,8,16,32,64MB, default=autodetect)");
module_param(pll, int, 0);
MODULE_PARM_DESC(pll, "int: override video clock");
module_param(mclk, int, 0);
MODULE_PARM_DESC(mclk, "int: override memory clock");
module_param(xclk, int, 0);
MODULE_PARM_DESC(xclk, "int: override accelerated engine clock");
module_param(active_plane, int, PANEL /*CRT */);
MODULE_PARM_DESC(active_plane, "Set active output: panel or crt (default=panel)");
module_param(dual_head, int, 0/*1*/);
MODULE_PARM_DESC(dual_head, "Dual Head support, enable CRT and PANEL output");
module_param(crt_clone, int, 0);
MODULE_PARM_DESC(crt_clone,
				 "Enable/disable routing of panel output to CRT monitor");
module_param(mode_option, charp, "1024x768-8@60");
MODULE_PARM_DESC(mode_option,
				"Specify resolution as \"<xres>x<yres>[-<bpp>][@<refresh>]\" ");

