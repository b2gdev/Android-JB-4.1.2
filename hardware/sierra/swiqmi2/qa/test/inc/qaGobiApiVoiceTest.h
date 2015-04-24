/**************
 *
 *  Filename: qaGobiApiVoiceTest.h
 *
 *  Purpose:  Contains prototypes for functions implementing specific tests
 *            for Voice called by the main() routine in qatest.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc., all rights reserved
 *
 **************/

#ifndef __GOBI_API_VOICE_TEST_H__
#define __GOBI_API_VOICE_TEST_H__

/* Defines */
#define MAX_VOICE_ORIGINATE_USSD_TESTCASE_NUM 0x01
#define MAX_VOICE_ANSWER_USSD_TESTCASE_NUM    0x01
#define MAX_VOICE_CANCEL_USSD_TESTCASE_NUM    0x01

/* Structure for test cases step */
typedef struct _VoiceOriginateUSSDTestCase_t
{
    enum   eQCWWANError expectedReturnValue;
    ULONG               testCaseId;
    BYTE                ussDCS;
    BYTE                ussLen;
    BYTE                *ussData;
    CHAR                desc[100];
} VoiceOriginateUSSDTestCase_t;

typedef struct _VoiceAnswerUSSDTestCase_t
{
    enum   eQCWWANError expectedReturnValue;
    ULONG               testCaseId;
    BYTE                ussDCS;
    BYTE                ussLen;
    BYTE                *ussData;
    CHAR                desc[100];
} VoiceAnswerUSSDTestCase_t;

typedef struct _VoiceCancelUSSDTestCase_t
{
    enum   eQCWWANError expectedReturnValue;
    ULONG               testCaseId;
    CHAR                desc[100];
} VoiceCancelUSSDTestCase_t;

typedef struct _VoiceSLQSDialCallTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              *pCallNumber;
    BYTE              *pCallType;
    BYTE              *pCLIRType;
    UUSInfo           *pUUSInfo;
    CUGInfo           *pCUGInfo;
    BYTE              *pEmergencyCategory;
    calledPartySubAdd *pCallPartySubAdd;
    ULONG             *pSvcType;
    BYTE              *pCallID;
    alphaIDInfo       *pAlphaIDInfo;
    BYTE              *pCCResultType;
    ccSUPSType        *pCCSUPSType;
    CHAR              desc[100];
} VoiceSLQSDialCallTestCase_t;

typedef struct _SLQSVoiceEndCallTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              *pCallId;
    CHAR              desc[100];
} SLQSVoiceEndCallTestCase_t;

typedef struct _VoiceSLQSSetSUPSServiceTestCase_t
{
    enum eQCWWANError  expectedReturnValue;
    ULONG              testCaseId;
    /* Request Parameters */
    BYTE               voiceSvc;
    BYTE               reason;
    BYTE               *pServiceClass;
    BYTE               *pCallBarringPasswd;
    BYTE               *pCallForwardingNumber;
    BYTE               *pTimerVal;
    callFwdTypeAndPlan *pCallFwdTypeAndPlan;
    /* Response Parameters */
    WORD               *pFailCause;
    alphaIDInfo        *pAlphaIDInfo;
    BYTE               *pCCResultType;
    BYTE               *pCallID;
    ccSUPSType         *pCCSUPSType;
    CHAR               desc[100];
} VoiceSLQSSetSUPSServiceTestCase_t;

typedef struct _VoiceSLQSSetConfigTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    /* Request Parameters */
    BYTE              *pAutoAnswer;
    airTimer          *pAirTimerConfig;
    roamTimer         *pRoamTimerConfig;
    BYTE              *pTTYMode;
    prefVoiceSO       *pPrefVoiceSO;
    BYTE              *pPrefVoiceDomain;
    /* Response Parameters */
    BYTE              *pAutoAnsStatus;
    BYTE              *pAirTimerStatus;
    BYTE              *pRoamTimerStatus;
    BYTE              *pTTYConfigStatus;
    BYTE              *pPrefVoiceSOStatus;
    BYTE              *pVoiceDomainPrefStatus;
    CHAR              desc[100];
} VoiceSLQSSetConfigTestCase_t;

typedef struct _SLQSAnswerCallTestCase_t
{
    enum   eQCWWANError expectedReturnValue;
    ULONG               testCaseId;
    BYTE                *pCallId;
    CHAR                desc[100];
} SLQSAnswerCallTestCase_t;

typedef struct _VoiceSLQSGetCLIRTestCase_t
{
    enum   eQCWWANError expectedReturnValue;
    ULONG               testCaseId;
    /* Response Parameters */
    CLIRResp            *pCLIRResp;
    WORD                *pFailCause;
    alphaIDInfo         *pAlphaIDInfo;
    BYTE                *pCCResType;
    BYTE                *pCallID;
    ccSUPSType          *pCCSUPSType;
    CHAR                desc[100];
} VoiceSLQSGetCLIRTestCase_t;

typedef struct _VoiceSLQSGetCLIPTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    /* Response Parameters */
    CLIPResp          *pCLIPResp;
    WORD              *pFailCause;
    alphaIDInfo       *pAlphaIDInfo;
    BYTE              *pCCResType;
    BYTE              *pCallID;
    ccSUPSType        *pCCSUPSType;
    CHAR              desc[100];
} VoiceSLQSGetCLIPTestCase_t;

typedef struct _VoiceSLQSGetCallWaitingTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    /* Response Parameters */
    BYTE              *pSvcClass;
    WORD              *pFailCause;
    alphaIDInfo       *pAlphaIDInfo;
    BYTE              *pCCResType;
    BYTE              *pCallID;
    ccSUPSType        *pCCSUPSType;
    CHAR              desc[100];
} VoiceSLQSGetCallWaitingTestCase_t;

typedef struct _VoiceSLQSGetCallBarringTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    /* Request Parameters */
    BYTE              reason;
    /* Response Parameters */
    BYTE              *pSvcClass;
    WORD              *pFailCause;
    alphaIDInfo       *pAlphaIDInfo;
    BYTE              *pCCResType;
    BYTE              *pCallID;
    ccSUPSType        *pCCSUPSType;
    CHAR              desc[100];
} VoiceSLQSGetCallBarringTestCase_t;

typedef struct _VoiceSLQSGetCallFWStatusTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    /* Request Parameters */
    BYTE              reason;
    BYTE              *pSvcClass;
    /* Response Parameters */
    getCallFWInfo     *pGetCallFWInfo;
    WORD              *pFailCause;
    alphaIDInfo       *pAlphaIDInfo;
    BYTE              *pCCResType;
    BYTE              *pCallID;
    ccSUPSType        *pCCSUPSType;
    getCallFWExtInfo  *pGetCallFWExtInfo;
    CHAR              desc[100];
} VoiceSLQSGetCallFWStatusTestCase_t;

typedef struct _VoiceSLQSSetCallBarringPwdTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    /* Request Parameters */
    BYTE              reason;
    CHAR              *pOldPassword;
    CHAR              *pNewPassword;
    CHAR              *pNewPasswordAgain;
    /* Response Parameters */
    WORD              *pFailCause;
    alphaIDInfo       *pAlphaIDInfo;
    BYTE              *pCCResType;
    BYTE              *pCallID;
    ccSUPSType        *pCCSUPSType;
    CHAR              desc[100];
} VoiceSLQSSetCallBarringPwdTestCase_t;

typedef struct _VoiceSLQSGetCallInfoTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    /* Request Parameter */
    BYTE              callID;
    /* Response Parameters */
    callInfo          *pCallInfo;
    remotePartyNum    *pRemotePartyNum;
    WORD              *pSrvOpt;
    BYTE              *pVoicePrivacy;
    BYTE              *pOTASPStatus;
    remotePartyName   *pRemotePartyName;
    UUSInfo           *pUUSInfo;
    BYTE              *pAlertType;
    alphaIDInfo       *pAlphaIDInfo;
    connectNumInfo    *pConnectNumInfo;
    diagInfo          *pDiagInfo;
    ULONG             *pAlertingPattern;
    CHAR              desc[100];

} VoiceSLQSGetCallInfoTestCase_t;

typedef struct _VoiceSLQSGetAllCallInfoTestCase_t
{
    enum eQCWWANError  expectedReturnValue;
    ULONG              testCaseId;
    /* Response Parameters */
    arrCallInfo        *pArrCallInfo;
    arrRemotePartyNum  *pArrRemotePartyNum;
    arrRemotePartyName *pArrRemotePartyName;
    arrAlertingType    *pArrAlertingType;
    arrUUSInfo         *pArrUUSInfo;
    arrSvcOption       *pArrSvcOption;
    BYTE               *pOTASPStatus;
    BYTE               *pVoicePrivacy;
    arrCallEndReason   *pArrCallEndReason;
    arrAlphaID         *pArrAlphaID;
    arrConnectPartyNum *pArrConnectPartyNum;
    arrDiagInfo        *pArrDiagInfo;
    arrCalledPartyNum  *pArrCalledPartyNum;
    arrRedirPartyNum   *pArrRedirPartyNum;
    arrAlertingPattern *pArrAlertingPattern;
    CHAR               desc[100];

} VoiceSLQSGetAllCallInfoTestCase_t;

typedef struct _VoiceSLQSVoiceManageCallsTestCases_t
{
    enum   eQCWWANError expectedReturnValue;
    ULONG               testCaseId;
    BYTE                SUPSType;
    BYTE                *pCallID;
    WORD                *pFailCause;
    CHAR                desc[100];
} VoiceSLQSVoiceManageCallsTestCase_t;

typedef struct _SLQSVoiceBurstDTMFTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    burstDTMFInfo     BurstDTMFInfo;
    DTMFLengths       *pBurstDTMFLengths;
    CHAR              desc[100];
} SLQSVoiceBurstDTMFTestCase_t;

typedef struct _SLQSVoiceStartContDTMFTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              *pCallID;
    BYTE              DTMFdigit;
    CHAR              desc[100];
} SLQSVoiceStartContDTMFTestCase_t;

typedef struct _SLQSVoiceStopContDTMFTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              callID;
    CHAR              desc[100];
} SLQSVoiceStopContDTMFTestCase_t;

typedef struct _SLQSVoiceSendFlashTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              *pCallID;
    BYTE              *pFlashPayLd;
    BYTE              *pFlashType;
    CHAR              desc[100];
} SLQSVoiceSendFlashTestCase_t;

typedef struct _SLQSVoiceSetPrefPrivacyTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              privacyPref;
    CHAR              desc[100];
} SLQSVoiceSetPrefPrivacyTestCase_t;

typedef struct _SLQSVoiceIndicationRegisterTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              *pRegDTMFEvents;
    BYTE              *pRegVoicePrivacyEvents;
    BYTE              *pSuppsNotifEvents;
    CHAR              desc[100];
} SLQSVoiceIndicationRegisterTestCase_t;

typedef struct _VoiceSLQSGetConfigTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    /* Request Parameters */
    BYTE              *pAutoAnswer;
    BYTE              *pAirTimer;
    BYTE              *pRoamTimer;
    BYTE              *pTTYMode;
    BYTE              *pPrefVoiceSO;
    BYTE              *pAMRStatus;
    BYTE              *pPrefVoicePrivacy;
    BYTE              *pNamID;
    BYTE              *pVoiceDomainPref;
    /* Response Parameters */
    BYTE              *pAutoAnswerStat;
    airTimer          *pAirTimerCnt;
    roamTimer         *pRoamTimerCnt;
    BYTE              *pCurrTTYMode;
    prefVoiceSO       *pCurPrefVoiceSO;
    curAMRConfig      *pCurAMRConfig;
    BYTE              *pCurVoicePrivacyPref;
    BYTE              *pCurVoiceDomainPref;
    CHAR              desc[100];
} VoiceSLQSGetConfigTestCase_t;

typedef struct _SLQSVoiceOrigUSSDNoWaitTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    struct USSInfo    USSInformation;
    CHAR              desc[100];
} SLQSVoiceOrigUSSDNoWaitTestCase_t;

typedef struct _SLQSVoiceBindSubscriptionTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              subsType;
    CHAR              desc[100];
} SLQSVoiceBindSubscriptionTestCase_t;

typedef struct _SLQSVoiceALSSetLineSwitchingTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              switchOption;
    CHAR              desc[100];
} SLQSVoiceALSSetLineSwitchingTestCase_t;

typedef struct _SLQSVoiceALSSelectLineTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              lineValue;
    CHAR              desc[100];
} SLQSVoiceALSSelectLineTestCase_t;

typedef struct _VoiceSLQSGetCOLPTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    /* Response Parameters */
    COLPResp          *pCOLPResp;
    WORD              *pFailCause;
    alphaIDInfo       *pAlphaIDInfo;
    BYTE              *pCCResType;
    BYTE              *pCallID;
    ccSUPSType        *pCCSUPSType;
    CHAR              desc[100];
} VoiceSLQSGetCOLPTestCase_t;

typedef struct _VoiceSLQSGetCOLRTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    /* Response Parameters */
    COLRResp          *pCOLRResp;
    WORD              *pFailCause;
    alphaIDInfo       *pAlphaIDInfo;
    BYTE              *pCCResType;
    BYTE              *pCallID;
    ccSUPSType        *pCCSUPSType;
    CHAR              desc[100];
} VoiceSLQSGetCOLRTestCase_t;

typedef struct _VoiceSLQSGetCNAPTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    /* Response Parameters */
    CNAPResp          *pCNAPResp;
    WORD              *pFailCause;
    alphaIDInfo       *pAlphaIDInfo;
    BYTE              *pCCResType;
    BYTE              *pCallID;
    ccSUPSType        *pCCSUPSType;
    CHAR              desc[100];
} VoiceSLQSGetCNAPTestCase_t;

#endif /* __GOBI_API_VOICE_TEST_H__ */
