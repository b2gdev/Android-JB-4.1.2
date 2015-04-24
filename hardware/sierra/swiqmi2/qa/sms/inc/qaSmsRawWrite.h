/*
 * \ingroup sms
 *
 * \file qaSmsRawWrite.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaSmsRawWrite.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SMS_RAW_WRITE_H__
#define __SMS_RAW_WRITE_H__

/* enum declarations */

/*
 * An enumeration of eQMI_SMS_RAW_WRITE request TLV IDs
 *
 */
enum eSMS_RAW_WRITE_REQ
{
    eTLV_RAW_MSG_DATA = 0x01
};

/*
 * This structure contains the GetSMS request parameters.
 */
struct QmiSmsRawWriteReq
{
    ULONG storageType;
    ULONG messageFormat;
    ULONG messageSize;
    BYTE  *pMessage;
};

/*
 * An enumeration of eQMI_SMS_RAW_WRITE response TLV IDs
 */
enum eSMS_RAW_WRITE_RESP
{
    eTLV_MSG_INDEX = 0x01
};

/*
 * An enumeration of eQMI_SMS_RAW_WRITE response TLV ID Length
 */
enum eSMS_RAW_WRITE_RESP_LENGTH
{
    eTLV_MSG_INDEX_LENGTH = 0x04
};

/*
 * This structure contains the GetSMS response parameters
 */
struct QmiSmsRawWriteResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* SaveSMS response parameters */
    ULONG *pMessageIndex;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSmsRawWrite(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG storageType,
    ULONG messageFormat,
    ULONG messageSize,
    BYTE  *pMessage );

extern enum eQCWWANError UpkQmiSmsRawWrite(
    BYTE                      *pMdmResp,
    struct QmiSmsRawWriteResp *pApiResp );

#endif /* __SMS_RAW_WRITE_H__ */
