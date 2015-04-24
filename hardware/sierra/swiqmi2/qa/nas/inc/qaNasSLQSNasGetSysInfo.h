/*
 * \ingroup nas
 *
 * \file    qaNasSLQSNasGetSysInfo.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaNasSLQSNasGetSysInfo.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SLQS_GET_SYS_INFO_H__
#define __NAS_SLQS_GET_SYS_INFO_H__

#include "qaGobiApiNas.h"

/* enum declarations */
/*
 * An enumeration of eNAS_SLQS_GET_SYS_INFO_RESP response TLV IDs
 */
enum eNAS_SLQS_GET_SYS_INFO_RESP
{
    eTLV_CDMA_SS_INFO       = 0x10,
    eTLV_HDR_SS_INFO        = 0x11,
    eTLV_GSM_SS_INFO        = 0x12,
    eTLV_WCDMA_SS_INFO      = 0x13,
    eTLV_LTE_SS_INFO        = 0x14,
    eTLV_CDMA_SYS_INFO      = 0x15,
    eTLV_HDR_SYS_INFO       = 0x16,
    eTLV_GSM_SYS_INFO       = 0x17,
    eTLV_WCDMA_SYS_INFO     = 0x18,
    eTLV_LTE_SYS_INFO       = 0x19,
    eTLV_ADD_CDMA_SYS_INFO  = 0x1A,
    eTLV_ADD_HDR_SYS_INFO   = 0x1B,
    eTLV_ADD_GSM_SYS_INFO   = 0x1C,
    eTLV_ADD_WCDMA_SYS_INFO = 0x1D,
    eTLV_ADD_LTE_SYS_INFO   = 0x1E,
    eTLV_GSM_CB_SYS_INFO    = 0x1F,
    eTLV_WCDMA_CB_SYS_INFO  = 0x20,
    eTLV_LTE_VS_SYS_INFO    = 0x21,
    eTLV_GSM_CD_SYS_INFO    = 0x22,
    eTLV_WCDMA_CD_SYS_INFO  = 0x23
};

/*
 * This structure contains the SLQSNasGetSysInfo response parameters.
 */
struct QmiNasSLQSGetSysInfoResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    nasGetSysInfoResp *pSysInfoResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiNasSlqsGetSysInfo(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiNasSlqsGetSysInfo(
    BYTE                            *pMdmResp,
    struct QmiNasSLQSGetSysInfoResp *pApiResp );

#endif /* __NAS_SLQS_GET_SYS_INFO_H__ */
