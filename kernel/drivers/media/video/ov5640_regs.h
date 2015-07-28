/*
 * Driver for OV5640 from OV
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#ifndef _OV5640_REGS_H
#define _OV5640_REGS_H

/*
 * ov5640 registers
 */
#define REG_CHIP_ID_MSB				(0x300a)
#define REG_CHIP_ID_LSB				(0x300b)

/**
 * struct ov5640_reg - Structure for TVP5146/47 register initialization values
 * @reg - Register offset
 * @val - Register Value for TOK_WRITE or delay in ms for TOK_DELAY
 */
struct ov5640_reg {
	unsigned short reg;
	unsigned char val;
};

/**
 * struct ov5640_init_seq - Structure for TVP5146/47/46M2/47M1 power up
 *		Sequence.
 * @ no_regs - Number of registers to write for power up sequence.
 * @ init_reg_seq - Array of registers and respective value to write.
 */
struct ov5640_init_seq {
	unsigned int no_regs;
	const struct ov5640_reg *init_reg_seq;
};

#define OV5640_CHIP_ID			(0x5640)
 
#endif
