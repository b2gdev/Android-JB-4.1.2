/**
 *
 * @ingroup swiril
 *
 * @file 
 * Provides SMS related QMI functions
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */
#include <telephony/ril.h>
#include <telephony/ril_cdma_sms.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "swiril_main.h"
#include "swiril_cache.h"
#include "swiril_misc_qmi.h"
#include "swiril_sms_qmi.h"
#include "swiril_network_qmi.h"
#include "swiril_device_qmi.h"

#include "SWIWWANCMAPI.h"
#include "qmerrno.h"
//#include "qmudefs.h"

#define LOG_TAG "RIL"
#include "swiril_log.h"

#include "at_channel.h"
#include "at_tok.h"
#include "at_misc.h"


#define MAX_MSG_LEN  256
#define MAX_SMSC_LEN 40

#define MESSAGE_CLASS_PRESENT_MASK 0x10
#define MESSAGE_CLASS_INFO_MASK    0x03

/* SMS Message format */
#define SMS_FORMAT_CDMA         0 
#define SMS_FORMAT_GSM_WCDMA    6

/* SMS Message Type*/
#define SMS_TYPE_UIM            0 /* on SIM */
#define SMS_TYPE_NV             1 /* on Handset*/

/* SMS Message format */
#define SMS_MESSAGE_MODE_CDMA         0 
#define SMS_MESSAGE_MODE_GSM_WCDMA    1

/* SMS deliver failure cause code per 3GPP2 N.S0005 Section 6.5.2.125 */
/* Definition for function requestSendSMSUMTS and requestSendSMSCDMA*/
#define  SMS_FAILURE_CAUSE_NETWORK_PROBLEM_BEGIN 0 
#define  SMS_FAILURE_CAUSE_NETWORK_PROBLEM_END 31 
#define  SMS_FAILURE_CAUSE_TERMINAL_PROBLEM_BEGIN 32 
#define  SMS_FAILURE_CAUSE_TERMINAL_PROBLEM_END 63 
#define  SMS_FAILURE_CAUSE_RADIO_INTERFACE_PROBLEM_BEGIN 64
#define  SMS_FAILURE_CAUSE_RADIO_INTERFACE_PROBLEM_END 95 
#define  SMS_FAILURE_CAUSE_GENERAL_PROBLEM_BEGIN 96 
#define  SMS_FAILURE_CAUSE_GENERAL_PROBLEM_END 255 

/* definition of status of message.  Per TS 27.005 3.1, "<stat>": */
#define RIL_SMS_STATUS_UNREAD 0
#define RIL_SMS_STATUS_READ 1
#define RIL_SMS_STATUS_UNSENT 2
#define RIL_SMS_STATUS_SENT 3

/* QMI definition of status of message */
#define QMI_SMS_STATUS_READ 0
#define QMI_SMS_STATUS_UNREAD 1
#define QMI_SMS_STATUS_SENT 2
#define QMI_SMS_STATUS_UNSENT 3

/* Definition for CDMA SMS */
#define bitsize(type) (sizeof(type) * 8)
#define copymask(type) ((0xffffffff) >> (32 - bitsize(type)))

#define MASK_B(offset, len) \
  ((0xff >> offset) & (0xff << (8 - (offset + len))))
#define  MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )

#define MASK_AND_SHIFT(width, offset, shift, data)  \
    /*lint -e504 Unusual shifter value */  \
                  ((((signed) (shift)) < 0) ?       \
                    MASK((width), (offset), (data)) << -(shift) :  \
                    MASK((width), (offset), (data)) >>  (((unsigned) (shift)))) \
    /*lint +e504 */

#define MASK(width, offset, data) \
    /*lint -e701 shift left  of signed quantity  */  \
    /*lint -e702 shift right of signed quantity  */  \
    /*lint -e572 Excessive shift value           */  \
    /*lint -e573 Signed-unsigned mix with divide */  \
    /*lint -e506 Constant value boolean          */  \
    /*lint -e649 Sign fill during constant shift */  \
                                                     \
   (((width) == bitsize(data)) ? (data) :   \
   ((((copymask(data) << (bitsize(data) - ((width) % bitsize(data)))) &copymask(data)) >>  (offset)) & (data))) \
                     \
    /*lint +e701 */  \
    /*lint +e702 */  \
    /*lint +e572 */  \
    /*lint +e573 */  \
    /*lint +e506 */  \
    /*lint +e649 */  

/*--------------PROTOCOL DEFINES----------------*/
#define POINT_TO_POINT_MSG  0 

#define TELESERVICE_ID_DEFAULT_LEN 5
#define TELESERVICE_IDENTIFIER 0
#define TELESERVICE_ID_DEFAULT_VALUE 4098
#define TELESERVICE_HDR_LEN 2

#define BEAREA_REPLY_DEFAULT_LEN 3
#define BEARER_REPLY_IDENTIFIER 6
#define BEARER_REPLY_HDR_LEN 1

#define USER_DATA_DEFAULT_LEN 200
 
#define BEARER_DATA_DEFAULT_LEN 2
#define BEARER_DATA_IDENTIFIER 8
#define MAX_SMS_MSG_LEN 160

#define PRIORIY_DEFAULT_LEN 3
#define PRIORIY_SUB_IDENTIFIER 8
#define PRIORIY_SUB_IDENTIFIER_HDR_LEN 1

#define REPLY_OPTION_SUB_IDENTIFIER 10
#define REPLY_OPTION_SUB_IDENTIFIERHDR_LEN 1

#define REL_VAL_PERIOD_DEFAULT_LEN 3
#define REL_VAL_PERIOD_SUB_IDENTIFIER 5
#define REL_VAL_PERIOD_SUB_IDENTIFIER_HDR_LEN 1

#define DESTADDR_IDENTIFIER 4
#define DESTADDR_HDR_LEN 7

#define ORIGADDR_IDENTIFIER 2
#define ORIGADDR_HDR_LEN 7

#define SERVICE_CATEGORY_IDENIFIER 1
#define SERVICE_CATEGORY_HDR_LEN 2

#define ORIGSUBADDR_IDENTIFIER 3

#define BINARY_FORMAT_NONE 0
#define BINARY_FORMAT_01_000000 64
#define BINARY_FORMAT_10_000000 128
#define BINARY_FORMAT_11_000000 192

#define REL_VALIDITY_0 11
#define REL_VALIDITY_1 71
#define REL_VALIDITY_2 167
#define REL_VALIDITY_3 169
#define REL_VALIDITY_4 171

#define MESSAGE_TYPE_SMS_DELIVER_MASK    0x00
#define MESSAGE_TYPE_SUBMIT_REPORT_MASK  0x01
#define MESSAGE_TYPE_STATUS_REPORT_MASK  0x02
#define MESSAGE_TYPE_RESERVED_MASK       0x03

#define MESSAGE_TYPE_POINT_TO_POINT 0x00
#define STORAGE_TYPE_NONE           0xFF
#define TRANSFER_AND_ACK            0x03

enum messageClassInfo {
    MESSAGE_CLASS_ZERO    = 0,
    MESSAGE_CLASS_ONE     = 1,
    MESSAGE_CLASS_TWO     = 2,
    MESSAGE_CLASS_THREE   = 3,
    MESSAGE_CLASS_UNKNOWN = 0xFF,
};

enum messageTypeInfo {
    MESSAGE_TYPE_SMS_DELIVER,
    MESSAGE_TYPE_STATUS_REPORT,
    MESSAGE_TYPE_SUBMIT_REPORT,
};

struct held_message {
    SMSEventInfo          *pMsg;
    enum messageClassInfo messageClass;
    struct held_message   *next;
};

static pthread_mutex_t s_held_message_mutex = PTHREAD_MUTEX_INITIALIZER;
static struct held_message *s_held_message_info = NULL;
static bool s_outstanding_acknowledge = false;

struct messageList
{
    ULONG messageIndex;
    ULONG messageTag;
};

struct messageDetails
{
    ULONG storageType;
    ULONG requestedTag;
    BYTE  messageMode;
    ULONG messageListSize;
    struct messageList *pMsg;
};

#define MAX_STORED_SMS      255

/* structure used by function requestSendSMSUMTS*/
typedef struct 
{
    ULONG  messageFormat; 
    ULONG  messageSize;
    USHORT messageID;
    ULONG  MessageFailureCode;
    BYTE   Message[MAX_MSG_LEN];    
} STRUCT_SendSMS;

/* structure used by function requestGet/SetSMSCAddressQMI */
typedef struct 
{
    BYTE   addressSize; 
    CHAR   SMSCAddress[MAX_SMSC_LEN]; 
    BYTE   typeSize; 
    CHAR   SMSCType[MAX_SMSC_LEN];
} STRUCT_SMSCAddress;

/* structure used by function requestWriteSMSToSimUMTS*/
typedef struct 
{
    ULONG   storageType; 
    ULONG   messageFormat; 
    ULONG   messageSize;
    ULONG   MessageIndex;
    BYTE    Message[MAX_MSG_LEN];    /* Max WMS Payload Size */
} STRUCT_SaveSMS;

/* Functions */
ULONG CDMADecodeMTTextMsg(BYTE *pRawMsg, ULONG pRawMsgLen, RIL_CDMA_SMS_Message *args);
ULONG CDMAEncodeMOTextMsg(BYTE *pOut, BYTE *pOutLen, RIL_CDMA_SMS_Message *args);
void NewSMSDispatch(SMSEventInfo *pSMSEventInfo);
void requestSendSMSUMTS_AT(void *data, size_t datalen, RIL_Token t);



unsigned char hex2bin(unsigned char h, unsigned char l)
{
    h |= 0x20; /* to lower */
    h -= 0x30;
    h -= -(h > 9) & 0x27;
    l |= 0x20;
    l -= 0x30;
    l -= -(l > 9) & 0x27;
    return h << 4 | l;
}

void bin2hex(unsigned char ch, unsigned char *h, unsigned char *l)
{
    unsigned char H, L;
    H = (ch >> 4 ) & 0xff;
    H += -(H > 9) & 0x27;
    H += 0x30;
    H |= 0x20; /* to lower */
    L = ch & 0x0f;
    L += -(L > 9) & 0x27;
    L += 0x30;
    L |= 0x20; /* to lower */
    *h = H;
    *l = L;
}

/**
 * Initialization of SMS service
 *
 * @param
 *     none
 *
 * @return
 *     none
 *
 * @note
 *     SMS Class 0 is routed as as TR message, without being stored in the
 *     device 
 */
void smsServiceInit(void)
{
    ULONG nRet;
    smsSetRoutesReq SetRoutesReq;
    memset (&SetRoutesReq, 0, sizeof(SetRoutesReq));

    SetRoutesReq.numOfRoutes = 1;
    SetRoutesReq.routeList[0].messageType   = MESSAGE_TYPE_POINT_TO_POINT;
    SetRoutesReq.routeList[0].messageClass  = MESSAGE_CLASS_ZERO;
    SetRoutesReq.routeList[0].routeStorage  = STORAGE_TYPE_NONE;
    SetRoutesReq.routeList[0].receiptAction = TRANSFER_AND_ACK;

    nRet = SLQSSmsSetRoutes( &SetRoutesReq );
    if (nRet != eQCWWAN_ERR_NONE ){
        LOGE("%s:: SLQSSmsSetRoutes failed nRet = %lu", __func__, nRet);
    }
}

/**
 * Removes the queued up message
 *
 * @param
 *     none
 *
 * @return
 *     pointer to the held_message structure, holding queued message
 *
 * @note
 *     Need to aquire a mutex before this function is invoked.
 */
static struct held_message *dequeue_held_message( void )
{
    struct held_message *message = NULL;

    if (s_held_message_info != NULL) {
        message = s_held_message_info;
        s_held_message_info = message->next;
        message->next = NULL;
    }
    return message;
}

/**
 * Get the first in queue, but will not remove the element from the queue
 *
 * @param
 *     none
 *
 * @return
 *     pointer to the held_message structure, holding queued message
 *
 * @note
 *     Need to aquire a mutex before this function is invoked.
 */
static struct held_message *get_held_message( void )
{
    return s_held_message_info;
}

/**
 * Get the SMS message class information from the incoming message PDU
 *
 * @param [in] message
 *     pointer to incoming SMS message PDU
 *
 * @param [in] msgLength
 *     length of the incoming SMS message PDU
 *
 * @return
 *     SMS class of the received SMS PDU
 *
 */
enum messageClassInfo retrieveMessageClass(BYTE *message, ULONG msgLength)
{
    BYTE  length = 0, tpDCS  = 0;

    /* Skip SMSC number information */
    length = *(message++);
    while (length--)
        message++;

    /* Skip the first address of SMS deliver message */
    message++;

    /**
     * Get the length of the sender address, and skip the address type
     * and address
     */
    length = *(message++);
    message++;
    length = (length+1)/2;
    while (length--)
        message++;

    /* Skip TP-PID */
    message++;

    /* Get TP-DCS */
    tpDCS = *message;

    /* If message class information is present, decode the same */
    if (tpDCS & MESSAGE_CLASS_PRESENT_MASK) {
        return (tpDCS & MESSAGE_CLASS_INFO_MASK);
    }
    return MESSAGE_CLASS_UNKNOWN;
}

/**
 * Get the SMS message type information from the incoming message PDU
 *
 * @param [in] message
 *     pointer to incoming SMS message PDU
 *
 * @param [in] SMSCPresent
 *     true if SMSC information is present, false otherwise
 *
 * @return
 *     SMS type of the received SMS PDU
 *
 */
enum messageTypeInfo retrieveMessageType(BYTE *message, bool SMSCPresent)
{
    BYTE  length = 0, smsTPDU  = 0;

    if (SMSCPresent) {
        length = *(message++);
        while (length--)
            message++;
    }

    /* Get the TPDU parameters */
    smsTPDU = *message;

    /* If message type information is present, decode the same */
    if (smsTPDU & MESSAGE_TYPE_SMS_DELIVER_MASK) {
        return MESSAGE_TYPE_SMS_DELIVER;
    } else if (smsTPDU & MESSAGE_TYPE_SUBMIT_REPORT_MASK) {
        return MESSAGE_TYPE_SUBMIT_REPORT;
    } else if (smsTPDU & MESSAGE_TYPE_STATUS_REPORT_MASK) {
        return MESSAGE_TYPE_STATUS_REPORT;
    }
    return MESSAGE_TYPE_SMS_DELIVER;
}

/**
 * Enqueue a received MT or TR message
 *
 * @param [in] msg 
 *     pointer to SMSEventInfo structure
 * @param [in] messageMode
 *     mode of the received message
 *
 * @return
 *     none
 *
 * @note
 *     Need to aquire a mutex before this function is invoked.
 */
static void enqueue_held_message(SMSEventInfo *msg, BYTE messageMode)
{
    struct held_message *p = NULL;
    struct held_message *message = malloc(sizeof(*message));
    SMSEventInfo *pMsg = malloc(sizeof(*pMsg));

    memset(message, 0, sizeof(*message));
    memset(pMsg, 0, sizeof(*pMsg));
    message->pMsg = pMsg;
    message->messageClass = MESSAGE_CLASS_UNKNOWN;

    /**
     * Create storage area, and store the information of the received SMS
     * in the queue
     */
    message->pMsg->smsEventType = msg->smsEventType;

    /* Set the messsage mode information */
    if (msg->pMessageModeInfo) {
        SMSMessageModeInfo *pMsgMode;
        message->pMsg->pMessageModeInfo = malloc(sizeof(SMSMessageModeInfo));
        pMsgMode = message->pMsg->pMessageModeInfo;
        pMsgMode->messageMode = msg->pMessageModeInfo->messageMode;
    /**
     * Message mode is mandatory, hence setting this message mode even if
     * this is not received from the device
     */ 
    } else {
        message->pMsg->pMessageModeInfo = malloc(sizeof(SMSMessageModeInfo));
        message->pMsg->pMessageModeInfo->messageMode = messageMode;
    }

    /* Set the MT message Information */
    if (msg->pMTMessageInfo) {
        SMSMTMessageInfo *pMTMesg;
        message->pMsg->pMTMessageInfo = malloc(sizeof(SMSMTMessageInfo));
        pMTMesg = message->pMsg->pMTMessageInfo;

        pMTMesg->storageType = msg->pMTMessageInfo->storageType;
        pMTMesg->messageIndex = msg->pMTMessageInfo->messageIndex;

        /**
         * Check to find whether the message is class 0 message, and if so needs
         * to be treated in priority and has to be inserted into the beginning
         * of the queue. This has to be only taken care for GSM messages now
         */ 
        if (message->pMsg->pMessageModeInfo->messageMode ==
            SMS_MESSAGE_MODE_GSM_WCDMA) {
            ULONG tag;
            ULONG format;
            ULONG msgLength = MAX_MSG_LEN;
            BYTE  receivedSMS[MAX_MSG_LEN];
            ULONG nRet = 0;

            nRet = SLQSGetSMS( pMTMesg->storageType,
                               pMTMesg->messageIndex,
                               &tag,
                               &format,
                               &msgLength,
                               receivedSMS,
                               &messageMode );

            if (nRet != eQCWWAN_ERR_NONE ){
                LOGE("%s:: SLQSGetSMS failed nRet = %lu", __func__, nRet);
                return ;
            }
            message->messageClass = retrieveMessageClass(receivedSMS, msgLength);

            /**
             * Treat message class 0 with highest priority, and enqueue to the
             * beginning of the queue if there is no outstanding ACK waiting,
             * or second of the queue if there is a wait for the ACK of the
             * previous SMS  
             */
            if (message->messageClass == MESSAGE_CLASS_ZERO) {
                if (s_outstanding_acknowledge) {
                    LOGI("%s:: Class 0 - Add second to the queue", __func__);
                    if (s_held_message_info == NULL) {
                        LOGE("%s:: s_held_message_info == NULL", __func__);
                    } else {
                        p = message;
                        p->next = s_held_message_info->next;
                        s_held_message_info->next = p;
                    }
                } else {
                    LOGI("%s:: Class 0 - Add to beginning of queue", __func__);
                    p = message;
                    p->next = s_held_message_info;
                    s_held_message_info = p;
                }
                return;
            }
        }
    }

    /* Set the Transfer Route message Information */
    if (msg->pTransferRouteMTMessageInfo) {
        BYTE retrievedMessage[MAX_MSG_LEN];
        SMSTransferRouteMTMessageInfo *pTRMesg;
        message->pMsg->pTransferRouteMTMessageInfo =
                 malloc(sizeof(SMSTransferRouteMTMessageInfo));
        pTRMesg = message->pMsg->pTransferRouteMTMessageInfo;

        pTRMesg->format = msg->pTransferRouteMTMessageInfo->format;
        pTRMesg->length = msg->pTransferRouteMTMessageInfo->length;
        memcpy((char *)pTRMesg->data,
               (const char *)msg->pTransferRouteMTMessageInfo->data,
               pTRMesg->length);

        retrievedMessage[0] = 0x00;
        memcpy((char *)&(retrievedMessage[1]),
               (const char *)msg->pTransferRouteMTMessageInfo->data,
               pTRMesg->length);

        message->messageClass =
                 retrieveMessageClass(retrievedMessage, pTRMesg->length+1);

        /**
         * Treat message class 0 with highest priority, and enqueue to the
         * beginning of the queue if there is no outstanding ACK waiting,
         * or second of the queue if there is a wait for the ACK of the
         * previous SMS  
         */
        if (message->messageClass == MESSAGE_CLASS_ZERO) {
            if (s_outstanding_acknowledge) {
                LOGI("%s:: Class 0 - Add second to the queue", __func__);
                if (s_held_message_info == NULL) {
                    LOGE("%s:: s_held_message_info == NULL", __func__);
                } else {
                    p = message;
                    p->next = s_held_message_info->next;
                    s_held_message_info->next = p;
                }
            } else {
                LOGI("%s:: Class 0 - Add to beginning of queue", __func__);
                p = message;
                p->next = s_held_message_info;
                s_held_message_info = p;
            }
        }
    }

    /* Set the SMSC address Information */
    if (msg->pSMSCAddressInfo) {
        SMSCAddressInfo *pSMSC;
        message->pMsg->pSMSCAddressInfo = malloc(sizeof(SMSCAddressInfo));
        pSMSC = message->pMsg->pSMSCAddressInfo;

        pSMSC->length = msg->pSMSCAddressInfo->length;
        strcpy((char *)pSMSC->data,
               (const char *)msg->pSMSCAddressInfo->data);
    }

    if (message->messageClass == MESSAGE_CLASS_ZERO)
        return;

    /* Add this element to the end of the queue */
    if (s_held_message_info == NULL)
       s_held_message_info = message; 
    else {
        struct held_message *p = s_held_message_info;
        while (p->next != NULL)
            p = p->next;

        p->next = message;
    }
}

/**
 * Free the node removed from the queue
 *
 * @param [in] msg 
 *     pointer to held_message structure
 *
 * @return
 *     none
 *
 */
void free_held_message(struct held_message *msg)
{
    if(msg) {
        if (msg->pMsg->pMessageModeInfo) {
            free(msg->pMsg->pMessageModeInfo);
        }
        if (msg->pMsg->pMTMessageInfo) {
            free(msg->pMsg->pMTMessageInfo);
        }
        if (msg->pMsg->pTransferRouteMTMessageInfo) {
            free(msg->pMsg->pTransferRouteMTMessageInfo);
        }
        if (msg->pMsg->pSMSCAddressInfo) {
            free(msg->pMsg->pSMSCAddressInfo);
        }
        free(msg->pMsg);
        free(msg);
    }
}

/**
 * Delete the message from the queue and the device
 *
 * @param
 *     none
 *
 * @return
 *     none
 *
 * @note
 *     Need to aquire a mutex before this function is invoked.
 */
void delete_held_message( void )
{
    struct held_message *pMessage;
    ULONG nRet = eQCWWAN_ERR_NONE;

    LOGI("%s:: Delete the message from queue and device", __func__);
    /**
     * Get the SMS which was ACKed from the queue, delete from device and free
     * from the queue
     */
    pMessage = dequeue_held_message();
    if(pMessage) {
        /**
         * Need to delete the SMS from device only if the message is MT message,
         * TR messages are not stored in the device
         */
        if (pMessage->pMsg->smsEventType & SMS_EVENT_MT_MESSAGE) {
            /**
             * Delete the SMS from the device based on the information stored
             * in queue. The storage type and messageIndex are mandatory,
             * message tag is not required and message mode is kept for
             * combatability with MC7750
             */ 
            nRet = SLQSDeleteSMS(pMessage->pMsg->pMTMessageInfo->storageType, 
                            &(pMessage->pMsg->pMTMessageInfo->messageIndex),
                            NULL, /* No need to specify message tag info */ 
                            &(pMessage->pMsg->pMessageModeInfo->messageMode));
            if (nRet != eQCWWAN_ERR_NONE )
                LOGE("%s:: SLQSDeleteSMS failed nRet = %lu", __func__, nRet);
        }
    }
    free_held_message(pMessage);
}

/**
 * To find whether the message is already present in the message queue
 *
 * @param [in] Msg
 *     structure SMSEventInfo containing message information
 *
 * @return
 *     boolean indicating true if message is present in queue, false otherwise
 *
 * @note
 *     This function is invoked when the SLQS SDK start/restart, either through
 *     device reboot or through a software crash or at initialization, and the
 *     messages are loaded from the UIM/NV. This function would decide whether
 *     the loaded message is already present in the internal message queue.
 *     This function also assumes that the message that is incoming is an MT
 *     message as only MT messages are stored in the device
 */
bool message_present_in_queue(SMSEventInfo Msg)
{
    struct held_message *p = s_held_message_info;

    /** 
     * Process only when message is an MT message, a TR message is not stored in
     * SIM or NV and hence this function should not be invoked in that scenario
     */
    if (!(Msg.smsEventType & SMS_EVENT_MT_MESSAGE))
        return false;

    /**
     * Process only when message is an MT message and return true when the
     * message matches with any of the message stored in the queue
     */
    while (p != NULL) {
        if ((p->pMsg->smsEventType & SMS_EVENT_MT_MESSAGE) &&
            (p->pMsg->pMessageModeInfo->messageMode
                 == Msg.pMessageModeInfo->messageMode) &&
            (p->pMsg->pMTMessageInfo->storageType
                 == Msg.pMTMessageInfo->storageType) &&
            (p->pMsg->pMTMessageInfo->messageIndex
                 == Msg.pMTMessageInfo->messageIndex)) {
            return true;
        }
        p = p->next;
    }
    return false;
}

/**
 * Enqueue the messages from the device message list to the internal message
 * queue. 
 *
 * @param [in] pMsgLst 
 *     pointer to messageDetails structure, containing the unread messages from
 *     the device
 *
 * @return
 *     none
 *
 * @note
 *     none
 */
void enqueue_messages_to_queue (struct messageDetails *pMsgLst)
{
    SMSEventInfo Msg;
    SMSMessageModeInfo messageModeInfo;
    SMSMTMessageInfo mtMessageInfo;
    ULONG index = 0;

    /* Initialize the Msg, and the required parameters */
    memset(&Msg, 0, sizeof(SMSEventInfo));
    Msg.pMessageModeInfo = &messageModeInfo;
    Msg.pMTMessageInfo = &mtMessageInfo;

    /* All the enqueued message are of MT type */
    Msg.smsEventType = SMS_EVENT_MT_MESSAGE;
    LOGI("Enqueue Messages to Queue Storage : %lx", pMsgLst->storageType);

    /* Enqueue all the messages that are present in the list */
    index = pMsgLst->messageListSize;
    while(index--)
    {
        LOGI("Index : %lx :: messageMode : %d :: messageIndex : %lx",
             index,
             pMsgLst->messageMode,
             pMsgLst->pMsg[index].messageIndex);

        Msg.pMessageModeInfo->messageMode = pMsgLst->messageMode;
        Msg.pMTMessageInfo->storageType   = pMsgLst->storageType;
        Msg.pMTMessageInfo->messageIndex  = pMsgLst->pMsg[index].messageIndex;

        /* Use the mutex to enqueue the SMS into the queue */
        pthread_mutex_lock(&s_held_message_mutex);
        if(!message_present_in_queue(Msg))
            enqueue_held_message(&Msg, pMsgLst->messageMode);
        pthread_mutex_unlock(&s_held_message_mutex);
    }
}

/**
 * Prioritize the message queue to push all the class 0 messages to the
 * beginning of the queue 
 *
 * @param
 *     none
 *
 * @return
 *     true if re-prioritized, else false
 *
 * @note
 *     Need to aquire a mutex before this function is invoked.
 */
bool parse_and_prioritize_queue()
{
    struct held_message *p = s_held_message_info;

    if (p != NULL) {
        /**
         * If the first element present in the queue is a class 0 message the
         * queue is already prioritized, hence we return false
         */
        if (p->messageClass == MESSAGE_CLASS_ZERO) {
            return false;
        }
        /**
         * If the second element present in the queue is a class 0 message the
         * queue needs to be prioritized, push all the class 0 messages to the
         * beginning of the queue and return true so that the latest class 0
         * message is dispatched 
         */
        if (p->next != NULL) {
            if (p->next->messageClass == MESSAGE_CLASS_ZERO) {
                struct held_message *q;
                p = p->next;

                /**
                 * If there are class 0 messages present and are not in front of
                 * the queue, the class 1 message on top has to be inserted to
                 * the end of the class 0 messages in the queue
                 */
                while (p != NULL) {
                    if (p->next != NULL) {
                        if(p->next->messageClass != MESSAGE_CLASS_ZERO) {
                            break;
                        }
                    } else {
                        break;
                    }
                    p = p->next;
                }
                q = s_held_message_info;
                s_held_message_info = s_held_message_info->next;
                q->next = p->next;
                p->next = q;
                return true;
            }
        }
    }
    return false;
}

/**
 * Enqueued function to fetch and dipatch the pending SMS, if any
 *
 * @param [in] params
 *     NULL in this case
 *
 * @return
 *     none
 */
static void onSMSFailure (void *params)
{
    struct held_message *message;

    pthread_mutex_lock(&s_held_message_mutex);
    /* Get the first message in the queue to be sent to the application */  
    message = get_held_message();
    if (message != NULL) {
        LOGI("Outstanding requests in queue, dequeueing and sending.");
        NewSMSDispatch(message->pMsg);
    }
    pthread_mutex_unlock(&s_held_message_mutex);
}

/**
 * Send the unsolicited SMS response for the application layer. This is invoked
 * when the application layer is ready to receive a new SMS.
 * (when there is no outstanding ACK's)
 *
 * @param [in] pSMSEventInfo 
 *     pointer to SMSEventInfo structure
 *
 * @return
 *     none
 *
 * @note
 *     This function will update s_outstanding_acknowledge to true if an
 *     unsolicited response has been sent. This function has to be invoked
 *     only after aquiring "s_held_message_mutex".
 */
void NewSMSDispatch(SMSEventInfo *pSMSEventInfo)
{
    BYTE messageMode;
    ULONG tag;
    ULONG format;
    ULONG msgLength = MAX_MSG_LEN;
    BYTE  message[MAX_MSG_LEN];
    BYTE  messageOut[MAX_MSG_LEN*2];
    ULONG i = 0;
    ULONG iIndex = 0;
    ULONG nRet = 0;

    /* new MT SMS received */
    if ( pSMSEventInfo->smsEventType & SMS_EVENT_MT_MESSAGE ) {
    
        /* clear PDU */
        memset(message, 0, sizeof(message));

        messageMode = pSMSEventInfo->pMessageModeInfo->messageMode;
        LOGD("%s:: MT Message, Storage Type: %lu, Index: %lu Mode: %d \n",
             __func__, 
             pSMSEventInfo->pMTMessageInfo->storageType,
             pSMSEventInfo->pMTMessageInfo->messageIndex,
             messageMode);

        /* call Sierra QMI interface */
        nRet = SLQSGetSMS( pSMSEventInfo->pMTMessageInfo->storageType,
                           pSMSEventInfo->pMTMessageInfo->messageIndex,
                           &tag,
                           &format,
                           &msgLength,
                           message,
                           &messageMode );

        if (nRet != eQCWWAN_ERR_NONE ){
            LOGE("%s:: SLQSGetSMS failed nRet = %lu", __func__, nRet);
            return ;
        }

        LOGD("%s:: GetSMS succeed, format: %lu, msgLength:%lu",
             __func__,
             format,
             msgLength);

        if(format == SMS_FORMAT_GSM_WCDMA){
            memset(messageOut, 0, sizeof(messageOut));
            for(i=0; i < msgLength; i++) {
                bin2hex(message[i], &messageOut[2*i], &messageOut[2*i+1]);
            }
            s_outstanding_acknowledge = true;
            RIL_onUnsolicitedResponse( RIL_UNSOL_RESPONSE_NEW_SMS,
                                       messageOut,
                                       msgLength * 2 );
        } else if(format == SMS_FORMAT_CDMA){
            RIL_CDMA_SMS_Message args;
            memset(&args, 0, sizeof(RIL_CDMA_SMS_Message));
            nRet = CDMADecodeMTTextMsg((BYTE *)message, msgLength, &args);
            if(nRet != eQCWWAN_ERR_NONE){
                LOGE("%s:: CDMADecodeMTTextMsg error %lu", __func__, nRet);
                return; 
            }
            s_outstanding_acknowledge = true;
            RIL_onUnsolicitedResponse( RIL_UNSOL_RESPONSE_CDMA_NEW_SMS,
                                       &args,
                                       sizeof(RIL_CDMA_SMS_Message) );
        } else {
            /**
             * The stored message from the beginning of the queue is deleted
             * from queue on unsupported format
             */  
            LOGE("%s:: Unsupported SMS format:  %lu", __func__, format);
            delete_held_message();
            enqueueRILEvent(RIL_EVENT_QUEUE_PRIO, onSMSFailure, NULL, NULL);
        }
    } else if ( pSMSEventInfo->smsEventType &
                SMS_EVENT_TRANSFER_ROUTE_MT_MESSAGE ) {
        SMSTransferRouteMTMessageInfo *pTRMesg = 
                pSMSEventInfo->pTransferRouteMTMessageInfo;
        /* new Transfer Routing MT SMS received */
        LOGD("%s:: Transfer route MT message received, msg length:%d",
             __func__, pTRMesg->length);
        
        /* clear PDU */
        memset(messageOut, 0, sizeof(messageOut));

        /* only support UMTS mode now */
        if (pTRMesg->format == SMS_FORMAT_GSM_WCDMA) {
            enum messageTypeInfo messageType;

            /* if smsc is NULL, add 0 to the PDU*/
            if(pSMSEventInfo->pSMSCAddressInfo == NULL) {
               LOGD("%s::SMSC is NULL", __func__);
               bin2hex(0x0, &messageOut[0], &messageOut[1]); 
               iIndex = 1; 
            } else {
                /* add smsc length to PDU*/
                bin2hex( pSMSEventInfo->pSMSCAddressInfo->length,
                         &messageOut[0],
                         &messageOut[1] );
                iIndex = 1;
                /* add smsc to PDU*/
                for(i=0; i < pSMSEventInfo->pSMSCAddressInfo->length; i++) {
                    bin2hex( pSMSEventInfo->pSMSCAddressInfo->data[i],
                             &messageOut[2* (i +iIndex)],
                             &messageOut[2*(i+iIndex)+1] );
                }
                iIndex += pSMSEventInfo->pSMSCAddressInfo->length;
            }

            /* add return status report message to PDU*/
            for(i=0; i < pTRMesg->length; i++) {
                bin2hex( pTRMesg->data[i],
                         &messageOut[2* (i +iIndex)],
                         &messageOut[2*(i+iIndex)+1]);
            }
            s_outstanding_acknowledge = true;

            messageType = retrieveMessageType (pTRMesg->data, false);
            if (messageType == MESSAGE_TYPE_SMS_DELIVER) {
                RIL_onUnsolicitedResponse(RIL_UNSOL_RESPONSE_NEW_SMS,
                                          messageOut,
                                          (pTRMesg->length + iIndex)*2 );
            } else if (messageType == MESSAGE_TYPE_STATUS_REPORT) {
                RIL_onUnsolicitedResponse(
                    RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT,
                    messageOut, (pTRMesg->length + iIndex)*2 );
            } else {
                /**
                 * The stored message from the beginning of the queue is deleted
                 * from the queue on unsupported message type
                 */  
                LOGE("%s:: Unsupported SMS type %d ", __func__, messageType);
                s_outstanding_acknowledge = false;
                delete_held_message();
                enqueueRILEvent(RIL_EVENT_QUEUE_PRIO, onSMSFailure, NULL, NULL);
            }

        } else {
            /**
             * The stored message from the beginning of the queue is deleted
             * from the queue on unsupported message format
             */  
            LOGE("%s:: unsupported format %d transfer route MT SMS received ",
                 __func__, pTRMesg->format);
            delete_held_message();
            enqueueRILEvent(RIL_EVENT_QUEUE_PRIO, onSMSFailure, NULL, NULL);
        }
    } else {
        /**
         * The stored message from the beginning of the queue is deleted from
         * queue on unsupported SMS event type
         */  
        LOGE("%s:: MT SMS type 0x%x not supported", __func__,
             pSMSEventInfo->smsEventType);
        delete_held_message();
        enqueueRILEvent(RIL_EVENT_QUEUE_PRIO, onSMSFailure, NULL, NULL);
    }
}

/**
 * Get the message mode based on the technology type and the device being used
 *
 * @param [out] pMessageMode
 *     The message mode used
 *
 * @return
 *     boolean indicating true if messageMode is retrieved, false otherwise
 *
 * @note
 *     none
 */
bool getMessageMode (BYTE *pMessageMode)
{
    SWI_FW_INFO_TYPE technology = SWI_FW_INFO_TYPE_UNKNOWN;
    bool bRet;

    /**
     * Set message mode according to technology type
     * MC7750-like modules use SMS in CDMA format only
     */
    bRet = getFirmwareInfoTechQMI(&technology);
    if(!bRet)
        return bRet;

    switch(technology)
    {
        case SWI_FW_INFO_TYPE_GOBI_CDMA:
        case SWI_FW_INFO_TYPE_SWI_DUAL_MODE:
            *pMessageMode = SMS_MESSAGE_MODE_CDMA;
            break;
        case SWI_FW_INFO_TYPE_GOBI_UMTS:
        case SWI_FW_INFO_TYPE_SWI_UMTS_ONLY:
            *pMessageMode = SMS_MESSAGE_MODE_GSM_WCDMA;
            break;
        default:
            bRet = false;
            break;
    }
    LOGI("Message Mode :: %d", *pMessageMode);
    return bRet;
}

/**
 * Get the list of unread SMS from the device. The list is created from the UIM
 * and the NV and enqueued to the message queue.
 *
 * @param [in] params
 *     The parameters which are passed to this enqueued function.
 *     They are unused.
 *
 * @return
 *     none
 *
 * @note
 *     none
 */
void getSMSList( void *params )
{
    struct messageList *msgLst;
    struct messageDetails msgDetails;
    struct held_message *message;
    BYTE  messageMode = SMS_MESSAGE_MODE_GSM_WCDMA; /*default to GSM/UMTS */
    ULONG nRet;
    bool isMsgEnqueued = false;

    if (!getMessageMode (&messageMode)) {
        LOGE("%s :: getMessageMode() error", __func__);
        return;
    }

    msgLst = malloc (sizeof(struct messageList)*MAX_STORED_SMS); 

    /* Get the list of all the unread SMS from the UIM */
    msgDetails.storageType     = SMS_TYPE_UIM;
    msgDetails.requestedTag    = QMI_SMS_STATUS_UNREAD;
    msgDetails.messageListSize = MAX_STORED_SMS;
    msgDetails.pMsg            = msgLst;
    msgDetails.messageMode     = messageMode;
    nRet = SLQSGetSMSList( msgDetails.storageType,
                           &msgDetails.requestedTag,
                           &msgDetails.messageListSize,
                           (BYTE *)msgDetails.pMsg,
                           &msgDetails.messageMode );
    if (nRet == eQCWWAN_ERR_NONE) {
        if(msgDetails.messageListSize > 0) {
            isMsgEnqueued = true;
        }
        enqueue_messages_to_queue(&msgDetails);
    } else {
            LOGE("SLQSGetSMSList (from UIM) failed : %lu", nRet);
    }

    /* Get the list of all the unread SMS from the NV */
    msgDetails.storageType     = SMS_TYPE_NV;
    msgDetails.requestedTag    = QMI_SMS_STATUS_UNREAD;
    msgDetails.messageListSize = MAX_STORED_SMS;
    msgDetails.pMsg            = msgLst;
    msgDetails.messageMode     = messageMode;
    nRet = SLQSGetSMSList( msgDetails.storageType,
                           &msgDetails.requestedTag,
                           &msgDetails.messageListSize,
                           (BYTE *)msgDetails.pMsg,
                           &msgDetails.messageMode );
    if (nRet == eQCWWAN_ERR_NONE) {
        if(msgDetails.messageListSize > 0) {
            isMsgEnqueued = true;
        }
        enqueue_messages_to_queue(&msgDetails);
    } else {
            LOGE("SLQSGetSMSList (from NV) failed : %lu", nRet);
    }

    /**
     * Get the first message in the queue to be sent to the application, if
     * there are enqueued messages
     */
    pthread_mutex_lock(&s_held_message_mutex);
    if(isMsgEnqueued && !s_outstanding_acknowledge) {
        message = get_held_message();
        if (message != NULL) {
            LOGI("Outstanding requests in queue, sending.");
            NewSMSDispatch(message->pMsg);
        }
    }
    pthread_mutex_unlock(&s_held_message_mutex);
    free(msgLst);
}


/**
 * RIL_REQUEST_SEND_SMS
 *
 * Sends an SMS message for immediate over the air transmission
 * 
 */
void requestSendSMSUMTS(void *data, size_t datalen, RIL_Token t)
{
    BYTE *SMSAddress = NULL;
    BYTE *PDU        = NULL;
    int  iPDUSize  = 0;
    int  iSMSCSize  = 0;
    int  i = 0;
    int  iIndex = 0; 
    RIL_Errno ret = RIL_E_SUCCESS;
    /* SMS message to save can be refered at http://dreamfabric.com/sms/ */
    STRUCT_SendSMS sParam;
    RIL_SMS_Response response;
    slqssendsmsparams_s messageParams;
    int with_udh = 0;

    /* clear structure */
    memset(&sParam, 0, sizeof(STRUCT_SendSMS));
    memset(&response, 0, sizeof(RIL_SMS_Response));

    SMSAddress = ((BYTE**)data)[0];
    PDU        = ((BYTE**)data)[1];

    LOGD("PDU: %s",PDU);
    
    /* Is UDH bit set?*/
    if ( hex2bin(PDU[0],PDU[1]) & 0x40 )
    { 
        #if 1
        LOGD("It is a multi-sms, send thr AT");
        /* hack for MC8355 fw 1.7, need to use AT command to send multi-sms */
        /* BZ48187, need this pack to pass PTCRB Multi-SMS test case (3GPP 34.123-1 16.1.9.1) */
        return requestSendSMSUMTS_AT(data, datalen, t);
        #else
        /* Use QMI to send multi-SMS by setting link_timer=3 */
        /* Test still fail on Anite PTCRB Multi-SMS test case, need further investigation*/
        LOGD("It is a multi-sms, send thr QMI");
        with_udh = 1;
        #endif
    }
    else
        LOGD("It is a normal sms");
    
    /* initialize structure */
    sParam.messageFormat = SMS_FORMAT_GSM_WCDMA; 

    /* copy SMSC to Message */
    if(SMSAddress == NULL)
        iIndex = 1; /*if smsc is NULL, add 0 to the PDU*/
    else {
        iSMSCSize = (int)( hex2bin(SMSAddress[0],  SMSAddress[1] ));
        if(iSMSCSize > 0){
            sParam.Message[0] = iSMSCSize;
            iIndex ++ ;

            for(i = 1;   i < iSMSCSize + 1; i ++) {
                sParam.Message[iIndex] = hex2bin(SMSAddress[i*2] , SMSAddress[i*2 + 1] );
                iIndex ++;
            }
        }
        else 
            iIndex = 1; /*if smsc is NULL, add 0 to the PDU*/
    }

    /* copy PDU to Message */
    if(PDU != NULL){
        iPDUSize = (int)strlen((const char *)PDU);
        sParam.messageSize = iPDUSize/2 + iIndex; 
        for(i=0; i < iPDUSize; i+=2) 
        {
            sParam.Message[iIndex] = hex2bin(PDU[i], PDU[i+1]);
            iIndex++;
        }
    }

    if(iPDUSize == 0){
        LOGE("%s SMS PDU size is 0", __func__);
        ret = RIL_E_GENERIC_FAILURE;
        goto error;
    }
    
    LOGD("SMSC is %s, messageSize is %lu", SMSAddress, sParam.messageSize);
    
    messageParams.messageFormat      = sParam.messageFormat;
    messageParams.messageSize        = sParam.messageSize;
    messageParams.pMessage           = sParam.Message;

    /* set link timer to 3s if it has udh, means it is a multi-sms */
    if(with_udh)
        messageParams.linktimer          = 3;
    else
        messageParams.linktimer          = 0;

    /* call Sierra QMI interface */
    ULONG nRet = SLQSSendSMS( &messageParams );

    if (nRet != eQCWWAN_ERR_NONE ){
        LOGE("Message sending response: %lu, nRet = %lu", messageParams.messageFailureCode, nRet);
        if(nRet == eQCWWAN_ERR_NO_CONNECTION)
            ret = RIL_E_RADIO_NOT_AVAILABLE;
        else{
            if(messageParams.messageFailureCode <= SMS_FAILURE_CAUSE_NETWORK_PROBLEM_END)
                ret = RIL_E_SMS_SEND_FAIL_RETRY;
            else if((messageParams.messageFailureCode >= SMS_FAILURE_CAUSE_RADIO_INTERFACE_PROBLEM_BEGIN)&&
                    (messageParams.messageFailureCode <= SMS_FAILURE_CAUSE_RADIO_INTERFACE_PROBLEM_END))
                ret = RIL_E_RADIO_NOT_AVAILABLE;
            else
                ret = RIL_E_GENERIC_FAILURE;
        }
        goto error;
    }
    else {
        LOGD("Send SMS: Message Reference %d", messageParams.messageID);
        response.errorCode  = -1;
        response.messageRef = messageParams.messageID;
        RIL_onRequestComplete(t, RIL_E_SUCCESS,  &response, sizeof(response));
    }
    return;

error:
    RIL_onRequestComplete(t, ret, NULL, 0);
}


/**
 * RIL_REQUEST_SMS_ACKNOWLEDGE
 *
 * Acknowledge successful or failed receipt of SMS previously indicated
  * via RIL_UNSOL_RESPONSE_NEW_SMS
*/
void requestSMSAcknowledgeUMTS(void *data, size_t datalen, RIL_Token t)
{
    int ackSuccess;
    int err;
    struct held_message *message;

    /**
     * Protect the entire function - need to make sure that any new incoming
     * SMS (in callback thread) is not serviced before treating the current ACK
     */
    pthread_mutex_lock(&s_held_message_mutex);

    /* If an ACK is received previously, ignore subsequent ACKs */
    if (!s_outstanding_acknowledge) {
        pthread_mutex_unlock(&s_held_message_mutex);
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
        return;
    }

    /** 
     * Received a valid ACK, this variable would be updated to true by
     * NewSMSDispatch(), if there is an SMS to be dispatched.
     */
    s_outstanding_acknowledge = false;

    ackSuccess = ((int *)data)[0];
    if (ackSuccess == 1)  {
        LOGD("requestSMSAcknowledge:ackSuccess : %d\n", ackSuccess);
    } else {
        LOGE("Failure %d to RIL_REQUEST_SMS_ACKNOWLEDGE\n", ackSuccess);

        /** 
         * The previous message was not successfully acked, try re-sending it
         * again later on reception of a New SMS. If there are any pending
         * class 0 messages, send it on priority and reprioritize the queue
         */  
        if (parse_and_prioritize_queue()) {
            /**
             * Send the first class 0 message in the queue to be sent to
             * the application
             */  
            message = get_held_message();
            if (message != NULL) {
                LOGI("Outstanding class 0 message in queue, Sending.");
                NewSMSDispatch(message->pMsg);
            }
        }
        goto end;
    }

    /**
     * The stored message from the beginning of the queue (for which the ACK
     * is received) is deleted from Device on successfull Ack
     */  
    delete_held_message();

    /* Get the first message in the queue to be sent to the application */  
    message = get_held_message();
    if (message != NULL) {
        LOGI("Outstanding requests in queue, dequeueing and sending.");
        NewSMSDispatch(message->pMsg);
    }

end:
    pthread_mutex_unlock(&s_held_message_mutex);
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
}

/**
 * Call back function for receiving new SMS
 *
 * @param [in] pSMSEventInfo 
 *     pointer to SMSEventInfo structure
 *
 * @return
 *     none
 */
static void NewSmsCallback(SMSEventInfo *pSMSEventInfo)
{
    BYTE  messageMode = SMS_MESSAGE_MODE_GSM_WCDMA; /*default to GSM/UMTS */
    struct held_message *message;

    if(pSMSEventInfo == NULL) {
        LOGE("%s:: pSMSEventInfo is NULL\n", __func__);
        return;
    }

    if (!getMessageMode (&messageMode)) {
        LOGE("%s :: getMessageMode() error", __func__);
        return;
    }

    pthread_mutex_lock(&s_held_message_mutex);
    /**
     * Need to enqueue the message, even if dispatched. This will be used to
     * delete on successfull reception of an ACK from the appliciation
     */
    enqueue_held_message(pSMSEventInfo, messageMode);

    if (s_outstanding_acknowledge) {
        LOGI("Waiting for ack for previous sms");
    } else {
        /* Get the first message in the queue to be sent to the application */  
        message = get_held_message();
        if (message != NULL) {
            LOGI("Outstanding requests in queue, sending from queue");
            NewSMSDispatch(message->pMsg);
        }
    }
    pthread_mutex_unlock(&s_held_message_mutex);
}

/**
 * Register new SMS notification call back function
 *
 * @param 
 *     none
 *
 * @return
 *     none
 */
void registerNewSMSCallbackQMI(void)
{
    LOGD("%s:: entered\n", __func__);
    ULONG nRet = 0;

    /* register callback function */
    nRet = SLQSSetSMSEventCallback(NewSmsCallback);
    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGE("%s:: SLQSSetSMSEventCallback failed, nRet: %lu\n", __func__, nRet );
    }
}

/**
 * RIL_REQUEST_WRITE_SMS_TO_SIM
 *
 * Stores a SMS message to SIM memory.
 * 
 */
void requestWriteSMSToSimUMTS(void *data, size_t datalen, RIL_Token t)
{
    ULONG nRet = 0;
    ULONG nStatus = 0;
    int   i=0    ;
    int   iIndex = 0;
    int   iPDUSize  = 0;
    int   iSMSCSize  = 0;
    RIL_SMS_WriteArgs *args = NULL;
    STRUCT_SaveSMS sParam;
    RIL_Errno ret = RIL_E_SUCCESS;
    BYTE  messageMode = SMS_MESSAGE_MODE_GSM_WCDMA; 

    /*SMS message to save can be refered at http://dreamfabric.com/sms/ */
    args = (RIL_SMS_WriteArgs *) data;
    LOGD("SMS to be saved: status: %d, PDU: %s, SMSC: %s", 
        args->status, args->pdu, args->smsc);
    
    /* clear structure */
    memset(&sParam, 0, sizeof(STRUCT_SaveSMS));

    sParam.storageType = SMS_TYPE_UIM;
    sParam.messageFormat = SMS_FORMAT_GSM_WCDMA;

    /* copy SMSC to Message */
    if(args->smsc == NULL)
        iIndex = 1; /*if smsc is NULL, add 0 to the PDU*/
    else {
        iSMSCSize = (int)( hex2bin(args->smsc[0],  args->smsc[1] ));
        if(iSMSCSize > 0){
            sParam.Message[0] = iSMSCSize;
            iIndex ++ ;

            for(i = 1;   i < iSMSCSize + 1; i ++) {
                sParam.Message[iIndex] = hex2bin(args->smsc[i*2] , args->smsc[i*2 + 1] );
                iIndex ++;
            }
        }
        else 
            iIndex = 1; /*if smsc is NULL, add 0 to the PDU*/
    }

    if(args->pdu != NULL)
        iPDUSize = (int)strlen(args->pdu);
    if(iPDUSize== 0){
        LOGE("SMS PDU size is 0 \n");
        ret = RIL_E_GENERIC_FAILURE;
        goto error;
    }
        
    sParam.messageSize = iIndex + iPDUSize/2; 

    /* copy PDU to Message after SMSC */
    for(i=0; i<(int)strlen(args->pdu); i+=2) 
    {
        sParam.Message[iIndex] = hex2bin(args->pdu[i], args->pdu[i+1]);
        iIndex++;
    }

    /* call Sierra QMI interface */
    nRet = SaveSMS( sParam.storageType, 
                sParam.messageFormat,
                sParam.messageSize,
                &sParam.Message[0],
                &sParam.MessageIndex);
    if (nRet != eQCWWAN_ERR_NONE ){
        LOGE("SaveSMS failed nRet = %lu", nRet);
        ret = RIL_E_GENERIC_FAILURE;
        goto error;
    }

    LOGD("SaveSMS succeed, saved index is %lu \n", sParam.MessageIndex);
    switch(args->status){
        case RIL_SMS_STATUS_UNREAD:
            nStatus = QMI_SMS_STATUS_UNREAD; 
            break;
        case RIL_SMS_STATUS_READ:
            nStatus = QMI_SMS_STATUS_READ; 
            break;
        case RIL_SMS_STATUS_UNSENT:
            nStatus = QMI_SMS_STATUS_UNSENT; 
            break;
        case RIL_SMS_STATUS_SENT:
            nStatus = QMI_SMS_STATUS_SENT; 
            break;
        default:
            nStatus = QMI_SMS_STATUS_UNSENT; 
            break;
    }

    LOGD("Status in QMI is %lu \n", nStatus);
    nRet = SLQSModifySMSStatus(
                sParam.storageType, 
                sParam.MessageIndex,
                nStatus,
                &messageMode);
    if (nRet != eQCWWAN_ERR_NONE ){
        LOGE("ModifySMSStatus failed nRet = %lu", nRet);
        /* SWI_TBD, curretly when args->status is UNREAD/READ ModifySMSStatus always failed, 
        now just ignore the error         */
#if 0
        ret = RIL_E_GENERIC_FAILURE;
        goto error;
#endif        
    }
    else
        LOGD("ModifySMSStatus succeed. \n");

    /* Java layer record index starts from 1, QMI SMS index starts from 0, so increase 1 to match */
    sParam.MessageIndex ++;
    LOGD("SaveSMS reporting index to upper layer is %lu \n", sParam.MessageIndex);
    RIL_onRequestComplete(t, RIL_E_SUCCESS, (int *)&sParam.MessageIndex, sizeof(int *));
    return;

error:
    RIL_onRequestComplete(t, ret, NULL, 0);
}

/**
 *
 * Deletes a SMS message from SIM memory.
 *
 * @param[in] data 
 *     Pointer to the request data
 * @param datalen 
 *     request data length
 * @param t
 *     token passed to the RIL command request
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     RIL_REQUEST_DELETE_SMS_ON_SIM
 *
 */
void requestDeleteSmsOnSimUMTS(void *data, size_t datalen, RIL_Token t)
{
    ULONG nStorageType; 
    ULONG nMessageIndex = -1; 
    ULONG nRet = 0;
    BYTE  messageMode = SMS_MESSAGE_MODE_GSM_WCDMA; 
    
    /* initialize DeleteSMS parameter */
    nStorageType = SMS_TYPE_UIM; /* delete SMS on SIM*/
    /* Java layer record index starts from 1, QMI SMS index starts from 0 */
    nMessageIndex =  ((ULONG *) data)[0] - 1;

    /* call Sierra QMI interface */
    /* Message Tag was not used from up layer*/
    nRet = SLQSDeleteSMS(nStorageType, &nMessageIndex, NULL, &messageMode);
    if (nRet != eQCWWAN_ERR_NONE ){
        LOGE("DeleteSMS failed for index: %lu, nRet = %lu", nMessageIndex, nRet);
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    }
    else {
        LOGD("DeleteSMS succeed for SMS Index %lu \n", nMessageIndex);
        RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    }
}

/**
 * RIL_REQUEST_GET_SMSC_ADDRESS
 *
 * Gets the SMS center address
 * 
 */
void requestGetSMSCAddressUMTS(void *data, size_t datalen, RIL_Token t)
{
    ULONG nRet = 0;
    char  *response;
    STRUCT_SMSCAddress sParam;

    /* clear structure */
    memset(&sParam, 0, sizeof(STRUCT_SMSCAddress));
    /* initialize structure */
    sParam.addressSize = sizeof(sParam.SMSCAddress);
    sParam.typeSize = sizeof(sParam.SMSCType);

    /* call Sierra QMI interface */
    nRet = GetSMSCAddress( sParam.addressSize, 
                           sParam.SMSCAddress,
                           sParam.typeSize,
                           sParam.SMSCType);
    if (nRet != eQCWWAN_ERR_NONE ){
        LOGE("GetSMSCAddress failed nRet = %lu", nRet);
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    }
    else {
        LOGD("GetSMSCAddress succeed, SCAddress: %s, SMSCType:%s\n", sParam.SMSCAddress, sParam.SMSCType);
        /* up layer application only need SMSC address, so other returns from 
        GetSMSCAddresss were ignored when send response to up layer*/
        response = &sParam.SMSCAddress[0];
        RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(char *));
    }
}

/**
 * RIL_REQUEST_SET_SMSC_ADDRESS
 *
 * Sets the SMS center address
 * 
 */
void requestSetSMSCAddressUMTS(void *data, size_t datalen, RIL_Token t)
{
    ULONG nRet = 0;
    const char *smsc = ((const char *)data);
    STRUCT_SMSCAddress sParam;

    /* clear structure */
    memset(&sParam, 0, sizeof(STRUCT_SMSCAddress));
    /* initialize structure */
    LOGD("SetSMSCAddress SCAddress: %s\n", smsc);
    strncpy(&sParam.SMSCAddress[0], smsc, strlen(smsc));

    /* up layer application just provide SMSC address, and SMCSType is optional 
    TLV, so SMSCType was ignored to function SetSMSCAddress*/
    /* call Sierra QMI interface */
    nRet= SetSMSCAddress( sParam.SMSCAddress, sParam.SMSCType);
    if (nRet != eQCWWAN_ERR_NONE ){
        LOGE("SetSMSCAddress for address :%s type: %s failed nRet = %lu", 
            sParam.SMSCAddress, sParam.SMSCType, nRet);
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    }
    else {
        LOGD("SetSMSCAddress succeed, SMSCAddress: %s, SMSCType:%s\n", sParam.SMSCAddress, sParam.SMSCType);
        RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    }
}

/**
 * RIL_REQUEST_CDMA_SEND_SMS
 *
 * Sends an SMS message for immediate over the air transmission
 * 
 */
void requestSendSMSCDMA(void *data, size_t datalen, RIL_Token t)
{
    ULONG nRet = 0;
    RIL_Errno ret = RIL_E_SUCCESS;
    RIL_SMS_Response response; 
    STRUCT_SendSMS sParam;
    RIL_CDMA_SMS_Message *args = NULL;
    slqssendsmsparams_s messageParams;

    /* clear structure */
    memset(&sParam, 0, sizeof(STRUCT_SendSMS));
    memset(&response, 0, sizeof(RIL_SMS_Response));

    /* initialize structure */
    sParam.messageFormat = SMS_FORMAT_CDMA; 

    args = (RIL_CDMA_SMS_Message *)data;
    /* check the input parameter */
    if(args == NULL){
        LOGE("requestSendSMSCDMA: data is NULL but datalen =%d. \n", (int)datalen);
        ret = RIL_E_GENERIC_FAILURE;
        goto error;
    }
    if(args->sAddress.number_of_digits == 0 || args->uBearerDataLen == 0){
        LOGE("requestSendSMSCDMA: SMS lenth is 0 or destination is NULL .\n");
        ret = RIL_E_GENERIC_FAILURE;
        goto error;
    }
        
    /* Encoding CDMA SMS */
    nRet = CDMAEncodeMOTextMsg(
                        (BYTE *)sParam.Message,
                        (BYTE *)&sParam.messageSize,
                        args);
    if(nRet != eQCWWAN_ERR_NONE){
        LOGE("CDMAEncodeMOTextMsg error %lu \n", nRet);
        ret = RIL_E_GENERIC_FAILURE;
        goto error;
    }
    
    messageParams.messageFormat      = sParam.messageFormat;
    messageParams.messageSize        = sParam.messageSize;
    messageParams.pMessage           = sParam.Message;

    /* call Sierra QMI interface */
    nRet = SLQSSendSMS( &messageParams );

    if (nRet != eQCWWAN_ERR_NONE ){
        LOGE("Message sending response: %lu, nRet = %lu", messageParams.messageFailureCode, nRet);
        if(nRet == eQCWWAN_ERR_NO_CONNECTION)
            ret = RIL_E_RADIO_NOT_AVAILABLE;
        else{
            if(messageParams.messageFailureCode <= SMS_FAILURE_CAUSE_NETWORK_PROBLEM_END)
                ret = RIL_E_SMS_SEND_FAIL_RETRY;
            else if((messageParams.messageFailureCode >= SMS_FAILURE_CAUSE_RADIO_INTERFACE_PROBLEM_BEGIN)&&
            (messageParams.messageFailureCode <= SMS_FAILURE_CAUSE_RADIO_INTERFACE_PROBLEM_END))
                ret = RIL_E_RADIO_NOT_AVAILABLE;
            else
                ret = RIL_E_GENERIC_FAILURE;
        }
        goto error;
    }
    else {
        LOGD("Send SMS: Message Reference %d", messageParams.messageID);
        response.errorCode = -1;
        response.messageRef = messageParams.messageID;
        RIL_onRequestComplete(t, RIL_E_SUCCESS, &response, sizeof(RIL_SMS_Response));
    }
    return;

error:
        RIL_onRequestComplete(t, ret, NULL, 0);
    
}

/**
 * RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE
 *
 * Acknowledge successful or failed receipt of SMS previously indicated
  * via RIL_UNSOL_RESPONSE_CDMA_NEW_SMS
*/
void requestSMSAcknowledgeCDMA(void *data, size_t datalen, RIL_Token t)
{
    RIL_CDMA_SMS_Ack *args = NULL;
    struct held_message *message;

    /**
     * Protect the entire function - need to make sure that any new incoming
     * SMS (in callback thread) is not serviced before treating the current ACK
     */
    pthread_mutex_lock(&s_held_message_mutex);

    /* If an ACK is received previously, ignore subsequent ACKs */
    if (!s_outstanding_acknowledge) {
        pthread_mutex_unlock(&s_held_message_mutex);
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
        return;
    }

    /** 
     * Received a valid ACK, this variable would be updated to true by
     * NewSMSDispatch(), if there is an SMS to be dispatched.
     */  
    s_outstanding_acknowledge = false;

    args = (RIL_CDMA_SMS_Ack *)data;
    /** 
     * If the previous message was not successfully acked, try re-sending it
     * again later on reception of a New SMS.
     */  
    if(args){
        if (args->uErrorClass == RIL_CDMA_SMS_ERROR) {
            LOGE("requestSMSAcknowledgeCDMA: SMS Error CauseCode : %d",
                 args->uSMSCauseCode);
            goto end;
        }
    } else {
        LOGE("requestSMSAcknowledgeCDMA: data is NULL \n");
        goto end;
    }

    /**
     * The stored message from the beginning of the queue (for which the ACK
     * is received) is deleted from Device on successfull Ack
     */  
    delete_held_message();

    /* Get the first message in the queue to be sent to the application */  
    message = get_held_message();
    if (message != NULL) {
        LOGI("Outstanding requests in queue, dequeueing and sending.");
        NewSMSDispatch(message->pMsg);
    }

end:
    pthread_mutex_unlock(&s_held_message_mutex);
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
}

/**
 * RIL_REQUEST_CDMA_WRITE_SMS_TO_RUIM
 *
 * Stores a SMS message to RUIM memory.
 * 
 */
void requestWriteSMSToRuimCDMA(void *data, size_t datalen, RIL_Token t)
{
    ULONG nRet = 0;
    ULONG nStatus = 0;
    RIL_CDMA_SMS_WriteArgs *args = NULL;
    STRUCT_SaveSMS sParam;
    RIL_Errno ret = RIL_E_SUCCESS;
    BYTE  messageMode = SMS_MESSAGE_MODE_CDMA; 

    args = (RIL_CDMA_SMS_WriteArgs *)data;
    LOGD("SMS to be saved: status: %d, message length: %d", 
        args->status, args->message.uBearerDataLen);
    
    /* clear structure */
    memset(&sParam, 0, sizeof(STRUCT_SaveSMS));

    /* Since current CDMA images only support NV and not SIM, always do write on NV*/
    sParam.storageType = SMS_TYPE_NV;
    sParam.messageFormat = SMS_FORMAT_CDMA;

    /* Encoding CDMA SMS */
    nRet = CDMAEncodeMOTextMsg(
                        (BYTE *)sParam.Message,
                        (BYTE *)&sParam.messageSize,
                        &args->message);
    if(nRet != eQCWWAN_ERR_NONE){
        LOGE("CDMAEncodeMOTextMsg error %lu \n", nRet);
        goto error;
    }

    /* call Sierra QMI interface */
    nRet = SaveSMS(
                sParam.storageType, 
                sParam.messageFormat,
                sParam.messageSize,
                &sParam.Message[0],
                &sParam.MessageIndex);
    if (nRet != eQCWWAN_ERR_NONE ){
        LOGE("SaveSMS failed nRet = %lu", nRet);
        if(nRet == eQCWWAN_ERR_NO_CONNECTION)
            ret = RIL_E_RADIO_NOT_AVAILABLE;
        else
            ret = RIL_E_GENERIC_FAILURE;
        goto error;
    }

    LOGD("SaveSMS succeed, saved index is %lu \n", sParam.MessageIndex);
    switch(args->status){
        case RIL_SMS_STATUS_UNREAD:
            nStatus = QMI_SMS_STATUS_UNREAD; 
            break;
        case RIL_SMS_STATUS_READ:
            nStatus = QMI_SMS_STATUS_READ; 
            break;
        case RIL_SMS_STATUS_UNSENT:
            nStatus = QMI_SMS_STATUS_UNSENT; 
            break;
        case RIL_SMS_STATUS_SENT:
            nStatus = QMI_SMS_STATUS_SENT; 
            break;
        default:
            nStatus = QMI_SMS_STATUS_UNSENT; 
            break;
    }

    LOGD("Status in QMI is %lu \n", nStatus);
    nRet = SLQSModifySMSStatus(
                sParam.storageType, 
                sParam.MessageIndex,
                nStatus,
                &messageMode);
    if (nRet != eQCWWAN_ERR_NONE ){
        LOGE("ModifySMSStatus failed nRet = %lu", nRet);
        /* SWI_TBD, curretly when args->status is UNREAD/READ ModifySMSStatus always failed, 
        now just ignore the error         */
#if 0        
        if(nRet == eQCWWAN_ERR_NO_CONNECTION)
            ret = RIL_E_RADIO_NOT_AVAILABLE;
        else
            ret = RIL_E_GENERIC_FAILURE;
        goto error;
#endif
    }
    else
        LOGD("ModifySMSStatus succeed. \n");
    
    RIL_onRequestComplete(t, RIL_E_SUCCESS, (int *)&sParam.MessageIndex, sizeof(int *));
    return;

error:
    RIL_onRequestComplete(t, ret, NULL, 0);
}

/**
 * RIL_REQUEST_CDMA_DELETE_SMS_ON_RUIM
 *
 * Deletes a SMS message from RUIM memory.
 * 
 */
void requestDeleteSmsOnRuimCDMA(void *data, size_t datalen, RIL_Token t)
{
    ULONG nStorageType; 
    ULONG nMessageIndex = -1; 
    ULONG nRet = 0;
    BYTE  messageMode = SMS_MESSAGE_MODE_CDMA; 
    
    /* initialize DeleteSMS parameter */
    /* Since current CDMA images only support NV and not SIM, always do delete on NV*/
    nStorageType = SMS_TYPE_NV; 
    nMessageIndex =  ((ULONG *) data)[0];

    /* call Sierra QMI interface */
    /* Message Tag was not used from up layer*/
    nRet = SLQSDeleteSMS(nStorageType, &nMessageIndex, NULL, &messageMode);
    if (nRet != eQCWWAN_ERR_NONE ){
        LOGE("DeleteSMS failed for index: %lu, nRet = %lu", nMessageIndex, nRet);
        if(nRet == eQCWWAN_ERR_NO_CONNECTION)
            RIL_onRequestComplete(t, RIL_E_RADIO_NOT_AVAILABLE, NULL, 0);
        else
            RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    }
    else {
        LOGD("DeleteSMS succeed for SMS Index %lu \n", nMessageIndex);
        RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    }
}

/**
 * Encoding the tele service ID to PDU
 *
 * @param [in] uTeleserviceID 
 *     Tele service ID
 * @param [out] pPackedBuffer 
 *     PDU included encoded teleservce ID
 * @param [out] Len 
 *     PDU length
 *
 * @return
 *     See ::eQCWWANError for additional information
 */
enum eQCWWANError EncodeTeleServiceHdr(int uTeleserviceID, BYTE* pPackedBuffer,ULONG* Len)
{
    USHORT teleidvalue;
    BYTE   *pBuf;
    ULONG  bytesPacked = 0;
    BYTE   temp = 0;

    if( pPackedBuffer == NULL || Len == NULL )
        return eQCWWAN_ERR_INVALID_ARG;

    if( *Len < TELESERVICE_ID_DEFAULT_LEN )
        return eQCWWAN_ERR_BUFFER_SZ;

    if(uTeleserviceID == 0)
        teleidvalue = (USHORT)TELESERVICE_ID_DEFAULT_VALUE;
    else
        teleidvalue = (USHORT)uTeleserviceID;

    pBuf = pPackedBuffer;

    *pBuf++ = POINT_TO_POINT_MSG;
    bytesPacked++;
    *pBuf++ = TELESERVICE_IDENTIFIER;
    bytesPacked++;
    *pBuf++ = TELESERVICE_HDR_LEN;
    bytesPacked++;

    temp = teleidvalue >> 8;
    *pBuf++ = temp;
    bytesPacked++;

    temp = (BYTE)teleidvalue;
    *pBuf++ = temp;
    bytesPacked++;

    /* Update Len to indicate output buffer len; */
    *Len = bytesPacked;

    pBuf = pPackedBuffer;

    return eQCWWAN_ERR_NONE;
}

/**
 * Encoding the Bearer reply option header to PDU
 *
 * @param [out] pPackedBuffer 
 *     PDU included encoded bearer reply option
 * @param [out] Len 
 *     PDU length
 *
 * @return
 *     See ::eQCWWANError for additional information
 */
enum eQCWWANError EncodeBearerReplyOptionHdr(BYTE* pPackedBuffer,ULONG * Len)
{
    BYTE  *pBuf;
    ULONG bytesPacked = 0;

    if( pPackedBuffer == NULL || Len == NULL )
        return eQCWWAN_ERR_INVALID_ARG;

    if( *Len < BEAREA_REPLY_DEFAULT_LEN )
        return eQCWWAN_ERR_BUFFER_SZ;

    pBuf = pPackedBuffer;
    *pBuf++ = BEARER_REPLY_IDENTIFIER;
    bytesPacked++;
    *pBuf++ = BEARER_REPLY_HDR_LEN;
    bytesPacked++;

    *pBuf = 0; /*  this is just a arbitrary value, good enough for IOT lab testing purposes */
    bytesPacked++;

    /* Update Len to indicate output buffer len; */
    *Len = bytesPacked;
    pBuf = pPackedBuffer;

    LOGD("EncodeBearerReplyOptionHdr Output: bytespacked %lu \n", bytesPacked);

    return eQCWWAN_ERR_NONE;
}

/**
 *  pack a byte to  bits
 *
 * @param [in] src 
 *     source byte to be packed
 * @param [in] dst 
 *     desitation bytes packed
 * @param [in] pos 
 *     position of packed bit
 * @param [in] Len 
 *     length of the byte to be packed
 *
 * @return
 *     none
 */
void PackByteToBits(BYTE src, BYTE dst[], WORD pos, WORD len )
{
   WORD   t_pos = pos % 8;
   WORD   bits  = 8 - t_pos;

   dst += (pos+len-1)/8;

   if ( bits >= len )
   {
       *dst &= (BYTE) ~MASK_B(t_pos, len);
       *dst |= (BYTE) (MASK_B(t_pos, len) & (src << (bits - len)));
   }
   else /* len > bits */
   {
       dst--;
       *dst &= (BYTE) ~MASK_B(t_pos, bits);
       *dst |= (BYTE) (MASK_B(t_pos, bits) & (src >> (len - bits)));

       dst++;
       *dst &= (BYTE) ~MASK_B(0, (len - bits));
       *dst |= (BYTE) (MASK_B(0, (len - bits)) & (src << (8 - (len - bits))));
   }
}

/**
 * Encoding the destination address to PDU
 *
 * @param [out] pPackedBuffer 
 *     PDU included encoded destination address
 * @param [in] pszDestAddr 
 *     destiantion address string
 * @param [in] DestAddrLen 
 *     length of destination addrss string
 * @param [in/out] Len 
 *     PDU length
 * @param [in] ReserveBits 
 *     receive bits of the destination address
 *
 * @return
 *     See ::eQCWWANError for additional information
 */
enum eQCWWANError EncodeDestAddrHdr(BYTE* pPackedBuffer,CHAR* pszDestAddr, BYTE DestAddrLen, ULONG* Len, BYTE ReserveBits)
{
    BYTE  DestAddrId, HdrLen, NumAndDigitMode,NumsOfDigits,Reserved;
    WORD  bit_pos = 0;
    BYTE  *numbers = NULL;
    char  *pszNum;
    BYTE  *pBuf;
    ULONG bytesPacked = 0;
    int ii=0;
    BYTE  RetNum = 0;

    if( pPackedBuffer == NULL || Len == NULL || pszDestAddr == NULL /*|| DestAddrLen != 10*/ ){
        LOGE("EncodeDestAddrHdr invalid parameter found. \n ");
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /*if( *Len < 9 )
        return eQCWWAN_ERR_BUFFER_SZ;*/

    NumsOfDigits = DestAddrLen;
    pszNum = pszDestAddr;
    numbers = (BYTE*)malloc(NumsOfDigits);
    memset(numbers, 0, NumsOfDigits);
    memcpy(numbers, pszDestAddr, NumsOfDigits);
    
    DestAddrId = DESTADDR_IDENTIFIER;                   /* For MO-SMS the id = 4; */
    PackByteToBits( DestAddrId,pPackedBuffer, bit_pos,8 );
    bit_pos += 8;
    bytesPacked++;

    HdrLen = (BYTE)*Len - 2;                           /* *Len is total memory allocated for Destination Addr buffer */
    PackByteToBits( HdrLen,pPackedBuffer, bit_pos,8 ); /* ParamId and ParamLen is 1 byte each in that buffer. Its not included  */
    bit_pos += 8;                                      /* HdrLen field. hence HdrLen = *Len - 2 */
    bytesPacked++;

    NumAndDigitMode = 0;                                         /* Num Mode and Digit Mode are 1 bit in size and */
    PackByteToBits( NumAndDigitMode,pPackedBuffer, bit_pos,2 );  /* set to zero for regular cell phone numbers. */
    bit_pos += 2;                                                /* hence copy 2 bits. */

    PackByteToBits( NumsOfDigits,pPackedBuffer, bit_pos,8 );     
    bit_pos += 8;

    for(ii = 0; ii < NumsOfDigits ; ii++ )             /* Pack each digit in 4 bit DTMF format. */
    {
        PackByteToBits( *(numbers+ii),pPackedBuffer, bit_pos,4 );
        bit_pos += 4;
    }

    if( ReserveBits )                                  /* If any reserved bits pack zero padding for those many bits. */
    {
        Reserved = 0;                                                
        PackByteToBits( Reserved,pPackedBuffer, bit_pos,ReserveBits ); 
        bit_pos += ReserveBits;
    }

    bytesPacked = (bit_pos/8);       
    /* Update Len to indicate output buffer len; */
    *Len = bytesPacked;
    pBuf = pPackedBuffer;

    if( numbers )
        free(numbers);

    return eQCWWAN_ERR_NONE;
}

/**
 * Encoding the message data to PDU
 *
 * @param [out] pPackedBuffer 
 *     PDU included encoded message data
 * @param [in] pszSmsMsg 
 *     source message data
 * @param [in] MsgLen 
 *     length of message data
 * @param [in/out] Len 
 *     PDU length
 *
 * @return
 *     See ::eQCWWANError for additional information
 *
 * @note
 *     Android application already encoded the user data, just copy it to out buffer
 */
enum eQCWWANError EncodeUserData(BYTE* pPackedBuffer,CHAR* pszSmsMsg, int MsgLen, ULONG * Len)
{
    if( pPackedBuffer == NULL || Len == NULL || pszSmsMsg == NULL || MsgLen > MAX_SMS_MSG_LEN ) {
        LOGE("EncodeUserData invalid parameter found. \n ");
        return eQCWWAN_ERR_INVALID_ARG;
    }
    
    if( *Len < (ULONG) MsgLen ){
        LOGE("EncodeUserData msg buffer too small. \n");
        return eQCWWAN_ERR_BUFFER_SZ;
    }
    memcpy(pPackedBuffer, pszSmsMsg, MsgLen);
    *Len = MsgLen;

    return eQCWWAN_ERR_NONE;
}

/**
 * Encoding the bearer data header to PDU
 *
 * @param [out] pPackedBuffer 
 *     PDU included bearer data header
 * @param [in] DataLen 
 *     length of message data
 * @param [in/out] Len 
 *     PDU length
 *
 * @return
 *     See ::eQCWWANError for additional information
 */
enum eQCWWANError EncodeBearerData(BYTE* pPackedBuffer,BYTE DataLen, ULONG* Len)
{
    BYTE  *pBuf;
    ULONG bytesPacked = 0;

    if( pPackedBuffer == NULL || Len == NULL  ){
        LOGE("EncodeBearerData invalid parameter found. \n ");
        return eQCWWAN_ERR_INVALID_ARG;
    }

    if( *Len < BEARER_DATA_DEFAULT_LEN ){
        LOGE("EncodeBearerData msg buffer too small. \n");
        return eQCWWAN_ERR_BUFFER_SZ;
    }
    
    pBuf = pPackedBuffer;
    *pBuf++ = BEARER_DATA_IDENTIFIER;
    bytesPacked++;

    *pBuf++ = DataLen;
    bytesPacked++;

    /* Update Len to indicate output buffer len; */
    *Len = bytesPacked;
    pBuf = pPackedBuffer;

    return eQCWWAN_ERR_NONE;

}

/**
 * Encoding relative validity header to PDU
 *
 * @param [out] pPackedBuffer 
 *     PDU included bearer data header
 * @param [in] relValue 
 *     relative validity value
 * @param [in/out] Len 
 *     PDU length
 *
 * @return
 *     See ::eQCWWANError for additional information
 */
enum eQCWWANError EncodeRelValidityHdr(BYTE* pPackedBuffer,BYTE* relValue, ULONG* Len)  
{
    BYTE  *pBuf;
    ULONG bytesPacked = 0;
    BYTE  RelValidity = 0;

    if( pPackedBuffer == NULL || Len == NULL ){
        LOGE("EncodeRelValidityHdr invalid parameter found. \n ");
        return eQCWWAN_ERR_INVALID_ARG;
    }

    if( *Len < REL_VAL_PERIOD_DEFAULT_LEN ){
        LOGE("EncodeRelValidityHdr msg buffer too small. \n");
        return eQCWWAN_ERR_BUFFER_SZ;
    }
   
    pBuf = pPackedBuffer;
    *pBuf++ = REL_VAL_PERIOD_SUB_IDENTIFIER;
    bytesPacked++;
    *pBuf++ = REL_VAL_PERIOD_SUB_IDENTIFIER_HDR_LEN;
    bytesPacked++;

   if(relValue)
      RelValidity = *relValue;

    switch(RelValidity)
    {
    case 0:
        *pBuf = REL_VALIDITY_0;     /*  decimal value as per 4.5.6-1 of spec */
        break;
    case 1:
        *pBuf = REL_VALIDITY_1;     /*  decimal value as per 4.5.6-1 of spec */
        break;
    case 2:
        *pBuf = REL_VALIDITY_2;    /*  decimal value as per 4.5.6-1 of spec */
        break;
    case 3:
        *pBuf = REL_VALIDITY_3;    /*  decimal value as per 4.5.6-1 of spec */
        break;
    case 4:
        *pBuf = REL_VALIDITY_4;    /*  decimal value as per 4.5.6-1 of spec */
        break;
    }
    bytesPacked++;

    /* Update Len to indicate output buffer len; */
    *Len = bytesPacked;
    pBuf = pPackedBuffer;

    return eQCWWAN_ERR_NONE;
}

/**
 * Encoding priority header to PDU
 *
 * @param [out] pPackedBuffer 
 *     PDU included bearer data header
 * @param [in] Priority 
 *     priority of the SMS message
 * @param [in/out] Len 
 *     PDU length
 *
 * @return
 *     See ::eQCWWANError for additional information
 */
enum eQCWWANError EncodePriorityHdr(BYTE* pPackedBuffer,BYTE Priority, ULONG* Len)
{
    BYTE  *pBuf;
    ULONG bytesPacked = 0;

    if( pPackedBuffer == NULL || Len == NULL ){
        LOGE("EncodePriorityHdr invalid parameter found. \n ");
        return eQCWWAN_ERR_INVALID_ARG;
    }


    if( *Len < 3 ){
        LOGE("EncodePriorityHdr msg buffer too small. \n");
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    if( Priority > 4 ){
        LOGE("EncodePriorityHdr unsupported Priority %d \n", (int )Priority);
        return eQCWWAN_ERR_INVALID_ARG;
    }

    pBuf = pPackedBuffer;
    *pBuf++ = PRIORIY_SUB_IDENTIFIER;
    bytesPacked++;
    *pBuf++ = PRIORIY_SUB_IDENTIFIER_HDR_LEN;
    bytesPacked++;

    switch(Priority)
    {
    case 0:
        *pBuf = BINARY_FORMAT_NONE;
        break;
    case 1:
        *pBuf = BINARY_FORMAT_01_000000;     /* Binary format 01-000000 */
        break;
    case 2:
        *pBuf = BINARY_FORMAT_10_000000;    /* Binary format 10-000000 */
        break;
    case 3:
        *pBuf = BINARY_FORMAT_11_000000;    /* Binary format 11-000000 */
        break;
    }
    bytesPacked++;

    /* Update Len to indicate output buffer len; */
    *Len = bytesPacked;
    pBuf = pPackedBuffer;

    return eQCWWAN_ERR_NONE;
}

/**
 * Encoding function for Mobile orient message
 *
 * @param [out] pOut 
 *     pointer of the encoded PDU
 * @param [out] pOutLen 
 *     length of the PDU
 * @param [in] args 
 *     pointer of the structure to be encoding
 *
 * @return
 *     See ::eQCWWANError for additional information
 */
ULONG CDMAEncodeMOTextMsg(
    BYTE   *pOut, 
    BYTE   *pOutLen, 
    RIL_CDMA_SMS_Message *args)
{
    ULONG  TeleserviceLen = 0, DestAddrLen = 0,CallbackAddrLen = 0, BearerDataLen = 0;
    ULONG  MsgIDLen = 0, PriorityLen = 0, UserDataLen = 0,  RelValidityLen = 0,  BearerReplyOptionLen=0,  TotalLen = 0; 
    BYTE   BearerMsgLen = 0;
    BYTE   Priority = 0;
    BYTE   *TeleServiceBuf = NULL;
    BYTE   *BearerReplyOptionBuf = NULL;
    BYTE   *DestAddrBuf = NULL;
    BYTE   *BearerDataBuf = NULL;
    BYTE   *UserDataBuf = NULL;
    BYTE   *RelValidityBuf = NULL;
    BYTE   *PriorityBuf = NULL;
    USHORT bits = 0, remainder = 0;
    BYTE   reserved = 0;
    BYTE   *pbTemp;
    ULONG rCode = eQCWWAN_ERR_GENERAL;

    if( args->sAddress.digits == NULL || args->aBearerData== NULL /*|| DestNumLen > 10*/ 
    || args->uBearerDataLen > MAX_SMS_MSG_LEN  || pOut == NULL){
        LOGE("CDMAEncodeMOTextMsg invalid parameter found. \n ");
        return eQCWWAN_ERR_INVALID_ARG;
    }
    
    if(args->sAddress.digit_mode != RIL_CDMA_SMS_DIGIT_MODE_4_BIT){
        LOGE("CDMAEncodeMOTextMsg unsupported CDMA SMS digit mode. \n ");
        return eQCWWAN_ERR_INVALID_ARG;
    }
    
    /* Teleservice Id  default LEN = 5 */
    TeleserviceLen = TELESERVICE_ID_DEFAULT_LEN;
    TeleServiceBuf = (BYTE*)malloc(TeleserviceLen);
    if( !TeleServiceBuf )
    {
        LOGE("Failed to allocate memory. \n");
        rCode = eQCWWAN_ERR_GENERAL;
        goto exit;
    }
    memset(TeleServiceBuf,0,TeleserviceLen);
    rCode = EncodeTeleServiceHdr(args->uTeleserviceID, TeleServiceBuf, &TeleserviceLen );
    if( rCode != eQCWWAN_ERR_NONE )
    {
        LOGE("EncodeTeleServiceHdr failed. \n");
        goto exit;
    }

    /* Bearer Reply Option default LEN = 3 */
    BearerReplyOptionLen = BEAREA_REPLY_DEFAULT_LEN;
    BearerReplyOptionBuf = (BYTE*)malloc(BearerReplyOptionLen);
    if( !BearerReplyOptionBuf )
    {
        LOGE("Failed to allocate memory. \n");
        rCode = eQCWWAN_ERR_GENERAL;
        goto exit;
    }
    memset(BearerReplyOptionBuf,0,BearerReplyOptionLen);
    rCode = EncodeBearerReplyOptionHdr(BearerReplyOptionBuf, &BearerReplyOptionLen );
    if( rCode != eQCWWAN_ERR_NONE )
    {
        LOGE("EncodeBearerReplyOptionHdr failed. \n");
        goto exit;
    }

    /* Calculate Destination Addr Len */
    bits = 8+8+1+1+8+(4*args->sAddress.number_of_digits);  
    /* ParamID(8bits)+ParamLen(8bits)+NumMode(1bit)+DigitMode(1bit)+NumofDigits(8bits)
        +(NumOfDigits*DTMF format len(4 bits) + reserve bits(if needed)*/
    remainder = bits%8;
    if( remainder )
        reserved =  8 - remainder;
    bits = bits + reserved;

    DestAddrLen = bits/8;
    DestAddrBuf = (BYTE*)malloc(DestAddrLen);
    if( !DestAddrBuf )
    {
        LOGE("Failed to allocate memory. \n");
        rCode = eQCWWAN_ERR_GENERAL;
        goto exit;
    }
    LOGD("Len of Destination Header is %lu",DestAddrLen);
    memset(DestAddrBuf,0,DestAddrLen);
    rCode = EncodeDestAddrHdr(
                        DestAddrBuf,
                        (CHAR *)args->sAddress.digits,
                        args->sAddress.number_of_digits,
                        &DestAddrLen,
                        reserved);
    if( rCode != eQCWWAN_ERR_NONE )
    {
        LOGE("EncodeDestAddrHdr failed. \n");
        goto exit;
    }

    /* Calculate Callback Addr Len */
    LOGD("Length of callback number is zero .\n");

    /* UserData default LEN = 200 */
    UserDataLen = USER_DATA_DEFAULT_LEN;
    UserDataBuf = (BYTE*)malloc(UserDataLen);
    if( !UserDataBuf )
    {
        LOGE("Failed to allocate memory. \n");
        rCode = eQCWWAN_ERR_GENERAL;
        goto exit;
    }
    memset(UserDataBuf,0,UserDataLen);
    rCode = EncodeUserData(UserDataBuf, 
                        (CHAR *)args->aBearerData,
                        args->uBearerDataLen, 
                        &UserDataLen);
    if( rCode != eQCWWAN_ERR_NONE )
    {
        LOGE("EncodeUserData failed .\n");
        goto exit;
    }
    /* Priority default LEN = 3 */
    PriorityLen = PRIORIY_DEFAULT_LEN;
    PriorityBuf = (BYTE*)malloc(PriorityLen);
    if( !PriorityBuf )
    {
        LOGE("Failed to allocate memory. \n");
        rCode = eQCWWAN_ERR_GENERAL;
        goto exit;
    }
    memset(PriorityBuf,0,PriorityLen);
    rCode = EncodePriorityHdr(PriorityBuf,Priority,&PriorityLen);  /*  pass in priority for second param as it may be normal or urgent */
    if( rCode != eQCWWAN_ERR_NONE )
    {
        LOGE("EncodePrioriyHdr failed. \n");
        goto exit;
    }

    /* Relative validity default LEN = 3 */
    RelValidityLen = REL_VAL_PERIOD_DEFAULT_LEN;
    RelValidityBuf = (BYTE*)malloc(RelValidityLen);
    if( !RelValidityBuf )
    {
        LOGE("Failed to allocate memory. \n");
        rCode = eQCWWAN_ERR_GENERAL;
        goto exit;
    }
    memset(RelValidityBuf,0,RelValidityLen);
    rCode = EncodeRelValidityHdr(RelValidityBuf, NULL, &RelValidityLen);  
    if( rCode != eQCWWAN_ERR_NONE )
    {
        LOGE("EncodeRelValidityHdr failed. \n");
        goto exit;
    }

    /* BearerData default LEN = 2 */
    BearerDataLen =  BEARER_DATA_DEFAULT_LEN;
    BearerDataBuf = (BYTE*)malloc(BearerDataLen);
    if( !BearerDataBuf )
    {
        LOGE("Failed to allocate memory. \n");
        rCode = eQCWWAN_ERR_GENERAL;
        goto exit;
    }
    memset(BearerDataBuf,0,BearerDataLen);
    BearerMsgLen = (BYTE)(MsgIDLen + UserDataLen + RelValidityLen + PriorityLen +/* ReplyOptionLen + */ CallbackAddrLen);
    rCode = EncodeBearerData(BearerDataBuf,BearerMsgLen,&BearerDataLen);
    if( rCode != eQCWWAN_ERR_NONE )
    {
        LOGE("EncodeBearerData failed. \n");
        goto exit;
    }

    TotalLen = TeleserviceLen + DestAddrLen + BearerReplyOptionLen 
               + MsgIDLen + UserDataLen + RelValidityLen 
               + PriorityLen + BearerDataLen + /*ReplyOptionLen + */ CallbackAddrLen;

    memset(pOut,0,TotalLen);

    /* Copy layer by layer */
    pbTemp = pOut;

    memcpy(pbTemp, TeleServiceBuf, TeleserviceLen );
    pbTemp += TeleserviceLen;

    memcpy(pbTemp, BearerReplyOptionBuf, BearerReplyOptionLen );
    pbTemp += BearerReplyOptionLen;

    memcpy(pbTemp, DestAddrBuf, DestAddrLen );
    pbTemp += DestAddrLen;

    memcpy(pbTemp, BearerDataBuf, BearerDataLen );
    pbTemp += BearerDataLen;

#if 0
    memcpy(pbTemp, MsgIDBuf, MsgIDLen );
    pbTemp += MsgIDLen;
    free(MsgIDBuf);
#endif

    memcpy(pbTemp, UserDataBuf, UserDataLen );
    pbTemp += UserDataLen;

    memcpy(pbTemp, RelValidityBuf, RelValidityLen );
    pbTemp += RelValidityLen;

    memcpy(pbTemp, PriorityBuf, PriorityLen );
    pbTemp += PriorityLen;

    *pOutLen = TotalLen;

    pbTemp = pOut;

exit:
    if(TeleServiceBuf)
        free(TeleServiceBuf);
    if(BearerReplyOptionBuf)
        free(BearerReplyOptionBuf);
    if(DestAddrBuf)
        free(DestAddrBuf);
    if(BearerDataBuf)
        free(BearerDataBuf);
    if(UserDataBuf)
        free(UserDataBuf);
    if(RelValidityBuf)
        free(RelValidityBuf);
    if(PriorityBuf)
        free(PriorityBuf);

    return rCode;
}

/**
 * Split the received PDU to diffent sections according to the header structure
 *
 * @param [in] pMTSms 
 *     pointer to the received PDU 
 * @param [in] MsgLen 
 *     length of received PDU
 * @param [out] TeleServiceIdValue 
 *     tel service  ID section from PDU
 * @param [out] ServiceCatagory 
 *     service catagory from the PDU
 * @param [out] pOrigAddrBuf 
 *     origin address section from PDU 
 * @param [out] pDestAddrBuf 
 *     destination address section from PDU 
 * @param [out] BearerReplySeq 
 *     bearer reply section from PDU 
 * @param [out] pBearerDataBuf 
 *     bearer data section from PDU 
 * @param [out] BearerDataLen 
 *     length of the bearer data
 *
 * @return
 *     See ::eQCWWANError for additional information
 */
enum eQCWWANError SplitParamMTSms(
    BYTE   *pMTSms, 
    ULONG  MsgLen, 
    USHORT *TeleServiceIdValue, 
    USHORT *ServiceCatagory, 
    BYTE   **pOrigAddrBuf, 
    BYTE   **pDestAddrBuf, 
    BYTE   *BearerReplySeq, 
    BYTE   **pBearerDataBuf, 
    BYTE   *BearerDataLen )
{
    BYTE   *pMsg = pMTSms;
    BYTE   *pEndMsg = pMsg + MsgLen;
    BYTE   OriginAddrLen;
    BYTE   DestAddrLen;
    USHORT SkipLen = 0;
    USHORT SkipIdentifier = 0;
    enum eQCWWANError rc = eQCWWAN_ERR_NONE;

    *pDestAddrBuf = NULL;
    *pOrigAddrBuf = NULL;
    *pBearerDataBuf = NULL;

    if( *pMsg != POINT_TO_POINT_MSG )
    {
        LOGE("Msg TYPE is NOT POINT_TO_POINT_MSG .\n");
        return eQCWWAN_ERR_QMI_NOT_SUPPORTED;
    }
    
    pMsg++;

    while ( pMsg < pEndMsg )
    {
        switch(*pMsg)
        {
        case TELESERVICE_IDENTIFIER:
            if( *(++pMsg) != TELESERVICE_HDR_LEN )
            {
                LOGE("TELESERVICE IDENTIFIER LEN is not = %d" , TELESERVICE_HDR_LEN );
                rc = eQCWWAN_ERR_GENERAL;
            }
            *TeleServiceIdValue = *(++pMsg); 
            *TeleServiceIdValue <<= 8;
            *TeleServiceIdValue += *(++pMsg);
            break;

        case BEARER_REPLY_IDENTIFIER:
            if( *(++pMsg) != BEARER_REPLY_HDR_LEN )
            {
                LOGE("BEARER_REPLY_HDR_LEN is not = %d", BEARER_REPLY_HDR_LEN );
                rc = eQCWWAN_ERR_GENERAL;
            }
            *BearerReplySeq = *(++pMsg);
            break;

        case ORIGADDR_IDENTIFIER:
            OriginAddrLen = *(++pMsg);
            if( OriginAddrLen != ORIGADDR_HDR_LEN )
            {
                LOGE("ORIGADDR_HDR_LEN is not = %d", ORIGADDR_HDR_LEN );
            }
            *pOrigAddrBuf = (BYTE *)malloc(OriginAddrLen);
            if(*pOrigAddrBuf == NULL )
            {
                LOGE("Failed to allocate memeory .\n");
                rc = eQCWWAN_ERR_GENERAL;
            }
            //increment ptr to point to data
            ++pMsg;
            memcpy((void*)*pOrigAddrBuf,(void*)pMsg,OriginAddrLen);
            pMsg = pMsg + (OriginAddrLen - 1);           
            break;

        case DESTADDR_IDENTIFIER:
            DestAddrLen = *(++pMsg);
            if( DestAddrLen != DESTADDR_HDR_LEN )
            {
                LOGE("DESTADDR_HDR_LEN is not = %d", DESTADDR_HDR_LEN );
            }
            *pDestAddrBuf = (BYTE *)malloc(DestAddrLen);
            if(*pDestAddrBuf == NULL )
            {
                LOGE("Failed to allocate memeory" );
                rc = eQCWWAN_ERR_GENERAL;
            }
            //increment ptr to point to data
            ++pMsg;
            memcpy((void*)*pDestAddrBuf,(void*)pMsg,DestAddrLen);
            pMsg = pMsg + (DestAddrLen - 1);           
            break;

        case BEARER_DATA_IDENTIFIER:
            *BearerDataLen = *(++pMsg);
            *pBearerDataBuf = (BYTE *)malloc(*BearerDataLen);
            if(*pBearerDataBuf == NULL )
            {
                LOGE("Failed to allocate memeory" );
                rc = eQCWWAN_ERR_GENERAL;
            }
            //increment ptr to point to data
            ++pMsg;
            memcpy((void*)*pBearerDataBuf,(void*)pMsg,(*BearerDataLen));
            pMsg = pMsg + ((*BearerDataLen) - 1);         
            break;

        case SERVICE_CATEGORY_IDENIFIER:
            if( *(++pMsg) != SERVICE_CATEGORY_HDR_LEN )
            {
                LOGE("SERVICE CATAGORY IDENTIFIER LEN is not = %d" , SERVICE_CATEGORY_HDR_LEN );
                rc = eQCWWAN_ERR_GENERAL;
            }
            *ServiceCatagory = *(++pMsg); 
            *ServiceCatagory <<= 8;
            *ServiceCatagory += *(++pMsg);
            break;
           
        case ORIGSUBADDR_IDENTIFIER:
            SkipIdentifier = *(pMsg);
            SkipLen = *(++pMsg);
            LOGD("Unhandled Identifier = %d of Len = %d .... Skipping it",SkipIdentifier, SkipLen );
            pMsg = pMsg + SkipLen;
            break;

        default:
            LOGE("Unknown IDENTIFIER in MT SMS data = %d",*pMsg);
            rc = eQCWWAN_ERR_GENERAL;
            break;
        }
        if( rc != eQCWWAN_ERR_NONE )
            break;
        else
            pMsg++;
    }

    return rc;
}

/**
 *  Unpack bits To A Byte
 *
 * @param [in] src 
 *     data to be unpacked
 * @param [in] pos 
 *     position of unpacked bit
 * @param [in] Len 
 *     length of the bits to be unpacked
 *
 * @return
 *     result of the unpacked byte 
 */
BYTE UnpackBitsToAByte( BYTE *src,  int pos, int len)
{
   BYTE result = 0;
   int rshift = 0;

   src += pos/8;
   pos %= 8;
   
   rshift = MAX( 8 - (pos + len), 0);

   if ( rshift > 0 ) {

     result = (BYTE)MASK_AND_SHIFT(len, pos, rshift, *src);
  
   } else {

     result = (BYTE)MASK(8-pos, pos, *src);
     src++;
     len -= 8 - pos;

      if ( len > 0 ) result = ( result<<len ) | (*src >> (8-len));  /* if any bits left */
   }

   return result;
}

/**
 * Decoding the origin destination address from PDU
 *
 * @param [in] pOrigAddrBuf 
 *     message  include origin desitination addrss infor
 * @param [out] pDigitMode 
 *     digital mode of the message
 * @param [out] pNumMode 
 *     number of mode in the message
 * @param [out] szOriginAddr 
 *     origin address the message
 * @param [out] Len 
 *     length of the origin address
 *
 * @return
 *     See ::eQCWWANError for additional information
 */
enum eQCWWANError DecodeOriginAddr(
    BYTE *pOrigAddrBuf,
    BYTE *pDigitMode, 
    BYTE *pNumMode, 
    BYTE *szOriginAddr, 
    BYTE *AddrLen )
{
    int  bit_pos = 0;
    BYTE rcvdAddrLen;
    int  i = 0;

    *pDigitMode = UnpackBitsToAByte( pOrigAddrBuf,  bit_pos, 1);
    bit_pos += 1;

    *pNumMode = UnpackBitsToAByte( pOrigAddrBuf,  bit_pos, 1);
    bit_pos += 1;

    if( *pDigitMode != 0 || *pNumMode != 0 )
    {
        LOGE("Number mode or Digit mode is not zero, Such address encoding is not supported .\n");
        return eQCWWAN_ERR_QMI_NOT_SUPPORTED;
    }

    rcvdAddrLen = UnpackBitsToAByte( pOrigAddrBuf,  bit_pos, 8);
    bit_pos += 8;

    *AddrLen = rcvdAddrLen;  /* Assign the length */

    for(i = 0; i < *AddrLen; i++ )
    {
        szOriginAddr[i] = UnpackBitsToAByte( pOrigAddrBuf,  bit_pos, 4);
        /* In DTMF 0 is represented as 10. And up layer function convertDtmfToAscii will convert 10 to 0. */
        bit_pos += 4;
    }

    return eQCWWAN_ERR_NONE;
}

/**
 * Decoding the bearer data from PDU
 *
 * @param [in] pBearerDataBuf 
 *     message  include bearer data
 * @param [in] BearerDataLen 
 *     length of the message
 * @param [out] pOutBearerData 
 *     pointer of bearer data
 * @param [out] Len 
 *     length of the bearer data
 *
 * @return
 *     See ::eQCWWANError for additional information
 * @note
 *     Android application will decode bearer data, just copy it to out buffer
 */
enum eQCWWANError DecodeBearerData(
    BYTE *pBearerDataBuf, 
    BYTE BearerDataLen, 
    BYTE *pOutBearerData,
    int *iOutDataLen)
{
    enum eQCWWANError rc = eQCWWAN_ERR_NONE;
    /* no need to decode the bearer data, just copy to RIL_CDMA_SMS_Message, up layer will do the decoding. */
    
    *iOutDataLen = (int)BearerDataLen;
    memcpy(pOutBearerData, pBearerDataBuf, BearerDataLen);
    return rc;    
}

/**
 * Decoding function for Mobile trasfered message
 *
 * @param [in] pRawMsg 
 *     pointer of the message to be decoded
 * @param [in] pRawMsgLen 
 *     length of the message to be decoded
 * @param [out] args 
 *     pointer of the structure which has the messged decoded
 *
 * @return
 *     See ::eQCWWANError for additional information
 */
ULONG CDMADecodeMTTextMsg(
    BYTE   *pRawMsg, 
    ULONG   pRawMsgLen, 
    RIL_CDMA_SMS_Message *args)
{
    USHORT TeleServiceIdValue = 0;
    USHORT ServiceCatagory = 0;
    BYTE   *pDestAddrBuf = NULL;
    BYTE   *pOriginAddrBuf = NULL;
    BYTE   OriginAddr[RIL_CDMA_SMS_ADDRESS_MAX] = "";
    int    i = 0;
    BYTE   BearerReplySequence = 255;  /* Bearer Reply field is 6 bits so it cannot be more than 63 */
    BYTE   *pBearerDataBuf = NULL;
    BYTE   BearerDataLen;
    BYTE   DigitMode, NumMode;

    enum eQCWWANError rc = eQCWWAN_ERR_GENERAL;

    rc = SplitParamMTSms(pRawMsg, 
                         pRawMsgLen, 
                         &TeleServiceIdValue, 
                         &ServiceCatagory,
                         &pOriginAddrBuf, 
                         &pDestAddrBuf,
                         &BearerReplySequence, 
                         &pBearerDataBuf, 
                         &BearerDataLen );
    if( rc != eQCWWAN_ERR_NONE )
    {
        LOGE("SplitParamMTSms failed .\n");
        if(pBearerDataBuf)
            free(pBearerDataBuf);
        if(pOriginAddrBuf)
            free(pOriginAddrBuf);
        if(pDestAddrBuf)
            free(pDestAddrBuf);

        return rc;
    }

    LOGD("SplitParamMTSms Decoded: TeleServiceId = %d, BearerReply Seq = %d, \
        Orig Addr PTR = %p, Bearer DATA PTR = %p, Bearer DATA LEN = %d \n",
        TeleServiceIdValue, BearerReplySequence,
        pOriginAddrBuf, pBearerDataBuf, (int)BearerDataLen ); 

    if(TeleServiceIdValue != 0)
        args->uTeleserviceID = TeleServiceIdValue;

    if (ServiceCatagory != 0)
        args->uServicecategory = ServiceCatagory;

    /* Only proceed if PTR is NOT NULL */
    if(pOriginAddrBuf)
    {
        rc = DecodeOriginAddr(pOriginAddrBuf,
                              &DigitMode, 
                              &NumMode, 
                              (BYTE *)args->sAddress.digits, 
                              &(args->sAddress.number_of_digits) );
        if( rc != eQCWWAN_ERR_NONE )
        {
            LOGE("DecodeOriginAddr failed.  .\n");
            if(pBearerDataBuf)
                free(pBearerDataBuf);
            if(pOriginAddrBuf)
                free(pOriginAddrBuf);
            return rc;
        }
        args->sAddress.digit_mode = DigitMode;

        for(i = 0; i < (int)args->sAddress.number_of_digits; i++){
            if( args->sAddress.digits[i] == 10 )
                OriginAddr[i] = '0';
            else
                OriginAddr[i] = args->sAddress.digits[i] + '0';
        }

        LOGD("DecodeOriginAddr Decoded: DigitMode =%d,NumMode=%d,AddrLen=%d,Number=%s",
             args->sAddress.digit_mode, NumMode, 
             args->sAddress.number_of_digits, 
             OriginAddr);

        if(pOriginAddrBuf != NULL )
            free(pOriginAddrBuf);
    }
    else if(pDestAddrBuf)  /* if we want to retrieve a 'Draft' msg, */
    {
        rc = DecodeOriginAddr(pDestAddrBuf,
                              &DigitMode, 
                              &NumMode,
                              (BYTE *)args->sAddress.digits, 
                              &(args->sAddress.number_of_digits) );
        if( rc != eQCWWAN_ERR_NONE )
        {
            LOGE("DecodeDestAddr failed .\n");
            if(pBearerDataBuf)
                free(pBearerDataBuf);
            if(pDestAddrBuf)
                free(pDestAddrBuf);
            return rc;
        }
        for(i = 0; i < (int)args->sAddress.number_of_digits; i++){
            if( args->sAddress.digits[i] == 10 )
                OriginAddr[i] = '0';
            else
                OriginAddr[i] = args->sAddress.digits[i] + '0';
        }
        LOGD("DecodeOriginAddr Decoded: DigitMode =%d,NumMode=%d,AddrLen=%d,Number=%s",
             args->sAddress.digit_mode, NumMode, 
             args->sAddress.number_of_digits, 
             OriginAddr);

        if(pDestAddrBuf != NULL )
            free(pDestAddrBuf);
    }
    else
    {
        LOGE("Originating Address not found .\n");
        /* Based on this return code, SwiRetrieveCdmaSMSMessage deletes the message */
        return eQCWWAN_ERR_QMI_RSP; 
    }

     /* Only proceed if PTR is NOT NULL */
    if(pBearerDataBuf )
    {
        rc = DecodeBearerData(pBearerDataBuf, 
                              BearerDataLen,  
                              (BYTE *)&args->aBearerData,
                              &args->uBearerDataLen);
        if( rc != eQCWWAN_ERR_NONE )
        {
            LOGE("DecodeBearerData failed .\n");

            if(pBearerDataBuf)
                free(pBearerDataBuf);
            if(pOriginAddrBuf)
                free(pOriginAddrBuf);
            return rc;
        }
        if(pBearerDataBuf != NULL )
            free(pBearerDataBuf);
    }
    else
        LOGE("Bearer Data Not found .\n");

    return eQCWWAN_ERR_NONE;
}


/**
 * Returns sms specific error code from response.
 */
AT_CMS_Error at_get_cms_error(const ATResponse *p_response)
{
    int ret;
    int err;
    char *p_cur;

    if (p_response == NULL)
        return CMS_ERROR_NON_CMS;
    else if (p_response->success > 0)
        return CMS_SUCCESS;

    p_cur = p_response->finalResponse;

    if ((p_cur == NULL) || !strStartsWith(p_cur, "+CMS ERROR:")) {
        return CMS_ERROR_NON_CMS;
    }

    /* Looks like a real +CMS ERROR line, so parse it */
    err = at_tok_start(&p_cur);

    if (err < 0) {
        return CMS_ERROR_NON_CMS;
    }

    err = at_tok_nextint(&p_cur, &ret);

    if (err < 0) {
        return CMS_ERROR_NON_CMS;
    }

    return (AT_CMS_Error) ret;
}

void requestSendSMSUMTS_AT(void *data, size_t datalen, RIL_Token t)
{
    int err;
    const char *smsc;
    const char *pdu;
    char *line;
    int tpLayerLength;
    char *cmd1, *cmd2;
    RIL_SMS_Response response;
    RIL_Errno ret = RIL_E_SUCCESS;
    ATResponse *atresponse = NULL;

    smsc = ((const char **) data)[0];
    pdu = ((const char **) data)[1];

   /* Is UDH bit set?*/
    if ( hex2bin(pdu[0],pdu[1]) & 0x40 )
    {
        /* hacking for framework didn't use ExpectMore */
        at_send_command("AT+CMMS=1", NULL);
    }

    tpLayerLength = strlen(pdu) / 2;

    /* NULL for default SMSC. */
    if (smsc == NULL) {
        smsc = "00";
    }

    asprintf(&cmd1, "AT+CMGS=%d", tpLayerLength);
    asprintf(&cmd2, "%s%s", smsc, pdu);

    err = at_send_command_sms(cmd1, cmd2, "+CMGS:", &atresponse);
    free(cmd1);
    free(cmd2);

    if (err != 0 || atresponse->success == 0)
        goto error;

    memset(&response, 0, sizeof(response));
    line = atresponse->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0)
        goto error;

    err = at_tok_nextint(&line, &response.messageRef);
    if (err < 0)
        goto error;

    response.ackPDU = NULL;
    response.errorCode = -1;
    RIL_onRequestComplete(t, RIL_E_SUCCESS, &response, sizeof(response));

finally:
    at_response_free(atresponse);
    return;

error:
    switch (at_get_cms_error(atresponse)) {
    case 332:
    case 331:
        ret = RIL_E_SMS_SEND_FAIL_RETRY;
        break;
    default:
        ret = RIL_E_GENERIC_FAILURE;
        break;
    }
    RIL_onRequestComplete(t, ret, NULL, 0);
    goto finally;
}
