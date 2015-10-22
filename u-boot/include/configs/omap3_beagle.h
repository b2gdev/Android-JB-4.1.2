/*
 * (C) Copyright 2006-2008
 * Texas Instruments.
 * Richard Woodruff <r-woodruff2@ti.com>
 * Syed Mohammed Khasim <x0khasim@ti.com>
 *
 * Configuration settings for the TI OMAP3530 Beagle board.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_ARCH_CPU_INIT	1

/*
 * High Level Configuration Options
 */
#define CONFIG_ARMCORTEXA8	1	/* This is an ARM V7 CPU core */
#define CONFIG_OMAP		1	/* in a TI OMAP core */
#define CONFIG_OMAP34XX		1	/* which is a 34XX */
#define CONFIG_OMAP3430		1	/* which is in a 3430 */
#define CONFIG_OMAP3_BEAGLE	1	/* working with BEAGLE */

#define CONFIG_SDRC	/* The chip has SDRC controller */

#include <asm/arch/cpu.h>		/* get chip and board defs */
#include <asm/arch/omap3.h>

/*
 * Display CPU and Board information
 */
#define CONFIG_DISPLAY_CPUINFO		1
#define CONFIG_DISPLAY_BOARDINFO	1

/* Clock Defines */
#define V_OSCK			26000000	/* Clock output from T2 */
#define V_SCLK			(V_OSCK >> 1)

#undef CONFIG_USE_IRQ				/* no support for IRQs */
#define CONFIG_MISC_INIT_R

#define CONFIG_CMDLINE_TAG		1	/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_INITRD_TAG		1
#define CONFIG_REVISION_TAG		1

/*
 * Size of malloc() pool
 */
#define CONFIG_ENV_SIZE			(128 << 10)	/* 128 KiB */
						/* Sector */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + (128 << 10))
#define CONFIG_SYS_GBL_DATA_SIZE	128	/* bytes reserved for */
						/* initial data */

/*
 * Hardware drivers
 */

/*
 * NS16550 Configuration
 */
#define V_NS16550_CLK			48000000	/* 48MHz (APLL96/2) */

#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	(-4)
#define CONFIG_SYS_NS16550_CLK		V_NS16550_CLK

/*
 * select serial console configuration
 */
#define CONFIG_CONS_INDEX		3
#define CONFIG_SYS_NS16550_COM3		OMAP34XX_UART3
#define CONFIG_SERIAL3			3	/* UART3 on Beagle Rev 2 */

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{4800, 9600, 19200, 38400, 57600,\
					115200}
#define CONFIG_MMC			1
#define CONFIG_OMAP3_MMC		1
#define CONFIG_DOS_PARTITION		1

/* DDR - I use Micron DDR */
#define CONFIG_OMAP3_MICRON_DDR		1

/* USB
 * Enable CONFIG_MUSB_HCD for Host functionalities MSC, keyboard
 * Enable CONFIG_MUSB_UDC for Device functionalities.
 */
#define CONFIG_USB_OMAP3		1
#define CONFIG_MUSB_HCD			1
/* #define CONFIG_MUSB_UDC		1 */
#define CONFIG_TWL4030_USB		1

#ifdef CONFIG_USB_OMAP3

#ifdef CONFIG_MUSB_HCD
#define CONFIG_CMD_USB

#define CONFIG_USB_STORAGE
#define CONGIG_CMD_STORAGE
#define CONFIG_CMD_FAT

#ifdef CONFIG_USB_KEYBOARD
#define CONFIG_SYS_USB_EVENT_POLL
#define CONFIG_PREBOOT "usb start"
#endif /* CONFIG_USB_KEYBOARD */

#endif /* CONFIG_MUSB_HCD */

#ifdef CONFIG_MUSB_UDC
/* USB device configuration */
#define CONFIG_USB_DEVICE		1
#define CONFIG_USB_TTY			1
#define CONFIG_SYS_CONSOLE_IS_IN_ENV	1
/* Change these to suit your needs */
#define CONFIG_USBD_VENDORID		0x0451
#define CONFIG_USBD_PRODUCTID		0x5678
#define CONFIG_USBD_MANUFACTURER	"Texas Instruments"
#define CONFIG_USBD_PRODUCT_NAME	"Beagle"
#endif /* CONFIG_MUSB_UDC */

#endif /* CONFIG_USB_OMAP3 */
/* commands to include */
#include <config_cmd_default.h>

#define CONFIG_CMD_EXT2		/* EXT2 Support			*/
#define CONFIG_CMD_FAT		/* FAT support			*/
#define CONFIG_CMD_JFFS2	/* JFFS2 Support		*/
#define CONFIG_CMD_MTDPARTS	/* Enable MTD parts commands */
#define CONFIG_MTD_DEVICE	/* needed for mtdparts commands */
#define MTDIDS_DEFAULT			"nand0=nand"
#define MTDPARTS_DEFAULT		"mtdparts=nand:512k(x-loader),"\
					"1920k(u-boot),128k(u-boot-env),"\
					"8m(kernel),16m(recovery),1m(misc),"\
					"196m(cache),-(system)"

#define CONFIG_CMD_I2C		/* I2C serial bus support	*/
#define CONFIG_CMD_MMC		/* MMC support			*/
#define CONFIG_CMD_NAND		/* NAND support			*/

#undef CONFIG_CMD_FLASH		/* flinfo, erase, protect	*/
#undef CONFIG_CMD_FPGA		/* FPGA configuration Support	*/
#undef CONFIG_CMD_IMI		/* iminfo			*/
#undef CONFIG_CMD_IMLS		/* List all found images	*/
#undef CONFIG_CMD_NET		/* bootp, tftpboot, rarpboot	*/
#undef CONFIG_CMD_NFS		/* NFS support			*/

#define CONFIG_SYS_NO_FLASH
#define CONFIG_HARD_I2C			1
#define CONFIG_SYS_I2C_SPEED		100000
#define CONFIG_SYS_I2C_SLAVE		1
#define CONFIG_SYS_I2C_BUS		0
#define CONFIG_SYS_I2C_BUS_SELECT	1
#define CONFIG_DRIVER_OMAP34XX_I2C	1

/*
 * TWL4030
 */
#define CONFIG_TWL4030_POWER		1
#define CONFIG_TWL4030_LED		1

/*
 * Board NAND Info.
 */
#define CONFIG_SYS_NAND_QUIET_TEST	1
#define CONFIG_NAND_OMAP_GPMC
#define CONFIG_SYS_NAND_ADDR		NAND_BASE	/* physical address */
							/* to access nand */
#define CONFIG_SYS_NAND_BASE		NAND_BASE	/* physical address */
							/* to access nand at */
							/* CS0 */
#define GPMC_NAND_ECC_LP_x16_LAYOUT	1

#define CONFIG_SYS_MAX_NAND_DEVICE	1		/* Max number of NAND */
							/* devices */
#define CONFIG_JFFS2_NAND
/* nand device jffs2 lives on */
#define CONFIG_JFFS2_DEV		"nand0"
/* start of jffs2 partition */
#define CONFIG_JFFS2_PART_OFFSET	0x680000
#define CONFIG_JFFS2_PART_SIZE		0xf980000	/* size of jffs2 */
							/* partition */

/* Environment information */
#define CONFIG_BOOTDELAY		5

#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x82000000\0" \
	"console=ttyO2,115200n8\0" \
	"vram=8M\0" \
	"tvmode=ntsc\0" \
	"defaultdisplay=tv\0" \
	"mmcroot=/dev/mmcblk0p2 rw\0" \
	"mmcrootfstype=ext3 rootwait\0" \
	"cpxl=nandecc hw 2 ; fatload mmc 0:3 82000000 MLO ; nand erase 0 80000 ; nand write 82000000 0 6000\0" \
	"cpub=nandecc sw ; fatload mmc 0:3 0x82000000 u-boot.bin ; nand erase 0x80000 0x1E0000 ; nand write 0x82000000 0x80000 0x34800\0" \
	"recoverycmd=run nandargs ; nandecc sw ; nand read 0x82000000 0xA80000 0x1000000 ; bootm 0x82000000\0" \
	"cpuimage=nandecc sw ; fatload mmc 0:3 0x82000000 norm_uImage ; nand erase 0x280000 0x800000 ; nand write 0x82000000 0x280000 0x800000\0" \
	"cpbkuimage=nandecc sw ; fatload mmc 0:3 0x82000000 bk_uImage ; nand erase 0xA80000 0x1000000 ; nand write 0x82000000 0xA80000 0x900000\0" \
	"cpcache=nandecc sw ; fatload mmc 0:3 0x82000000 ubi_cache.img ; nand erase 0x1B80000 0xC000000 ; nand write 0x82000000 0x1B80000 0x200000\0" \
	"cpsys=nandecc sw ; fatload mmc 0:3 0x82000000 ubi_system.img ; nand erase 0xDF80000; nand write 0x82000000 0xDF80000 0xE000000\0" \
	"cpall=run cpxl ; run cpub ; run cpuimage ; run cpbkuimage ; run cpcache ; run cpsys\0" \
	"cpkernelsys=run cpuimage ; run cpcache ; run cpsys\0" \
	"mmcargs=setenv bootargs console=${console} " \
		"androidboot.console=ttyO2 mem=512M" \
		"rootdelay=1 init=/init ip=off omap_vout.vid1_static_vrfb_alloc=y " \
		"omapfb.vram=0:8M " \
		"vram=${vram} " \
		"omapfb.mode=tv:${tvmode} " \
		"omapdss.def_disp=${defaultdisplay} " \
		"root=${mmcroot} " \
		"rootfstype=${mmcrootfstype}\0" \
	"nandargs=setenv bootargs console=${console} " \
		"init=/init noinitrd ip=off androidboot.console=ttyO2 rootwait mem=512M " \
		"omap_vout.vid1_static_vrfb_alloc=y rw ubi.mtd=system,2048 ubi.mtd=cache,2048 rootwait no_console_suspend=1 " \
		"omapfb.vram=0:8M " \
		"vram=${vram} " \
		"omapfb.mode=tv:${tvmode} " \
		"omapdss.def_disp=${defaultdisplay} " \
	"loaduimage=fatload mmc 0 ${loadaddr} uImage\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"run mmcargs; " \
		"bootm ${loadaddr}\0" \
	"nandboot=echo Booting from nand ...; " \
		"run nandargs; " \
		"nandecc sw; "\
		"nand read ${loadaddr} 280000 3C0000; " \
		"bootm ${loadaddr}\0" \

#define CONFIG_BOOTCOMMAND \
	"if mmc init; then " \
		"if run loaduimage; then " \
			"run mmcboot; " \
		"else run nandboot; " \
		"fi; " \
	"else run nandboot; fi"

#define CONFIG_AUTO_COMPLETE		1
/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_SYS_PROMPT		"OMAP3 beagleboard.org # "
#define CONFIG_SYS_CBSIZE		512	/* Console I/O Buffer Size */
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS		32	/* max number of command args */
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE		(CONFIG_SYS_CBSIZE)

#define CONFIG_SYS_MEMTEST_START	(OMAP34XX_SDRC_CS0)	/* memtest */
								/* works on */
#define CONFIG_SYS_MEMTEST_END		(OMAP34XX_SDRC_CS0 + \
					0x01F00000) /* 31MB */

#define CONFIG_SYS_LOAD_ADDR		(OMAP34XX_SDRC_CS0)	/* default */
							/* load address */

/*
 * OMAP3 has 12 GP timers, they can be driven by the system clock
 * (12/13/16.8/19.2/38.4MHz) or by 32KHz clock. We use 13MHz (V_SCLK).
 * This rate is divided by a local divisor.
 */
#define CONFIG_SYS_TIMERBASE		(OMAP34XX_GPT2)
#define CONFIG_SYS_PTV			2       /* Divisor: 2^(PTV+1) => 8 */
#define CONFIG_SYS_HZ			1000

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	(128 << 10)	/* regular stack 128 KiB */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	(4 << 10)	/* IRQ stack 4 KiB */
#define CONFIG_STACKSIZE_FIQ	(4 << 10)	/* FIQ stack 4 KiB */
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	2	/* CS1 may or may not be populated */
#define PHYS_SDRAM_1		OMAP34XX_SDRC_CS0
#define PHYS_SDRAM_1_SIZE	(32 << 20)	/* at least 32 MiB */
#define PHYS_SDRAM_2		OMAP34XX_SDRC_CS1

/* SDRAM Bank Allocation method */
#define SDRC_R_B_C		1

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */

/* **** PISMO SUPPORT *** */

/* Configure the PISMO */
#define PISMO1_NAND_SIZE		GPMC_SIZE_128M
#define PISMO1_ONEN_SIZE		GPMC_SIZE_128M

#define CONFIG_SYS_MAX_FLASH_SECT	520	/* max number of sectors on */
						/* one chip */
#define CONFIG_SYS_MAX_FLASH_BANKS	2	/* max number of flash banks */
#define CONFIG_SYS_MONITOR_LEN		(256 << 10)	/* Reserve 2 sectors */

#define CONFIG_SYS_FLASH_BASE		boot_flash_base

/* Monitor at start of flash */
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_FLASH_BASE
#define CONFIG_SYS_ONENAND_BASE		ONENAND_MAP

#define CONFIG_ENV_IS_IN_NAND		1
#define ONENAND_ENV_OFFSET		0x260000 /* environment starts here */
#define SMNAND_ENV_OFFSET		0x260000 /* environment starts here */

#define CONFIG_SYS_ENV_SECT_SIZE	boot_flash_sec
#define CONFIG_ENV_OFFSET		boot_flash_off
#define CONFIG_ENV_ADDR			SMNAND_ENV_OFFSET

/*-----------------------------------------------------------------------
 * CFI FLASH driver setup
 */
/* timeout values are in ticks */
#define CONFIG_SYS_FLASH_ERASE_TOUT	(100 * CONFIG_SYS_HZ)
#define CONFIG_SYS_FLASH_WRITE_TOUT	(100 * CONFIG_SYS_HZ)

/* Flash banks JFFS2 should use */
#define CONFIG_SYS_MAX_MTD_BANKS	(CONFIG_SYS_MAX_FLASH_BANKS + \
					CONFIG_SYS_MAX_NAND_DEVICE)
#define CONFIG_SYS_JFFS2_MEM_NAND
/* use flash_info[2] */
#define CONFIG_SYS_JFFS2_FIRST_BANK	CONFIG_SYS_MAX_FLASH_BANKS
#define CONFIG_SYS_JFFS2_NUM_BANKS	1

#ifndef __ASSEMBLY__
extern unsigned int boot_flash_base;
extern volatile unsigned int boot_flash_env_addr;
extern unsigned int boot_flash_off;
extern unsigned int boot_flash_sec;
extern unsigned int boot_flash_type;
#endif

#endif /* __CONFIG_H */
