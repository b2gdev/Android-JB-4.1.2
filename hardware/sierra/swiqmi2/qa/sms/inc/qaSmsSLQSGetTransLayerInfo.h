/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSGetTransLayerInfo.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSmsSLQSGetTransLayerInfo.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SMS_SLQS_GET_TRANS_LAYER_INFO_H__
#define __SMS_SLQS_GET_TRANS_LAYER_INFO_H__

#include "qaGobiApiSms.h"

/* enum declarations */

/*
 * An enumeration of eQMI_SMS_SLQS_GET_TRANS_LAYER_INFO_RESP response TLV IDs
 */
enum eQMI_SMS_SLQS_GET_TRANS_LAYER_INFO_RESP
{
    eTLV_TRANS_LAYER_REG_INFO = 0x10,
    eTLV_TRANS_LAYER_INFO     = 0x11,
};

/*
 * This structure contains the SLQSGetTransLayerInfo response parameters.
 */
struct QmiSmsSLQSGetTransLayerInfoResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    getTransLayerInfoResp *pGetTransLayerResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSmsSLQSGetTransLayerInfo(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiSmsSLQSGetTransLayerInfo(
    BYTE                                   *pMdmResp,
    struct QmiSmsSLQSGetTransLayerInfoResp *pApiResp );

#endif /* __SMS_SLQS_GET_TRANS_LAYER_INFO_H__ */

