/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSGetMessageProtocol.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSmsSLQSGetMessageProtocol.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __SMS_SLQS_GET_MESSAGE_PROTOCOL_H__
#define __SMS_SLQS_GET_MESSAGE_PROTOCOL_H__

#include "qaGobiApiSms.h"

/* enum declarations */

/*
 * An enumeration of eQMI_SMS_SLQS_GET_MESSAGE_PROTOCOL_RESP request TLV IDs
 */
enum eQMI_SMS_SLQS_GET_MESSAGE_PROTOCOL_RESP
{
    eTLV_MESSAGE_PROTOCOL = 0x01
};

/*
 * This structure contains the SLQSSmsGetMessageProtocol response parameters.
 */
struct QmiSmsSLQSGetMsgProtocolResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
    smsMsgprotocolResp *pMessageProtocol;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSmsSLQSGetMessageProtocol(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiSmsSLQSMessageProtocol(
    BYTE                                *pMdmResp,
    struct QmiSmsSLQSGetMsgProtocolResp *pApiResp );

#endif /* __SMS_SLQS_GET_MESSAGE_PROTOCOL_H__ */
