/**************
 *
 *  Filename: qaGobiApiSmsTest.h
 *
 *  Purpose:  Short Message Service (SMS) API test driver function prototypes
 *
 * Copyright: © 2011-2012 Sierra Wireless, Inc., all rights reserved
 *
 **************/

#ifndef __GOBI_API_SMS_TEST_H__
#define __GOBI_API_SMS_TEST_H__

/* Defines */
#define nMaxStrLen                                     0xFF
#define nByteStreamLen                                 0XFF
#define nMaxSMSCAddrLen                                0x15
#define nMaxSMSCTypeLen                                0x04
#define MAX_SLQS_DELETE_SMS_TESTCASE_NUM               0x21
#define MAX_SLQS_GET_SMS_TESTCASE_NUM                  0x0F
#define MAX_SEND_SMS_TESTCASE_NUM                      0x05
#define MAX_GET_SMSC_ADDRESS_TESTCASE_NUM              0x06
#define MAX_SET_SMSC_ADDRESS_TESTCASE_NUM              0x04
#define MAX_SAVE_SMS_TESTCASE_NUM                      0x04
#define MAX_SLQS_GET_SMS_LIST_TESTCASE_NUM             0x0D
#define MAX_SLQS_MODIFY_SMS_STATUS_TESTCASE_NUM        0x09
#define MAX_SLQS_GET_SMS_BROADCAST_CONFIG_TESTCASE_NUM 0x07
#define MAX_SLQS_SET_SMS_BROADCAST_CONFIG_TESTCASE_NUM 0x06
#define MAX_SET_SMS_BC_ACTIVATION_TESTCASE_NUM         0x07
#define MAX_ENCODE_TEXT_TESTCASE_NUM                   0x07
#define MAX_SLQS_DECODE_CDMA_SMS_TESTCASE_NUM          0x05
#define MAX_ENCODE_WCDMA_TESTCASE_NUM                  0x05
#define MAX_DECODE_WCDMA_TESTCASE_NUM                  0x05
#define HEX_BUFFER_SIZE                                200
#define STORAGEINDEX_NV                                1
#define STORAGEINDEX_UIM                               0

/* Structure for test cases step */
typedef struct _SLQSDeleteSMSTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             storageType;
    ULONG             *pMessageIndex;
    ULONG             *pMessageTag;
    BYTE              *pMessageMode;
    CHAR              desc[100];
} SLQSDeleteSMSTestCase_t;

typedef struct _SLQSGetSMSTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             storageType;
    ULONG             messageIndex;
    ULONG             messageSize;
    ULONG             *pMessageTag;
    ULONG             *pMessageFormat;
    BYTE              *pMessage;
    BYTE              *pMessageMode;
    CHAR              desc[100];
} SLQSGetSMSTestCase_t;

typedef struct _SendSMSTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             messageFormat;
    ULONG             messageSize;
    BYTE              *message;
    USHORT            *messageID;
    ULONG             *messageFailureCode;
    CHAR              desc[100];
} SendSMSTestCase_t;

typedef struct _GetSMSCAddressTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              addressSize;
    CHAR              *pSMSCAddress;
    BYTE              typeSize;
    CHAR              *pSMSCType;
    CHAR              desc[100];
} GetSMSCAddressTestCase_t;

typedef struct _SetSMSCAddressTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              *pSMSCAddress;
    CHAR              *pSMSCType;
    CHAR              desc[100];
} SetSMSCAddressTestCase_t;

typedef struct _SaveSMSTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             storageType;
    ULONG             messageFormat;
    ULONG             messageSize;
    BYTE              *message;
    CHAR              desc[100];
} SaveSMSTestCase_t;

typedef struct _SLQSGetSMSListTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             storageType;
    ULONG             requestedTag;
    ULONG             messageListSize;
    BYTE              *pMessageMode;
    CHAR              desc[100];
} SLQSGetSMSListTestCase_t;

typedef struct _SLQSModifySMSStatusTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             storageType;
    ULONG             messageIndex;
    ULONG             messageTag;
    BYTE              *pMessageMode;
    CHAR              desc[100];
} SLQSModifySMSStatusTestCase_t;

typedef struct _SLQSSetBroadcastConfigTestCase_t
{
    enum eQCWWANError          expectedReturnValue;
    ULONG                      testCaseId;
    BYTE                       mode;
    qaQmi3GPPBroadcastCfgInfo  *pBroadcastConfig;
    qaQmi3GPP2BroadcastCfgInfo *pCDMABroadcastConfig;
    CHAR                       desc[100];
} SLQSSetBroadcastConfigTestCase_t;

typedef struct _SLQSGetSMSBroadcastConfigTestCase_t
{
    enum eQCWWANError          expectedReturnValue;
    ULONG                      testCaseId;
    BYTE                       mode;
    qaQmi3GPPBroadcastCfgInfo  *q3GPPBroadcastCfgInfo;
    qaQmi3GPP2BroadcastCfgInfo *q3GPP2BroadcastCfgInfo;
    CHAR                       desc[100];
} SLQSGetSMSBroadcastConfigTestCase_t;

typedef struct _SetSMSBCActivationTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              mode;
    BYTE              bc_activate;
    CHAR              desc[100];
} SetSMSBCActivationTestCase_t;

typedef struct _SLQSCDMAEncodeMOTextMsgTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              *pMessageSize;
    BYTE              messageId;
    CHAR              *pDestAddr;
    CHAR              *pCallbackAddr;
    ULONG             textMsgLength;
    WORD              *pTextMsg;
    BYTE              *pPriority;
    BYTE              *pEncodingAlphabet;
    BYTE              *pRelValidity;
    CHAR              desc[100];
} SLQSCDMAEncodeMOTextMsgTestCase_t;

typedef struct _SLQSDecodeTextToPDUTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} SLQSDecodeTextToPDUTestCase_t;

typedef struct _SLQSWCDMAEncodeMOTextMsgTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             messageFormat;
    ULONG             messageSize;
    CHAR              *pDestAddr;
    CHAR              *pTextMsg;
    ULONG             *messageFailureCode1;
    CHAR              desc[100];
} SLQSWCDMAEncodeMOTextMsgTestCase_t;

typedef struct _SLQSWCDMADecodeMTTextMsgTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} SLQSWCDMADecodeMTTextMsgTestCase_t;

typedef struct _SLQSGetTransLayerInfoTestCase_t
{
    enum                  eQCWWANError expectedReturnValue;
    ULONG                              testCaseId;
    getTransLayerInfoResp              *pGetTransLayerInfoResp;
    CHAR                               desc[100];
} SLQSGetTransLayerInfoTestCase_t;

typedef struct _SLQSGetTransNWRegInfoTestCase_t
{
    enum                  eQCWWANError expectedReturnValue;
    ULONG                              testCaseId;
    getTransNWRegInfoResp              *pTransNWRegInfo;
    CHAR                               desc[100];
} SLQSGetTransNWRegInfoTestCase_t;

typedef struct _SLQSGetIndicationRegisterTestCase_t
{
    enum                  eQCWWANError expectedReturnValue;
    ULONG                              testCaseId;
    getIndicationRegResp               *pGetIndicationRegResp;
    CHAR                               desc[100];
} SLQSGetIndicationRegisterTestCase_t;

typedef struct _SLQSSetIndicationRegisterTestCase_t
{
    enum  eQCWWANError  expectedReturnValue;
    ULONG               testCaseId;
    setIndicationRegReq *pSetIndicationRegReq;
    CHAR                desc[100];
} SLQSSetIndicationRegisterTestCase_t;

typedef struct _SLQSSetsmsRouteTestCase_t
{
    enum eQCWWANError  expectedReturnValue;
    ULONG              testCaseId;
    smsSetRoutesReq   *pSetRoutesReq;
    CHAR               desc[100];
} SLQSSetsmsRouteTestCase_t;

typedef struct _SLQSSmsGetMessageProtocolTestCase_t
{
    enum eQCWWANError  expectedReturnValue;
    ULONG              testCaseId;
    smsMsgprotocolResp *pMessageProtocol;
    CHAR               desc[100];
} SLQSSmsGetMessageProtocolTestCase_t;

typedef struct _SLQSSmsGetMaxStorageSizeTestCase_t
{
    enum eQCWWANError   expectedReturnValue;
    ULONG               testCaseId;
    smsMaxStorageSizeReq  *pReq;
    smsMaxStorageSizeResp *pResp;
    CHAR               desc[100];
} SLQSSmsGetMaxStorageSizeTestCase_t;

/* Define the test cases to be executed */
#define QMI_WMS_UIM         0x00
#define QMI_WMS_NV          0x01
#define QMI_WMS_MT_READ     0x00
#define QMI_WMS_MT_NOT_READ 0x01
#define QMI_WMS_MO_SENT     0x02
#define QMI_WMS_MO_NOT_SENT 0x03

ULONG lRead          = QMI_WMS_MT_READ;
ULONG unread        = QMI_WMS_MT_NOT_READ;
ULONG sent          = QMI_WMS_MO_SENT;
ULONG unsent        = QMI_WMS_MO_NOT_SENT;
ULONG messageIndex  = 3;
ULONG invalidMsgTag = 22;

BYTE msgModeCDMA    = 0x00;
BYTE msgModeGW      = 0x01;
BYTE invalidMsgMode = 0x02;

SLQSDeleteSMSTestCase_t
    SLQSDeleteSMSTestCases[MAX_SLQS_DELETE_SMS_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 0x03, NULL, NULL, &msgModeCDMA,\
        "Invalid Storage Type" },
    { eQCWWAN_ERR_INVALID_ARG, 2, 0x03, NULL, NULL, &msgModeGW,\
        "Invalid Storage Type" },
    { eQCWWAN_ERR_INVALID_ARG, 3, QMI_WMS_UIM, NULL, &invalidMsgTag,\
        &msgModeCDMA, "Invalid message tag" },
    { eQCWWAN_ERR_INVALID_ARG, 4, QMI_WMS_UIM, NULL, &invalidMsgTag,\
        &msgModeGW, "Invalid message tag" },
    { eQCWWAN_ERR_INVALID_ARG, 5, QMI_WMS_UIM, NULL, NULL, &invalidMsgMode,\
        "Invalid message mode" },
    { eQCWWAN_ERR_NONE,        6, QMI_WMS_UIM, &messageIndex, NULL,\
        &msgModeCDMA, "Delete by index from UIM in CDMA Mode" },
    { eQCWWAN_ERR_NONE,        7, QMI_WMS_UIM, &messageIndex, NULL,\
        &msgModeGW, "Delete by index from UIM in GW Mode" },
    { eQCWWAN_ERR_NONE,        8, QMI_WMS_NV, &messageIndex, NULL,\
        &msgModeCDMA, "Delete by index from NV in CDMA Mode" },
    { eQCWWAN_ERR_NONE,        9, QMI_WMS_NV, &messageIndex, NULL,\
        &msgModeGW, "Delete by index from NV in GW Mode" },
    { eQCWWAN_ERR_NONE,       10, QMI_WMS_UIM, NULL, &lRead, &msgModeCDMA,\
        "Delete All read messages from UIM in CDMA Mode" },
    { eQCWWAN_ERR_NONE,       11, QMI_WMS_UIM, NULL, &lRead, &msgModeGW,\
        "Delete All read messages from UIM in GW Mode" },
    { eQCWWAN_ERR_NONE,       12, QMI_WMS_UIM, NULL, &unread, &msgModeCDMA,\
        "Delete All unread messages from UIM in CDMA Mode" },
    { eQCWWAN_ERR_NONE,       13, QMI_WMS_UIM, NULL, &unread, &msgModeGW,\
        "Delete All unread messages from UIM in GW Mode" },
    { eQCWWAN_ERR_NONE,       14, QMI_WMS_UIM, NULL, &sent, &msgModeCDMA,\
      "Delete All sent messages from UIM in CDMA Mode" },
    { eQCWWAN_ERR_NONE,       15, QMI_WMS_UIM, NULL, &sent, &msgModeGW,\
      "Delete All sent messages from UIM in GW Mode" },
    { eQCWWAN_ERR_NONE,       16, QMI_WMS_UIM, NULL, &unsent, &msgModeCDMA,\
       "Delete All unsent messages from UIM in CDMA Mode" },
    { eQCWWAN_ERR_NONE,       17, QMI_WMS_UIM, NULL, &unsent, &msgModeGW,\
       "Delete All unsent messages from UIM in GW Mode" },
    { eQCWWAN_ERR_NONE,       18, QMI_WMS_UIM, NULL, NULL, &msgModeCDMA,\
       "Delete All messages with CDMA Mode from UIM in CDMA Mode" },
    { eQCWWAN_ERR_NONE,       19, QMI_WMS_UIM, NULL, NULL, &msgModeGW,\
       "Delete All messages with GW Mode from UIM in GW Mode" },
    { eQCWWAN_ERR_NONE,       20, QMI_WMS_UIM, NULL, NULL, &msgModeCDMA,\
       "Delete All messages from UIM in CDMA Mode" },
    { eQCWWAN_ERR_NONE,       21, QMI_WMS_UIM, NULL, NULL, &msgModeGW,\
       "Delete All messages from UIM in GW Mode" },
    { eQCWWAN_ERR_NONE,       22, QMI_WMS_NV, NULL, &lRead, &msgModeCDMA,\
       "Delete All read messages from NV in CDMA Mode" },
    { eQCWWAN_ERR_NONE,       23, QMI_WMS_NV, NULL, &lRead, &msgModeGW,\
       "Delete All read messages from NV in GW Mode" },
    { eQCWWAN_ERR_NONE,       24, QMI_WMS_NV, NULL, &unread, &msgModeCDMA,\
       "Delete All unread messages from NV in CDMA Mode" },
    { eQCWWAN_ERR_NONE,       25, QMI_WMS_NV, NULL, &unread, &msgModeGW,\
       "Delete All unread messages from NV in GW Mode" },
    { eQCWWAN_ERR_NONE,       26, QMI_WMS_NV, NULL, &sent, &msgModeCDMA,\
       "Delete All sent messages from NV in CDMA Mode" },
    { eQCWWAN_ERR_NONE,       27, QMI_WMS_NV, NULL, &sent, &msgModeGW,\
       "Delete All sent messages from NV in GW Mode" },
    { eQCWWAN_ERR_NONE,       28, QMI_WMS_NV, NULL, &unsent, &msgModeCDMA,\
       "Delete All unsent messages from NV in CDMA Mode" },
    { eQCWWAN_ERR_NONE,       29, QMI_WMS_NV, NULL, &unsent, &msgModeGW,\
       "Delete All unsent messages from NV in GW Mode" },
    { eQCWWAN_ERR_NONE,       30, QMI_WMS_NV, NULL, NULL, &msgModeCDMA,\
       "Delete All messages with CDMA Mode from NV in CDMA Mode" },
    { eQCWWAN_ERR_NONE,       31, QMI_WMS_NV, NULL, NULL, &msgModeGW,\
       "Delete All messages with GW Mode from NV in GW Mode" },
    { eQCWWAN_ERR_NONE,       32, QMI_WMS_NV, NULL, NULL, &msgModeCDMA,\
       "Delete All messages from NV in CDMA mode" },
    { eQCWWAN_ERR_NONE,       33, QMI_WMS_NV, NULL, NULL, &msgModeGW,\
       "Delete All messages from NV in GW Mode" },
};

ULONG messageTag;
ULONG messageFormat;
BYTE  message[nMaxStrLen + 2];

SLQSGetSMSTestCase_t
    SLQSGetSMSTestCases[MAX_SLQS_GET_SMS_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 0x03, 1, nMaxStrLen+1, NULL,\
         NULL, NULL, &msgModeCDMA, "Sending all out params as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 2, 0x03, 1, nMaxStrLen+1, NULL,\
         NULL, message, &msgModeCDMA, "Sending two OUT params as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 3, 0x03, 1, nMaxStrLen+1, NULL,\
         &messageFormat, NULL, &msgModeCDMA, "Sending two OUT params as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 4, 0x03, 1, nMaxStrLen+1, &messageTag,\
         NULL, NULL, &msgModeCDMA, "Sending two OUT params as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 5, 0x03, 1, nMaxStrLen+1, NULL,\
         &messageFormat, message, &msgModeCDMA, "Sending one OUT param as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 6, 0x03, 1, nMaxStrLen+1, &messageTag,\
         NULL, message, &msgModeCDMA, "Sending one OUT param as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 7, 0x03, 1, nMaxStrLen+1, &messageTag,\
         &messageFormat, NULL, &msgModeCDMA, "Sending one OUT param as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 8, 0x03, 1, nMaxStrLen+1, &messageTag,\
         &messageFormat, message, &msgModeCDMA, "Invalid Storage Type" },
    { eQCWWAN_ERR_INVALID_ARG, 9, 0x03, 1, nMaxStrLen+1, &messageTag,\
         &messageFormat, message, &msgModeGW, "Invalid Storage Type" },
    { eQCWWAN_ERR_BUFFER_SZ,  10, 0x00, 1, 1, &messageTag,\
         &messageFormat, message, &msgModeCDMA, "Sending small message Size" },
    { eQCWWAN_ERR_BUFFER_SZ,  11, 0x00, 1, 1, &messageTag,\
         &messageFormat, message, &msgModeGW, "Sending small message Size" },
    { eQCWWAN_ERR_NONE,       12, 0x00, 1, nMaxStrLen+1, &messageTag,\
         &messageFormat, message, &msgModeCDMA, "All valid UIM" },
    { eQCWWAN_ERR_NONE,       13, 0x00, 1, nMaxStrLen+1, &messageTag,\
         &messageFormat, message, &msgModeGW, "All valid UIM" },
    { eQCWWAN_ERR_NONE,       14, 0x01, 1, nMaxStrLen+1, &messageTag,\
         &messageFormat, message, &msgModeCDMA, "All valid NV" },
    { eQCWWAN_ERR_NONE,       15, 0x01, 1, nMaxStrLen+1, &messageTag,\
         &messageFormat, message, &msgModeGW, "All valid NV" }
};

/* Keeping SMSC information byte as 0x00 as default */
BYTE smsBuffer[] = { 0x00, 0x01, 0x00, 0x0C, 0x91, 0x19, 0x79, 0x97, 0x11,
                     0x21, 0x82, 0x00, 0x00, 0x05, 0xE8, 0x32, 0x9B, 0xFD,
                     0x06 };

ULONG  messageFailureCode;
USHORT messageID;

SendSMSTestCase_t
      SendSMSTestCases[MAX_SEND_SMS_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 0x07, sizeof(smsBuffer), smsBuffer,\
      &messageID, &messageFailureCode, "Invalid Message Format" },
    { eQCWWAN_ERR_INVALID_ARG, 2, 0x00, sizeof(smsBuffer), smsBuffer,\
      NULL, NULL, "Sending NULL for OUT param"},
    { eQCWWAN_ERR_INVALID_ARG, 2, 0x02, sizeof(smsBuffer), smsBuffer,\
      &messageID, &messageFailureCode, "Reserved Value Sent for Message Format" },
    { eQCWWAN_ERR_NONE, 3, 0x00, sizeof(smsBuffer), smsBuffer,\
      &messageID, &messageFailureCode, "All valid CDMA" },
    { eQCWWAN_ERR_NONE, 4, 0x06, sizeof(smsBuffer), smsBuffer,\
      &messageID, &messageFailureCode, "All valid GSM/WCDMA PP" }
};

CHAR SMSCAddress[nMaxSMSCAddrLen + 2];
CHAR SMSCType[nMaxSMSCTypeLen + 1];

GetSMSCAddressTestCase_t
      GetSMSCAddressTestCases[MAX_GET_SMSC_ADDRESS_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, nMaxSMSCAddrLen, NULL, nMaxSMSCTypeLen,\
        NULL, "Invalid Argument" },

    { eQCWWAN_ERR_INVALID_ARG, 2, nMaxSMSCAddrLen, NULL, nMaxSMSCTypeLen,\
        SMSCType, "Invalid Argument" },

    { eQCWWAN_ERR_INVALID_ARG, 3, nMaxSMSCAddrLen, SMSCAddress,\
        nMaxSMSCTypeLen, NULL, "Invalid Argument" },

    { eQCWWAN_ERR_BUFFER_SZ, 4, nMaxSMSCAddrLen, SMSCAddress, 0, SMSCType,\
      "Small Size for SMSCtype" },

    { eQCWWAN_ERR_BUFFER_SZ, 5, 0, SMSCAddress, nMaxSMSCTypeLen , SMSCType,\
      "Small Size for SMSCAddress" },

    { eQCWWAN_ERR_NONE, 6, nMaxSMSCAddrLen, SMSCAddress, nMaxSMSCTypeLen,\
      SMSCType, "All Valid" }
};

SetSMSCAddressTestCase_t
      SetSMSCAddressTestCases[MAX_SET_SMSC_ADDRESS_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, "+123456789123456789000", "012",\
                                  "Invalid SMSC Address length including +" },
    { eQCWWAN_ERR_INVALID_ARG, 2, "123456789123456789000", "012",\
                                  "Invalid SMSC Address length excluding +" },
    { eQCWWAN_ERR_NONE,        3, "+919815011111", "145",\
                                  "All Valid" },
    { eQCWWAN_ERR_NONE,        4, "11111", "129",\
                                  "All Valid" }
};

/* Keeping SMSC information byte as 0x00 as default */
BYTE msgBuffer[] = { 0x00, 0x01, 0x00, 0x0C, 0x91, 0x19, 0x79, 0x97, 0x11,
                  0x21, 0x82, 0x00, 0x00, 0x05, 0xE8, 0x32, 0x9B, 0xFD,
                  0x06 };

SaveSMSTestCase_t
      SaveSMSTestCases[MAX_SAVE_SMS_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 0x02, 0x06, sizeof(msgBuffer), msgBuffer,\
        "Invalid Storage Type" },
    { eQCWWAN_ERR_INVALID_ARG, 2, 0x02, 0x07, sizeof(msgBuffer), msgBuffer,\
        "Invalid Format" },
    { eQCWWAN_ERR_NONE, 3, 0x00, 0x06, sizeof(msgBuffer), msgBuffer,\
        "Write to UIM" },
    { eQCWWAN_ERR_NONE, 4, 0x01, 0x06, sizeof(msgBuffer), msgBuffer,\
        "Write to NV" }
};

SLQSGetSMSListTestCase_t
      SLQSGetSMSListTestCases[MAX_SLQS_GET_SMS_LIST_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 0x00,  0x04, nMaxStrLen, &invalidMsgMode,
        "Invalid Message Mode" },
    { eQCWWAN_ERR_BUFFER_SZ,   2, 0x01,  0x01,          0, &msgModeCDMA,
        "Small SMS List Size, Mode: CDMA" },
    { eQCWWAN_ERR_INVALID_ARG, 3, 0x02,  0x02, nMaxStrLen, &msgModeCDMA,
        "Invalid Storage Type , Mode: CDMA" },
    { eQCWWAN_ERR_INVALID_ARG, 4, 0x00,  0x05, nMaxStrLen, &msgModeCDMA,
        "Invalid Message Tag, Mode: CDMA" },
    { eQCWWAN_ERR_BUFFER_SZ,   5, 0x00,  0x03,          0, &msgModeCDMA,
        "Small SMS List Size, Mode: CDMA" },
    { eQCWWAN_ERR_NONE,        6, 0x01,  0x02, nMaxStrLen, &msgModeCDMA,
        "Get all messages list,UIM, Mode: CDMA" },
    { eQCWWAN_ERR_NONE,        7, 0x00,  0x03, nMaxStrLen, &msgModeCDMA,
        "Get all messages list,NV, Mode: CDMA" },
    { eQCWWAN_ERR_BUFFER_SZ,   8, 0x01,  0x01,          0, &msgModeGW,
        "Small SMS List Size, Mode: GW" },
    { eQCWWAN_ERR_INVALID_ARG, 9, 0x02,  0x02, nMaxStrLen, &msgModeGW,
        "Invalid Storage Type , Mode: GW" },
    { eQCWWAN_ERR_INVALID_ARG, 10, 0x00,  0x05, nMaxStrLen, &msgModeGW,
        "Invalid Message Tag, Mode: GW" },
    { eQCWWAN_ERR_BUFFER_SZ,   11, 0x00,  0x03,          0, &msgModeGW,
        "Small SMS List Size, Mode: GW" },
    { eQCWWAN_ERR_NONE,        12, 0x01,  0x02, nMaxStrLen, &msgModeGW,
        "Get all messages list,UIM, Mode: GW" },
    { eQCWWAN_ERR_NONE,        13, 0x00,  0x03, nMaxStrLen, &msgModeGW,
        "Get all messages list,NV, Mode: GW" }
};

SLQSModifySMSStatusTestCase_t
      SLQSModifySMSStatusTestCases[MAX_SLQS_MODIFY_SMS_STATUS_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 0x00, 1,  1,  &invalidMsgMode,
      "Invalid Message Mode" },
    { eQCWWAN_ERR_INVALID_ARG, 2, 0x03, 1,  1,  &msgModeCDMA,
      "Invalid Storage Type, Mode: CDMA" },
    { eQCWWAN_ERR_INVALID_ARG, 3, 0x01, 1,  22, &msgModeCDMA,
      "Invalid message tag, Mode: CDMA" },
    { eQCWWAN_ERR_INVALID_ARG, 4, 0x00, 10, 0,  &msgModeCDMA,
      "All valid UIM, Mode: CDMA" },
    { eQCWWAN_ERR_INVALID_ARG, 5, 0x01, 10, 0,  &msgModeCDMA,
      "All valid NV, Mode: CDMA" },
    { eQCWWAN_ERR_INVALID_ARG, 6, 0x03, 1,  1,  &msgModeGW,
      "Invalid Storage Type, Mode: GW" },
    { eQCWWAN_ERR_INVALID_ARG, 7, 0x01, 1,  22, &msgModeGW,
      "Invalid message tag, Mode: GW" },
    { eQCWWAN_ERR_NONE,        8, 0x00, 10, 0,  &msgModeGW,
      "All valid UIM, Mode: GW" },
    { eQCWWAN_ERR_NONE,        9, 0x01, 10, 0,  &msgModeGW,
      "All valid NV, Mode: GW" }
};
#define MODE_INVALID 0x02
#define MODE_CDMA    0x00
#define MODE_GW      0x01

qaQmi3GPPBroadcastCfgInfo  q3GPPBroadcastCfgInfo;
qaQmi3GPP2BroadcastCfgInfo q3GPP2BroadcastCfgInfo;

SLQSGetSMSBroadcastConfigTestCase_t
    SLQSGetSMSBroadcastConfigTestCases[MAX_SLQS_GET_SMS_BROADCAST_CONFIG_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_INVALID_ARG, 1 , MODE_INVALID, &q3GPPBroadcastCfgInfo,
    &q3GPP2BroadcastCfgInfo,"Invalid mode" },
    {eQCWWAN_ERR_INVALID_ARG, 2 , MODE_CDMA   , NULL,
    NULL,"Invalid Arguments CDMA mode"},
    {eQCWWAN_ERR_NONE       , 3 , MODE_CDMA   , NULL,
    &q3GPP2BroadcastCfgInfo,"Valid Arguments CDMA mode"},
    {eQCWWAN_ERR_NONE       , 4 , MODE_CDMA   , &q3GPPBroadcastCfgInfo,
    &q3GPP2BroadcastCfgInfo,"Valid Arguments CDMA mode"},
    {eQCWWAN_ERR_INVALID_ARG, 5 , MODE_GW     , NULL,
    NULL,"Invalid Arguments GW mode"},
    {eQCWWAN_ERR_NONE       , 6 , MODE_GW     , &q3GPPBroadcastCfgInfo,
    NULL,"Valid Arguments GW mode"},
    {eQCWWAN_ERR_NONE       , 7 , MODE_GW     , &q3GPPBroadcastCfgInfo,
    &q3GPP2BroadcastCfgInfo,"Valid Arguments GW mode"},
};

qaQmi3GPPBroadcastCfgInfo  lBroadcastConfig;
qaQmi3GPP2BroadcastCfgInfo lCDMABroadcastConfig;

SLQSSetBroadcastConfigTestCase_t
      SLQSSetBroadcastConfigTestCases[MAX_SLQS_SET_SMS_BROADCAST_CONFIG_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 0x00, NULL,
                                 NULL, "NULL Arguments" },
    { eQCWWAN_ERR_NONE, 2, 0x00, &lBroadcastConfig,
                                 &lCDMABroadcastConfig, "Valid CDMA Mode" },
    { eQCWWAN_ERR_NONE, 3, 0x01, &lBroadcastConfig,
                                 &lCDMABroadcastConfig, "Valid GW Mode" },
    { eQCWWAN_ERR_INVALID_ARG, 4, 0x00, NULL,
                                 &lCDMABroadcastConfig, "Valid CDMA Mode" },
    { eQCWWAN_ERR_INVALID_ARG, 5, 0x01, &lBroadcastConfig,
                                  NULL, "Valid GW Mode" },
    { eQCWWAN_ERR_INVALID_ARG, 6, 0x05, &lBroadcastConfig,
                                 &lCDMABroadcastConfig, "Invalid Message Mode" },
};

SetSMSBCActivationTestCase_t
    SetSMSBCActivationTestCases[MAX_SET_SMS_BC_ACTIVATION_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 0x02, 0x01,
      "Invalid Mode" },
    { eQCWWAN_ERR_INVALID_ARG, 2, 0x01, 0x02,
      "Invalid bc_activate" },
    { eQCWWAN_ERR_INVALID_ARG, 3, 0x02, 0x02,
      "Both Invalid parameters" },
    { eQCWWAN_ERR_NONE, 4, 0x01, 0x01,
      "All valid Mode: GW, Activate" },
    { eQCWWAN_ERR_NONE, 5, 0x01, 0x00,
      "All valid Mode: GW, Disable" },
    { eQCWWAN_ERR_NONE, 6, 0x00, 0x01,
      "All valid Mode: CDMA, Activate" },
    { eQCWWAN_ERR_NONE, 7, 0x00, 0x00,
      "All valid Mode: CDMA, Disable" }
};

CHAR smsMessage[]  = "SLQSSend SMS API";
CHAR smsMessage1[] = " SLQSSend SMS API SLQSSend SMS API SLQSSend SMS API "
                     "SLQSSend SMS API SLQSSend SMS API SLQSSend SMS API "
                     "SLQSSend SMS API SLQSSend SMS API SLQSSend SMS API "
                     "SLQSSend SMS API SLQSSend SMS API SLQSSend SMS API "
                     "SLQSSend SMS API SLQSSend SMS API SLQSSend SMS API ";

CHAR  mobileNo[] = "2146625849";
ULONG messageFailureCode;
BYTE  MessageSize = 250;
BYTE  EncodingAlphabet = 2;
BYTE  messageId = 0;
CHAR  callbackAddr[] ="919501116194";
BYTE  priority = 2;

SLQSCDMAEncodeMOTextMsgTestCase_t
      SLQSCDMAEncodeMOTextMsgTestCases[MAX_ENCODE_TEXT_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, &MessageSize, 0, 0, NULL,\
      sizeof(smsMessage1)-1,(WORD*)smsMessage1, NULL, &EncodingAlphabet,\
      NULL, "Mobile number invalid: Message Sending should fail" },

    { eQCWWAN_ERR_INVALID_ARG, 2, NULL, 0, mobileNo, NULL,\
      sizeof(smsMessage1)-1,(WORD*)smsMessage1, NULL, &EncodingAlphabet,\
      NULL, "MessageSize is Invalid: Message Sending should fail" },

    { eQCWWAN_ERR_INVALID_ARG, 3, &MessageSize, 0, 0, NULL,\
      sizeof(smsMessage1),(WORD*)smsMessage1, NULL, &EncodingAlphabet,\
      NULL, "Valid Message Length: Message Sending should work" },

    { eQCWWAN_ERR_NONE, 4, &MessageSize, 1, mobileNo, NULL,\
      sizeof(smsMessage1)-1,(WORD*)smsMessage1, NULL, &EncodingAlphabet,\
      NULL, "Message with more than 160 Char " },

    { eQCWWAN_ERR_NONE, 5, &MessageSize, 0, mobileNo, NULL,\
      sizeof(smsMessage)-1,(WORD*)smsMessage, NULL, &EncodingAlphabet,\
      NULL, "Message with less than 160 Char " },

    { eQCWWAN_ERR_NONE, 6, &MessageSize, 0, mobileNo, callbackAddr,\
      sizeof(smsMessage)-1,(WORD*)smsMessage, NULL, &EncodingAlphabet,\
      NULL, "Valid Callback address:Message Sending should work" },

    { eQCWWAN_ERR_NONE, 7, &MessageSize, 0, mobileNo, callbackAddr,\
      sizeof(smsMessage)-1,(WORD*)smsMessage, &priority, &EncodingAlphabet,\
      NULL, "Valid Priority:Message Sending should work" }
};

SLQSDecodeTextToPDUTestCase_t
SLQSDecodeCDMASMSTestCases[MAX_SLQS_DECODE_CDMA_SMS_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, "Valid Case" },

    { eQCWWAN_ERR_NONE, 2, "Valid Case" },

    { eQCWWAN_ERR_NONE, 3, "Invalid Case" },

    { eQCWWAN_ERR_NONE, 4, "Invalid Case" },

    { eQCWWAN_ERR_NONE, 5, "Valid Case " },

};

SLQSWCDMAEncodeMOTextMsgTestCase_t
      SLQSWCDMAEncodeMOTextMsgTestCases[MAX_ENCODE_WCDMA_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 0x07, sizeof(smsMessage), mobileNo,\
      smsMessage, &messageFailureCode, " Invalid Message Format:"
                                       " Message Sending should fail" },

    { eQCWWAN_ERR_NONE , 2, 0x06, sizeof(smsMessage), mobileNo,\
      smsMessage, NULL, "OUT Parameter is NULL: Message Sending should work"},

    { eQCWWAN_ERR_INVALID_ARG, 3, 0x06, sizeof(smsMessage), mobileNo,\
      NULL, &messageFailureCode, " Message is NULL:Message Sending"
                                 " should fail"},

    { eQCWWAN_ERR_NONE , 4, 0x06, sizeof(smsMessage1), mobileNo, smsMessage1,\
      NULL, "Valid Case"},

    { eQCWWAN_ERR_NONE , 5, 0x06, sizeof(smsMessage), NULL, smsMessage,\
      NULL, " Mobile number field is NULL: Message Sending should fail "},
};

SLQSWCDMADecodeMTTextMsgTestCase_t
    SLQSWCDMADecodeMTTextMsgTestCases[MAX_DECODE_WCDMA_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE,        1, "Valid Case"},

    { eQCWWAN_ERR_INVALID_ARG, 2, "Invalid Case"},

    { eQCWWAN_ERR_NONE,        3, "Valid Case"},

    { eQCWWAN_ERR_NONE,        4, "Valid Case"},

    { eQCWWAN_ERR_INVALID_ARG, 5, "Invalid Case"}
};

#endif /* __GOBI_API_SMS_TEST_H__ */
