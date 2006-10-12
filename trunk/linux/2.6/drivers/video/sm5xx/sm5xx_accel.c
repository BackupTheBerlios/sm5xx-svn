/*
 * drivers/video/sm5xx/sm5xx_accel.c
 *
 * DESCRIPTION:
 *  SM5xx acceleration routines
 *
 * AUTHOR:
 *  Andrey Volkov <avolkov@varma-el.com>
 *
 * COPYRIGHT:
 *  2005-2006, Varma Electronics Oy
 *
 * LICENCE:
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License. See the file COPYING in the main directory of this archive
 *  for more details.
 *
 */

#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/interrupt.h>
#include <linux/fb.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/pm.h>
#include <linux/dma-mapping.h>
#include <linux/circ_buf.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>

#include <asm/timex.h>

#include <mfd/sm5xx/sm5xx.h>
#include <mfd/sm5xx/sm5xx_ci_regs.h>
#include <mfd/sm5xx/sm5xx_fb_regs.h>
#include <mfd/sm5xx/sm5xx_de_regs.h>
#include "sm5xx_fb.h"

#define sm5xx_read32(regindex)	_sm5xx_read32((regindex), bus)
#define sm5xx_write32(regindex,val)	_sm5xx_write32((regindex), val, bus)
#define to_smfb(fb_info_ptr)	((struct sm5xx_fb *)(fb_info_ptr->par));

#define DE_CONTROL_RECT_FILL   (FINIT(DE_CONTROL, STATUS, START) | \
								FINIT(DE_CONTROL, DIRECTION, LEFT_TO_RIGHT) | \
								FINIT(DE_CONTROL, LAST_PIXEL, OFF) | \
								FINIT(DE_CONTROL, COMMAND, RECTANGLE_FILL) | \
								FINIT(DE_CONTROL, ROP_SELECT, ROP2))

#define	CI_WAIT_2D_MASK		(FMASK(CMD_INTPR_STATUS,2D_ENGINE)| \
					   		 FMASK(CMD_INTPR_STATUS,2D_SETUP) | \
					   		 FMASK(CMD_INTPR_STATUS,2D_FIFO) | \
					   		 FMASK(CMD_INTPR_STATUS,2D_MEMORY_FIFO))

#define	CI_WAIT_2D			(FINIT(CMD_INTPR_STATUS,2D_ENGINE,IDLE) | \
				  			 FINIT(CMD_INTPR_STATUS,2D_SETUP,IDLE) | \
				  			 FINIT(CMD_INTPR_STATUS,2D_FIFO,EMPTY) | \
				  			 FINIT(CMD_INTPR_STATUS,2D_MEMORY_FIFO,EMPTY))

#define	CI_WAIT_CSC_MASK	FMASK(CMD_INTPR_STATUS, CSC_STATUS)

#define	CI_WAIT_CSC			FINIT(CMD_INTPR_STATUS, CSC_STATUS, IDLE)

#define	CI_WAIT_CI_MASK		FMASK(CMD_INTPR_STATUS, COMMAND_FIFO)

#define	CI_WAIT_CI			FINIT(CMD_INTPR_STATUS, COMMAND_FIFO, EMPTY)

#define SM5XX_ROP_COPY		FINIT_VAL(DE_CONTROL, ROP, 0xc)
#define SM5XX_ROP_XOR		FINIT_VAL(DE_CONTROL, ROP, 0x6)

static irqreturn_t sm5xx_fb_accel_irq(struct sm5xx_bus *bus,
								u32 irq_mask, void *data, struct pt_regs *regs)
{
	/*TODO*/
	sm5xx_write32(DE_STATUS, FINIT(DE_STATUS,2D,CLEAR));
	return IRQ_HANDLED;
}

#define IDLE_MASK	(CI_WAIT_2D_MASK | CI_WAIT_CSC_MASK | CI_WAIT_CI_MASK)
#define IDLE_STATUS	(CI_WAIT_2D | CI_WAIT_CSC | CI_WAIT_CI)

static int wait_for_idle(struct sm5xx_bus *bus)
{
 u32 i;

 for( i = 0x10000;
 	  i > 0 && ((sm5xx_read32(CMD_INTPR_STATUS) & IDLE_MASK) != IDLE_STATUS);
 	  i--);

 if (i==0)
 	printk(KERN_ERR "LOCKUP !!!!\n");

 return i == 0;
}

int sm5xx_fb_sync(struct fb_info *info)
{
	struct sm5xx_bus *bus = to_sm5xx_bus( info->device );

	if (!info->var.accel_flags )
		return 0;

	return wait_for_idle(bus);
}

static void sm5xx_fb_start_fill_rect(struct fb_info *info, void *dst_base,
		const struct fb_fillrect *rect, int enable_irq)
{
	struct sm5xx_bus *bus = to_sm5xx_bus( info->device );
	u32 dst, reg;
	u32 pitch, window_width;
	u32 color = rect->color;

	if (dst_base == NULL) {
		dst = sm5xx_virt_to_offset(bus,info->screen_base);
		pitch = info->var.xres;
		window_width = info->var.xres;
		switch (info->var.bits_per_pixel) {
			case 16:
			case 32:
				color = ((u32 *) info->pseudo_palette)[rect->color];
				break;
		}
		reg = FINIT(DE_STRETCH_FORMAT, PATTERN_XY, NORMAL)|
			  FINIT_VAL(DE_STRETCH_FORMAT, PIXEL_FORMAT,
			 			info->var.bits_per_pixel>>4) 	 |
			  FINIT(DE_STRETCH_FORMAT, ADDRESSING, XY);

	}
	else {
		dst = sm5xx_virt_to_offset(bus, dst_base);
		pitch = rect->width;
		window_width = rect->width;
		reg = FINIT(DE_STRETCH_FORMAT, PATTERN_XY, NORMAL)|
			  FINIT(DE_STRETCH_FORMAT, PIXEL_FORMAT, 8)	 |
			  FINIT(DE_STRETCH_FORMAT, ADDRESSING, XY);
	}

#if 0
//	u32 *ptr = bus->ci_list.buf;
	u32 *ptr = bus->mem;

	wait_for_idle(bus);
	writel( 0x10000000 | DE_STRETCH_FORMAT,  ptr++ );
	writel( reg, ptr++);

	writel( 0x10000000 | DE_WINDOW_DESTINATION_BASE, ptr++ );
	writel( FINIT(DE_WINDOW_DESTINATION_BASE, EXT, LOCAL) |
		    FINIT_VAL(DE_WINDOW_DESTINATION_BASE, ADDRESS, dst), ptr++ );

	writel( 0x10000000 | DE_WINDOW_WIDTH, ptr++);
	writel( FINIT_VAL(DE_WINDOW_WIDTH, DESTINATION, window_width) , ptr++ );

	writel( 0x10000000 | DE_PITCH, ptr++);
	writel( FINIT_VAL(DE_PITCH, DESTINATION, pitch), ptr++);

	writel( 0x10000000 | DE_FOREGROUND, ptr++);
	writel( color, ptr++);

	writel( 0x10000000 | DE_DESTINATION, ptr++ );
	writel( FINIT(DE_DESTINATION, WRAP, DISABLE)	 |
		    FINIT_VAL(DE_DESTINATION, X, rect->dx)|
		    FINIT_VAL(DE_DESTINATION, Y, rect->dy), ptr++);

	writel( 0x10000000 | DE_DIMENSION, ptr++);
	writel( FINIT_VAL(DE_DIMENSION, X, rect->width) |
		   FINIT_VAL(DE_DIMENSION, Y_ET, rect->height), ptr++ );

	switch (rect->rop) {
		case ROP_XOR:
			reg = DE_CONTROL_RECT_FILL | FINIT_VAL(DE_CONTROL, ROP, 0x6);
			break;
		default:
		case ROP_COPY:
			reg = DE_CONTROL_RECT_FILL | FINIT_VAL(DE_CONTROL, ROP, 0xc);
			break;
	}
	writel( 0x10000000 | DE_STATUS, ptr++);
	writel( 0, ptr++);

	writel( 0x10000000 | DE_CONTROL, ptr++);
	writel( reg, ptr++);

	writel( 0x60000000 | CI_WAIT_2D_MASK, ptr++);
	writel( CI_WAIT_2D, ptr++);

	writel( 0x80000000, ptr++);
	writel( 0, ptr);

	reg = FINIT(CMD_INTPR_CTRL,STATUS,RUNNING) |
//		  CI_MEMORY_TYPE |
		  FINIT(CMD_INTPR_CTRL,EXT,LOCAL) |
		  FINIT_VAL(CMD_INTPR_CTRL,ADDRESS, 0);
//		  FINIT_VAL(CMD_INTPR_CTRL,ADDRESS, bus->h_ci_list);

	sm5xx_write32(CMD_INTPR_CTRL,reg);
	wait_for_idle(bus);

#else
	wait_for_idle(bus);
	/* Don't clear CSC ready, if it set */
	sm5xx_write32(DE_STATUS, FINIT(DE_STATUS,CSC,ACTIVE));

	sm5xx_write32( DE_STRETCH_FORMAT, reg);

	sm5xx_write32( DE_WINDOW_DESTINATION_BASE,
				   FINIT(DE_WINDOW_DESTINATION_BASE, EXT, LOCAL) |
				   FINIT_VAL(DE_WINDOW_DESTINATION_BASE, ADDRESS, dst));

	sm5xx_write32( DE_WINDOW_WIDTH,
				   FINIT_VAL(DE_WINDOW_WIDTH, DESTINATION, window_width));

	sm5xx_write32( DE_PITCH, FINIT_VAL(DE_PITCH, DESTINATION, pitch));

	sm5xx_write32( DE_FOREGROUND, color);

	sm5xx_write32( DE_DESTINATION, FINIT(DE_DESTINATION, WRAP, DISABLE) |
								   FINIT_VAL(DE_DESTINATION, X, rect->dx)|
								   FINIT_VAL(DE_DESTINATION, Y, rect->dy));

	sm5xx_write32( DE_DIMENSION, FINIT_VAL(DE_DIMENSION, X, rect->width) |
								 FINIT_VAL(DE_DIMENSION, Y_ET, rect->height));

	switch (rect->rop) {
		case ROP_XOR:
			reg = DE_CONTROL_RECT_FILL | SM5XX_ROP_XOR;
			break;
		default:
		case ROP_COPY:
			reg = DE_CONTROL_RECT_FILL | SM5XX_ROP_COPY;
			break;
	}
	sm5xx_write32(DE_CONTROL, reg );
#endif

}

static void
sm5xx_fb_fillrect(struct fb_info *info, const struct fb_fillrect *rect)
{
	if (info->state != FBINFO_STATE_RUNNING)
		return;
	sm5xx_fb_start_fill_rect(info, NULL, rect, 0);
}

static void
sm5xx_fb_copyarea(struct fb_info *info, const struct fb_copyarea *area)
{
	struct sm5xx_bus *bus = to_sm5xx_bus( info->device );
	u32 nDirection;
	u32 reg;
	u32 sx,sy;
	u32 dx,dy;

	if (info->state != FBINFO_STATE_RUNNING)
		return;

	sx = area->sx;
	sy = area->sy;
	dx = area->dx;
	dy = area->dy;

 /* Determine direction of operation */
	if (sy < dy)
		nDirection = FINIT(DE_CONTROL, DIRECTION, RIGHT_TO_LEFT);
	else if (sy > dy)
		nDirection = FINIT(DE_CONTROL, DIRECTION, LEFT_TO_RIGHT);
	else {
		if (sx <= dx)
			nDirection = FINIT(DE_CONTROL, DIRECTION, RIGHT_TO_LEFT);
		else
			nDirection = FINIT(DE_CONTROL, DIRECTION, LEFT_TO_RIGHT);
	}

	if (nDirection == FINIT(DE_CONTROL, DIRECTION, RIGHT_TO_LEFT)) {
		sx += area->width - 1;
		sy += area->height - 1;
		dx += area->width - 1;
		dy += area->height - 1;
	}

	wait_for_idle(bus);
	/* Don't clear CSC ready, if it set */
	sm5xx_write32(DE_STATUS, FINIT(DE_STATUS,CSC,ACTIVE));

	sm5xx_write32(DE_PITCH,
				  FINIT_VAL(DE_PITCH, DESTINATION, info->var.xres) |
				  FINIT_VAL(DE_PITCH, SOURCE, info->var.xres));

	sm5xx_write32(DE_WINDOW_WIDTH,
				  FINIT_VAL(DE_WINDOW_WIDTH, DESTINATION, info->var.xres) |
				  FINIT_VAL(DE_WINDOW_WIDTH, SOURCE, info->var.xres));

	sm5xx_write32(DE_WINDOW_SOURCE_BASE,
				  FINIT(DE_WINDOW_SOURCE_BASE, EXT, LOCAL) |
				  FINIT_VAL(DE_WINDOW_SOURCE_BASE, ADDRESS,
				  			sm5xx_virt_to_offset(bus,info->screen_base)));

	sm5xx_write32(DE_WINDOW_DESTINATION_BASE,
				  FINIT(DE_WINDOW_DESTINATION_BASE, EXT, LOCAL) |
				  FINIT_VAL(DE_WINDOW_DESTINATION_BASE, ADDRESS,
				  			sm5xx_virt_to_offset(bus,info->screen_base) ));

	sm5xx_write32(DE_SOURCE,
				  FINIT(DE_SOURCE, WRAP, DISABLE) |
				  FINIT_VAL(DE_SOURCE, X_K1, sx) |
				  FINIT_VAL(DE_SOURCE, Y_K2, sy));

	sm5xx_write32(DE_DESTINATION,
				  FINIT(DE_DESTINATION, WRAP, DISABLE) |
				  FINIT_VAL(DE_DESTINATION, X, dx) |
				  FINIT_VAL(DE_DESTINATION, Y, dy));

	sm5xx_write32(DE_DIMENSION,
				  FINIT_VAL(DE_DIMENSION, X, area->width) |
				  FINIT_VAL(DE_DIMENSION, Y_ET, area->height));

	reg = nDirection | SM5XX_ROP_COPY |
		  FINIT(DE_CONTROL, ROP_SELECT, ROP2) |
		  FINIT(DE_CONTROL, COMMAND, BITBLT) |
		  FINIT(DE_CONTROL, STATUS, START);
	sm5xx_write32(DE_CONTROL, reg);
}

static void
sm5xx_fb_mono_imageblit(struct fb_info *info, const struct fb_image *image)
{
	struct sm5xx_bus *bus = to_sm5xx_bus( info->device );
	u32 pitch, size;
	u32 de_ctrl, reg;
	u32 fgcolor, bgcolor;
	u32 i;

	if (info->fix.visual == FB_VISUAL_TRUECOLOR ||
		info->fix.visual == FB_VISUAL_DIRECTCOLOR) {
		fgcolor = ((u32*)(info->pseudo_palette))[image->fg_color];
		bgcolor = ((u32*)(info->pseudo_palette))[image->bg_color];
	} else {
		fgcolor = image->fg_color;
		bgcolor = image->bg_color;
	}

	pitch = (((image->width + 7) >> 3) + 3) & ~3;
	size = ((pitch * image->height + 3) & ~3)/4;

	wait_for_idle(bus);
	/* Don't clear CSC ready, if it set */
	sm5xx_write32(DE_STATUS, FINIT(DE_STATUS,CSC,ACTIVE));

/* dst */
	reg = FINIT(DE_WINDOW_DESTINATION_BASE, EXT, LOCAL) |
		  FINIT_VAL(DE_WINDOW_DESTINATION_BASE, ADDRESS,
					sm5xx_virt_to_offset(bus,info->screen_base));
	sm5xx_write32(DE_WINDOW_DESTINATION_BASE,reg);

	reg = FINIT(DE_DESTINATION, WRAP, DISABLE) |
		  FINIT_VAL(DE_DESTINATION, X, image->dx) |
		  FINIT_VAL(DE_DESTINATION, Y, image->dy);
	sm5xx_write32(DE_DESTINATION,reg);

/* ctrl */
	reg = FINIT(DE_STRETCH_FORMAT, PATTERN_XY, NORMAL) |
		  FINIT_VAL(DE_STRETCH_FORMAT, PIXEL_FORMAT,
		  			info->var.bits_per_pixel>>4) |
		  FINIT(DE_STRETCH_FORMAT, ADDRESSING, XY);
	sm5xx_write32(DE_STRETCH_FORMAT,reg);

	reg = FINIT_VAL(DE_PITCH, DESTINATION, info->var.xres) |
		  FINIT_VAL(DE_PITCH, SOURCE, info->var.xres);
	sm5xx_write32(DE_PITCH,reg);

	reg = FINIT_VAL(DE_WINDOW_WIDTH, DESTINATION, info->var.xres) |
		  FINIT_VAL(DE_WINDOW_WIDTH, SOURCE, info->var.xres);
	sm5xx_write32(DE_WINDOW_WIDTH,reg);

	reg = FINIT_VAL(DE_DIMENSION, X, image->width) |
		  FINIT_VAL(DE_DIMENSION, Y_ET, image->height);
	sm5xx_write32(DE_DIMENSION,reg);

	sm5xx_write32(DE_FOREGROUND, fgcolor);
	sm5xx_write32(DE_BACKGROUND, bgcolor);

	de_ctrl = SM5XX_ROP_COPY |
			  FINIT(DE_CONTROL, ROP_SELECT, ROP2) |
			  FINIT(DE_CONTROL, COMMAND, HOST_WRITE) |
			  FINIT(DE_CONTROL, HOST, MONO) |
			  FINIT(DE_CONTROL, MONO_DATA, NOT_PACKED) |
			  FINIT(DE_CONTROL, STATUS, START);
	sm5xx_write32(DE_CONTROL, de_ctrl);

	u32 *pfont = (u32 *)image->data;
	for (i = 0; i < size; i++)
		sm5xx_mem_writel( *pfont++, bus->regbase + DE_DATAPORT);

}

static void
sm5xx_fb_color_imageblit(struct fb_info *info, const struct fb_image *image)
{
	struct sm5xx_bus *bus = to_sm5xx_bus( info->device );
	ulong num_dwords, num_bytes;
	u32 de_ctrl, reg;
	u32 i;

	if (!image->width || !image->height)
		return;
	if ( info->var.bits_per_pixel != image->depth ) {
		cfb_imageblit(info, image);
		return;
	}

	num_bytes = image->height * (image->width*image->depth + 7)/8;
	num_dwords = (num_bytes+3)/4;

 /* Program 2D Drawing Engine */
	wait_for_idle(bus);
	/* Don't clear CSC ready, if it set */
	sm5xx_write32(DE_STATUS, FINIT(DE_STATUS,CSC,ACTIVE));

	sm5xx_write32(DE_WINDOW_DESTINATION_BASE,
				  FINIT(DE_WINDOW_DESTINATION_BASE, EXT, LOCAL) |
				  FINIT_VAL(DE_WINDOW_DESTINATION_BASE, ADDRESS,
				  			sm5xx_virt_to_offset(bus,info->screen_base)));

	sm5xx_write32(DE_DESTINATION,
				  FINIT(DE_DESTINATION, WRAP, DISABLE) |
				  FINIT_VAL(DE_DESTINATION, X, image->dx) |
				  FINIT_VAL(DE_DESTINATION, Y, image->dy));

	sm5xx_write32(DE_DIMENSION,
				  FINIT_VAL(DE_DIMENSION, X, image->width) |
				  FINIT_VAL(DE_DIMENSION, Y_ET, image->height));

	sm5xx_write32(DE_PITCH,
				  FINIT_VAL(DE_PITCH, DESTINATION, info->var.xres) |
				  FINIT_VAL(DE_PITCH, SOURCE, info->var.xres));

	sm5xx_write32(DE_WINDOW_WIDTH,
				  FINIT_VAL(DE_WINDOW_WIDTH, DESTINATION, info->var.xres) |
				  FINIT_VAL(DE_WINDOW_WIDTH, SOURCE, info->var.xres));

	reg = FINIT(DE_STRETCH_FORMAT, PATTERN_XY, NORMAL) |
		  FINIT_VAL(DE_STRETCH_FORMAT, PIXEL_FORMAT, image->depth>>4) |
		  FINIT(DE_STRETCH_FORMAT, ADDRESSING, XY);
	sm5xx_write32(DE_STRETCH_FORMAT,reg);

	de_ctrl = SM5XX_ROP_COPY | FINIT(DE_CONTROL, ROP_SELECT, ROP2) |
			  FINIT(DE_CONTROL, COMMAND, HOST_WRITE) |
			  FINIT(DE_CONTROL, STATUS, START);

	sm5xx_write32(DE_CONTROL, de_ctrl);

	u32 *pimage = (u32 *)image->data;
	for (i = 0; i < num_dwords; i++)
		sm5xx_mem_writel( *pimage++, bus->regbase + DE_DATAPORT);
}

static void
sm5xx_fb_imageblit(struct fb_info *info, const struct fb_image *image)
{
	if (info->state != FBINFO_STATE_RUNNING)
		return;

	if (image->depth == 1 )
		sm5xx_fb_mono_imageblit(info, image);
	else
		sm5xx_fb_color_imageblit(info, image);
}

/*
 * The hardware cursor definition requires 2 bits per pixel. The
 * Cursor size reguardless of the visible cursor size is 64 pixels
 * by 64 lines. The total memory required to define the cursor is
 * 16 bytes / line for 64 lines or 1024 bytes of data, which stored
 * in LITTLE ENDIAN order. The data must be in a contigiuos format.
 * The 2 bit cursor code values are
 * as follows:
 *
 *	00 - transparent (current display pixel)
 *	01 - pixel colour 1 = cursor background colour
 *	10 - pixel colour 2 = cursor foreground colour
 *	11 - pixel colour 3 = not used now (may be colour for cursor shadow?)
 *
 */
/* HW cursor parameters */
#define MAX_CURS			64
#define MAX_CURSOR_SIZEOF	(MAX_CURS*MAX_CURS/4)

static const u8 cursor_image_lookup[16] = {
	0x55, 0x56, 0x59, 0x5a, 0x65, 0x66, 0x69, 0x6a,
	0x95, 0x96, 0x99, 0x9a, 0xa5, 0xa6, 0xa9, 0xaa
};

static const u8 cursor_mask_lookup[16] = {
	0x00, 0x03,	0x0c, 0x0f,	0x30, 0x33,	0x3c, 0x3f,
	0xc0, 0xc3, 0xcc, 0xcf, 0xf0, 0xf3, 0xfc, 0xff
};

static int sm5xx_fb_cursor(struct fb_info *info, struct fb_cursor *cursor)
{
	struct sm5xx_fb *par = to_smfb(info);
	struct sm5xx_bus *bus = to_sm5xx_bus( info->device );
	u32 reg;
	u8 __iomem *dst = (u8 __iomem *)info->sprite.addr;
	int wait_for_clean = 0;

	if (info->state != FBINFO_STATE_RUNNING)
		return 0;

	if (cursor->image.width > MAX_CURS || cursor->image.height > MAX_CURS ||
		cursor->image.depth !=1)
		return soft_cursor(info, cursor);

	reg = sm5xx_read32(par->hw_cursor_reg_base + HWC_ADDRESS);
	sm5xx_write32(par->hw_cursor_reg_base + HWC_ADDRESS,
				 FSET(reg, HWC_ADDRESS, ENABLE, DISABLE));

	if (cursor->set & FB_CUR_SETSIZE) {
		struct fb_fillrect rect =  {
			.dx = 0,
			.dy = 0,
			.color = 0,
			.rop = ROP_COPY,
			.width = MAX_CURS/4,
			.height = MAX_CURS
		};
		sm5xx_fb_start_fill_rect(info, dst, &rect, 0);
		wait_for_clean = 1;
	}

	if (cursor->set & (FB_CUR_SETPOS | FB_CUR_SETHOT)) {
		int x,y;
		reg = 0;

		x = cursor->image.dx - cursor->hot.x - info->var.xoffset;
		y = cursor->image.dy - cursor->hot.y - info->var.yoffset;

		if (x<0) {
			reg |= FINIT(HWC_LOCATION,LEFT,OUTSIDE);
			x = -x;
		}

		if (y<0) {
			reg |= FINIT(HWC_LOCATION,TOP,OUTSIDE);
			y = -y;
		}

		reg |= FINIT_VAL(HWC_LOCATION,Y,y) | FINIT_VAL(HWC_LOCATION,X,x);
		sm5xx_write32(par->hw_cursor_reg_base + HWC_LOCATION, reg);
	}

	/* Set color map */
	if (cursor->set & FB_CUR_SETCMAP) {
		u32 fg_idx, bg_idx;
		u16 fg,bg;

		bg_idx = cursor->image.bg_color;
		fg_idx = cursor->image.fg_color;

		fg = RGB16(info->cmap.red[fg_idx],
				 info->cmap.green[fg_idx],
			 	 info->cmap.blue[fg_idx]);

		bg = RGB16(info->cmap.red[bg_idx],
				 info->cmap.green[bg_idx],
				 info->cmap.blue[bg_idx]);

		reg = FINIT_VAL(HWC_COLOR_12, 2_RGB565, fg) |
			  FINIT_VAL(HWC_COLOR_12, 1_RGB565, bg);
		sm5xx_write32(par->hw_cursor_reg_base + HWC_COLOR_12, reg);
	}

	if(wait_for_clean)
		wait_for_idle(bus);

	if (cursor->set & (FB_CUR_SETSHAPE | FB_CUR_SETIMAGE )) {
		u8 *src = (u8 *)cursor->image.data;
		u8 *msk = (u8 *)cursor->mask;
		u8 *ptr;
		uint i, j, offset;
		union {
			u32 l;
			u16 w;
			u8 b[4];
		} data;
		u8 m, b;
		uint width = (cursor->image.width + 7) >> 3;
		uint height = cursor->image.height;

		/* info->sprite.scan_align = SM501_ALIGN, width <= 8,
		   so next expression ALWAYS >=0 */
		offset = info->sprite.scan_align - width*2;

		for (i = 0; i < height; i++) {
			ptr = data.b;
			data.l = 0;
			for (j = 0; j < width; j++) {
				b = *src++;
				m = *msk++;
				if(cursor->rop == ROP_XOR)
					b ^= m;
				else
					b &= m;
				/* Lower 4 bits of mask */
				*ptr++ = cursor_mask_lookup[m & 0x0f ] &
						 cursor_image_lookup[b & 0x0f];
				/* Upper 4 bits of mask data */
				*ptr++ = cursor_mask_lookup[m >> 4 ] &
						 cursor_image_lookup[b >> 4];
				if (j & 1) {
					sm5xx_mem_writel(data.l, dst);
					dst+=4;
					ptr = data.b;
					data.l = 0;
				}
			}
			if (j & 1) {
				sm5xx_mem_writew(data.w, dst);
				dst+=2;
			}
			dst += offset;
		}
	}

	if (cursor->enable)
		sm5xx_write32(par->hw_cursor_reg_base + HWC_ADDRESS,
					 FSET(sm5xx_read32(par->hw_cursor_reg_base + HWC_ADDRESS),
					 HWC_ADDRESS,ENABLE,ENABLE));
	return 0;
}

int sm5xx_fb_cursor_init(struct fb_info *info)
{
	struct sm5xx_fb *par = to_smfb(info);
	struct sm5xx_bus *bus = to_sm5xx_bus( info->device );

	/* image always 64x64 pixels, and each pixel has 2bpp */
	info->sprite.addr = sm5xx_malloc(bus, MAX_CURSOR_SIZEOF, SM5XX_ALIGN);

	if (!info->sprite.addr)
		return -ENOMEM;

	sm5xx_write32(par->hw_cursor_reg_base + HWC_ADDRESS,
				  FINIT(HWC_ADDRESS,EXT,LOCAL) |
				  FINIT_VAL(HWC_ADDRESS,ADDRESS,
				  		 	sm5xx_virt_to_offset(bus,info->sprite.addr)));

	info->sprite.size = MAX_CURSOR_SIZEOF;
	info->sprite.scan_align = SM5XX_ALIGN;	/* Scratch pad 64 bytes wide */
	info->sprite.buf_align = SM5XX_ALIGN; 	/* and 64 lines tall. */
	info->sprite.flags = FB_PIXMAP_IO;
	info->fbops->fb_cursor = sm5xx_fb_cursor;

#if 0
	volatile int dbg = 1;
//	while(dbg)
//		__asm("nop");
	u32 stat,i, failed=0, tmp, old_failed = 0;
	volatile u32 *ptr;

	cycles_t delta = 0, delta1 = 0;

//	volatile int dbg = 1;
//	while(dbg)
//		__asm("nop");

	for (stat=0;stat<10000;stat++)
	{
		ptr = info->sprite.addr;
		cycles_t c1 = get_cycles();
		for(i=0;i<MAX_CURSOR_SIZEOF/4;i++)
			sm5xx_mem_writel(0x55555555, ptr++);
		cycles_t c2 = get_cycles();
		delta1 += c2-c1;

		c1 = get_cycles();
		struct fb_fillrect rect;
		rect.dx = 0;
		rect.dy = 0;
		rect.color = 0;
		rect.rop = ROP_COPY;
		rect.width = MAX_CURS/4;
		rect.height = MAX_CURS;
		sm5xx_fb_start_fill_rect(info, info->sprite.addr, &rect, 0);
		c2 = get_cycles();

		delta += c2-c1;
		ptr = info->sprite.addr;
		for (i=0;i<MAX_CURSOR_SIZEOF/4;i++) {
			tmp = sm5xx_mem_readl(ptr++);
			if (tmp) {
				int j;
				u32 *ci_dat = bus->mem;
				printk("stat = %d (%d), *ptr = %x \n",stat,stat-old_failed,tmp);
				sm5xx_dump_regs(bus);
/*				printk("CI data:");
				  for (j=0x0;j<0x60;j+=4) {
					if(j%16 == 0)
						printk("\n %08X:",j);
					printk(" %08X",readl(ci_dat++));
				  }
				printk("\n");
*/
				old_failed = stat;
				failed++;

				delta -= c2-c1;
				break;
			}
		}
	}
	printk("delta_mem_cpy = %d, delta = %d (faild = %d)\n",
			delta1/stat, delta/(stat-failed), failed);
	dbg = 1;
	while(dbg)
		__asm("nop");
#endif
	return 0;
}

void sm5xx_fb_accel_setup(struct fb_info *info)
{
	info->fbops->fb_fillrect  = sm5xx_fb_fillrect;
	info->fbops->fb_copyarea  = sm5xx_fb_copyarea;
	info->fbops->fb_imageblit = sm5xx_fb_imageblit;
}

int sm5xx_fb_accel_init(struct sm5xx_bus *bus)
{
	u32 reg;

	sm5xx_enable_subsystem(bus, FINIT(POWER_MODE_GATE,2D,ENABLE),
				 	 			FINIT(POWER_MODE_GATE,2D,ENABLE));

	for (reg = DE_SOURCE; reg<DE_STATUS; reg+=4)
		sm5xx_write32(reg,0);

	sm5xx_write32(DE_MASKS, FINIT_VAL(DE_MASKS, BYTE_MASK, 0xFFFF) |
				  			FINIT_VAL(DE_MASKS, BIT_MASK, 0xFFFF));

	return sm5xx_request_irq(bus, FINIT(INT_MASK,2D,ENABLE),
				 			 sm5xx_fb_accel_irq, NULL, 0);
}
