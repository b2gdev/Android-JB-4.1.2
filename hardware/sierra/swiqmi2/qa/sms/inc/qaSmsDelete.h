/*
 * \ingroup sms
 *
 * \file    qaSmsDelete.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSmsDelete.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SMS_DELETE_H__
#define __SMS_DELETE_H__

/* enum declarations */

/*
 * An enumeration of eQMI_SMS_DELETE request TLV IDs
 *
 */
enum eSMS_DELETE_REQ
{
    eTLV_STORAGE_TYPE  = 0x01,
    eTLV_MESSAGE_INDEX = 0x10,
    eTLV_MESSAGE_TAG   = 0x11,
    eTLV_MESSAGE_MOD   = 0x12
};

/*
 * This structure contains the DeleteSMS request parameters.
 */
struct QmiSmsDeleteReq
{
    ULONG storageType;
    ULONG *pMessageIndex;
    ULONG *pMessageTag;
    BYTE  *pMode;
};

/*
 * This structure contains the DeleteSMS response parameters
 */
struct QmiSmsDeleteResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSmsDelete(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG storageType,
    ULONG *pMessageIndex,
    ULONG *pMessageTag,
    BYTE  *pMode );

extern enum eQCWWANError UpkQmiSmsDelete(
    BYTE                    *pMdmResp,
    struct QmiSmsDeleteResp *pApiResp );

#endif /* __SMS_DELETE_H__ */
