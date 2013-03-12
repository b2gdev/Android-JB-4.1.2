/*
 * am3517evm.c - board file for TI's AM3517 family of devices.
 *
 * Author: Vaibhav Hiremath <hvaibhav@ti.com>
 *
 * Based on ti/evm/evm.c
 *
 * Copyright (C) 2010
 * Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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

#include <common.h>
#include <asm/io.h>
#include <asm/arch/mem.h>
#include <asm/arch/mux.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/emac_defs.h>
#include <asm/mach-types.h>
#include <i2c.h>
#include <fastboot.h>
#include "am3517crane.h"


#define AM3517_IP_SW_RESET     0x48002598
#define CPGMACSS_SW_RST                (1 << 1)
#define ETHERNET_NRST           65
#define EMACID_ADDR_LSB         0x48002380
#define EMACID_ADDR_MSB         0x48002384

#ifdef	CONFIG_CMD_FASTBOOT
#ifdef	FASTBOOT_PORT_OMAPZOOM_NAND_FLASHING
/* Initialize the name of fastboot flash name mappings */
fastboot_ptentry ptn[6] = {
	{
		.name   = "xloader",
		.start  = 0x0000000,
		.length = 0x0020000,
		/* Written into the first 4 0x20000 blocks
		   Use HW ECC */
		.flags  = FASTBOOT_PTENTRY_FLAGS_WRITE_I |
			FASTBOOT_PTENTRY_FLAGS_WRITE_HW_ECC |
			FASTBOOT_PTENTRY_FLAGS_REPEAT_4,
	},
	{
		.name   = "bootloader",
		.start  = 0x0080000,
		.length = 0x01C0000,
		/* Skip bad blocks on write
		   Use HW ECC */
		.flags  = FASTBOOT_PTENTRY_FLAGS_WRITE_I |
			FASTBOOT_PTENTRY_FLAGS_WRITE_SW_ECC,
	},
	{
		.name   = "environment",
		.start  = SMNAND_ENV_OFFSET,  /* set in config file */
		.length = 0x0040000,
		.flags  = FASTBOOT_PTENTRY_FLAGS_WRITE_SW_ECC |
			FASTBOOT_PTENTRY_FLAGS_WRITE_ENV,
	},
	{
		.name   = "boot",
		/* Test with start close to bad block
		   The is dependent on the individual board.
		   Change to what is required */
		/* .start  = 0x0a00000, */
		/* The real start */
		.start  = 0x0280000,
		.length = 0x0500000,
		.flags  = FASTBOOT_PTENTRY_FLAGS_WRITE_SW_ECC |
			FASTBOOT_PTENTRY_FLAGS_WRITE_I,
	},
	{
		.name   = "system",
		.start  = 0x0780000,
		.length = 0x1F880000,
		.flags  = FASTBOOT_PTENTRY_FLAGS_WRITE_SW_ECC |
			FASTBOOT_PTENTRY_FLAGS_WRITE_I,
	},
};
#endif /* FASTBOOT_PORT_OMAPZOOM_NAND_FLASHING */
#endif /* CONFIG_CMD_FASTBOOT */






/*
 * Routine: board_init
 * Description: Early hardware init.
 */
int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	gpmc_init(); /* in SRAM or SDRAM, finish GPMC */
	/* board id for Linux */
	gd->bd->bi_arch_number = MACH_TYPE_CRANEBOARD;
	/* boot param addr */
	gd->bd->bi_boot_params = (OMAP34XX_SDRC_CS0 + 0x100);

#ifdef	CONFIG_CMD_FASTBOOT
#ifdef	FASTBOOT_PORT_OMAPZOOM_NAND_FLASHING
	int i;

	for (i = 0; i < 6; i++)
		fastboot_flash_add_ptn(&ptn[i]);
#endif /* FASTBOOT_PORT_OMAPZOOM_NAND_FLASHING */
#endif /* CONFIG_CMD_FASTBOOT */

	return 0;
}

/*
 * Routine: misc_init_r
 * Description: Init i2c, ethernet, etc... (done here so udelay works)
 */
int misc_init_r(void)
{
	volatile unsigned int ctr;
	u32 reset;


#ifdef CONFIG_DRIVER_OMAP34XX_I2C
	i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
#endif

	dieid_num_r();


#if defined(CONFIG_DRIVER_TI_EMAC)

	omap_request_gpio(65);
	omap_set_gpio_direction(65, 0);
	omap_set_gpio_dataout(65, 0);
	ctr  = 0;
	do {
		udelay(1000);
		ctr++;
	} while (ctr < 300);
	omap_set_gpio_dataout(65, 1);
	ctr = 0;
	/* allow the PHY to stabilize and settle down */
	do {
		udelay(1000);
		ctr++;
	} while (ctr < 300);

	/*ensure that the module is out of reset*/
	reset = readl(AM3517_IP_SW_RESET);
	reset &= (~CPGMACSS_SW_RST);
	writel(reset, AM3517_IP_SW_RESET);

#endif



	return 0;
}


/*
 * Initializes on-chip ethernet controllers.
 * to override, implement board_eth_init()
 */
int cpu_eth_init(bd_t *bis)
{
#if defined(CONFIG_DRIVER_TI_EMAC)
	/* Check for efficient way of code*/
	u8 mac_id[32];

	u16 aa, bb, cc, dd, ee, ff;
	u32 emac_lsb, emac_msb;

	memset(mac_id, '\0', sizeof(mac_id));

	printf("davinci_emac_initialize\n");
	davinci_emac_initialize();

	emac_lsb = readl(EMACID_ADDR_LSB);
	emac_msb = readl(EMACID_ADDR_MSB);

	printf("EMAC LSB = 0x%08x\n", emac_lsb);
	printf("EMAC MSB = 0x%08x\n", emac_msb);

	cc  = (emac_msb & 0x000000FF) >> 0;
	bb  = (emac_msb & 0x0000FF00) >> 8;
	aa  = (emac_msb  & 0x00FF0000) >> 16;

	ff  = (emac_lsb & 0x000000FF);
	ee  = (emac_lsb & 0x0000FF00) >> 8;
	dd  = (emac_lsb & 0x00FF0000) >> 16;

	sprintf(mac_id, "%02x:%02x:%02x:%02x:%02x:%02x"
			, aa, bb, cc, dd, ee, ff);

	printf("-----------------------------\n");
	printf("EMAC ID %s\n", mac_id);
	printf("-----------------------------\n");

	setenv("ethaddr", mac_id);

#endif
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
	MUX_AM3517CRANE();
}
