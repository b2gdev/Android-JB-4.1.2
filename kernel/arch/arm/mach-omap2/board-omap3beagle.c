/*
 * linux/arch/arm/mach-omap2/board-omap3beagle.c
 *
 * Copyright (C) 2008 Texas Instruments
 *
 * Modified from mach-omap2/board-3430sdp.c
 *
 * Initial code: Syed Mohammed Khasim
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/leds.h>
#include <linux/gpio.h>
#include <linux/input.h>
#include <linux/gpio_keys.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/nand.h>
#include <linux/mmc/host.h>

/* {PS} BEGIN: */
#ifdef CONFIG_WL12XX_PLATFORM_DATA
#include <linux/wl12xx.h>
#include <linux/regulator/fixed.h>
#endif
/* {PS} END: */

#include <linux/usb/android_composite.h>

#include <linux/regulator/machine.h>
#include <linux/i2c/twl.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/flash.h>

#include <plat/board.h>
#include <plat/common.h>
#include <plat/display.h>
#include <plat/gpmc.h>
#include <plat/nand.h>
#include <plat/usb.h>

// {RD} BEGIN:
#include <plat/mmc.h>
#include <linux/ti_wilink_st.h>
#include "linux/i2c/twl.h"
#define TIOSETWL1271POWER 0x6000
// {RD} END:

#include <linux/mfd/wm8994/pdata.h> 	/* {PS} */
#include <linux/spi/spi.h>				/* {AE} */
#include <plat/mcspi.h>					/* {PS} */
#include <linux/i2c/lis331dlh.h>		/* {PS} */
#include <linux/i2c/hmc5883l.h>			/* {RD} */
#include <linux/reboot.h>				/* {RD} */
#include "mux.h"
#include "hsmmc.h"
#include "timer-gp.h"
#include "board-flash.h"

/* {PS} BEGIN: */
#ifdef CONFIG_SND_SOC_WL1271BT
#include "control.h"
#endif
/* {PS} END: */

#define NAND_BLOCK_SIZE		SZ_128K

#ifdef CONFIG_USB_ANDROID

#define GOOGLE_VENDOR_ID		0x18d1
#define GOOGLE_PRODUCT_ID		0x9018
#define GOOGLE_ADB_PRODUCT_ID		0x9015

/* {RD} REBOOT_MODE */
#define REBOOT_MODE_NONE		0
#define REBOOT_MODE_DOWNLOAD		1
#define REBOOT_MODE_CHARGING		3
#define REBOOT_MODE_RECOVERY		4
#define REBOOT_MODE_FAST_BOOT		5

#define OMAP_DIE_ID_0		0x4830A218
#define OMAP_DIE_ID_1		0x4830A21C

int is_bt_on = 0; /* {RD} */

#define MAX_USB_SERIAL_NUM	17

static char device_serial[MAX_USB_SERIAL_NUM] = "0123456789ABCDEF";

int u_boot_serial_number = 0;

static int __init set_device_serial_number(char *str)
{
 get_option(&str, &u_boot_serial_number);
    return 1;
}

__setup("androidboot.serialno=", set_device_serial_number);

static int tcbin_notifier_call(struct notifier_block *this,
					unsigned long code, void *_cmd)
{
	int ret;
	unsigned char val;
	int mode = REBOOT_MODE_NONE;	

	printk(KERN_INFO "Reboot notifier call code: %X\n",code);
	
	if(code == SYS_RESTART){
		gpio_set_value(65, 1);		/* OMAP_STATUS_2 - HIGH	- Notify MSP430 that the OMAP is restarting */
		if (_cmd) {
			if (!strcmp((char *)_cmd, "recovery"))
				mode = REBOOT_MODE_RECOVERY;
			else if (!strcmp((char *)_cmd, "bootloader"))
				mode = REBOOT_MODE_FAST_BOOT;
			
			printk(KERN_INFO "Reboot notifier call with mode: %s\n",(char *)_cmd);

			if(	mode != REBOOT_MODE_NONE ){
				ret = twl_i2c_write_u8(TWL4030_MODULE_RTC, (10+mode), 12);
				if (ret != 0)
					printk(KERN_ERR "twl i2c write error: %d\n",ret);
			}																	
		}	
	}
	
	/*{KW}: inform MSP430 about power status, set to low */
    printk("OMAP_STATUS_1 gpio down\r\n");
    gpio_set_value(64, 0);	

	gpio_set_value(11, 0);		/* 3GM_SEC_PWR_EN	- LOW	- Disable power*/
	gpio_set_value(16, 1);		/* 3GM_PWR_nEN		- HIGH	- Turn off 3G modem power supply */
				
	return NOTIFY_DONE;
}

static struct notifier_block tcbin_reboot_notifier = {
	.notifier_call = tcbin_notifier_call,
};
/* {RD} Allow only ADB Mode */
static char *usb_functions_adb[] = {
	"adb",
};

static char *usb_functions_all[] = {
	"adb",
};

static struct android_usb_product usb_products[] = {
	/* {RD} Allow only ADB Mode */	
	{
		.product_id	= GOOGLE_PRODUCT_ID,
		.num_functions	= ARRAY_SIZE(usb_functions_adb),
		.functions	= usb_functions_adb,
	},
};

static struct usb_mass_storage_platform_data mass_storage_pdata = {
	.nluns		= 1,
	.vendor		= "rowboat",
	.product	= "rowboat gadget",
	.release	= 0x100,
};

static struct platform_device usb_mass_storage_device = {
	.name	= "usb_mass_storage",
	.id	= -1,
	.dev	= {
		.platform_data = &mass_storage_pdata,
	},
};

static struct android_usb_platform_data android_usb_pdata = {
	.vendor_id	= GOOGLE_VENDOR_ID,
	.product_id	= GOOGLE_PRODUCT_ID,
	.functions	= usb_functions_all,
	.products	= usb_products,
	.num_products	= ARRAY_SIZE(usb_products),
	.version	= 0x0100,
	.product_name	= "Team_CBI_Notetaker",
	.manufacturer_name	= "Zone24x7.Inc.",
	.serial_number	= device_serial,
	.num_functions	= ARRAY_SIZE(usb_functions_all),
};

static struct platform_device androidusb_device = {
	.name	= "android_usb",
	.id	= -1,
	.dev	= {
		.platform_data = &android_usb_pdata,
	},
};

static void omap3beagle_android_gadget_init(void)
{
	platform_device_register(&androidusb_device);
}
#endif

/* {PS} BEGIN: */
static struct omap2_mcspi_device_config cp430_spi_chip_info = {
	.turbo_mode	= 0,
	.single_channel	= 1,	/* 0: slave, 1: master */
};
/* {PS} END: */

static struct spi_board_info beagle_mcspi_board_info[] = {
	/* {AE} BEGIN: spi 1.0 */
	{
		.modalias			= "cp430",
		.bus_num			= 1,
		.chip_select		= 0,
		.max_speed_hz		= 250000,
		.controller_data	= &cp430_spi_chip_info,
		.mode 				= SPI_MODE_3 | SPI_CS_HIGH,
	},
	/* {AE} END: */
};

/*
 * OMAP3 Beagle revision
 * Run time detection of Beagle revision is done by reading GPIO.
 * GPIO ID -
 *	AXBX	= GPIO173, GPIO172, GPIO171: 1 1 1
 *	C1_3	= GPIO173, GPIO172, GPIO171: 1 1 0
 *	C4	= GPIO173, GPIO172, GPIO171: 1 0 1
 *	XM	= GPIO173, GPIO172, GPIO171: 0 0 0
 */
enum {
	OMAP3BEAGLE_BOARD_UNKN = 0,
	OMAP3BEAGLE_BOARD_AXBX,
	OMAP3BEAGLE_BOARD_C1_3,
	OMAP3BEAGLE_BOARD_C4,
	OMAP3BEAGLE_BOARD_XM,
	OMAP3BEAGLE_BOARD_XMC,
};

extern void omap_pm_sys_offmode_select(int);
extern void omap_pm_sys_offmode_pol(int);
extern void omap_pm_sys_clkreq_pol(int);
extern void omap_pm_auto_off(int);
extern void omap_pm_auto_ret(int);

static u8 omap3_beagle_version = OMAP3BEAGLE_BOARD_C4;	/* {PS} : TCBIN is based on Rev C4 board */

static u8 omap3_beagle_get_rev(void)
{
	return omap3_beagle_version;
}

/**
 * Board specific initialization of PM components
 */
static void __init omap3_beagle_pm_init(void)
{
	/* Use sys_offmode signal */
	omap_pm_sys_offmode_select(1);

	/* sys_clkreq - active high */
	omap_pm_sys_clkreq_pol(1);

	/* sys_offmode - active low */
	omap_pm_sys_offmode_pol(0);

	/* Automatically send OFF command */
	omap_pm_auto_off(1);

	/* Automatically send RET command */
	omap_pm_auto_ret(1);
}

static void __init omap3_beagle_init_rev(void)
{
/* {PS} BEGIN: TODO - update with correct board revision identification mechanism */
/* {PS} BEGIN: b2g is based on Rev C4 board */
	printk(KERN_INFO "OMAP3 TCBIN Rev: C\n");
	omap3_beagle_version = OMAP3BEAGLE_BOARD_C4;
/* {PS} END: */
	
	if(u_boot_serial_number == 0){
		system_serial_high = omap_readl(OMAP_DIE_ID_1);
		system_serial_low = omap_readl(OMAP_DIE_ID_0);
		sprintf(device_serial, "%08X%08X", system_serial_high,
				system_serial_low);
	}else{
		system_serial_high = 0;
		system_serial_low = u_boot_serial_number & 0xFFFFFFFF;
		sprintf(device_serial, "%u", u_boot_serial_number);		
	}
			
	return;
}

static struct mtd_partition omap3beagle_nand_partitions[] = {
	/* All the partition sizes are listed in terms of NAND block size */
	{
		.name		= "X-Loader",
		.offset		= 0,
		.size		= 4 * NAND_BLOCK_SIZE,
#ifndef CONFIG_IS_RECOVERY_KERNEL
		.mask_flags	= MTD_WRITEABLE,	/* force read-only */
#endif		
	},
	{
		.name		= "U-Boot",
		.offset		= MTDPART_OFS_APPEND,	/* Offset = 0x80000 */
		.size		= 15 * NAND_BLOCK_SIZE,
#ifndef CONFIG_IS_RECOVERY_KERNEL		
		.mask_flags	= MTD_WRITEABLE,	/* force read-only */
#endif		
	},
	{
		.name		= "U-Boot Env",
		.offset		= MTDPART_OFS_APPEND,	/* Offset = 0x260000 */
		.size		= 1 * NAND_BLOCK_SIZE,
	},
	{
		.name		= "Kernel",
		.offset		= MTDPART_OFS_APPEND,	/* Offset = 0x280000 */
		.size		= 64 * NAND_BLOCK_SIZE,
	},
	{
		.name		= "recovery",
		.offset		= MTDPART_OFS_APPEND,	/* Offset = 0xA80000 */
		.size		= 128 * NAND_BLOCK_SIZE,
	},
	{
		.name		= "misc",
		.offset		= MTDPART_OFS_APPEND,	/* Offset = 0x1A80000 */
		.size		= 8 * NAND_BLOCK_SIZE,
	},
	{
		.name		= "system",
		.offset		= MTDPART_OFS_APPEND,	/* Offset = 0x1B80000 */
		.size		= MTDPART_SIZ_FULL
	},
};

/* DSS */

/* {PS} BEGIN: */
static int beagle_enable_dvi(struct omap_dss_device *dssdev)
{
	if (gpio_is_valid(dssdev->reset_gpio))
		gpio_set_value(dssdev->reset_gpio, 1);

	return 0;
}

static void beagle_disable_dvi(struct omap_dss_device *dssdev)
{
	if (gpio_is_valid(dssdev->reset_gpio))
		gpio_set_value(dssdev->reset_gpio, 0);
}

static struct omap_dss_device beagle_dvi_device = {
	.type = OMAP_DISPLAY_TYPE_DPI,
	.name = "dvi",
	.driver_name = "generic_panel",
	.phy.dpi.data_lines = 24,
	.reset_gpio = -EINVAL,
	.platform_enable = beagle_enable_dvi,
	.platform_disable = beagle_disable_dvi,
};
/* {PS} END: */

/* {PS} BEGIN: */
static int omap3_beagle_enable_tv(struct omap_dss_device *dssdev)
{
	printk("%s\n", __FUNCTION__);	
	return 0;
}

static void omap3_beagle_disable_tv(struct omap_dss_device *dssdev)
{
	printk("%s\n", __FUNCTION__);
}
/* {PS} END: */

/* {PS} BEGIN: */
static struct omap_dss_device beagle_tv_device = {
	.name = "tv",
	.driver_name = "venc",
	.type = OMAP_DISPLAY_TYPE_VENC,
	/* {PS} BEGIN: */
	.phy.venc.type = OMAP_DSS_VENC_TYPE_COMPOSITE,
	.platform_enable	= omap3_beagle_enable_tv,
	.platform_disable	= omap3_beagle_disable_tv,	
	/* {PS} END: */
};

static struct omap_dss_device *beagle_dss_devices[] = {
	&beagle_dvi_device,
	&beagle_tv_device,
};

static struct omap_dss_board_info beagle_dss_data = {
	.num_devices = ARRAY_SIZE(beagle_dss_devices),
	.devices = beagle_dss_devices,
	.default_device = &beagle_dvi_device,
};

static struct platform_device beagle_dss_device = {
	.name          = "omapdss",
	.id            = -1,
	.dev            = {
		.platform_data = &beagle_dss_data,
	},
};
/* {PS} END: */

static struct regulator_consumer_supply beagle_vdac_supply =
	REGULATOR_SUPPLY("vdda_dac", "omapdss");

static struct regulator_consumer_supply beagle_vdvi_supply =
	REGULATOR_SUPPLY("vdds_dsi", "omapdss");

static void __init beagle_display_init(void)
{
/* {PS} BEGIN: DVI reset is not used */
}

#include "sdram-micron-mt46h32m32lf-6.h"

static struct omap2_hsmmc_info mmc[] = {
	{
		.mmc		= 1,
		.caps		= MMC_CAP_4_BIT_DATA, /* {PS} : | MMC_CAP_8_BIT_DATA, */
		.gpio_wp	= 164,
	},
/* {PS} BEGIN: Micro SD slot */
	{
		.mmc			= 2,
		.caps			= MMC_CAP_4_BIT_DATA,
		.transceiver	= true,
		.gpio_wp		= -EINVAL,
		.nonremovable   = true,
	},
/* {PS} END: */
	
/* {PS} BEGIN: */
#ifdef CONFIG_WL12XX_PLATFORM_DATA
	{
	.name           = "wl1271",
	.mmc            = 3,	/* {PS} : WLAN is on MMC3 */
	.caps           = MMC_CAP_4_BIT_DATA | MMC_CAP_POWER_OFF_CARD,
	.gpio_wp        = -EINVAL,
	.gpio_cd        = -EINVAL,
	.nonremovable   = true,
	.ocr_mask = MMC_VDD_32_33 | MMC_VDD_33_34, /*{RD} 3V3*/
	},
#endif
/* {PS} END: */	
	{}	/* Terminator */
};

/* {PS} BEGIN: */
#ifdef CONFIG_WL12XX_PLATFORM_DATA
#define TCBIN_WLAN_PMENA_GPIO       (150)	/* {PS} : WL_EN */
#define TCBIN_WLAN_IRQ_GPIO         (149)	/* {PS} : WL_INT */
#endif

static struct regulator_consumer_supply tcbin_vmmc3_supply =
	REGULATOR_SUPPLY("vmmc", "mmci-omap-hs.2");

static struct regulator_init_data tcbin_vmmc3 = {
	.constraints = {
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
	},
	.num_consumer_supplies  = 1,
	.consumer_supplies = &tcbin_vmmc3_supply,
};

static struct fixed_voltage_config tcbin_vwlan = {
	.supply_name            = "vwl1271",
	.microvolts             = 1800000, /* 1.80V */
	.gpio                   = TCBIN_WLAN_PMENA_GPIO,
	.startup_delay          = 70000, /* 70ms */
	.enable_high            = 1,
	.enabled_at_boot        = 0,
	.init_data              = &tcbin_vmmc3,
};

static struct platform_device tcbin_wlan_regulator = {
	.name           = "reg-fixed-voltage",
	.id             = 1,
	.dev = {
		.platform_data  = &tcbin_vwlan,
	},
};

struct wl12xx_platform_data tcbin_wlan_data __initdata = {
	.irq = OMAP_GPIO_IRQ(TCBIN_WLAN_IRQ_GPIO),
	.board_ref_clock = WL12XX_REFCLOCK_38, /* 38.4 MHz */
};
/* {PS} END: */

static struct regulator_consumer_supply beagle_vmmc1_supply = {
	.supply			= "vmmc",
};

/* {PS} BEGIN: */
static struct regulator_consumer_supply beagle_vmmc2_supply = {
	.supply			= "vmmc",
};
/* {PS} END: */

static struct regulator_consumer_supply beagle_vsim_supply = {
	/* {PS} BEGIN: not used for MMC */
	.supply			= "vsim",
};

static struct regulator_consumer_supply beagle_vaux3_supply = {
	.supply         = "cam_1v8",
};

static struct regulator_consumer_supply beagle_vaux4_supply = {
	.supply         = "vcc_vaux4_2v5",
};

static struct gpio_led gpio_leds[];

/* {PS} BEGIN: */
#ifdef CONFIG_SND_SOC_WL1271BT
/* WL1271 Audio */
static struct platform_device wl1271bt_audio_device = {
	.name		= "wl1271bt",
	.id		= -1,
};

static struct platform_device wl1271bt_codec_device = {
	.name		= "wl1271bt-dummy-codec",
	.id		= -1,
};

static void wl1271bt_clk_setup(void)
{
	u16 reg;
	u32 val;

	/*
	 * Set DEVCONF0 register to connect
	 * MCBSP1_CLKR -> MCBSP1_CLKX & MCBSP1_FSR -> MCBSP1_FSX
	 */
	reg = OMAP2_CONTROL_DEVCONF0;
	val = omap_ctrl_readl(reg);
	val = val | 0x18;
	omap_ctrl_writel(val, reg);
}
#endif
/* {PS} END: */

/* {RD} BEGIN: */
static int bt_init_power(void)
{
	int ret = 0;
	u8 reg_value = 0;

	/* Enable GPIO */
	ret = twl_i2c_read_u8(TWL4030_MODULE_GPIO,
				&reg_value, REG_GPIO_CTRL);
	if (ret != 0)
		goto err;

	/* T2-GPIO.13 -> output */
	ret = twl_i2c_read_u8(TWL4030_MODULE_GPIO,
				&reg_value, REG_GPIODATADIR2);
	if (ret != 0)
		goto err;

	reg_value |= 0x20;
	ret = twl_i2c_write_u8(TWL4030_MODULE_GPIO,
				reg_value, REG_GPIODATADIR2);
	if (ret != 0)
		goto err;
	/* T2-GPIO.13 -> LOW */
	ret = twl_i2c_read_u8(TWL4030_MODULE_GPIO,
				&reg_value, REG_GPIODATAOUT2);
	if (ret != 0)
		goto err;

	reg_value &= ~(0x20);
	ret = twl_i2c_write_u8(TWL4030_MODULE_GPIO,
				reg_value, REG_GPIODATAOUT2);
	if (ret != 0)
		goto err;

	mdelay(50);
	/* T2-GPIO.13 -> HIGH */
	reg_value |= (0x20);
	ret = twl_i2c_write_u8(TWL4030_MODULE_GPIO,
				reg_value, REG_GPIODATAOUT2);
	if (ret != 0)
		goto err;
	mdelay(50);
	/* T2-GPIO.13 -> LOW */
	reg_value &= ~(0x20);
	ret = twl_i2c_write_u8(TWL4030_MODULE_GPIO,
				reg_value, REG_GPIODATAOUT2);
	if (ret != 0)
		goto err;
	
	return 0;
err:
	printk(KERN_ERR "WL1271: BT_EN GPIO initialization FAILED\n");
	return ret;
} /* End of init_bt_power() */

int plat_kim_suspend(struct platform_device *pdev, pm_message_t state)
{
	/* TODO: wait for HCI-LL sleep */	
	return 0;
}

int plat_kim_resume(struct platform_device *pdev)
{
	return 0;
}

int plat_kim_chip_disable(struct kim_data_s *kim_data)
{
	int err = 0;
	u8 reg_value = 0;
	
	reg_value &= ~(0x20);

	err = twl_i2c_write_u8(TWL4030_MODULE_GPIO,
				reg_value, REG_GPIODATAOUT2);
	if (err != 0) {
		printk(KERN_INFO "WL1271: Set BT_EN failed %d\n",
								err);
		return err;
	}
	is_bt_on = 0;
	printk(KERN_INFO "BT: Powering OFF\n");
	return 0;
}

int plat_kim_chip_enable(struct kim_data_s *kim_data)
{
	int err = 0;
	u8 reg_value = 0;

	if(is_bt_on){
		printk(KERN_INFO "BT: BT is aldreay ON! disabling BT first\n");
		plat_kim_chip_disable(kim_data);
	}
	
	reg_value |= (0x20);

	err = twl_i2c_write_u8(TWL4030_MODULE_GPIO,
				reg_value, REG_GPIODATAOUT2);
	if (err != 0) {
		printk(KERN_INFO "BT: Set BT_EN failed %d\n",
								err);
		return err;
	}
	is_bt_on = 1;
	printk(KERN_INFO "BT: Powering ON\n");
	return 0;
}

struct ti_st_plat_data wilink_pdata = {
	.dev_name = "/dev/ttyO1",
	.flow_cntrl = 1,
	.baud_rate = 3000000,
	.suspend = plat_kim_suspend,
	.resume = plat_kim_resume,
	.chip_enable = plat_kim_chip_enable,
	.chip_disable = plat_kim_chip_disable,
};

static struct platform_device wl12xx_device = {
	.name		= "kim",
	.id		= -1,
	.dev.platform_data = &wilink_pdata,
};

static struct platform_device btwilink_device = {
	.name = "btwilink",
	.id = -1,
};

static inline void __init wl12xx_bluetooth_enable(void)
{
	printk(KERN_INFO "wl12xx_bluetooth_enable\n");
	//Call BT init "BT_EN GPIO initialized"
	bt_init_power();
	
	platform_device_register(&wl12xx_device);
	platform_device_register(&btwilink_device);
}


static int wl12xx_set_power(struct device *dev, int slot, int on, int vdd)
{
        if (on) {
			printk("%s WLAN ENABLE ON\n", __FUNCTION__);
			gpio_request(150, "WL_EN");         /* {PS} : WL_EN				*/
			gpio_direction_output(150, 0);		/* {PS} : WL_EN				- LOW	- Disable Wi-Fi */
			gpio_set_value(150, 1);		/* {PS} : WL_EN	*/
            mdelay(70);
		
        } else {
			printk("%s WLAN ENABLE OFF\n", __FUNCTION__);
			gpio_request(150, "WL_EN");         /* {PS} : WL_EN				*/
			gpio_direction_output(150, 0);		/* {PS} : WL_EN				- LOW	- Disable Wi-Fi */
			gpio_set_value(150, 0);		/* {PS} : WL_EN	*/
        }

        return 0;
}
/* {RD} END: */

static int beagle_twl_gpio_setup(struct device *dev,
		unsigned gpio, unsigned ngpio)
{
// {RD} BEGIN
	struct omap_mmc_platform_data *pdata2;
	struct device *dev2;
// {RD} END
	int ret;
	unsigned char val;
	
	if (omap3_beagle_get_rev() == OMAP3BEAGLE_BOARD_XM || omap3_beagle_get_rev() == OMAP3BEAGLE_BOARD_XMC) {
		mmc[0].gpio_wp = -EINVAL;
	} else if ((omap3_beagle_get_rev() == OMAP3BEAGLE_BOARD_C1_3) ||
		(omap3_beagle_get_rev() == OMAP3BEAGLE_BOARD_C4)) {
		/* {PS} BEGIN: */
		omap_mux_init_gpio(164, OMAP_PIN_INPUT); /* {PS} : MMC1_WP */
		mmc[0].gpio_wp = 164;
		
		#if 0
		omap_mux_init_gpio(23, OMAP_PIN_INPUT);
		mmc[0].gpio_wp = 23;
		#endif
		/* {PS} END: */
	} else {
		omap_mux_init_gpio(29, OMAP_PIN_INPUT);
	}
	
	/* gpio + 0 is "mmc0_cd" (input/IRQ) */
	mmc[0].gpio_cd = gpio + 0;	/* {PS} : MMC1_CD is connected to GPIO_0 of twl4030, same as BeagleBoard */
	mmc[1].gpio_cd = gpio + 1;	/* {PS} : MMC2_CD is connected to GPIO_1 of twl4030 */
	omap2_hsmmc_init(mmc);

	/* link regulators to MMC adapters */
	beagle_vmmc1_supply.dev = mmc[0].dev;
	beagle_vmmc2_supply.dev = mmc[1].dev;
	
	/* {PS} BEGIN: Enable USB Host power supply (VCC_5V1_USB) */
	gpio_request(gpio + 2, "USBHOST_PWR_EN");	/* TWL4030 GPIO.2 */
	gpio_direction_output(gpio + 2, 1);			/* {PS} : gpio_direction_output(gpio + 2, 0); */
	/* {PS} END: */

	/* {RD} BEGIN: */
	wl12xx_bluetooth_enable();
	
#ifdef CONFIG_WL12XX_PLATFORM_DATA
	/* WL12xx WLAN Init */
	if (wl12xx_set_platform_data(&tcbin_wlan_data))
		pr_err("error setting wl12xx data\n");	
#endif
	
#ifdef CONFIG_SND_SOC_WL1271BT
	wl1271bt_clk_setup();
#endif	

	dev2 = mmc[2].dev;
	if (!dev2) {
		pr_err("wl12xx mmc device initialization failed\n");
	}else{
		pdata2 = dev->platform_data;
		pdata2->slots[0].set_power = wl12xx_set_power;
	}
	/* {RD} END: */
	
	/* {RD} BEGIN: */
	ret = twl_i2c_read_u8(TWL4030_MODULE_RTC, &val, 12);
	if (ret != 0)
		printk(KERN_ERR "twl i2c write error: %d\n",ret);	
	else{
		if(val >= 10){			
			ret = twl_i2c_write_u8(TWL4030_MODULE_RTC,0, 12);
			if (ret != 0)
				printk(KERN_ERR "twl i2c write error: %d\n",ret);
		}
	}	
	/* {RD} END: */
	
	return 0;
}

static struct twl4030_gpio_platform_data beagle_gpio_data = {
	.gpio_base	= OMAP_MAX_GPIO_LINES,
	.irq_base	= TWL4030_GPIO_IRQ_BASE,
	.irq_end	= TWL4030_GPIO_IRQ_END,
	.use_leds	= true,
	/* {PS} BEGIN: */
	/* .pullups	= BIT(1), */					/* {PS} : MMC2_CD has external PU */
	.pulldowns	= 				/* BIT(2) | */	/* {PS} : USBHOST_PWR_EN has external PD */ 
				  	  	  	    /* BIT(6) | */	/* {PS} : USBHOST_nFAULT has external UP*/ 
				  BIT(7) |						/* {PS} : NC pin */ 
				  BIT(8) | 						/* {PS} : NC pin */
				  	  	  	    /* BIT(13)| */	/* {PS} : CC_BT_EN has external PD */ 
				  BIT(15)| 						/* {PS} : NC pin */
				  BIT(16)|						/* {PS} : DSS_CC_PWREN uses internal PD */ 
				  BIT(17),						/* {PS} : DSS_CC_RST uses internal PD */
	/* {PS} END: */
	.setup		= beagle_twl_gpio_setup,
};

/* VMMC1 for MMC1 pins CMD, CLK, DAT0..DAT3 (20 mA, plus card == max 220 mA) */
static struct regulator_init_data beagle_vmmc1 = {
	.constraints = {
		.min_uV			= 1850000,
		.max_uV			= 3150000,
		.valid_modes_mask	= REGULATOR_MODE_NORMAL
					| REGULATOR_MODE_STANDBY,
		.valid_ops_mask		= REGULATOR_CHANGE_VOLTAGE
					| REGULATOR_CHANGE_MODE
					| REGULATOR_CHANGE_STATUS,
	},
	.num_consumer_supplies	= 1,
	.consumer_supplies	= &beagle_vmmc1_supply,
};

/* {PS} BEGIN: VMMC2 for MMC2 */
static struct regulator_init_data beagle_vmmc2 = {
	.constraints = {
		.min_uV			= 3150000,
		.max_uV			= 3150000,
		.valid_modes_mask	= REGULATOR_MODE_NORMAL
					| REGULATOR_MODE_STANDBY,
		.valid_ops_mask		= REGULATOR_CHANGE_VOLTAGE
					| REGULATOR_CHANGE_MODE
					| REGULATOR_CHANGE_STATUS,
	},
	.num_consumer_supplies	= 1,
	.consumer_supplies	= &beagle_vmmc2_supply,
};
/* {PS} END: */

/* VSIM for MMC1 pins DAT4..DAT7 (2 mA, plus card == max 50 mA) */
static struct regulator_init_data beagle_vsim = {
	.constraints = {
		/* {PS} BEGIN: */
		.min_uV			= 3000000,
		.max_uV			= 3000000,
		.apply_uV     	= true,
		.valid_modes_mask	= REGULATOR_MODE_NORMAL,
		.valid_ops_mask		= REGULATOR_CHANGE_MODE
					| REGULATOR_CHANGE_STATUS,
					
		#if 0	
		.min_uV			= 1800000,
		.max_uV			= 3000000,
		.valid_modes_mask	= REGULATOR_MODE_NORMAL
					| REGULATOR_MODE_STANDBY,
		.valid_ops_mask		= REGULATOR_CHANGE_VOLTAGE
					| REGULATOR_CHANGE_MODE
					| REGULATOR_CHANGE_STATUS,
		#endif
		/* {PS} END: */
	},
	.num_consumer_supplies	= 1,
	.consumer_supplies	= &beagle_vsim_supply,
};

/* VDAC for DSS driving S-Video (8 mA unloaded, max 65 mA) */
static struct regulator_init_data beagle_vdac = {
	.constraints = {
		.min_uV			= 1800000,
		.max_uV			= 1800000,
		.valid_modes_mask	= REGULATOR_MODE_NORMAL
					| REGULATOR_MODE_STANDBY,
		.valid_ops_mask		= REGULATOR_CHANGE_MODE
					| REGULATOR_CHANGE_STATUS,
	},
	.num_consumer_supplies	= 1,
	.consumer_supplies	= &beagle_vdac_supply,
};

/* VPLL2 for digital video outputs */
static struct regulator_init_data beagle_vpll2 = {
	.constraints = {
		.name			= "VDVI",
		.min_uV			= 1800000,
		.max_uV			= 1800000,
		.valid_modes_mask	= REGULATOR_MODE_NORMAL
					| REGULATOR_MODE_STANDBY,
		.valid_ops_mask		= REGULATOR_CHANGE_MODE
					| REGULATOR_CHANGE_STATUS,
	},
	.num_consumer_supplies	= 1,
	.consumer_supplies	= &beagle_vdvi_supply,
};

/* {PS} : */
/* VAUX3 for CAM_1V8 */
static struct regulator_init_data beagle_vaux3 = {
	.constraints = {
		.min_uV                 = 1800000,
		.max_uV                 = 1800000,
		.apply_uV               = true,
		.valid_modes_mask       = REGULATOR_MODE_NORMAL
					| REGULATOR_MODE_STANDBY,
		.valid_ops_mask         = REGULATOR_CHANGE_MODE
					| REGULATOR_CHANGE_STATUS,
	},
	.num_consumer_supplies  = 1,
	.consumer_supplies      = &beagle_vaux3_supply,
};

static struct regulator_init_data beagle_vaux4 = {
	.constraints = {
		.min_uV                 = 2500000,
		.max_uV                 = 2500000,
		.apply_uV               = true,
		.valid_modes_mask       = REGULATOR_MODE_NORMAL
			| REGULATOR_MODE_STANDBY,
		.valid_ops_mask         = REGULATOR_CHANGE_MODE
			| REGULATOR_CHANGE_STATUS,
	},
	.num_consumer_supplies  = 1,
	.consumer_supplies      = &beagle_vaux4_supply,
};
/* {PS} END: */

static struct twl4030_usb_data beagle_usb_data = {
	.usb_mode	= T2_USB_MODE_ULPI,
};

/**
 * Macro to configure resources
 */
#define TWL4030_RESCONFIG(res,grp,typ1,typ2,state)	\
	{						\
		.resource	= res,			\
		.devgroup	= grp,			\
		.type		= typ1,			\
		.type2		= typ2,			\
		.remap_sleep	= state			\
	}

static struct twl4030_resconfig  __initdata board_twl4030_rconfig[] = {
	TWL4030_RESCONFIG(RES_VPLL1, DEV_GRP_P1, 3, 1, RES_STATE_OFF),		/* ? */
	TWL4030_RESCONFIG(RES_VINTANA1, DEV_GRP_ALL, 1, 2, RES_STATE_SLEEP),
	TWL4030_RESCONFIG(RES_VINTANA2, DEV_GRP_ALL, 0, 2, RES_STATE_SLEEP),
	TWL4030_RESCONFIG(RES_VINTDIG, DEV_GRP_ALL, 1, 2, RES_STATE_SLEEP),
	TWL4030_RESCONFIG(RES_VIO, DEV_GRP_ALL, 2, 2, RES_STATE_SLEEP),
	TWL4030_RESCONFIG(RES_VDD1, DEV_GRP_P1, 4, 1, RES_STATE_OFF),		/* ? */
	TWL4030_RESCONFIG(RES_VDD2, DEV_GRP_P1, 3, 1, RES_STATE_OFF),		/* ? */
	TWL4030_RESCONFIG(RES_REGEN, DEV_GRP_ALL, 2, 1, RES_STATE_SLEEP),
	TWL4030_RESCONFIG(RES_NRES_PWRON, DEV_GRP_ALL, 0, 1, RES_STATE_SLEEP),
	TWL4030_RESCONFIG(RES_CLKEN, DEV_GRP_ALL, 3, 2, RES_STATE_SLEEP),
	TWL4030_RESCONFIG(RES_SYSEN, DEV_GRP_ALL, 6, 1, RES_STATE_SLEEP),
	TWL4030_RESCONFIG(RES_HFCLKOUT, DEV_GRP_P3, 0, 2, RES_STATE_SLEEP),	/* ? */
	TWL4030_RESCONFIG(0, 0, 0, 0, 0),
};

/**
 * Optimized 'Active to Sleep' sequence
 */
static struct twl4030_ins omap3beagle_sleep_seq[] __initdata = {
	{ MSG_SINGULAR(DEV_GRP_NULL, RES_HFCLKOUT, RES_STATE_SLEEP), 20},
	{ MSG_BROADCAST(DEV_GRP_NULL, RES_GRP_ALL, RES_TYPE_R0, RES_TYPE2_R1, RES_STATE_SLEEP), 2 },
	{ MSG_BROADCAST(DEV_GRP_NULL, RES_GRP_ALL, RES_TYPE_R0, RES_TYPE2_R2, RES_STATE_SLEEP), 2 },
};

static struct twl4030_script omap3beagle_sleep_script __initdata = {
	.script	= omap3beagle_sleep_seq,
	.size	= ARRAY_SIZE(omap3beagle_sleep_seq),
	.flags	= TWL4030_SLEEP_SCRIPT,
};

/**
 * Optimized 'Sleep to Active (P12)' sequence
 */
static struct twl4030_ins omap3beagle_wake_p12_seq[] __initdata = {
	{ MSG_BROADCAST(DEV_GRP_NULL, RES_GRP_ALL, RES_TYPE_R0, RES_TYPE2_R1, RES_STATE_ACTIVE), 2 }
};

static struct twl4030_script omap3beagle_wake_p12_script __initdata = {
	.script = omap3beagle_wake_p12_seq,
	.size   = ARRAY_SIZE(omap3beagle_wake_p12_seq),
	.flags  = TWL4030_WAKEUP12_SCRIPT,
};

/**
 * Optimized 'Sleep to Active' (P3) sequence
 */
static struct twl4030_ins omap3beagle_wake_p3_seq[] __initdata = {
	{ MSG_BROADCAST(DEV_GRP_NULL, RES_GRP_ALL, RES_TYPE_R0, RES_TYPE2_R2, RES_STATE_ACTIVE), 2 }
};

static struct twl4030_script omap3beagle_wake_p3_script __initdata = {
	.script = omap3beagle_wake_p3_seq,
	.size   = ARRAY_SIZE(omap3beagle_wake_p3_seq),
	.flags  = TWL4030_WAKEUP3_SCRIPT,
};

/**
 * Optimized warm reset sequence (for less power surge)
 */
static struct twl4030_ins omap3beagle_wrst_seq[] __initdata = {
	{ MSG_SINGULAR(DEV_GRP_NULL, RES_RESET, RES_STATE_OFF), 0x2 },
	{ MSG_SINGULAR(DEV_GRP_NULL, RES_MAIN_REF, RES_STATE_WRST), 2 },
	{ MSG_BROADCAST(DEV_GRP_NULL, RES_GRP_ALL, RES_TYPE_R0, RES_TYPE2_R2, RES_STATE_WRST), 0x2},
	{ MSG_SINGULAR(DEV_GRP_NULL, RES_VUSB_3V1, RES_STATE_WRST), 0x2 },
	{ MSG_SINGULAR(DEV_GRP_NULL, RES_VPLL1, RES_STATE_WRST), 0x2 },
	{ MSG_SINGULAR(DEV_GRP_NULL, RES_VDD2, RES_STATE_WRST), 0x7 },
	{ MSG_SINGULAR(DEV_GRP_NULL, RES_VDD1, RES_STATE_WRST), 0x25 },
	{ MSG_BROADCAST(DEV_GRP_NULL, RES_GRP_RC, RES_TYPE_ALL, RES_TYPE2_R0, RES_STATE_WRST), 0x2 },
	{ MSG_SINGULAR(DEV_GRP_NULL, RES_RESET, RES_STATE_ACTIVE), 0x2 },

};

static struct twl4030_script omap3beagle_wrst_script __initdata = {
	.script = omap3beagle_wrst_seq,
	.size   = ARRAY_SIZE(omap3beagle_wrst_seq),
	.flags  = TWL4030_WRST_SCRIPT,
};

static struct twl4030_script __initdata *board_twl4030_scripts[] = {
	&omap3beagle_wake_p12_script,
	&omap3beagle_wake_p3_script,
	&omap3beagle_sleep_script,
	&omap3beagle_wrst_script
};

static struct twl4030_power_data __initdata omap3beagle_script_data = {
	.scripts		= board_twl4030_scripts,
	.num			= ARRAY_SIZE(board_twl4030_scripts),
	.resource_config	= board_twl4030_rconfig,
   /* {SW} BEGIN: adding power off support */
   .use_poweroff = true,
   /* {SW} END: */
};

static struct twl4030_platform_data beagle_twldata = {
	.irq_base	= TWL4030_IRQ_BASE,
	.irq_end	= TWL4030_IRQ_END,

	/* platform_data for children goes here */
	.usb		= &beagle_usb_data,
	.gpio		= &beagle_gpio_data,
	/* .codec		= &beagle_codec_data, */	/* {PS} : Audio CODEC is not used */
	.vmmc1		= &beagle_vmmc1,
	.vmmc2		= &beagle_vmmc2,
	.vsim		= &beagle_vsim,
	.vdac		= &beagle_vdac,
	.vpll2		= &beagle_vpll2,
	.vaux3		= &beagle_vaux3,
	.vaux4		= &beagle_vaux4,
	.power		= &omap3beagle_script_data,
};

static struct i2c_board_info __initdata beagle_i2c1_boardinfo[] = {
	{
		I2C_BOARD_INFO("twl4030", 0x48),
		.flags = I2C_CLIENT_WAKE,
		.irq = INT_34XX_SYS_NIRQ,
		.platform_data = &beagle_twldata,
	},
};

/* {PS} BEGIN: wm8994 regulator info.  */
static struct regulator_consumer_supply wm8994_ldo1_supply = {
	.supply			= "AVDD1",	/* {PS} */
};

static struct regulator_consumer_supply wm8994_ldo2_supply = {
	.supply			= "DCVDD",	/* {PS} */
};

static struct regulator_init_data wm8994_ldo1 = {
	.constraints = {
		.min_uV			= 3000000,	/* {PS} - LDO1 is used to supply AVDD1, which is 3V */
		.max_uV			= 3000000,
		.valid_modes_mask	= REGULATOR_MODE_NORMAL		/* {PS} */
					| REGULATOR_MODE_STANDBY,
		.valid_ops_mask		= REGULATOR_CHANGE_VOLTAGE	/* {PS} */
					| REGULATOR_CHANGE_MODE			// {RD} disable REGULATOR_CHANGE_STATUS
					| REGULATOR_CHANGE_STATUS,
	},
	.num_consumer_supplies	= 1,
	.consumer_supplies	= &wm8994_ldo1_supply,
};

static struct regulator_init_data wm8994_ldo2 = {
	.constraints = {
		.min_uV			= 1000000,	/* {PS} - LDO2 is used to supply DCVDD, which is 1V */
		.max_uV			= 1000000,
		.valid_modes_mask	= REGULATOR_MODE_NORMAL		/* {PS} */
					| REGULATOR_MODE_STANDBY,
		.valid_ops_mask		= REGULATOR_CHANGE_VOLTAGE	/* {PS} */
					| REGULATOR_CHANGE_MODE			// {RD} disable REGULATOR_CHANGE_STATUS
					| REGULATOR_CHANGE_STATUS,
	},
	.num_consumer_supplies	= 1,
	.consumer_supplies	= &wm8994_ldo2_supply,
};

static struct wm8994_pdata wm8994_pdata = {
	.gpio_base = 0, /* {PS} */
	.gpio_defaults = {0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0},
	.ldo = {
        	{
			.enable = 0,		/* {PS} AUD_PWR_EN use GPIO_142 */
			.supply = "AVDD1",	/* {PS} */
			.init_data = &wm8994_ldo1,
		},
		{
			.enable = 142,		/* {PS} GPIO 142 is common for both LDO1 and LDO2, control is given to LDO1 */
			.supply = "DCVDD",	/* {PS} */
			.init_data = &wm8994_ldo2,
		}	  
	},
	.irq_base = OMAP_TEMP_IRQ_BASE,//OMAP_GPIO_IRQ(113),	/* {PS} : AUD_INT */

        .num_drc_cfgs = 0,		/* {PS} */
        .drc_cfgs = NULL,		/* {PS} */

        .num_retune_mobile_cfgs = 0,	/* {PS} */
        .retune_mobile_cfgs = NULL,	/* {PS} */

        /* LINEOUT can be differential or single ended */
        .lineout1_diff = 1,		/* {PS} */
        .lineout2_diff = 1,		/* {PS} */

        /* Common mode feedback */
        .lineout1fb = 1,			/* {PS} */
        .lineout2fb = 1,			/* {PS} */

        /* Microphone biases: 0=0.9*AVDD1 1=0.65*AVVD1 */
        .micbias1_lvl = 0,		/* {PS} */
        .micbias2_lvl = 0,		/* {PS} */

        /* Jack detect threashold levels, see datasheet for values */
        .jd_scthr = 1,			/* {RD} */
        .jd_thr = 0,			/* {RD} */
};
/* {PS} END: wm8994 regulator info.  */

/* {PS} BEGIN: LIS331DLH accelerometer data */
struct lis331dlh_platform_data  lis331dlh_omap3beagle_data  = {

	.min_interval = 1,
	.poll_interval = 200,

	.g_range = LIS331DLH_G_8G,

	.axis_map_x = 0, /* {RD} axis correction */
	.axis_map_y = 1, /* {RD} axis correction */
	.axis_map_z = 2,

	.negate_x = 1,  
	.negate_y = 1,  
	.negate_z = 0,  
};
/* {PS} END: */

/* {RD} BEGIN: hmc5883l compass data */
struct hmc5883l_platform_data  hmc5883l_omap3beagle_data  = {
	.axis_map_x = 2,
	.axis_map_y = 0,
	.axis_map_z = 1,
	.negate_x = 0,  
	.negate_y = 0,  
	.negate_z = 1,  
};
/* {RD} END: */

/* {PS} BEGIN: */
static struct i2c_board_info __initdata beagle_i2c2_boardinfo[] = {
	{
		I2C_BOARD_INFO("battery", 0x55), /* {KW}  */
	},
	{
		I2C_BOARD_INFO("bq24150", 0x6b), /* {DW} */
	},
	{
		I2C_BOARD_INFO("wm8994", (0x34 >> 1)),  /* {PS} */
		#if 0
		.flags = I2C_CLIENT_WAKE,
 		.irq = INT_34XX_SYS_NIRQ,
 		#endif
 		.irq = OMAP_GPIO_IRQ(113),
		.platform_data = &wm8994_pdata,
	},
	{
		I2C_BOARD_INFO("lis331dlh", 0x18),	
		.platform_data = &lis331dlh_omap3beagle_data,
	},
	{
		I2C_BOARD_INFO("hmc5883l", 0x1E),  	/* {PK}  */
		.platform_data = &hmc5883l_omap3beagle_data, /* {RD}  */
	},
};

static struct i2c_board_info __initdata beagle_i2c3_boardinfo[] = {
/* { DSG : Set in board-omap3beagle-camera.c } */
/*	
	{
		I2C_BOARD_INFO("camera", 0x3C),  	
	},
*/
	{
		I2C_BOARD_INFO("lm3553", 0x53),		/* {PK} */
	},
};

static int __init omap3_beagle_i2c_init(void)
{
	omap_register_i2c_bus(1, 2600, beagle_i2c1_boardinfo,	/* {PS} */
			ARRAY_SIZE(beagle_i2c1_boardinfo));

	/* {PS} BEGIN: */		
	omap_register_i2c_bus(2, 100,  beagle_i2c2_boardinfo,  /* {DW} i2c running at 100kHz */
			ARRAY_SIZE(beagle_i2c2_boardinfo));				
	/* {PS} END: */

	/* Bus 3 is attached to the DVI port where devices like the pico DLP
	 * projector don't work reliably with 400kHz */
	
	/* {PS} BEGIN: */
	omap_register_i2c_bus(3, 400, beagle_i2c3_boardinfo, ARRAY_SIZE(beagle_i2c3_boardinfo));
	/* {PS} END: */

	return 0;
}

static void __init omap3_beagle_init_irq(void)
{
	omap2_init_common_infrastructure();
	omap2_init_common_devices(mt46h32m32lf6_sdrc_params,
				  mt46h32m32lf6_sdrc_params);
	omap_init_irq();
	gpmc_init();
#ifdef CONFIG_OMAP_32K_TIMER
	if (omap3_beagle_version == OMAP3BEAGLE_BOARD_AXBX)
		omap2_gp_clockevent_set_gptimer(12);
	else
		omap2_gp_clockevent_set_gptimer(1);
#endif
}

/* {PS} BEGIN: */
static struct platform_device cp430_keypad = {
	.name	= "cp430_keypad",	
	.id	= -1,
};
/* {PS} END: */



static struct platform_device *omap3_beagle_devices[] __initdata = {
	&beagle_dss_device,
	&usb_mass_storage_device,
/* {PS} BEGIN: */
#ifdef CONFIG_SND_SOC_WL1271BT
	&wl1271bt_audio_device,
	&wl1271bt_codec_device,
#endif
/* {PS} END: */
/* {PS} BEGIN: 	*/
	&cp430_keypad,
/* {PS} END: 	*/
};

static void __init omap3beagle_flash_init(void)
{
	u8 cs = 0;
	u8 nandcs = GPMC_CS_NUM + 1;

	/* find out the chip-select on which NAND exists */
	while (cs < GPMC_CS_NUM) {
		u32 ret = 0;
		ret = gpmc_cs_read_reg(cs, GPMC_CS_CONFIG1);

		if ((ret & 0xC00) == 0x800) {
			printk(KERN_INFO "Found NAND on CS%d\n", cs);
			if (nandcs > GPMC_CS_NUM)
				nandcs = cs;
		}
		cs++;
	}

	if (nandcs > GPMC_CS_NUM) {
		printk(KERN_INFO "NAND: Unable to find configuration "
				 "in GPMC\n ");
		return;
	}

	if (nandcs < GPMC_CS_NUM) {
		printk(KERN_INFO "Registering NAND on CS%d\n", nandcs);
		board_nand_init(omap3beagle_nand_partitions,
			ARRAY_SIZE(omap3beagle_nand_partitions),
			nandcs, NAND_BUSWIDTH_16);
	}
}

/* {PS} BEGIN: */
static const struct ehci_hcd_omap_platform_data ehci_pdata __initconst = {

	.port_mode[0] = EHCI_HCD_OMAP_MODE_PHY,
	.port_mode[1] = EHCI_HCD_OMAP_MODE_PHY,
	.port_mode[2] = EHCI_HCD_OMAP_MODE_UNKNOWN,

	.phy_reset  = true,
	.reset_gpio_port[0]  = -EINVAL,
	.reset_gpio_port[1]  = 147,
	.reset_gpio_port[2]  = -EINVAL
};
/* {PS} END: */

#ifdef CONFIG_OMAP_MUX
static struct omap_board_mux board_mux[] __initdata = {
/* {PS} BEGIN: */
#ifdef CONFIG_WL12XX_PLATFORM_DATA
	/* WLAN IRQ - GPIO 149 */
	OMAP3_MUX(UART1_RTS, OMAP_MUX_MODE4 | OMAP_PIN_INPUT),

	/* WLAN POWER ENABLE - GPIO 150 */
	OMAP3_MUX(UART1_CTS, OMAP_MUX_MODE4 | OMAP_PIN_OUTPUT),

	/* MMC3 SDIO pin muxes for WL12xx */
	OMAP3_MUX(MCSPI1_CS2, OMAP_MUX_MODE3 | OMAP_PIN_INPUT_PULLUP),		/*MMC3_CLKO*/
	OMAP3_MUX(MCSPI1_CS1, OMAP_MUX_MODE3 | OMAP_PIN_INPUT_PULLUP),		/*MMC3_CMD*/
	OMAP3_MUX(ETK_D4, OMAP_MUX_MODE2 | OMAP_PIN_INPUT_PULLUP),		/*MMC3_DAT0*/
	OMAP3_MUX(ETK_D5, OMAP_MUX_MODE2 | OMAP_PIN_INPUT_PULLUP),		/*MMC3_DAT1*/
	OMAP3_MUX(ETK_D6, OMAP_MUX_MODE2 | OMAP_PIN_INPUT_PULLUP),		/*MMC3_DAT2*/
	OMAP3_MUX(ETK_D3, OMAP_MUX_MODE2 | OMAP_PIN_INPUT_PULLUP),		/*MMC3_DAT3*/
#endif
/* {PS} END: */
    OMAP3_MUX(SYS_NIRQ, OMAP_MUX_MODE0 | OMAP_PIN_INPUT_PULLUP |
                OMAP_PIN_OFF_INPUT_PULLUP | OMAP_PIN_OFF_OUTPUT_LOW |
                OMAP_PIN_OFF_WAKEUPENABLE),
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};
#endif

static struct omap_musb_board_data musb_board_data = {
	.interface_type		= MUSB_INTERFACE_ULPI,
	.mode			= MUSB_OTG,
	.power			= 100,
};

/* {PS} BEGIN: TCBIN GPIO initialization */
static void __init omap3_tcbin_gpio_init(void)
{
	/* {PS} : Request GPIOs */
/*  gpio_request(98,  "CAM_nRST");	*/			/* {KW} : Added in board-omap3beagle-camera.c */
/*	gpio_request(167, "CAM_PWDN");	*/			/* {KW} : Added in board-omap3beagle-camera.c */
	gpio_request(157, "CAM_LED_nRST");			/* {PS} : CAM_LED_nRST		*/
	gpio_request(11,  "3GM_SEC_PWR_EN");		/* {RD} : 3GM_SEC_PWR_EN	*/
//	gpio_request(12,  "CP_INT");				/* {PS} : CP_INT			*/
	gpio_request(13,  "3GM_UART_DCD_INT");		/* {PS} : 3GM_UART_DCD_INT	*/
//	gpio_request(14,  "ACC_INT");				/* {PS} : ACC_INT			*/
	gpio_request(15,  "3GM_UART_RI_INT");		/* {PS} : 3GM_UART_RI_INT	*/
	gpio_request(16,  "3GM_PWR_EN");			/* {PS} : 3GM_PWR_nEN		*/
//	gpio_request(21,  "USB_PWR_EN");			/* {PS} : USB_PWR_EN		*/
	gpio_request(23,  "3GM_OE");				/* {PS} : 3GM_OE			*/
	gpio_request(64,  "OMAP_STATUS_1");			/* {RD} : OMAP_STATUS_1		*/
	gpio_request(65,  "OMAP_STATUS_2");			/* {RD} : OMAP_STATUS_2		*/
	gpio_request(113, "AUD_INT");				/* {RD} : AUD_INT			*/
	gpio_request(126, "TV_PWR_EN");				/* {PS} : TV_PWR_EN			*/
	gpio_request(127, "TV_OUT_EN");				/* {PS} : TV_OUT_EN			*/
	gpio_request(128, "3GM_W_nDSBL");			/* {PS} : 3GM_W_nDSBL		*/
	gpio_request(129, "3GM_RST");				/* {PS} : 3GM_RST			*/
//	gpio_request(140, "GPS_nEN");				/* {PS} : GPS_nEN			*/
	gpio_request(141, "PWR03_EN");				/* {PS} : PWR03_EN			*/
//	gpio_request(142, "AUD_PWR_EN");			/* {PS} : AUD_PWR_EN		*/
//	gpio_request(147, "USB2HS_nRST");			/* {PS} : USB2HS_nRST		*/
//	gpio_request(149, "WL_INT");				/* {PS} : WL_INT			*/
	gpio_request(150, "WL_EN");					/* {PS} : WL_EN				*/
/*	gpio_request(152, "CAM_PWR_EN");		*/	/* {KW} : Added in board-omap3beagle-camera.c */
	gpio_request(153, "CHRG_OTG");				/* {PS} : CHRG_OTG			*/
	gpio_request(154, "3GM_GPIO_1");			/* {PS} : 3GM_GPIO_1		*/
	gpio_request(155, "BT_WKUP");				/* {PS} : BT_WKUP			*/
	gpio_request(156, "FM_EN");					/* {PS} : FM_EN				*/
//	gpio_request(163, "GPS_PWR_EN");			/* {PS} : GPS_PWR_EN		*/
//	gpio_request(164, "MMC1_WP");				/* {PS} : MMC1_WP			*/


	/* {PS} : Set direction */
//	gpio_direction_input(12);			/* {PS} : CP_INT			*/	/* Input */
	gpio_direction_input(13);			/* {PS} : 3GM_UART_DCD_INT	*/	/* Input */
//	gpio_direction_input(14);			/* {PS} : ACC_INT			*/	/* Input */
	gpio_direction_input(15);			/* {PS} : 3GM_UART_RI_INT	*/	/* Input */
	gpio_direction_input(113);			/* {RD} : AUD_INT			*/	/* Input */
//	gpio_direction_input(149);			/* {PS} : WL_INT			*/	/* Input */
	gpio_direction_input(154);			/* {PS} : 3GM_GPIO_1		*/	/* Input */
	gpio_direction_input(155);			/* {PS} : BT_WKUP			*/	/* Input */	
//	gpio_direction_input(164);			/* {PS} : MMC1_WP			*/	/* Input */
	
	gpio_direction_output(11, 1);		/* {RD} : 3GM_SEC_PWR_EN	- HIGH	- Enable power*/
	gpio_direction_output(64, 1);		/* {RD} : OMAP_STATUS_1		- HIGH	- Notify MSP430 that the OMAP is up*/
	gpio_direction_output(65, 0);		/* {RD} : OMAP_STATUS_0		- LOW	- Notify MSP430 that the OMAP not restarting*/
	/*	gpio_direction_output(98, 0);	*/	/* {KW} : Added in board-omap3beagle-camera.c */
	/*	gpio_direction_output(167, 1);	*/	/* {KW} : Added in board-omap3beagle-camera.c */
	gpio_direction_output(157, 0);		/* {PS} : CAM_LED_nRST		- LOW 	- Reset Camera LED driver */
	gpio_direction_output(16, 0);		/* {PS} : 3GM_PWR_nEN		- LOW	- Turn on 3G modem power supply */
//	gpio_direction_output(21, 0);		/* {PS} : USB_PWR_EN		- LOW	- Turn off USB Hub power supply */
	gpio_direction_output(23, 0);		/* {PS} : 3GM_OE			- LOW 	- Disconnect 3G modem data bus */
	gpio_direction_output(126, 0);		/* {PS} : TV_PWR_EN			- LOW	- Turn off TV power supply */
	gpio_direction_output(127, 1);		/* {PS} : TV_OUT_EN			- HIGH 	- Disable TV out */
	gpio_direction_output(128, 1);		/* {PS} : 3GM_W_nDSBL		- HIGH	- Enable 3G modem */
	gpio_direction_output(129, 0);		/* {PS} : 3GM_RST			- LOW	- Not Reset 3G modem */
//	gpio_direction_output(140, 1);		/* {PS} : GPS_nEN			- HIGH	- Disable GPS */
	gpio_direction_output(141, 0);		/* {PS} : PWR03_EN			- LOW	- Turn off Wi-Fi power supply */
//	gpio_direction_output(142, 0);		/* {PS} : AUD_PWR_EN		- LOW 	- Turn off Audio power supply */
//	gpio_direction_output(147, 0);		/* {PS} : USB2HS_nRST		- LOW	- Reset USB Transciever */
	gpio_direction_output(150, 0);		/* {PS} : WL_EN				- LOW	- Disable Wi-Fi */
	/*  gpio_direction_output(152, 0);	*/	/* {AH} : Added in board-omap3beagle-camera.c */
	gpio_direction_output(153, 0);		/* {PS} : CHRG_OTG			- LOW	- Disable OTG mode */
	gpio_direction_output(156, 0);		/* {PS} : FM_EN				- LOW	- Disable FM */
//	gpio_direction_output(163, 0);		/* {PS} : GPS_PWR_EN		- LOW	- Turn off GPS power supply */

	
	/* {PS} : Set output value */
//	gpio_set_value(98, 0);		/* {KW} : Added in board-omap3beagle-camera.c */
//	gpio_set_value(167, 1);		/* {KW} : Added in board-omap3beagle-camera.c */
	gpio_set_value(11, 1);		/* {RD} : 3GM_SEC_PWR_EN	- HIGH	- Enable power*/

	gpio_set_value(157, 1);		/* {PS} : CAM_LED_nRST		- HIGH 	- Not reset Camera LED driver */
//	gpio_set_value(157, 0);		/* {PS} : CAM_LED_nRST		- LOW 	- Reset Camera LED driver */

//	gpio_set_value(16, 1);		/* {PS} : 3GM_PWR_nEN		- HIGH	- Turn off 3G modem power supply */	
	gpio_set_value(16, 0);		/* {PS} : 3GM_PWR_nEN		- LOW	- Turn on 3G modem power supply */

//	gpio_set_value(21, 1);		/* {PS} : USB_PWR_EN		- HIGH	- Turn on USB Hub power supply */
//	gpio_set_value(21, 0);		/* {PS} : USB_PWR_EN		- LOW	- Turn off USB Hub power supply */
	
//	gpio_set_value(23, 0);		/* {PS} : 3GM_OE			- LOW 	- Disconnect 3G modem data bus */
	gpio_set_value(23, 1);		/* {PS} : 3GM_OE			- HIGH 	- Connect 3G modem data bus */
	
	gpio_set_value(64, 1);		/* {RD} : OMAP_STATUS_1		- HIGH	- Notify MSP430 that the OMAP is up*/
	gpio_set_value(65, 0);		/* {RD} : OMAP_STATUS_2		- LOW	- Notify MSP430 that the OMAP is not restarting*/
	
//	gpio_set_value(126, 1);		/* {PS} : TV_PWR_EN			- HIGH	- Turn on TV power supply */
	gpio_set_value(126, 0);		/* {PS} : TV_PWR_EN			- LOW	- Turn off TV power supply */
	
//	gpio_set_value(127, 1);		/* {PS} : TV_OUT_EN			- HIGH 	- Disable TV out */	
	gpio_set_value(127, 0);		/* {PS} : TV_OUT_EN			- LOW 	- Enable TV out */
	
	gpio_set_value(128, 1);		/* {PS} : 3GM_W_nDSBL		- HIGH	- Enable 3G modem radio */
//	gpio_set_value(128, 0);		/* {PS} : 3GM_W_nDSBL		- LOW	- Disable 3G modem  radio */
	
//	gpio_set_value(129, 1);		/* {PS} : 3GM_RST		- HIGH	- Reset 3G modem */
	gpio_set_value(129, 0);		/* {PS} : 3GM_RST		- LOW	- Not reset 3G modem */	
	
//	gpio_set_value(140, 1);		/* {PS} : GPS_nEN			- HIGH	- Disable GPS */
//	gpio_set_value(140, 0);		/* {PS} : GPS_nEN			- LOW	- Enable GPS */

	gpio_set_value(141, 1);		/* {PS} : PWR03_EN			- HIGH	- Turn on Wi-Fi power supply */
//	gpio_set_value(141, 0);		/* {PS} : PWR03_EN			- LOW	- Turn off Wi-Fi power supply */
	
//	gpio_set_value(142, 1);		/* {PS} : AUD_PWR_EN		- HIGH 	- Turn on Audio power supply */
//	gpio_set_value(142, 0);		/* {PS} : AUD_PWR_EN		- LOW 	- Turn off Audio power supply */
	
//	gpio_set_value(147, 0);		/* {PS} : USB2HS_nRST		- LOW	- Reset USB Transciever */

	gpio_set_value(150, 1);		/* {PS} : WL_EN				- HIGH	- Enable Wi-Fi */
	printk(KERN_INFO "WLAN GPIO enabled\n");
//	gpio_set_value(150, 0);		/* {PS} : WL_EN				- LOW	- Disable Wi-Fi */

//	gpio_set_value(152, 1);		/* {PS} : CAM_PWR_EN		- HIGH	- Turn on Camera power supply */
//	gpio_set_value(152, 0);		/* {PS} : CAM_PWR_EN		- LOW	- Turn off Camera power supply */	

	gpio_set_value(153, 0);		/* {PS} : CHRG_OTG			- LOW	- Disable OTG mode */
	
	gpio_set_value(156, 0);		/* {PS} : FM_EN				- LOW	- Disable FM */
	
//	gpio_set_value(163, 1);		/* {PS} : GPS_PWR_EN		- HIGH	- Turn on GPS power supply */
//	gpio_set_value(163, 0);		/* {PS} : GPS_PWR_EN		- LOW	- Turn off GPS power supply */	
	
}
/* {PS} END: */


static void __init omap3_beagle_init(void)
{
	
	printk("%s BEAGLEBOARD INIT\n", __FUNCTION__);
	
	omap3_mux_init(board_mux, OMAP_PACKAGE_CBB);
	omap3_beagle_init_rev();
	
	/* {PS} BEGIN: TCBIN GPIO initialization */
	omap3_tcbin_gpio_init();
	/* {PS} END: */
	
	omap3_beagle_i2c_init();
	platform_add_devices(omap3_beagle_devices,
			ARRAY_SIZE(omap3_beagle_devices));
	omap_serial_init();

	/* {PS} BEGIN: */
	spi_register_board_info(beagle_mcspi_board_info,ARRAY_SIZE(beagle_mcspi_board_info)); /* {AE} */
	/* {PS} END: */

	usb_musb_init(&musb_board_data);
	usb_ehci_init(&ehci_pdata);
	omap3beagle_flash_init();

	/* Ensure SDRC pins are mux'd for self-refresh */
	omap_mux_init_signal("sdrc_cke0", OMAP_PIN_OUTPUT);
	omap_mux_init_signal("sdrc_cke1", OMAP_PIN_OUTPUT);

	beagle_display_init();

#ifdef CONFIG_USB_ANDROID
	omap3beagle_android_gadget_init();
#endif
	omap3_beagle_pm_init();

	/* {RD} */
	register_reboot_notifier(&tcbin_reboot_notifier);					
}

MACHINE_START(OMAP3_BEAGLE, "OMAP3 Beagle Board")
	/* Maintainer: Syed Mohammed Khasim - http://beagleboard.org */
	.boot_params	= 0x80000100,
	.map_io		= omap3_map_io,
	.reserve	= omap_reserve,
	.init_irq	= omap3_beagle_init_irq,
	.init_machine	= omap3_beagle_init,
	.timer		= &omap_timer,
MACHINE_END
