/*
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR /PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/bits.h>
#include <asm/arch/clocks.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/sys_info.h>

static char *rev_s[CPU_3XX_MAX_REV] = {
				"1.0",
				"2.0",
				"2.1",
				"3.0",
				"3.1",
				"UNKNOWN",
				"UNKNOWN",
				"3.1.2"};

/*
 * sr32: clear & set a value in a bit range for a 32 bit address
 */
void sr32(u32 addr, u32 start_bit, u32 num_bits, u32 value)
{
	u32 tmp, msk = 0;
	msk = 1 << num_bits;
	--msk;
	tmp = __raw_readl(addr) & ~(msk << start_bit);
	tmp |= value << start_bit;
	__raw_writel(tmp, addr);
}

/*
 * wait_on_value(): common routine to allow waiting for changes in
 * volatile regs.
 */
u32 wait_on_value(u32 read_bit_mask, u32 match_value, u32 read_addr, u32 bound)
{
	u32 i = 0, val;
	do {
		++i;
		val = __raw_readl(read_addr) & read_bit_mask;
		if (val == match_value)
			return 1;
		if (i == bound)
			return 0;
	} while (1);
}

/*
 *  get_device_type(): tell if GP/HS/EMU/TST
 */
u32 get_device_type(void)
{
	int mode;
	mode = __raw_readl(CONTROL_STATUS) & (DEVICE_MASK);
	return mode >>= 8;
}

/*
 *  get_cpu_type(): extract cpu info
 */
u32 get_cpu_type(void)
{
	return __raw_readl(CONTROL_OMAP_STATUS);
}

/*
 * get_cpu_id(): extract cpu id
 * returns 0 for ES1.0, cpuid otherwise
 */
u32 get_cpu_id(void)
{
	u32 cpuid = 0;

	/*
	 * On ES1.0 the IDCODE register is not exposed on L4
	 * so using CPU ID to differentiate between ES1.0 and > ES1.0.
	 */
	__asm__ __volatile__("mrc p15, 0, %0, c0, c0, 0":"=r"(cpuid));
	if ((cpuid & 0xf) == 0x0) {
		return 0;
	} else {
		/* Decode the IDs on > ES1.0 */
		cpuid = __raw_readl(CONTROL_IDCODE);
	}

	return cpuid;
}

/*
 * get_cpu_family(void): extract cpu info
 */
u32 get_cpu_family(void)
{
	u16 hawkeye;
	u32 cpu_family;
	u32 cpuid = get_cpu_id();

	if (cpuid == 0)
		return CPU_OMAP34XX;

	hawkeye = (cpuid >> HAWKEYE_SHIFT) & 0xffff;
	switch (hawkeye) {
	case HAWKEYE_OMAP34XX:
		cpu_family = CPU_OMAP34XX;
		break;
	case HAWKEYE_AM35XX:
		cpu_family = CPU_AM35XX;
		break;
	case HAWKEYE_OMAP36XX:
		cpu_family = CPU_OMAP36XX;
		break;
	default:
		cpu_family = CPU_OMAP34XX;
	}

	return cpu_family;
}

/*
 * get_cpu_rev(void): extract version info
 */
u32 get_cpu_rev(void)
{
	u32 cpuid = get_cpu_id();

	if (cpuid == 0)
		return CPU_3XX_ES10;
	else
		return (cpuid >> CPU_3XX_ID_SHIFT) & 0xf;
}

/*
 * print_cpuinfo(void): print CPU information
 */
int print_cpuinfo(void)
{
	char *cpu_family_s, *cpu_s, *sec_s;

	switch (get_cpu_family()) {
	case CPU_OMAP34XX:
		cpu_family_s = "OMAP";
		switch (get_cpu_type()) {
		case OMAP3503:
			cpu_s = "3503";
			break;
		case OMAP3515:
			cpu_s = "3515";
			break;
		case OMAP3525:
			cpu_s = "3525";
			break;
		case OMAP3530:
			cpu_s = "3530";
			break;
		default:
			cpu_s = "35XX";
			break;
		}
		break;
	case CPU_AM35XX:
		cpu_family_s = "AM";
		switch (get_cpu_type()) {
		case AM3505:
			cpu_s = "3505";
			break;
		case AM3517:
			cpu_s = "3517";
			break;
		default:
			cpu_s = "35XX";
			break;
		}
		break;
	case CPU_OMAP36XX:
		cpu_family_s = "OMAP";
		switch (get_cpu_type()) {
		case OMAP3730:
			cpu_s = "3630/3730";
			break;
		default:
			cpu_s = "36XX/37XX";
			break;
		}
		break;
	default:
		cpu_family_s = "OMAP";
		cpu_s = "35XX";
	}

	switch (get_device_type()) {
	case TST_DEVICE:
		sec_s = "TST";
		break;
	case EMU_DEVICE:
		sec_s = "EMU";
		break;
	case HS_DEVICE:
		sec_s = "HS";
		break;
	case GP_DEVICE:
		sec_s = "GP";
		break;
	default:
		sec_s = "?";
	}

	printf("%s%s-%s ES%s\n",
			cpu_family_s, cpu_s, sec_s, rev_s[get_cpu_rev()]);

	return 0;
}

/*
 * get_sysboot_value(void): return SYS_BOOT[4:0]
 */
u32 get_sysboot_value(void)
{
	int mode;
	mode = __raw_readl(CONTROL_STATUS) & (SYSBOOT_MASK);
	return mode;
}

/*
 * get_sys_clkin_sel(): returns the sys_clkin_sel field value based on
 *   input oscillator clock frequency.
 */
void get_sys_clkin_sel(u32 osc_clk, u32 *sys_clkin_sel)
{
	if (osc_clk == S38_4M)
		*sys_clkin_sel = 4;
	else if (osc_clk == S26M)
		*sys_clkin_sel = 3;
	else if (osc_clk == S19_2M)
		*sys_clkin_sel = 2;
	else if (osc_clk == S13M)
		*sys_clkin_sel = 1;
	else if (osc_clk == S12M)
		*sys_clkin_sel = 0;
}

/*
 * secure_unlock(void): setup security registers for access
 * (GP Device only)
 */
void secure_unlock(void)
{
	/* Permission values for registers -Full fledged permissions to all */
	#define UNLOCK_1 0xFFFFFFFF
	#define UNLOCK_2 0x00000000
	#define UNLOCK_3 0x0000FFFF
	/* Protection Module Register Target APE (PM_RT)*/
	__raw_writel(UNLOCK_1, RT_REQ_INFO_PERMISSION_1);
	__raw_writel(UNLOCK_1, RT_READ_PERMISSION_0);
	__raw_writel(UNLOCK_1, RT_WRITE_PERMISSION_0);
	__raw_writel(UNLOCK_2, RT_ADDR_MATCH_1);

	__raw_writel(UNLOCK_3, GPMC_REQ_INFO_PERMISSION_0);
	__raw_writel(UNLOCK_3, GPMC_READ_PERMISSION_0);
	__raw_writel(UNLOCK_3, GPMC_WRITE_PERMISSION_0);

	__raw_writel(UNLOCK_3, OCM_REQ_INFO_PERMISSION_0);
	__raw_writel(UNLOCK_3, OCM_READ_PERMISSION_0);
	__raw_writel(UNLOCK_3, OCM_WRITE_PERMISSION_0);
	__raw_writel(UNLOCK_2, OCM_ADDR_MATCH_2);

	/* IVA Changes */
	__raw_writel(UNLOCK_3, IVA2_REQ_INFO_PERMISSION_0);
	__raw_writel(UNLOCK_3, IVA2_READ_PERMISSION_0);
	__raw_writel(UNLOCK_3, IVA2_WRITE_PERMISSION_0);

	__raw_writel(UNLOCK_1, SMS_RG_ATT0); /* SDRC region 0 public */
}

/*
 * try_unlock_memory(void): If chip is GP type, unlock the SRAM for
 *  general use.
 */
void try_unlock_memory(void)
{
	int mode;

	/* if GP device unlock device SRAM for general use */
	/* secure code breaks for Secure/Emulation device - HS/E/T*/
	mode = get_device_type();
	if (mode == GP_DEVICE) {
		secure_unlock();
	}
	return;
}
