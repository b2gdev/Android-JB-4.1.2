/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSGetMaxStorageSize.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSmsSLQSGetMaxStorageSize.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __SMS_SLQS_GET_STORE_MAX_SIZE_H__
#define __SMS_SLQS_GET_STORE_MAX_SIZE_H__

#include "qaGobiApiSms.h"

/* enum declarations */

/*
 * An enumeration of eQMI_SMS_SLQS_GET_MAX_SIZE_REQ request TLV IDs
 *
 */
enum eQMI_SMS_SLQS_GET_STORE_MAX_SIZE_REQ
{
    eTLV_MEMORY_STORE = 0x01,
    eTLV_MESSAGE_MODE = 0x10
};

/*
 * An enumeration of eQMI_SMS_SLQS_GET_MAX_SIZE_RESP response TLV IDs
 */
enum eQMI_SMS_SLQS_GET_STORE_MAX_SIZE_RESP
{
    eTLV_STORE_MAX_SIZE = 0x01,
    eTLV_STORE_FREE_SLOT = 0x10
};

/*
 * This structure contains the SLQSSmsGetMaxStorageSize response parameters.
 */
struct QmiSmsSLQSGetMaxStorageSizeResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
    smsMaxStorageSizeResp *pMaxStorageSizeResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSmsSLQSGetMaxStorageSize(
    WORD               *pMlength,
    BYTE               *pParamField,
    smsMaxStorageSizeReq *pMaxStorageSizeReq );

extern enum eQCWWANError UpkQmiSmsSLQSGetmaxStorageSize(
    BYTE                                *pMdmResp,
    struct QmiSmsSLQSGetMaxStorageSizeResp *pApiResp );

#endif /* __SMS_SLQS_GET_STORE_MAX_SIZE_H__ */
