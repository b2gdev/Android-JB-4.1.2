/**************
 *
 *  Filename: qaGobiApiRmsTest.h
 *
 *  Purpose:  Remote Management Service (RMS) API test driver function prototypes
 *
 * Copyright: © 2010 Sierra Wireless, Inc., all rights reserved
 *
 **************/

#ifndef __GOBI_API_RMS_TEST_H__
#define __GOBI_API_RMS_TEST_H__

/* Defines */
#define MAX_RMS_GET_SMS_WAKE_TESTCASE_NUM 0x03
#define MAX_RMS_SET_SMS_WAKE_TESTCASE_NUM 0x02

/* Structure for test cases step */
typedef struct _GetSMSWakeTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             *pEnabled;
    ULONG             *pWakeMask;
    CHAR              desc[100];
} GetSMSWakeTestCase_t;

typedef struct _SetSMSWakeTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             bEnable;
    ULONG             wakeMask;
    CHAR              desc[100];
} SetSMSWakeTestCase_t;

/* Define the test cases to be executed */

ULONG enabled, wakeMask;

/* Define the test cases to be executed */
const GetSMSWakeTestCase_t \
      GetSMSWakeTestCases[ MAX_RMS_GET_SMS_WAKE_TESTCASE_NUM ] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, &enabled,      NULL, "Invalid Parameter" },
    { eQCWWAN_ERR_INVALID_ARG, 2,     NULL, &wakeMask, "Invalid Parameter" },
    { eQCWWAN_ERR_NONE,        3, &enabled, &wakeMask, "All valid" }
};

const SetSMSWakeTestCase_t \
      SetSMSWakeTestCases[MAX_RMS_SET_SMS_WAKE_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 4, 0, "Invalid argument" },
    { eQCWWAN_ERR_NONE,        2, 1, 10, "All valid" },
};

#endif /* __GOBI_API_RMS_TEST_H__ */
