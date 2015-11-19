/*
 * drivers/media/video/mt9v113.h
 *
 * Copyright (C) 2008 Texas Instruments Inc
 * Author: Vaibhav Hiremath <hvaibhav@ti.com>
 *
 * Contributors:
 *     Sivaraj R <sivaraj@ti.com>
 *     Brijesh R Jadav <brijesh.j@ti.com>
 *     Hardik Shah <hardik.shah@ti.com>
 *     Manjunath Hadli <mrh@ti.com>
 *     Karicheri Muralidharan <m-karicheri2@ti.com>
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef _OV5640_H
#define _OV5640_H

#include <media/v4l2-subdev.h>
#include <media/media-entity.h>

/*
 * Other macros
 */
#define OV5640_MODULE_NAME		"ov5640"

/* Number of pixels and number of lines per frame for different standards */
#define VGA_NUM_ACTIVE_PIXELS		640
#define VGA_NUM_ACTIVE_LINES		480

struct ov5640_platform_data {
	int (*s_power)(struct v4l2_subdev *subdev, int on);
	int (*s_suspend)(struct v4l2_subdev *subdev);
	int (*s_resume)(struct v4l2_subdev *subdev);
	void (*s_shutdown)(struct v4l2_subdev *subdev);
	int (*set_xclk)(struct v4l2_subdev *subdev, int hz);
	int (*configure_interface)(struct v4l2_subdev *subdev, u32 pixclk);
};

/*i2c adress for MT9V113*/
#define OV5640_I2C_ADDR		(0x78 >> 1)

#define I2C_ONE_BYTE_TRANSFER		(1)
#define I2C_TWO_BYTE_TRANSFER		(2)
#define I2C_THREE_BYTE_TRANSFER		(3)
#define I2C_FOUR_BYTE_TRANSFER		(4)
#define I2C_TXRX_DATA_MASK		(0x00FF)
#define I2C_TXRX_DATA_MASK_UPPER	(0xFF00)
#define I2C_TXRX_DATA_SHIFT		(8)

#define OV5640_CLK_MAX		(54000000) /* 54MHz */
#define OV5640_CLK_MIN		(6000000)  /* 6Mhz */

/*Addresses and commands of AF Module*/
#define OV5640_AF_REG_ACK		0x3023
#define OV5640_AF_REG_MAIN		0x3022
#define OV5640_AF_REG_PARA4		0x3028
#define OV5640_AF_REG_FW_STAT	0x3029

#define OV5640_AF_SINGLE_AF		0x03
#define OV5640_AF_CONT_AF		0x04
#define OV5640_AF_PAUSE_AF		0x06
#define OV5640_AF_RELEASE_AF	0x08

#define OV5640_AF_FW_IDLE		0x70
#define OV5640_AF_IDLE_TRIES	200
#define OV5640_AF_ACK_TRIES		1000

enum Operations {
	PREVIEW_OPERATION,
	CAPTURE_OPERATION
};

enum ov5640_size {
	OV5640_SIZE_QVGA,
	OV5640_SIZE_VGA,
	OV5640_SIZE_720P,
	OV5640_SIZE_1080P,
	OV5640_SIZE_5MP,
	OV5640_SIZE_LAST,
};

static const struct v4l2_frmsize_discrete ov5640_frmsizes[OV5640_SIZE_LAST] = {
	{  320,  240 },
	{  640,  480 },
	{ 1280,  720 },
	{ 1920, 1080 },
	{ 2592, 1944 },
};

#endif				/* ifndef _OV5640_H */

