/**
 * \ingroup voice
 *
 * \file    qaGobiApiVoice.h
 *
 * \brief   Voice Service API function prototypes
 *
 * Copyright: © 2011-12 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __GOBI_API_VOICE_H__
#define __GOBI_API_VOICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MAXUSSDLENGTH          182
#define MAX_CALL_NO_LEN        81
#define MAX_DESCRIPTION_LENGTH 255
#define PASSWORD_LENGTH        4
#define MAX_NO_OF_CALLS        20

/**
 *  This structure contains USS Information
 *
 *  \param  ussDCS
 *          - 1 - ASCII coding scheme
 *          - 2 - 8-BIT coding scheme
 *          - 3 - UCS2
 *
 *  \param  ussLen
 *          - Range 1 to 182
 *
 *  \param  ussData
 *          - Data encoded as per the DCS
 *
 */
struct USSInfo
{
    BYTE ussDCS;
    BYTE ussLen;
    BYTE ussData[MAXUSSDLENGTH];
};

/**
 *  Initiates a USSD operation.
 *
 *  \param  pInfo[IN]
 *          - USS information
 *          - See \ref USSInfo for more details
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5\n
 *          Timeout: 5 mins
 *
 */
ULONG OriginateUSSD(
    BYTE  *pInfo );

/**
 *  Responds to a USSD request from the network.
 *
 *  \param  pInfo[IN]
 *          - USS information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5\n
 *          Timeout: 5 mins
 *
 */
ULONG AnswerUSSD(
    BYTE  *pInfo );

/**
 *  Cancels an in-progress USSD operation.
 *
 *  \param  None
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5\n
 *          Timeout: 30 Secs
 *
 */
ULONG CancelUSSD();

/**
 *  This structure contains User to User Signaling Service Information.
 *
 *  \param  UUSType
 *          - UUS type values are:
 *              - 0x00 - UUS_DATA
 *              - 0x01 - UUS_TYPE1_IMPLICIT
 *              - 0x02 - UUS_TYPE1_REQUIRED
 *              - 0x03 - UUS_TYPE1_NOT_REQUIRED
 *              - 0x04 - UUS_TYPE2_REQUIRED
 *              - 0x05 - UUS_TYPE2_NOT_REQUIRED
 *              - 0x06 - UUS_TYPE3_REQUIRED
 *              - 0x07 - UUS_TYPE3_NOT_REQUIRED
 *              - 0xFF - Not Available
 *
 *  \param  UUSDcs
 *          - UUS data coding scheme values are:
 *              - 0x01 - UUS_DCS_USP
 *              - 0x02 - UUS_DCS_OHLP
 *              - 0x03 - UUS_DCS_X244
 *              - 0x04 - UUS_DCS_SMCF
 *              - 0x05 - UUS_DCS_IA5
 *              - 0x06 - UUS_DCS_RV12RD
 *              - 0x07 - UUS_DCS_Q931UNCCM
 *              - 0xFF - Not Available
 *
 *  \param  UUSDatalen
 *          - Number of sets of the following elements.
 *              - UUSData
 *          - If zero(0) then no further information exists.
 *
 *  \param  UUSData[MAX_DESCRIPTION_LENGTH]
 *          - UUS data encoded as per coding scheme
 *
 */
typedef struct
{
    BYTE UUSType;
    BYTE UUSDcs;
    BYTE UUSDatalen;
    BYTE UUSData[MAX_DESCRIPTION_LENGTH];
} UUSInfo;

/**
 *  This structure contains Closed User Group Information
 *
 *  \param  CUGIndex
 *          - Range 0x00... 0x7FFF
 *
 *  \param  SuppPrefCUG
 *          - Suppress preferential CUG
 *              - 0x00 - FALSE
 *              - 0x01 - TRUE
 *
 *  \param  SuppOA
 *          - Suppress OA subscription option
 *              - 0x00 - FALSE
 *              - 0x01 - TRUE
 *
 */
typedef struct
{
    WORD CUGIndex;
    BYTE SuppPrefCUG;
    BYTE SuppOA;
} CUGInfo;

/**
 *  This structure contains information about the Called Sub Party Addresses.
 *
 *  \param  extBit
 *          - Extension bit.
 *
 *  \param  subAddrType
 *          - Subaddress type.
 *              - 0x00 - NSAP
 *              - 0x01 - USER
 *
 *  \param  oddEvenInd
 *          - Even/odd indicator.
 *              - 0x00 - Even number of address signals
 *              - 0x01 - Odd number of address signals
 *
 *  \param  subAddrLen
 *          - Number of sets of the following elements:
 *              - SubAddress
 *
 *  \param  subAddr[MAX_DESCRIPTION_LENGTH]
 *          - Array of the SubAddress in BCD number format.
 *
 */
typedef struct
{
    BYTE extBit;
    BYTE subAddrType;
    BYTE oddEvenInd;
    BYTE subAddrLen;
    BYTE subAddr[MAX_DESCRIPTION_LENGTH];
} calledPartySubAdd;

/**
 *  This structure contains Voice Call Request Parameters
 *
 *  \param  callNumber[81]
 *          - Number to be dialed in ASCII string, NULL terminated.
 *          - Length Range [1 to 81]
 *
 *  \param  pCallType
 *          - the type of call to be dialed. CALL_TYPE_VOICE is automatically
 *            selected if this parameter is not provided.
 *            When CALL_TYPE_NON_STD_OTASP is selected, the call is sent as a
 *            nonstandard OTASP call regardless of the digit string
 *            Call type values are:
 *              - 0x00 - CALL_TYPE_VOICE - Voice (automatic selection)
 *              - 0x01 - CALL_TYPE_VOICE_FORCED -
 *                  Avoid modem call classification
 *              - 0x08 - CALL_TYPE_NON_STD_OTASP - Nonstandard OTASP*
 *              - 0x09 - CALL_TYPE_EMERGENCY - Emergency
 *
 *  \param  pCLIRType
 *          - CLIR type values are:
 *              - 0x01 - CLIR_SUPPRESSION - Suppression
 *              - 0x02 - CLIR_INVOCATION - Invocation
 *
 *  \param  pUUSInFo
 *          - Pointer to structure of UUSInfo
 *              - See \ref UUSInfo for more information
 *
 *  \param  pCUGInfo
 *          - Pointer to structure of CUGInfo
 *              - See \ref CUGInfo for more information
 *
 *  \param  pEmergencyCategory
 *          - Bit mask of emergency number categories. This is only applicable
 *            when the call type is set to Emergency.
 *               - Bit 0 - VOICE_EMER_CAT_POLICE_BIT - Police
 *               - Bit 1 - VOICE_EMER_CAT_AMBULANCE_BIT - Ambulance
 *               - Bit 2 - VOICE_EMER_CAT_FIRE_BRIGADE_BIT- Fire brigade
 *               - Bit 3 - VOICE_EMER_CAT_MARINE_GUARD_BIT - Marine guard
 *               - Bit 4 - VOICE_EMER_CAT_MOUNTAIN_RESCUE_BIT - Mountain rescue
 *               - Bit 5 - VOICE_EMER_CAT_MANUAL_ECALL_BIT -
 *                   Manual emergency call
 *               - Bit 6 - VOICE_EMER_CAT_AUTO_ECALL_BIT -
 *                   Automatic emergency call
 *               - Bit 7 - VOICE_EMER_CAT_SPARE_BIT - Spare bit
 *
 *  \param  pCallPartySubAdd
 *          - Pointer to structure of calledPartySubAdd
 *              - See \ref calledPartySubAdd for more information
 *
 *  \param  pSvcType
 *          - Service Type.
 *              - 0x01 - VOICE_DIAL_CALL_SRV_TYPE_AUTOMATIC - Automatic
 *              - 0x02 - VOICE_DIAL_CALL_SRV_TYPE_GSM - GSM
 *              - 0x03 - VOICE_DIAL_CALL_SRV_TYPE_WCDMA - WCDMA
 *              - 0x04 - VOICE_DIAL_CALL_SRV_TYPE_CDMA_AUTOMATIC -
 *                  CDMA automatic
 *              - 0x05 - VOICE_DIAL_CALL_SRV_TYPE_GSM_WCDMA - GSM or WCDMA
 *              - 0x06 - VOICE_DIAL_CALL_SRV_TYPE_LTE -LTE
 */
typedef struct
{
    BYTE              callNumber[MAX_CALL_NO_LEN];
    BYTE              *pCallType;
    BYTE              *pCLIRType;
    UUSInfo           *pUUSInFo;
    CUGInfo           *pCUGInfo;
    BYTE              *pEmergencyCategory;
    calledPartySubAdd *pCallPartySubAdd;
    ULONG             *pSvcType;
} voiceCallRequestParams;

/**
 *  This structure contains information about the Alpha Identifier.
 *
 *  \param  alphaDcs
 *          - Alpha coding scheme
 *              - 0x01 - GSM Default_Char
 *              - 0x02 - UCS2
 *              - 0xFF - Not Available
 *
 *  \param  alphaLen
 *          - Number of sets of the following elements:
 *              - pAlpha_text
 *          - If zero(0) then no further information exists.
 *
 *  \param  alphaText[MAX_DESCRIPTION_LENGTH]
 *          - Data encoded as per the alpha_dcs
 *
 */
typedef struct
{
   BYTE alphaDcs;
   BYTE alphaLen;
   BYTE alphaText[MAX_DESCRIPTION_LENGTH];
} alphaIDInfo;

/**
 *  This structure contains information about the Call Control Supplementary
 *  Service Types
 *
 *  \param  svcType
 *          - Service type.
 *              - 0x01 - VOICE_CC_SUPS_RESULT_SERVICE_TYPE_ACTIVATE -
 *                  Activate
 *              - 0x02 - VOICE_CC_SUPS_RESULT_SERVICE_TYPE_DEACTIVATE -
 *                  Deactivate
 *              - 0x03 - VOICE_CC_SUPS_RESULT_SERVICE_TYPE_REGISTER -
 *                  Register
 *              - 0x04 - VOICE_CC_SUPS_RESULT_SERVICE_TYPE_ERASE - Erase
 *              - 0x05 - VOICE_CC_SUPS_RESULT_SERVICE_TYPE_INTERROGATE -
 *                  Interrogate
 *              - 0x06 - VOICE_CC_SUPS_RESULT_SERVICE_TYPE_REGISTER_PASSWORD -
 *                  Register password
 *              - 0x07 - VOICE_CC_SUPS_RESULT_SERVICE_TYPE_USSD - USSD
 *              - 0xFF - Not Available
 *
 *  \param  reason
 *          - Call control supplementary service result reason
 *          - Values:
 *              - See qaGobiApiTableCallControlReturnReasons.h for return reasons.
 *
 */
typedef struct
{
   BYTE svcType;
   BYTE reason;
} ccSUPSType;

/**
 *  This structure contains Voice Call Response Parameters
 *
 *  \param  pCallID
 *          - Unique call identifier for the dialed call
 *
 *  \param  pAlphaIDInfo
 *          - Pointer to structure of alphaIDInfo
 *              - See \ref alphaIDInfo for more information
 *
 *  \param  pCCResultType
 *          - Call Control Result Type.
 *              - 0x00 - CC_RESULT_TYPE_VOICE - Voice
 *              - 0x01 - CC_RESULT_TYPE_SUPS - Supplementary service
 *              - 0x02 - CC_RESULT_TYPE_USSD -
 *                  Unstructured supplementary service
 *
 *  \param  pCCSUPSType
 *          - Pointer to structure of ccSUPSType
 *          - Data is present when pCCResultType is present and
 *            is other than Voice.
 *              - See \ref ccSUPSType for more information
 *
 */
typedef struct
{
    BYTE        *pCallID;
    alphaIDInfo *pAlphaIDInfo;
    BYTE        *pCCResultType;
    ccSUPSType  *pCCSUPSType;
} voiceCallResponseParams;

/**
 *  Originates a voice call (MO call).
 *
 *  \param  pCallRequestParams[IN]
 *          - Pointer to structure of voiceCallRequestParams
 *              - See \ref voiceCallRequestParams for more information
 *
 *  \param  pCallResponseParams[OUT]
 *          - Pointer to structure of voiceCallResponseParams
 *              - See \ref voiceCallResponseParams for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          This API originates a voice call (MO).
 *          If the function returns success with a call_id, the device
 *          has started the requested operation. It does not mean that the call
 *          has been connected. SLQSVoiceSetAllCallStatusCallback() callback
 *          can be subscribed to learn if the call was successful.
 *
 */
ULONG SLQSVoiceDialCall( voiceCallRequestParams  *pCallRequestParams,
                         voiceCallResponseParams *pCallResponseParams );

/**
 *  This message ends a voice call
 *
 *  \param  pCallId [IN/OUT]
 *          - Unique call identifier for the call that must be ended
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          If the function returns success, the device has started the
 *          requested operation. It does not mean that the call has been ended.
 *          The application should always process the
 *          SLQSVoiceSetAllCallStatusCallback() callback to learn if the call
 *          was ended.
 *
 */
ULONG SLQSVoiceEndCall(
    BYTE *pCallId );

/**
 *  Service Class information
 */
enum serviceClassInformation
{
    VOICE_SUPS_SRV_CLASS_NONE             = 0x00,
    VOICE_SUPS_SRV_CLASS_VOICE            = 0x01,
    VOICE_SUPS_SRV_CLASS_DATA             = 0x02,
    VOICE_SUPS_SRV_CLASS_FAX              = 0x04,
    VOICE_SUPS_SRV_CLASS_SMS              = 0x08,
    VOICE_SUPS_SRV_CLASS_DATACIRCUITSYNC  = 0x10,
    VOICE_SUPS_SRV_CLASS_DATACIRCUITASYNC = 0x20,
    VOICE_SUPS_SRV_CLASS_PACKETACCESS     = 0x40,
    VOICE_SUPS_SRV_CLASS_PADACCESS        = 0x80,
};

/**
 *  This structure contains Supplementary Service request parameters related to
 *  different features and their activation, deactivation, registration
 *  and erasure (applicable only for 3GPP)
 *
 *  \param  numberType
 *          - Call forwarding number type
 *              - 0x00 - QMI_VOICE_NUM_TYPE_UNKNOWN
 *                  Unknown
 *              - 0x01 - QMI_VOICE_NUM_TYPE_INTERNATIONAL
 *                  International
 *              - 0x02 - QMI_VOICE_NUM_TYPE_NATIONAL
 *                  National
 *              - 0x03 - QMI_VOICE_NUM_TYPE_NETWORK_SPECIFIC
 *                  Network-specific
 *              - 0x04 - QMI_VOICE_NUM_TYPE_SUBSCRIBER
 *                  Subscriber
 *              - 0x05 - QMI_VOICE_NUM_TYPE_RESERVED
 *                  Reserved
 *              - 0x06 - QMI_VOICE_NUM_TYPE_ABBREVIATED
 *                  Abbreviated
 *              - 0x07 - QMI_VOICE_NUM_TYPE_RESERVED_EXTENSION
 *                  Reserved extension
 *
 *  \param  numberPlan
 *          - Call forwarding number plan
 *              - 0x00 - QMI_VOICE_NUM_PLAN_UNKNOWN
 *                  Unknown
 *              - 0x01 - QMI_VOICE_NUM_PLAN_ISDN
 *                  ISDN
 *              - 0x03 - QMI_VOICE_NUM_PLAN_DATA
 *                  Data
 *              - 0x04 - QMI_VOICE_NUM_PLAN_TELEX
 *                  Telex
 *              - 0x08 - QMI_VOICE_NUM_PLAN_NATIONAL
 *                  National
 *              - 0x09 - QMI_VOICE_NUM_PLAN_PRIVATE
 *                  Private
 *              - 0x0B - QMI_VOICE_NUM_PLAN_RESERVED_CTS
 *                  Reserved cordless telephony system
 *              - 0x0F - QMI_VOICE_NUM_PLAN_RESERVED_EXTENSION
 *                  Reserved extension
 */
typedef struct
{
    BYTE numberType;
    BYTE numberPlan;
} callFwdTypeAndPlan;

/**
 *  This structure contains Supplementary Service request parameters related to
 *  different features and their activation, deactivation, registration
 *  and erasure (applicable only for 3GPP)
 *
 *  \param  voiceSvc
 *          - Manages all call-independent supplementary services, such as
 *            activation, deactivation, registration, and erasure (mandatory)
 *              - 0x01 - VOICE_SERVICE_ACTIVATE
 *              - 0x02 - VOICE_SERVICE_DEACTIVATE
 *              - 0x03 - VOICE_SERVICE_REGISTER
 *              - 0x04 - VOICE_SERVICE_ERASE
 *
 *  \param  reason
 *          - supplementary service reason values (mandatory)
 *              - 0x01 - QMI_VOICE_REASON_FWD_UNCONDITIONAL\n
 *                   Unconditional call forwarding
 *              - 0x02 - QMI_VOICE_REASON_FWD_MOBILEBUSY\n
 *                  Forward when the mobile is busy
 *              - 0x03 - QMI_VOICE_REASON_FWD_NOREPLY\n
 *                  Forward when there is no reply
 *              - 0x04 - QMI_VOICE_REASON_FWD_UNREACHABLE\n
 *                  Forward when the call is unreachable
 *              - 0x05 - QMI_VOICE_REASON_FWD_ALLFORWARDING\n
 *                  All forwarding
 *              - 0x06 - QMI_VOICE_REASON_FWD_ALLCONDITIONAL\n
 *                  All conditional forwarding
 *              - 0x07 - QMI_VOICE_REASON_BARR_ALLOUTGOING\n
 *                  All outgoing calls are barred
 *              - 0x08 - QMI_VOICE_REASON_BARR_OUTGOINGINT\n
 *                  Outgoing internal calls are barred
 *              - 0x09 - QMI_VOICE_REASON_BARR_OUTGOINGINTEXTOHOME\n
 *                  Outgoing calls external to home are barred
 *              - 0x0A - QMI_VOICE_REASON_BARR_ALLINCOMING\n
 *                  All incoming calls are barred
 *              - 0x0B - QMI_VOICE_REASON_BARR_INCOMINGROAMING\n
 *                  Roaming incoming calls are barred
 *              - 0x0C - QMI_VOICE_REASON_BARR_ALLBARRING\n
 *                  All calls are barred
 *              - 0x0D - QMI_VOICE_REASON_BARR_ALLOUTGOINGBARRING\n
 *                  All outgoing calls are barred
 *              - 0x0E - QMI_VOICE_REASON_BARR_ALLINCOMINGBARRING\n
 *                  All incoming calls are barred
 *              - 0x0F - QMI_VOICE_REASON_CALLWAITING\n
 *                  Call waiting
 *
 *  \param  pServiceClass
 *          - Service class is a combination (sum) of information
 *            class constants (optional)
 *              - See \ref serviceClassInformation for more information
 *
 *  \param  pCallBarringPasswd
 *          - Password is required if call barring is provisioned using a
 *            password. Password consists of 4 ASCII digits.
 *            Range: 0000 to 9999 (optional)
 *
 *  \param  pCallForwardingNumber
 *          - Call forwarding number to be registered with the network. This has
 *            to be included in the request only when the service is set to
 *            VOICE_SERVICE_REGISTER. NULL terminated ASCII string. (optional)
 *
 *  \param  pTimerVal
 *          - Call forwarding no reply timer value in seconds. This has to be
 *            included in the request only when the service is set to
 *            VOICE_SERVICE_REGISTER and the reason is
 *            QMI_VOICE_REASON_FWD_NOREPLY. (optional)
 *              - Range: 5 to 30 in steps of 5
 *
 *  \param  pCallFwdTypeAndPlan
 *          - Information about call forwarding type and plan. This parameter is
 *            ignored when the Call Forwarding Number is not included (optional)
 *              - See \ref callFwdTypeAndPlan for more information
 *
 */
typedef struct
{
    BYTE voiceSvc;
    BYTE reason;
    BYTE *pServiceClass;
    BYTE *pCallBarringPasswd;
    BYTE *pCallForwardingNumber;
    BYTE *pTimerVal;
    callFwdTypeAndPlan *pCallFwdTypeAndPlan;
} voiceSetSUPSServiceReq;

/**
 *  This structure contains Supplementary Service response parameters related to
 *  different features and their activation, deactivation, registration
 *  and erasure (applicable only for 3GPP)
 *
 *  \param  pFailCause
 *          - Supplementary service failure causes (optional, supply NULL if
 *            not required).
 *              - 0xFFFF is the value when the information is not received from
 *                device
 *
 *  \param  pAlphaIDInfo
 *          - Pointer to structure of alphaIDInfo. The parameter used to pass
 *            the alpha (if any) given by the SIM/R-UIM after call control
 *            (optional, supply NULL if not required)
 *              - See \ref alphaIDInfo for more information
 *
 *  \param  pCCResultType
 *          - Call control result types (optional, supply NULL if not required)
 *              - 0x00 - CC_RESULT_TYPE_VOICE - Voice
 *              - 0x01 - CC_RESULT_TYPE_SUPS - Supplementary service
 *              - 0x02 - CC_RESULT_TYPE_USSD - Unstructured supplementary service
 *              - 0xFF - if the device does not provide this information
 *
 *  \param  pCallID
 *          - Unique call identifier for the dialed call (optional, supply NULL
 *            if not required)
 *              - 0x00 - if the device does not provide this information
 *
 *  \param  pCCSUPSType
 *          - Data is present when pCCResultType is present and is other
 *            than Voice. (optional, supply NULL if not required)
 *              - See \ref ccSUPSType for more information
 */
typedef struct
{
    WORD        *pFailCause;
    alphaIDInfo *pAlphaIDInfo;
    BYTE        *pCCResultType;
    BYTE        *pCallID;
    ccSUPSType  *pCCSUPSType;
} voiceSetSUPSServiceResp;

/**
 *  This API manages call-independent supplementary services, e.g., activation
 *  of call forwarding (to forward incoming calls to a third party), activation
 *  of call barring (to request the network to block some of the call attempts),
 *  and activation of call waiting (to be notified of an incoming call even when
 *  the user is engaged in an active or held call).
 *
 *  \param  pVoiceSetSUPSServiceReq [IN]
 *          - Pointer to structure of voiceSetSUPSServiceReq
 *              - See \ref voiceSetSUPSServiceReq for more information
 *
 *  \param  pVoiceSetSUPSServiceResp [OUT]
 *          - Pointer to structure of voiceSetSUPSServiceResp
 *              - See \ref voiceSetSUPSServiceResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          Some of the call-independent services are provided by the network
 *          operator as part of the service agreement. If they are not
 *          provided by default, the user has to explicitly request them.
 *          This API provides the facility to the control point for
 *          sending the explicit request to the network for enabling/disabling
 *
 */
ULONG SLQSVoiceSetSUPSService (
    voiceSetSUPSServiceReq  *pVoiceSetSUPSServiceReq,
    voiceSetSUPSServiceResp *pVoiceSetSUPSServiceResp );

/**
 *  This structure contains information about the Air Timer.
 *
 *  \param  namID
 *          - Index of the NAM(Number Assignment Module) to be configured.
 *          - Range 0 to 3.
 *          - Some modems support only 1 or 2 NAMs.
 *          - 0xFF,if not available.
 *
 *  \param  airTimerValue
 *          - Time in minutes.
 *          - Cumulative air time is slammed.
 *          - 0xFFFFFFFF,if not available.
 *
 */
typedef struct
{
    BYTE  namID;
    ULONG airTimerValue;
} airTimer;

/**
 *  This structure contains information about the Roam Timer.
 *
 *  \param  namID
 *          - Index of the NAM(Number Assignment Module) to be configured.
 *          - Range 0 to 3.
 *          - Some modems support only 1 or 2 NAMs.
 *          - 0xFF,if not available.
 *
 *  \param  roamTimerValue
 *          - Time in minutes.
 *          - Cumulative air time is slammed.
 *          - 0xFFFFFFFF,if not available.
 *
 */
typedef struct
{
    BYTE  namID;
    ULONG roamTimerValue;
} roamTimer;

/**
 *  This structure contains information about the Preferred Voice Service Options.
 *
 *  \param  namID
 *          - Index of the NAM(Number Assignment Module) to be configured.
 *          - Range 0 to 3.
 *          - Some modems support only 1 or 2 NAMs.
 *          - 0xFF,if not available.
 *
 *  \param  evrcCapability
 *          - EVRC capability.
 *          - Values:
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *              - 0xFF - Not Available
 *
 *  \param  homePageVoiceSO
 *          - Home page voice SO; most preferred CDMA SO to be requested from
 *            the network when receiving an incoming (MT) voice call within
 *            the home network.
 *          - Values:
 *              - 0x0000 - VOICE_SO_WILD - Any service option
 *              - 0x0001 - VOICE_SO_IS_96A - IS-96A
 *              - 0x0003 - VOICE_SO_EVRC - EVRC
 *              - 0x0011 - VOICE_SO_13K_IS733 - 13K_IS733
 *              - 0x0038 - VOICE_SO_SELECTABLE_MODE_VOCODER -
 *                  Selectable mode vocoder
 *              - 0x0044 - VOICE_SO_4GV_NARR0W_BAND - 4GV narrowband
 *              - 0x0046 - VOICE_SO_4GV_WIDE_BAND - 4GV wideband
 *              - 0x8000 - VOICE_SO_13K - 13K
 *              - 0x8001 - VOICE_SO_IS_96 - IS-96
 *              - 0x8023 - VOICE_SO_WVRC - WVRC
 *              - 0xFFFF - Not Available
 *
 *  \param  homeOrigVoiceSO
 *          - Home origination voice SO; most preferred CDMA SO to be requested
 *            from the network when receiving an incoming (MT) voice call within
 *            the home network.
 *          - Values:
 *              - 0x0000 - VOICE_SO_WILD - Any service option
 *              - 0x0001 - VOICE_SO_IS_96A - IS-96A
 *              - 0x0003 - VOICE_SO_EVRC - EVRC
 *              - 0x0011 - VOICE_SO_13K_IS733 - 13K_IS733
 *              - 0x0038 - VOICE_SO_SELECTABLE_MODE_VOCODER -
 *                  Selectable mode vocoder
 *              - 0x0044 - VOICE_SO_4GV_NARR0W_BAND - 4GV narrowband
 *              - 0x0046 - VOICE_SO_4GV_WIDE_BAND - 4GV wideband
 *              - 0x8000 - VOICE_SO_13K - 13K
 *              - 0x8001 - VOICE_SO_IS_96 - IS-96
 *              - 0x8023 - VOICE_SO_WVRC - WVRC
 *              - 0xFFFF - Not Available
 *
 *  \param  roamOrigVoiceSO
 *          - Roaming origination voice SO; most preferred CDMA SO to be
 *            requested from the network when receiving an incoming (MT)
 *            voice call within the home network.
 *          - Values:
 *              - 0x0000 - VOICE_SO_WILD - Any service option
 *              - 0x0001 - VOICE_SO_IS_96A - IS-96A
 *              - 0x0003 - VOICE_SO_EVRC - EVRC
 *              - 0x0011 - VOICE_SO_13K_IS733 - 13K_IS733
 *              - 0x0038 - VOICE_SO_SELECTABLE_MODE_VOCODER -
 *                  Selectable mode vocoder
 *              - 0x0044 - VOICE_SO_4GV_NARR0W_BAND - 4GV narrowband
 *              - 0x0046 - VOICE_SO_4GV_WIDE_BAND - 4GV wideband
 *              - 0x8000 - VOICE_SO_13K - 13K
 *              - 0x8001 - VOICE_SO_IS_96 - IS-96
 *              - 0x8023 - VOICE_SO_WVRC - WVRC
 *              - 0xFFFF - Not Available
 *
 */
typedef struct
{
    BYTE namID;
    BYTE evrcCapability;
    WORD homePageVoiceSO;
    WORD homeOrigVoiceSO;
    WORD roamOrigVoiceSO;
} prefVoiceSO;

/**
 *  This structure contains information about the Set Configuration
 *  Request Parameters.
 *
 *  \param  pAutoAnswer
 *          - Value specified is written to NV_AUTO_ANSWER_I. (optional)
 *          - Values:
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pAirTimerConfig
 *          - Value specified is written to NV_AIR_CNT_I. (optional)
 *          - See \ref airTimer for more information
 *
 *  \param  pRoamTimerConfig
 *          - Value specified is written to NV_ROAM_CNT_I. (optional)
 *          - See \ref roamTimer for more information
 *
 *  \param  pTTYMode
 *          - Value specified is written to NV_TTY_I. (optional)
 *          - Values:
 *              - 0x00 - TTY_MODE_FULL - Full
 *              - 0x01 - TTY_MODE_VCO - Voice carry over
 *              - 0x02 - TTY_MODE_HCO - Hearing carry over
 *              - 0x03 - TTY_MODE_OFF - Off
 *
 *  \param  pPrefVoiceSO
 *          - Value specified is written to NV_PREF_VOICE_SO_I. (optional)
 *          - See \ref prefVoiceSO for more information
 *
 *  \param  pPrefVoiceDomain
 *          - Preferred Voice-Domain. (optional)
 *          - Values:
 *              - 0x00 - VOICE_DOMAIN_PREF_CS_ONLY -
 *                  Circuit-switched (CS) only
 *              - 0x01 - VOICE_DOMAIN_PREF_PS_ONLY -
 *                  Packet-switched (PS) only
 *              - 0x02 - VOICE_DOMAIN_PREF_CS_PREF -
 *                  CS is preferred, PS is secondary
 *              - 0x03 - VOICE_DOMAIN_PREF_PS_PREF -
 *                  PS is preferred, CS is secondary
 *
 *  \note  One of the optional parameters must be present in the request.
 *
 */
typedef struct
{
    BYTE        *pAutoAnswer;
    airTimer    *pAirTimerConfig;
    roamTimer   *pRoamTimerConfig;
    BYTE        *pTTYMode;
    prefVoiceSO *pPrefVoiceSO;
    BYTE        *pPrefVoiceDomain;
} voiceSetConfigReq;

/**
 *  This structure contains information about the Set Configuration
 *  Response Parameters.
 *
 *  \param  pAutoAnsStatus
 *          - Auto Answer Status. (optional)
 *          - Values:
 *              - 0x00 - Information was written successfully
 *              - 0x01 - Information write failed
 *              - 0xFF - Not Available.
 *
 *  \param  pAirTimerStatus
 *          - Air Timer Status. (optional)
 *          - Values:
 *              - 0x00 - Information was written successfully
 *              - 0x01 - Information write failed
 *              - 0xFF - Not Available.
 *
 *  \param  pRoamTimerStatus
 *          - Roam Timer Status. (optional)
 *          - Values:
 *              - 0x00 - Information was written successfully
 *              - 0x01 - Information write failed
 *              - 0xFF - Not Available.
 *
 *  \param  pTTYConfigStatus
 *          - TTY Config Status. (optional)
 *          - Values:
 *              - 0x00 - Information was written successfully
 *              - 0x01 - Information write failed
 *              - 0xFF - Not Available.
 *
 *  \param  pPrefVoiceSOStatus
 *          - Preferred Voice SO Status. (optional)
 *          - Values:
 *              - 0x00 - Information was written successfully
 *              - 0x01 - Information write failed
 *              - 0xFF - Not Available.
 *
 *  \param  pVoiceDomainPrefStatus
 *          - Voice-Domain Preference Status. (optional)
 *          - Values:
 *              - 0x00 - Information was written successfully
 *              - 0x01 - Information write failed
 *              - 0xFF - Not Available.
 *
 *  \note  Parameters which are mentioned as NULL will be ignored.
 *
 */
typedef struct
{
    BYTE *pAutoAnsStatus;
    BYTE *pAirTimerStatus;
    BYTE *pRoamTimerStatus;
    BYTE *pTTYConfigStatus;
    BYTE *pPrefVoiceSOStatus;
    BYTE *pVoiceDomainPrefStatus;
} voiceSetConfigResp;

/**
 *  This message sets various configuration parameters that control the modem
 *  behavior related to circuit-switched services.
 *
 *  \param  pVoiceSetConfigReq[IN]
 *          - Pointer to structure of voiceSetConfigReq
 *              - See \ref voiceSetConfigReq for more information
 *
 *  \param  pVoiceSetConfigResp[OUT]
 *          - Pointer to structure of voiceSetConfigResp
 *              - See \ref voiceSetConfigResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          Any invalid value in a request message causes the device to
 *          reject the message without updating any configuration information.
 *          In the case of a successful update of all requested information, a
 *          QMI_ERR_NONE error is returned. In the case where a subset of
 *          information failed to be written, a QMI_ERR_INTERNAL error is
 *          returned with corresponding optional information requested
 *          in the request message.
 *
 */
ULONG SLQSVoiceSetConfig (
        voiceSetConfigReq  *pVoiceSetConfigReq,
        voiceSetConfigResp *pVoiceSetConfigResp );

/**
 *  Contains the parameters passed for SLQSVoiceAnswerCall.
 *
 *  \param  pCallId[IN/OUT]
 *          - Unique call identifier for the call that must be answered.
 *
 */
typedef struct
{
    BYTE *pCallId;
} voiceAnswerCall;

/**
 *  Answers an incoming voice call.
 *
 *  \param  pVoiceAnswerCall[IN/OUT]
 *          - Pointer to structure of voiceAnswerCall
 *              - See \ref voiceAnswerCall for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 10 Secs\n
 *          This API is used to answer an incoming voice call when the
 *          incoming voice call is the only call present at that time. If
 *          there are other calls while an incoming call (waiting call) is
 *          received, API "SLQSVoiceSendFlash" can be used case of 3GPP2(CDMA)
 *          and API "SLQSVoiceManageCalls" in the case of 3GPP(UMTS). If the
 *          result indicates success,the device has started the requested
 *          operation and it does not mean that the call has been answered.
 *          "SLQSVoiceSetAllCallStatusCallback" can be subscribed to check
 *          the call Information/State.
 *
 */
ULONG SLQSVoiceAnswerCall(
    voiceAnswerCall *pVoiceAnswerCall );

/**
 *  This structure contains information about the Calling Line Identification
 *  Restriction (CLIR) supplementary service responses.
 *
 *  \param  ActiveStatus
 *          - Active status.
 *          - Values:
 *              - 0x00 - ACTIVE_STATUS_INACTIVE - Inactive
 *              - 0x01 - ACTIVE_STATUS_ACTIVE - Active
 *              - 0xFF - Not Available
 *
 *  \param  ProvisionStatus
 *          - Provisioned status.
 *          - Values:
 *              - 0x00 - PROVISION_STATUS_NOT_PROVISIONED - Not provisioned
 *              - 0x01 - PROVISION_STATUS_PROVISIONED_PERMANENT -
 *                  Permanently provisioned
 *              - 0x02 - PROVISION_STATUS_PRESENTATION_RESTRICTED -
 *                  Restricted presentation
 *              - 0x03 - PROVISION_STATUS_PRESENTATION_ALLOWED -
 *                  Allowed presentation
 *              - 0xFF - Not Available
 *
 */
typedef struct
{
    BYTE ActiveStatus;
    BYTE ProvisionStatus;
} CLIRResp;

/**
 *  This structure contains Voice Get Calling Line Identification
 *  Restriction (CLIR) Response Parameters
 *
 *  \param  pCLIRResp
 *          - Pointer to structure of CLIRResp (optional)
 *              - See \ref CLIRResp for more information
 *
 *  \param  pFailCause
 *          - Supplementary services failure cause (optional)
 *          - see qaGobiApiTableVoiceCallEndReasons.h for more information.
 *          - 0xFFFF,if Not Available
 *
 *  \param  pAlphaIDInfo
 *          - Pointer to structure of alphaIDInfo (optional)
 *              - See \ref alphaIDInfo for more information
 *
 *  \param  pCCResType
 *          - Call Control Result Type (optional)
 *              - 0x00 - CC_RESULT_TYPE_VOICE - Voice
 *              - 0x01 - CC_RESULT_TYPE_SUPS - Supplementary service
 *              - 0x02 - CC_RESULT_TYPE_USSD -
 *                  Unstructured supplementary service
 *              - 0xFF - Not Available
 *
 *  \param  pCallID
 *          - Call ID of the voice call that resulted from call
 *            control. (optional)
 *          - It is present when pCCResType is present and is Voice.
 *          - If zero(0) then invalid.
 *
 *  \param  pCCSUPSType
 *          - Supplementary service data that resulted from call
 *            control (optional)
 *          - Data is present when pCCResultType is present and
 *            is other than Voice.
 *              - See \ref ccSUPSType for more information
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not returned or has default value.
 *
 */
typedef struct
{
   CLIRResp    *pCLIRResp;
   WORD        *pFailCause;
   alphaIDInfo *pAlphaIDInfo;
   BYTE        *pCCResType;
   BYTE        *pCallID;
   ccSUPSType  *pCCSUPSType;
} voiceGetCLIRResp;

/**
 *  Queries the status of the Calling Line Identification Restriction (CLIR)
 *  supplementary service (applicable only for 3GPP).
 *
 *  \param  pVoiceGetCLIRResp[OUT]
 *          - Pointer to structure of voiceGetCLIRResp
 *              - See \ref voiceGetCLIRResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          This API queries the status of the CLIR supplementary service.
 *          The active_status field is only applicable when provision_status
 *          is PROVISIONED, i.e., there is not any case where provision_status
 *          is NOT_PROVISIONED and active_status is ACTIVE.
 *          The request is a blocking request, i.e., the response is sent only
 *          after confirmation is received from the network. The maximum time
 *          it takes for the response to be sent is approximately 30 sec.
 *          This API is applicable only in 3GPP devices.
 *
 */
ULONG SLQSVoiceGetCLIR( voiceGetCLIRResp  *pVoiceGetCLIRResp );

/**
 *  This structure contains information about the Calling Line Identification
 *  Presentation (CLIP) supplementary service responses.
 *
 *  \param  ActiveStatus
 *          - Active status.
 *          - Values:
 *              - 0x00 - ACTIVE_STATUS_INACTIVE - Inactive
 *              - 0x01 - ACTIVE_STATUS_ACTIVE - Active
 *              - 0xFF - Not Available
 *
 *  \param  ProvisionStatus
 *          - Provisioned status.
 *          - Values:
 *              - 0x00 - PROVISION_STATUS_NOT_PROVISIONED - Not provisioned
 *              - 0x01 - PROVISION_STATUS_PROVISIONED - Provisioned
 *              - 0xFF - Not Available
 *
 */
typedef struct
{
    BYTE ActiveStatus;
    BYTE ProvisionStatus;
} CLIPResp;

/**
 *  This structure contains Voice Get Calling Line Identification
 *  Presentation(CLIP) Response Parameters
 *
 *  \param  pCLIPResp
 *          - Pointer to structure of CLIPResp (optional)
 *              - See \ref CLIPResp for more information
 *
 *  \param  pFailCause
 *          - Supplementary services failure cause (optional)
 *          - see qaGobiApiTableVoiceCallEndReasons.h for more information.
 *          - 0xFFFF,if Not Available
 *
 *  \param  pAlphaIDInfo
 *          - Pointer to structure of alphaIDInfo (optional)
 *              - See \ref alphaIDInfo for more information
 *
 *  \param  pCCResType
 *          - Call Control Result Type (optional)
 *              - 0x00 - CC_RESULT_TYPE_VOICE - Voice
 *              - 0x01 - CC_RESULT_TYPE_SUPS - Supplementary service
 *              - 0x02 - CC_RESULT_TYPE_USSD -
 *                  Unstructured supplementary service
 *              - 0xFF - Not Available
 *
 *  \param  pCallID
 *          - Call ID of the voice call that resulted from call
 *            control. (optional)
 *          - It is present when pCCResType is present and is Voice.
 *          - If zero(0) then invalid.
 *
 *  \param  pCCSUPSType
 *          - Supplementary service data that resulted from call
 *            control (optional)
 *          - Data is present when pCCResultType is present and
 *            is other than Voice.
 *              - See \ref ccSUPSType for more information
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not returned or has default value.
 *
 */
typedef struct
{
   CLIPResp    *pCLIPResp;
   WORD        *pFailCause;
   alphaIDInfo *pAlphaIDInfo;
   BYTE        *pCCResType;
   BYTE        *pCallID;
   ccSUPSType  *pCCSUPSType;
} voiceGetCLIPResp;

/**
 *  Queries the status of the Calling Line Identification Presentation (CLIP)
 *  supplementary service (applicable only for 3GPP).
 *
 *  \param  pVoiceGetCLIPResp[OUT]
 *          - Pointer to structure of voiceGetCLIPResp
 *              - See \ref voiceGetCLIPResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          This API queries the status of the CLIP supplementary service.
 *          The active_status field is only applicable when provision_status is
 *          PROVISIONED, i.e., there is not any case where provision_status is
 *          NOT_PROVISIONED and active_status is ACTIVE.
 *          The request is a blocking request, i.e., the response is sent only
 *          after confirmation is received from the network. The maximum time
 *          it takes for the response to be sent is approximately 30 sec.
 *          This API is applicable only in 3GPP devices.
 *
 */
ULONG SLQSVoiceGetCLIP( voiceGetCLIPResp *pVoiceGetCLIPResp );

/**
 *  This structure contains Voice Get Call Waiting Response Parameters
 *
 *  \param  pSvcClass[IN/OUT]
 *          - Service class is a combination (sum) of information class
 *            constants (optional)
 *          - See qaGobiApiTableSupServiceInfoClasses.h for service classes.
 *          - Service Class is set to 0 if call waiting is not active for any
 *            of the information classes.
 *          - 0xFF,if Not Available
 *
 *  \param  pFailCause
 *          - Supplementary services failure cause (optional)
 *          - see qaGobiApiTableVoiceCallEndReasons.h for more information.
 *          - 0xFFFF,if Not Available
 *
 *  \param  pAlphaIDInfo
 *          - Pointer to structure of alphaIDInfo (optional)
 *              - See \ref alphaIDInfo for more information
 *
 *  \param  pCCResType
 *          - Call Control Result Type (optional)
 *              - 0x00 - CC_RESULT_TYPE_VOICE - Voice
 *              - 0x01 - CC_RESULT_TYPE_SUPS - Supplementary service
 *              - 0x02 - CC_RESULT_TYPE_USSD -
 *                  Unstructured supplementary service
 *              - 0xFF - Not Available
 *
 *  \param  pCallID
 *          - Call ID of the voice call that resulted from call
 *            control. (optional)
 *          - It is present when pCCResType is present and is Voice.
 *          - If zero(0) then invalid.
 *
 *  \param  pCCSUPSType
 *          - Supplementary service data that resulted from call
 *            control (optional)
 *          - Data is present when pCCResultType is present and
 *            is other than Voice.
 *              - See \ref ccSUPSType for more information
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not returned or has default value.
 *
 */
typedef struct
{
   BYTE        *pSvcClass;
   WORD        *pFailCause;
   alphaIDInfo *pAlphaIDInfo;
   BYTE        *pCCResType;
   BYTE        *pCallID;
   ccSUPSType  *pCCSUPSType;
} voiceGetCallWaitInfo;

/**
 *  Queries the status of Call Waiting Supplementary Service
 *  (applicable only for 3GPP).
 *
 *  \param  pVoiceGetCallWaitInfo[IN/OUT]
 *          - Pointer to structure of voiceGetCallWaitInfo
 *              - See \ref voiceGetCallWaitInfo for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          This API queries the status of the call waiting supplementary
 *          service, i.e., to find whether the call waiting supplementary
 *          service is active.
 *          The request is a blocking request, i.e., the response is sent only
 *          after confirmation is received from the network. The maximum time
 *          it takes for the response to be sent is approximately 30 sec.
 *          This API is applicable only in 3GPP devices.
 *
 */
ULONG SLQSVoiceGetCallWaiting( voiceGetCallWaitInfo *pVoiceGetCallWaitInfo );

/**
 *  This structure contains Voice Get Call Barring Request Parameters
 *
 *  \param  reason
 *          - Call Barring Reason
 *          - Values:
 *              - 0x07 - QMI_VOICE_REASON_BARR_ALLOUTGOING - All outgoing
 *              - 0x08 - QMI_VOICE_REASON_BARR_OUTGOINGINT - Outgoing internal
 *              - 0x09 - QMI_VOICE_REASON_BARR_OUTGOINGINTEXTOHOME -
 *                  Outgoing external to home
 *              - 0x0A - QMI_VOICE_REASON_BARR_ALLINCOMING - All incoming
 *              - 0x0B - QMI_VOICE_REASON_BARR_INCOMINGROAMING -
 *                  Roaming incoming
 *              - 0x0C - QMI_VOICE_REASON_BARR_ALLBARRING -
 *                  All calls are barred
 *              - 0x0D - QMI_VOICE_REASON_BARR_ALLOUTGOINGBARRING -
 *                  All outgoing calls are barred
 *              - 0x0E - QMI_VOICE_REASON_BARR_ALLINCOMINGBARRING -
 *                  All incoming calls are barred
 *
 *  \param  pSvcClass[IN/OUT]
 *          - Service class is a combination (sum) of information class
 *            constants (optional)
 *          - See qaGobiApiTableSupServiceInfoClasses.h for service classes.
 *          - Service Class is set to 0 if call waiting is not active for any
 *            of the information classes.
 *          - 0xFF,if Not Available
 *
 */
typedef struct
{
   BYTE reason;
   BYTE *pSvcClass;
} voiceGetCallBarringReq;

/**
 *  This structure contains Voice Get Call Barring Response Parameters
 *
 *  \param  pSvcClass[IN/OUT]
 *          - Service class is a combination (sum) of information class
 *            constants (optional)
 *          - See qaGobiApiTableSupServiceInfoClasses.h for service classes.
 *          - Service Class is set to 0 if call waiting is not active for any of
 *            the information classes.
 *          - 0xFF,if Not Available
 *
 *  \param  pFailCause
 *          - Supplementary services failure cause (optional)
 *          - see qaGobiApiTableVoiceCallEndReasons.h for more information.
 *          - 0xFFFF,if Not Available
 *
 *  \param  pAlphaIDInfo
 *          - Pointer to structure of alphaIDInfo (optional)
 *              - See \ref alphaIDInfo for more information
 *
 *  \param  pCCResType
 *          - Call Control Result Type  (optional)
 *              - 0x00 - CC_RESULT_TYPE_VOICE - Voice
 *              - 0x01 - CC_RESULT_TYPE_SUPS - Supplementary service
 *              - 0x02 - CC_RESULT_TYPE_USSD -
 *                  Unstructured supplementary service
 *              - 0xFF - Not Available
 *
 *  \param  pCallID
 *          - Call ID of the voice call that resulted from call
 *            control. (optional)
 *          - It is present when pCCResType is present and is Voice.
 *          - If zero(0) then invalid.
 *
 *  \param  pCCSUPSType
 *          - Supplementary service data that resulted from call
 *            control (optional)
 *          - Data is present when pCCResultType is present and
 *            is other than Voice.
 *              - See \ref ccSUPSType for more information
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not returned or has default value.
 *
 */
typedef struct
{
    BYTE        *pSvcClass;
    WORD        *pFailCause;
    alphaIDInfo *pAlphaIDInfo;
    BYTE        *pCCResType;
    BYTE        *pCallID;
    ccSUPSType  *pCCSUPSType;
} voiceGetCallBarringResp;

/**
 *  Queries the status of Call Barring Supplementary Service
 *  (applicable only for 3GPP).
 *
 *  \param  pVoiceGetCallBarringReq[IN]
 *          - Pointer to structure of voiceGetCallBarringReq
 *              - See \ref voiceGetCallBarringReq for more information
 *
 *  \param  pVoiceGetCallBarringResp[OUT]
 *          - Pointer to structure of voiceGetCallBarringResp
 *              - See \ref voiceGetCallBarringResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          This API queries the status of the call barring supplementary
 *          service, i.e., to find whether the call barring supplementary
 *          service is active and, if active, for which service classes it is
 *          active.
 *          The request is a blocking request, i.e., the response is sent only
 *          after confirmation is received from the network. The maximum time
 *          it takes for the response to be sent is approximately 30 sec.
 *          This API is applicable only in 3GPP devices.
 *
 */
ULONG SLQSVoiceGetCallBarring( voiceGetCallBarringReq  *pVoiceGetCallBarringReq,
                               voiceGetCallBarringResp *pVoiceGetCallBarringResp );

/**
 *  This structure contains Voice Get Call Forwarding Status Request Parameters
 *
 *  \param  Reason
 *          - Call Forwarding Reason
 *          - Values:
 *              - 0x01 - QMI_VOICE_REASON_FWDREASON_UNCONDITIONAL -
 *                  Unconditional call forwarding
 *              - 0x02 - QMI_VOICE_REASON_FWDREASON_MOBILEBUSY -
 *                  Forward when the mobile is busy
 *              - 0x03 - QMI_VOICE_REASON_FWDREASON_NOREPLY -
 *                  Forward when there is no reply
 *              - 0x04 - QMI_VOICE_REASON_FWDREASON_UNREACHABLE -
 *                  Forward when the call is unreachable
 *              - 0x05 - QMI_VOICE_REASON_FWDREASON_ALLFORWARDING -
 *                  All forwarding
 *              - 0x06 - QMI_VOICE_REASON_FWDREASON_ALLCONDITIONAL -
 *                  All conditional forwarding
 *
 *  \param  pSvcClass
 *          - Service Class is a combination (sum) of information class constants
 *          - See qaGobiApiTableSupServiceInfoClasses.h for service classes.
 *
 */
typedef struct
{
    BYTE Reason;
    BYTE *pSvcClass;
} voiceGetCallFWReq;

/**
 *  This structure contains information for Get Call Forwarding Information.
 *
 *  \param  SvcStatus
 *          - Service status. Values:
 *              - 0x00 - SERVICE_STATUS_INACTIVE - Inactive
 *              - 0x01 - SERVICE_STATUS_ACTIVE - Active
 *
 *  \param  SvcClass
 *          - Service Class is a combination (sum) of information class
 *            constants
 *          - See qaGobiApiTableSupServiceInfoClasses.h for service classes.
 *
 *  \param  numLen
 *          - Provides the length of number which follow.
 *
 *  \param  number[255]
 *          - number of numLen length, NULL terminated.
 *
 *  \param  noReplyTimer
 *          - No reply timer value in seconds
 *          - A value of 0 indicates that noReplyTimer is ignored.
 */
typedef struct
{
    BYTE SvcStatus;
    BYTE SvcClass;
    BYTE numLen;
    BYTE number[255];
    BYTE noReplyTimer;
} callFWInfo;

/**
 *  This structure contains information for Get Call Forwarding
 *  Extended Information.
 *
 *  \param  SvcStatus
 *          - Service status. Values:
 *              - 0x00 - SERVICE_STATUS_INACTIVE - Inactive
 *              - 0x01 - SERVICE_STATUS_ACTIVE - Active
 *
 *  \param  SvcClass
 *          - Service Class is a combination (sum) of information class
 *            constants
 *          - See qaGobiApiTableSupServiceInfoClasses.h for service classes.
 *
 *  \param  noReplyTimer
 *          - No reply timer value in seconds
 *          - A value of 0 indicates that no_reply_timer is ignored.
 *
 *  \param  PI
 *          - Presentation indicator; refer to [S1, Table 2.7.4.4-1]
 *            for valid values.
 *
 *  \param  SI
 *          - Number screening indicator.
 *          - Values:
 *              - 0x00 - QMI_VOICE_SI_USER_PROVIDED_NOT_SCREENED -
 *                  Provided user is not screened
 *              - 0x01 - QMI_VOICE_SI_USER_PROVIDED_VERIFIED_PASSED -
 *                  Provided user passed verification
 *              - 0x02 - QMI_VOICE_SI_USER_PROVIDED_VERIFIED_FAILED -
 *                  Provided user failed verification
 *              - 0x03 - QMI_VOICE_SI_NETWORK_PROVIDED - Provided network
 *
 *  \param  numType
 *          - Number type.
 *          - Values:
 *              - 0x00 - QMI_VOICE_NUM_TYPE_UNKNOWN - Unknown
 *              - 0x01 - QMI_VOICE_NUM_TYPE_INTERNATIONAL - International
 *              - 0x02 - QMI_VOICE_NUM_TYPE_NATIONAL - National
 *              - 0x03 - QMI_VOICE_NUM_TYPE_NETWORK_SPECIFIC - Network-specific
 *              - 0x04 - QMI_VOICE_NUM_TYPE_SUBSCRIBER - Subscriber
 *              - 0x05 - QMI_VOICE_NUM_TYPE_RESERVED - Reserved
 *              - 0x06 - QMI_VOICE_NUM_TYPE_ABBREVIATED - Abbreviated
 *              - 0x07 - QMI_VOICE_NUM_TYPE_RESERVED_EXTENSION -
 *                  Reserved extension
 *
 *  \param  numPlan
 *          - Number plan.
 *          - Values:
 *              - 0x00 - QMI_VOICE_NUM_PLAN_UNKNOWN - Unknown
 *              - 0x01 - QMI_VOICE_NUM_PLAN_ISDN - ISDN
 *              - 0x03 - QMI_VOICE_NUM_PLAN_DATA - Data
 *              - 0x04 - QMI_VOICE_NUM_PLAN_TELEX - Telex
 *              - 0x08 - QMI_VOICE_NUM_PLAN_NATIONAL - National
 *              - 0x09 - QMI_VOICE_NUM_PLAN_PRIVATE - Private
 *              - 0x0B - QMI_VOICE_NUM_PLAN_RESERVED_CTS -
 *                  Reserved cordless telephony system
 *              - 0x0F - QMI_VOICE_NUM_PLAN_RESERVED_EXTENSION -
 *                  Reserved extension
 *
 *  \param  numLen
 *          - Provides the length of number which follow.
 *
 *  \param  number[255]
 *          - number of numLen length, NULL terminated.
 *
 */
typedef struct
{
    BYTE SvcStatus;
    BYTE SvcClass;
    BYTE noReplyTimer;
    BYTE PI;
    BYTE SI;
    BYTE numType;
    BYTE numPlan;
    BYTE numLen;
    BYTE number[255];
} callFWExtInfo;

/**
 *  This structure contains an array of Call Forwarded Information.
 *
 *  \param  numInstances
 *          - Number of callFWInfo that follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  CallFWInfo[MAX_NO_OF_CALLS]
 *          - Array of callFWInfo.
 *              - See \ref callFWInfo for more information.
 *
 */
typedef struct
{
    BYTE       numInstances;
    callFWInfo CallFWInfo[MAX_NO_OF_CALLS];
} getCallFWInfo;

/**
 *  This structure contains an array of Call Forwarded Extended Information.
 *
 *  \param  numInstances
 *          - Number of callFWExtInfo that follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  CallFWExtInfo[MAX_NO_OF_CALLS]
 *          - Array of CallFWExtInfo.
 *              - See \ref CallFWExtInfo for more information.
 *
 */
typedef struct
{
    BYTE          numInstances;
    callFWExtInfo CallFWExtInfo[MAX_NO_OF_CALLS];
} getCallFWExtInfo;

/**
 *  This structure contains Voice Get Call Forwarding Status Response
 *  Parameters
 *
 *  \param  pGetCallFWInfo
 *          - Pointer to structure of getCallFWInfo (optional)
 *              - See \ref getCallFWInfo for more information
 *
 *  \param  pFailCause
 *          - Supplementary services failure cause (optional)
 *          - see qaGobiApiTableVoiceCallEndReasons.h for more information.
 *          - 0xFFFF,if Not Available
 *
 *  \param  pAlphaIDInfo
 *          - Pointer to structure of alphaIDInfo
 *              - See \ref alphaIDInfo for more information
 *
 *  \param  pCCResType
 *          - Call Control Result Type (optional)
 *              - 0x00 - CC_RESULT_TYPE_VOICE - Voice
 *              - 0x01 - CC_RESULT_TYPE_SUPS - Supplementary service
 *              - 0x02 - CC_RESULT_TYPE_USSD -
 *                  Unstructured supplementary service
 *              - 0xFF - Not Available
 *
 *  \param  pCallID
 *          - Call ID of the voice call that resulted from call
 *            control. (optional)
 *          - It is present when pCCResType is present and is Voice.
 *          - If zero(0) then invalid.
 *
 *  \param  pCCSUPSType
 *          - Supplementary service data that resulted from call
 *            control (optional)
 *          - Data is present when pCCResultType is present and
 *            is other than Voice.
 *              - See \ref ccSUPSType for more information
 *
 *  \param  pGetCallFWExtInfo
 *          - Pointer to structure of getCallFWExtInfo (optional)
 *              - See \ref getCallFWExtInfo for more information
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not returned or has default value.
 *
 */
typedef struct
{
    getCallFWInfo    *pGetCallFWInfo;
    WORD             *pFailCause;
    alphaIDInfo      *pAlphaIDInfo;
    BYTE             *pCCResType;
    BYTE             *pCallID;
    ccSUPSType       *pCCSUPSType;
    getCallFWExtInfo *pGetCallFWExtInfo;
} voiceGetCallFWResp;

/**
 *  Queries the status of Call Forwarding Supplementary Service.
 *
 *  \param  pVoiceGetCallFWReq[IN]
 *          - Pointer to structure of voiceGetCallFWReq
 *              - See \ref voiceGetCallFWReq for more information
 *
 *  \param  pVoiceGetCallFWResp[OUT]
 *          - Pointer to structure of voiceGetCallFWResp
 *              - See \ref voiceGetCallFWResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          This API queries the status of the call forwarding supplementary
 *          service, i.e., to find whether the call forwarding supplementary
 *          service is active and, if active, for which service classes and call
 *          forwarding number it is active.
 *          The request is a blocking request, i.e., the response is sent only
 *          after confirmation is received from the network. The maximum time
 *          it takes for the response to be sent is approximately 30 sec.
 *          This API is applicable only in 3GPP devices.
 *
 */
ULONG SLQSVoiceGetCallForwardingStatus( voiceGetCallFWReq  *pVoiceGetCallFWReq,
                                        voiceGetCallFWResp *pVoiceGetCallFWResp );

/**
 *  This structure contains Voice Set Call Barring Password Request Parameters
 *
 *  \param  Reason
 *          - Call Barring Reason
 *          - Values:
 *              - 0x07 - QMI_VOICE_REASON_BARR_ALLOUTGOING - All outgoing
 *              - 0x08 - QMI_VOICE_REASON_BARR_OUTGOINGINT - Outgoing internal
 *              - 0x09 - QMI_VOICE_REASON_BARR_OUTGOINGINTEXTOHOME -
 *                  Outgoing external to home
 *              - 0x0A - QMI_VOICE_REASON_BARR_ALLINCOMING - All incoming
 *              - 0x0B - QMI_VOICE_REASON_BARR_INCOMINGROAMING -
 *                  Roaming incoming
 *              - 0x0C - QMI_VOICE_REASON_BARR_ALLBARRING -
 *                  All calls are barred
 *              - 0x0D - QMI_VOICE_REASON_BARR_ALLOUTGOINGBARRING -
 *                  All outgoing calls are barred
 *              - 0x0E - QMI_VOICE_REASON_BARR_ALLINCOMINGBARRING -
 *                  All incoming calls are barred
 *
 *  \param  oldPasswd[PASSWORD_LENGTH]
 *          - Old password.
 *              - Password consists of 4 ASCII digits.
 *              - Range: 0000 to 9999.
 *
 *  \param  newPasswd[PASSWORD_LENGTH]
 *          - New password.
 *              - Password consists of 4 ASCII digits.
 *              - Range: 0000 to 9999.
 *
 *  \param  newPasswdAgain[PASSWORD_LENGTH]
 *          - New password Again.
 *              - Password consists of 4 ASCII digits.
 *              - Range: 0000 to 9999.
 *
 */
typedef struct
{
    BYTE Reason;
    BYTE oldPasswd[PASSWORD_LENGTH];
    BYTE newPasswd[PASSWORD_LENGTH];
    BYTE newPasswdAgain[PASSWORD_LENGTH];
} voiceSetCallBarringPwdInfo;

/**
 *  This structure contains Voice Set Call Barring Password Response Parameters
 *
 *  \param  pFailCause
 *          - Supplementary services failure cause (optional)
 *          - see qaGobiApiTableVoiceCallEndReasons.h for more information.
 *          - 0xFFFF,if Not Available
 *
 *  \param  pAlphaIDInfo
 *          - Pointer to structure of alphaIDInfo (optional)
 *              - See \ref alphaIDInfo for more information
 *
 *  \param  pCCResType
 *          - Call Control Result Type (optional)
 *              - 0x00 - CC_RESULT_TYPE_VOICE - Voice
 *              - 0x01 - CC_RESULT_TYPE_SUPS - Supplementary service
 *              - 0x02 - CC_RESULT_TYPE_USSD -
 *                  Unstructured supplementary service
 *              - 0xFF - Not Available
 *
 *  \param  pCallID
 *          - Call ID of the voice call that resulted from call
 *            control. (optional)
 *          - It is present when pCCResType is present and is Voice.
 *          - If zero(0) then invalid.
 *
 *  \param  pCCSUPSType
 *          - Supplementary service data that resulted from call
 *            control (optional)
 *          - Data is present when pCCResultType is present and
 *            is other than Voice.
 *              - See \ref ccSUPSType for more information
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not returned or has default value.
 *
 */
typedef struct
{
    WORD        *pFailCause;
    alphaIDInfo *pAlphaIDInfo;
    BYTE        *pCCResType;
    BYTE        *pCallID;
    ccSUPSType  *pCCSUPSType;
} voiceSetCallBarringPwdResp;

/**
 *  Sets a Call Barring Password (applicable only for 3GPP).
 *
 *  \param  pVoiceSetCallBarringPwdInfo[IN]
 *          - Pointer to structure of voiceSetCallBarringPwdInfo
 *              - See \ref voiceSetCallBarringPwdInfo for more information
 *
 *  \param  pSetCallBarringPwdResp[OUT]
 *          - Pointer to structure of voiceSetCallBarringPwdResp
 *              - See \ref voiceSetCallBarringPwdResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          This API changes the call barring supplementary service password.
 *          The request is a blocking request, i.e., the response is sent only
 *          after confirmation is received from the network. The maximum time
 *          it takes for the response to be sent is approximately 30 sec.
 *          This API is applicable only in 3GPP devices.
 *
 */
ULONG SLQSVoiceSetCallBarringPassword(
        voiceSetCallBarringPwdInfo *pVoiceSetCallBarringPwdInfo,
        voiceSetCallBarringPwdResp *pSetCallBarringPwdResp );

/**
 *  This structure contains Information about call state changes. For example,
 *  when an incoming call is received, this structure is populated and indicate
 *  the incoming call information. When this incoming call is answered, the
 *  call status changes from INCOMING to CONVERSATION, which means a change
 *  in the call information and this structure is populated again with the
 *  changes and notified to/retrived by the user.
 *
 *  \param  callID
 *          - Call identifier for the call queried for information.
 *          - If zero(0) then invalid.
 *
 *  \param  callState
 *          - Call state.
 *              - 0x01 - CALL_STATE_ORIGINATION - Origination
 *              - 0x02 - CALL_STATE_INCOMING - Incoming
 *              - 0x03 - CALL_STATE_CONVERSATION - Conversation
 *              - 0x04 - CALL_STATE_CC_IN_PROGRESS -
 *                 Call is originating but waiting for call control to complete
 *              - 0x05 - CALL_STATE_ALERTING - Alerting
 *              - 0x06 - CALL_STATE_HOLD - Hold
 *              - 0x07 - CALL_STATE_WAITING - Waiting
 *              - 0x08 - CALL_STATE_DISCONNECTING - Disconnecting
 *              - 0x09 - CALL_STATE_END - End
 *              - 0x0A - CALL_STATE_SETUP - MT call is in Setup state in 3GPP
 *              - 0xFF - Not Available
 *
 *  \param  callType
 *          - Call type.
 *              - 0x00 - CALL_TYPE_VOICE - Voice
 *              - 0x02 - CALL_TYPE_VOICE_IP - Voice over IP
 *              - 0x06 - CALL_TYPE_OTAPA - OTAPA
 *              - 0x07 - CALL_TYPE_STD_OTASP - Standard OTASP
 *              - 0x08 - CALL_TYPE_NON_STD_OTASP - Nonstandard OTASP
 *              - 0x09 - CALL_TYPE_EMERGENCY - Emergency
 *              - 0xFF - Not Available
 *
 *  \param  direction
 *          - Direction.
 *              - 0x01 - CALL_DIRECTION_MO - MO call
 *              - 0x02 - CALL_DIRECTION_MT - MT call
 *              - 0xFF - Not Available
 *
 *  \param  mode
 *          - Mode.
 *          - If the mode field is "0x01 - CDMA",the optional Service Option,
 *            Voice Privacy, and OTASP Status (only for OTASP calls) TLVs are
 *            included in the response.
 *              - 0x01 - CALL_MODE_CDMA - CDMA
 *              - 0x02 - CALL_MODE_GSM - GSM
 *              - 0x03 - CALL_MODE_UMTS - UMTS
 *              - 0x04 - CALL_MODE_LTE - LTE
 *              - 0x05 - CALL_MODE_TDS - TD-SCDMA
 *              - 0xFF - Not Available
 *
 */
typedef struct
{
    BYTE callID;
    BYTE callState;
    BYTE callType;
    BYTE direction;
    BYTE mode;
} callInfo;

/**
 *  This structure contains information about the numbers that are dialed from
 *  the device or from which a call is received on the device.
 *
 *  \param  presentationInd
 *          - Presentation indicator.
 *              - 0x00 - PRESENTATION_ALLOWED - Allowed presentation
 *              - 0x01 - PRESENTATION_RESTRICTED - Restricted presentation
 *              - 0x02 - PRESENTATION_NUM_UNAVAILABLE -
 *                 Unavailable presentation
 *              - 0x04 - PRESENTATION_PAYPHONE -
 *                 Payphone presentation (GSM/UMTS specific)
 *              - 0xFF - Not Available
 *
 *  \param  numLen
 *          - Provides the length of number which follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  remPartyNumber[MAX_CALL_NO_LEN]
 *          - Array of numbers in ASCII, NULL ending.
 *
 */
typedef struct
{
    BYTE presentationInd;
    BYTE numLen;
    BYTE remPartyNumber[MAX_CALL_NO_LEN];
} remotePartyNum;

/**
 *  This structure contains information about the names that are dialed from
 *  the device or from which a call is received on the device.
 *
 *  \param  namePI
 *          - Name presentation indicator.
 *              - 0x00 - PRESENTATION_NAME_PRESENTATION_ALLOWED -
 *                 Allowed presentation
 *              - 0x01 - PRESENTATION_NAME_PRESENTATION_RESTRICTED -
 *                 Restricted presentation
 *              - 0x02 - PRESENTATION_NAME_UNAVAILABLE -
 *                 Unavailable presentation
 *              - 0x03 - PRESENTATION_NAME_NAME_PRESENTATION_RESTRICTED -
 *                 Restricted name presentation
 *              - 0xFF - Not Available
 *
 *  \param  codingScheme
 *          - Refer to Table10 qaGobiApiTableCodingScheme.h for coding schemes
 *          - 0xFF - Not Available
 *
 *  \param  nameLen
 *          - Provides the length of name which follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  callerName[MAX_DESCRIPTION_LENGTH]
 *          - Name in ASCII, NULL ending.
 *
 */
typedef struct
{
    BYTE namePI;
    BYTE codingScheme;
    BYTE nameLen;
    BYTE callerName[MAX_DESCRIPTION_LENGTH];
} remotePartyName;

/**
 *  This structure contains information about the numbers connected to a device.
 *  It contains information such as number type, eg International or Local.
 *
 *  \param  numPresInd
 *          - Presentation indicator
 *              - 0x00 - PRESENTATION_ALLOWED - Allowed presentation
 *              - 0x01 - PRESENTATION_RESTRICTED - Restricted presentation
 *              - 0x02 - PRESENTATION_NUM_UNAVAILABLE -
 *                 Unavailable presentation
 *              - 0x04 - PRESENTATION_PAYPHONE -
 *                 Payphone presentation (GSM/UMTS specific)
 *              - 0xFF - Not Available
 *
 *  \param  screeningInd
 *          - Screening indicator.
 *              - 0x00 - QMI_VOICE_SI_USER_PROVIDED_NOT_SCREENED -
 *                 Provided user is not screened
 *              - 0x01 - QMI_VOICE_SI_USER_PROVIDED_VERIFIED_PASSED -
 *                 Provided user passed verification
 *              - 0x02 - QMI_VOICE_SI_USER_PROVIDED_VERIFIED_FAILED -
 *                 Provided user failed verification
 *              - 0x03 - QMI_VOICE_SI_NETWORK_PROVIDED -
 *                 Provided network
 *              - 0xFF - Not Available
 *
 *  \param  numType
 *          - Number type.
 *              - 0x00 - QMI_VOICE_NUM_TYPE_UNKNOWN - Unknown
 *              - 0x01 - QMI_VOICE_NUM_TYPE_INTERNATIONAL - International
 *              - 0x02 - QMI_VOICE_NUM_TYPE_NATIONAL - National
 *              - 0x03 - QMI_VOICE_NUM_TYPE_NETWORK_SPECIFIC - Network-specific
 *              - 0x04 - QMI_VOICE_NUM_TYPE_SUBSCRIBER - Subscriber
 *              - 0x05 - QMI_VOICE_NUM_TYPE_RESERVED - Reserved
 *              - 0x06 - QMI_VOICE_NUM_TYPE_ABBREVIATED - Abbreviated
 *              - 0x07 - QMI_VOICE_NUM_TYPE_RESERVED_EXTENSION -
 *                 Reserved extension
 *              - 0xFF - Not Available
 *
 *  \param  numPlan
 *          - Number plan.
 *              - 0x00 - QMI_VOICE_NUM_PLAN_UNKNOWN - Unknown
 *              - 0x01 - QMI_VOICE_NUM_PLAN_ISDN - ISDN
 *              - 0x03 - QMI_VOICE_NUM_PLAN_DATA - Data
 *              - 0x04 - QMI_VOICE_NUM_PLAN_TELEX - Telex
 *              - 0x08 - QMI_VOICE_NUM_PLAN_NATIONAL - National
 *              - 0x09 - QMI_VOICE_NUM_PLAN_PRIVATE - Private
 *              - 0x0B - QMI_VOICE_NUM_PLAN_RESERVED_CTS -
 *                 Reserved cordless telephony system
 *              - 0x0F - QMI_VOICE_NUM_PLAN_RESERVED_EXTENSION -
 *                 Reserved extension
 *              - 0xFF - Not Available
 *
 *  \param  callerIDLen
 *          - Provides the length of caller ID which follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  callerID[MAX_CALL_NO_LEN]
 *          - callerID of numLen length, NULL terminated.
 *
 */
typedef struct
{
    BYTE numPresInd;
    BYTE screeningInd;
    BYTE numType;
    BYTE numPlan;
    BYTE callerIDLen;
    BYTE callerID[MAX_CALL_NO_LEN];
} connectNumInfo;

/**
 *  This structure contains Diagnostic Information
 *
 *  \param  diagInfoLen
 *          - Provides the length of information which follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  diagnosticInfo[MAX_DESCRIPTION_LENGTH]
 *          - Diagnostic information.
 *
 */
typedef struct
{
   BYTE diagInfoLen;
   BYTE diagnosticInfo[MAX_DESCRIPTION_LENGTH];
} diagInfo;

/**
 *  This structure contains information of the request parameters associated
 *  with a call.
 *
 *  \param  callID
 *          - Call identifier for the call queried for information.
 *
 */
typedef struct
{
    BYTE callID;
} voiceCallInfoReq;

/**
 *  This structure contains information of the response parameters associated
 *  with a call.
 *
 *  \param  pCallInfo
 *          - See \ref callInfo for more information.
 *
 *  \param  pRemotePartyNum
 *          - See \ref remotePartyNum for more information.
 *
 *  \param  pSrvOpt
 *          - Service option
 *          - Applicable only for 3GPP2 devices.
 *          - See Table8 qaGobiApiTableServiceOptions.h for standard service
 *            option number assignments.
 *
 *  \param  pVoicePrivacy
 *          - Voice Privacy.
 *          - Applicable only for 3GPP2 devices.
 *          - Values.
 *              - 0x00 - VOICE_PRIVACY_STANDARD - Standard privacy
 *              - 0x01 - VOICE_PRIVACY_ENHANCED - Enhanced privacy
 *              - 0xFF - Not Available
 *
 *  \param  pOTASPStatus
 *          - OTASP status for the OTASP call.
 *          - Applicable only for 3GPP2 devices.
 *              - 0x00 - OTASP_STATUS_SPL_UNLOCKED -
 *              SPL unlocked; only for user-initiated OTASP
 *              - 0x01 - OTASP_STATUS_SPRC_RETRIES_EXCEEDED -
 *                 SPC retries exceeded; only for user-initiated OTASP
 *              - 0x02 - OTASP_STATUS_AKEY_EXCHANGED -
 *                 A-key exchanged; only for user-initiated OTASP
 *              - 0x03 - OTASP_STATUS_SSD_UPDATED - SSD updated; for both
 *                 user-initiated OTASP and network-initiated OTASP (OTAPA)
 *              - 0x04 - OTASP_STATUS_NAM_DOWNLOADED - NAM downloaded;
 *                 only for user-initiated OTASP
 *              - 0x05 - OTASP_STATUS_MDN_DOWNLOADED - MDN downloaded;
 *                 only for user-initiated OTASP
 *              - 0x06 - OTASP_STATUS_IMSI_DOWNLOADED - IMSI downloaded;
 *                 only for user-initiated OTASP
 *              - 0x07 - OTASP_STATUS_PRL_DOWNLOADED - PRL downloaded;
 *                 only for user-initiated OTASP
 *              - 0x08 - OTASP_STATUS_COMMITTED - Commit successful;
 *                 only for user-initiated OTASP
 *              - 0x09 - OTASP_STATUS_OTAPA_STARTED - OTAPA started;
 *                 only for network-initiated OTASP (OTAPA)
 *              - 0x0A - OTASP_STATUS_OTAPA_STOPPED - OTAPA stopped;
 *                 only for network-initiated OTASP (OTAPA)
 *              - 0x0B - OTASP_STATUS_OTAPA_ABORTED - OTAPA aborted;
 *                 only for network-initiated OTASP (OTAPA)
 *              - 0x0C - OTASP_STATUS_OTAPA_COMMITTED - OTAPA committed;
 *                 only for network-initiated OTASP (OTAPA)
 *              - 0xFF - Not Available
 *
 *  \param  pRemotePartyName
 *          - Applicable only for 3GPP devices.
 *          - See \ref remotePartyName for more information.
 *
 *  \param  pUUSInfo
 *          - Applicable only for 3GPP devices.
 *          - See \ref UUSInfo for more information.
 *
 *  \param  pAlertType
 *          - Alerting type.
 *          - Applicable only for 3GPP devices.
 *              - 0x00 - ALERTING_LOCAL - Local
 *              - 0x01 - ALERTING_REMOTE - Remote
 *              - 0xFF - Not Available
 *
 *  \param  pAlphaIDInfo
 *          - Applicable only for 3GPP devices.
 *          - See \ref alphaIDInfo for more information.
 *
 *  \param  pConnectNumInfo
 *          - See \ref connectNumInfo for more information.
 *
 *  \param  pDiagInfo
 *          - See \ref diagInfo for more information.
 *
 *  \param  pAlertingPattern
 *          - Alerting pattern.
 *              - 0x00 - QMI_VOICE_ALERTING_PATTERN_1 - Pattern 1
 *              - 0x01 - QMI_VOICE_ALERTING_PATTERN_2 - Pattern 2
 *              - 0x02 - QMI_VOICE_ALERTING_PATTERN_3 - Pattern 3
 *              - 0x04 - QMI_VOICE_ALERTING_PATTERN_5 - Pattern 5
 *              - 0x05 - QMI_VOICE_ALERTING_PATTERN_6 - Pattern 6
 *              - 0x06 - QMI_VOICE_ALERTING_PATTERN_7 - Pattern 7
 *              - 0x07 - QMI_VOICE_ALERTING_PATTERN_8 - Pattern 8
 *              - 0x08 - QMI_VOICE_ALERTING_PATTERN_9 - Pattern 9
 *              - 0xFF - Not Available
 *
 */
typedef struct
{
    callInfo        *pCallInfo;
    remotePartyNum  *pRemotePartyNum;
    WORD            *pSrvOpt;
    BYTE            *pVoicePrivacy;
    BYTE            *pOTASPStatus;
    remotePartyName *pRemotePartyName;
    UUSInfo         *pUUSInfo;
    BYTE            *pAlertType;
    alphaIDInfo     *pAlphaIDInfo;
    connectNumInfo  *pConnectNumInfo;
    diagInfo        *pDiagInfo;
    ULONG           *pAlertingPattern;
} voiceCallInfoResp;

/**
 *  This API queries the information associated with a call and gives
 *  information about a particular call whose call Id is sent in as request.
 *
 *  \param  pGetCallInfoReq[IN]
 *          - See \ref voiceCallInfoReq for more information.
 *
 *  \param  pGetCallInfoResp[OUT]
 *          - See \ref voiceCallInfoResp for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          If no voice call is in progress or an invalid call_id is sent
 *          in the request, an error is returned as the response.\n
 *          This API requires a firmware with atleast voice 2.0 support.
 *
 */
ULONG SLQSVoiceGetCallInfo( voiceCallInfoReq  *pGetCallInfoReq,
                            voiceCallInfoResp *pGetCallInfoResp );

/**
 *  This structure contains information related to call state change.
 *
 *  \param  Callinfo
 *          - See \ref callInfo for more information.
 *
 *  \param  isEmpty
 *          - Multiparty indicator.
 *              - 0x00 - False
 *              - 0x01 - True
 *
 *  \param  ALS
 *          - Alternate Line Service line indicator.
 *          - Feature for supporting two different phone numbers on the same
 *            mobile device.
 *              - 0x00 - ALS_LINE1 - Line 1 (default)
 *              - 0x01 - ALS_LINE2 - Line 2
 *
 */
typedef struct
{
     callInfo Callinfo;
     BYTE     isEmpty;
     BYTE     ALS;
} getAllCallInformation;

/**
 *  This structure contains information for All Call Remote Party Numbers
 *
 *  \param  callID
 *          - Unique call identifier for the call.
 *
 *  \param  RemotePartyNum
 *          - See \ref remotePartyNum for more information.
 *
 */
typedef struct
{
    BYTE           callID;
    remotePartyNum RemotePartyNum;
} getAllCallRmtPtyNum;

/**
 *  This structure contains information for All Call Remote Party Names
 *
 *  \param  callID
 *          - Unique call identifier for the call.
 *
 *  \param  RemotePartyName
 *          - See \ref remotePartyName for more information.
 *
 */
typedef struct
{
    BYTE            callID;
    remotePartyName RemotePartyName;
} getAllCallRmtPtyName;

/**
 *  This structure contains information for User to User Signaling Service
 *  for All Calls.
 *
 *  \param  callID
 *          - Unique call identifier for the call.
 *
 *  \param  uusInfo
 *          - See \ref UUSInfo for more information.
 *
 */
typedef struct
{
    BYTE    callID;
    UUSInfo uusInfo;
} allCallsUUSInfo;

/**
 *  This structure contains information for Alpha Identifier for All Calls
 *
 *  \param  callID
 *          - Unique call identifier for the call.
 *
 *  \param  AlphaIDInfo
 *          - See \ref alphaIDInfo for more information.
 *
 */
typedef struct
{
    BYTE        callID;
    alphaIDInfo AlphaIDInfo;
} allCallsAlphaIDInfo;


/**
 *  This structure contains Diagnostic Information for All Calls
 *
 *  \param  callID
 *          - Unique call identifier for the call.
 *
 *  \param  DiagInfo
 *          - See \ref diagInfo for more information.
 *
 */
typedef struct
{
   BYTE     callID;
   diagInfo DiagInfo;
} allCallsDiagInfo;

/**
 *  This structure contains information for Connected Peer Numbers.
 *
 *  \param  callID
 *          - Unique call identifier for the call.
 *
 *  \param  numPI
 *          - Number presentation indicator.
 *              - 0x00 - PRESENTATION_ALLOWED - Allowed presentation
 *              - 0x01 - PRESENTATION_RESTRICTED - Restricted presentation
 *              - 0x02 - PRESENTATION_NUM_UNAVAILABLE -
 *                 Unavailable presentation
 *              - 0x04 - PRESENTATION_PAYPHONE -
 *                 Payphone presentation (GSM/UMTS specific)
 *              - 0xFF - Not Available
 *
 *  \param  numSI
 *          - Number screening indicator.
 *              - 0x00 - QMI_VOICE_SI_USER_PROVIDED_NOT_SCREENED -
 *                  Provided user is not screened
 *              - 0x01 - QMI_VOICE_SI_USER_PROVIDED_VERIFIED_PASSED -
 *                  Provided user passed verification
 *              - 0x02 - QMI_VOICE_SI_USER_PROVIDED_VERIFIED_FAILED -
 *                  Provided user failed verification
 *              - 0x03 - QMI_VOICE_SI_NETWORK_PROVIDED - Provided network
 *
 *  \param  numType
 *          - Number type.
 *              - 0x00 - QMI_VOICE_NUM_TYPE_UNKNOWN - Unknown
 *              - 0x01 - QMI_VOICE_NUM_TYPE_INTERNATIONAL - International
 *              - 0x02 - QMI_VOICE_NUM_TYPE_NATIONAL - National
 *              - 0x03 - QMI_VOICE_NUM_TYPE_NETWORK_SPECIFIC - Network-specific
 *              - 0x04 - QMI_VOICE_NUM_TYPE_SUBSCRIBER - Subscriber
 *              - 0x05 - QMI_VOICE_NUM_TYPE_RESERVED - Reserved
 *              - 0x06 - QMI_VOICE_NUM_TYPE_ABBREVIATED - Abbreviated
 *              - 0x07 - QMI_VOICE_NUM_TYPE_RESERVED_EXTENSION -
 *                  Reserved extension
 *
 *  \param  numPlan
 *          - Number plan.
 *              - 0x00 - QMI_VOICE_NUM_PLAN_UNKNOWN - Unknown
 *              - 0x01 - QMI_VOICE_NUM_PLAN_ISDN - ISDN
 *              - 0x03 - QMI_VOICE_NUM_PLAN_DATA - Data
 *              - 0x04 - QMI_VOICE_NUM_PLAN_TELEX - Telex
 *              - 0x08 - QMI_VOICE_NUM_PLAN_NATIONAL - National
 *              - 0x09 - QMI_VOICE_NUM_PLAN_PRIVATE - Private
 *              - 0x0B - QMI_VOICE_NUM_PLAN_RESERVED_CTS -
 *                  Reserved cordless telephony system
 *              - 0x0F - QMI_VOICE_NUM_PLAN_RESERVED_EXTENSION -
 *                  Reserved extension
 *
 *  \param  numLen
 *          - Provides the length of number which follow.
 *
 *  \param  number[MAX_CALL_NO_LEN]
 *          - number of numLen length, NULL terminated.
 *
 */
typedef struct
{
    BYTE callID;
    BYTE numPI;
    BYTE numSI;
    BYTE numType;
    BYTE numPlan;
    BYTE numLen;
    BYTE number[MAX_CALL_NO_LEN];
} peerNumberInfo;

/**
 *  This structure contains an array of Call Info
 *
 *  \param  numInstances
 *          - Number of getAllCallInfo that follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  getAllCallInfo[MAX_NO_OF_CALLS]
 *          - Array of CallInfo.
 *          - See \ref getAllCallInfo for more information.
 *
 */
typedef struct
{
    BYTE                  numInstances;
    getAllCallInformation getAllCallInfo[MAX_NO_OF_CALLS];
} arrCallInfo;

/**
 *  This structure contains an array of Remote Party Numbers
 *
 *  \param  numInstances
 *          - Number of remotePartyNum that follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  RmtPtyNum[MAX_NO_OF_CALLS]
 *          - Array of remotePartyNum.
 *          - See \ref getAllCallRmtPtyNum for more information.
 *
 */
typedef struct
{
    BYTE                numInstances;
    getAllCallRmtPtyNum RmtPtyNum[MAX_NO_OF_CALLS];
} arrRemotePartyNum;

/**
 *  This structure contains an array of Remote Party Names
 *
 *  \param  numInstances
 *          - Number of remotePartyName that follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  GetAllCallRmtPtyName[MAX_NO_OF_CALLS]
 *          - Array of remotePartyName.
 *          - See \ref getAllCallRmtPtyName for more information.
 *
 */
typedef struct
{
    BYTE                 numInstances;
    getAllCallRmtPtyName GetAllCallRmtPtyName[MAX_NO_OF_CALLS];
} arrRemotePartyName;

/**
 *  This structure contains an array of Alerting Type.
 *
 *  \param  numInstances
 *          - Number of callID, AlertingType that follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  callID[MAX_NO_OF_CALLS]
 *          - Array of Unique call identifier for the call.
 *
 *  \param  AlertingType[MAX_NO_OF_CALLS]
 *          - Array of Alerting type.
 *              - 0x00 - ALERTING_LOCAL - Local
 *              - 0x01 - ALERTING_REMOTE - Remote
 *
 */
typedef struct
{
    BYTE numInstances;
    BYTE callID[MAX_NO_OF_CALLS];
    BYTE AlertingType[MAX_NO_OF_CALLS];
} arrAlertingType;

/**
 *  This structure contains an array of User to User Signaling Service
 *  Information
 *
 *  \param  numInstances
 *          - Number of allCallsUUSInfo that follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  AllCallsUUSInfo[MAX_NO_OF_CALLS]
 *          - Array of allCallsUUSInfo.
 *          - See \ref allCallsUUSInfo for more information.
 *
 */
typedef struct
{
    BYTE            numInstances;
    allCallsUUSInfo AllCallsUUSInfo[MAX_NO_OF_CALLS];
} arrUUSInfo;

/**
 *  This structure contains array an of Servicing option.
 *
 *  \param  numInstances
 *          - Number of callID, srvOption that follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  callID[MAX_NO_OF_CALLS]
 *          - Array of Unique call identifier for the call.
 *
 *  \param  srvOption[MAX_NO_OF_CALLS]
 *          - Array of Service option.
 *          - See Table9 qaGobiApiTableServiceOptions.h for standard service
 *            option number assignments.
 *
 */
typedef struct
{
  BYTE numInstances;
  BYTE callID[MAX_NO_OF_CALLS];
  WORD srvOption[MAX_NO_OF_CALLS];
} arrSvcOption;

/**
 *  This structure contains an array of Call End Reasons.
 *
 *  \param  numInstances
 *          - Number of callID, callEndReason that follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  callID[MAX_NO_OF_CALLS]
 *          - Array of Unique call identifier for the call.
 *
 *  \param  callEndReason[MAX_NO_OF_CALLS]
 *          - Array of Call End Reason .
 *          - See Table9 qaGobiApiTableVoiceCallEndReasons.h for a list of valid
 *            voice-related call end reasons
 *
 */
typedef struct
{
   BYTE numInstances;
   BYTE callID[MAX_NO_OF_CALLS];
   WORD callEndReason[MAX_NO_OF_CALLS];
} arrCallEndReason;

/**
 *  This structure contains an array of Alpha ID Info
 *
 *  \param  numInstances
 *          - Number of allCallsAlphaIDInfo that follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  allCallsAlphaIDInfo[MAX_NO_OF_CALLS]
 *          - Array of allCallsAlphaIDInfo.
 *          - See \ref allCallsAlphaIDInfo for more information.
 *
 */
typedef struct
{
    BYTE                numInstances;
    allCallsAlphaIDInfo allCallsAlphaIDInfoArr[MAX_NO_OF_CALLS];
} arrAlphaID;

/**
 *  This structure contains an array of Connected Party Numbers consisting of
 *  information regarding all the devices connected.
 *
 *  \param  numInstances
 *          - Number of ConnectedPartyNum that follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  ConnectedPartyNum[MAX_NO_OF_CALLS]
 *          - Array of ConnectedPartyNum.
 *          - See \ref peerNumberInfo for more information.
 *
 */
typedef struct
{
     BYTE           numInstances;
     peerNumberInfo ConnectedPartyNum[MAX_NO_OF_CALLS];
} arrConnectPartyNum;

/**
 *  This structure contains an array of Diagnostic Information.
 *
 *  \param  numInstances
 *          - Number of DiagInfo that follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  DiagInfo[MAX_NO_OF_CALLS]
 *          - Array of DiagInfo.
 *          - See \ref allCallsDiagInfo for more information.
 *
 */
typedef struct
{
    BYTE             numInstances;
    allCallsDiagInfo DiagInfo[MAX_NO_OF_CALLS];
} arrDiagInfo;

/**
 *  This structure contains an array of Called Party Numbers consisting of
 *  information of all the numbers which have been called from the device.
 *
 *  \param  numInstances
 *          - Number of calledPartyNum that follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  CalledPartyNum[MAX_NO_OF_CALLS]
 *          - Array of CalledPartyNum.
 *          - See \ref peerNumberInfo for more information.
 *
 */
typedef struct
{
     BYTE           numInstances;
     peerNumberInfo CalledPartyNum[MAX_NO_OF_CALLS];
} arrCalledPartyNum;

/**
 *  This structure contains an array of Redirecting Party Numbers consisting of
 *  information of all the numbers which have been redirected from the device.
 *
 *  \param  numInstances
 *          - Number of redirPartyNum that follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  RedirPartyNum[MAX_NO_OF_CALLS]
 *          - Array of RedirPartyNum.
 *          - See \ref peerNumberInfo for more information.
 *
 */
typedef struct
{
    BYTE           numInstances;
    peerNumberInfo RedirPartyNum[MAX_NO_OF_CALLS];
} arrRedirPartyNum;

/**
 *  This structure contains an array of Alerting Pattern.
 *
 *  \param  numInstances
 *          - Number of callID, alertingPattern that follow.
 *          - If zero(0) then no further information exists.
 *
 *  \param  callID[MAX_NO_OF_CALLS]
 *          - Array of Unique call identifier for the call.
 *
 *  \param  alertingPattern[MAX_NO_OF_CALLS]
 *          - Array of Alerting pattern.
 *              - 0x00 - QMI_VOICE_ALERTING_PATTERN_1 - Pattern 1
 *              - 0x01 - QMI_VOICE_ALERTING_PATTERN_2 - Pattern 2
 *              - 0x02 - QMI_VOICE_ALERTING_PATTERN_3 - Pattern 3
 *              - 0x04 - QMI_VOICE_ALERTING_PATTERN_5 - Pattern 5
 *              - 0x05 - QMI_VOICE_ALERTING_PATTERN_6 - Pattern 6
 *              - 0x06 - QMI_VOICE_ALERTING_PATTERN_7 - Pattern 7
 *              - 0x07 - QMI_VOICE_ALERTING_PATTERN_8 - Pattern 8
 *              - 0x08 - QMI_VOICE_ALERTING_PATTERN_9 - Pattern 9
 *
 */
typedef struct
{
     BYTE  numInstances;
     BYTE  callID[MAX_NO_OF_CALLS];
     ULONG alertingPattern[MAX_NO_OF_CALLS];
} arrAlertingPattern;

/**
 *  This structure contains information about the response parameters
 *  with all the calls originating or terminating from a particular device.
 *
 *  \param  pArrCallInfo
 *          - See \ref arrCallInfo for more information.
 *
 *  \param  pArrRemotePartyNum
 *          - See \ref arrRemotePartyNum for more information.
 *
 *  \param  pArrRemotePartyName
 *          - See \ref arrRemotePartyName for more information.
  *
 *  \param  pArrAlertingType
 *          - See \ref arrAlertingType for more information.
 *
 *  \param  pArrUUSInfo
 *          - See \ref arrUUSInfo for more information.
 *
 *  \param  pArrSvcOption
 *          - See \ref arrSvcOption for more information.
 *
 *  \param  pOTASPStatus
 *          - OTASP status for the OTASP call.
 *          - Applicable only for 3GPP2 devices.
 *              - 0x00 - OTASP_STATUS_SPL_UNLOCKED -
 *              SPL unlocked; only for user-initiated OTASP
 *              - 0x01 - OTASP_STATUS_SPRC_RETRIES_EXCEEDED -
 *                 SPC retries exceeded; only for user-initiated OTASP
 *              - 0x02 - OTASP_STATUS_AKEY_EXCHANGED -
 *                 A-key exchanged; only for user-initiated OTASP
 *              - 0x03 - OTASP_STATUS_SSD_UPDATED - SSD updated; for both
 *                 user-initiated OTASP and network-initiated OTASP (OTAPA)
 *              - 0x04 - OTASP_STATUS_NAM_DOWNLOADED - NAM downloaded;
 *                 only for user-initiated OTASP
 *              - 0x05 - OTASP_STATUS_MDN_DOWNLOADED - MDN downloaded;
 *                 only for user-initiated OTASP
 *              - 0x06 - OTASP_STATUS_IMSI_DOWNLOADED - IMSI downloaded;
 *                 only for user-initiated OTASP
 *              - 0x07 - OTASP_STATUS_PRL_DOWNLOADED - PRL downloaded;
 *                 only for user-initiated OTASP
 *              - 0x08 - OTASP_STATUS_COMMITTED - Commit successful;
 *                 only for user-initiated OTASP
 *              - 0x09 - OTASP_STATUS_OTAPA_STARTED - OTAPA started;
 *                 only for network-initiated OTASP (OTAPA)
 *              - 0x0A - OTASP_STATUS_OTAPA_STOPPED - OTAPA stopped;
 *                 only for network-initiated OTASP (OTAPA)
 *              - 0x0B - OTASP_STATUS_OTAPA_ABORTED - OTAPA aborted;
 *                 only for network-initiated OTASP (OTAPA)
 *              - 0x0C - OTASP_STATUS_OTAPA_COMMITTED - OTAPA committed;
 *                 only for network-initiated OTASP (OTAPA)
 *              - 0xFF - Not Available
 *
 *  \param  pVoicePrivacy
 *          - Voice Privacy.
 *          - Values.
 *              - 0x00 - VOICE_PRIVACY_STANDARD - Standard privacy
 *              - 0x01 - VOICE_PRIVACY_ENHANCED - Enhanced privacy
 *              - 0xFF - Not Available
 *
 *  \param  pArrCallEndReason
 *          - See \ref arrCallEndReason for more information.
 *
 *  \param  pArrAlphaID
 *          - See \ref arrAlphaID for more information.
 *
 *  \param  pArrConnectPartyNum
 *          - See \ref arrConnectPartyNum for more information.
 *
 *  \param  pArrDiagInfo
 *          - See \ref arrDiagInfo for more information.
 *
 *  \param  pArrCalledPartyNum
 *          - See \ref arrCalledPartyNum for more information.
 *
 *  \param  pArrRedirPartyNum
 *          - See \ref arrRedirPartyNum for more information.
 *
 *  \param  pArrAlertingPattern
 *          - See \ref arrAlertingPattern for more information.
 *
 */
typedef struct
{
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
} voiceGetAllCallInfo;

/**
 *  This API queries the information associated with all the calls originating
 *  or terminating from a particular device.
 *
 *  \param  pGetAllCallInfo[OUT]
 *          - See \ref voiceGetAllCallInfo for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          This command is used by the control point to get information
 *          of all the calls to and fro from the device in progress.
 *          The information keeps on updating constantly, as the state of
 *          a call changes example,from incoming to conversation to terminated.\n
 *          This API requires a firmware with atleast voice 2.0 support.
 *
 */
ULONG SLQSVoiceGetAllCallInfo( voiceGetAllCallInfo *pGetAllCallInfo );

/**
 *  This structure contains Manage Calls Information.
 *
 *  \param  SUPSType
 *          - Supplementary service type during the call.
 *            - 0x01 - SUPS_TYPE_RELEASE_HELD_OR_WAITING
 *                     - Release is held or waiting
 *            - 0x02 - SUPS_TYPE_RELEASE_ACTIVE_ACCEPT_HELD_OR_WAITING
 *                     - Release is active and accepting held or waiting
 *            - 0x03 - SUPS_TYPE_HOLD_ACTIVE_ACCEPT_WAITING_OR_HELD
 *                     - Hold is active and accepting waiting or held
 *            - 0x04 - SUPS_TYPE_HOLD_ALL_EXCEPT_SPECIFIED_CALL
 *                     - Hold all calls except a specified one
 *            - 0x05 - SUPS_TYPE_MAKE_CONFERENCE_CALL
 *                     - Make a conference call
 *            - 0x06 - SUPS_TYPE_EXPLICIT_CALL_TRANSFER
 *                     - Explicit call transfer
 *            - 0x07 - SUPS_TYPE_CCBS_ACTIVATION
 *                     - Activate completion of calls to busy subscriber
 *            - 0x08 - SUPS_TYPE_END_ALL_CALLS
 *                     - End all calls
 *            - 0x09 - SUPS_TYPE_RELEASE_SPECIFIED_CALL
 *                     - Release a specified call
 *
 *  \param  pCallID[Optional]
 *          - Applicable only for SUPSType 0x04, 0x07, and 0x09
 *
 */
typedef struct
{
    BYTE SUPSType;
    BYTE *pCallID;
}voiceManageCallsReq;

/**
 *  This structure contains Failure cause Information.
 *  Populated when API Fails.
 *
 *  \param  pFailCause
 *          - Supplementary service failure causes (optional, supply NULL if
 *            not required).
 *          - See Table8 qaGobiApiTableVoiceCallEndReasons.h for supplementary
 *            services failure cause
 *              - 0xFFFF is the value when the information is not received from
 *                device
 */
typedef struct
{
    WORD *pFailCause;
}voiceManageCallsResp;

/**
 *  Manages the calls by using the supplementary service applicable during the
 *  call. In cases of successful API completion if the state of any call is
 *  changed, it is indicated using CallBack SLQSVoiceSetAllCallStatusCallBack.
 *  If there are other calls while an incoming voice call (waiting call)
 *  is received, this API is used to Answer the call. This API is applicable
 *  only in "3GPP devices".
 *
 *  \param  pVoiceManageCallsReq[IN]
 *          - Request structure of to manage calls.
 *
 *  \param  pVoiceManageCallsResp[OUT]
 *          - Response Struture to manage Calls
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: SL9090\n
 *          Timeout: 10 Secs\n
 *          Applicable only for "3GPP"
 *
 */
ULONG SLQSVoiceManageCalls (
    voiceManageCallsReq  *pVoiceManageCallsReq,
    voiceManageCallsResp *pVoiceManageCallsResp );

/**
 *  This structure contains Voice Burst DTMF Information
 *
 *  \param  pCallID[IN/OUT]
 *          - Call ID associated with call on which the DTMF information
 *            has to be sent.\n A burst DTMF request is sent to the current
 *            active/alerting call when pCallId is set to 0xFF.
 *          - This is IN/OUT parameter, value passed by user will be packed in
 *            request and value received from the device would be returned to
 *            the user.
 *          - If the call ID value received is 0, no value has been
 *            returned by the device
 *
 *  \param  digitCnt
 *          - Length of DTMF digit buffer which follows
 *
 *  \param pDigitBuff[MAX_DESCRIPTION_LENGTH]
 *         - DTMF digit buffer in ASCII, NULL terminated
 *
 */
typedef struct
{
    BYTE *pCallID;
    BYTE digitCnt;
    BYTE pDigitBuff[MAX_DESCRIPTION_LENGTH];
}burstDTMFInfo;

/**
 *  This structure contains Voice Burst DTMF pulse length information
 *
 *  \param  DTMFPulseWidth
 *          - DTMF pulse width. Values:
 *            - 0x00 - DTMF_ONLENGTH_95MS - 95 ms
 *            - 0x01 - DTMF_ONLENGTH_150MS - 150 ms
 *            - 0x02 - DTMF_ONLENGTH_200MS - 200 ms
 *            - 0x03 - DTMF_ONLENGTH_250MS - 250 ms
 *            - 0x04 - DTMF_ONLENGTH_300MS - 300 ms
 *            - 0x05 - DTMF_ONLENGTH_350MS - 350 ms
 *            - 0x06 - DTMF_ONLENGTH_SMS  SMS Tx special pulse width
 *
 *  \param  DTMFInterdigitInterval
 *          - DTMF interdigit interval Values:
 *            - 0x00 - DTMF_OFFLENGTH_60MS  - 60 ms
 *            - 0x01 - DTMF_OFFLENGTH_100MS - 100 ms
 *            - 0x02 - DTMF_OFFLENGTH_150MS - 150 ms
 *            - 0x03 - DTMF_OFFLENGTH_200MS - 200 ms
 *
 */
typedef struct
{
    BYTE DTMFPulseWidth;
    BYTE DTMFInterdigitInterval;
}DTMFLengths;

/**
 *  This structure contains parameters of burst Dual-Tone Multifrequency (DTMF)
 *
 *  \param  BurstDTMFInfo
 *          - Burst DTMF Information
 *            - See \ref burstDTMFInfo for more information
 *  \param  pBurstDTMFLengths [optional]
 *          - DTMF Lengths
 *            - See \ref DTMFLengths for more information
 *
 */
typedef struct
{
    burstDTMFInfo BurstDTMFInfo;
    DTMFLengths   *pBurstDTMFLengths;
}voiceBurstDTMFInfo;

/**
 * Sends a burst Dual-Tone Multifrequency (DTMF) (applicable only for 3GPP2)
 *
 *  \param  pBurstDTMFInfo
 *          - Structure containing parameters of burst DTMF.
 *          - See \ref voiceBurstDTMFInfo for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          Applicable only for 3GPP2.
 *          This API sends a burst DTMF.If API result indicates success,
 *          this means the device has started the requested operation.
 *          It does not mean that the burst DTMF request has been sent to the
 *          network. A burst DTMF request is sent to the current
 *          active/alerting call when CallId is set to 0xFF.
 *          This API is applicable only in 3GPP2.
 */
ULONG SLQSVoiceBurstDTMF(
    voiceBurstDTMFInfo *pBurstDTMFInfo );

/**
 *  This structure contains parameters of continuous DTMF
 *
 *  \param  pCallID[IN/OUT]
 *          - Call ID associated with call on which the DTMF information
 *            has to be sent.\n Start continuous DTMF request is sent to the
 *            current active/alerting call when pCallId is set to 0xFF.
 *          - This is IN/OUT parameter, value passed by user will be packed in
 *            request and value received from the device would be returned to
 *            the user.
 *          - If the call ID value received is 0, no value has been
 *            returned by the device
 *
 *  \param  DTMFdigit
 *          - DTMF digit in ASCII.
 *
 */
typedef struct
{
    BYTE *pCallID;
    BYTE DTMFdigit;
}voiceContDTMFinfo;

/**
 * Starts a continuous DTMF.
 *
 *  \param  pContDTMFInfo
 *          - Structure containing Continuous DTMF Information.
 *            - See \ref voiceContDTMFinfo for more informtaion.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          This API starts a continuous DTMF. If the API results indicates
 *          success,it means that the device has started the requested
 *          operation. It does not mean that the start continuous DTMF request
 *          has been sent to the network. A start continuous DTMF request is
 *          sent to the current active/alerting call when CallId is set to
 *          0xFF.
 */
ULONG SLQSVoiceStartContDTMF(
    voiceContDTMFinfo *pContDTMFInfo );

/**
 *  This structure contains parameters of stop continuous DTMF
 *
 *  \param  pCallID[IN/OUT]
 *          - Call ID associated with call on which the DTMF information
 *            has to be sent.\n Stop continuous DTMF request is sent to the
 *            current active/alerting call when pCallId is set to 0xFF.
 *          - This is IN/OUT parameter, value passed by user will be packed in
 *            request and value received from the device would be returned to
 *            the user.
 *          - If the call ID value received is 0, no value has been
 *            returned by the device
 *
 */
typedef struct
{
    BYTE callID;
}voiceStopContDTMFinfo;

/**
 * Stops a continuous DTMF.
 *
 *  \param  pVoiceStopContDTMFinfo
 *          - Structure containing Continuous Stop DTMF Information.
 *            - See \ref voiceStopContDTMFinfo for more informtaion.
 *
 *          - Start continuous DTMF request is sent to
 *            the current active/alerting call when CallId is set to 0xFF.
 *          - This is IN/OUT params;value passed by user will packed in
 *            request and before unpacking response this will be assigned with
 *            an invalid callID value "0". It change to a valid value if
 *            received as part of response otherwise Invalid value willl be
 *            present.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          This API starts a continuous DTMF.
 *          If the API results indicates success,
 *          it means that the device has started the requested operation.
 *          It does not mean that the start continuous DTMF request has been
 *          sent to the network. A stop continuous DTMF request is sent to
 *          the current active/alerting call when CallId is set to 0xFF.
 */
ULONG SLQSVoiceStopContDTMF(
    voiceStopContDTMFinfo *pVoiceStopContDTMFinfo);

/**
 *  This structure contains the flash information associated with a call.
 *
 *  \param  pCallID[IN/OUT]
 *          - Unique call identifier associated with the current call.
 *
 *  \param  pFlashPayLd[IN](optional)
 *          - Payload in ASCII to be sent in Flash.
 *          - Variable Length, NULL terminated.
 *
 *  \param  pFlashType[IN](optional)
 *          - Flash type.
 *              - 0 - Simple Flash (default)
 *              - 1 - Activate answer hold
 *              - 2 - Deactivate answer hold
 *
 */
typedef struct
{
    BYTE *pCallID;
    BYTE *pFlashPayLd;
    BYTE *pFlashType;
} voiceFlashInfo;

/**
 *  This API sends a simple flash message. Applicable only for 3GPP2 devices.
 *
 *  \param  pFlashInfo[IN/OUT]
 *          - See \ref voiceFlashInfo for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 10 Secs\n
 *          If success, it only means the device has started the requested
 *          operation and not that the Flash has been sent.
 *          If the optional parameter Flash Type is not set, the default
 *          flash type is assumed to be a simple flash.
 *          If the parameter Flash Type is set to 1 the call ID corresponding
 *          to it is either an incoming or waiting call's call ID.
 *          If the parameter Flash Type is set to 2 the call ID corresponding
 *          to it is a held call's call ID.
 *          A Flash request is sent to the appropriate call when call_id is
 *          set to 0xFF.
 *
 */
ULONG SLQSVoiceSendFlash ( voiceFlashInfo *pFlashInfo );

/**
 *  This structure contains the preferred voice privacy values.
 *
 *  \param  privacyPref
 *          - Voice Privacy Preference
 *              - 0x00 - VOICE_PRIVACY_STANDARD - Standard privacy
 *              - 0x01 - VOICE_PRIVACY_ENHANCED - Enhanced privacy
 *
 */
typedef struct
{
    BYTE privacyPref;
} voiceSetPrefPrivacy;

/**
 *  This API sets the voice privacy preference. Applicable only for 3GPP2 devices.
 *
 *  \param  pSetPrefPrivacy[IN]
 *          - See \ref voiceSetPrefPrivacy for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 10 Secs
 *
 */
ULONG SLQSVoiceSetPreferredPrivacy ( voiceSetPrefPrivacy *pSetPrefPrivacy );

/**
 *  This structure contains parameters of Indication Register Information
 *
 *  \param  pRegDTMFEvents(optional)
 *          - Registration Indication For DTMF Events.
 *          - When this registration is enabled, the device learns of DTMF
 *            events via the QMI_VOICE_DTMF_IND indication.
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pRegVoicePrivacyEvents(optional)
 *          - Registration Indication For Voice Privacy Events.
 *          - When this registration is enabled, the device learns of DTMF
 *            events via the QMI_VOICE_PRIVACY_IND indication.
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \param  pSuppsNotifEvents(optional)
 *          - Registration Indication For Supplementary Service Notification
 *            Events.
 *          - When this registration is enabled, the device learns of DTMF
 *            events via the QMI_VOICE_SUPS_NOTIFICATION_IND indication.
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *
 *  \note   One of the optional parameter is mandatory to be present in the
 *          request.
 */
typedef struct
{
    BYTE *pRegDTMFEvents;
    BYTE *pRegVoicePrivacyEvents;
    BYTE *pSuppsNotifEvents;
} voiceIndicationRegisterInfo;

/**
 *  Sets the registration state for different QMI_VOICE indications for the
 *  requesting control point
 *
 *  \param  pVoiceIndicationRegisterInfo[IN]
 *          - Structure containing Indication Register Information.
 *              - See \ref voiceIndicationRegisterInfo for more informtaion.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: SL9090\n
 *          Timeout: 10 Secs
 *
 *          This API is used by a device to register/deregister for different
 *          QMI VOICE indications.
 *          The device's registration state variables that control registration
 *          for indications will be modified to reflect the settings indicated in the
 *          request message.
 *          At least one optional parameter must be present in the request.
 *
 */
ULONG SLQSVoiceIndicationRegister(
    voiceIndicationRegisterInfo *pVoiceIndicationRegisterInfo );

/**
 *  This structure contains information about the DTMF (Dual Tone
 *  Multi-Frequency).
 *
 *  \param  callID
 *          - Call identifier for the current call.
 *
 *  \param  DTMFEvent
 *          - DTMF event
 *              - 0x00 - DTMF_EVENT_REV_BURST - Sends a CDMA-burst DTMF
 *              - 0x01 - DTMF_EVENT_REV_START_CONT -
 *                 Starts a continuous DTMF tone
 *              - 0x03 - DTMF_EVENT_REV_STOP_CONT -
 *                 Stops a continuous DTMF tone
 *              - 0x05 - DTMF_EVENT_FWD_BURST -
 *                 Received a CDMA-burst DTMF message
 *              - 0x06 - DTMF_EVENT_FWD_START_CONT -
 *                 Received a start-continuous DTMF tone order
 *              - 0x07 - DTMF_EVENT_FWD_STOP_CONT -
 *                 Received a stop-continuous DTMF tone order
 *
 *  \param  digitCnt
 *          - Number of set of following element i.e. digitBuff.
 *
 *  \param  digitBuff[MAX_DESCRIPTION_LENGTH]
 *          - DTMF digit buffer in ASCII string which is NULL terminated
 *
 */
typedef struct
{
    BYTE callID;
    BYTE DTMFEvent;
    BYTE digitCnt;
    BYTE digitBuff[MAX_DESCRIPTION_LENGTH];
} DTMFInfo;

/**
 *  This structure contains information about the Supplementary Services.
 *
 *  \param  svcType
 *          - Service type.
 *              - 0x01 - SERVICE_TYPE_ACTIVATE - Activate
 *              - 0x02 - SERVICE_TYPE_DEACTIVATE - Deactivate
 *              - 0x03 - SERVICE_TYPE_REGISTER - Register
 *              - 0x04 - SERVICE_TYPE_ERASE - Erase
 *              - 0x05 - SERVICE_TYPE_INTERROGATE - Interrogate
 *              - 0x06 - SERVICE_TYPE_REGISTER_PASSWORD - Register password
 *              - 0x07 - SERVICE_TYPE_USSD - USSD
 *
 *  \param  isModByCC
 *          - Indicates whether the supplementary service data is modified by
 *            the card (SIM/USIM) as part of the call control:
 *              - 0 - False
 *              - 1 - True
 *
 */
typedef struct
{
    BYTE svcType;
    BYTE isModByCC;
} SUPSInfo;

/**
 *  This structure contains New Password Data.
 *
 *  \param  newPwd[PASSWORD_LENGTH]
 *          - New password.
 *              - Password consists of 4 ASCII digits.
 *              - Range: 0000 to 9999.
 *
 *  \param  newPwdAgain[PASSWORD_LENGTH]
 *          - New password again.
 *              - Password consists of 4 ASCII digits.
 *              - Range: 0000 to 9999.
 *
 */
typedef struct
{
    BYTE newPwd[PASSWORD_LENGTH];
    BYTE newPwdAgain[PASSWORD_LENGTH];
} newPwdData;

/**
 *  This structure contains information about the Connected Line Identification
 *  Presentation (COLP) supplementary service responses.
 *
 *  \param  ActiveStatus
 *          - Active status.
 *          - Values:
 *              - 0x00 - ACTIVE_STATUS_INACTIVE - Inactive
 *              - 0x01 - ACTIVE_STATUS_ACTIVE - Active
 *              - 0xFF - Not Available
 *
 *  \param  ProvisionStatus
 *          - Provisioned status.
 *          - Values:
 *              - 0x00 - PROVISION_STATUS_NOT_PROVISIONED - Not provisioned
 *              - 0x01 - PROVISION_STATUS_PROVISIONED - Provisioned
 *              - 0xFF - Not Available
 *
 */
typedef struct
{
    BYTE ActiveStatus;
    BYTE ProvisionStatus;
} COLPResp;

/**
 *  This structure contains information about the Connected Line Identification
 *  Restriction (COLR) supplementary service responses.
 *
 *  \param  ActiveStatus
 *          - Active status.
 *          - Values:
 *              - 0x00 - ACTIVE_STATUS_INACTIVE - Inactive
 *              - 0x01 - ACTIVE_STATUS_ACTIVE - Active
 *              - 0xFF - Not Available
 *
 *  \param  ProvisionStatus
 *          - Provisioned status.
 *          - Values:
 *              - 0x00 - PROVISION_STATUS_NOT_PROVISIONED - Not provisioned
 *              - 0x01 - PROVISION_STATUS_PROVISIONED - Provisioned
 *              - 0xFF - Not Available
 *
 */
typedef struct
{
    BYTE ActiveStatus;
    BYTE ProvisionStatus;
} COLRResp;

/**
 *  This structure contains information about the Calling Name Presentation
 *  (CNAP) supplementary service responses.
 *
 *  \param  ActiveStatus
 *          - Active status.
 *          - Values:
 *              - 0x00 - ACTIVE_STATUS_INACTIVE - Inactive
 *              - 0x01 - ACTIVE_STATUS_ACTIVE - Active
 *              - 0xFF - Not Available
 *
 *  \param  ProvisionStatus
 *          - Provisioned status.
 *          - Values:
 *              - 0x00 - PROVISION_STATUS_NOT_PROVISIONED - Not provisioned
 *              - 0x01 - PROVISION_STATUS_PROVISIONED - Provisioned
 *              - 0xFF - Not Available
 *
 */
typedef struct
{
    BYTE ActiveStatus;
    BYTE ProvisionStatus;
} CNAPResp;

/**
 *  This structure contains Voice Get Configuration Request Parameters
 *
 *  \param  pAutoAnswer(optional)
 *          - Indicator to retrieve the Auto Answer Information.
 *              - 0x01 - Include auto answer information
 *
 *  \param  pAirTimer(optional)
 *          - Indicator to retrieve the Air Timer Information.
 *              - 0x01 - Include air calls timer count information
 *          - Currently Not Supported.
 *
 *  \param  pRoamTimer(optional)
 *          - Indicator to retrieve the Roam Timer Information.
 *              - 0x01 - Include roam calls timer information
 *          - Currently Not Supported.
 *
 *  \param  pTTYMode(optional)
 *          - Indicator to retrieve the TTY Mode Information.
 *              - 0x01 - Include TTY configuration status information
 *
 *  \param  pPrefVoiceSO(optional)
 *          - Indicator to retrieve the Preferred Voice SO Information.
 *              - 0x01 - Include preferred voice configuration status
 *                       information
 *          - Currently Not Supported.
 *
 *  \param  pAMRStatus(optional)
 *          - Indicator to retrieve the AMR Status Information.
 *              - 0x01 - Include AMR status information
 *
 *  \param  pPrefVoicePrivacy(optional)
 *          - Indicator to retrieve the Preferred Voice Privacy Information.
 *              - 0x01 - Include preferred voice privacy status information
 *
 *  \param  pNamID(optional)
 *          - Index of the Number Assignment Module Index (CDMA subscription)
 *            to be configured
 *          - Range: 0 to 3.
 *          - Some modems support only 1 or 2 NAMs.
 *          - The NAM Index is valid only when the request contains at least
 *            one of Air Timer, Roam Timer, and Preferred Voice SO.
 *          - If no nam_id value is specified in the request,the default value
 *            is 0.
 *
 *  \param  pVoiceDomainPref(optional)
 *          - Indicator to retrieve the Preferred Voice Domain Information.
 *              - 0x01 - Include voice domain preference information
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not returned.
 *
 */
typedef struct
{
    BYTE *pAutoAnswer;
    BYTE *pAirTimer;
    BYTE *pRoamTimer;
    BYTE *pTTYMode;
    BYTE *pPrefVoiceSO;
    BYTE *pAMRStatus;
    BYTE *pPrefVoicePrivacy;
    BYTE *pNamID;
    BYTE *pVoiceDomainPref;
} voiceGetConfigReq;

/**
 *  This structure contains the Current Adaptive Multi Rate Configuration
 *  Information.
 *
 *  \param  gsmAmrStat
 *          - GSM AMR Status
 *              - 0x00 - Disable
 *              - 0x01 - Enable
 *              - 0xFF - Not Available
 *
 *  \param  wcdmaAmrStat
 *          - WCDMA AMR Status
 *          - One or a combination of the following bitmask values:
 *              - Bit 0 - AMR codec advertised is not supported
 *              - Bit 1 - Controls WCDMA AMR wideband
 *              - Bit 2 - Controls GSM half rate AMR
 *              - Bit 3 - Controls GSM AMR wideband
 *              - Bit 4 - Controls GSM AMR narrowband
 *          - 0xFF, if not available
 *
 */
typedef struct
{
    BYTE gsmAmrStat;
    BYTE wcdmaAmrStat;
} curAMRConfig;

/**
 *  This structure contains Voice Get Configuration Response Parameters.
 *
 *  \param  pAutoAnswerStat(optional)
 *          - Auto Answer Status
 *          - Value returned is read from NV_AUTO_ANSWER_I.
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled
 *              - 0xFF - Not Available
 *
 *  \param  pAirTimerCnt(optional)
 *          - Air Timer Count
 *          - Value returned is read from NV_AIR_CNT_I.
 *          - See \ref airTimer for more information
 *
 *  \param  pRoamTimerCnt(optional)
 *          - Roam Timer Count
 *          - Value returned is read from NV_ROAM_CNT_I.
 *          - See \ref roamTimer for more information
 *
 *  \param  pCurrTTYMode(optional)
 *          - Current TTY Mode
 *          - Value returned is read from NV_TTY_I.
 *              - 0x00 - TTY_MODE_FULL - Full
 *              - 0x01 - TTY_MODE_VCO - Voice carry over
 *              - 0x02 - TTY_MODE_HCO - Hearing carry over
 *              - 0x03 - TTY_MODE_OFF - Off
 *              - 0xFF - Not Available
 *
 *  \param  pCurPrefVoiceSO(optional)
 *          - Current Preferred Voice SO
 *          - Value returned is read from NV_PREF_VOICE_SO_I.
 *          - See \ref prefVoiceSO for more information
 *
 *  \param  pCurAMRConfig(optional)
 *          - Current Adaptive Multi-Rate Configuration.
 *          - Values returned are read from NV_GSM_ARM_CALL_CONFIG_I and
 *            NV_UMTS_AMR_CODEC_PREFERENCE_CONFIG_I.
 *          - See \ref curAMRConfig for more information
 *
 *  \param  pCurVoicePrivacyPref(optional)
 *          - Current Voice Privacy Preference
 *          - Value returned is read from NV_VOICE_PRIV_I.
 *              - 0x00 - Standard privacy
 *              - 0x01 - Enhanced privacy
 *              - 0xFF - Not Available
 *
 *  \param  pCurVoiceDomainPref(optional)
 *          - Current Voice Domain Preference.
 *              - 0x00 - Circuit-switched (CS) only
 *              - 0x01 - Packet-switched (PS) only
 *              - 0x02 - CS is preferred; PS is secondary
 *              - 0x03 - PS is preferred; CS is secondary
 *              - 0xFF - Not Available
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not returned or has default value.
 *
 */
typedef struct
{
    BYTE         *pAutoAnswerStat;
    airTimer     *pAirTimerCnt;
    roamTimer    *pRoamTimerCnt;
    BYTE         *pCurrTTYMode;
    prefVoiceSO  *pCurPrefVoiceSO;
    curAMRConfig *pCurAMRConfig;
    BYTE         *pCurVoicePrivacyPref;
    BYTE         *pCurVoiceDomainPref;
} voiceGetConfigResp;

/**
 * This API retrieves various configuration parameters that control the modem
 * behavior related to circuit switched services.
 *
 *  \param  pVoiceGetConfigReq
 *          - Structure containing Get Config request parameters.
 *              - See \ref voiceGetConfigReq for more informtaion.
 *
 *  \param  pVoiceGetConfigResp
 *          - Structure containing Get Config response parameters.
 *              - See \ref voiceGetConfigResp for more informtaion.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          Any invalid value in a request message causes the service point
 *          to reject the message without retrieving any configuration
 *          information.
 *
 */
ULONG SLQSVoiceGetConfig (
          voiceGetConfigReq  *pVoiceGetConfigReq,
          voiceGetConfigResp *pVoiceGetConfigResp );

/**
 *  This structure contains Orig USSD No Wait Information Parameters.
 *
 *  \param  USSInformation
 *          - See \ref USSInfo for more informtaion.
 *
 */
typedef struct
{
    struct USSInfo USSInformation;
} voiceOrigUSSDNoWaitInfo;

/**
 * This API initiates a USSD operation such that the response for this request
 * is returned immediately and the data is returned via an indication.
 *
 *  \param  pVoiceOrigUSSDNoWaitInfo
 *          - See \ref voiceOrigUSSDNoWaitInfo for more informtaion.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          This API starts a new USSD operation. The response to the request
 *          is sent immediately. The response result is sent to the client via
 *          the SLQSVoiceOrigUSSDNoWaitCallback. This command is applicable
 *          only in 3GPP devices.
 *
 */
ULONG SLQSVoiceOrigUSSDNoWait (
          voiceOrigUSSDNoWaitInfo *pVoiceOrigUSSDNoWaitInfo );

/**
 *  This structure contains Bind Subscription Information Parameters.
 *
 *  \param  subsType
 *          - Subscription Type.
 *              - 0x00 - VOICE_SUBS_TYPE_PRIMARY - Primary
 *              - 0x01 - VOICE_SUBS_TYPE_SECONDARY - Secondary
 *
 */
typedef struct
{
    BYTE subsType;
} voiceBindSubscriptionInfo;

/**
 * This API binds a subscription type to a specific voice client ID.
 *
 *  \param  pVoiceBindSubscriptionInfo
 *          - See \ref voiceBindSubscriptionInfo for more informtaion.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          Some versions of the modem support the Dual SIM feature. With this
 *          feature the modem can register with two different cellular networks
 *          simultaneously. Each network registration is associated with a
 *          different subscription, e.g., phone number, such that the modem
 *          appears to the network to be two different users. By default, the
 *          Voice client is bound to the primary subscription. This command
 *          allows the Voice client to change this binding. After receiving a
 *          successful response to this command, all future commands sent by
 *          the client will affect the newly bound subscription only.
 *
 */
ULONG SLQSVoiceBindSubscription (
          voiceBindSubscriptionInfo *pVoiceBindSubscriptionInfo );

/**
 *  This structure contains ALS Set Line Switching Information Parameters.
 *
 *  \param  switchOption
 *          - Switch Option.
 *              - 0x00 - VOICE_LINE_SWITCHING_NOT_ALLOWED -
 *                 Line switching is not allowed
 *              - 0x01 - VOICE_LINE_SWITCHING_ALLOWED -
 *                 Line switching is allowed
 *
 */
typedef struct
{
    BYTE switchOption;
} voiceALSSetLineSwitchInfo;

/**
 * This API sets the line switch setting on the card.
 *
 *  \param  pVoiceALSSetLineSwitchInfo
 *          - See \ref voiceALSSetLineSwitchInfo for more informtaion.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          This API sets a line to be switchable or unswitchable, and the
 *          switch status is updated on the card. The API is supported only
 *          for specific SIM/USIMs that support alternate line service.
 *          This command is applicable only in 3GPP devices.
 *          A No Effect error is returned if the update on the card fails.
 *
 */
ULONG SLQSVoiceALSSetLineSwitching (
          voiceALSSetLineSwitchInfo *pVoiceALSSetLineSwitchInfo );

/**
 *  This structure contains ALS Select Line Information Parameters.
 *
 *  \param  lineValue
 *          - ALS Line Value.
 *              - 0x00 - ALS_LINE1 - Line 1 (default)
 *              - 0x01 - ALS_LINE2 - Line 2
 *
 */
typedef struct
{
    BYTE lineValue;
} voiceALSSelectLineInfo;

/**
 * This API allows the user to select the preferred line.
 *
 *  \param  pVoiceALSSelectLineInfo
 *          - See \ref voiceALSSelectLineInfo for more informtaion.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs\n
 *          This API allows the user to select the preferred line, and
 *          the status is updated on the card. The API is supported only
 *          for specific SIM/USIMs that support alternate line service.
 *          This command is applicable only in 3GPP devices.
 *          A No Effect error is returned if the update on the card fails.
 *
 */
ULONG SLQSVoiceALSSelectLine (
          voiceALSSelectLineInfo *pVoiceALSSelectLineInfo );

/**
 *  This structure contains Voice Get Connected Line Identification
 *  Presentation(COLP) Response Parameters
 *
 *  \param  pCOLPResp
 *          - Pointer to structure of COLPResp (optional)
 *              - See \ref COLPResp for more information
 *
 *  \param  pFailCause
 *          - Supplementary services failure cause (optional)
 *          - see qaGobiApiTableVoiceCallEndReasons.h for more information.
 *          - 0xFFFF,if Not Available
 *
 *  \param  pAlphaIDInfo
 *          - Pointer to structure of alphaIDInfo (optional)
 *              - See \ref alphaIDInfo for more information
 *
 *  \param  pCCResType
 *          - Call Control Result Type (optional)
 *              - 0x00 - CC_RESULT_TYPE_VOICE - Voice
 *              - 0x01 - CC_RESULT_TYPE_SUPS - Supplementary service
 *              - 0x02 - CC_RESULT_TYPE_USSD -
 *                  Unstructured supplementary service
 *              - 0xFF - Not Available
 *
 *  \param  pCallID
 *          - Call ID of the voice call that resulted from call
 *            control. (optional)
 *          - It is present when pCCResType is present and is Voice.
 *          - If zero(0) then invalid.
 *
 *  \param  pCCSUPSType
 *          - Supplementary service data that resulted from call
 *            control (optional)
 *          - Data is present when pCCResultType is present and
 *            is other than Voice.
 *              - See \ref ccSUPSType for more information
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not returned or has default value.
 *
 */
typedef struct
{
    COLPResp     *pCOLPResp;
    WORD         *pFailCause;
    alphaIDInfo  *pAlphaIDInfo;
    BYTE         *pCCResType;
    BYTE         *pCallID;
    ccSUPSType   *pCCSUPSType;
} voiceGetCOLPResp;

/**
 *  Queries the status of the Connected Line Identification Presentation (COLP)
 *  supplementary service (applicable only for 3GPP).
 *
 *  \param  pVoiceGetCOLPResp[OUT]
 *          - Pointer to structure of voiceGetCOLPResp
 *              - See \ref voiceGetCOLPResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs
 *
 *          This API queries the status of the COLP supplementary service.
 *          A response indicates whether COLP is active/inactive and
 *          provisioned/not provisioned in the network.
 *          The active_status field is only applicable when provision_status is
 *          PROVISIONED, i.e., there is not any case where provision_status is
 *          NOT_PROVISIONED and active_status is ACTIVE.
 *          This API is applicable only in 3GPP devices.
 *
 */
ULONG SLQSVoiceGetCOLP(
        voiceGetCOLPResp *pVoiceGetCOLPResp );

/**
 *  This structure contains Voice Get Connected Line Identification
 *  Restriction(COLR) Response Parameters
 *
 *  \param  pCOLRResp
 *          - Pointer to structure of COLRResp (optional)
 *              - See \ref COLRResp for more information
 *
 *  \param  pFailCause
 *          - Supplementary services failure cause (optional)
 *          - see qaGobiApiTableVoiceCallEndReasons.h for more information.
 *          - 0xFFFF,if Not Available
 *
 *  \param  pAlphaIDInfo
 *          - Pointer to structure of alphaIDInfo (optional)
 *              - See \ref alphaIDInfo for more information
 *
 *  \param  pCCResType
 *          - Call Control Result Type (optional)
 *              - 0x00 - CC_RESULT_TYPE_VOICE - Voice
 *              - 0x01 - CC_RESULT_TYPE_SUPS - Supplementary service
 *              - 0x02 - CC_RESULT_TYPE_USSD -
 *                  Unstructured supplementary service
 *              - 0xFF - Not Available
 *
 *  \param  pCallID
 *          - Call ID of the voice call that resulted from call
 *            control. (optional)
 *          - It is present when pCCResType is present and is Voice.
 *          - If zero(0) then invalid.
 *
 *  \param  pCCSUPSType
 *          - Supplementary service data that resulted from call
 *            control (optional)
 *          - Data is present when pCCResultType is present and
 *            is other than Voice.
 *              - See \ref ccSUPSType for more information
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not returned or has default value.
 *
 */
typedef struct
{
    COLRResp    *pCOLRResp;
    WORD        *pFailCause;
    alphaIDInfo *pAlphaIDInfo;
    BYTE        *pCCResType;
    BYTE        *pCallID;
    ccSUPSType  *pCCSUPSType;
} voiceGetCOLRResp;

/**
 *  Queries the status of the Connected Line Identification Restriction (COLR)
 *  supplementary service (applicable only for 3GPP).
 *
 *  \param  pVoiceGetCOLRResp[OUT]
 *          - Pointer to structure of voiceGetCOLRResp
 *              - See \ref voiceGetCOLRResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs
 *
 *          This API queries the status of the COLR supplementary service.
 *          A response indicates whether COLR is active/inactive and
 *          provisioned/not provisioned in the network.
 *          The active_status field is only applicable when provision_status is
 *          PROVISIONED, i.e., there is not any case where provision_status is
 *          NOT_PROVISIONED and active_status is ACTIVE.
 *          This API is applicable only in 3GPP devices.
 *
 */
ULONG SLQSVoiceGetCOLR(
        voiceGetCOLRResp *pVoiceGetCOLRResp );

/**
 *  This structure contains Voice Get Calling Name
 *  Presentation(CNAP) Response Parameters
 *
 *  \param  pCNAPResp
 *          - Pointer to structure of CNAPResp (optional)
 *              - See \ref CNAPResp for more information
 *
 *  \param  pFailCause
 *          - Supplementary services failure cause (optional)
 *          - see qaGobiApiTableVoiceCallEndReasons.h for more information.
 *          - 0xFFFF,if Not Available
 *
 *  \param  pAlphaIDInfo
 *          - Pointer to structure of alphaIDInfo (optional)
 *              - See \ref alphaIDInfo for more information
 *
 *  \param  pCCResType
 *          - Call Control Result Type (optional)
 *              - 0x00 - CC_RESULT_TYPE_VOICE - Voice
 *              - 0x01 - CC_RESULT_TYPE_SUPS - Supplementary service
 *              - 0x02 - CC_RESULT_TYPE_USSD -
 *                  Unstructured supplementary service
 *              - 0xFF - Not Available
 *
 *  \param  pCallID
 *          - Call ID of the voice call that resulted from call
 *            control. (optional)
 *          - It is present when pCCResType is present and is Voice.
 *          - If zero(0) then invalid.
 *
 *  \param  pCCSUPSType
 *          - Supplementary service data that resulted from call
 *            control (optional)
 *          - Data is present when pCCResultType is present and
 *            is other than Voice.
 *              - See \ref ccSUPSType for more information
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not returned or has default value.
 *
 */
typedef struct
{
    CNAPResp     *pCNAPResp;
    WORD         *pFailCause;
    alphaIDInfo  *pAlphaIDInfo;
    BYTE         *pCCResType;
    BYTE         *pCallID;
    ccSUPSType   *pCCSUPSType;
} voiceGetCNAPResp;

/**
 *  Queries the status of the Calling Name Presentation(CNAP)
 *  supplementary service (applicable only for 3GPP).
 *
 *  \param  pVoiceGetCNAPResp[OUT]
 *          - Pointer to structure of voiceGetCNAPResp
 *              - See \ref voiceGetCNAPResp for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: SL9090\n
 *          Timeout: 30 Secs
 *
 *          This API queries the status of the CNAP supplementary service.
 *          A response indicates whether CNAP is active/inactive and
 *          provisioned/not provisioned in the network.
 *          The active_status field is only applicable when provision_status is
 *          PROVISIONED, i.e., there is not any case where provision_status is
 *          NOT_PROVISIONED and active_status is ACTIVE.
 *          This API is applicable only in 3GPP devices.
 *
 */
ULONG SLQSVoiceGetCNAP(
        voiceGetCNAPResp *pVoiceGetCNAPResp );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_VOICE_H__ */
