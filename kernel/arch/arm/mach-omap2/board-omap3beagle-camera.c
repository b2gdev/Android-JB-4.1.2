/*
 * BeagleXM: Driver for Leopard Module Board
 *
 * Copyright (C) 2011 Texas Instruments Inc
 * Author: Vaibhav Hiremath <hvaibhav@ti.com>
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
 */

#include <linux/io.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>

#include <mach/gpio.h>
#ifndef CONFIG_VIDEO_OV5640
#define CONFIG_VIDEO_OV5640
#endif
#include <media/mt9v113.h>
#ifdef CONFIG_VIDEO_OV5640
#include <media/ov5640.h>
#endif

#include <../drivers/media/video/isp/isp.h>

#include "devices.h"

#define CAM_USE_XCLKA			0
#define RESETB_GPIO				98
#define CAM_PWR_EN				152

static struct regulator *beagle_1v8;
static struct regulator *beagle_2v8;

#ifdef CONFIG_VIDEO_MT9V113
static int beagle_mt9v113_s_power(struct v4l2_subdev *subdev, int on)
{
	struct isp_device *isp = v4l2_dev_to_isp_device(subdev->v4l2_dev);

	printk(KERN_INFO "%s() ENTER\n", __func__);
	if (!beagle_1v8 || !beagle_2v8) {
		dev_err(isp->dev, "No regulator available\n");
		return -ENODEV;
	}
	if (on) {
		/* Check Voltage-Levels */
		if (regulator_get_voltage(beagle_1v8) != 1800000)
			regulator_set_voltage(beagle_1v8, 1800000, 1800000);
		if (regulator_get_voltage(beagle_2v8) != 1800000)
			regulator_set_voltage(beagle_2v8, 1800000, 1800000);
		/*
		 * Power Up Sequence
		 */
		/* Set RESET_BAR to 0 */
		gpio_set_value(RESETB_GPIO, 0);
		/* Turn on VDD */
		regulator_enable(beagle_1v8);
		mdelay(1);
		regulator_enable(beagle_2v8);

		mdelay(50);
		/* Enable EXTCLK */
		if (isp->platform_cb.set_xclk)
			isp->platform_cb.set_xclk(isp, 24000000, CAM_USE_XCLKA);
		/*
		 * Wait at least 70 CLK cycles (w/EXTCLK = 24MHz):
		 * ((1000000 * 70) / 24000000) = aprox 3 us.
		 */
		udelay(3);
		/* Set RESET_BAR to 1 */
		gpio_set_value(RESETB_GPIO, 1);
		/*
		 * Wait at least 100 CLK cycles (w/EXTCLK = 24MHz):
		 * ((1000000 * 100) / 24000000) = aprox 5 us.
		 */
		udelay(5);
	} else {
		/*
		 * Power Down Sequence
		 */
		if (regulator_is_enabled(beagle_1v8))
			regulator_disable(beagle_1v8);
		if (regulator_is_enabled(beagle_2v8))
			regulator_disable(beagle_2v8);

		if (isp->platform_cb.set_xclk)
			isp->platform_cb.set_xclk(isp, 0, CAM_USE_XCLKA);
	}
	printk(KERN_INFO "%s() ENTER\n", __func__);
	return 0;
}
#endif
#ifdef CONFIG_VIDEO_OV5640
#define POWER_DOWN_GPIO 167
static int beagle_ov5640_s_power(struct v4l2_subdev *subdev, int on)
{
	struct isp_device *isp = v4l2_dev_to_isp_device(subdev->v4l2_dev);
	printk(KERN_INFO "%s() ENTER\n", __func__);
	
	if (on) {
		/* Request POWER_DOWN_GPIO and set to output */		
		gpio_request(POWER_DOWN_GPIO, "CAM_PWDN");		/* {PS} : CAM_PWDN		*/
		gpio_direction_output(POWER_DOWN_GPIO, 1);		/* {PS} : CAM_PWDN			- HIGH 	- Power down Camera */
		gpio_set_value(POWER_DOWN_GPIO, 1);				/* {PS} : CAM_PWDN			- HIGH 	- Power down Camera */
		/*
		 * Power Up Sequence
		 */
		/* Set POWER_DOWN to 1 */
		gpio_set_value(POWER_DOWN_GPIO, 1);
		/* Set RESET_BAR to 0 */
		gpio_set_value(RESETB_GPIO, 0);		
 
		mdelay(50);
		/* Enable EXTCLK */
		if (isp->platform_cb.set_xclk)
			isp->platform_cb.set_xclk(isp, 24000000, CAM_USE_XCLKA);
		/*
		 * Wait at least 70 CLK cycles (w/EXTCLK = 24MHz):
		 * ((1000000 * 70) / 24000000) = aprox 3 us.
		 */
		udelay(3);
		/* Set POWER_DOWN to 0 */
		gpio_set_value(POWER_DOWN_GPIO, 0);
		/*
		 * Wait more than 1ms - sensor power up stable to RESETB pull up
		 */
		mdelay(2);
		/* Set RESET_BAR to 1 */
		gpio_set_value(RESETB_GPIO, 1);
		/*
		 * Wait at least 100 CLK cycles (w/EXTCLK = 24MHz):
		 * ((1000000 * 100) / 24000000) = aprox 5 us.
		 */
		udelay(5);
		/*
		 * Wait 20ms before initializaion of the sensor
		 */
		mdelay(20);
	} else {
		/* Set RESET_BAR to 0 */
		gpio_set_value(RESETB_GPIO, 0);
		
		if (isp->platform_cb.set_xclk)
			isp->platform_cb.set_xclk(isp, 0, CAM_USE_XCLKA);
		
		/* Set POWER_DOWN to 1 */
		gpio_set_value(POWER_DOWN_GPIO, 1);
	}
	printk(KERN_INFO "%s() EXIT\n", __func__);
	return 0;
}

static int beagle_ov5640_s_suspend(struct v4l2_subdev *subdev){
	struct isp_device *isp = v4l2_dev_to_isp_device(subdev->v4l2_dev);
	printk(KERN_INFO "%s() ENTER\n", __func__);
	
	if (isp->platform_cb.set_xclk)
		isp->platform_cb.set_xclk(isp, 0, CAM_USE_XCLKA);
		
	/* Set POWER_DOWN to 1 */
	gpio_set_value(POWER_DOWN_GPIO, 1);
	
	printk(KERN_INFO "%s() EXIT\n", __func__);
	return 0;
}

static int beagle_ov5640_s_resume(struct v4l2_subdev *subdev){
	struct isp_device *isp = v4l2_dev_to_isp_device(subdev->v4l2_dev);
	printk(KERN_INFO "%s() ENTER\n", __func__);
	
	/* Enable EXTCLK */
	if (isp->platform_cb.set_xclk)
		isp->platform_cb.set_xclk(isp, 24000000, CAM_USE_XCLKA);
	/*
	 * Wait at least 70 CLK cycles (w/EXTCLK = 24MHz):
	 * ((1000000 * 70) / 24000000) = aprox 3 us.
	 */
	udelay(3);
	/* Set POWER_DOWN to 0 */
	gpio_set_value(POWER_DOWN_GPIO, 0);
	
	printk(KERN_INFO "%s() EXIT\n", __func__);
	return 0;
}

static void beagle_ov5640_s_shutdown(struct v4l2_subdev *subdev){
	struct isp_device *isp = v4l2_dev_to_isp_device(subdev->v4l2_dev);
	printk(KERN_INFO "%s() ENTER\n", __func__);
	
	/* Set RESET_BAR to 0 */
	gpio_set_value(RESETB_GPIO, 0);
	
	if (isp->platform_cb.set_xclk)
		isp->platform_cb.set_xclk(isp, 0, CAM_USE_XCLKA);
	
	/* Set POWER_DOWN to 1 */
	gpio_set_value(POWER_DOWN_GPIO, 1);
	
	/* Turn off Camera power supply */
	gpio_set_value(CAM_PWR_EN, 0);
	
	printk(KERN_INFO "%s() EXIT\n", __func__);
}
#endif

#ifdef CONFIG_VIDEO_MT9V113
static int beagle_mt9v113_configure_interface(struct v4l2_subdev *subdev,
					      u32 pixclk)
{
	struct isp_device *isp = v4l2_dev_to_isp_device(subdev->v4l2_dev);
	printk(KERN_INFO "%s() ENTER\n", __func__);
	if (isp->platform_cb.set_pixel_clock)
		isp->platform_cb.set_pixel_clock(isp, pixclk);
		printk(KERN_INFO "%s() EXIT\n", __func__);
	return 0;
}
#endif
#ifdef CONFIG_VIDEO_OV5640
static int beagle_ov5640_configure_interface(struct v4l2_subdev *subdev,
					      u32 pixclk)
{
	struct isp_device *isp = v4l2_dev_to_isp_device(subdev->v4l2_dev);
	printk(KERN_INFO "%s() ENTER\n", __func__);
 
	if (isp->platform_cb.set_pixel_clock)
		isp->platform_cb.set_pixel_clock(isp, pixclk);
	printk(KERN_INFO "%s() EXIT\n", __func__);
	return 0;
}
#endif
 
#ifdef CONFIG_VIDEO_MT9V113
static struct mt9v113_platform_data beagle_mt9v113_platform_data = {
	.s_power		= beagle_mt9v113_s_power,
	.configure_interface	= beagle_mt9v113_configure_interface,
};
#endif
#ifdef CONFIG_VIDEO_OV5640
static struct ov5640_platform_data beagle_ov5640_platform_data = {
	.s_power		= beagle_ov5640_s_power,
	.s_suspend		= beagle_ov5640_s_suspend,
	.s_resume		= beagle_ov5640_s_resume,
	.s_shutdown		= beagle_ov5640_s_shutdown,
	.configure_interface	= beagle_ov5640_configure_interface,
};
#endif
 
#define CAMERA_I2C_BUS_NUM		3

#ifdef CONFIG_MACH_FLASHBOARD
#define MT9V113_I2C_BUS_NUM		3
#else
#define MT9V113_I2C_BUS_NUM		2
#endif

#ifdef CONFIG_VIDEO_MT9V113
static struct i2c_board_info beagle_camera_i2c_devices[] = {
	{
		I2C_BOARD_INFO(MT9V113_MODULE_NAME, MT9V113_I2C_ADDR),
		.platform_data = &beagle_mt9v113_platform_data,
	},
};
#endif
#ifdef CONFIG_VIDEO_OV5640
static struct i2c_board_info beagle_ov5640_i2c_devices[] = {
	{
		I2C_BOARD_INFO(OV5640_MODULE_NAME, OV5640_I2C_ADDR),
		.platform_data = &beagle_ov5640_platform_data,
	},
};
#endif
 
#ifdef CONFIG_VIDEO_MT9V113
static struct isp_subdev_i2c_board_info beagle_camera_primary_subdevs[] = {
	{
		.board_info = &beagle_camera_i2c_devices[0],
		.i2c_adapter_id = MT9V113_I2C_BUS_NUM,
	},
	{ NULL, 0 },
};
#endif
#ifdef CONFIG_VIDEO_OV5640
static struct isp_subdev_i2c_board_info beagle_ov5640_primary_subdevs[] = {
	{
		.board_info = &beagle_ov5640_i2c_devices[0],
		.i2c_adapter_id = CAMERA_I2C_BUS_NUM,
	},
	{ NULL, 0 },
};
#endif

static struct isp_v4l2_subdevs_group beagle_camera_subdevs[] = {
#ifdef CONFIG_VIDEO_MT9V113
	{
		.subdevs = beagle_camera_primary_subdevs,
		.interface = ISP_INTERFACE_PARALLEL,
		.bus = {
			.parallel = {
				.data_lane_shift	= 2,
				.clk_pol		= 0,
				.bridge			= 3,
			},
		},
	},
#endif
#ifdef CONFIG_VIDEO_OV5640
	{
		.subdevs = beagle_ov5640_primary_subdevs,
		.interface = ISP_INTERFACE_PARALLEL,
		.bus = {
			.parallel = {
				.data_lane_shift	= 1,	/* {  DSG: Set to 1 from 2 } */
				.clk_pol		= 0,
				.hdpol			= 0,
				.vdpol			= 0,
				.fldmode		= 0,
				.bridge			= 3,
				.width			= 0,	/* {  DSG: Set to 8} */
			},
		},
	},
#endif	
	{ NULL, 0 },
};

static struct isp_platform_data beagle_isp_platform_data = {
	.subdevs = beagle_camera_subdevs,
};

static int __init beagle_cam_init(void)
{	
	printk( KERN_INFO "%s() ENTER\n", __func__);
	gpio_request(CAM_PWR_EN, "CAM_PWR_EN");			/* {PS} : CAM_PWR_EN		*/
	gpio_direction_output(CAM_PWR_EN, 0);			/* {PS} : CAM_PWR_EN		- LOW	- Turn off Camera power supply */
	gpio_set_value(CAM_PWR_EN, 1);					/* {PS} : CAM_PWR_EN		- HIGH	- Turn on Camera power supply */
	
	/*
	 * Sensor reset GPIO
	 */
	if (gpio_request(RESETB_GPIO,  "CAM_nRST") != 0) {  	/* {PS} : CAM_nRST		*/
		printk(KERN_ERR "beagle-cam: Could not request GPIO %d\n",
				RESETB_GPIO);
		regulator_put(beagle_1v8);
		regulator_put(beagle_2v8);
		return -ENODEV;
	}
	/* set to output mode */
	gpio_direction_output(RESETB_GPIO, 0);	/* {PS} : CAM_nRST			- LOW	- Reset Camera */
	gpio_set_value(RESETB_GPIO, 0);			/* {PS} : CAM_nRST			- LOW	- Reset Camera */

	omap3_init_camera(&beagle_isp_platform_data);

	printk(KERN_INFO "%s() EXIT\n", __func__);
	return 0;
}

static void __exit beagle_cam_exit(void)
{
	printk(KERN_INFO "%s() ENTER\n", __func__);
 	gpio_free(RESETB_GPIO);
	printk(KERN_INFO "%s() EXIT\n", __func__);
}

module_init(beagle_cam_init);
module_exit(beagle_cam_exit);

MODULE_AUTHOR("Texas Instruments");
MODULE_DESCRIPTION("BeagleXM Camera Module");
MODULE_LICENSE("GPL");
