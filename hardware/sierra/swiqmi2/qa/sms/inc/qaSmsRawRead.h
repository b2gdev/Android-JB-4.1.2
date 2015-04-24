/*
 * \ingroup sms
 *
 * \file qaSmsRawRead.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaSmsRawRead.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SMS_RAW_READ_H__
#define __SMS_RAW_READ_H__

/* enum declarations */

/*
 * An enumeration of eQMI_SMS_RAW_READ request TLV IDs
 */
enum eSMS_RAW_READ_REQ
{
    eTLV_MEMORY_STORAGE_ID = 0x01,
    eTLV_MSG_MODE          = 0x10
};

/*
 * This structure contains the GetSMS request parameters.
 */
struct QmiSmsRawReadReq
{
    ULONG storageType;
    ULONG messageIndex;
    BYTE  *pMode;
};

/*
 * An enumeration of eQMI_SMS_RAW_READ request TLV IDs
 *
 */
enum eSMS_RAW_READ_RESP
{
    eTLV_RAW_MESSAGE_DATA = 0x01
};

/*
 * This structure contains the GetSMS response parameters
 */
struct QmiSmsRawReadResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetSMS response parameters */
    ULONG *pMessageTag;
    ULONG *pMessageFormat;
    ULONG *pMessageSize;
    BYTE  *pMessage;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSmsRawRead(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG storageType,
    ULONG messageIndex,
    BYTE  *pMode );

extern enum eQCWWANError UpkQmiSmsRawRead(
    BYTE                     *pMdmResp,
    struct QmiSmsRawReadResp *pApiResp );

#endif /* __SMS_RAW_READ_H__ */
