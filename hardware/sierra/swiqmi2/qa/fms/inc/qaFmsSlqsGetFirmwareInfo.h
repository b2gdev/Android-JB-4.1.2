/*
 * \ingroup fms
 *
 * \file    qaFmsSlqsGetFirmwareInfo.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaFmsSlqsGetFirmwareInfo.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __FMS_SLQS_GET_FIRMWARE_INFO_H__
#define __FMS_SLQS_GET_FIRMWARE_INFO_H__

/*
 * An enumeration of eQMI_FMS_SLQS_GET_FIRMWARE_INFO response TLV IDs
 */
enum eQMI_FMS_SLQS_GET_FIRMWARE_INFO_RESP
{
    eTLV_MODEL_ID            = 0x10,
    eTLV_BOOT_VERSION        = 0x11,
    eTLV_APPLICATION_VERSION = 0x12,
    eTLV_SKU                 = 0x13,
    eTLV_PACKAGE             = 0x14,
    eTLV_CARRIER             = 0x15,
    eTLV_PRI                 = 0x16
};

/*
 * This structure contains the SLQSGetFirmwareInfo response parameters
 */
struct QmiFmsSlqsGetFirmwareInfoResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* SLQSGetFirmwareInfo response parameters */
    struct slqsfwinfo_s *pinfo;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiFmsSlqsGetFirmwareInfo(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiFmsSlqsGetFirmwareInfo(
    BYTE                                 *pMdmResp,
    struct QmiFmsSlqsGetFirmwareInfoResp *pApiResp );

#endif /* __FMS_SLQS_GET_FIRMWARE_INFO_H__ */
