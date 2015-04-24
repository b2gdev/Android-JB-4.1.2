/**************
 *
 *  Filename: qaGobiApiNasTest.h
 *
 *  Purpose:  Contains prototypes for functions implementing specific
 *            tests for Network Access Service (NAS)
 *            called by the main() routine in qatest.c
 *
 * Copyright: © 2010-2012 Sierra Wireless, Inc., all rights reserved
 *
 **************/

#ifndef __GOBI_API_NAS_TEST_H__
#define __GOBI_API_NAS_TEST_H__

/* Defines */
#define nMaxStrLen                                   0xFF
#define MAX_GET_HOME_NETWORK_TESTCASE_NUM            0x01
#define MAX_GET_SERVING_SYSTEM_TESTCASE_NUM          0x01
#define MAX_GET_SIGNAL_STRENGTHS_TESTCASE_NUM        0x01
#define MAX_INIT_NETWORK_REG_NUM                     0x02
#define MAX_PERFORM_NETWORK_SCAN_TESTCASE_NUM        0x01
#define QMI_NAS_AUTOMATIC_REGISTRATION               0x01
#define QMI_NAS_MANUAL_REGISTRATION                  0x02
#define MAX_GET_SERVING_NETWORK_CAPS_TESTCASE_NUM    0x01
#define MAX_SET_NETWORK_PREFERENCE_TESTCASE_NUM      0x14
#define MAX_GET_NETWORK_PREFERENCE_TESTCASE_NUM      0x01
#define MAX_GET_RF_INFO_TESTCASE_NUM                 0x01
#define MAX_INITIATE_DOMAIN_ATTACH_TESTCASE_NUM      0x06
#define MAX_GET_ACCOLC_TESTCASE_NUM                  0x01
#define MAX_SET_ACCOLC_TESTCASE_NUM                  0x01
#define MAX_SET_CDMA_NETWORK_PARAMETERS_TESTCASE_NUM 0x01
#define MAX_GET_CDMA_NETWORK_PARAMETERS_TESTCASE_NUM 0x01
#define MAX_GET_ANAAA_AUTH_TESTCASE_NUM              0x01
#define MAX_GET_CDMA_SERVING_SYSTEM_TESTCASE_NUM     0x01
#define MAX_SLQS_SET_BAND_PREFERENCE_TESTCASE_NUM    0x03
#define MAX_SLQS_INDICATION_REGISTER_TESTCASE_NUM    0x03
#define MAX_SLQS_GET_SIGNAL_STRENGTH_TESTCASE_NUM    0x02
#define MAX_SLQS_PERFORM_NETWORK_SCAN_TESTCASE_NUM   0x03
#define FALSE 0
#define TRUE !FALSE

/* Structure for test cases step */
typedef struct _GetHomeNetworkTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              nameSize;
    CHAR              desc[100];
} GetHomeNetworkTestCase_t;

typedef struct _GetServingSystemTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             pRadioIfacesSize;
    BYTE              nameSize;
    CHAR              desc[100];
} GetServingSystemTestCase_t;

typedef struct _GetSignalStrengthsTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             pArraySizes;
    CHAR              desc[100];
} GetSignalStrengthsTestCase_t;

typedef struct _InitNetworkRegTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             regType;
    WORD              mcc;
    WORD              mnc;
    ULONG             rat;
    CHAR              desc[100];
} InitNetworkRegTestCase_t;

typedef struct _PerformNetworkScanTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              pInstanceSize;
    CHAR              desc[100];
} PerformNetworkScanTestCase_t;

typedef struct _GetServingNetworkCapsTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              DataCapsSize;
    CHAR              desc[100];
} GetServingNetworkCapsTestCase_t;

typedef struct _SetNetworkPreferenceTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             techpref;
    ULONG             duration;
    CHAR              desc[100];
} SetNetworkPreferenceTestCase_t;

typedef struct _GetNetworkPreferenceTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetNetworkPreferenceTestCase_t;

typedef struct _GetRFInfoTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              InstanceSize;
    CHAR              desc[100];
} GetRFInfoTestCase_t;

typedef struct _InitiateDomainAttachTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             action;
    CHAR              desc[100];
} InitiateDomainAttachTestCase_t;

typedef struct _GetACCOLCTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetACCOLCTestCase_t;

typedef struct _SetACCOLCTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              SPC[6];
    BYTE              accolc;
    CHAR              desc[100];
} SetACCOLCTestCase_t;

typedef struct _SetCDMANetworkParametersTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              spc[6];
    BYTE              forcerev0;
    BYTE              customSCP;
    ULONG             protocol;
    ULONG             broadcast;
    ULONG             application;
    ULONG             roaming;
    CHAR              desc[100];
} SetCDMANetworkParametersTestCase_t;

typedef struct _GetCDMANetworkParametersTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetCDMANetworkParametersTestCase_t;

typedef struct _GetANAAAAuthStatusTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetANAAAAuthStatusTestCase_t;

typedef struct _SLQSGetServingSystemTestCase_t
{
    enum eQCWWANError       expectedReturnValue;
    ULONG                   testCaseId;
    qaQmiServingSystemParam *pServingSystemInfo;
    CHAR                    desc[100];
} SLQSGetServingSystemTestCase_t;

typedef struct _SLQSSetBandPreferenceTestCase_t
{
    enum      eQCWWANError expectedReturnValue;
    ULONG                  testCaseId;
    ULONGLONG              bandpreference;
    CHAR                   desc[100];
} SLQSSetBandPreferenceTestCase_t;

typedef struct _SLQSIndicationRegisterTestCase_t
{
    enum  eQCWWANError expectedReturnValue;
    ULONG                  testCaseId;
    BYTE                   systemSelectionInd;
    BYTE                   DDTMInd;
    BYTE                   servingSystemInd;
    CHAR                   desc[100];
}SLQSIndicationRegisterTestCase_t;

typedef struct _SLQSGetSignalStrengthTestCase_t
{
    enum eQCWWANError             expectedReturnValue;
    ULONG                         testCaseId;
    struct slqsSignalStrengthInfo *pSignalInfo;
    CHAR                          desc[100];
} SLQSGetSignalStrengthTestCase_t;

typedef struct _SLQSPerformNetworkScanTestCase_t
{
    enum   eQCWWANError           expectedReturnValue;
    ULONG                         testCaseId;
    BYTE                          *pNetworkInfoInstances;
    struct SlqsNas3GppNetworkInfo *pNetworkInfo;
    BYTE                          *pRATInstances;
    struct SlqsNas3GppNetworkRAT  *pRATInfo;
    CHAR                          desc[100];
} SLQSPerformNetworkScanTestCase_t;

typedef struct _SLQSSetSysSelectionPrefTestCase_t
{
    enum      eQCWWANError     expectedReturnValue;
    ULONG                      testCaseId;
    BYTE                       *pEmerMode;
    WORD                       *pModePref;
    ULONGLONG                  *pBandPref;
    WORD                       *pPRLPref;
    WORD                       *pRoamPref;
    ULONGLONG                  *pLTEBandPref;
    struct    netSelectionPref *pNetSelPref;
    BYTE                       *pChgDuration;
    BYTE                       *pMNCIncPCSDigStat;
    ULONG                      *pSrvDomainPref;
    ULONG                      *pGWAcqOrderPref;
    CHAR                       desc[100];
} SLQSSetSysSelectionPrefTestCase_t;

typedef struct _SLQSGetSysSelectionPrefTestCase_t
{
    enum      eQCWWANError expectedReturnValue;
    ULONG                  testCaseId;
    BYTE                   *pEmerMode;
    WORD                   *pModePref;
    ULONGLONG              *pBandPref;
    WORD                   *pPRLPref;
    WORD                   *pRoamPref;
    ULONGLONG              *pLTEBandPref;
    BYTE                   *pNetSelPref;
    ULONG                  *pSrvDomainPref;
    ULONG                  *pGWAcqOrderPref;
    CHAR                   desc[100];
} SLQSGetSysSelectionPrefTestCase_t;

typedef struct _SLQSNasGetSysInfoTestCase_t
{
    enum eQCWWANError  expectedReturnValue;
    ULONG              testCaseId;
    SrvStatusInfo      *pCDMASrvStatusInfo;
    SrvStatusInfo      *pHDRSrvStatusInfo;
    GSMSrvStatusInfo   *pGSMSrvStatusInfo;
    GSMSrvStatusInfo   *pWCDMASrvStatusInfo;
    GSMSrvStatusInfo   *pLTESrvStatusInfo;
    CDMASysInfo        *pCDMASysInfo;
    HDRSysInfo         *pHDRSysInfo;
    GSMSysInfo         *pGSMSysInfo;
    WCDMASysInfo       *pWCDMASysInfo;
    LTESysInfo         *pLTESysInfo;
    AddCDMASysInfo     *pAddCDMASysInfo;
    WORD               *pAddHDRSysInfo;
    AddSysInfo         *pAddGSMSysInfo;
    AddSysInfo         *pAddWCDMASysInfo;
    WORD               *pAddLTESysInfo;
    CallBarringSysInfo *pGSMCallBarringSysInfo;
    CallBarringSysInfo *pWCDMACallBarringSysInfo;
    BYTE               *pLTEVoiceSupportSysInfo;
    BYTE               *pGSMCipherDomainSysInfo;
    BYTE               *pWCDMACipherDomainSysInfo;
    CHAR               desc[100];
} SLQSNasGetSysInfoTestCase_t;

typedef struct _SLQSNasSwiModemStatusTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    LTEInfo           *pLTEInfo;
    CHAR              desc[100];
} SLQSNasSwiModemStatusTestCase_t;

typedef struct _SLQSGetHDRColorCodeTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              *pColorCode;
    CHAR              desc[100];
} SLQSGetHDRColorCodeTestCase_t;

typedef struct _SLQSGetTxRxInfoTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              radio_if;
    rxInfo            *pRXChain0Info;
    rxInfo            *pRXChain1Info;
    txInfo            *pTXInfo;
    CHAR              desc[100];
} SLQSGetTxRxInfoTestCase_t;

typedef struct _SLQSGetSigInfoTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CDMASSInfo        *pCDMASSInfo;
    HDRSSInfo         *pHDRSSInfo;
    BYTE              *pGSMSSInfo;
    CDMASSInfo        *pWCDMASSInfo;
    LTESSInfo         *pLTESSInfo;
    CHAR              desc[100];
} SLQSGetSigInfoTestCase_t;

typedef struct _SLQSNasIndicationRegisterExtTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              *pSystemSelectionInd;
    BYTE              *pDDTMInd;
    BYTE              *pServingSystemInd;
    BYTE              *pDualStandByPrefInd;
    BYTE              *pSubscriptionInfoInd;
    BYTE              *pNetworkTimeInd;
    BYTE              *pSysInfoInd;
    BYTE              *pSignalStrengthInd;
    BYTE              *pErrorRateInd;
    BYTE              *pHDRNewUATIAssInd;
    BYTE              *pHDRSessionCloseInd;
    BYTE              *pManagedRoamingInd;
    CHAR              desc[100];
} SLQSNasIndicationRegisterExtTestCase_t;

typedef struct _SLQSGetPLMNNameTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    nasPLMNNameReq    *pPLMNNameReq;
    nasPLMNNameResp   *pPLMNNameResp;
    CHAR              desc[100];
} SLQSGetPLMNNameTestCase_t;

typedef struct _SLQSGetOperatorNameDataTestCase_t
{
    enum eQCWWANError   expectedReturnValue;
    ULONG               testCaseId;
    nasOperatorNameResp *pOperatorNameData;
    CHAR                desc[100];
} SLQSGetOperatorNameDataTestCase_t;


typedef struct _SLQSGet3GPP2SubscriptionTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              namID;
    namName           *pNAMNameInfo;
    dirNum            *pDirNum;
    homeSIDNID        *pHomeSIDNID;
    minBasedIMSI      *pMinBasedIMSI;
    trueIMSI          *pTrueIMSI;
    CDMAChannel       *pCDMAChannel;
    CHAR              desc[100];
} SLQSGet3GPP2SubscriptionTestCase_t;


typedef struct _SLQSNasGetCellLocationInfoTestCase_t
{
    enum eQCWWANError       expectedReturnValue;
    ULONG                   testCaseId;
    GERANInfo               *pGERANInfo;
    UMTSInfo                *pUMTSInfo;
    CDMAInfo                *pCDMAInfo;
    LTEInfoIntrafreq        *pLTEInfoIntrafreq;
    LTEInfoInterfreq        *pLTEInfoInterfreq;
    LTEInfoNeighboringGSM   *pLTEInfoNeighboringGSM;
    LTEInfoNeighboringWCDMA *pLTEInfoNeighboringWCDMA;
    CHAR                    desc[100];
} SLQSNasGetCellLocationInfoTestCase_t;

struct MNRegInfo
{
    WORD  mcc;
    WORD  mnc;
    ULONG rat;
};

typedef struct _SLQSNasInitNetworkRegTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             regAction;
    struct MNRegInfo  *pMNRegInfo;
    ULONG             *pChangeDuration;
    BOOL              *pMncPcsDigitStatus;
    CHAR              desc[100];
} SLQSNasInitNetworkRegTestCase_t;

/* Define the test cases to be executed */
const GetHomeNetworkTestCase_t \
      GetHomeNetworkTestCases[MAX_GET_HOME_NETWORK_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, nMaxStrLen - 1, "Valid GetHomeNetwork"}
};

const GetServingSystemTestCase_t \
      GetServingSystemTestCases[MAX_GET_SERVING_SYSTEM_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, nMaxStrLen - 1,nMaxStrLen - 1, "Valid GetServingSystem"}
};

const GetSignalStrengthsTestCase_t \
      GetSignalStrengthsTestCases[MAX_GET_SIGNAL_STRENGTHS_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, nMaxStrLen - 1, "Valid GetSignalStrengths"}
};

const InitNetworkRegTestCase_t \
InitNetworkRegTestCases[MAX_INIT_NETWORK_REG_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, QMI_NAS_MANUAL_REGISTRATION, 302, 220, 5 /* UMTS ACCESS TECHNOLOGY */,
     "Valid InitializeNetworkRegistration"},
    {eQCWWAN_ERR_NONE, 1, QMI_NAS_AUTOMATIC_REGISTRATION, 302, 220, 5 /* UMTS ACCESS TECHNOLOGY */,
     "Valid InitializeNetworkRegistration"}
};

const PerformNetworkScanTestCase_t \
PerformNetworkScanTestCases[MAX_PERFORM_NETWORK_SCAN_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, nMaxStrLen -1, "Valid PerformNetworkScan"}
};

const GetServingNetworkCapsTestCase_t \
GetServingNetworkCapsTestCases[MAX_GET_SERVING_NETWORK_CAPS_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, 8, "Valid GetServingNetworkCaps"}
};

/* Network Preference Test Case Parameters */
/* Technology Preference from Gopi 3000 Addentum */
#define TECH_3GPP2          0x01
#define TECH_3GPP           0x02
#define TECH_ANALOG_3GPP2   (0x04|TECH_3GPP2)
#define TECH_ANALOG_3GPP    (0x04|TECH_3GPP)
#define TECH_DIGITAL_3GPP2  (0x08|TECH_3GPP2)
#define TECH_DIGITAL_3GPP   (0x08|TECH_3GPP)
#define TECH_HDR_3GPP2      (0x10|TECH_3GPP2)
#define TECH_HDR_3GPP       (0x10|TECH_3GPP)

/* Duration Values from Gopi 3000 Addentum */
#define DURTN_PER           0x00
#define DURTN_PWR_CYCLE     0x01

const SetNetworkPreferenceTestCase_t \
SetNetworkPreferenceTestCases[MAX_SET_NETWORK_PREFERENCE_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, 0, 3, "Invalid Parameters : Duration > 1"},
    {eQCWWAN_ERR_NONE, 1, 0, 0, "Automatic Use by Modem : No bits set"},
    {eQCWWAN_ERR_NONE, 2, 0, 0, "Same parameters, return 1026 [No Effect]"},
    {eQCWWAN_ERR_NONE, 3, TECH_3GPP2, DURTN_PER,
        "3GPP2 :  Permenant Duration"},
    {eQCWWAN_ERR_NONE, 2, TECH_3GPP2, DURTN_PWR_CYCLE,
        "3GPP2 :  Power Cycle Duration"},
    {eQCWWAN_ERR_NONE, 3, TECH_3GPP, DURTN_PER,
        "3GPP :  Permenant Duration"},
    {eQCWWAN_ERR_NONE, 3, TECH_3GPP, DURTN_PWR_CYCLE,
        "3GPP :  Power Cycle Duration"},
    {eQCWWAN_ERR_NONE, 3, TECH_ANALOG_3GPP2, DURTN_PER,
        "TECH_ANALOG_3GPP2 :  Permenant Duration"},
    {eQCWWAN_ERR_NONE, 3, TECH_ANALOG_3GPP2, DURTN_PWR_CYCLE,
        "TECH_ANALOG_3GPP2 :  Power Cycle Duration"},
    {eQCWWAN_ERR_NONE, 3, TECH_ANALOG_3GPP, DURTN_PER,
        "TECH_ANALOG_3GPP :  Permenant Duration"},
    {eQCWWAN_ERR_NONE, 3, TECH_ANALOG_3GPP, DURTN_PWR_CYCLE,
        "TECH_ANALOG_3GPP :  Power Cycle Duration"},
    {eQCWWAN_ERR_NONE, 3, TECH_DIGITAL_3GPP2, DURTN_PER,
        "TECH_DIGITAL_3GPP2 :  Permenant Duration"},
    {eQCWWAN_ERR_NONE, 3, TECH_DIGITAL_3GPP2, DURTN_PWR_CYCLE,
        "TECH_DIGITAL_3GPP2 :  Power Cycle Duration"},
    {eQCWWAN_ERR_NONE, 3, TECH_DIGITAL_3GPP, DURTN_PER,
        "TECH_DIGITAL_3GPP :  Permenant Duration"},
    {eQCWWAN_ERR_NONE, 3, TECH_DIGITAL_3GPP, DURTN_PWR_CYCLE,
        "TECH_DIGITAL_3GPP :  Power Cycle Duration"},
    {eQCWWAN_ERR_NONE, 3, TECH_HDR_3GPP2, DURTN_PER,
        "TECH_HDR_3GPP2 :  Permenant Duration"},
    {eQCWWAN_ERR_NONE, 3, TECH_HDR_3GPP2, DURTN_PWR_CYCLE,
        "TECH_HDR_3GPP2 :  Power Cycle Duration"},
    {eQCWWAN_ERR_NONE, 3, TECH_HDR_3GPP, DURTN_PER,
        "TECH_HDR_3GPP :  Permenant Duration"},
    {eQCWWAN_ERR_NONE, 3, TECH_HDR_3GPP, DURTN_PWR_CYCLE,
        "TECH_HDR_3GPP2 :  Power Cycle Duration"},
    {eQCWWAN_ERR_NONE, 1, 0, 0, "Automatic Use by Modem : No bits set"}
};

const GetNetworkPreferenceTestCase_t \
GetNetworkPreferenceTestCases[MAX_GET_NETWORK_PREFERENCE_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "Valid GetNetworkPreference"}
};

const GetRFInfoTestCase_t \
GetRFInfoTestCases[MAX_GET_RF_INFO_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, nMaxStrLen -1, "Valid GetRFInfo"}
};

/* Network Attach Parameters */
/* Technology Preference QMI Guided */
#define DOMAIN_ATTACH   0x01
#define DOMAIN_DETACH   0x02
#define INVALID_ARG_0   0x00
#define INVALID_ARG_3   0x03

const InitiateDomainAttachTestCase_t \
InitiateDomainAttachTestCases[MAX_INITIATE_DOMAIN_ATTACH_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, INVALID_ARG_0, "Indalid Argument Lower limit test"},
    {eQCWWAN_ERR_NONE, 1, INVALID_ARG_3, "Invalid Argument Upper Limit Test"},
    {eQCWWAN_ERR_NONE, 1, DOMAIN_DETACH, "Valid InitiateDomainAttach : Detach"},
    {eQCWWAN_ERR_NONE, 1, DOMAIN_DETACH,
        "Valid InitiateDomainAttach : Detach Again"},
    {eQCWWAN_ERR_NONE, 1, DOMAIN_ATTACH, "Valid InitiateDomainAttach : Attach"},
    {eQCWWAN_ERR_NONE, 1, DOMAIN_ATTACH,
        "Valid InitiateDomainAttach : Attach Again"}
};

const GetACCOLCTestCase_t \
GetACCOLCTestCases[MAX_GET_ACCOLC_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "Valid GetACCOLC"}
};

const SetACCOLCTestCase_t \
SetACCOLCTestCases[MAX_SET_ACCOLC_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "12345", 1, "Valid SetACCOLC"}
};

const SetCDMANetworkParametersTestCase_t \
SetCDMANetworkParametersTestCases[MAX_SET_CDMA_NETWORK_PARAMETERS_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "12345", 1, 1, 0x00000001, 0x00000001, 0x00000001, 1, "Valid SetCDMANetworkParameters"}
};

const GetCDMANetworkParametersTestCase_t \
GetCDMANetworkParametersTestCases[MAX_GET_CDMA_NETWORK_PARAMETERS_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "Valid GetCDMANetworkParameters"}
};

const GetANAAAAuthStatusTestCase_t \
GetANAAAAuthStatusTestCases[MAX_GET_ANAAA_AUTH_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "Valid GetANAAAAuthStatus"}
};

const SLQSSetBandPreferenceTestCase_t \
SLQSSetBandPreferenceTestCases[MAX_SLQS_SET_BAND_PREFERENCE_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, 0x0000000000000000, "Valid SetBandPreference"},
    {eQCWWAN_ERR_NONE, 2, 0x0000000000000001, "Valid SetBandPreference"},
    {eQCWWAN_ERR_NONE, 3, 0x0000000000000003, "Valid SetBandPreference"}
};

const SLQSIndicationRegisterTestCase_t \
SLQSIndicationRegisterTestCases[MAX_SLQS_INDICATION_REGISTER_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, 2, 2, 2, "Invalid IndicationRegister : 2:2:2"},
    {eQCWWAN_ERR_NONE, 2, 1, 1, 1, "Valid IndicationRegister : Enable"},
    {eQCWWAN_ERR_NONE, 3, 0, 0, 0, "Valid IndicationRegister : Disable"},
};

struct slqsSignalStrengthInfo signalInfo;
const SLQSGetSignalStrengthTestCase_t \
    SLQSGetSignalStrengthTestCases[MAX_SLQS_GET_SIGNAL_STRENGTH_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, NULL,        "Invalid case - Sending Null pointer" },
    { eQCWWAN_ERR_NONE, 2, &signalInfo, "Valid case - All info bits masked" }
};

#define MAX_NUM_NETWORKS 30

BYTE NetworkInfoInstances = MAX_NUM_NETWORKS;
BYTE RATInstances         = MAX_NUM_NETWORKS;
struct SlqsNas3GppNetworkInfo NetworkInfo[MAX_NUM_NETWORKS];
struct SlqsNas3GppNetworkRAT  RATInfo[MAX_NUM_NETWORKS];

const SLQSPerformNetworkScanTestCase_t \
    SLQSPerformNetworkScanTestCases[MAX_SLQS_PERFORM_NETWORK_SCAN_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1,                  NULL,        NULL,
         &RATInstances, RATInfo, "Invalid Case - Invalid Arguments"},
    {eQCWWAN_ERR_NONE, 2, &NetworkInfoInstances, NetworkInfo,
                  NULL,    NULL, "Valid Case - Fetching only Network Info"},
    {eQCWWAN_ERR_NONE, 3, &NetworkInfoInstances, NetworkInfo,
         &RATInstances, RATInfo, "Valid Case - Fetching Network "
                                                      "as well as RAT Info"},
};

#endif /* __GOBI_API_NAS_TEST_H__ */
