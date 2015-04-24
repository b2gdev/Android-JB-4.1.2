/*
 * \ingroup nas
 *
 * \file qaNasSLQSGetSigInfo.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasSLQSGetSigInfo.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SLQS_GET_SIG_INFO_MSG_H__
#define __NAS_SLQS_GET_SIG_INFO_MSG_H__

#include "qaGobiApiNas.h"

/*
 * An enumeration of eQMI_NAS_SLQS_GET_SIG_INFO response TLV IDs
 */
enum eNAS_SLQS_GET_SIG_INFO_RESP
{
    eTLV_CDMA_SS_SIG_INFO  = 0x10,
    eTLV_HDR_SS_SIG_INFO   = 0x11,
    eTLV_GSM_SS_SIG_INFO   = 0x12,
    eTLV_WCDMA_SS_SIG_INFO = 0x13,
    eTLV_LTE_SS_SIG_INFO   = 0x14
};

/*
 * This structure contains the GetSigInfo response parameters.
 *
 * \sa  qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasSlqsGetSigInfoResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Response parameters */
    nasGetSigInfoResp *pGetSigInfoResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiNasSlqsGetSigInfo(
    WORD *pMlength,
    BYTE *pParamField );

extern enum eQCWWANError UpkQmiNasSlqsGetSigInfo(
    BYTE                            *pMdmResp,
    struct QmiNasSlqsGetSigInfoResp *pApiResp );

#endif /* __NAS_SLQS_GET_SIG_INFO_MSG_H__ */
