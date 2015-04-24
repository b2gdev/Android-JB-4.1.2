/**************
 *
 *  Filename: qaGobiApiDMSTest.h
 *
 *  Purpose:  Contains prototypes for functions implementing specific
 *            tests for Device Management Service (DMS)
 *            called by the main() routine in qatest.c
 *
 * Copyright: © 2010 Sierra Wireless, Inc., all rights reserved
 *
 **************/

#ifndef __GOBI_API_DMS_TEST_H__
#define __GOBI_API_DMS_TEST_H__

/* Defines */
#define nMaxStrLen                              0xFF
#define MAX_GET_MANUFACTURER_TESTCASE_NUM       0x01
#define MAX_GET_MODEL_ID_TESTCASE_NUM           0x01
#define MAX_GET_FIRMWARE_REVISION_TESTCASE_NUM  0x01
#define MAX_GET_FIRMWARE_REVISIONS_TESTCASE_NUM 0x01
#define MAX_GET_PRL_VERSION_TESTCASE_NUM        0x01
#define MAX_GET_IMSI_TESTCASE_NUM               0x01
#define MAX_GET_SERIAL_NUMBERS_TESTCASE_NUM     0x01
#define MAX_GET_HARDWARE_REVISION_TESTCASE_NUM  0x01
#define MAX_GET_NETWORK_TIME_TESTCASE_NUM       0x04
#define MAX_UIM_SET_PIN_PROTECTION_TESTCASE_NUM 0x08
#define MAX_UIM_VERIFY_PIN_TESTCASE_NUM         0x04
#define MAX_UIM_UNBLOCK_PIN_TESTCASE_NUM        0x01
#define MAX_UIM_CHANGE_PIN_TESTCASE_NUM         0x05
#define MAX_GET_VOICE_NUMBER_TESTCASE_NUM       0x05
#define MAX_UIM_GET_CK_STATUS_TESTCASE_NUM      0x07
#define MAX_UIM_GET_ICCID_TESTCASE_NUM          0x03
#define MAX_UIM_GET_PIN_STATUS_TESTCASE_NUM     0x05
#define MAX_UIM_SET_CK_PROTECTION_TESTCASE_NUM  0x09
#define MAX_GET_OFFLINE_REASON_TESTCASE_NUM     0x04
#define MAX_UIM_UNBLOCK_CK_TESTCASE_NUM         0x03
#define MAX_DEVICE_CAP_TESTCASE_NUM             0x02
#define MAX_RESET_FACTORY_DEFAULTS_TESTCASE_NUM 0x02
#define MAX_VALIDATE_SPC_TESTCASE_NUM           0x01
#define MAX_ACTIVATE_AUTOMATIC_TESTCASE_NUM     0x01
#define MAX_GET_ACTIVATION_STATE_TESTCASE_NUM   0x01
#define MAX_UIM_GET_STATE_TESTCASE_NUM          0x01
#define MAX_GET_BANDCAPABLTY_TESTCASE_NUM       0x02

/* Structure for test cases step */
typedef struct _GetManufacturerTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetManufacturerTestCase_t;

typedef struct _GetModelIDTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetModelIDTestCase_t;

typedef struct _GetFirmwareRevisionTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetFirmwareRevisionTestCase_t;

typedef struct _GetFirmwareRevisionsTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetFirmwareRevisionsTestCase_t;

typedef struct _GetPRLVersionTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetPRLVersionTestCase_t;

typedef struct _GetIMSITestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetIMSITestCase_t;

typedef struct _GetSerialNumbersTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetSerialNumbersTestCase_t;

typedef struct _GetHardwareRevisionTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetHardwareRevisionTestCase_t;

typedef struct _GetNetworkTimeTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG           testCaseId;
    ULONGLONG       *timeStamp;
    ULONG           *timeSource;
    CHAR            desc[100];
} GetNetworkTimeTestCase_t;

typedef struct _UIMSetPINProtectionTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             id;
    ULONG             bEnable;
    CHAR              value[nMaxStrLen + 1];
    ULONG             *pVerifyRetries;
    ULONG             *pUnblockRetries;
    CHAR              desc[100];
} UIMSetPINProtectionTestCase_t;

typedef struct _UIMUnblockPINTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             id;
    CHAR              pukValue[nMaxStrLen + 1];
    CHAR              newValue[nMaxStrLen + 1];
    CHAR              desc[100];
} UIMUnblockPINTestCase_t;

typedef struct _UIMVerifyPINTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             id;
    CHAR              value[nMaxStrLen + 1];
    CHAR              desc[100];
} UIMVerifyPINTestCase_t;

typedef struct _UIMChangePINTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             id;
    CHAR              oldValue[nMaxStrLen + 1];
    CHAR              newValue[nMaxStrLen + 1];
    CHAR              desc[100];
} UIMChangePINTestCase_t;

typedef struct _GetVoiceNumberTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG    testCaseId;
    BYTE     voiceNumberSize;
    CHAR     *pVoiceNumber;
    BYTE     minSize;
    CHAR     *pMIN;
    CHAR     desc[100];
} GetVoiceNumberTestCase_t;

typedef struct _SetPowerTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             powerMode;
    CHAR              desc[100];
} SetPowerTestCase_t;

typedef struct _GetPowerTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetPowerTestCase_t;

typedef struct _UIMGetCKStatusTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             id;
    CHAR              desc[100];
} UIMGetCKStatusTestCase_t;

typedef struct _UIMGetICCIDTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              strSize;
    CHAR              *pString;
    CHAR              desc[100];
} UIMGetICCIDTestCase_t;

typedef struct _UIMGetPINStatusTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             id;
    ULONG             *pStatus;
    CHAR              desc[100];
} UIMGetPINStatusTestCase_t;

typedef struct _GetOfflineReasonTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG   testCaseId;
    ULONG   *ReasonMask;
    ULONG   *Platform;
    CHAR    desc[100];
} GetOfflineReasonTestCase_t;

#define MAX_CK_VALUE_LEN 8
typedef struct _UIMSetCkProtectionTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             id;
    ULONG             status;
    CHAR              value[MAX_CK_VALUE_LEN + 1];
    CHAR              desc[100];
} UIMSetCkProtectionTestCase_t;

typedef struct _UIMUnblockCkTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             id;
    CHAR              value[MAX_CK_VALUE_LEN + 1];
    CHAR              desc[100];
} UIMUnblockCkTestCase_t;

typedef struct _GetDeviceCapabilitiesTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             radioIfacesSize;
    CHAR              desc[100];
} GetDeviceCapabilitiesTestCase_t;

typedef struct _ResetFactoryDefaultsTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              pSPC[7];
    CHAR              desc[100];
} ResetFactoryDefaultsTestCase_t;

typedef struct _ValidateSpcTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              pSPC[7];
    CHAR              desc[100];
} ValidateSpcTestCase_t;

typedef struct _ActivateAutomaticTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              activationCode[14];
    CHAR              desc[100];
} ActivateAutomaticTestCase_t;

typedef struct _GetActivationStateTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetActivationStateTestCase_t;

typedef struct _UIMGetStateTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} UIMGetStateTestCase_t;

typedef struct _GetBandCapabilityTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONGLONG         *pBandCapability;
    CHAR              desc[100];
} GetBandCapabilityTestCase_t;

typedef struct _SLQSGetCustFeaturesTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    custFeaturesInfo  *pCustFeaturesInfo;
    CHAR              desc[100];
} SLQSGetCustFeaturesTestCase_t;

typedef struct _SLQSSetCustFeaturesTestCase_t
{
    enum eQCWWANError   expectedReturnValue;
    ULONG               testCaseId;
    custFeaturesSetting *pCustFeaturesSetting;
    CHAR                desc[100];
} SLQSSetCustFeaturesTestCase_t;

typedef struct _SLQSGetCurrentPRLInfo_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    WORD              *pPRLVersion;
    BYTE              *pPRLPreference;
    CHAR              desc[100];
} SLQSGetCurrentPRLInfo_t;

/* Define the test cases to be executed */
const GetManufacturerTestCase_t \
      GetManufacturerTestCases[MAX_GET_MANUFACTURER_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 2, "ALL Valid"}
};

const GetModelIDTestCase_t \
      GetModelIDTestCases[MAX_GET_MODEL_ID_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 2, "ALL Valid"}
};

const GetFirmwareRevisionTestCase_t \
      GetFirmwareRevisionTestCases[MAX_GET_FIRMWARE_REVISION_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 2, "ALL Valid"}
};

const GetFirmwareRevisionsTestCase_t \
      GetFirmwareRevisionsTestCases[MAX_GET_FIRMWARE_REVISIONS_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 2, "ALL Valid"}
};

const GetPRLVersionTestCase_t \
      GetPRLVersionTestCases[MAX_GET_PRL_VERSION_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 2, "ALL Valid"}
};

const GetIMSITestCase_t \
      GetIMSITestCases[MAX_GET_IMSI_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 2, "ALL Valid"}
};

const GetSerialNumbersTestCase_t \
      GetSerialNumbersTestCases[MAX_GET_SERIAL_NUMBERS_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 2, "ALL Valid"}
};

const GetHardwareRevisionTestCase_t \
      GetHardwareRevisionTestCases[MAX_GET_HARDWARE_REVISION_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 2, "ALL Valid"}
};

ULONGLONG  timeStamp;
ULONG      timeSource;

const GetNetworkTimeTestCase_t \
      GetNetworkTimeTestCases[MAX_GET_NETWORK_TIME_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, NULL,       NULL,        "All Parameters Invalid"},
    {eQCWWAN_ERR_NONE, 2, NULL,       &timeSource, "Invalid timeStamp"},
    {eQCWWAN_ERR_NONE, 3, &timeStamp, NULL,        "Invalid timeSource"},
    {eQCWWAN_ERR_NONE, 4, &timeStamp, &timeSource, "Valid Device Time"}
};

ULONG verifyRetries;
ULONG unblockRetries;

const UIMSetPINProtectionTestCase_t \
      UIMSetPINProtectionTestCases[MAX_UIM_SET_PIN_PROTECTION_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_INVALID_ARG, 1, 3, 1, "1234", &verifyRetries, &unblockRetries,
    "Invalid PIN id "},
    {eQCWWAN_ERR_INVALID_ARG, 1, 1, 1, "1234", NULL, NULL,
    "Invalid Verify and Unblock Retries"},
    {eQCWWAN_ERR_INVALID_ARG, 1, 1, 1, "1234", NULL, &unblockRetries,
    "Invalid Verify Retries"},
    {eQCWWAN_ERR_INVALID_ARG, 1, 1, 1, "1234", &verifyRetries, NULL,
    "Invalid Unblock Retries"},
    {eQCWWAN_ERR_NONE,        1, 1, 0, "1234", &verifyRetries, &unblockRetries,
    "Disable Without Enabling"},
    {eQCWWAN_ERR_NONE,        1, 1, 1, "1234", &verifyRetries, &unblockRetries,
    "Enable PIN"},
    {eQCWWAN_ERR_NONE,        1, 1, 1, "1234", &verifyRetries, &unblockRetries,
    "Enable PIN already enabled"},
    {eQCWWAN_ERR_NONE,        1, 1, 0, "1234", &verifyRetries, &unblockRetries,
    "Disable PIN After Enabled"}
};

const UIMUnblockPINTestCase_t \
      UIMUnblockPINTestCases[MAX_UIM_UNBLOCK_PIN_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, 1, "GOBI123", "GOBI456", "ALL Valid"}
};

const UIMVerifyPINTestCase_t \
      UIMVerifyPINTestCases[MAX_UIM_VERIFY_PIN_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, 3, "1234", "Invalid PIN ID - Should return 1011"},
    {eQCWWAN_ERR_NONE, 1, 0, "1234", "Invalid PIN ID - Should return 1011"},
    {eQCWWAN_ERR_NONE, 1, 1, "1234", "Valid PIN - Set 1234 before the test"},
    {eQCWWAN_ERR_NONE, 1, 1, "3421", "Invalid PIN"}
};

const UIMChangePINTestCase_t \
      UIMChangePINTestCases[MAX_UIM_CHANGE_PIN_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, 0, "1234", "1234", "Error PIN ID Lower Limit : 1011"},
    {eQCWWAN_ERR_NONE, 1, 0, "1234", "1234", "Error PIN ID Upper Limit : 1011"},
    {eQCWWAN_ERR_NONE, 1, 1, "6666", "7777", "Error PIN Value : 1012"},
    {eQCWWAN_ERR_NONE, 1, 1, "1234", "4567", "ALL Valid"},
    {eQCWWAN_ERR_NONE, 1, 1, "4567", "1234", "ALL Valid"}
};


CHAR    pVoiceNumber[20];
CHAR    pMIN[20];

const GetVoiceNumberTestCase_t \
      GetVoiceNumberTestCases[MAX_GET_VOICE_NUMBER_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, 0,  NULL,         20, pMIN, "Invalid Voice Number"},
    {eQCWWAN_ERR_NONE, 2, 20, pVoiceNumber, 0,  NULL,   "Invalid MIN"},
    {eQCWWAN_ERR_NONE, 3, 3,  pVoiceNumber, 20, pMIN, "Buffer UnderSize : VN"},
    {eQCWWAN_ERR_NONE, 4, 20, pVoiceNumber, 3,  pMIN, "Buffer UnderSize : MIN"},
    {eQCWWAN_ERR_NONE, 5, 20, pVoiceNumber, 20, pMIN, "Valid Paramaters"},
};

/* To be used by all Control Key related APIs */
#define QMI_DMS_PN 0x00 /* Network Personalization */
#define QMI_DMS_PU 0x01 /* Network sUbset Personalization */
#define QMI_DMS_PP 0x02 /* Service Provider Personalization */
#define QMI_DMS_PC 0x03 /* Corporate Personalization */
#define QMI_DMS_PF 0x04 /* UIM Personalization */

const UIMGetCKStatusTestCase_t \
      UIMGetCKStatusTestCases[MAX_UIM_GET_CK_STATUS_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, -1,         "Invalid Parameter : Lower Limit"},
    {eQCWWAN_ERR_NONE, 1, 5,         "Invalid Parameter : Upper Limit"},
    {eQCWWAN_ERR_NONE, 1, QMI_DMS_PN, "Valid : QMI_DMS_PN"},
    {eQCWWAN_ERR_NONE, 1, QMI_DMS_PU, "Valid : QMI_DMS_PU"},
    {eQCWWAN_ERR_NONE, 1, QMI_DMS_PP, "Valid : QMI_DMS_PP"},
    {eQCWWAN_ERR_NONE, 1, QMI_DMS_PC, "Valid : QMI_DMS_PC"},
    {eQCWWAN_ERR_NONE, 1, QMI_DMS_PF, "Valid : QMI_DMS_PF"},
};

CHAR pString[nMaxStrLen];

const UIMGetICCIDTestCase_t \
      UIMGetICCIDTestCases[MAX_UIM_GET_ICCID_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, nMaxStrLen, NULL,    "Parameter Invalid"},
    {eQCWWAN_ERR_NONE, 1, 4,          pString, "Buffer UnderSize Error"},
    {eQCWWAN_ERR_NONE, 1, nMaxStrLen, pString, "ALL Valid"}
};

ULONG Status;
const UIMGetPINStatusTestCase_t \
      UIMGetPINStatusTestCases[MAX_UIM_GET_PIN_STATUS_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, 1, NULL,    "InValid Parameter"},
    {eQCWWAN_ERR_NONE, 1, 0, &Status, "InValid Pin ID : Lower Limit Check"},
    {eQCWWAN_ERR_NONE, 1, 3, &Status, "InValid Pin ID : Upper Limit Check"},
    {eQCWWAN_ERR_NONE, 1, 1, &Status, "Valid Pin ID : 1"},
    {eQCWWAN_ERR_NONE, 1, 2, &Status, "Valid Pin ID : 2"},
};

ULONG ReasonMask = 0;
ULONG Platform = 0;

const GetOfflineReasonTestCase_t \
      GetOfflineReasonTestCases[MAX_GET_OFFLINE_REASON_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, NULL,         NULL,       "Invalid Parameters"},
    {eQCWWAN_ERR_NONE, 1, &ReasonMask,  NULL,       "Platform Invalid"},
    {eQCWWAN_ERR_NONE, 1, NULL,         &Platform,  "Reason Mask Invalid"},
    {eQCWWAN_ERR_NONE, 1, &ReasonMask,  &Platform,  "ALL Valid"}
};

const UIMSetCkProtectionTestCase_t \
      UIMSetCkProtectionTestCases[MAX_UIM_SET_CK_PROTECTION_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, -1,         0,  "GOBI3000", "Invalid ID : Lower"},
    {eQCWWAN_ERR_NONE, 2, 5,          0,  "GOBI3000", "Invalid ID"},
    {eQCWWAN_ERR_NONE, 3, QMI_DMS_PN, -1, "GOBI3000", "Invalid State"},
    {eQCWWAN_ERR_NONE, 4, QMI_DMS_PN, 1,  "GOBI3000", "Invalid State"},
    {eQCWWAN_ERR_NONE, 5, QMI_DMS_PN, 0,  "GOBI3000", "ALL Valid"},
    {eQCWWAN_ERR_NONE, 6, QMI_DMS_PU, 0,  "GOBI3000", "ALL Valid"},
    {eQCWWAN_ERR_NONE, 7, QMI_DMS_PP, 0,  "GOBI3000", "ALL Valid"},
    {eQCWWAN_ERR_NONE, 8, QMI_DMS_PC, 0,  "GOBI3000", "ALL Valid"},

    {eQCWWAN_ERR_NONE, 9, QMI_DMS_PF, 0,  "GOBI3000", "ALL Valid"},
};

const UIMUnblockCkTestCase_t \
      UIMUnblockCkTestCases[MAX_UIM_UNBLOCK_CK_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, -1, "GOBI3000", "Invalid ID : Lower Limit"},
    {eQCWWAN_ERR_NONE, 2, -5, "GOBI3000", "Invalid ID : Upper Limit"},
    {eQCWWAN_ERR_NONE, 3, 1, "GOBI3000", "ALL Valid"}
};

const GetDeviceCapabilitiesTestCase_t \
      GetDeviceCapabilitiesTestCases[MAX_DEVICE_CAP_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, nMaxStrLen -1, "Valid data Capabilities"},
    {eQCWWAN_ERR_NONE, 2, 0, "invalid : buffer undersize error"}

};

const ResetFactoryDefaultsTestCase_t \
      ResetFactoryDefaultsTestCases[MAX_RESET_FACTORY_DEFAULTS_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "123456", "Invalid SPC"},
    {eQCWWAN_ERR_NONE, 1, "000000", "Valid SPC"}
};

const ValidateSpcTestCase_t \
      ValidateSpcTestCases[MAX_VALIDATE_SPC_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "123456", "ALL Valid"}
};

const ActivateAutomaticTestCase_t \
      ActivateAutomaticTestCases[MAX_ACTIVATE_AUTOMATIC_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "8GOBI3000", "Valid Activation Code"}
};

const GetActivationStateTestCase_t \
      GetActivationStateTestCases[MAX_GET_ACTIVATION_STATE_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "ALL Valid"}
};

const UIMGetStateTestCase_t \
      UIMGetStateTestCases[MAX_UIM_GET_STATE_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "ALL Valid"}
};

ULONGLONG bandCapability;
const GetBandCapabilityTestCase_t \
      GetBandCapabilityTestCases[MAX_GET_BANDCAPABLTY_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, NULL,            "Invalid Argument"},
    {eQCWWAN_ERR_NONE, 2, &bandCapability, "ALL Valid"}
};

#endif /* __GOBI_API_DMS_TEST_H__ */
