/**************
 *
 *  Filename: qaGobiApiSwiOmadmsTest.h
 *
 *  Purpose:  Contains prototypes for functions implementing specific tests
 *            for Open Mobile Alliance Device Management Service (OMADMS)
 *            called by the main() routine in qatest.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc., all rights reserved
 *
 **************/

#ifndef __GOBI_API_SWIOMADMS_TEST_H__
#define __GOBI_API_SWIOMADMS_TEST_H__

/* Defines */
#define MAX_SLQS_OMADM_START_SESSION_TESTCASE_NUM    0x02
#define MAX_SLQS_OMADM_CANCEL_SESSION_TESTCASE_NUM   0x02
#define MAX_SLQS_OMADM_GET_SESSION_INFO_TESTCASE_NUM 0x03
#define MAX_SLQS_OMADM_SEND_SELECTION_TESTCASE_NUM   0x03
#define MAX_SLQS_OMADM_GET_SETTINGS_TESTCASE_NUM     0x08
#define MAX_SLQS_OMADM_SET_SETTINGS_TESTCASE_NUM     0x06

/* Structure for test cases step */
typedef struct _SLQSOMADMStartSessionTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             sessiontype;
    CHAR              desc[100];
} SLQSOMADMStartSessionTestCase_t;

typedef struct _SLQSOMADMCancelSessionTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             session;
    CHAR              desc[100];
} SLQSOMADMCancelSessionTestCase_t;

typedef struct _SLQSOMADMGetSessionInfoTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             *psessionType;
    ULONG             *psessionState;
    CHAR              desc[100];
} SLQSOMADMGetSessionInfoTestCase_t;

typedef struct _SLQSOMADMSendSelectionTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             selection;
    CHAR              desc[100];
} SLQSOMADMSendSelectionTestCase_t;

typedef struct _SLQSOMADMGetSettingsTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             *pbOMADMEnabled;
    ULONG             *pbFOTADownload;
    ULONG             *pbFOTAUpdate;
    CHAR              desc[100];
} SLQSOMADMGetSettingsTestCase_t;

typedef struct _SLQSOMADMSetSettingsTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             bFOTADownload;
    ULONG             bFOTAUpdate;
    CHAR              desc[100];
} SLQSOMADMSetSettingsTestCase_t;

typedef struct _SLQSOMADMSetSettings2TestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              pFOTADownload;
    BYTE              pFOTAUpdate;
    BYTE             *pAutoSdm;
    CHAR              desc[100];
} SLQSOMADMSetSettings2TestCase_t;

typedef struct _SLQSOMADMGetSettings2TestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             *pOMADMEnabled;
    BYTE              *pFOTADownload;
    BYTE              *pFOTAUpdate;
    BYTE              *pAutoSdm;
    CHAR              desc[100];
} SLQSOMADMGetSettings2TestCase_t;

/* Define the test cases to be executed */
const SLQSOMADMStartSessionTestCase_t \
    SLQSOMADMStartSessionTestCases[MAX_SLQS_OMADM_START_SESSION_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 0x00, "Invalid SessionType" },
    { eQCWWAN_ERR_NONE,        2, 0x01, "All valid" }
};

const SLQSOMADMCancelSessionTestCase_t \
    SLQSOMADMCancelSessionTestCases[
                               MAX_SLQS_OMADM_CANCEL_SESSION_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 0x00, "Invalid SessionType" },
    { eQCWWAN_ERR_NONE,        2, 0x01, "All valid" }
};

ULONG sessionTypeInvalid = 0x00;
ULONG sessionTypeFOTA    = 0x01;
ULONG sessionState;
const SLQSOMADMGetSessionInfoTestCase_t \
    SLQSOMADMGetSessionInfoTestCases[
                               MAX_SLQS_OMADM_GET_SESSION_INFO_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, &sessionTypeFOTA, NULL,
      "OUT parameter NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 2, &sessionTypeInvalid, NULL,
      "Invalid SessionType" },
    { eQCWWAN_ERR_NONE,        3, &sessionTypeFOTA, &sessionState,
      "All valid" }
};

const SLQSOMADMSendSelectionTestCase_t \
    SLQSOMADMSendSelectionTestCases\
        [MAX_SLQS_OMADM_SEND_SELECTION_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 0x13, "Invalid value: User Selection" },
    { eQCWWAN_ERR_NONE,        2, 0x01, "All Valid: Accept" },
    { eQCWWAN_ERR_NONE,        3, 0x02, "All Valid: Reject" }
};

ULONG bOMADMEnabled;
ULONG bFOTADownload;
ULONG bFOTAUpdate;

const SLQSOMADMGetSettingsTestCase_t \
    SLQSOMADMGetSettingsTestCases[MAX_SLQS_OMADM_GET_SETTINGS_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, NULL, NULL, NULL,\
        "Sending all OUT param pointers as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 2, &bOMADMEnabled, NULL, NULL,\
        "Sending two OUT param pointers as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 3, NULL, &bFOTADownload, NULL,\
        "Sending two OUT param pointers as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 4, NULL, NULL, &bFOTAUpdate,\
        "Sending two OUT param pointers as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 5, &bOMADMEnabled, &bFOTADownload, NULL,\
        "Sending an OUT param pointer as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 6, NULL, &bFOTADownload, &bFOTAUpdate,\
        "Sending an OUT param pointer as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 7, &bOMADMEnabled, NULL, &bFOTAUpdate,\
        "Sending an OUT param pointer as NULL" },
    { eQCWWAN_ERR_NONE, 8, &bOMADMEnabled, &bFOTADownload, &bFOTAUpdate,\
        "All valid" }
};

const SLQSOMADMSetSettingsTestCase_t \
    SLQSOMADMSetSettingsTestCases[MAX_SLQS_OMADM_SET_SETTINGS_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, 0x13, 0x00,\
          "Sending invalid value for Automatic Download" },
    { eQCWWAN_ERR_INVALID_ARG, 2, 0x00, 0x23,\
          "Sending invalid value for Automatic Update" },
    { eQCWWAN_ERR_NONE, 3, 0x00, 0x00, "All Valid" },
    { eQCWWAN_ERR_NONE, 4, 0x00, 0x01, "All Valid" },
    { eQCWWAN_ERR_NONE, 5, 0x01, 0x00, "All Valid" },
    { eQCWWAN_ERR_NONE, 6, 0x01, 0x01, "All Valid" },
};

#endif /* __GOBI_API_SWIOMADMS_TEST_H__ */
