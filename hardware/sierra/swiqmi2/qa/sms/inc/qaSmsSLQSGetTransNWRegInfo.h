/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSGetTransNWRegInfo.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSmsSLQSGetTransNWRegInfo.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SMS_SLQS_GET_TRANS_NW_REG_INFO_H__
#define __SMS_SLQS_GET_TRANS_NW_REG_INFO_H__

#include "qaGobiApiSms.h"

/* enum declarations */

/*
 * An enumeration of eQMI_SMS_GET_TRANS_NW_REG_INFO_RESP response TLV IDs
 */
enum eQMI_SMS_GET_TRANS_NW_REG_INFO_RESP
{
    eTLV_TRANS_NW_REG_STAT = 0x10,
};

/*
 * This structure contains the SLQSGetTransNWRegInfo response parameters.
 */
struct QmiSmsSLQSGetTransNWRegInfoResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    getTransNWRegInfoResp *pTransNWRegInfo;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSmsSLQSGetTransNWRegInfo(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiSmsSLQSGetTransNWRegInfo(
    BYTE                                   *pMdmResp,
    struct QmiSmsSLQSGetTransNWRegInfoResp *pApiResp );

#endif /* __SMS_SLQS_GET_TRANS_NW_REG_INFO_H__ */

