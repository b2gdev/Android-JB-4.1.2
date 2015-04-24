/**************
 *
 *  Filename: qaGobiApiWdsTest.h
 *
 *  Purpose:  Contains prototypes for functions implementing specific
 *            tests for Wireless Data Service (WDS)
 *            called by the main() routine in qatest.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc., all rights reserved
 *
 **************/

#ifndef __GOBI_API_WDS_TEST_H__
#define __GOBI_API_WDS_TEST_H__

/* Include Files */

/* Defines */
#define nMaxStrLen                                     0xFF
#define ENTER_KEY                                      0x0A
#define OPTION_LEN                                     0x04
#define EXTENDED_ERR                                   1081
#define MAX_GET_DEFAULT_PROFILE_TESTCASE_NUM           0x01
#define MAX_GET_DEFAULT_PROFILE_LTE_TESTCASE_NUM       0x02
#define MAX_GET_AUTOCONNECT_TESTCASE_NUM               0x01
#define MAX_GET_SESSION_STATE_TESTCASE_NUM             0x01
#define MAX_GET_PACKET_STATUS_TESTCASE_NUM             0x01
#define MAX_GET_BYTE_TOTALS_TESTCASE_NUM               0x01
#define MAX_GET_DORMANCY_STATE_TESTCASE_NUM            0x01
#define MAX_GET_DATA_BR_TECH_TESTCASE_NUM              0x02
#define MAX_GET_SESSION_DURATION_TESTCASE_NUM          0x01
#define MAX_GET_IP_ADDRESS_TESTCASE_NUM                0x01
#define MAX_GET_IP_ADDRESSLTE_TESTCASE_NUM             0x01
#define MAX_GET_CONNECTION_RATE_TESTCASE_NUM           0x01
#define MAX_SET_AUTOCONNECT_TESTCASE_NUM               0x02
#define MAX_SET_DEFAULT_PROFILE_TESTCASE_NUM           0x01
#define MAX_SET_DEFAULT_PROFILELTE_TESTCASE_NUM        0x01
#define MAX_SET_MOBILE_IP_TESTCASE_NUM                 0x04
#define MAX_GET_MOBILE_IP_TESTCASE_NUM                 0x01
#define MAX_GET_LAST_MOBILE_IP_ERROR_TESTCASE_NUM      0x01
#define MAX_GET_MOBILE_IP_PROFILE_TESTCASE_NUM         0x02
#define MAX_SET_MOBILE_IP_PROFILE_TESTCASE_NUM         0x02
#define MAX_SLQS_GET_RUNTIME_SETTINGS_TESTCASE_NUM     0x01
#define MAX_SLQS_SET_PROFILE_TESTCASE_NUM              0x11
#define MAX_SLQS_GET_PROFILE_TESTCASE_NUM              0x11
#define MAX_SET_MOBILE_IP_PARAM_TESTCASE_NUM           0x02
#define MAX_SLQS_DELETE_PROFILE_TESTCASE_NUM           0x03
#define MAX_SLQS_CREATE_PROFILE_TESTCASE_NUM           0x04
#define MAX_SLQS_CREATE_PROFILE2_TESTCASE_NUM          0x03
#define MAX_SLQS_AC_SETTINGS_TESTCASE_NUM              0x06
#define MAX_GET_CUR_DATA_BR_TECH_TESTCASE_NUM          0x04
#define MAX_SLQS_MODIFY_PROFILE_TESTCASE_NUM           0x03
#define MAX_SLQS_MODIFY_PROFILE2_TESTCASE_NUM          0x03

/* Enums */

/* User options enumeration */
enum eUserOptions{
    eACENABLED = 0x31,
    eACDISABLED,
    eACPAUSED,
    eACRALWAYSALLOWED,
    eACRHOMENWONLY,
    eEXIT_APP = ENTER_KEY
};

/* Structure for test cases step */
typedef struct _StartDataSession2TestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             *pTechnology;
    ULONG             *pPrimaryDNS;
    ULONG             *pSecondaryDNS;
    ULONG             *pPrimaryNBNS;
    ULONG             *pSecondaryNBNS;
    CHAR              *pAPNName;
    ULONG             *pIPAddress;
    ULONG             *pAuthentication;
    CHAR              *pUsername;
    CHAR              *pPassword;
    ULONG             *pSessionID;
    ULONG             *pFailureReason;
    CHAR              desc[100];
} StartDataSession2TestCase_t;

typedef struct _StopDataSessionTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              requireSession; /* 1=Yes 0=No */
    CHAR              desc[100];
} StopDataSessionTestCase_t;

typedef struct _StartDataSessionLTETestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             Technology;
    ULONG             PrimaryDNSv4;
    ULONG             SecondaryDNSv4;
    ULONG             PrimaryNBNSv4;
    ULONG             SecondaryNBNSv4;
    USHORT            PrimaryDNSv6;
    USHORT            SecondaryDNSv6;
    CHAR              APNName[nMaxStrLen];
    ULONG             IPAddressv4;
    USHORT            IPAddressv6[8];
    ULONG             Authentication;
    CHAR              Username[nMaxStrLen];
    CHAR              Password[nMaxStrLen];
    CHAR              desc[100];
    BYTE              ipfamily;
} StartDataSessionLTETestCase_t;

typedef struct _GetDefaultProfileTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             profileType;
    CHAR              desc[100];
} GetDefaultProfileTestCase_t;

typedef struct _GetDefaultProfileLTETestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             profileType;
    CHAR              desc[100];
} GetDefaultProfileLTETestCase_t;

typedef struct _GetAutoconnectTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetAutoconnectTestCase_t;

typedef struct _GetSessionStateTestCase_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    CHAR  desc[100];
} GetSessionStateTestCase_t;

typedef struct _GetPacketStatusTestCase_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    CHAR  desc[100];
} GetPacketStatusTestCase_t;

typedef struct _GetByteTotalsTestCase_t
{
    ULONG      expectedReturnValue;
    ULONG      testCaseId;
    CHAR       desc[100];
} GetByteTotalsTestCase_t;

typedef struct _GetDormancyStateTestCase_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    CHAR  desc[100];
} GetDormancyStateTestCase_t;

typedef struct _GetDataBrTechTestCase_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    ULONG *pDataBearer;
    CHAR  desc[100];
} GetDataBrTechTestCase_t;

typedef struct _GetCurDataBrTechTestCase_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    QmiWDSDataBearers *pDataBearers;
    QmiWDSDataBearerTechnology *pCurDataBearerTechnology;
    QmiWDSDataBearerTechnology *pLastCallDataBearerTechnology;
    CHAR  desc[100];
} GetCurDataBrTechTestCase_t;

typedef struct _GetSessionDurationTestCase_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    CHAR  desc[100];
} GetSessionDurationTestCase_t;

typedef struct _GetIPAddressTestCaseStep_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    CHAR  desc[100];
} GetIPAddressTestCaseStep_t;

typedef struct _GetIPAddressLTETestCaseStep_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    CHAR  desc[100];
} GetIPAddressLTETestCaseStep_t;

typedef struct _GetConnectionRateTestCaseStep_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    CHAR  desc[100];
} GetConnectionRateTestCaseStep_t;

typedef struct _SetAutoConnectTestCaseStep_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    ULONG Setting;
    CHAR  desc[100];
} SetAutoConnectTestCaseStep_t;

typedef struct _SetDefaultProfileTestCaseStep_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             profileType;
    ULONG             PDPType;
    ULONG             IPAddress;
    ULONG             PrimaryDNS;
    ULONG             SecondaryDNS;
    ULONG             Authentication;
    CHAR              Name[nMaxStrLen];
    CHAR              APNName[nMaxStrLen];
    CHAR              Username[nMaxStrLen];
    CHAR              Password[nMaxStrLen];
    CHAR              desc[100];
} SetDefaultProfileTestCaseStep_t;

typedef struct _SetDefaultProfileLTETestCaseStep_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             profileType;
    ULONG             PDPType;
    ULONG             IPAddressv4;
    ULONG             PrimaryDNSv4;
    ULONG             SecondaryDNSv4;
    USHORT            IPAddressv6[8];
    USHORT            PrimaryDNSv6;
    USHORT            SecondaryDNSv6;
    ULONG             Authentication;
    CHAR              Name[nMaxStrLen];
    CHAR              APNName[nMaxStrLen];
    CHAR              Username[nMaxStrLen];
    CHAR              Password[nMaxStrLen];
    CHAR              desc[100];
} SetDefaultProfileLTETestCaseStep_t;

typedef struct _SetMobileIPTestCaseStep_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    ULONG mode;
    CHAR  desc[100];
} SetMobileIPTestCaseStep_t;

typedef struct _GetMobileIPTestCaseStep_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    CHAR  desc[100];
} GetMobileIPTestCaseStep_t;

typedef struct _GetMobileIPProfileTestCaseStep_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    BYTE  index;
    CHAR  naiSize;
    CHAR  desc[100];
}  GetMobileIPProfileTestCaseStep_t;

typedef struct _GetLastMobileIPErrorTestCaseStep_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    CHAR  desc[100];
} GetLastMobileIPErrorTestCaseStep_t;

typedef struct _SLQSGetRuntimeSettingsTestCaseStep_t
{
    ULONG                             expectedReturnValue;
    ULONG                             testCaseId;
    CHAR                              *pProfileName;
    ULONG                             *pPDPType;
    CHAR                              *pAPNName;
    ULONG                             *pPrimaryDNSV4;
    ULONG                             *pSecondaryDNSV4;
    struct UMTSQoS                    *pUMTSGrantedQos;
    struct GPRSQoS                    *pGPRSGrantedQos;
    CHAR                              *pUsername;
    ULONG                             *pAuthentication;
    ULONG                             *pIPAddressV4;
    struct ProfileIdentifier          *pProfileID;
    ULONG                             *pGWAddressV4;
    ULONG                             *pSubnetMaskV4;
    BYTE                              *pPCSCFAddrPCO;
    struct PCSCFIPv4ServerAddressList *pServerAddrList;
    struct PCSCFFQDNAddressList       *pPCSCFFQDNAddrList;
    USHORT                            *pPrimDNSV6;
    USHORT                            *pSecondDNSV6;
    ULONG                             *pMtu;
    struct DomainNameList             *pDomainList;
    BYTE                              *pIPFamilyPreference;
    BYTE                              *pIMCNflag;
    WORD                              *pTechnology;
    struct IPV6AddressInfo            *pIPV6AddrInfo;
    struct IPV6GWAddressInfo          *pIPV6GWAddrInfo;
    CHAR                              desc[100];
} SLQSGetRuntimeSettingsTestCaseStep_t;

typedef struct _SLQSSetProfileTestCaseStep_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             profileType;
    BYTE              profileId;
    ULONG             PDPType;
    ULONG             IPAddress;
    ULONG             PrimaryDNS;
    ULONG             SecondaryDNS;
    ULONG             Authentication;
    CHAR              Name[nMaxStrLen];
    CHAR              APNName[nMaxStrLen];
    CHAR              Username[nMaxStrLen];
    CHAR              Password[nMaxStrLen];
    CHAR              desc[100];
} SLQSSetProfileTestCaseStep_t;

typedef struct _SlqsGetProfileSettingsTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             profileType;
    BYTE              profileId;
    CHAR              desc[100];
} SlqsGetProfileSettingsTestCase_t;

typedef struct _SLQSStartStopDataSessTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BOOL              action;
    ULONG             Technology;
    ULONG             profID3GPP;
    ULONG             profID3GPP2;
    CHAR              desc[100];
} SLQSStartStopDataSessTestCase_t;

typedef struct _SetMobileIPParamTestCaseStep_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    CHAR  SPC[7];
    ULONG Mode;
    BYTE  RetryLimit;
    BYTE  RetryInterval;
    BYTE  ReRegPeriod;
    BYTE  ReRegTraffic;
    BYTE  HAAuthenticator;
    BYTE  HA2002bis;
    CHAR  desc[100];
} SetMobileIPParamTestCaseStep_t;

typedef struct _SLQSSetIPFamilyPreferenceTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              IPFamilyPreference;
    CHAR              desc[100];
} SLQSSetIPFamilyPreferenceTestCase_t;

typedef struct _SLQSDeleteProfileTestCase_t
{
    enum eQCWWANError              expectedReturnValue;
    ULONG                          testCaseId;
    BYTE                           profileIndex;
    struct SLQSDeleteProfileParams *pDeleteProfile;
    CHAR                           desc[100];
} SLQSDeleteProfileTestCase_t;

typedef struct _SLQSCreateProfileTestCaseStep_t
{
    ULONG  expectedReturnValue;
    ULONG  testCaseId;
    BYTE   *pProfileType;
    BYTE   *pProfileID;
    CHAR   *pProfilename;
    BYTE   *pPDPtype;
    BYTE   *pPdpHdrCompType;
    BYTE   *pPdpDataCompType;
    CHAR   *pAPNName;
    ULONG  *pPriDNSIPv4AddPref;
    ULONG  *pSecDNSIPv4AddPref;
    struct UMTSQoS *pUMTSReqQoS;
    struct UMTSQoS *pUMTSMinQoS;
    struct GPRSRequestedQoS *pGPRSRequestedQos;
    struct GPRSRequestedQoS *pGPRSMinimumQoS;
    CHAR   *pUsername;
    CHAR   *pPassword;
    BYTE   *pAuthenticationPref;
    ULONG  *pIPv4AddrPref;
    BYTE   *pPcscfAddrUsingPCO;
    BYTE   *pPdpAccessConFlag;
    BYTE   *pPcscfAddrUsingDhcp;
    BYTE   *pImCnFlag;
    struct TFTIDParams *pTFTID1Params;
    struct TFTIDParams *pTFTID2Params;
    BYTE   *pPdpContext;
    BYTE   *pSecondaryFlag;
    BYTE   *pPrimaryID;
    USHORT *pIPv6AddPref;
    struct UMTSReqQoSSigInd *pUMTSReqQoSSigInd;
    struct UMTSReqQoSSigInd *pUMTSMinQosSigInd;
    USHORT *pPriDNSIPv6addpref;
    USHORT *pSecDNSIPv6addpref;
    BYTE   *pAddrAllocPref;
    struct QosClassID *pQosClassID;
    CHAR   desc[100];
} SLQSCreateProfileTestCaseStep_t;
typedef struct _SLQSCreateProfile2TestCaseStep_t
{
    ULONG  expectedReturnValue;
    ULONG  testCaseId;
    BYTE   *pProfileType;
    BYTE   *pProfileID;
    BYTE   *pNegoDnsSrvrPref;
    ULONG  *pPppSessCloseTimerDO;
    ULONG  *pPppSessCloseTimer1x;
    BYTE   *pAllowLinger;
    USHORT *pLcpAckTimeout;
    USHORT *pIpcpAckTimeout;
    USHORT *pAuthTimeout;
    BYTE   *pLcpCreqRetryCount;
    BYTE   *pIpcpCreqRetryCount;
    BYTE   *pAuthRetryCount;
    BYTE   *pAuthProtocol;
    CHAR   *pUserId;
    CHAR   *pAuthPassword;
    BYTE   *pDataRate;
    ULONG  *pAppType;
    BYTE   *pDataMode;
    BYTE   *pAppPriority;
    CHAR   *pApnString;
    BYTE   *pPdnType;
    BYTE   *pIsPcscfAddressNedded;
    ULONG  *pPrimaryV4DnsAddress;
    ULONG  *pSecondaryV4DnsAddress;
    USHORT *pPriV6DnsAddress;
    USHORT *pSecV6DnsAddress;
    CHAR   desc[100];
} SLQSCreateProfile2TestCaseStep_t;

typedef struct _SLQSAutoConnectTestCaseStep_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    BOOL  action;
    BYTE  acsetting;
    BYTE  acroamsetting;
    CHAR  desc[100];
} SLQSAutoConnectTestCaseStep_t;

typedef struct _SLQSModifyProfileTestCaseStep_t
{
    ULONG  expectedReturnValue;
    ULONG  testCaseId;
    BYTE   *pProfileType;
    BYTE   *pProfileID;
    CHAR   *pProfilename;
    BYTE   *pPDPtype;
    BYTE   *pPdpHdrCompType;
    BYTE   *pPdpDataCompType;
    CHAR   *pAPNName;
    ULONG  *pPriDNSIPv4AddPref;
    ULONG  *pSecDNSIPv4AddPref;
    struct UMTSQoS *pUMTSReqQoS;
    struct UMTSQoS *pUMTSMinQoS;
    struct GPRSRequestedQoS *pGPRSRequestedQos;
    struct GPRSRequestedQoS *pGPRSMinimumQoS;
    CHAR   *pUsername;
    CHAR   *pPassword;
    BYTE   *pAuthenticationPref;
    ULONG  *pIPv4AddrPref;
    BYTE   *pPcscfAddrUsingPCO;
    BYTE   *pPdpAccessConFlag;
    BYTE   *pPcscfAddrUsingDhcp;
    BYTE   *pImCnFlag;
    struct TFTIDParams *pTFTID1Params;
    struct TFTIDParams *pTFTID2Params;
    BYTE   *pPdpContext;
    BYTE   *pSecondaryFlag;
    BYTE   *pPrimaryID;
    USHORT *pIPv6AddPref;
    struct UMTSReqQoSSigInd *pUMTSReqQoSSigInd;
    struct UMTSReqQoSSigInd *pUMTSMinQosSigInd;
    USHORT *pPriDNSIPv6addpref;
    USHORT *pSecDNSIPv6addpref;
    BYTE   *pAddrAllocPref;
    struct QosClassID *pQosClassID;
    CHAR   desc[100];
} SLQSModifyProfileTestCaseStep_t;

typedef struct _SLQSModifyProfile2TestCaseStep_t
{
    ULONG  expectedReturnValue;
    ULONG  testCaseId;
    BYTE   *pProfileType;
    BYTE   *pProfileID;
    BYTE   *pNegoDnsSrvrPref;
    ULONG  *pPppSessCloseTimerDO;
    ULONG  *pPppSessCloseTimer1x;
    BYTE   *pAllowLinger;
    USHORT *pLcpAckTimeout;
    USHORT *pIpcpAckTimeout;
    USHORT *pAuthTimeout;
    BYTE   *pLcpCreqRetryCount;
    BYTE   *pIpcpCreqRetryCount;
    BYTE   *pAuthRetryCount;
    BYTE   *pAuthProtocol;
    CHAR   *pUserId;
    CHAR   *pAuthPassword;
    BYTE   *pDataRate;
    ULONG  *pAppType;
    BYTE   *pDataMode;
    BYTE   *pAppPriority;
    CHAR   *pApnString;
    BYTE   *pPdnType;
    BYTE   *pIsPcscfAddressNedded;
    ULONG  *pPrimaryV4DnsAddress;
    ULONG  *pSecondaryV4DnsAddress;
    USHORT *pPriV6DnsAddress;
    USHORT *pSecV6DnsAddress;
    CHAR   desc[100];
} SLQSModifyProfile2TestCaseStep_t;

typedef struct _SLQSSetGet3gppConfigItemTestCase_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    WORD  *pLTEAttachProfile;
    WORD  *pProfileList;
    BYTE  *pDefaultPDNEnabled;
    BYTE  *p3gppRelease;
    CHAR  desc[100];
} SLQSSetGet3gppConfigItemTestCase_t;

typedef struct _SLQSGetProfileSettings3GPPTestCase_t
{
    ULONG                   expectedReturnValue;
    ULONG                   testCaseId;
    BYTE                    ProfileType;
    BYTE                    ProfileID;
    CHAR                    *pProfilename;
    WORD                    *pProfileNameSize;
    BYTE                    *pPDPtype;
    BYTE                    *pPdpHdrCompType;
    BYTE                    *pPdpDataCompType;
    CHAR                    *pAPNName;
    WORD                    *pAPNNameSize;
    ULONG                   *pPriDNSIPv4AddPref;
    ULONG                   *pSecDNSIPv4AddPref;
    struct UMTSQoS          *pUMTSReqQoS;
    struct UMTSQoS          *pUMTSMinQoS;
    struct GPRSRequestedQoS *pGPRSRequestedQos;
    struct GPRSRequestedQoS *pGPRSMinimumQoS;
    CHAR                    *pUsername;
    WORD                    *pUserNameSize;
    CHAR                    *pPassword;
    WORD                    *pPasswordSize;
    BYTE                    *pAuthenticationPref;
    ULONG                   *pIPv4AddrPref;
    BYTE                    *pPcscfAddrUsingPCO;
    BYTE                    *pPdpAccessConFlag;
    BYTE                    *pPcscfAddrUsingDhcp;
    BYTE                    *pImCnFlag;
    struct TFTIDParams      *pTFTID1Params;
    struct TFTIDParams      *pTFTID2Params;
    BYTE                    *pPdpContext;
    BYTE                    *pSecondaryFlag;
    BYTE                    *pPrimaryID;
    USHORT                  *pIPv6AddPref;
    struct UMTSReqQoSSigInd *pUMTSReqQoSSigInd;
    struct UMTSReqQoSSigInd *pUMTSMinQosSigInd;
    USHORT                  *pPriDNSIPv6addpref;
    USHORT                  *pSecDNSIPv6addpref;
    BYTE                    *pAddrAllocPref;
    struct QosClassID       *pQosClassID;
    BYTE                    *pAPNDisabledFlag;
    ULONG                   *pPDNInactivTimeout;
    BYTE                    *pAPNClass;
    WORD                    *pExtErrCode;
    CHAR                    desc[100];
} SLQSGetProfileSettings3GPPTestCase_t;

typedef struct _SLQSGetProfileSettings3GPP2TestCase_t
{
    ULONG  expectedReturnValue;
    ULONG  testCaseId;
    BYTE   ProfileType;
    BYTE   ProfileID;
    BYTE   *pNegoDnsSrvrPref;
    ULONG  *pPppSessCloseTimerDO;
    ULONG  *pPppSessCloseTimer1x;
    BYTE   *pAllowLinger;
    USHORT *pLcpAckTimeout;
    USHORT *pIpcpAckTimeout;
    USHORT *pAuthTimeout;
    BYTE   *pLcpCreqRetryCount;
    BYTE   *pIpcpCreqRetryCount;
    BYTE   *pAuthRetryCount;
    BYTE   *pAuthProtocol;
    CHAR   *pUserId;
    WORD   *pUserIdSize;
    CHAR   *pAuthPassword;
    WORD   *pAuthPwdSize;
    BYTE   *pDataRate;
    ULONG  *pAppType;
    BYTE   *pDataMode;
    BYTE   *pAppPriority;
    CHAR   *pApnString;
    WORD   *pApnStrSize;
    BYTE   *pPdnType;
    BYTE   *pIsPcscfAddressNedded;
    ULONG  *pPrimaryV4DnsAddress;
    ULONG  *pSecondaryV4DnsAddress;
    USHORT *pPriV6DnsAddress;
    USHORT *pSecV6DnsAddress;
    BYTE   *pRATType;
    BYTE   *pAPNEnabled3GPP2;
    ULONG  *pPDNInactivTimeout3GPP2;
    BYTE   *pAPNClass3GPP2;
    WORD   *pExtErrCode;
    CHAR   desc[100];
} SLQSGetProfileSettings3GPP2TestCase_t;

typedef struct _SetMobileIPProfileTestCaseStep_t
{
    ULONG expectedReturnValue;
    ULONG testCaseId;
    CHAR  *pSPC;
    BYTE  index;
    BYTE  *pEnabled;
    ULONG *pAddress;
    ULONG *pPrimaryHA;
    ULONG *pSecondaryHA;
    BYTE  *pRevTunneling;
    CHAR  *pNAI;
    ULONG *pHASPI;
    ULONG *pAAASPI;
    CHAR  *pMNHA;
    CHAR  *pMNAAA;
    CHAR  desc[100];
} SetMobileIPProfileTestCaseStep_t;

typedef struct _SLQSWdsSetEventReportTestCase_t
{
    ULONG     expectedReturnValue;
    ULONG     testCaseId;
    BYTE      *pCurrChannelRateInd;
    TrStatInd *pTransferStatInd;
    BYTE      *pDataBearerTechInd;
    BYTE      *pDormancyStatusInd;
    BYTE      *pMIPStatusInd;
    BYTE      *pCurrDataBearerTechInd;
    BYTE      *pDataCallStatusChangeInd;
    BYTE      *pCurrPrefDataSysInd;
    BYTE      *pEVDOPageMonPerChangeInd;
    BYTE      *pDataSystemStatusChangeInd;
    CHAR      desc[100];
} SLQSWdsSetEventReportTestCase_t;

typedef struct _SLQSWdsSwiPDPRuntimeSettingsTestCase_t
{
    ULONG                  expectedReturnValue;
    ULONG                  testCaseId;
    BYTE                   contextId;
    BYTE                   *pContextId;
    BYTE                   *pBearerId;
    CHAR                   *pAPNName;
    ULONG                  *pIPv4Address;
    ULONG                  *pIPv4GWAddress;
    ULONG                  *pPrDNSIPv4Address;
    ULONG                  *pSeDNSIPv4Address;
    struct IPV6AddressInfo *pIPv6Address;
    struct IPV6AddressInfo *pIPv6GWAddress;
    WORD                   *pPrDNSIPv6Address;
    WORD                   *pSeDNSIPv6Address;
    ULONG                  *pPrPCSCFIPv4Address;
    ULONG                  *pSePCSCFIPv4Address;
    WORD                   *pPrPCSCFIPv6Address;
    WORD                   *pSePCSCFIPv6Address;
    CHAR                   desc[100];
} SLQSWdsSwiPDPRuntimeSettingsTestCase_t;

/* Define the test cases to be executed */
const GetDefaultProfileTestCase_t
      GetDefaultProfileTestCases[MAX_GET_DEFAULT_PROFILE_TESTCASE_NUM] =
{
//    {eQCWWAN_ERR_INVALID_QMI_RSP,1,1,"Invalid Profile Type"},
    {eQCWWAN_ERR_NONE,0,0,"Get Default Profile Valid Case"}
};

const GetDefaultProfileLTETestCase_t
      GetDefaultProfileLTETestCases[MAX_GET_DEFAULT_PROFILE_LTE_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_INVALID_QMI_RSP, 1, 1, "Invalid Profile Type"},
    {eQCWWAN_ERR_NONE, 2, 0, "Get Default LTE Profile for UMTS"}
};

const GetAutoconnectTestCase_t
      GetAutoconnectTestCases[MAX_GET_AUTOCONNECT_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "Get Autoconnectsetting"}
};

const GetSessionStateTestCase_t
    GetSessionStateTestCases[MAX_GET_SESSION_STATE_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "Valid SessionState"}
};

const GetPacketStatusTestCase_t
    GetPacketStatusTestCases[MAX_GET_PACKET_STATUS_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "Valid PacketStatus"}
};

const GetByteTotalsTestCase_t
    GetByteTotalsTestCases[MAX_GET_BYTE_TOTALS_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "Valid ByteTotals"}
};

const GetDormancyStateTestCase_t
    GetDormancyStateTestCases[MAX_GET_DORMANCY_STATE_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "Valid DormancyState"}
};

ULONG DataBearer;

const GetDataBrTechTestCase_t
    GetDataBrTechTestCases[MAX_GET_DATA_BR_TECH_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, NULL,        "NULL Parameter Testing"},
    {eQCWWAN_ERR_NONE, 2, &DataBearer, "Valid DataBearer Technology"}
};

QmiWDSDataBearers          DataBearers;
QmiWDSDataBearerTechnology curDataBearerTechnology;
QmiWDSDataBearerTechnology lastCallDataBearerTechnology;

const GetCurDataBrTechTestCase_t
    GetCurDataBrTechTestCases[MAX_GET_CUR_DATA_BR_TECH_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, NULL, NULL,  NULL, "NULL Parameter Testing"},
    {eQCWWAN_ERR_NONE, 2, &DataBearers, &curDataBearerTechnology, NULL,
                                      "Valid current databearer technology"},
    {eQCWWAN_ERR_NONE, 3, &DataBearers, NULL, &lastCallDataBearerTechnology,
                                    "Valid last call databearer technology"},
    {eQCWWAN_ERR_NONE, 4, &DataBearers, &curDataBearerTechnology,
                                &lastCallDataBearerTechnology,  "All Valid"}
};

const GetSessionDurationTestCase_t
    GetSessionDurationTestCases[MAX_GET_SESSION_DURATION_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "Valid SessionDuration"}
};

const GetIPAddressTestCaseStep_t
    GetIPAddressTestCases[MAX_GET_IP_ADDRESS_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "All Valid"}
};

const GetIPAddressLTETestCaseStep_t
    GetIPAddressLTETestCases[MAX_GET_IP_ADDRESSLTE_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "All Valid"}
};

const SetAutoConnectTestCaseStep_t
    SetAutoconnectTestCases[MAX_SET_AUTOCONNECT_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, 0, "All Valid"},
    {eQCWWAN_ERR_NONE, 2, 1, "All Valid"}
};

const GetConnectionRateTestCaseStep_t
    GetConnectionRateTestCases[MAX_GET_CONNECTION_RATE_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "All Valid"}
};

const SetDefaultProfileTestCaseStep_t
    SetDefaultProfileTestCases[MAX_SET_DEFAULT_PROFILE_TESTCASE_NUM] =
{
     {eQCWWAN_ERR_NONE, 1, 0, 0, 0, 0, 0, 1,
      "Infosys", "internet.com", "wapuser", "wap", "All Valid"}
};

const SetDefaultProfileLTETestCaseStep_t
    SetDefaultProfileLTETestCases[MAX_SET_DEFAULT_PROFILELTE_TESTCASE_NUM] =
{
     {eQCWWAN_ERR_INVALID_QMI_RSP, 1, 0, 0, 0, 0, 0, { 0,0,0,0,0,0,0,0 }, 0, 0, 3,
      "Infosys", "internet.com", "wapuser", "wap", "All Valid"}
};

const SetMobileIPTestCaseStep_t
    SetMobileIPTestCases[MAX_SET_MOBILE_IP_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1,3, "Invalid mode case"},
    {eQCWWAN_ERR_NONE, 1,0, "valid mode case"},
    {eQCWWAN_ERR_NONE, 1,1, "valid mode case"},
    {eQCWWAN_ERR_NONE, 1,2, "valid mode case"}
};

const GetMobileIPTestCaseStep_t
    GetMobileIPTestCases[MAX_GET_MOBILE_IP_TESTCASE_NUM] =
{
     {eQCWWAN_ERR_NONE, 1, "valid Error case"}
};

const GetMobileIPProfileTestCaseStep_t
    GetMobileIPProfileTestCases[MAX_GET_MOBILE_IP_PROFILE_TESTCASE_NUM ] =
{
    {eQCWWAN_ERR_NONE, 1, 0,  255, "All Valid"},
    {eQCWWAN_ERR_NONE, 1, 1,  255, "All Valid"}
};

const GetLastMobileIPErrorTestCaseStep_t
GetLastMobileIPErrorTestCases[MAX_GET_LAST_MOBILE_IP_ERROR_TESTCASE_NUM] =
{
     {eQCWWAN_ERR_NONE, 1, "valid Error case"}
};

CHAR                              ProfileName[nMaxStrLen];
ULONG                             PDPType;
CHAR                              APNName[nMaxStrLen];
ULONG                             PrimaryDNSV4;
ULONG                             SecondaryDNSV4;
struct UMTSQoS                    UMTSGrantedQos;
struct GPRSQoS                    GPRSGrantedQos;
CHAR                              Username[nMaxStrLen];
ULONG                             Authentication;
ULONG                             IPAddressV4;
struct                            ProfileIdentifier ProfileID;
ULONG                             GWAddressV4;
ULONG                             SubnetMaskV4;
BYTE                              PCSCFAddrPCO;
struct PCSCFIPv4ServerAddressList ServerAddrList;
struct PCSCFFQDNAddressList       PCSCFFQDNAddrList;
USHORT                            PrimDNSV6[8];
USHORT                            SecondDNSV6[8];
ULONG                             Mtu;
struct DomainNameList             DomainList;
BYTE                              IPFamilyPreference;
BYTE                              IMCNflag;
WORD                              Technology;
struct IPV6AddressInfo            IPV6AddrInfo;
struct IPV6GWAddressInfo          IPV6GWAddrInfo;

const SLQSGetRuntimeSettingsTestCaseStep_t
    SLQSGetRuntimeSettingsTestCases[
                             MAX_SLQS_GET_RUNTIME_SETTINGS_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1,
      ProfileName,
      &PDPType,
      APNName,
      &PrimaryDNSV4,
      &SecondaryDNSV4,
      &UMTSGrantedQos,
      &GPRSGrantedQos,
      Username,
      &Authentication,
      &IPAddressV4,
      &ProfileID,
      &GWAddressV4,
      &SubnetMaskV4,
      &PCSCFAddrPCO,
      &ServerAddrList,
      &PCSCFFQDNAddrList,
      PrimDNSV6,
      SecondDNSV6,
      &Mtu,
      &DomainList,
      &IPFamilyPreference,
      &IMCNflag,
      &Technology,
      &IPV6AddrInfo,
      &IPV6GWAddrInfo, "All Valid"}
};

const SLQSSetProfileTestCaseStep_t
    SLQSSetProfileTestCases[MAX_SLQS_SET_PROFILE_TESTCASE_NUM] =
{
     {eQCWWAN_ERR_NONE, 1, 0, 20, 0, 0, 0, 0, 1,
      "Sierra", "verizen", "internet", "internet", "Invalid Profile Id"},
     {eQCWWAN_ERR_NONE, 2, 0, 1, 0, 0, 0, 0, 1,
      "SWI1", "internet.com", "wapuser", "wap", "All Valid"},
     {eQCWWAN_ERR_NONE, 3, 0, 2, 0, 0, 0, 0, 1,
      "SWI2", "vzwinternet", "", "", "All Valid"},
     {eQCWWAN_ERR_NONE, 4, 0, 3, 0, 0, 0, 0, 1,
      "SWI3", "vzwinternet", "", "", "All Valid"},
     {eQCWWAN_ERR_NONE, 5, 0, 4, 0, 0, 0, 0, 1,
      "SWI4", "vzwapp", "", "", "All Valid"},
     {eQCWWAN_ERR_NONE, 6, 0, 5, 0, 0, 0, 0, 1,
      "SWI5", "verizon.com5", "wapuser5", "wap5", "All Valid"},
     {eQCWWAN_ERR_NONE, 7, 0, 6, 0, 0, 0, 0, 1,
      "SWI6", "verizon.com6", "wapuser6", "wap6", "All Valid"},
     {eQCWWAN_ERR_NONE, 8, 0, 7, 0, 0, 0, 0, 1,
      "SWI7", "verizon.com7", "wapuser7", "wap7", "All Valid"},
     {eQCWWAN_ERR_NONE, 9, 0, 8, 0, 0, 0, 0, 1,
      "SWI8", "verizon.com8", "wapuser8", "wap8", "All Valid"},
     {eQCWWAN_ERR_NONE, 10, 0, 9, 0, 0, 0, 0, 1,
      "SWI9", "vzwims", "", "", "All Valid"},
     {eQCWWAN_ERR_NONE, 11, 0, 10, 0, 0, 0, 0, 1,
      "SWI10", "vzwadmin", "", "", "All Valid"},
     {eQCWWAN_ERR_NONE, 12, 0, 11, 0, 0, 0, 0, 1,
      "SWI11", "internet.com", "", "", "All Valid"},
     {eQCWWAN_ERR_NONE, 13, 0, 12, 0, 0, 0, 0, 1,
      "SWI12", "vzwapp", "", "", "All Valid"},
     {eQCWWAN_ERR_NONE, 14, 0, 13, 0, 0, 0, 0, 1,
      "SWI13", "verizon.com13", "wapuser13", "wap13", "All Valid"},
     {eQCWWAN_ERR_NONE, 15, 0, 14, 0, 0, 0, 0, 1,
      "SWI14", "verizon.com14", "wapuser14", "wap14", "All Valid"},
     {eQCWWAN_ERR_NONE, 16, 0, 15, 0, 0, 0, 0, 1,
      "SWI15", "internet.com15", "wapuser15", "wap15", "All Valid"},
     {eQCWWAN_ERR_NONE, 17, 0, 16, 0, 0, 0, 0, 1,
      "SWI16", "verizen16", "internet16", "internet16", "All Valid"}

};

const SlqsGetProfileSettingsTestCase_t
      SlqsGetProfileSettingsTestCases[MAX_SLQS_GET_PROFILE_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_INVALID_QMI_RSP,1,0, 20,"Invalid Profile Id"},
    {eQCWWAN_ERR_NONE, 2, 0, 1,"SLQS Get Profile Valid Case"},
    {eQCWWAN_ERR_NONE, 3, 0, 2,"SLQS Get Profile Valid Case"},
    {eQCWWAN_ERR_NONE, 4, 0, 3,"SLQS Get Profile Valid Case"},
    {eQCWWAN_ERR_NONE, 5, 0, 4,"SLQS Get Profile Valid Case"},
    {eQCWWAN_ERR_NONE, 6, 0, 5,"SLQS Get Profile Valid Case"},
    {eQCWWAN_ERR_NONE, 7, 0, 6,"SLQS Get Profile Valid Case"},
    {eQCWWAN_ERR_NONE, 8, 0, 7,"SLQS Get Profile Valid Case"},
    {eQCWWAN_ERR_NONE, 9, 0, 8,"SLQS Get Profile Valid Case"},
    {eQCWWAN_ERR_NONE, 10, 0, 9,"SLQS Get Profile Valid Case"},
    {eQCWWAN_ERR_NONE, 11, 0, 10,"SLQS Get Profile Valid Case"},
    {eQCWWAN_ERR_NONE, 12, 0, 11,"SLQS Get Profile Valid Case"},
    {eQCWWAN_ERR_NONE, 13, 0, 12,"SLQS Get Profile Valid Case"},
    {eQCWWAN_ERR_NONE, 14, 0, 13,"SLQS Get Profile Valid Case"},
    {eQCWWAN_ERR_NONE, 15, 0, 14,"SLQS Get Profile Valid Case"},
    {eQCWWAN_ERR_NONE, 16, 0, 15,"SLQS Get Profile Valid Case"},
    {eQCWWAN_ERR_NONE, 17, 0, 16,"SLQS Get Profile Valid Case"}
};

const SetMobileIPParamTestCaseStep_t
    SetMobileIPParamTestCases[MAX_SET_MOBILE_IP_PARAM_TESTCASE_NUM] =
{
    {eQCWWAN_ERR_NONE, 1, "123456", 1, 5, 10, 5, 1, 1, 1, "All valid case"},
    {eQCWWAN_ERR_NONE, 2, "abcdef", 0, 1, 1,  1, 1, 1, 1, "valid case"}
};

/* 3GPP profile Paramters */
USHORT SourceIP1[] = { 1,2,3,4,5,6,7,8 };
USHORT SourceIP2[] = { 1,2,3,4,5,6,7,8 };

/* structure object created for SLQSCreateProfile */
struct UMTSQoS objUMTSQoS =
{.trafficClass =0x02, .maxUplinkBitrate = 100, .maxDownlinkBitrate = 200,\
 .grntUplinkBitrate=100,.grntDownlinkBitrate=200,.qosDeliveryOrder= 0x02,\
 .maxSDUSize=300, .sduErrorRatio=0x02,.resBerRatio =0x02,.deliveryErrSDU =0x02,\
 .transferDelay =0, .trafficPriority = 0};

struct GPRSRequestedQoS objGPRSRequestedQoS;
struct UMTSReqQoSSigInd objUMTSReqQoSSigInd =
{.UMTSReqQoS = {.trafficClass =0x02, .maxUplinkBitrate = 100,\
 .maxDownlinkBitrate = 200, .grntUplinkBitrate=100,.grntDownlinkBitrate=200,\
 .qosDeliveryOrder= 0x02, .maxSDUSize=300, .sduErrorRatio=0x02,\
 .resBerRatio = 0x02,.deliveryErrSDU =0x02, .transferDelay =0,\
 .trafficPriority = 0}, .SigInd =0};

struct UMTSReqQoSSigInd objUMTSMinQoSSigInd = {.SigInd =0};

struct QosClassID objQosClassID =
{.QCI=1,.gDlBitRate= 100, .maxDlBitRate =200,.gUlBitRate=100,\
 .maxUlBitRate = 200 };
struct TFTIDParams objTFTID1Params =
{.eValid =0, .ipVersion =4,.pSourceIP = &SourceIP1[0]};

struct TFTIDParams objTFTID2Params =
 {.eValid =0, .ipVersion =6, .pSourceIP = &SourceIP2[0]};

BYTE profileID1  = 0x10;
BYTE profileID3  = 0x0C;
BYTE profileType = 0;

BYTE   pdpType = 0x00;
BYTE   pdpHdrCompType = 2;
BYTE   pdpDataCompType = 3;
ULONG  priDNSIPv4AddPref = 0;
ULONG  secDNSIPv4AddPref = 0;
BYTE   authenticationPref = 1;
ULONG  IPv4AddrPref = 0;
BYTE   pcscfAddrUsingPCO = 1;
BYTE   pdpAccessConFlag = 2;
BYTE   pcscfAddrUsingDhcp = 1;
BYTE   imCnFlag = 1;
BYTE   pdpContext = 1;
BYTE   secondaryFlag = 1;
BYTE   primaryID = 0;
USHORT IPv6AddPref[]       = { 0,0,0,0,0,0,0,0 };
USHORT PriDNSIPv6addpref[] = { 0,0,0,0,0,0,0,0 };
USHORT SecDNSIPv6addpref[] = { 0,0,0,0,0,0,0,0 };
BYTE   AddrAllocPref = 1;

const SLQSCreateProfileTestCaseStep_t
    SLQSCreateProfileTestCases[
                             MAX_SLQS_CREATE_PROFILE_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE,     1,                     &profileType,
      &profileID1,          "profile1",            &pdpType,
      &pdpHdrCompType,      &pdpDataCompType,      "wwww",
      &priDNSIPv4AddPref,   &secDNSIPv4AddPref,    &objUMTSQoS,
      &objUMTSQoS,          &objGPRSRequestedQoS,  &objGPRSRequestedQoS,
      "Sierra",             "12345",               &authenticationPref,
      &IPv4AddrPref,        &pcscfAddrUsingPCO,    &pdpAccessConFlag,
      &pcscfAddrUsingDhcp,  &imCnFlag,             &objTFTID1Params,
      &objTFTID2Params,     &pdpContext,           &secondaryFlag,
      &primaryID,           &IPv6AddPref[0],       &objUMTSReqQoSSigInd,
      &objUMTSReqQoSSigInd, &PriDNSIPv6addpref[0], &SecDNSIPv6addpref[0],
      &AddrAllocPref,       &objQosClassID,
      "All valid 3GPP:All optional parameter packed "},

    { eQCWWAN_ERR_NONE, 2,    &profileType, &profileID3, "",
      NULL,             NULL, NULL,         "",         NULL,
      NULL,             NULL, NULL,         NULL,       NULL,
      "",                "",  NULL,         NULL,       NULL,
      NULL,             NULL, NULL,         NULL,       NULL,
      NULL,             NULL, NULL,         NULL,       NULL,
      NULL,             NULL, NULL,         NULL,       NULL,
     "All valid 3GPP: All optional parameter NULL "},

    { eQCWWAN_ERR_NONE, 3,    &profileType, NULL, "",   NULL,
      NULL,             NULL, "",           NULL, NULL, NULL,
      NULL,             NULL, NULL,           "", "",   NULL,
      NULL,             NULL, NULL,         NULL, NULL, NULL,
      NULL,             NULL, NULL,         NULL, NULL, NULL,
      NULL,             NULL, NULL,         NULL, NULL,
      "Valid 3GPP: profileId is NULL"},

    { eQCWWAN_ERR_NONE,     4,                     &profileType,
      NULL,                 "profile1",            &pdpType,
      &pdpHdrCompType,      &pdpDataCompType,      "wwww",
      &priDNSIPv4AddPref,   &secDNSIPv4AddPref,    &objUMTSQoS,
      &objUMTSQoS,          &objGPRSRequestedQoS,  &objGPRSRequestedQoS,
      "Sierra",             "12345",               &authenticationPref,
      NULL,                 &pcscfAddrUsingPCO,    &pdpAccessConFlag,
      &pcscfAddrUsingDhcp,  &imCnFlag,             &objTFTID1Params,
      &objTFTID2Params,     &pdpContext,           &secondaryFlag,
      &primaryID,           &IPv6AddPref[0],       &objUMTSReqQoSSigInd,
      &objUMTSReqQoSSigInd, &PriDNSIPv6addpref[0], &SecDNSIPv6addpref[0],
      &AddrAllocPref,       &objQosClassID,
      "All valid 3GPP:All optional parameter packed "},
};

/* 3GPP2 Profile Paramters */
BYTE   profileID2 = 105;
BYTE   profileType2 = 1;
BYTE   negoDnsSrvrPref = 0;
ULONG  pppSessCloseTimerDO = 5;
ULONG  pppSessCloseTimer1x = 6;
BYTE   allowLinger = 0;
USHORT lcpAckTimeout = 5 ;
USHORT ipcpAckTimeout = 4;
USHORT authTimeout = 10 ;
BYTE   lcpCreqRetryCount =5;
BYTE   ipcpCreqRetryCount= 5;
BYTE   authRetryCount =5;
BYTE   authProtocol = 3;
BYTE   dataRate = 1;
ULONG  appType = 1;
BYTE   dataMode = 1;
BYTE   appPriority = 2;
BYTE   pdnType =0;
BYTE   isPcscfAddressNedded =0;
ULONG  primaryV4DnsAddress =0;
ULONG  secondaryV4DnsAddress= 0;
USHORT priV6DnsAddress = 0;
USHORT secV6DnsAddress = 0;

const SLQSCreateProfile2TestCaseStep_t
    SLQSCreateProfile2TestCases[
                             MAX_SLQS_CREATE_PROFILE2_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE,     1,                      &profileType2,
      &profileID2,          &negoDnsSrvrPref,       &pppSessCloseTimerDO,
      &pppSessCloseTimer1x, &allowLinger,           &lcpAckTimeout,
      &ipcpAckTimeout,      &authTimeout,           &lcpCreqRetryCount,
      &ipcpCreqRetryCount,  &authRetryCount,        &authProtocol,
      "Sierra",             "",                     &dataRate,
      &appType,             &dataMode,              &appPriority,
      "www",                &pdnType,               &isPcscfAddressNedded,
      &primaryV4DnsAddress, &secondaryV4DnsAddress, &priV6DnsAddress,
      &secV6DnsAddress, "All valid 3GPP2:All optional parameter packed"},

    { eQCWWAN_ERR_NONE,   2,    &profileType2,  &profileID2,
      NULL,               NULL, NULL,           NULL,
      NULL,               NULL, NULL,          NULL ,
      NULL,               NULL, NULL,             "",
        "",               NULL, NULL,           NULL,
      NULL,                 "", NULL,           NULL,
      NULL,              NULL,  NULL,           NULL,
      "Valid 3GPP2:All optional parameter NULL "},

    { eQCWWAN_ERR_NONE,    3, &profileType2, NULL, NULL, NULL, NULL,
      NULL,             NULL, NULL,          NULL,        NULL, NULL, NULL,
      NULL,               "",   "",          NULL,        NULL, NULL, NULL,
      "",               NULL, NULL,          NULL,        NULL, NULL, NULL,
     "Invalid 3GPP2: ProfileID is NULL "}
};

const SLQSAutoConnectTestCaseStep_t
    SLQSAutoConnectTestCases[
                             MAX_SLQS_AC_SETTINGS_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, 1, 1, 0, "Autoconnect Enabled"},
    { eQCWWAN_ERR_NONE, 2, 1, 0, 0, "Autoconnect Disabled"},
    { eQCWWAN_ERR_NONE, 3, 1, 2, 0, "Autoconnect Paused"},
    { eQCWWAN_ERR_NONE, 4, 1, 1, 0, "Autoconnect roaming Always allowed"},
    { eQCWWAN_ERR_NONE, 5, 1, 1, 1, "Autoconnect while in home service "
                                    "area only"},
    { eQCWWAN_ERR_NONE, 6, 0, 1, 0, "GetAutoConnect Settings"},
};

/* 3GPP profile Paramters */
USHORT SourceIPV6[] = { 1,2,3,4,5,6,7,8 };
USHORT SourceIPV4[2] = {0xC0A8, 0x0101};

/* structure object Modifyd for SLQSModifyProfile */
struct UMTSQoS objUMTSQoS2 =
{.trafficClass =0x02, .maxUplinkBitrate = 100, .maxDownlinkBitrate = 200,\
 .grntUplinkBitrate=100,.grntDownlinkBitrate=200,.qosDeliveryOrder= 0x02,\
 .maxSDUSize=300, .sduErrorRatio=0x02,.resBerRatio =0x02,.deliveryErrSDU =0x02,\
 .transferDelay =0, .trafficPriority = 0};

struct GPRSRequestedQoS objGPRSRequestedQoS2;
struct UMTSReqQoSSigInd objUMTSReqQoSSigInd2 =
{.UMTSReqQoS = {.trafficClass =0x02, .maxUplinkBitrate = 100,\
 .maxDownlinkBitrate = 200, .grntUplinkBitrate=100,.grntDownlinkBitrate=200,\
 .qosDeliveryOrder= 0x02, .maxSDUSize=300, .sduErrorRatio=0x02,\
 .resBerRatio = 0x02,.deliveryErrSDU =0x02, .transferDelay =0,\
 .trafficPriority = 0}, .SigInd =0};

struct UMTSReqQoSSigInd objUMTSMinQoSSigInd2 = {.SigInd =0};

struct QosClassID objQosClassID2 =
{.QCI=1,.gDlBitRate= 100, .maxDlBitRate =200,.gUlBitRate=100,\
 .maxUlBitRate = 200 };

struct TFTIDParams objTFTID1Params1 =
{.eValid =0, .ipVersion =4, .pSourceIP = &SourceIPV4[0]};

struct TFTIDParams objTFTID2Params2 =
{.eValid =0, .ipVersion =6, .pSourceIP = &SourceIPV6[0]};

BYTE profileID12  = 0x01;
BYTE profileID32  = 0x02;
BYTE profileType22 = 0;

BYTE   pdpType2 = 0x00;
BYTE   pdpHdrCompType2 = 2;
BYTE   pdpDataCompType2 = 3;
ULONG  priDNSIPv4AddPref2 = 0;
ULONG  secDNSIPv4AddPref2 = 0;
BYTE   authenticationPref2 = 1;
ULONG  IPv4AddrPref2 = 0;
BYTE   pcscfAddrUsingPCO2 = 1;
BYTE   pdpAccessConFlag2 = 2;
BYTE   pcscfAddrUsingDhcp2 = 1;
BYTE   imCnFlag2 = 1;
BYTE   pdpContext2 = 1;
BYTE   secondaryFlag2 = 1;
BYTE   primaryID2 = 0;
USHORT IPv6AddPref2[]       = { 0,0,0,0,0,0,0,0 };
USHORT PriDNSIPv6addpref2[] = { 0,0,0,0,0,0,0,0 };
USHORT SecDNSIPv6addpref2[] = { 0,0,0,0,0,0,0,0 };
BYTE   AddrAllocPref2 = 1;

const SLQSModifyProfileTestCaseStep_t
    SLQSModifyProfileTestCases[
                             MAX_SLQS_MODIFY_PROFILE_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE,               1,             &profileType22,
      &profileID12,          "profile1",             &pdpType2,
      &pdpHdrCompType2,      &pdpDataCompType2,      "wwww",
      &priDNSIPv4AddPref2,   &secDNSIPv4AddPref2,    &objUMTSQoS2,
      &objUMTSQoS2,          &objGPRSRequestedQoS2,  &objGPRSRequestedQoS2,
      "Sierra",              "12345",                &authenticationPref2,
      &IPv4AddrPref2,        &pcscfAddrUsingPCO2,    &pdpAccessConFlag2,
      &pcscfAddrUsingDhcp2,  &imCnFlag2,             &objTFTID1Params1,
      &objTFTID2Params2,     &pdpContext2,           &secondaryFlag2,
      &primaryID2,           &IPv6AddPref2[0],       &objUMTSReqQoSSigInd2,
      &objUMTSReqQoSSigInd2, &PriDNSIPv6addpref2[0], &SecDNSIPv6addpref2[0],
      &AddrAllocPref2,       &objQosClassID2,
      "All valid 3GPP:All optional parameter packed "},

    { eQCWWAN_ERR_NONE, 2,    &profileType, &profileID32, "Hello1",
      NULL,             NULL, NULL,         "http",         NULL,
      NULL,             NULL, NULL,         NULL,       NULL,
      "",                "",  NULL,         NULL,       NULL,
      NULL,             NULL, NULL,         NULL,       NULL,
      NULL,             NULL, NULL,         NULL,       NULL,
      NULL,             NULL, NULL,         NULL,       NULL,
     "All valid 3GPP: All optional parameter NULL "},

    { eQCWWAN_ERR_NONE, 2,    NULL, &profileID32, "Hello1",
      NULL,             NULL, NULL,         "http",         NULL,
      NULL,             NULL, NULL,         NULL,       NULL,
      "",                "",  NULL,         NULL,       NULL,
      NULL,             NULL, NULL,         NULL,       NULL,
      NULL,             NULL, NULL,         NULL,       NULL,
      NULL,             NULL, NULL,         NULL,       NULL,
     "All valid 3GPP: ProfileID IS NULL "},
};

/* 3GPP2 Profile Paramters */
BYTE   lprofileID = 105;
BYTE   lprofileType2 = 1;
BYTE   lnegoDnsSrvrPref = 0;
ULONG  lpppSessCloseTimerDO = 5;
ULONG  lpppSessCloseTimer1x = 6;
BYTE   lallowLinger = 0;
USHORT llcpAckTimeout = 5 ;
USHORT lipcpAckTimeout = 4;
USHORT lauthTimeout = 10 ;
BYTE   llcpCreqRetryCount =5;
BYTE   lipcpCreqRetryCount= 5;
BYTE   lauthRetryCount =5;
BYTE   lauthProtocol = 3;
BYTE   ldataRate = 1;
ULONG  lappType = 1;
BYTE   ldataMode = 1;
BYTE   lappPriority = 2;
BYTE   lpdnType =0;
BYTE   lisPcscfAddressNedded =0;
ULONG  lprimaryV4DnsAddress =0;
ULONG  lsecondaryV4DnsAddress= 0;
USHORT lpriV6DnsAddress = 0;
USHORT lsecV6DnsAddress = 0;

const SLQSModifyProfile2TestCaseStep_t
    SLQSModifyProfile2TestCases[
                             MAX_SLQS_MODIFY_PROFILE2_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE,     1,                      &lprofileType2,
      &lprofileID,          &lnegoDnsSrvrPref,      &lpppSessCloseTimerDO,
      &lpppSessCloseTimer1x, &lallowLinger,         &llcpAckTimeout,
      &lipcpAckTimeout,      &lauthTimeout,         &llcpCreqRetryCount,
      &lipcpCreqRetryCount,  &lauthRetryCount,      &lauthProtocol,
      "lSierra",             "",                    &ldataRate,
      &lappType,             &ldataMode,            &lappPriority,
      "www",                &lpdnType,              &lisPcscfAddressNedded,
      &lprimaryV4DnsAddress, &lsecondaryV4DnsAddress, &lpriV6DnsAddress,
      &lsecV6DnsAddress, "All valid 3GPP2:All optional parameter packed"},

    { eQCWWAN_ERR_NONE,   2,    &lprofileType2,  &lprofileID,
      NULL,               NULL, NULL,           NULL,
      NULL,               NULL, NULL,          NULL ,
      NULL,               NULL, NULL,             "",
        "",               NULL, NULL,           NULL,
      NULL,                 "", NULL,           NULL,
      NULL,              NULL,  NULL,           NULL,
      "Valid 3GPP2:All optional parameter NULL "},

    { eQCWWAN_ERR_NONE,   2,    &lprofileType2, NULL,
      NULL,               NULL, NULL,           NULL,
      NULL,               NULL, NULL,           NULL,
      NULL,               NULL, NULL,            "",
        "",               NULL, NULL,           NULL,
      NULL,                 "", NULL,           NULL,
      NULL,              NULL,  NULL,           NULL,
      "Valid 3GPP2: ProfileID is NULL "}
};

#endif /* __GOBI_API_WDS_TEST_H__ */
