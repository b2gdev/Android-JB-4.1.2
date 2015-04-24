/*
 * \ingroup sms
 *
 * \file    qaSmsRawSend.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSmsRawSend.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SMS_RAW_SEND_H__
#define __SMS_RAW_SEND_H__

/* enum declarations */

/*
 * An enumeration of eQMI_SMS_RAW_SEND request TLV IDs
 *
 */
enum eQMI_SMS_RAW_SEND_REQ
{
    eTLV_MESSAGE_DATA = 0x01,
    eTLV_LINK_TIMER   = 0x12
};

/*
 * This structure contains the SendSMS request parameters.
 */
struct QmiSmsRawSendReq
{
    ULONG messageFormat;
    ULONG messageSize;
    BYTE linktimer;
    BYTE  *pMessage;
};

/*
 * An enumeration of eQMI_SMS_RAW_SEND request TLV IDs
 */
enum eQMI_SMS_RAW_SEND_RESP
{
    eTLV_MESSAGE_ID = 0x01,
    eTLV_CAUSE_CODE = 0x10
};

/*
 * This structure contains the SendSMS response parameters
 */
struct QmiSmsRawSendResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* SendSMS response parameters */
    WORD  *pMessageID; 
    ULONG *pMessageFailureCode;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSmsRawSend(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG messageFormat,
    ULONG messageSize,
    BYTE linktimer,
    BYTE  *pMessage );

extern enum eQCWWANError UpkQmiSmsRawSend(
    BYTE                     *pMdmResp,
    struct QmiSmsRawSendResp *pApiResp );

#endif /* __SMS_RAW_SEND_H__ */
