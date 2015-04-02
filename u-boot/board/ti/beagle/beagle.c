/*
 * (C) Copyright 2004-2008
 * Texas Instruments, <www.ti.com>
 *
 * Author :
 *	Sunil Kumar <sunilsaini05@gmail.com>
 *	Shashi Ranjan <shashiranjanmca05@gmail.com>
 *
 * Derived from Beagle Board and 3430 SDP code by
 *	Richard Woodruff <r-woodruff2@ti.com>
 *	Syed Mohammed Khasim <khasim@ti.com>
 *
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <twl4030.h>
#include <asm/io.h>
#include <asm/arch/mux.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/gpio.h>
#include <asm/mach-types.h>
#include "beagle.h"
#include <linux/mtd/nand.h>

/* {PS} BEGIN: */
static int beagle_revision = REVISION_C4; /* {PS} : This u-boot is only for Rev C4 based design */ 
/* static int beagle_revision; */
/* {PS} END: */

/*
 * Routine: board_init
 * Description: Early hardware init.
 */
int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	gpmc_init(); /* in SRAM or SDRAM, finish GPMC */
	/* board id for Linux */
	gd->bd->bi_arch_number = MACH_TYPE_OMAP3_BEAGLE;
	/* boot param addr */
	gd->bd->bi_boot_params = (OMAP34XX_SDRC_CS0 + 0x100);

	return 0;
}

/*
 * Routine: beagle_get_revision
 * Description: Return the revision of the BeagleBoard this code is running on.
 */
int beagle_get_revision(void)
{
	return beagle_revision;
}

/*
 * Routine: beagle_identify
 * Description: Detect if we are running on a Beagle revision Ax/Bx,
 *		C1/2/3, C4 or D. This can be done by reading
 *		the level of GPIO173, GPIO172 and GPIO171. This should
 *		result in
 *		GPIO173, GPIO172, GPIO171: 1 1 1 => Ax/Bx
 *		GPIO173, GPIO172, GPIO171: 1 1 0 => C1/2/3
 *		GPIO173, GPIO172, GPIO171: 1 0 1 => C4
 *		GPIO173, GPIO172, GPIO171: 0 0 0 => XM
 */
void beagle_identify(void)
{
	omap_request_gpio(171);
	omap_request_gpio(172);
	omap_request_gpio(173);
	omap_set_gpio_direction(171, 1);
	omap_set_gpio_direction(172, 1);
	omap_set_gpio_direction(173, 1);

	beagle_revision = omap_get_gpio_datain(173) << 2 |
			  omap_get_gpio_datain(172) << 1 |
			  omap_get_gpio_datain(171);
	omap_free_gpio(171);
	omap_free_gpio(172);
	omap_free_gpio(173);
}

/*
 * Routine: misc_init_r
 * Description: Configure board specific parts
 */
int misc_init_r(void)
{
	struct gpio *gpio5_base = (struct gpio *)OMAP34XX_GPIO5_BASE;
	struct gpio *gpio6_base = (struct gpio *)OMAP34XX_GPIO6_BASE;
	int ret;
	unsigned char val;
	char *s;
	
	char *ecc[3]     = { "nandecc", "sw", NULL, };
	char addr[32];
	char *read[6] = { "nand", "read", NULL,
				"0xF80000", "0x1000", NULL, };
	unsigned char buf[4096];				
				
	/*
	 * Configure drive strength for IO cells
	 */
	*(ulong *)(CONTROL_PROG_IO1) &= ~(PRG_I2C2_PULLUPRESX);

	/* beagle_identify(); */ /* {PS} : beagle_revision has been set to REVISION_C4 */

	twl4030_power_init();
	
	/* twl4030_led_init(TWL4030_LED_LEDEN_LEDAON | TWL4030_LED_LEDEN_LEDBON); */ /* {PS} : disable TPS6590 LEDs */

	switch (beagle_revision) {
	case REVISION_AXBX:
		printf("Beagle Rev Ax/Bx\n");
		setenv("mpurate", "600");
		break;
	case REVISION_CX:
		printf("Beagle Rev C1/C2/C3\n");
		MUX_BEAGLE_C();
		setenv("mpurate", "600");
		break;
	case REVISION_C4:
		printf("TCBIN Rev C\n"); /* {PS} : printf("Beagle Rev C4\n"); */
		/* {PS} : MUX_BEAGLE_C(); */
		/* Set VAUX2 to 1.8V for EHCI PHY */
		twl4030_pmrecv_vsel_cfg(TWL4030_PM_RECEIVER_VAUX2_DEDICATED,
					TWL4030_PM_RECEIVER_VAUX2_VSEL_18,
					TWL4030_PM_RECEIVER_VAUX2_DEV_GRP,
					TWL4030_PM_RECEIVER_DEV_GRP_P1);
		/* {KW}: setenv("mpurate", "720"); */
		setenv("mpurate", "1000"); /*{KW}: value for beagle-XM */		
		break;
	case REVISION_XM:
	case REVISION_XMC:
		printf("Beagle xM Rev A/C\n");
		MUX_BEAGLE_XM();
		/* Set VAUX2 to 1.8V for EHCI PHY */
		twl4030_pmrecv_vsel_cfg(TWL4030_PM_RECEIVER_VAUX2_DEDICATED,
					TWL4030_PM_RECEIVER_VAUX2_VSEL_18,
					TWL4030_PM_RECEIVER_VAUX2_DEV_GRP,
					TWL4030_PM_RECEIVER_DEV_GRP_P1);
		setenv("mpurate", "1000");
		break;
	default:
		printf("Beagle unknown 0x%02x\n", beagle_revision);
	}
	
	// {RD} Begin: check conditions to load recovery mode	
	twl4030_i2c_read_u8(TWL4030_CHIP_RTC,&val,(TWL4030_BASEADD_RTC+12));	
	if(val == 14){		
		printf("Entering recovery mode via TWL RTC flag\n");
		setenv("dorecovery","1");		
	}else{
		do_switch_ecc(NULL, 0, 2, ecc);
		sprintf(addr, "0x%x", buf);
		read[2] = addr;
		do_nand(NULL, 0, 5, read);
		if(!strncmp(buf+2048, "boot-recovery", 13)){
				printf("Entering recovery mode via bootloader control block flag\n");
				setenv("dorecovery","1");
		}else
			setenv("dorecovery","0");		
	}								
	// {RD} End: 
		
	/* Configure GPIOs to output */
	
	/* {PS} BEGIN */
	writel(~(GPIO10 | GPIO8 | GPIO2 | GPIO1), &gpio6_base->oe);
	writel(~(GPIO31 | GPIO30 | GPIO29 | GPIO28 | GPIO22), &gpio5_base->oe);
	/* writel(~(GPIO23 | GPIO10 | GPIO8 | GPIO2 | GPIO1), &gpio6_base->oe); */
	/* writel(~(GPIO31 | GPIO30 | GPIO29 | GPIO28 | GPIO22 | GPIO21 | GPIO15 | GPIO14 | GPIO13 | GPIO12), &gpio5_base->oe); */
	/* {PS} END: */
	
	/* Set GPIOs */
	/* {PS} BEGIN: */
	writel(GPIO10 | GPIO8 | GPIO2 | GPIO1, &gpio6_base->setdataout);
	writel(GPIO31 | GPIO30 | GPIO29 | GPIO28 | GPIO22, &gpio5_base->setdataout);
	/* writel(GPIO23 | GPIO10 | GPIO8 | GPIO2 | GPIO1, &gpio6_base->setdataout); */
	/* writel(GPIO31 | GPIO30 | GPIO29 | GPIO28 | GPIO22 | GPIO21 | GPIO15 | GPIO14 | GPIO13 | GPIO12, &gpio5_base->setdataout); */
	/* {PS} END: */
	
	/* {PS} BEGIN: Set GPIO direction and value */
	
	/* {PS} : Request GPIOs */
	omap_request_gpio(98);				/* {PS} : CAM_nRST			*/
	omap_request_gpio(167);				/* {PS} : CAM_PWDN			*/
	omap_request_gpio(157);				/* {PS} : CAM_LED_nRST		*/
	omap_request_gpio(11);				/* {RD} : 3GM_SEC_PWR_EN	*/
	omap_request_gpio(12);				/* {PS} : CP_INT			*/
	omap_request_gpio(13);				/* {PS} : 3GM_UART_DCD_INT	*/
	omap_request_gpio(14);				/* {PS} : ACC_INT			*/
	omap_request_gpio(15);				/* {PS} : 3GM_UART_RI_INT	*/
	omap_request_gpio(16);				/* {PS} : 3GM_PWR_nEN		*/
	omap_request_gpio(21);				/* {PS} : USB_PWR_EN		*/
	omap_request_gpio(23);				/* {PS} : 3GM_OE			*/
	omap_request_gpio(53);				/* {RD} : emmc reset		*/
	omap_request_gpio(54);				/* {RD} : Enable U107		*/
	omap_request_gpio(64);				/* {RD} : OMAP_STATUS_1		*/
	omap_request_gpio(126);				/* {PS} : TV_PWR_EN			*/
	omap_request_gpio(127);				/* {PS} : TV_OUT_EN			*/
	omap_request_gpio(128);				/* {PS} : 3GM_W_nDSBL		*/
	omap_request_gpio(129);				/* {PS} : 3GM_RST		*/
	omap_request_gpio(140);				/* {PS} : GPS_nEN			*/
	omap_request_gpio(141);				/* {PS} : PWR03_EN			*/
	omap_request_gpio(142);				/* {PS} : AUD_PWR_EN		*/
	omap_request_gpio(147);				/* {PS} : USB2HS_nRST		*/
	omap_request_gpio(149);				/* {PS} : WL_INT			*/
	omap_request_gpio(150);				/* {PS} : WL_EN				*/
	omap_request_gpio(152);				/* {PS} : CAM_PWR_EN		*/
	omap_request_gpio(153);				/* {PS} : CHRG_OTG			*/
	omap_request_gpio(154);				/* {PS} : 3GM_GPIO_1		*/
	omap_request_gpio(155);				/* {PS} : BT_WKUP			*/
	omap_request_gpio(156);				/* {PS} : FM_EN				*/
	omap_request_gpio(163);				/* {PS} : GPS_PWR_EN		*/
	omap_request_gpio(164);				/* {PS} : MMC1_WP			*/


	/* {PS} : Set direction */
	omap_set_gpio_direction(98, 0);		/* {PS} : CAM_nRST			*/
	omap_set_gpio_direction(167, 0);	/* {PS} : CAM_PWDN			*/
	omap_set_gpio_direction(157, 0);	/* {PS} : CAM_LED_nRST		*/
	omap_set_gpio_direction(53, 0);		/* {RD} : emmc reset		*/
	omap_set_gpio_direction(54, 0);		/* {RD} : Enable U107		*/
	omap_set_gpio_direction(64, 0);		/* {RD} : OMAP_STATUS_1		*/	
	omap_set_gpio_direction(11, 0);		/* {RD} : 3GM_SEC_PWR_EN	*/	
	omap_set_gpio_direction(12, 1);		/* {PS} : CP_INT			*/	/* Input */
	omap_set_gpio_direction(13, 1);		/* {PS} : 3GM_UART_DCD_INT	*/	/* Input */
	omap_set_gpio_direction(14, 1);		/* {PS} : ACC_INT			*/	/* Input */
	omap_set_gpio_direction(15, 1);		/* {PS} : 3GM_UART_RI_INT	*/	/* Input */
	omap_set_gpio_direction(16, 0);		/* {PS} : 3GM_PWR_nEN		*/
	omap_set_gpio_direction(21, 0);		/* {PS} : USB_PWR_EN		*/
	omap_set_gpio_direction(23, 0);		/* {PS} : 3GM_OE			*/
	omap_set_gpio_direction(126, 0);	/* {PS} : TV_PWR_EN			*/
	omap_set_gpio_direction(127, 0);	/* {PS} : TV_OUT_EN			*/
	omap_set_gpio_direction(128, 0);	/* {PS} : 3GM_W_nDSBL		*/
	omap_set_gpio_direction(129, 0);	/* {PS} : 3GM_RST		*/
	omap_set_gpio_direction(140, 0);	/* {PS} : GPS_nEN			*/
	omap_set_gpio_direction(141, 0);	/* {PS} : PWR03_EN			*/
	omap_set_gpio_direction(142, 0);	/* {PS} : AUD_PWR_EN		*/
	omap_set_gpio_direction(147, 0);	/* {PS} : USB2HS_nRST		*/
	omap_set_gpio_direction(149, 1);	/* {PS} : WL_INT			*/	/* Input */
	omap_set_gpio_direction(150, 0);	/* {PS} : WL_EN				*/
	omap_set_gpio_direction(152, 0);	/* {PS} : CAM_PWR_EN		*/
	omap_set_gpio_direction(153, 0);	/* {PS} : CHRG_OTG			*/
	omap_set_gpio_direction(154, 1);	/* {PS} : 3GM_GPIO_1		*/	/* Input */
	omap_set_gpio_direction(155, 0);	/* {PS} : BT_WKUP			*/
	omap_set_gpio_direction(156, 0);	/* {PS} : FM_EN				*/
	omap_set_gpio_direction(163, 0);	/* {PS} : GPS_PWR_EN		*/
	omap_set_gpio_direction(164, 1);	/* {PS} : MMC1_WP			*/	/* Input */

	
	/* {PS} : Set output value */
	omap_set_gpio_dataout(98, 0);		/* {PS} : CAM_nRST			- LOW	- Reset Camera */
	omap_set_gpio_dataout(167, 1);		/* {PS} : CAM_PWDN			- HIGH 	- Power down Camera */
	omap_set_gpio_dataout(157, 0);		/* {PS} : CAM_LED_nRST		- LOW 	- Reset Camera LED driver */
	omap_set_gpio_dataout(53, 1);		/* {RD} : emmc reset		- LOW	*/
	omap_set_gpio_dataout(54, 0);		/* {RD} : Enable U107		- LOW	*/
	omap_set_gpio_dataout(64, 0);		/* {RD} : OMAP_STATUS_1		- LOW	- PWR STATUS Gpio pin low */
	omap_set_gpio_dataout(11, 1);		/* {RD} : 3GM_SEC_PWR_EN	- HIGH	- Turn on 3G modem via secondary enable pin */	
	omap_set_gpio_dataout(16, 0);		/* {PS} : 3GM_PWR_nEN		- LOW	- Turn on 3G modem power supply */
	omap_set_gpio_dataout(21, 0);		/* {PS} : USB_PWR_EN		- LOW	- Turn off USB Hub power supply */
	omap_set_gpio_dataout(23, 0);		/* {PS} : 3GM_OE			- LOW 	- Disconnect 3G modem data bus */
	omap_set_gpio_dataout(126, 0);		/* {PS} : TV_PWR_EN			- LOW	- Turn off TV power supply */
	omap_set_gpio_dataout(127, 0);		/* {PS} : TV_OUT_EN			- LOW 	- Disable TV out */
	omap_set_gpio_dataout(128, 1);		/* {PS} : 3GM_W_nDSBL		- HIGH	- Enable 3G modem */
	omap_set_gpio_dataout(129, 0);		/* {PS} : 3GM_RST		- LOW	- Not reset 3G modem */
	omap_set_gpio_dataout(140, 1);		/* {PS} : GPS_nEN			- HIGH	- Disable GPS */
	omap_set_gpio_dataout(141, 0);		/* {PS} : PWR03_EN			- LOW	- Turn off Wi-Fi power supply */
	omap_set_gpio_dataout(142, 0);		/* {PS} : AUD_PWR_EN		- LOW 	- Turn off Audio power supply */
	omap_set_gpio_dataout(147, 0);		/* {PS} : USB2HS_nRST		- LOW	- Reset USB Transciever */
	omap_set_gpio_dataout(150, 0);		/* {PS} : WL_EN				- LOW	- Disable Wi-Fi */
	omap_set_gpio_dataout(152, 0);		/* {PS} : CAM_PWR_EN		- LOW	- Turn off Camera power supply */
	omap_set_gpio_dataout(153, 0);		/* {PS} : CHRG_OTG			- LOW	- Disable OTG mode */
	omap_set_gpio_dataout(155, 0);		/* {PS} : BT_WKUP			- LOW	- Disable Bluetooth */
	omap_set_gpio_dataout(156, 0);		/* {PS} : FM_EN				- LOW	- Disable FM */
	omap_set_gpio_dataout(163, 0);		/* {PS} : GPS_PWR_EN		- LOW	- Turn off GPS power supply */

	/* {PS} END: */
	
	dieid_num_r();
	
	return 0;
}

/*
 * Routine: set_muxconf_regs
 * Description: Setting up the configuration Mux registers specific to the
 *		hardware. Many pins need to be moved from protect to primary
 *		mode.
 */
void set_muxconf_regs(void)
{
	/* {PS} BEGIN:		*/
	MUX_BEAGLE_TCBIN();
	/* MUX_BEAGLE(); */
	/* {PS} END: 		*/
}
