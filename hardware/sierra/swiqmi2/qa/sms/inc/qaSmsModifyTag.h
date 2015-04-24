/*
 * \ingroup sms
 *
 * \file qaSmsModifyTag.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaSmsModifyTag.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SMS_MODIFY_TAG_H__
#define __SMS_MODIFY_TAG_H__

/* enum declarations */

/*
 * An enumeration of eQMI_SMS_MODIFY_TAG request TLV IDs
 */
enum eSMS_MODIFY_TAG_REQ
{
    eTLV_SMS_MESSAGE_TAG  = 0x01,
    eTLV_SMS_MESSAGE_MODE = 0x10
};

/*
 * This structure contains the ModifySMSStatus request parameters.
 */
struct QmiSmsModifyTagReq
{
    ULONG storageType;
    ULONG messageIndex;
    ULONG messageTag;
    BYTE  *pMode;
};

/*
 * This structure contains the ModifySMSStatus response parameters.
 */
struct QmiSmsModifyTagResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSmsModifyTag(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG storageType,
    ULONG messageIndex,
    ULONG messageTag,
    BYTE  *pMode );

extern enum eQCWWANError UpkQmiSmsModifyTag(
    BYTE                       *pMdmResp,
    struct QmiSmsModifyTagResp *pApiResp );

#endif /* __SMS_MODIFY_TAG_H__ */
