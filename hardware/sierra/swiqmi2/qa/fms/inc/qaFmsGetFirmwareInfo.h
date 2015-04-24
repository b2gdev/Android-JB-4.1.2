/*
 * \ingroup fms
 *
 * \file    qaFmsGetFirmwareInfo.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaFmsGetFirmwareInfo.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __FMS_GET_FIRMWARE_INFO_H__
#define __FMS_GET_FIRMWARE_INFO_H__

#include "qaGobiApiFms.h"

#define QMI_MAX_REV_ID_LENGTH 255
#define QMI_PRI_STRING_LENGTH 8

/* defines to extract parameters from firmware ID */
#define GOBI_FW_MINOR_ID_MASK           0x0000007F
#define GOBI_FW_XTRA_DISABLED_MASK      0x00000080
#define GOBI_FW_XTRA_DISABLED_OFFSET    7
#define GOBI_FW_GPS_DISABLED_MASK       0x00000100
#define GOBI_FW_GPS_DISABLED_OFFSET     8
#define GOBI_FW_RESERVED_MASK           0x0000FE00
#define GOBI_FW_RESERVED_OFFSET         9
#define GOBI_FW_MAJOR_ID_MASK           0x00FF0000
#define GOBI_FW_MAJOR_ID_OFFSET         16
#define GOBI_FW_SYSTEM_MASK             0x03000000
#define GOBI_FW_SYSTEM_OFFSET           24
#define GOBI_FW_COMPATIBILITY_MASK      0xFC000000
#define GOBI_FW_COMPATIBILITY_OFFSET    26

enum eQMISystemValues
{
    eQMI_SYSTEM_VALUE_CDMA = 1,
    eQMI_SYSTEM_VALUE_UMTS
};

/*
 * Prototypes
 */
enum eQCWWANError qaParseFirmwareID(
    ULONG   pFirmwareID,
    ULONG   Carrier,
    ULONG   *pTechnology,
    ULONG   *pGPSCapability );

#endif /* __FMS_GET_FIRMWARE_INFO_H__ */
