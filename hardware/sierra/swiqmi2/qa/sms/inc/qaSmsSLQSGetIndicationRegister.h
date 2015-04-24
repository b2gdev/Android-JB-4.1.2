/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSGetIndicationRegister.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSmsSLQSGetIndicationRegister.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SMS_SLQS_GET_IND_REGISTER_H__
#define __SMS_SLQS_GET_IND_REGISTER_H__

#include "qaGobiApiSms.h"

/* enum declarations */

/*
 * An enumeration of eQMI_SMS_SLQS_GET_IND_REGISTER_RESP response TLV IDs
 */
enum eQMI_SMS_SLQS_GET_INDICATION_REGISTER_RESP
{
    eTLV_TRANS_LAYER_INFO_EVENTS  = 0x10,
    eTLV_TRANS_NW_REG_INFO_EVENTS = 0x11,
    eTLV_CALL_STATUS_INFO_EVENTS  = 0x12,
};

/*
 * This structure contains the SLQSGetIndicationRegister response parameters.
 */
struct QmiSmsSLQSGetIndicationRegisterResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    getIndicationRegResp *pGetIndicationRegInfo;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSmsSLQSGetIndicationReg(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiSmsSLQSGetIndicationReg(
    BYTE                                       *pMdmResp,
    struct QmiSmsSLQSGetIndicationRegisterResp *pApiResp );

#endif /* __SMS_SLQS_GET_IND_REGISTER_H__ */

