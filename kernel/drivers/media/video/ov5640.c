/*
 * Driver for OV5640
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/v4l2-mediabus.h>
#include <linux/delay.h>

#include <media/v4l2-chip-ident.h>
#include <media/v4l2-device.h>
#include <media/v4l2-subdev.h>
#include <media/v4l2-ctrls.h>
#include <media/ov5640.h>

#include "ov5640_regs.h"

/* Macro's */
#define I2C_RETRY_COUNT             (5)

#define OV5640_DEF_WIDTH			640
#define OV5640_DEF_HEIGHT			480
#define OV5640_CAPTURE_WIDTH		2592
#define OV5640_CAPTURE_HEIGHT		1944

#define PREVIEW_SHUTTER_MAX			1770
#define AUTO_FLASH_GAIN_THREHOLD	70

//#define DEBUG_OV5640				1
#ifdef DEBUG_OV5640
#define debug_print(...) \
            do{ printk(__VA_ARGS__); } while (0)
#else 
#define debug_print(...) \
            do{} while (0)
#endif

static bool defConfDone = 0;
static bool fireflash = 0;
static uint preview_shutter, preview_gain16;
static uint preview_HTS, preview_sysclk;
static u8 average;

enum Operations operation = PREVIEW_OPERATION;
enum v4l2_ov5640_flasher_mode current_flasher_mode = V4L2_OV5640_FLASHER_AUTO;

/* Debug functions */
static int debug = 0;
//module_param(debug, bool, 0644);
MODULE_PARM_DESC(debug, "Debug level (0-1)");

/*
 * struct ov5640 - sensor object
 * @subdev: v4l2_subdev associated data
 * @pad: media entity associated data
 * @format: associated media bus format
 * @rect: configured resolution/window
 * @pdata: Board specific
 * @ver: Chip version
 * @power: Current power state (0: off, 1: on)
 */
struct ov5640 {
	struct v4l2_subdev subdev;
	struct media_pad pad;
	struct v4l2_mbus_framefmt format;
	struct v4l2_rect rect;

	struct v4l2_ctrl_handler ctrls;

	const struct ov5640_platform_data *pdata;
	unsigned int ver;
	bool power;
};

#define to_ov5640(sd)	container_of(sd, struct ov5640, subdev)
/*
 * struct ov5640_fmt -
 * @mbus_code: associated media bus code
 * @fmt: format descriptor
 */
struct ov5640_fmt {
	unsigned int mbus_code;
	struct v4l2_fmtdesc fmt;
};
/*
 * List of image formats supported by ov5640
 * Currently we are using 8 bit and 8x2 bit mode only, but can be
 * extended to 10 bit mode.
 */
static const struct ov5640_fmt ov5640_fmt_list[] = {
	{
		.mbus_code = V4L2_MBUS_FMT_UYVY8_2X8,
		.fmt = {
			.index = 0,
			.type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
			.flags = 0,
			.description = "8-bit UYVY 4:2:2 Format",
			.pixelformat = V4L2_PIX_FMT_UYVY,
		},
	},
	{
		.mbus_code = V4L2_MBUS_FMT_YUYV8_2X8,
		.fmt = {
			.index = 1,
			.type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
			.flags = 0,
			.description = "8-bit YUYV 4:2:2 Format",
			.pixelformat = V4L2_PIX_FMT_YUYV,
		},
	},
	
};

static int ov5640_read_reg(struct i2c_client *client, unsigned short reg)
{
	int err = 0;
	struct i2c_msg msg[1];
	unsigned char data[2];
	unsigned char val = 0;
	
	if (!client->adapter)
		return -ENODEV;

	msg->addr = client->addr;
	msg->flags = 0;
	msg->len = I2C_TWO_BYTE_TRANSFER;
	msg->buf = data;
	data[0] = (reg & I2C_TXRX_DATA_MASK_UPPER) >> I2C_TXRX_DATA_SHIFT;
	data[1] = (reg & I2C_TXRX_DATA_MASK);
	err = i2c_transfer(client->adapter, msg, 1);
	if (err >= 0) {
		msg->flags = I2C_M_RD;
		msg->len = I2C_ONE_BYTE_TRANSFER;	/* 1 byte read */
		err = i2c_transfer(client->adapter, msg, 1);
		if (err >= 0) {
			val = (data[0] & I2C_TXRX_DATA_MASK);
		}
	}

	v4l_dbg(1, debug, client,
		 "ov5640_read_reg reg=0x%x, val=0x%x\n", reg, val);

	return (int)(0xff & val);
}

static int ov5640_write_reg(struct i2c_client *client, unsigned short reg,
			     unsigned char val)
{
	int err = -EAGAIN; /* To enter below loop, err has to be negative */
	int trycnt = 0;
	struct i2c_msg msg[1];
	unsigned char data[4];

	v4l_dbg(1, debug, client,
			"ov5640_write_reg reg=0x%x, val=0x%x\n", reg, val);

	if (!client->adapter)
		return -ENODEV;

	while ((err < 0) && (trycnt < I2C_RETRY_COUNT)) {
		trycnt++;
		msg->addr = client->addr;
		msg->flags = 0;
		msg->len = I2C_THREE_BYTE_TRANSFER;
		msg->buf = data;
		data[0] = (reg & I2C_TXRX_DATA_MASK_UPPER) >>
			I2C_TXRX_DATA_SHIFT;
		data[1] = (reg & I2C_TXRX_DATA_MASK);
		data[2] = (val & I2C_TXRX_DATA_MASK);
		err = i2c_transfer(client->adapter, msg, 1);
	}
	if (err < 0){
		debug_print( "\n KERN_INFO I2C write failed");
	}

	if (err==1) err=0;
	return err;
}

/* 
 * Clear one bit of a reg
 */
static int ov5640_clr_reg(struct i2c_client *client, unsigned short reg,
			     unsigned char val)
{
	int reg_value = 0;
	reg_value = ov5640_read_reg(client, reg);
	reg_value &= (~val);
	return ov5640_write_reg(client, reg, reg_value);
}

/*
 * ov5640_write_regs : Initializes a list of registers
 *		if reg equals 0, then entire write operation terminates
 *
 * reglist - list of registers to be written
 * Returns zero if successful, or non-zero otherwise.
 */
static int ov5640_write_regs(struct i2c_client *client,
			      const struct ov5640_reg reglist[])
{
	int err;
	const struct ov5640_reg *next = reglist;

	for (; next->reg != 0; next++) {
		err = ov5640_write_reg(client, next->reg, next->val);
		if (err < 0) {
			v4l_err(client, "Write failed. Err[%d]\n", err);
			return err;
		}
	}
	return 0;
}

/*
 * Acquire light frequency to be used in anti banding filter
 */
static int ov5640_get_light_frequency(struct i2c_client *client)
{
	// get banding filter value
	int temp, temp1, light_frequency;

	temp = ov5640_read_reg(client, 0x3c01);

	if (temp & 0x80) {
		// manual
		temp1 = ov5640_read_reg(client, 0x3c00);
		if (temp1 & 0x04) {
		// 50Hz
		light_frequency = 50;
		}
		else {
			// 60Hz
			light_frequency = 60;
		}
	}
	else
	{
		// auto
		temp1 = ov5640_read_reg(client, 0x3c0c);
		if (temp1 & 0x01) {
			// 50Hz
			light_frequency = 50;
		}
		else {
			// 60Hz
			light_frequency = 60;
		}
	}
	return light_frequency;
}

/*
 * Acquire system clock
 */
static int ov5640_get_sysclk(struct i2c_client *client)
{
	// calculate sysclk
	int XVCLK = 2400;
	int temp1, temp2;
	int Multiplier, PreDiv, VCO, SysDiv, Pll_rdiv, sclk_rdiv, sysclk;
	int Bit_div2x = 4;//default
	int sclk_rdiv_map[] = {1, 2, 4, 8};

	temp1 = ov5640_read_reg(client, 0x3034);
	temp2 = temp1 & 0x0f;
	if (temp2 == 8 || temp2 == 10) {
		Bit_div2x = temp2 / 2;
	}

	temp1 = ov5640_read_reg(client, 0x3035);
	SysDiv = temp1>>4;
	if(SysDiv == 0) {
		SysDiv = 16;
	}

	temp1 = ov5640_read_reg(client, 0x3036);
	Multiplier = temp1;

	temp1 = ov5640_read_reg(client, 0x3037);
	PreDiv = temp1 & 0x0f;
	Pll_rdiv = ((temp1 >> 4) & 0x01) + 1;

	temp1 = ov5640_read_reg(client, 0x3108);
	temp2 = temp1 & 0x03;
	sclk_rdiv = sclk_rdiv_map[temp2];

	VCO = XVCLK * Multiplier / PreDiv;

	sysclk = VCO / SysDiv / Pll_rdiv * 2 / Bit_div2x / sclk_rdiv;

	return sysclk;
}

/*
 * Set-up shutter and gain for capturing, configure for 5M capture
 * Returns zero if successful, or non-zero otherwise.
 */
static int ov5640_new_capture(struct v4l2_subdev *subdev)
{
	int ret = 0;
	u8 tmpreg = 0;
	uint capture_shutter = 0, capture_gain16 = 0;						//Shutter time = capture_shutter * timer per row
	uint capture_gain16_shutter = 0;
	uint AE_TARGET = 0x39;
	uint AE_low = AE_TARGET * 23 / 25;
	uint AE_high = AE_TARGET * 27 / 25;
	uint capture_max_band;
	uint capture_bandingfilter = 0;
	
	uint capture_HTS, capture_VTS, capture_sysclk;
	
	int light_frequency;
	u8 n;
	
	struct i2c_client *client = NULL;
	
	client = v4l2_get_subdevdata(subdev);
	
	ov5640_write_reg(client, 0x3503, 0x03); 							//AE manual mode

	ret = ov5640_write_regs(client, ov5640_5m_cap);
	if (ret) {
		debug_print("ov5640_5m_cap failed\n");
		goto out;
	}

	mdelay(3);

	// read capture VTS, HTS, sysclk
	tmpreg = ov5640_read_reg(client, 0x380c);
	capture_HTS = (((uint) (tmpreg & 0x1f))<<8);
	tmpreg = ov5640_read_reg(client, 0x380d);
	capture_HTS += tmpreg;
	tmpreg = ov5640_read_reg(client, 0x380e);
	capture_VTS = (((uint) (tmpreg))<<8);
	tmpreg = ov5640_read_reg(client, 0x380f);
	capture_VTS += tmpreg;
	capture_sysclk = ((uint) (ov5640_get_sysclk(client)));

	// calculate capture banding filter
	light_frequency = ov5640_get_light_frequency(client);
	if (light_frequency == 60) {
		// 60Hz
		capture_bandingfilter = capture_sysclk * 100 / capture_HTS * 100 / 120;
		debug_print("light_frequency == 60\n");
	}
	else {
		// 50Hz
		capture_bandingfilter = capture_sysclk * 100 / capture_HTS;
		debug_print("light_frequency == 50\n");
	}
	capture_max_band = ((capture_VTS - 4)/capture_bandingfilter);

	// calculate capture shutter/gain16
	if (((uint) (average)) > AE_low && ((uint) (average)) < AE_high) {
		// in stable range
		capture_gain16_shutter = preview_gain16 * preview_shutter * capture_sysclk/preview_sysclk * preview_HTS/capture_HTS * AE_TARGET / ((uint) (average));
	}
	else {
		capture_gain16_shutter = preview_gain16 * preview_shutter * capture_sysclk/preview_sysclk * preview_HTS/capture_HTS;
	}
	
	// seperate capture shutter and gain16
	if(preview_shutter < PREVIEW_SHUTTER_MAX){							//Light level is adequate
		capture_gain16 = preview_gain16 * 9 / 10;						//90% of the preview gain for capture gain
		if(capture_gain16 == 0){
			capture_gain16 = 1;
		}
		capture_shutter = capture_gain16_shutter / capture_gain16;
		n = capture_shutter / capture_bandingfilter;					//Shutter should be an integer multiple of light frequency to avoid flickering
		if(n > 2){														//Assign shutter to closest integer multiple of light frequency
			if(abs(capture_shutter - n*capture_bandingfilter) > abs(capture_shutter - (n+1)*capture_bandingfilter)){
				capture_shutter = (n+1) * capture_bandingfilter;		
			}else{
				capture_shutter = n*capture_bandingfilter;
			}
		}																//Do not change shutter to integer multiple of light frequency if shutter is too small
		if(capture_shutter > capture_VTS){								//Shutter should be less than frame period
			capture_shutter = capture_bandingfilter * capture_max_band;
		}
		capture_gain16 = capture_gain16_shutter / capture_shutter;
	}else{																//Light level is low - Mostly inside an indoor environment
		capture_shutter = ((int) (capture_gain16_shutter/48/capture_bandingfilter)) * capture_bandingfilter;
		if(capture_shutter > capture_VTS){
			capture_shutter = capture_bandingfilter*capture_max_band;
		}
		capture_gain16 = capture_gain16_shutter / capture_shutter;		//Keep gain at 48
	}
	
	debug_print("Capture Exposure %d, Gain %d\n",capture_shutter,capture_gain16);

	// write capture gain
	tmpreg = (u8) ((capture_gain16 >> 8) & 0x03);						//Gain
	ret = ov5640_write_reg(client, 0x350a, tmpreg);	
	if (ret) { goto out; }
	tmpreg = (u8) (capture_gain16 & 0xff);
	ret = ov5640_write_reg(client, 0x350b, tmpreg);	
	if (ret) { goto out; }

	// write capture shutter
	if (capture_shutter > (capture_VTS - 4)) {
		capture_VTS = capture_shutter + 4;
		
		tmpreg = (u8) ((capture_VTS >> 8) & 0xff);
		ov5640_write_reg(client, 0x380e, tmpreg);
		tmpreg = (u8) (capture_VTS & 0xff);
		ov5640_write_reg(client, 0x380f, tmpreg);
	}
	
	tmpreg = (u8) ((capture_shutter >> 12) & 0x0f);						//Exposure
	ret = ov5640_write_reg(client, 0x3500, tmpreg);	
	if (ret) { goto out; }
	tmpreg = (u8) ((capture_shutter >> 4) & 0xff);
	ret = ov5640_write_reg(client, 0x3501, tmpreg);	
	if (ret) { goto out; }
	tmpreg = (u8) ((capture_shutter & 0x0f) << 4);
	ret = ov5640_write_reg(client, 0x3502, tmpreg);	
	if (ret) { goto out; }

	debug_print("Capture sequence done\n");

	//OV5640_set_EV(8);
out:
	return ret;
}

/*
 * Configure the ov5640 back to VGA streaming
 * Returns zero if successful, or non-zero otherwise.
 */
static int ov5640_conf_back_vga(struct v4l2_subdev *subdev)
{
	struct i2c_client *client;
	debug_print( "%s() ENTER\n", __func__);
	client = v4l2_get_subdevdata(subdev);

	/* common register initialization */
	debug_print( "%s() EXIT\n", __func__);
	return ov5640_write_regs(client, ov5640_back_to_vga);
}

/*
 * Configure the ov5640 with the current register settings
 * Returns zero if successful, or non-zero otherwise.
 */
static int ov5640_def_config(struct v4l2_subdev *subdev)
{
	struct i2c_client *client;
	debug_print( "%s() ENTER\n", __func__);
	client = v4l2_get_subdevdata(subdev);

	/* common register initialization */
	debug_print( "%s() EXIT\n", __func__);
	return ov5640_write_regs(client, ov5640_new_reg_list);
}

/*
 * Configure the ov5640 to VGA mode
 * Returns zero if successful, or non-zero otherwise.
 */
static int ov5640_af_def_config(struct v4l2_subdev *subdev)
{
	struct i2c_client *client = NULL;
	int ret = 0;
	int i=0;

	debug_print(  "%s() ENTER\n", __func__);
	client = v4l2_get_subdevdata(subdev);
	
	/*
	 * AF firmware
	 */
	ret = ov5640_write_regs(client, ov5640_af_reg);
	if (ret) {
		debug_print("%s() write ov5640_af_reg failed\n", __func__);
		v4l_err(client, "Failed to load af firmware\n");
	}
	else
	{
		mdelay(10);														//Delay 10ms after loading the AF firmware
		debug_print("%s() write ov5640_af_reg completed\n", __func__);
		// wait until the status is idle
		i=0;
		while ((ov5640_read_reg(client, OV5640_AF_REG_FW_STAT) != OV5640_AF_FW_IDLE) && (i < OV5640_AF_IDLE_TRIES)) {
			mdelay(5);													//Loop every 5ms and check the idle state
			i++;
		}
		if(i < OV5640_AF_IDLE_TRIES){
			debug_print("AF FW - IDLE Successful\n");
		}
		else{
			debug_print("AF FW - Not IDLE\n");
		}
	}

	debug_print("%s() EXIT\n", __func__);
	return ret;
}

/*
 * Detect if an ov5640 is present, and if so which revision.
 * A device is considered to be detected if the chip ID (LSB and MSB)
 * registers match the expected values.
 * Any value of the rom version register is accepted.
 * Returns ENODEV error number if no device is detected, or zero
 * if a device is detected.
 */
static int ov5640_detect(struct v4l2_subdev *subdev)
{
	struct ov5640 *decoder;
	struct i2c_client *client;
	unsigned short val = 0;
	debug_print( "%s() ENTER\n", __func__);
	decoder = to_ov5640(subdev);
	client = v4l2_get_subdevdata(subdev);
	

	val = (ov5640_read_reg(client, REG_CHIP_ID_MSB) << 8) + ov5640_read_reg(client, REG_CHIP_ID_LSB);
	v4l_dbg(1, debug, client, "chip id detected 0x%x\n", val);

	if (OV5640_CHIP_ID != val) {
		/* We didn't read the values we expected, so this must not be
		 * ov5640.
		 */
		v4l_err(client, "chip id mismatch read 0x%x, expecting 0x%x\n",
				val, OV5640_CHIP_ID);
		return -ENODEV;
	}

	decoder->ver = val;

	v4l_info(client, "%s found at 0x%x (%s)\n", client->name,
			client->addr << 1, client->adapter->name);
	debug_print( "%s() EXIT\n", __func__);
	return 0;
}

/* --------------------------------------------------------------------------
 * V4L2 subdev core operations
 */
/*
 * ov5640_g_chip_ident - get chip identifier
 */
static int ov5640_g_chip_ident(struct v4l2_subdev *subdev,
			       struct v4l2_dbg_chip_ident *chip)
{
	debug_print( "%s() ENTER\n", __func__);
#if 0
	struct i2c_client *client = v4l2_get_subdevdata(subdev);

	return v4l2_chip_ident_i2c_client(client, chip, V4L2_IDENT_OV5640, 0);
#else
	debug_print( "%s() EXIT\n", __func__);
	return 0;
#endif
}

/*
 * ov5640_dev_init - sensor init, tries to detect the sensor
 * @subdev: pointer to standard V4L2 subdev structure
 */
static int ov5640_dev_init(struct v4l2_subdev *subdev)
{
	struct ov5640 *decoder;
	int rval;
	debug_print( "%s() ENTER\n", __func__);
	decoder = to_ov5640(subdev);	

	rval = decoder->pdata->s_power(subdev, 1);
	if (rval)
		return rval;

	rval = ov5640_detect(subdev);

	decoder->pdata->s_power(subdev, 0);

	debug_print( "%s() EXIT\n", __func__);
	return rval;
}

/*
 * ov5640_s_config - set the platform data for future use
 * @subdev: pointer to standard V4L2 subdev structure
 * @irq:
 * @platform_data: sensor platform_data
 */
static int ov5640_s_config(struct v4l2_subdev *subdev, int irq,
			   void *platform_data)
{
	struct ov5640 *decoder;
	int rval;
	debug_print( "%s() ENTER\n", __func__);
	decoder = to_ov5640(subdev);
	
	if (platform_data == NULL)
		return -ENODEV;

	decoder->pdata = platform_data;

	rval = ov5640_dev_init(subdev);
	if (rval)
		return rval;

	debug_print( "%s() EXIT\n", __func__);
	return 0;
}

/*
 * ov5640_s_power - Set power function
 * @subdev: pointer to standard V4L2 subdev structure
 * @on: power state to which device is to be set
 *
 * Sets devices power state to requested state, if possible.
 */
static int ov5640_s_power(struct v4l2_subdev *subdev, int on)
{
	struct ov5640 *decoder;
	struct i2c_client *client;
	int rval;
	debug_print( "%s() ENTER\n", __func__);
	decoder = to_ov5640(subdev);
	client = v4l2_get_subdevdata(subdev);	

	if (on) {
		rval = decoder->pdata->s_power(subdev, 1);
		if (rval)
			goto out;
		rval = ov5640_def_config(subdev);
		if (rval) {
			decoder->pdata->s_power(subdev, 0);
			goto out;
		}
		defConfDone = 1;
		rval = ov5640_af_def_config(subdev);
		if (rval) {
			decoder->pdata->s_power(subdev, 0);
			goto out;
		}
	} else {
		rval = decoder->pdata->s_power(subdev, 0);
		if (rval)
			goto out;
	}

	decoder->power = on;
out:
	if (rval < 0)
		v4l_err(client, "Unable to set target power state\n");

	debug_print( "%s() EXIT\n", __func__);
	return rval;
}

/* --------------------------------------------------------------------------
 * V4L2 subdev file operations
 */
static int ov5640_open(struct v4l2_subdev *subdev, struct v4l2_subdev_fh *fh)
{
	struct v4l2_mbus_framefmt *format;
	struct v4l2_rect *crop;

	debug_print( "%s() ENTER\n", __func__);

	/*
	 * Default configuration -
	 *	Resolution: VGA
	 *	Format: UYVY
	 *	crop = window
	 */
	crop = v4l2_subdev_get_try_crop(fh, 0);
	crop->left = 0;
	crop->top = 0;
	crop->width = OV5640_DEF_WIDTH;
	crop->height = OV5640_DEF_HEIGHT;

	format = v4l2_subdev_get_try_format(fh, 0);
	format->code = V4L2_MBUS_FMT_UYVY8_2X8;
	format->width = OV5640_DEF_WIDTH;
	format->height = OV5640_DEF_HEIGHT;
	format->field = V4L2_FIELD_NONE;
	format->colorspace = V4L2_COLORSPACE_JPEG;

	debug_print( "%s() EXIT\n", __func__);
	return 0;
}

/* --------------------------------------------------------------------------
 * V4L2 subdev video operations
 */

/* Find a frame size in an array */
static int ov5640_find_framesize(u32 width, u32 height)
{
	int i;
	debug_print( "%s() ENTER\n", __func__);
	for (i = 0; i < OV5640_SIZE_LAST; i++) {
		if ((ov5640_frmsizes[i].width >= width) &&
		    (ov5640_frmsizes[i].height >= height))
			break;
	}

	/* If not found, select biggest */
	if (i >= OV5640_SIZE_LAST)
		i = OV5640_SIZE_LAST - 1;
	debug_print( "%s() EXIT\n", __func__);
	return i;
}

static int ov5640_s_stream(struct v4l2_subdev *subdev, int streaming)
{
	struct i2c_client *client = NULL;
	int ret = 0;

	debug_print( "%s() ENTER\n", __func__);
#ifndef DEBUG_OV5640
	udelay(500);														//Time needed before powering up/down
#endif
	client = v4l2_get_subdevdata(subdev);

	if(streaming){
		debug_print("%s Start stream\n", __FUNCTION__);
#ifndef DEBUG_OV5640
		udelay(700);													//Time needed before powering up
#endif
		ret = ov5640_clr_reg(client, 0x3008, 0x40);
		if (ret) { goto out; }
	}else{
		u8 tmpreg = 0;
		debug_print("%s Stop stream\n", __FUNCTION__);
#ifndef DEBUG_OV5640
		udelay(700);													//Time needed before powering down
#endif
		tmpreg = ov5640_read_reg(client, 0x3008);						//Soft power down
		ov5640_write_reg(client, 0x3008, tmpreg | 0x40);
		if (ret) { goto out; }
	}
out:
	debug_print( "%s() EXIT\n", __func__);
#ifndef DEBUG_OV5640
	udelay(500);														//Time needed after powering up/down
#endif
	return ret;
}

/* --------------------------------------------------------------------------
 * V4L2 subdev pad operations
 */
static int ov5640_enum_mbus_code(struct v4l2_subdev *subdev,
				  struct v4l2_subdev_fh *fh,
				  struct v4l2_subdev_mbus_code_enum *code)
{
	struct ov5640 *ov5640;
	debug_print( "%s() ENTER\n", __func__);
	ov5640 = to_ov5640(subdev);

	if (code->index >= ARRAY_SIZE(ov5640_fmt_list))
		return -EINVAL;

	code->code = ov5640->format.code;

	debug_print( "%s() EXIT\n", __func__);
	return 0;
}

static int ov5640_enum_frame_size(struct v4l2_subdev *subdev,
				   struct v4l2_subdev_fh *fh,
				   struct v4l2_subdev_frame_size_enum *fse)
{
	int i;
	debug_print( "%s() ENTER\n", __func__);

	/* Is requested media-bus format/pixelformat not found on sensor? */
	for (i = 0; i < ARRAY_SIZE(ov5640_fmt_list); i++) {
		if (fse->code == ov5640_fmt_list[i].mbus_code )
			goto fmt_found;
	}
	if (i >= ARRAY_SIZE(ov5640_fmt_list))
		return -EINVAL;

fmt_found:
	if ((fse->index >= OV5640_SIZE_LAST))
		return -EINVAL;

	fse->min_width = ov5640_frmsizes[fse->index].width;
	fse->max_width = fse->min_width;
	fse->min_height = ov5640_frmsizes[fse->index].height;
	fse->max_height = fse->min_height;

	debug_print( "%s() EXIT\n", __func__);
	return 0;
}

static int ov5640_get_pad_format(struct v4l2_subdev *subdev,
				  struct v4l2_subdev_fh *fh,
				  struct v4l2_subdev_format *fmt)
{
	struct ov5640 *ov5640;
	debug_print( "%s() ENTER\n", __func__);
	ov5640 = to_ov5640(subdev);

	fmt->format = ov5640->format;
	debug_print( "%s() EXIT %08X\n", __func__, fmt->format);
	return 0;
}

/*
 * Prepare ov5640 for capturing and streaming
 */
static int ov5640_set_pad_format(struct v4l2_subdev *subdev,
				  struct v4l2_subdev_fh *fh,
				  struct v4l2_subdev_format *fmt)
{
	struct ov5640 *ov5640;
	int ret = 0;
	debug_print( "%s() ENTER\n", __func__);
	
	ov5640 = to_ov5640(subdev);
	
	ov5640->format.code = V4L2_MBUS_FMT_UYVY8_2X8;
	ov5640->format.width = fmt->format.width;
	ov5640->format.height = fmt->format.height;
	ov5640->format.field = V4L2_FIELD_NONE;
	ov5640->format.colorspace = V4L2_COLORSPACE_JPEG;
	
	if((ov5640->format.width == OV5640_CAPTURE_WIDTH)  && (ov5640->format.height == OV5640_CAPTURE_HEIGHT)){
		if(defConfDone && (operation == PREVIEW_OPERATION)){
			debug_print("Configuring for capture\n");
			ret = ov5640_new_capture(subdev);
			if (ret) { goto out; }
			operation = CAPTURE_OPERATION;
		}
	}else if((ov5640->format.width == OV5640_DEF_WIDTH)  && (ov5640->format.height == OV5640_DEF_HEIGHT)){
		if(defConfDone && (operation == CAPTURE_OPERATION)){
			debug_print("Configuring for streaming\n");
			ret = ov5640_conf_back_vga(subdev);
			if (ret) { goto out; }
			operation = PREVIEW_OPERATION;
		}
	}else{
		debug_print("Configuring is not correct\n");
	}

	debug_print( "%s() EXIT\n", __func__);
out:
	return ret;
}

static int ov5640_get_crop(struct v4l2_subdev *subdev,
		struct v4l2_subdev_fh *fh, struct v4l2_subdev_crop *crop)
{
	struct ov5640 *ov5640;
	debug_print( "%s() ENTER\n", __func__);
	ov5640 = to_ov5640(subdev);

	crop->rect = ov5640->rect;
	debug_print( "%s() EXIT\n", __func__);
	return 0;
}

static int ov5640_set_crop(struct v4l2_subdev *subdev,
		struct v4l2_subdev_fh *fh, struct v4l2_subdev_crop *crop)
{
	struct ov5640 *ov5640;
	debug_print( "%s() ENTER\n", __func__);
	ov5640 = to_ov5640(subdev);	

	ov5640->rect = crop->rect;

	debug_print( "%s() EXIT\n", __func__);
	return 0;
}

static const struct v4l2_subdev_core_ops ov5640_core_ops = {
	.g_chip_ident = ov5640_g_chip_ident,
	.s_config = ov5640_s_config,
	.s_power = ov5640_s_power,
};

static struct v4l2_subdev_file_ops ov5640_subdev_file_ops = {
	.open		= ov5640_open,
};

static const struct v4l2_subdev_video_ops ov5640_video_ops = {
	.s_stream = ov5640_s_stream,
};

static const struct v4l2_subdev_pad_ops ov5640_pad_ops = {
	.enum_mbus_code	= ov5640_enum_mbus_code,
	.enum_frame_size= ov5640_enum_frame_size,
	.get_fmt	= ov5640_get_pad_format,
	.set_fmt	= ov5640_set_pad_format,
	.get_crop	= ov5640_get_crop,
	.set_crop	= ov5640_set_crop,
};

static const struct v4l2_subdev_ops ov5640_ops = {
	.core	= &ov5640_core_ops,
	.file	= &ov5640_subdev_file_ops,
	.video	= &ov5640_video_ops,
	.pad	= &ov5640_pad_ops,
};

/* -----------------------------------------------------------------------------
 * V4L2 subdev control operations
 */

#define V4L2_CID_TEST_PATTERN		(V4L2_CID_USER_BASE | 0x1001)

/*
 * Fire flasher
 */
static void ov5640_strobe(struct i2c_client *client){
	if(fireflash){
		debug_print("Flasher - Strobe\n");
		ov5640_write_reg(client, 0x3019, 0x02);
	}
}

/*
 * Off flasher
 */
static void ov5640_strobe_stop(struct i2c_client *client){
	if(fireflash){
		debug_print("Flasher - Strobe off\n");
		ov5640_write_reg(client, 0x3019, 0x00);
	}
}

/*
 * Commands to AF module - Continuous AF or release AF
 */
static void ov5640_af_command(struct i2c_client *client,int af_command){
	int ret = 0;
	int i = 0;
	
	ov5640_write_reg(client, OV5640_AF_REG_ACK, 0x01);
	ret = ov5640_write_reg(client, OV5640_AF_REG_MAIN, af_command);
	if (ret) {
		debug_print("%s() failed to set command: 0x%x\n", __func__, af_command);
		v4l_err(client, "Failed to set command: 0x%x\n",af_command);

	}else{
		debug_print("AF Command - Entered loop\n");
		i=0;
		while ((ov5640_read_reg(client, OV5640_AF_REG_ACK) != 0x00) && (i < OV5640_AF_ACK_TRIES)){
			mdelay(1);
			i++;
		}
		if(i < OV5640_AF_ACK_TRIES){
			debug_print("AF Command - Exit loop - %d - Successful\n",af_command);
		}
		else{
			debug_print("AF Command - Exit loop - %d - Unsuccessful\n",af_command);
		}
	}
}

static void ov5640_get_preview_params(struct i2c_client *client){
	u8 tmpreg = 0;
	bool do_with_flash = 0;
	
	do{
		preview_sysclk = ((uint) (ov5640_get_sysclk(client)));
		tmpreg = ov5640_read_reg(client, 0x380c);
		preview_HTS = (((uint) (tmpreg & 0x1f))<<8);
		tmpreg = ov5640_read_reg(client, 0x380d);
		preview_HTS += tmpreg;

		//AE below
		tmpreg = ov5640_read_reg(client, 0x3500);    					//Exposure
		preview_shutter = (((uint) (tmpreg & 0x0f)) << 12);
		tmpreg = ov5640_read_reg(client, 0x3501);
		preview_shutter += (((uint) tmpreg) << 4);
		tmpreg = ov5640_read_reg(client, 0x3502);
		preview_shutter += (tmpreg >> 4);
		tmpreg = ov5640_read_reg(client, 0x350a);						//Gain
		preview_gain16 = (((uint) (tmpreg & 0x03)) << 8);
		tmpreg = ov5640_read_reg(client, 0x350b);
		preview_gain16 += tmpreg;

		// get average
		average = ov5640_read_reg(client, 0x56a1);
		debug_print("Preview Exposure %d, Gain %d, average %d\n",preview_shutter,preview_gain16,average);
		
		if(do_with_flash){
			ov5640_strobe_stop(client);
			return;
		}

		if(((preview_shutter >= PREVIEW_SHUTTER_MAX) && (preview_gain16 > AUTO_FLASH_GAIN_THREHOLD) && (current_flasher_mode == V4L2_OV5640_FLASHER_AUTO)) || (current_flasher_mode == V4L2_OV5640_FLASHER_ON)){					//Flasher fire or not
			fireflash = 1;												//Conditions satisfied to fire flash
			do_with_flash = 1;
			debug_print("Fire flash and get params again\n");
			ov5640_strobe(client);
			mdelay(1000);
		}else{
			fireflash = 0;
		}
	}while(do_with_flash);
}

/*
 * ov5640_s_ctrl - V4L2 decoder interface handler for VIDIOC_S_CTRL ioctl
 * @s: pointer to standard V4L2 device structure
 * @ctrl: pointer to v4l2_control structure
 *
 * If the requested control is supported, sets the control's current
 * value in HW. Otherwise, returns -EINVAL if the control is not supported.
 */
static int ov5640_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct ov5640 *ov5640;
	struct i2c_client *client;
	int err;

    ov5640 = container_of(ctrl->handler, struct ov5640, ctrls);
	client = v4l2_get_subdevdata(&ov5640->subdev);
	err = -EINVAL;

	if (ctrl == NULL) {
		debug_print("%s ctrl == NULL\n", __FUNCTION__);	
		return err;
	}

	debug_print("%s --> %08X\n", __FUNCTION__, ctrl->id);

	switch (ctrl->id) {
		
	case V4L2_CID_AUTO_FOCUS_STOP:
		ov5640_af_command(client,OV5640_AF_RELEASE_AF);					//AF - Release AF
		err = 0;
		break;
	
	case V4L2_CID_AUTO_FOCUS_START:
		ov5640_af_command(client,OV5640_AF_CONT_AF);					//AF - Continuous AF
		err = 0;
		break;
		
	case V4L2_CID_FLASH_STROBE:											//Flasher - Strobe
		ov5640_strobe(client);
		err = 0;
		break;
		
	case V4L2_CID_FLASH_STROBE_STOP:									//Flasher - Off
		ov5640_strobe_stop(client);
		err = 0;
		break;
		
	case V4L2_OV5640_GET_PREVIEW_PARAMS:								//Get preview params
		ov5640_get_preview_params(client);
		err = 0;
		break;
		
	case V4L2_OV5640_FLASHER_MODE:										//Flasher - Mode
		current_flasher_mode = ctrl->val;
		debug_print("Flasher Mode - %d\n", current_flasher_mode);
		err = 0;
		break;
		
	default:
		v4l_err(client, "invalid control id %d\n", ctrl->id);
		debug_print("%s Default case\n", __FUNCTION__);
		return err;
	}

	v4l_dbg(1, debug, client,
			"Set Control: ID - %d - %d", ctrl->id, ctrl->val);

	debug_print( "%s() EXIT\n", __func__);
	return err;
}

static struct v4l2_ctrl_ops ov5640_ctrl_ops = {
	.s_ctrl = ov5640_s_ctrl,
};

static const char * v4l2_ov5640_flasher_mode_menu[] = {
	"auto",
	"on",
	"off",
	"torch",
};

static const struct v4l2_ctrl_config ov5640_ctrls[] = {
	{
		.ops		= &ov5640_ctrl_ops,
		.id			= V4L2_CID_TEST_PATTERN,
		.type		= V4L2_CTRL_TYPE_INTEGER,
		.name		= "Test pattern",
		.min		= 0,
		.max		= 1023,
		.step		= 1,
		.def		= 0,
		.flags		= 0,
	},
	{
		.ops		= &ov5640_ctrl_ops,
		.id			= V4L2_OV5640_FLASHER_MODE,
		.type		= V4L2_CTRL_TYPE_MENU,
		.name		= "OV5640 Flasher Mode",
		.min		= V4L2_OV5640_FLASHER_AUTO,
		.max		= V4L2_OV5640_FLASHER_TORCH,
		.def		= V4L2_OV5640_FLASHER_AUTO,
		.menu_skip_mask = 0,
		.flags		= V4L2_CTRL_FLAG_WRITE_ONLY,
		.qmenu		= v4l2_ov5640_flasher_mode_menu,
	},
	{
		.ops		= &ov5640_ctrl_ops,
		.id			= V4L2_OV5640_GET_PREVIEW_PARAMS,
		.type		= V4L2_CTRL_TYPE_BUTTON,
		.name		= "OV5640 Set Capture Params",
		.min		= 0,
		.max		= 0,
		.step		= 0,
		.def		= 0,
		.flags		= V4L2_CTRL_FLAG_WRITE_ONLY,
	}
};

/*
 * ov5640_probe - sensor driver i2c probe handler
 * @client: i2c driver client device structure
 *
 * Register sensor as an i2c client device and V4L2
 * sub-device.
 */
static int ov5640_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	struct ov5640 *ov5640;
	int i, ret;
	debug_print( "%s() ENTER\n", __func__);

	/* Check if the adapter supports the needed features */
	if (!i2c_check_functionality(client->adapter,
				I2C_FUNC_SMBUS_BYTE_DATA)) {
		v4l_err(client, "ov5640: I2C Adapter doesn't support" \
				" I2C_FUNC_SMBUS_WORD\n");
		return -EIO;
	}
	
	if (!client->dev.platform_data) {
		v4l_err(client, "No platform data!!\n");
		return -ENODEV;
	}

	ov5640 = kzalloc(sizeof(*ov5640), GFP_KERNEL);
	if (ov5640 == NULL) {
		v4l_err(client, "Could not able to alocate memory!!\n");
		return -ENOMEM;
	}

	ov5640->pdata = client->dev.platform_data; 

	/*
	 * Initialize and register available controls
	 */
	v4l2_ctrl_handler_init(&ov5640->ctrls, ARRAY_SIZE(ov5640_ctrls) + 8);
	v4l2_ctrl_new_std(&ov5640->ctrls, &ov5640_ctrl_ops,
			V4L2_CID_AUTOGAIN, 0, 1, 1, 1);
	v4l2_ctrl_new_std(&ov5640->ctrls, &ov5640_ctrl_ops,					//AF - Start AF
			V4L2_CID_AUTO_FOCUS_START, 0, 0, 0, 0);
	v4l2_ctrl_new_std(&ov5640->ctrls, &ov5640_ctrl_ops,					//AF - Release AF
			V4L2_CID_AUTO_FOCUS_STOP, 0, 0, 0, 0);
	v4l2_ctrl_new_std(&ov5640->ctrls, &ov5640_ctrl_ops,					//Flasher Strobe
			V4L2_CID_FLASH_STROBE, 0, 0, 0, 0);
	v4l2_ctrl_new_std(&ov5640->ctrls, &ov5640_ctrl_ops,					//Flasher Off
			V4L2_CID_FLASH_STROBE_STOP, 0, 0, 0, 0);

	for (i = 0; i < ARRAY_SIZE(ov5640_ctrls); ++i)
		v4l2_ctrl_new_custom(&ov5640->ctrls, &ov5640_ctrls[i], NULL);

	ov5640->subdev.ctrl_handler = &ov5640->ctrls;

	if (ov5640->ctrls.error)
		v4l_info(client, "%s: error while initialization control %d\n",
				__func__, ov5640->ctrls.error);

	/*
	 * Default configuration -
	 *	Resolution: VGA
	 *	Format: UYVY
	 *	crop = window
	 */
	ov5640->rect.left = 0;
	ov5640->rect.top = 0;
	ov5640->rect.width = OV5640_DEF_WIDTH;
	ov5640->rect.height = OV5640_DEF_HEIGHT;

	ov5640->format.code = V4L2_MBUS_FMT_UYVY8_2X8;
	ov5640->format.width = OV5640_DEF_WIDTH;
	ov5640->format.height = OV5640_DEF_HEIGHT;
	ov5640->format.field = V4L2_FIELD_NONE;
	ov5640->format.colorspace = V4L2_COLORSPACE_JPEG;

	/*
	 * Register as a subdev
	 */
	v4l2_i2c_subdev_init(&ov5640->subdev, client, &ov5640_ops);
	ov5640->subdev.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;

	/*
	 * Register as media entity
	 */
	ov5640->pad.flags = MEDIA_PAD_FLAG_OUTPUT;
	ret = media_entity_init(&ov5640->subdev.entity, 1, &ov5640->pad, 0);
	if (ret < 0) {
		v4l_err(client, "failed to register as a media entity\n");
		kfree(ov5640);
	}	
	debug_print( "%s() EXIT\n", __func__);
	return ret;
}

/*
 * ov5640_remove - Sensor driver i2c remove handler
 * @client: i2c driver client device structure
 *
 * Unregister sensor as an i2c client device and V4L2
 * sub-device.
 */
static int __exit ov5640_remove(struct i2c_client *client)
{
	struct v4l2_subdev *subdev;
	struct ov5640 *ov5640;
	debug_print( "%s() ENTER\n", __func__);
	subdev = i2c_get_clientdata(client);
	ov5640 = to_ov5640(subdev);

	v4l2_device_unregister_subdev(subdev);
	media_entity_cleanup(&subdev->entity);
	kfree(ov5640);

	debug_print( "%s() EXIT\n", __func__);
	return 0;
}

static const struct i2c_device_id ov5640_id[] = {
	{ OV5640_MODULE_NAME, 0 },
	{ },
};

MODULE_DEVICE_TABLE(i2c, ov5640_id);

static struct i2c_driver ov5640_i2c_driver = {
	.driver = {
		.name = OV5640_MODULE_NAME,
		.owner = THIS_MODULE,
	},
	.probe = ov5640_probe,
	.remove = __exit_p(ov5640_remove),
	.id_table = ov5640_id,
};

static int __init ov5640_init(void)
{
	debug_print("%s() ENTER\n", __func__);
	debug_print("%s() EXIT\n", __func__);
	return i2c_add_driver(&ov5640_i2c_driver);
}

static void __exit ov5640_cleanup(void)
{
	debug_print("%s() ENTER\n", __func__);
	i2c_del_driver(&ov5640_i2c_driver);
	debug_print("%s() EXIT\n", __func__);
}

module_init(ov5640_init);
module_exit(ov5640_cleanup);

MODULE_DESCRIPTION("OmniVision OV5640 camera driver");
MODULE_AUTHOR("");
MODULE_LICENSE("GPL");
