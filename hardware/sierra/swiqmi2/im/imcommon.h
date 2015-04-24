/*************
 *
 * Filename:    imcommon.h
 *
 * Purpose:     Protocol-independent image management support
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef IMCOMMON_H
#define IMCOMMON_H

/*---------------
  Include files
 ---------------*/
#include "aa/aaglobal.h"
#include "qa/inc/SwiDataTypes.h"

/* Residual CRC value to compare against return value of crc_16_calc().
 * Use crc_16_calc() to calculate a 16 bit CRC, and append it to the buffer.
 * When crc_16_calc() is applied to the unchanged result, it returns CRC_16_OK.
 */
#define CRC_16_OK  0xE2F0

/* Mask for CRC-16 polynomial:
 *
 *      x^16 + x^12 + x^5 + 1
 *
 * This is more commonly referred to as CCITT-16.
 * Note:  the x^16 tap is left off, it's implicit.
 */
#define CRC_16_POLYNOMIAL 0x1021

/* Seed value for CRC register.  The all ones seed is part of CCITT-16, as
 * well as allows detection of an entire data stream of zeroes.
 */
#define CRC_16_SEED 0xFFFF

/* Residual CRC value to compare against return value of a CRC_16_L_STEP().
 * Use CRC_16_L_STEP() to calculate a 16 bit CRC, complement the result,
 * and append it to the buffer.  When CRC_16_L_STEP() is applied to the
 * unchanged entire buffer, and complemented, it returns CRC_16_L_OK.
 * Crc_16_l_calc() of the same entire buffer returns CRC_16_L_OK.
 */
#define CRC_16_L_OK 0x0F47
/*
 * Mask for CRC-16 polynomial:
 *
 *     x^16 + x^15 + x^2 + 1
 *
 * Note:  the x^16 tap is left off, it's implicit.
 */
#define CRC_16_L_POLYNOMIAL 0x8408

/* 2^CRC_TAB_BITS */
#define CRC_TAB_SIZE 256

/*
 * Seed value for CRC register.  The all ones seed is part of CCITT-16, as
 * well as allows detection of an entire data stream of zeroes.
 */
#define CRC_16_L_SEED 0xFFFF
/*---------------
  Prototypes
 ---------------*/

/* imcalculate16CRC */
swi_uint16 imCalculate16CRC(
    swi_uint8  *dataBuf,
    swi_uint32 length );

#endif /* IMCOMMON_H */
