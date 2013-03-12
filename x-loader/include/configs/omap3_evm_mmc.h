/*
 * x-loader configuration for OMAP3EVM optimized for SD/MMC only
 * Derived from /include/configs/omap3evm.h
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * Select the processor
 */
#define CONFIG_ARMCORTEXA8
#define CONFIG_OMAP
#define CONFIG_OMAP34XX
#define CONFIG_OMAP3430

/*
 * Select the target platform
 */
#define CONFIG_OMAP3EVM

/*
 * Include architecture specific definitions
 */
#include <asm/arch/cpu.h>

/*
 * Clock related configuation
 */
#define V_OSCK			26000000

#if (V_OSCK > 19200000)
#define V_SCLK			(V_OSCK >> 1)
#else
#define V_SCLK			V_OSCK
#endif

#undef CFG_CLKS_IN_HZ

/*
 * Memory related configuation
 */
#define CONFIG_DDR_256MB_STACKED
#define CFG_3430SDRAM_DDR
#define CFG_OMAPEVM_DDR

/*
 * SDRAM bank allocation method
 */
#define SDRC_R_B_C

/*
 * Select support for MMC/SD
 */
#define CONFIG_MMC

/*
 * De-select support for NAND and OneNAND
 * This doesn't impact use of NAND and/or OneNAND in u-boot/ kernel
 */
#undef CFG_NAND
#undef CFG_ONENAND

/*
 * De-select support for serial prints
 * (Takes approx 3.5K bytes)
 */
#undef CFG_PRINTF

/*
 * Select necessary commands
 */
#define CFG_CMD_MMC
#define CFG_CMD_FAT
#define CONFIG_DOS_PARTITION

/*
 * Define load address
 */
#define CFG_LOADADDR		0x80008000

/*
 * Stack size
 * Used to setup stack sizes in start.S
 */
#define CONFIG_STACKSIZE	(128*1024)

/*
 * GPMC related definitions
 */
#define OMAP34XX_GPMC_CS0_SIZE	GPMC_SIZE_128M

#define GPMC_CONFIG		(OMAP34XX_GPMC_BASE+0x50)
#define GPMC_NAND_COMMAND_0	(OMAP34XX_GPMC_BASE+0x7C)
#define GPMC_NAND_ADDRESS_0	(OMAP34XX_GPMC_BASE+0x80)
#define GPMC_NAND_DATA_0	(OMAP34XX_GPMC_BASE+0x84)

/*
 * Select the POP memory part used on the EVM.
 */
#define CFG_NAND_K9F1G08R0A
#define NAND_16BIT

/*
 * Base addresses for NAND and OneNAND
 * Required for clean build even if we don't intend to use them
 * through this configuration.
 */
#define NAND_BASE_ADR		NAND_BASE
#define ONENAND_BASE		ONENAND_MAP
#define ONENAND_ADDR		ONENAND_BASE

/*
 * Define macros for read/write to/from the NAND chip
 *
 * TODO: Should be moved out of configuration file
 */
#ifdef NAND_16BIT

#define WRITE_NAND_COMMAND(d, adr) \
	do {*(volatile u16 *)GPMC_NAND_COMMAND_0 = d;} while(0)
#define WRITE_NAND_ADDRESS(d, adr) \
	do {*(volatile u16 *)GPMC_NAND_ADDRESS_0 = d;} while(0)
#define WRITE_NAND(d, adr) \
	do {*(volatile u16 *)GPMC_NAND_DATA_0 = d;} while(0)
#define READ_NAND(adr) \
	(*(volatile u16 *)GPMC_NAND_DATA_0)
#define NAND_WAIT_READY()
#define NAND_WP_OFF()  \
	do {*(volatile u32 *)(GPMC_CONFIG) |= 0x00000010;} while(0)
#define NAND_WP_ON()  \
	do {*(volatile u32 *)(GPMC_CONFIG) &= ~0x00000010;} while(0)

#else /* to support 8-bit NAND devices */

#define WRITE_NAND_COMMAND(d, adr) \
	do {*(volatile u8 *)GPMC_NAND_COMMAND_0 = d;} while(0)
#define WRITE_NAND_ADDRESS(d, adr) \
	do {*(volatile u8 *)GPMC_NAND_ADDRESS_0 = d;} while(0)
#define WRITE_NAND(d, adr) \
	do {*(volatile u8 *)GPMC_NAND_DATA_0 = d;} while(0)
#define READ_NAND(adr) \
	(*(volatile u8 *)GPMC_NAND_DATA_0);
#define NAND_WAIT_READY()
#define NAND_WP_OFF()  \
	do {*(volatile u32 *)(GPMC_CONFIG) |= 0x00000010;} while(0)
#define NAND_WP_ON()  \
	do {*(volatile u32 *)(GPMC_CONFIG) &= ~0x00000010;} while(0)

#endif

#define NAND_CTL_CLRALE(adr)
#define NAND_CTL_SETALE(adr)
#define NAND_CTL_CLRCLE(adr)
#define NAND_CTL_SETCLE(adr)
#define NAND_DISABLE_CE()
#define NAND_ENABLE_CE()

/*
 * Set clock configuration
 *
 * TODO: Although definition appears incorrect for AM37x,
 *       actual initialization is correct. Needs clean-up
 *       in mem.h
 */
#define PRCM_CLK_CFG2_332MHZ		/* VDD2=1.15v - 166MHz DDR */


/*
 * This macro is expected to be used to identify portions of code
 * that is excluded from build to achieve better optimization.
 */
#define CONFIG_OPTIMIZE

#endif /* __CONFIG_H */
