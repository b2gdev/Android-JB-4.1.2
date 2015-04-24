/**************
 *
 *  Filename: qaGobiApiOmaDmTest.h
 *
 *  Purpose:  Contains prototypes for functions implementing specific tests
 *            for Open Mobile Alliance Device Management Service (OMADMS)
 *            called by the main() routine in qatest.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc., all rights reserved
 *
 **************/

#ifndef __GOBI_API_OMADM_TEST_H__
#define __GOBI_API_OMADM_TEST_H__

/* Defines */
#define MAX_OMADMS_START_SESSION_TESTCASE_NUM    0x04
#define MAX_OMADMS_CANCEL_SESSION_TESTCASE_NUM   0x01
#define MAX_OMADMS_GET_SESSION_INFO_TESTCASE_NUM 0x0A
#define MAX_OMADMS_GET_PENDING_NIA_TESTCASE_NUM  0x03

/* Structure for test cases step */
typedef struct _OMADMSStartSessionTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             sessiontype;
    CHAR              desc[100];
} OMADMSStartSessionTestCase_t;

/* Define the test cases to be executed */
const OMADMSStartSessionTestCase_t \
    OMADMSStartSessionTestCases[MAX_OMADMS_START_SESSION_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 0x03, "Invalid SessionType" },
    { eQCWWAN_ERR_NONE,        2, 0x00, "All valid: device configure" },
    { eQCWWAN_ERR_NONE,        3, 0x01, "All valid: PRL update" },
    { eQCWWAN_ERR_NONE,        4, 0x02, "All valid: hands-free activation" },
};

/* Structure for test cases step */
typedef struct _OMADMSCancelSessionTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} OMADMSCancelSessionTestCase_t;

/* Define the test cases to be executed */
const OMADMSCancelSessionTestCase_t \
    OMADMSCancelSessionTestCases[
                               MAX_OMADMS_CANCEL_SESSION_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, "All valid" }
};

/* Structure for test cases step */
typedef struct _OMADMSGetSessionInfoTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             *pSessionState;
    ULONG             *pSessionType;
    ULONG             *pFailureReason;
    BYTE              *pRetryCount;
    WORD              *pSessionPause;
    WORD              *pTimeRemaining;
    CHAR              desc[100];
} OMADMSGetSessionInfoTestCase_t;

/* Define the test cases to be executed */
ULONG sessionState;
ULONG SessionType;
ULONG FailureReason;
BYTE  RetryCount;
WORD  SessionPause;
WORD  TimeRemaining;

const OMADMSGetSessionInfoTestCase_t \
    OMADMSGetSessionInfoTestCases[
                               MAX_OMADMS_GET_SESSION_INFO_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, NULL, NULL,
      NULL, NULL, NULL, NULL,
    "ALL parameters NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 2, NULL, &SessionType,
    &FailureReason, &RetryCount, &SessionPause, &TimeRemaining,
    "Session State parameter NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 3, &sessionState, NULL,
    &FailureReason, &RetryCount, &SessionPause, &TimeRemaining,
    "Session Type parameter NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 4, &sessionState, &SessionType,
    NULL, &RetryCount, &SessionPause, &TimeRemaining,
    "Failure Reason parameter NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 5, &sessionState, &SessionType,
    &FailureReason, NULL, &SessionPause, &TimeRemaining,
    "Retry Count parameter NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 6, &sessionState, &SessionType,
    &FailureReason, &RetryCount, NULL, &TimeRemaining,
    "Session Pause parameter NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 7, &sessionState, &SessionType,
    &FailureReason, &RetryCount, &SessionPause, NULL,
    "Time Remaining parameter NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 8, NULL, &SessionType,
    &FailureReason, &RetryCount, NULL, &TimeRemaining,
    "Two parameters NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 9, &sessionState, NULL,
    NULL, &RetryCount, NULL, &TimeRemaining,
    "Three parameters NULL" },
    { eQCWWAN_ERR_NONE, 10, &sessionState, &SessionType,
    &FailureReason, &RetryCount, &SessionPause, &TimeRemaining,
    "All valid" }
};

/* Structure for test cases */
typedef struct _OMADMSGetPendingNIATestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             *pSessionType;
    USHORT            *pSessionID;
    CHAR              desc[100];
} OMADMSGetPendingNIATestCase_t;

/* Define the test cases to be executed */
ULONG  SessionType;
USHORT SessionId;

const OMADMSGetPendingNIATestCase_t \
OMADMSGetPendingNIATestCases[MAX_OMADMS_GET_PENDING_NIA_TESTCASE_NUM]=
{
    {eQCWWAN_ERR_INVALID_ARG,1,NULL,NULL,"Invalid Arguments"},
    {eQCWWAN_ERR_INVALID_ARG,2,NULL,&SessionId,"Invalid Arguments"},
    {eQCWWAN_ERR_NONE,3,&SessionType,&SessionId,"All Valid"}
};

#endif /* __GOBI_API_OMADM_TEST_H__ */
