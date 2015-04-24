/*
 * \ingroup cbk
 *
 * \file qaCbkNasSysInfoInd.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaCbkNasSysInfoInd.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SYS_INFO_IND_H__
#define __NAS_SYS_INFO_IND_H__

#include "qaGobiApiCbk.h"

/*
 * An enumeration of eQMI_NAS_SYS_INFO_IND response TLV IDs
 *
 */
enum eQMI_NAS_SYS_INFO_IND
{
    eTLV_IND_CDMA_SS_INFO       = 0x10,
    eTLV_IND_HDR_SS_INFO        = 0x11,
    eTLV_IND_GSM_SS_INFO        = 0x12,
    eTLV_IND_WCDMA_SS_INFO      = 0x13,
    eTLV_IND_LTE_SS_INFO        = 0x14,
    eTLV_IND_CDMA_SYS_INFO      = 0x15,
    eTLV_IND_HDR_SYS_INFO       = 0x16,
    eTLV_IND_GSM_SYS_INFO       = 0x17,
    eTLV_IND_WCDMA_SYS_INFO     = 0x18,
    eTLV_IND_LTE_SYS_INFO       = 0x19,
    eTLV_IND_ADD_CDMA_SYS_INFO  = 0x1A,
    eTLV_IND_ADD_HDR_SYS_INFO   = 0x1B,
    eTLV_IND_ADD_GSM_SYS_INFO   = 0x1C,
    eTLV_IND_ADD_WCDMA_SYS_INFO = 0x1D,
    eTLV_IND_ADD_LTE_SYS_INFO   = 0x1E,
    eTLV_IND_GSM_CB_SYS_INFO    = 0x1F,
    eTLV_IND_WCDMA_CB_SYS_INFO  = 0x20,
    eTLV_IND_LTE_VS_SYS_INFO    = 0x21,
    eTLV_IND_GSM_CD_SYS_INFO    = 0x22,
    eTLV_IND_WCDMA_CD_SYS_INFO  = 0x23,
    eTLV_IND_SYS_INFO_NO_CHANGE = 0x24
};

/*
 * Prototypes
 *
 */
enum eQCWWANError UpkQmiCbkNasSysInfoInd(
    BYTE       *pMdmResp,
    nasSysInfo *pApiResp );

#endif /* __NAS_SYS_INFO_IND_H__ */
