/**************
 *
 *  Filename: qaGobiApiCatTest.h
 *
 *  Purpose:  Card Application Toolkit (CAT) API test driver function prototypes
 *
 * Copyright: © 2010 Sierra Wireless, Inc., all rights reserved
 *
 **************/

#ifndef __GOBI_API_CAT_TEST_H__
#define __GOBI_API_CAT_TEST_H__

/* Defines */
#define nMaxStrLen                 0xFF
#define MAX_CAT_SEND_ENVELOPE_NUM  0x02
#define MAX_CAT_SEND_TR_NUM        0x03

/* Structure for test cases step */
typedef struct _CATSendEnvelopeCommandTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             cmdID;
    ULONG             dataLen;
    BYTE              *pData;
    CHAR              desc[100];
} CATSendEnvelopeCommandTestCase_t;

typedef struct _CATSendTerminalResponseTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             refID;
    ULONG             dataLen;
    BYTE              data[nMaxStrLen];
    CHAR              desc[100];
} CATSendTerminalResponseTestCase_t;

/* Define the test cases to be executed */
BYTE buffer[] = { 0xD3, 0x03, 0x03, 0x00, 0x15 };

CATSendEnvelopeCommandTestCase_t
      CATSendEnvelopeCommandTestCases[MAX_CAT_SEND_ENVELOPE_NUM] =
{
    { eQCWWAN_ERR_NONE, 3, 0x20, sizeof(buffer), buffer, "Invalid Paramter" },
    { eQCWWAN_ERR_NONE, 3, 0x01, sizeof(buffer), buffer, "All valid" }
};

CATSendTerminalResponseTestCase_t
CATSendTerminalResponseTestCases[MAX_CAT_SEND_TR_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 0x05, 2, "Hello World", "Invalid refID" },
    { eQCWWAN_ERR_INVALID_ARG, 2, 0x06, 0, "Hello World", "Invalid data size" },
    { eQCWWAN_ERR_NONE,        3, 0x02, 1, "Hello World", "All valid" }
};

#endif /* __GOBI_API_CAT_TEST_H__ */
