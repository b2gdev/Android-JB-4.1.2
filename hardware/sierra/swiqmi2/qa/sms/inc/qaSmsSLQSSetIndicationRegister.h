/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSSetIndicationRegister.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSmsSLQSSetIndicationRegister.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SMS_SLQS_SET_INDICATION_REGISTER_H__
#define __SMS_SLQS_SET_INDICATION_REGISTER_H__

#include "qaGobiApiSms.h"

/* enum declarations */

/*
 * An enumeration of eQMI_SMS_SLQS_SET_INDICATION_REGISTER request TLV IDs
 *
 */
enum eQMI_SMS_SLQS_SET_INDICATION_REGISTER_REQ
{
    eTLV_SET_TRANS_LAYER_INFO_EVENTS  = 0x10,
    eTLV_SET_TRANS_NW_REG_INFO_EVENTS = 0x11,
    eTLV_SET_CALL_STATUS_INFO_EVENTS  = 0x12,
};

/*
 * This structure contains the SLQSSetIndicationRegister request parameters
 */
struct QmiSmsSLQSSetIndicationRegisterReq
{
    setIndicationRegReq *pSetIndicationRegisterReq;
};

/*
 * This structure contains the SLQSSetIndicationRegister response parameters
 */
struct QmiSmsSLQSSetIndicationRegisterResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSmsSLQSSetIndicationReg(
    WORD                *pMlength,
    BYTE                *pBuffer,
    setIndicationRegReq *pSetIndicationRegisterReq );

extern enum eQCWWANError UpkQmiSmsSLQSSetIndicationReg(
    BYTE                                       *pMdmResp,
    struct QmiSmsSLQSSetIndicationRegisterResp *pApiResp );

#endif /* __SMS_SLQS_SET_INDICATION_REGISTER_H__ */
