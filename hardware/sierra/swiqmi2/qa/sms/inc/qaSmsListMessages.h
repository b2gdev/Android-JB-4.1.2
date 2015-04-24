/*
 * \ingroup sms
 *
 * \file    qaSmsListMessages.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSmsListMessage.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SMS_LIST_MESSAGES_MSG_H__
#define __SMS_LIST_MESSAGES_MSG_H__

/*
 * An enumeration of eQMI_SMS_LIST_MESSAGE_REQ response TLV IDs
 */
enum eQMI_SMS_LIST_MESSAGE_REQ
{
    eTLV_REQ_MEMORY_STORAGE    = 0x01,
    eTLV_REQ_TAG               = 0x10,
    eTLV_SMS_LIST_MESSAGE_MODE = 0x11
};

/*
 * This structure contains the GetSMSList response parameters
 */
struct QmiSmsListMessagesReq
{
    ULONG storageType;
    ULONG *pRequestedTag;
    BYTE  *pMode;
};

/*
 * An enumeration of eQMI_SMS_LIST_MESSAGE_RESP response TLV IDs
 */
enum eQMI_SMS_LIST_MESSAGE_RESP
{
    eTLV_MESSAGES_LIST = 0x01
};

/*
 * This structure contains the MessageList response parameters.
 *
 *  \param  messageIndex
 *          - Message index of each matched message
 *
 *  \param  messageTag
 *          - Messagetag
 *
 */
struct QmiSmsMessageList
{
    ULONG messageIndex;
    ULONG messageTag;
};

/*
 * This structure contains the GetSMSList response parameters
 */
struct QmiSmsListMessagesResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult       results;
    ULONG                    *pMessageListSize;
    struct QmiSmsMessageList *pMessageList;
};

/*
 * Packs parameter of GetSMSList
 */
extern enum eQCWWANError PkQmiSmsListMessages(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG storageType,
    ULONG *pRequestedTag,
    BYTE  *pMode );

extern enum eQCWWANError UpkQmiSmsListMessages(
    BYTE                          *pMdmResp,
    struct QmiSmsListMessagesResp *pApiResp );

#endif /*__SMS_LIST_MESSAGES_MSG_H__ */
