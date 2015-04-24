/**************
 *
 *  Filename: qaGobiApiSarTest.h
 *
 *  Purpose:  Contains prototypes for functions implementing specific tests
 *            for Specific Absorption Rate (SAR)
 *            called by the main() routine in qatest.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc., all rights reserved
 *
 **************/

#ifndef __GOBI_API_SAR_TEST_H__
#define __GOBI_API_SAR_TEST_H__

/* Defines */
#define MAX_SAR_RF_GET_STATE_TESTCASE_NUM    0x02
#define MAX_SAR_RF_SET_STATE_TESTCASE_NUM    0x03

/* Structure for test cases step */
typedef struct _SARRFGetStateTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             *pSarrfstate;
    CHAR              desc[100];
} SARRFGetStateTestCase_t;

typedef struct _SARRFSetStateTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             rfsarstate;
    CHAR              desc[100];
} SARRFSetStateTestCase_t;

/* Define the test cases to be executed */
ULONG sarrfstate;

const SARRFGetStateTestCase_t \
    SARRFGetStateTestCases[MAX_SAR_RF_GET_STATE_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE,        1, &sarrfstate, "Valid case"},
    { eQCWWAN_ERR_INVALID_ARG, 2, NULL,        "Invalid Case"}
};

const SARRFSetStateTestCase_t \
    SARRFSetStateTestCases[MAX_SAR_RF_SET_STATE_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, QMI_SAR_RF_STATE_DEFAULT,
      "QMI_SAR_RF_STATE_DEFAULT" },
    { eQCWWAN_ERR_NONE, 2, QMI_SAR_RF_STATE_4,
      "QMI_SAR_RF_STATE_4" },
    { eQCWWAN_ERR_INVALID_ARG, 3, -1,
      "Invalid state" }
};

#endif /* __GOBI_API_SAR_TEST_H__ */
