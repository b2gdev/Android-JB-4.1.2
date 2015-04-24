/**
 * \ingroup  cbk
 *
 * \file     qaGobiApiCbk.h
 *
 * \brief    Callback Service API function prototypes
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __GOBI_API_CBK_H__
#define __GOBI_API_CBK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "qaGobiApiNas.h"
#include "qaGobiApiVoice.h"
#include "qaGobiApiSms.h"
#include "qaGobiApiUim.h"

#define SIGSTRENGTH_THRESHOLD_ARR_SZ  5
#define QMI_WMS_MAX_PAYLOAD_LENGTH    256
#define QMI_MAX_VOICE_NUMBER_LENGTH   81
#define MAX_NO_OF_UUSINFO             20
#define MAXUSSDLENGTH                 182
#define MAX_NO_OF_CALLS               20
#define CBK_ENABLE_EVENT              0x01
#define CBK_DISABLE_EVENT             0x00
#define CBK_NOCHANGE                  0xFF
#define MAX_NO_OF_APPLICATIONS        10
#define MAX_NO_OF_SLOTS               5
#define MAX_NO_OF_FILES               255
#define MAX_PATH_LENGTH               255
#define EVENT_MASK_CARD               0x00000001
#define EVENT_MASK_DEREGISTER_ALL     0x00000000
#define REGISTER_EVENT                0x01
#define DEREGISTER_EVENT              0x00

/**
 *  Session state callback function.
 *
 *  \param  state
 *          - Current Link Status\n
 *              - 1 Disconnected
 *              - 2 Connected
 *              - 3 Suspended (Unsupported)
 *              - 4 Authenticating
 *
 *  \param  sessionEndReason
 *          -  See \ref Tables for Call End Reason
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *
 */
typedef void (* tFNSessionState)(
   ULONG                      state,
   ULONG                      sessionEndReason );

/**
 *  Enables/disables the session state callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0 - disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: none; does not require communication with the device
 *
 */
ULONG SetSessionStateCallback(
    tFNSessionState pCallback );

/**
 *  Data bearer technology callback function.
 *
 *  \param  dataBearer
 *          - Data bearer technology
 *              - 0x00 - Indicates that this field is ignored
 *              - 0x01 - CDMA 1X
 *              - 0x02 - EV-DO Rev 0
 *              - 0x03 - GPRS
 *              - 0x04 - WCDMA
 *              - 0x05 - EV-DO Rev A
 *              - 0x06 - EDGE
 *              - 0x07 - HSDPA and WCDMA
 *              - 0x08 - WCDMA and HSUPA
 *              - 0x09 - HSDPA and HSUPA
 *              - 0x0A - LTE
 *              - 0x0B - EV-DO Rev A EHRPD
 *              - 0x0C - HSDPA+ and WCDMA
 *              - 0x0D - HSDPA+ and HSUPA
 *              - 0x0E - DC_HSDPA+ and WCDMA
 *              - 0x0F - DC_HSDPA+ and HSUPA
 *              - 0x8000 - NULL Bearer
 *              - 0xFF - Unknown Technology
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7700/50,MC83x5\n
 *
 */
typedef void (* tFNDataBearer)( ULONG dataBearer );

/**
 *  Enables/disables the Data Bearer state callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0 - disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7700/50,MC83x5\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetDataBearerCallback(
    tFNDataBearer pCallback );

/**
 *  Dormancy status callback function.
 *
 *  \param  dormancyStatus
 *          - Dormancy status
 *              - 1  - traffic channel dormant
 *              - 2  - traffic channel active
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *
 */
typedef void (* tFNDormancyStatus)( ULONG dormancyStatus );

/**
 *  Enables/disables the Dormancy Status callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0 - disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetDormancyStatusCallback(
    tFNDormancyStatus pCallback );

/**
 *  Power operating mode callback function.
 *
 *  \param  operatingMode
 *          - Service Operating mode\n
 *              See \ref Tables for Operating Modes
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *
 */
typedef void (* tFNPower)( ULONG operatingMode );

/**
 *  Enables/disables the Operating Mode callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0 - disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetPowerCallback(
    tFNPower pCallback );

/**
 *  RX/TX byte counts callback function.
 *
 *  \param  totalBytesTX
 *          - Bytes transmitted without error
 *
 *  \param  totalBytesRX
 *          -  Bytes received without error
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *
 */
typedef void (* tFNByteTotals)(
   ULONGLONG totalBytesTX,
   ULONGLONG totalBytesRX );

/**
 *  Enables/disables the Byte Totals callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0 - disable)
 *
 *  \param  interval
 *           - Interval in seconds.
 *           - ignored when disabling, should be non-zero when enabling
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetByteTotalsCallback(
    tFNByteTotals pCallback,
    BYTE          interval );

/*
 *  For internal use only, not to be exposed to the user
 *  This structure will hold the input parameters passed for SetBytesTotalCbk
 *  by the user
 *
 * \param  interval
 *           - Interval in seconds.
 *           - ignored when disabling, should be non-zero when enabling
 *
 * Note:    None
 *
 */
struct BytesTotalDataType
{
    BYTE interval;
};

/*
 *  For internal use only, not to be exposed to the user
 *  Enables the Byte Totals callback function.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   None
 *
 */
ULONG iSetByteTotalsCallback(
    tFNByteTotals pCallback );

/**
 *  Activation status callback function.
 *
 *  \param  activationStatus
 *          - Service Activation Code
 *              - 0 - Service not activated
 *              - 1 - Service activated
 *              - 2 - Activation connecting
 *              - 3 - Activation connected
 *              - 4 - OTASP security authenticated
 *              - 5 - OTASP NAM downloaded
 *              - 6 - OTASP MDN downloaded
 *              - 7 - OTASP IMSI downloaded
 *              - 8 - OTASP PRL downloaded
 *              - 9 - OTASP SPC downloaded
 *              - 10 - OTASP settings committed
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC83x5\n
 *
 */
typedef void (* tFNActivationStatus)( ULONG activationStatus );

/**
 *  Enables/disables the Activation Status callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0 - disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds
 */
ULONG SetActivationStatusCallback(
    tFNActivationStatus pCallback );

/**
 *  Mobile IP status callback function.
 *
 *  \param  mipStatus
 *          - Mobile IP Status
 *              - 0 - success
 *              - All others error codes as defined in RFC 2002
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5\n
 *
 */
typedef void (* tFNMobileIPStatus)( ULONG mipStatus );

/**
 *  Enables/disables the Mobile IP Status callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0 - disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetMobileIPStatusCallback(
    tFNMobileIPStatus pCallback );

/**
 *  Roaming indicator callback function.
 *
 *  \param  roaming
 *          - Roaming Indication\n
 *              - 0  - Roaming\n
 *              - 1  - Home\n
 *              - 2  - Roaming partner\n
 *              - >2 - Operator defined values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7750\n
 *
 */
typedef void (* tFNRoamingIndicator)( ULONG roaming );

/**
 *  Enables/disables the Roaming Indicator callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0 - disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7750\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetRoamingIndicatorCallback(
    tFNRoamingIndicator pCallback );

/**
 *  Serving system data capabilities callback function.
 *
 *  \param  dataCapsSize
 *          - Number of elements the data capability array contains
 *
 *  \param  pDataCaps
 *          - Data Capabilities Array.
 *              - 1 - GPRS
 *              - 2 - EDGE
 *              - 3 - HSDPA
 *              - 4 - HSUPA
 *              - 5 - WCDMA
 *              - 6 - CDMA 1xRTT
 *              - 7 - CDMA 1xEV-DO Rev 0
 *              - 8 - CDMA 1xEV-DO Rev. A
 *              - 9 - GSM
 *              - 10 - EVDO Rev. B
 *              - 11 - LTE
 *              - 12 - HSDPA Plus
 *              - 13 - Dual Carrier HSDPA Plus
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *
 */
typedef void (* tFNDataCapabilities)(
   BYTE                       dataCapsSize,
   BYTE *                     pDataCaps );

/**
 *  Enables/disables the data capabilities callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer  (0 - disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: None\n
 *          Does not require communication with the device
 *
 */
ULONG SetDataCapabilitiesCallback(
    tFNDataCapabilities pCallback );

/**
 *  Signal strength callback function.
 *
 *  \param  signalStrength
 *          - Received signal strength (in dBm)
 *
 *  \param  radioInterface
 *          - Radio interface technology of the signal being measured\n
 *              See \ref Tables for Radio Interface
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *
 */
typedef void (* tFNSignalStrength)(
   INT8                       signalStrength,
   ULONG                      radioInterface );

/**
 *  Enables/disables the Signal Strength callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0-Disable)
 *
 *  \param  thresholdsSize
 *          - Number of elements threshold array contains; a maximum of five
 *            thresholds is supported;
 *          - This parameter is not used when disabling the callback.
 *
 *  \param  pThresholds[IN]
 *          - Signal threshold array for each entry (in dBm).
 *          - This parameter is not used when disabling the callback.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 * \note    The signal strength callback function is called when a threshold in
 *          the threshold array is crossed.
 *
 */
ULONG SetSignalStrengthCallback(
    tFNSignalStrength pCallback,
    BYTE              thresholdsSize,
    INT8              *pThresholds );

/*
 *  For internal use only, not to be exposed to the user
 *  This structure will hold the input parameters passed for
 *  SetSignalStrengthCallback by the user
 *
 *  \param  thresholdsSize
 *          - Number of elements threshold array contains; a maximum of five
 *            thresholds is supported;
 *
 *  \param  pThresholds[IN]
 *          - Signal threshold array for each entry (in dBm).
 *
 * Note:    None
 *
 */
struct SignalStrengthDataType
{
    BYTE thresholdsSize;
    INT8 thresholds[SIGSTRENGTH_THRESHOLD_ARR_SZ];
};

/*
 *  For internal use only, not to be exposed to the user
 *  Enables the Signal Strength callback function.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   None
 *
 */
ULONG iSetSignalStrengthCallback(
    tFNSignalStrength pCallback );

/**
 *  RF information callback function.
 *
 *  \param  radioInterface
 *          - Radio interface technology of the signal being measured\n
 *              See \ref Tables for Radio Interface
 *
 *  \param  activeBandClass
 *          - Active band class\n
 *              See \ref Tables for Active Band Class
 *
 *  \param  activeChannel
 *          - Active channel
 *              - 0 - Channel is not relevant to the reported technology
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *
 */
typedef void (* tFNRFInfo)(
   ULONG                      radioInterface,
   ULONG                      activeBandClass,
   ULONG                      activeChannel );

/**
 *  Enables/disables the radio frequency information callback function. The most
 *  recent successfully subscribed callback function will be the only function
 *  that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0 - Disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetRFInfoCallback(
    tFNRFInfo pCallback );

/**
 *  LU reject callback function.
 *
 *  \param  serviceDomain
 *          - Service domain\n
 *              1 - Circuit Switched
 *
 *  \param  rejectCause
 *          - Reject cause\n
 *              See 3GPP TS 24.008, Section 4.4.4.7
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: None\n
 *
 */
typedef void (* tFNLUReject)(
   ULONG                      serviceDomain,
   ULONG                      rejectCause );

/**
 *  Enables/disables the LU reject callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 * \param  pCallback[IN]
 *         - Callback function pointer (0 - disable)
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 * \note   Technology Supported: UMTS/CDMA\n
 *         Device Supported: None\n
 *         Timeout: 2 seconds
 *
 */
ULONG SetLURejectCallback(
    tFNLUReject pCallback );

/**
 *  New SMS message callback function.
 *
 *  \param  storageType
 *          - SMS message storage type for the new message\n
 *              0 - UIM
 *              1 - NV
 *
 *  \param  messageIndex
 *          - Index of the new message
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *
 */
typedef void (* tFNNewSMS)(
   ULONG                      storageType,
   ULONG                      messageIndex );

/**
 *  Enables/disables the new SMS callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0 - Disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetNewSMSCallback(
    tFNNewSMS pCallback );

/**
 *  This enumeration defines the different type of SMS events that are received
 */
typedef enum SMSEventType
{
    SMS_EVENT_MT_MESSAGE                = 0x01,
    SMS_EVENT_TRANSFER_ROUTE_MT_MESSAGE = 0x02,
    SMS_EVENT_MESSAGE_MODE              = 0x04,
    SMS_EVENT_ETWS                      = 0x08,
    SMS_EVENT_ETWS_PLMN                 = 0x10,
    SMS_EVENT_SMSC_ADDRESS              = 0x20,
    SMS_EVENT_SMS_ON_IMS                = 0x40,
} eSMSEventType;

/**
 *  This structure holds information related to MT SMS
 *
 *  \param  storageType
 *          - SMS message storage type for the new message\n
 *              0 - UIM
 *              1 - NV
 *
 *  \param  messageIndex
 *          - Index of the new message
 */
typedef struct SMSMTMessage
{
    ULONG storageType;
    ULONG messageIndex;
} SMSMTMessageInfo;

/**
 *  This structure holds information related to transfer route MT SMS
 *
 *  \param  ackIndicator
 *          - Parameter to indicate if ACK must be sent by the control point
 *              0x00 - Send ACK
 *              0x01 - Do not send ACK
 *
 *  \param  transactionID
 *          - Transaction ID of the message
 *
 *  \param  format
 *          - Message format
 *              0x00 - CDMA
 *              0x02 - 0x05 - Reserved
 *              0x06 - GW_PP
 *              0x07 - GW_BC
 *
 *  \param  length
 *          - Length of the raw message. This length should not exceed the
 *            maximum WMS payload length of 256 bytes
 *
 *  \param  data
 *          - Raw message data
 */
typedef struct SMSTransferRouteMTMessage
{
    BYTE  ackIndicator;
    ULONG transactionID;
    BYTE  format;
    WORD  length;
    BYTE  data[QMI_WMS_MAX_PAYLOAD_LENGTH];
} SMSTransferRouteMTMessageInfo;

/**
 *  This structure holds information related to message mode
 *
 *  \param  messageMode
 *          - Message mode
 *              0x00 - CDMA
 *              0x01 - GW
 */
typedef struct SMSMessageMode
{
    BYTE  messageMode;
} SMSMessageModeInfo;

/**
 *  This structure holds information related earthquake and Tsunami warning
 *  system
 *
 *  \param  notificationType
 *          - Message mode
 *              0x00 - Primary
 *              0x01 - Secondary GSM
 *              0x02 - Secondary UMTS
 *
 *  \param  length
 *          - Number of sets of following elements
 *
 *  \param  data
 *          - Raw message data
 */
typedef struct SMSEtwsMessage
{
    BYTE notificationType;
    WORD length;
    BYTE data[QMI_WMS_MAX_PAYLOAD_LENGTH];
} SMSEtwsMessageInfo;

/**
 *  This structure holds information related ETWS PLMN
 *
 *  \param  mobileCountryCode
 *          - 16 bit representation of MCC
 *              value range : 0 -999
 *
 *  \param  mobileNetworkCode
 *          - 16 bit representation of MNC
 *              value range : 0 -999
 *
 */
typedef struct SMSEtwsPlmn
{
    WORD mobileCountryCode;
    WORD mobileNetworkCode;
} SMSEtwsPlmnInfo;

/**
 *  This structure holds SMSC information
 *
 *  \param  length
 *          - Number of sets of following element
 *
 *  \param  data
 *          - SMSC address
 */
typedef struct SMSCAddress
{
    BYTE length;
    BYTE data[QMI_WMS_MAX_PAYLOAD_LENGTH];
} SMSCAddressInfo;

/**
 *  This structure holds information related to message mode
 *
 *  \param  smsOnIMS
 *          - Indicates whether the message is received from IMS
 *              0x00 - Message is not recieved from IMS
 *              0x01 - Message is recieved from IMS
 *              0x02-0xFF - Reserved
 *          Note: In multiple modem solutions, this TLV may be used to help the
 *          client determine with which modem to communicate. This TLV may not
 *          be supported on all implementations.
 */
typedef struct SMSOnIMS
{
    BYTE smsOnIMS;
}SMSOnIMSInfo;

/**
 *  This structure will hold the information related to received SMS events
 *
 *  \param  smsEventType
 *          - Type of the SMS events that are received. This is a bit map of
 *            \ref SMSEventType. Only the parameters (which follows) related to
 *            the events received would be filled, and the rest of the
 *            parameters would be NULL
 *
 *  \param  pMTMessageInfo
 *          - pointer to the \ref SMSMTMessageInfo structure\n
 *            NULL, if this event is not present in the smsEventType parameter
 *
 *  \param  pTransferRouteMTMessageInfo
 *          - pointer to the \ref SMSTransferRouteMTMessageInfo structure\n.
 *            NULL, if this event is not present in the smsEventType parameter
 *
 *  \param  pMessageModeInfo
 *          - pointer to the \ref SMSMessageModeInfo structure\n
 *            NULL, if this event is not present in the smsEventType parameter
 *
 *  \param  pEtwsMessageInfo
 *          - pointer to the \ref SMSEtwsMessageInfo structure\n
 *            NULL, if this event is not present in the smsEventType parameter
 *
 *  \param  pEtwsPlmnInfo
 *          - pointer to the \ref SMSEtwsPlmnInfo structure\n
 *            NULL, if this event is not present in the smsEventType parameter
 *
 *  \param  pSMSCAddressInfo
 *          - pointer to the \ref SMSCAddressInfo structure\n
 *            NULL, if this event is not present in the smsEventType parameter
 *
 *  \param  pSMSOnIMSInfo
 *          - pointer to the \ref SMSOnIMSInfo structure\n
 *            NULL, if this event is not present in the smsEventType parameter
 * Note:    None
 *
 */
typedef struct SMSEventInfo_s
{
    BYTE                          smsEventType;
    SMSMTMessageInfo              *pMTMessageInfo;
    SMSTransferRouteMTMessageInfo *pTransferRouteMTMessageInfo;
    SMSMessageModeInfo            *pMessageModeInfo;
    SMSEtwsMessageInfo            *pEtwsMessageInfo;
    SMSEtwsPlmnInfo               *pEtwsPlmnInfo;
    SMSCAddressInfo               *pSMSCAddressInfo;
    SMSOnIMSInfo                  *pSMSOnIMSInfo;
} SMSEventInfo;

/**
 *  SMS event related callback function.
 *
 *  \param  pSMSEventInfo[OUT]
 *          - Events related to SMS, see \ref SMSEventInfo for details
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750,GOBI,MC7700\n
 *
 */
typedef void (* tFNSMSEvents)( SMSEventInfo *pSMSEventInfo );

/**
 *  Enables/disables the events related to SMS callback function.
 *  The most recent successfully subscribed callback function will be the only
 *  function that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0 - Disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750,GOBI,MC7700\n
 *          Timeout: 2 seconds
 *
 */
ULONG SLQSSetSMSEventCallback( tFNSMSEvents pCallback );

/**
 *  New NMEA sentence callback function.
 *
 *  \param  pNMEA
 *          - NULL-terminated string containing the position data
 *            in NMEA sentence format
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5\n
 *
 */
typedef void (* tFNNewNMEA)( LPCSTR pNMEA );

/**
 *  Enables/disables the NMEA sentence callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0 - Disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetNMEACallback(
    tFNNewNMEA pCallback );

/**
 *  PDS session state callback function.
 *
 *  \param  enabledStatus
 *          - GPS enabled status
 *              - 0 - Disable
 *              - 1 - Enable
 *
 *  \param  trackingStatus
 *          - GPS tracking status
 *              - 0 - Unknown
 *              - 1 - Inactive
 *              - 2 - Active
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5\n
 *
 */
typedef void (* tFNPDSState)(
   ULONG                      enabledStatus,
   ULONG                      trackingStatus );

/**
 *  Enables/disables the PDS service state callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0 - Disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetPDSStateCallback(
    tFNPDSState pCallback );

/**
 *  CAT event callback function.
 *
 *  \param  eventID
 *          - Event ID
 *              - 16 - Display Text
 *              - 17 - Get In-Key
 *              - 18 - Get Input
 *              - 19 - Setup Menu
 *              - 20 - Select Item
 *              - 21 - Send SMS - Alpha Identifier
 *              - 22 - Setup Event List
 *              - 23 - Setup Idle Mode Text
 *              - 24 - Language Notification
 *              - 25 - Refresh
 *              - 26 - End Proactive Session
 *
 *  \param  eventLen
 *          - Length of pData (in bytes)
 *
 *  \param  pEventData
 *          - Data specific to the CAT event ID
 *            See \ref currentCatEvent for details
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5\n
 *
 */
typedef void (* tFNCATEvent)(
   ULONG                      eventID,
   ULONG                      eventLen,
   BYTE *                     pEventData );

/**
 *  Enables/disables the CAT event callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0 - Disable)
 *  \param  eventMask
 *          - bitmask of CAT events to register for
 *              - 0x00000001 - Display Text
 *              - 0x00000002 - Get In-Key
 *              - 0x00000004 - Get Input
 *              - 0x00000008 - Setup Menu
 *              - 0x00000010 - Select Item
 *              - 0x00000020 - Send SMS - Alpha Identifier
 *              - 0x00000040 - Setup Event: User Activity
 *              - 0x00000080 - Setup Event: Idle Screen Notify
 *              - 0x00000100 - Setup Event: Language Sel Notify
 *              - 0x00000200 - Setup Idle Mode Text
 *              - 0x00000400 - Language Notification
 *              - 0x00000800 - Refresh
 *              - 0x00001000 - End Proactive Session
 *  \param  pErrorMask[OUT]
 *          - error bitmask. Each bit set indicates the proactive command that
 *            caused the error
 *              - 0x00000001 - Display Text
 *              - 0x00000002 - Get In-Key
 *              - 0x00000004 - Get Input
 *              - 0x00000008 - Setup Menu
 *              - 0x00000010 - Select Item
 *              - 0x00000020 - Send SMS - Alpha Identifier
 *              - 0x00000040 - Setup Event: User Activity
 *              - 0x00000080 - Setup Event: Idle Screen Notify
 *              - 0x00000100 - Setup Event: Language Sel Notify
 *              - 0x00000200 - Setup Idle Mode Text
 *              - 0x00000400 - Language Notification
 *              - 0x00000800 - Refresh
 *              - 0x00001000 - End Proactive Session
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetCATEventCallback(
    tFNCATEvent pCallback,
    ULONG       eventMask,
    ULONG       *pErrorMask );

/*
 *  For internal use only, not to be exposed to the user
 *  This structure will hold the input parameters passed for
 *  SetCATEventCallback by the user
 *
 *  \param  eventMask
 *          - bitmask of CAT events to register for
 *
 *  \param  pErrorMask[OUT]
 *          - error bitmask. Each bit set indicates the proactive command that
 *            caused the error
 *
 * Note:    None
 *
 */
struct CATEventDataType
{
    ULONG eventMask;
    ULONG *pErrorMask;
};

/*
 *  For internal use only, not to be exposed to the user
 *  Enables the CAT Event callback function.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   None
 *
 */
ULONG iSetCATEventCallback(
    tFNCATEvent pCallback );

/**
 *  Device State enumeration
 *
 */
typedef enum device_state_enum
{
    DEVICE_STATE_DISCONNECTED,
    DEVICE_STATE_READY
}eDevState;

/**
 *  Device State Change callback function prototype
 *
 *  \param  device_state
 *          - the current state of the device
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: None\n
 *          Does not require communication with the device
 *
 */
typedef void (* tFNDeviceStateChange)(
    eDevState   device_state );

/**
 *  Used by the client application to register a Callback function for
 *  Device State Change (DSC) event notifications. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - a valid function pointer to be notified of DSC events
 *          - NULL to disable DSC event notification
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: N/A
 *
 */
ULONG SetDeviceStateChangeCbk(
    tFNDeviceStateChange pCallback );

/**
 *  Firmware Download Completion callback function prototype
 *
 *  \param  error_code
 *          - error code returned from firmware download operation
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: None\n
 *          Does not require communication with the device
 *
 */
/* RILSTART */
typedef void (* tFNFwDldCompletion)( ULONG fwdld_completion_status );
/* RILSTOP */
/**
 *  Used by the client application to register a Callback function for
 *  a Firmware Download Completion (FDC) event notification. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - a valid function pointer to enable FDC event notification
 *          - NULL to disable FDC event notification
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: N/A
 */
ULONG SetFwDldCompletionCbk(
    tFNFwDldCompletion pCallback );

/**
 *  SWIOMA-DM network-initiated alert callback function
 *
 *  \param  sessionType
 *          - 0x00 - SWIOMA-DM FOTA
 *          - 0x01 - SWIOMA-DM Config
 *
 *  \param  psessionTypeFields
 *          - Pointer to structure containing info for that sessionType
 *          - See \ref sessionInfo for more details
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750\n
 *
 */
typedef void (* tFNSLQSOMADMAlert)(
    ULONG sessionType,
    BYTE  *psessionTypeFields );

/**
 *  Enables/disables the SWIOMADM network-initiated alert callback function. The
 *  most recent successfully subscribed callback function will be the only
 *  function that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - a valid function pointer to enable SLQSOMADMAlert notification
 *          - NULL to disable SLQSOMADMAlert notification
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750\n
 *          Timeout: 2 seconds
 */
ULONG SetSLQSOMADMAlertCallback(
    tFNSLQSOMADMAlert pCallback );

/**
 *  OMA-DM state callback function
 *
 *  \param  sessionState
 *          - Session state
 *              - 0x00 - Complete, information was updated
 *              - 0x01 - Complete, update information is unavailable
 *              - 0x02 - Failed
 *              - 0x03 - Retrying
 *              - 0x04 - Connecting
 *              - 0x05 - Connected
 *              - 0x06 - Authenticated
 *              - 0x07 - Mobile Directory Number (MDN) downloaded
 *              - 0x08 - Mobile Station Identifier (MSID) downloaded
 *              - 0x09 - PRL downloaded
 *              - 0x0A - Mobile IP (MIP) profile downloaded
 *
 *  \param  failureReason
 *          - Session failure reason (when state indicates failure)
 *              - 0x00 - Unknown
 *              - 0x01 - Network is unavailable
 *              - 0x02 - Server is unavailable
 *              - 0x03 - Authentication failed
 *              - 0x04 - Maximum retry exceeded
 *              - 0x05 - Session is cancelled
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC83x5\n
 *
 */
typedef void (* tFNOMADMState)(
    ULONG sessionState,
    ULONG failureReason );

/**
 *  Enables/disables the OMADM state callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - a valid function pointer to enable OMADMState notification
 *          - NULL to disable OMADMState notification
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds
 */
ULONG SetOMADMStateCallback(
    tFNOMADMState pCallback );

#define  MAX_RADIO_INTERFACE_LIST 255
/**
 *  This structure will hold the serving system parameters information
 *
 *  \param  registrationState - Registration state of the mobile
 *          - 0 - QMI_NAS_NOT_REGISTERED  Not registered;mobile is not
 *                currently searching for a new network to provide service
 *          - 1 - QMI_NAS_REGISTERED  Registered with a network
 *          - 2 - QMI_NAS_NOT_REGISTERED_SEARCHING  Not registered, but mobile
 *                is currently searching for a new network to provide service
 *          - 3 - QMI_NAS_REGISTRATION_DENIED Registration denied by the
 *                visible network
 *          - 4 - QMI_NAS_REGISTRATION_UNKNOWN Registration state is unknown
 *
 *  \param  csAttachState  - Circuit Switch domain attach state of the mobile
 *          - 0 - Unknown or not applicable
 *          - 1 - Attached
 *          - 2 - Detached
 *
 *  \param  psAttachState  - Packet domain attach state of the mobile
 *          - 0 - Unknown or not applicable
 *          - 1 - Attached
 *          - 2 - Detached
 *
 *  \param  selectedNetwork - Type of selected radio access network
 *          - 0x00 - Unknown
 *          - 0x01 - 3GPP2 network
 *          - 0x02 - 3GPP network
 *
 *  \param  radioInterfaceNo - Number of radio interfaces currently in use;
 *                             this  indicates how many radio_if identifiers
 *                             follow this field
 *
 *  \param  radioInterfaceList - Radio interface currently in use
 *                               (each is 1 byte)
 *          - 0x00 - None (no service)
 *          - 0x01 - cdma2000 1X
 *          - 0x02 - cdma2000 HRPD (1xEV-DO)
 *          - 0x03 - AMPS
 *          - 0x04 - GSM
 *          - 0x05 - UMTS
 *          - 0x08 - LTE
 *
 *  Note:   None
 */
struct ServingSystemInfo
{
    BYTE registrationState;
    BYTE csAttachState;
    BYTE psAttachState;
    BYTE selectedNetwork;
    BYTE radioInterfaceNo;
    BYTE radioInterfaceList[MAX_RADIO_INTERFACE_LIST];
};

/**
 *  Serving System callback function
 *
 *  \param  pServingSystem
 *          - ServingSystemInfo structure
 *
 *  \note   Technology Supported: UMTS/CDMA/LTE\n
 *          Device Supported: MC83x5,MC77xx\n
 *
 */
typedef void (* tFNServingSystem)(
    struct ServingSystemInfo *pServingSystem );

/**
 *  Enables/disables the Serving System callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - a valid function pointer to enable ServingSystem notification
 *          - NULL to disable ServingSystem notification
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \note   Technology Supported: UMTS/CDMA/LTE\n
 *          Device Supported: MC83x5,MC77xx\n
 *          Timeout: 2 seconds
 */
ULONG SLQSSetServingSystemCallback(
    tFNServingSystem pCallback );

/**
 *  Band Preference Callback function
 *
 *  \param  pBandPref - Bit mask representing the current band preference
 *          Bit position meanings:
 *          - 0 - BC0_A - Band Class 0, A-System
 *          - 1 - BC0_B - Band Class 0, B-System, Band Class 0
 *                AB , GSM 850 Band
 *          - 2 - BC1 - Band Class 1, all blocks
 *          - 3 - BC2 - Band Class 2 place holder
 *          - 4 - BC3 - Band Class 3, A-System
 *          - 5 - BC4 - Band Class 4, all blocks
 *          - 6 - BC5 - Band Class 5, all blocks
 *          - 7 - GSM_DCS_1800 - GSM DCS band
 *          - 8 - GSM_EGSM_900 - GSM Extended GSM (E-GSM) band
 *          - 9 - GSM_PGSM_900 - GSM Primary GSM (P-GSM) band
 *          - 10 - BC6 - Band Class 6
 *          - 11 - BC7 - Band Class 7
 *          - 12 - BC8 - Band Class 8
 *          - 13 - BC9 - Band Class 9
 *          - 14 - BC10 - Band Class 10
 *          - 15 - BC11 - Band Class 11
 *          - 16 - GSM_450 - GSM 450 band
 *          - 17 - GSM_480 - GSM 480 band
 *          - 18 - GSM_750 - GSM 750 band
 *          - 19 - GSM_850 - GSM 850 band
 *          - 20 - GSM_RGSM_900 - GSM Railways GSM Band
 *          - 21 - GSM_PCS_1900 - GSM PCS band
 *          - 22 - WCDMA_I_IMT_2000 - WCDMA EUROPE JAPAN
 *                 and CHINA IMT 2100 band
 *          - 23 - WCDMA_II_PCS_1900 - WCDMA US PCS 1900 band
 *          - 24 - WCDMA_III_1700 - WCDMA EUROPE and CHINA DCS 1800 band
 *          - 25 - WCDMA_IV_1700 - WCDMA US 1700 band
 *          - 26 - WCDMA_V_850 - WCDMA US 850 band
 *          - 27 - WCDMA_VI_800 - WCDMA JAPAN 800 band
 *          - 28 - BC12 - Band Class 12
 *          - 29 - BC14 - Band Class 14
 *          - 30 - RESERVED_2 - Reserved 2
 *          - 31 - BC15 - Band Class 15
 *          - 32 - 47 - Reserved
 *          - 48 - WCDMA_VII_2600 - WCDMA EUROPE 2600 band
 *          - 49 - WCDMA_VIII_900 - WCDMA EUROPE and JAPAN 900 band
 *          - 50 - WCDMA_IX_1700 - WCDMA JAPAN 1700 band
 *          - 51 to 55 - Reserved
 *          - 56 - BBC16 - Band Class 16
 *          - 57 - BC17 - Band Class 17
 *          - 58 - BC18 - Band Class 18
 *          - 59 - BC19 - Band Class 19
 *          - 60 to 64 - Reserved
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: NA
 *          To set the band preference the  API SLQSSetBandPreference()
 *          should be used
 *
 */
typedef void (* tFNBandPreference)(
    ULONGLONG  band_pref );

/**
 *  Enables/disables the Band Preference callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - a valid function pointer to enable Band Preference Indication
 *            notification
 *          - NULL to disable Band Preference notification
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: NA
 *          To set the band preference the  API SLQSSetBandPreference()
 *          should be used
 */
ULONG SLQSSetBandPreferenceCbk(
    tFNBandPreference pCallback );

/**
 * USSD releasecallback function prototype
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5\n
 */
typedef void (* tFNUSSDRelease)( void );

/**
 *  Enables/disables the USSD release callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - a valid function pointer to enable ServingSystem notification
 *          - NULL to disable ServingSystem notification
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5\n
 *          Timeout: Does not require communication with the device
 */
ULONG SetUSSDReleaseCallback(
    tFNUSSDRelease pCallback );

/**
 * SetUSSDNotificationCallback function prototype
 *
 * \param  type - Notification type
 *         - 0x01 - No action required
 *         - 0x02 - Action required
 *
 * \param  pNetworkInfo
 *         - USS information from the network (0 indicates that
 *           no info was received)
 *           - See \ref USSInfo for more details
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5\n
 */
typedef void (* tFNUSSDNotification)(
    ULONG type,
    BYTE  *pNetworkInfo );

/**
 *  Enables/disables the USSDNotification callback function. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - a valid function pointer to enable ServingSystem notification
 *          - NULL to disable ServingSystem notification
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5\n
 *          Timeout: Does not require communication with device
 */
ULONG SetUSSDNotificationCallback(
    tFNUSSDNotification pCallback );

/**
 *  Structure for Received Signal Strength Information.
 *
 *  \param  rxSignalStrengthInfo
 *          - See \ref rxSignalStrengthListElement for more information.
 *
 *  \param  ecioInfo
 *          - See \ref ecioListElement for more information.
 *
 *  \param  io
 *          - Received IO in dBm; IO is only applicable for 1xEV-DO.
 *
 *  \param  sinr
 *          - SINR level
 *              - SINR is only applicable for 1xEV-DO; valid levels are 0 to 8
 *                where maximum value for
 *                0 - SINR_LEVEL_0 is -9 dB
 *                1 - SINR_LEVEL_1 is -6 dB
 *                2 - SINR_LEVEL_2 is -4.5 dB
 *                3 - SINR_LEVEL_3 is -3 dB
 *                4 - SINR_LEVEL_4 is -2 dB
 *                5 - SINR_LEVEL_5 is +1 dB
 *                6 - SINR_LEVEL_6 is +3 dB
 *                7 - SINR_LEVEL_7 is +6 dB
 *                8 - SINR_LEVEL_8 is +9 dB
 *
 *  \param  errorRateInfo
 *          - See \ref errorRateListElement for more information.
 *
 *  \param  rsrqInfo
 *          - See \ref rsrqInformation for more information.
 *
 *  \note    None
 *
 */
struct SLQSSignalStrengthsInformation{
    struct rxSignalStrengthListElement rxSignalStrengthInfo;
    struct ecioListElement             ecioInfo;
    ULONG                              io;
    BYTE                               sinr;
    struct errorRateListElement        errorRateInfo;
    struct rsrqInformation             rsrqInfo;
};

/**
 *  Received Signal Strength Information callback function.
 *
 *  \param  sSLQSSignalStrengthsInfo
 *          - See \ref SLQSSignalStrengthsInformation for more information.
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/71/50\n
 *
 */
typedef void (* tFNSLQSSignalStrengths )(
    struct SLQSSignalStrengthsInformation sSLQSSignalStrengthsInfo );

/**
 *  Structure for storing the input parameters passed for
 *  SLQSSetSignalStrengthsCallback by the user.
 *
 *  \param  rxSignalStrengthDelta
 *          - RSSI delta(in dBm) at which an event report indication,
 *            including the current RSSI, will be sent to the requesting
 *            control point.
 *
 *  \param  ecioDelta
 *          - ECIO delta at which an event report indication, ecioDelta
 *            including the current ECIO, will be sent to the requesting
 *            control point.
 *          - ECIO delta is an unsigned 1 byte value that increments in
 *            negative 0.5 dBm, e.g., ecio_delta of 2 means a change of -1 dBm.
 *
 *  \param  ioDelta
 *          - IO delta (in dBm) at which an event report indication,
 *            ioDelta including the current IO, will be sent to the
 *            requesting control point.
 *
 *
 *  \param  sinrDelta
 *          - SINR delta level at which an event report indication, sinrDelta
 *            including the current SINR, will be sent to the requesting control
 *            point.
 *
 *  \param  rsrqDelta
 *          - RSRQ delta level at which an event report indication,
 *            including the current RSRQ, will be sent to the requesting
 *            control point.
 *
 *  \param  ecioThresholdListLen
 *          - Number of elements in the ECIO threshold list.
 *
 *  \param  ecioThresholdList
 *          - A sequence of thresholds delimiting EcIo event reporting
 *            bands. Every time a new EcIo value crosses a threshold value,
 *            an event report indication message with the new ECIO
 *            value is sent to the requesting control point. For this field:
 *            - Maximum number of threshold values is 10
 *            - At least one value must be specified.
 *
 *  \param  sinrThresholdListLen
 *          - Number of elements in the SINR threshold list.
 *
 *  \param  sinrThresholdList
 *          - A sequence of thresholds delimiting SINR event reporting bands.
 *            Every time a new SINR value crosses a threshold value, an event
 *            report indication message with the new sinr value is sent to the
 *            requesting control point. For this field:
 *            - Maximum number of threshold values is 5
 *            - At least one value must be specified.
 *
 * \note    None
 *
 */
struct SLQSSignalStrengthsIndReq{
    BYTE  rxSignalStrengthDelta;
    BYTE  ecioDelta;
    BYTE  ioDelta;
    BYTE  sinrDelta;
    BYTE  rsrqDelta;
    BYTE  ecioThresholdListLen;
    SHORT ecioThresholdList[10];
    BYTE  sinrThresholdListLen;
    BYTE  sinrThresholdList[5];
};

/**
 *  Enables/disables the Received Signal Strength Information callback
 *  function. The most recent successfully subscribed callback function will be
 *  the only function that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0-Disable)
 *
 *  \param  pSLQSSignalStrengthsIndReq
 *          - See \ref SLQSSignalStrengthsIndReq for more information
 *          - This parameter is not used when disabling the callback.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10\n
 *          Timeout: 2 seconds
 *
 * \note    The signal strength callback function is called when a threshold in
 *          the threshold array is crossed.
 *
 */
ULONG SLQSSetSignalStrengthsCallback(
    tFNSLQSSignalStrengths           pCallback,
    struct SLQSSignalStrengthsIndReq *pSLQSSignalStrengthsIndReq );

/*
 *  For internal use only, not to be exposed to the user
 *  Enables the Received Signal Strength Information callback function.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   None
 *
 */
ULONG iSLQSSetSignalStrengthsCallback(
    tFNSLQSSignalStrengths pCallback );

/**
 *  Contains the parameters passed for Explicit Communication Transfer by
 *  the device.
 *
 *  \param  ECTCallState
 *          - ECT call state:
 *              - 0x00 - ECT_CALL_STATE_NONE - None
 *              - 0x01 - ECT_CALL_STATE_ALERTING - Alerting
 *              - 0x02 - ECT_CALL_STATE_ACTIVE - Active
 *
 *  \param  presentationInd
 *          - Presentation indicator
 *              - 0x00 - presentationAllowedAddress
 *              - 0x01 - presentationRestricted
 *              - 0x02 - numberNotAvailable
 *              - 0x04 - presentationRestrictedAddress
 *
 *  \param  number
 *          - Number in ASCII characters terminated by NULL
 *
 */
typedef struct
{
    BYTE ECTCallState;
    BYTE presentationInd;
    BYTE number[QMI_MAX_VOICE_NUMBER_LENGTH];
} ECTNum;

/**
 *  Contains the parameters passed for SLQSVoiceSetSUPSNotificationCallback by
 *  the device.
 *
 *  \param  callID
 *          - Unique identifier of the call for which the notification is
 *            applicable. (mandatory)
 *
 *  \param  notifType
 *          - Notification type parameter (mandatory)
 *              - 0x01 - NOTIFICATION_TYPE_OUTGOING_CALL_IS_FORWARDED\n
 *                  Originated MO call is being forwarded to another user
 *              - 0x02 - NOTIFICATION_TYPE_OUTGOING_CALL_IS_WAITING\n
 *                  Originated MO call is waiting at the called user
 *              - 0x03 - NOTIFICATION_TYPE_OUTGOING_CUG_CALL\n
 *                  Outgoing call is a CUG call
 *              - 0x04 - NOTIFICATION_TYPE_OUTGOING_CALLS_BARRED\n
 *                  Outgoing calls are barred
 *              - 0x05 - NOTIFICATION_TYPE_OUTGOING_CALL_IS_DEFLECTED\n
 *                  Outgoing call is deflected
 *              - 0x06 - NOTIFICATION_TYPE_INCOMING_CUG_CALL\n
 *                  Incoming call is a CUG call
 *              - 0x07 - NOTIFICATION_TYPE_INCOMING_CALLS_BARRED\n
 *                  Incoming calls are barred
 *              - 0x08 - NOTIFICATION_TYPE_INCOMING_FORWARDED_CALL\n
 *                  Incoming call received is a forwarded call
 *              - 0x09 - NOTIFICATION_TYPE_INCOMING_DEFLECTED_CALL\n
 *                  Incoming call is a deflected call
 *              - 0x0A - NOTIFICATION_TYPE_INCOMING_CALL_IS_FORWARDED\n
 *                  Incoming call is forwarded to another user
 *              - 0x0B - NOTIFICATION_TYPE_UNCOND_CALL_FORWARD_ACTIVE\n
 *                  Unconditional call forwarding is active
 *              - 0x0C - NOTIFICATION_TYPE_COND_CALL_FORWARD_ACTIVE\n
 *                  Conditional call forwarding is active
 *              - 0x0D - NOTIFICATION_TYPE_CLIR_SUPPRESSION_REJECTED\n
 *                  CLIR suppression is rejected
 *              - 0x0E - NOTIFICATION_TYPE_CALL_IS_ON_HOLD\n
 *                  Call is put on hold at the remote party
 *              - 0x0F - NOTIFICATION_TYPE_CALL_IS_RETRIEVED\n
 *                  Call is retrieved at the remote party from the hold state
 *              - 0x10 - NOTIFICATION_TYPE_CALL_IS_IN_MPTY\n
 *                  Call is in a conference
 *              - 0x11 - NOTIFICATION_TYPE_INCOMING_CALL_IS_ECT\n
 *                  Incoming call is an explicit call transfer
 *
 *  \param  pCUGIndex
 *          - The CUG Index used to indicate that the incoming/outgoing
 *            call is a CUG call. (optional, NULL when not present)\n
 *            Range: 0x00 to 0x7FFF.
 *
 *  \param  pECTNum
 *          - The ECT Number is used to indicate that the incoming call is an
 *            explicitly transferred call. (optional, NULL when not present)\n
 *            Refer ECTNum for details.
 *
 * \note    None
 *
 */
typedef struct
{
    BYTE   callID;
    BYTE   notifType;
    WORD   *pCUGIndex;
    ECTNum *pECTNum;
} voiceSUPSNotification;

/**
 *  Supplementary service notification callback.
 *
 *  \param  pVoiceSUPSNotification
 *          - See \ref voiceSUPSNotification for more information.
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: SL9090\n
 */
typedef void (*tFNSUPSNotification)
    ( voiceSUPSNotification *pVoiceSUPSNotification );

/**
 *  Enables/disables the supplementary service notification callback function.
 *  The most recent successfully subscribed callback function will be the only
 *  function that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0-Disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 2 seconds
 */
ULONG SLQSVoiceSetSUPSNotificationCallback( tFNSUPSNotification pCallback );

/**
 *  SDK terminated callback function prototype
 *
 *  \param  psReason
 *          - sdk termination reason string
 *
 *  \note   Technology Supported: N/A\n
 *          Device Supported: N/A\n
 *          Timeout: None\n
 *          Does not require communication with the device
 *
 */
typedef void (* tFNSDKTerminated)(
    BYTE *psReason );

/**
 *  Used by the client application to register a Callback function for
 *  SDK terminated event notifications. The most recent
 *  successfully subscribed callback function will be the only function that
 *  is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - a valid function pointer to be notified of SWI events
 *          - NULL to disable SWI event notification
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: N/A\n
 *          Device Supported: N/A\n
 *          Timeout: N/A\n
 *          The following signals will trigger this callback:
  * <pre>
 *          2 INT      4 ILL      5 TRAP     6 ABRT     7 BUS
 *          8 FPE     11 SEGV    13 PIPE    15 TERM    31 SYS
 * </pre>
 *
 */
ULONG SLQSSetSDKTerminatedCallback(
    tFNSDKTerminated pCallback );

/**
 *  This structure contains VoiceCall Information parameters.
 *  arrCallInfomation will be populated in case of change in the call
 *  information. Other paramters are optional therefore are populated based on
 *  device and technology type being used.
 *
 *  \param  arrCallInfomation [mandatory]
 *          - Array of Call Information\n
 *            This must be populated if Indication is received
 *            See \ref arrCallInfo for more information.
 *            - Applicable for both "3GPP/3GPP2"
 *
 *  \param  pArrRemotePartyNum [optional]
 *          - Array of Remote Party Name.( NULL when not present)\n
 *            See \ref arrRemotePartyNum for more information.\n
 *            - Applicable only for "3GPP/3GPP2"
 *
 *  \param  pArrRemotePartyName [optional]
 *          - Array of Alerting Type.( NULL when not present)\n
 *            See \ref arrRemotePartyName for more information.
 *            - Applicable only for "3GPP"
 *
 *  \param  pArrAlertingType [optional]
 *          - Array of Alerting Type( NULL when not present)\n
 *            See \ref arrAlertingType for more information.
 *            - Applicable only for "3GPP"
 *
 *  \param  pArrSvcOption [optional]
 *          - Array of Service Option.(NULL when not present)\n
 *            See \ref arrSvcOption for more information.
 *            - Applicable only for "3GPP"
 *
 *  \param  pArrCallEndReason [optional]
 *          - Array of Call End Reason.( NULL when not present)\n
 *            See \ref arrCallEndReason for more information.
 *            - Applicable only for "3GPP"
 *
 *  \param  pArrAlphaID [optional]
 *          - Array of Alpha Identifier( NULL when not present)\n
 *            See \ref arrAlphaID for more information.
 *            - Applicable only for "3GPP"
 *
 *  \param  pArrConnectPartyNum [optional]
 *          - Array of Connected Party Number.( NULL when not present)\n
 *            See \ref arrConnectPartyNum for more information.
 *            - Applicable for both "3GPP/3GPP2"
 *
 *  \param  pArrDiagInfo [optional]
 *          - Array of Diagnostic Information.( NULL when not present)\n
 *            See \ref arrDiagInfo for more information.
 *            - Applicable only for "3GPP"
 *
 *  \param  pArrCalledPartyNum [optional]
 *          - Array of Called Party Number.( NULL when not present)\n
 *            See \ref arrCalledPartyNum for more information.
 *            - Applicable only for "3GPP"
 *
 *  \param  pArrRedirPartyNum [optional]
 *          - Array of Redirecting Party Number.( NULL when not present)\n
 *           See \ref arrRedirPartyNum for more information.
 *            - Applicable only for "3GPP"
 *
 *  \param  pArrAlertingPattern [optional]
 *          - Array of Alerting Pattern.( NULL when not present)\n
 *            See \ref arrAlertingPattern for more information.
 *            - Applicable only for "3GPP"
 *
 *  \note   Optional paramters would be NULL, if not received from the device.
 *
 */
typedef struct
{
    arrCallInfo        arrCallInfomation;
    arrRemotePartyNum  *pArrRemotePartyNum;
    arrRemotePartyName *pArrRemotePartyName;
    arrAlertingType    *pArrAlertingType;
    arrSvcOption       *pArrSvcOption;
    arrCallEndReason   *pArrCallEndReason;
    arrAlphaID         *pArrAlphaID;
    arrConnectPartyNum *pArrConnectPartyNum;
    arrDiagInfo        *pArrDiagInfo;
    arrCalledPartyNum  *pArrCalledPartyNum;
    arrRedirPartyNum   *pArrRedirPartyNum;
    arrAlertingPattern *pArrAlertingPattern;
} voiceSetAllCallStatusCbkInfo;

/**
 *  Voice Call Status Callback function.
 *  This funtion pointer will be executed to process received Indication.
 *
 *
 *  \param  pVoiceSetAllCallStatusCbkInfo
 *          - Call back will populated memory pointed by this parameter
 *            when a call is originated, connected, or ended.\n
 *            See \ref voiceSetAllCallStatusCbkInfo for more information.
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported:     SL9090\n
 *
 */
typedef void (* tFNAllCallStatus )(
    voiceSetAllCallStatusCbkInfo *pVoiceSetAllCallStatusCbkInfo );

/**
 *  Enables/disables Voice Call Status Callback function.
 *  User can subscribe this callback get the call state change notifications.
 *  eg:- Call originated,connected, or ended. Whenever there is a change in
 *  the call information, there will be a indication with the information.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0 - Disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported:     SL9090\n
 *          Timeout: 2 seconds\n
 *          This callback requires a firmware with atleast voice 2.0 support.
 *
 */
ULONG SLQSVoiceSetAllCallStatusCallBack (
    tFNAllCallStatus pCallback );

/**
 *  Contains the parameters passed for SLQSSetTransLayerInfoCallback by
 *  the device.
 *
 *  \param  regInd
 *          - Indicates whether the transport layer is registered or not
 *          - Values:
 *              - 0x00 - Transport layer is not registered
 *              - 0x01 - Transport layer is registered
 *
 *  \param  pTransLayerInfo
 *          - Optional parameter
 *          - See \ref transLayerInfo for more information
 *
 * \note    None
 *
 */
typedef struct _transLayerInfoNotification
{
    BYTE           regInd;
    transLayerInfo *pTransLayerInfo;
} transLayerNotification;

/**
 *  Transport Layer Information callback.
 *
 *  \param  transLayerNotification
 *          - See \ref transLayerNotification for more information.
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7700/10/50\n
 */
typedef void (*tFNtransLayerInfo)
    ( transLayerNotification *pTransLayerNotification );

/**
 *  Enables/disables the Transport Layer information callback function.
 *  The most recent successfully subscribed callback function will be the only
 *  function that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0-Disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7700/10/50\n
 *          Timeout: 2 seconds
 */
ULONG SLQSSetTransLayerInfoCallback( tFNtransLayerInfo pCallback );

/**
 *  Contains the parameters passed for SLQSSetTransNWRegInfoCallback by
 *  the device.
 *
 *  \param  NWRegStat
 *          - provides the transport network registration information
 *          - Values:
 *              - 0x00 - No Service
 *              - 0x01 - In Progress
 *              - 0x02 - Failed
 *              - 0x03 - Limited Service
 *              - 0x04 - Full Service
 *
 * \note    None
 *
 */
typedef struct _transNWRegInfoNotification
{
    BYTE NWRegStat;
} transNWRegInfoNotification;

/**
 *  Transport Network Registration Information callback.
 *
 *  \param  pTransNWRegInfoNotification
 *          - See \ref transNWRegInfoNotification for more information.
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7700/10/50\n
 */
typedef void (*tFNtransNWRegInfo)
    ( transNWRegInfoNotification *pTransNWRegInfoNotification );

/**
 *  Enables/disables the Transport Network Registration information callback
 *  function. The most recent successfully subscribed callback function will
 *  be the only function that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0-Disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7700/10/50\n
 *          Timeout: 2 seconds
 */
ULONG SLQSSetTransNWRegInfoCallback( tFNtransNWRegInfo pCallback );

/**
 *  System Selection Preference Callback function
 *
 *  \param  pSysSelectPrefInfo
 *          - Current System Selection preferences for the device.
 *          - See \ref sysSelectPrefInfo for more information
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7700/10/50, MC83x5\n
 */
typedef void (* tFNSysSelectionPref )
       ( sysSelectPrefInfo *pSysSelectPrefInfo );

/**
 *  Enables/disables the System Selection Preference callback function.
 *  The most recent successfully subscribed callback function will be the
 *  only function that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - a valid function pointer to enable System Selection Preference
 *            Indication notification
 *          - NULL to disable Band Preference notification
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7700/10/50, MC83x5\n
 *          Timeout: 2 seconds\n
 *          To set the system selection preferences the  API
 *          SLQSSetSysSelectionPref() should be used
 */
ULONG SLQSSetSysSelectionPrefCallBack( tFNSysSelectionPref pCallback );

/**
 *  UIM Refresh Callback function
 *
 *  \param  pUIMRefreshEvent
 *          - Pointer to Refresh Event structure.
 *          - See \ref UIMRefreshEvent for more information
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7700/10/50, MC83x5\n
 */
typedef void (* tFNUIMRefresh )
    ( UIMRefreshEvent *pUIMRefreshEvent );

/**
 *  Enables/disables the UIM refresh callback function.
 *  The most recent successfully subscribed callback function will be the
 *  only function that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - a valid function pointer to enable UIM Refresh
 *            Indication notification
 *          - NULL to disable Band Preference notification
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7700/10/50, MC83x5\n
 *          Timeout: 2 seconds\n
 *          SLQSUIMRefreshRegister() API should be invoked prior to the
 *          invocation of the callback for the events to be registered.
 */
ULONG SLQSUIMSetRefreshCallBack ( tFNUIMRefresh pCallback );

/**
 *  Structure consist of cardstatus params
 *
 *  \param  statusChange
 *          - See \ref cardStatus for more information
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7700/10/50, MC83x5\n
 */
typedef struct
{
    cardStatus statusChange;
}UIMStatusChangeInfo;

/**
 *  UIM Status Change Callback function
 *
 *  \param  pUIMStatusChangeInfo
 *          - Pointer to UIM status change stucture.
 *          - See \ref UIMStatusChangeInfo for more information
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7700/10/50, MC83x5\n
 */
typedef void ( * tFNUIMStatusChangeInfo)
    (UIMStatusChangeInfo  *pUIMStatusChangeInfo );

/**
 *  Enables/disables the UIM Status Change Callback function.
 *  The most recent successfully subscribed callback function will be the
 *  only function that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - a valid function pointer to enable UIM Status Change
 *            Indication notification
 *          - NULL to disable Band Preference notification
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7700/10/50, MC83x5\n
 *          Timeout: 2 seconds\n
 */
ULONG SLQSUIMSetStatusChangeCallBack (
    tFNUIMStatusChangeInfo pCallback );

/**
 *  Contains the parameters passed for SLQSVoiceSetPrivacyChangeCallBack by
 *  the device.
 *
 *  \param  callID
 *          - Unique identifier of the call for which the voice privacy is
 *            applicable. (mandatory)
 *
 *  \param  voicePrivacy
 *          - Voice Privacy (mandatory)
 *              - 0x00 - VOICE_PRIVACY_STANDARD - Standard privacy
 *              - 0x01 - VOICE_PRIVACY_ENHANCED - Enhanced privacy
 *
 * \note    None
 *
 */
typedef struct
{
    BYTE callID;
    BYTE voicePrivacy;
} voicePrivacyInfo;

/**
 *  Preferred voice privacy indication callback.
 *
 *  \param  pVoicePrivacyInfo
 *          - See \ref voicePrivacyInfo for more information.
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: SL9090\n
 */
typedef void ( * tFNPrivacyChange) ( voicePrivacyInfo *pVoicePrivacyInfo );

/**
 *  Enables/disables the voice privacy change callback function.
 *  The most recent successfully subscribed callback function will be the only
 *  function that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0-Disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: SL9090\n
 *          Timeout: 2 seconds\n
 *          This callback communicates a change in the voice privacy of a
 *          call. This is applicable only in 3GPP2 devices.
 *
 */
ULONG SLQSVoiceSetPrivacyChangeCallBack( tFNPrivacyChange pCallback );

/**
 *  This structure contains the parameters passed for
 *  SLQSVoiceSetDTMFEventCallBack by the device.
 *
 *  \param  DTMFInformation(mandatory)
 *          - See \ref DTMFInfo for more information.
 *
 *  \param  pOnLength(optional)
 *          - DTMF Pulse Width
 *              - 0x00 - DTMF_ONLENGTH_95MS - 95 ms
 *              - 0x01 - DTMF_ONLENGTH_150MS - 150 ms
 *              - 0x02 - DTMF_ONLENGTH_200MS - 200 ms
 *              - 0x03 - DTMF_ONLENGTH_250MS - 250 ms
 *              - 0x04 - DTMF_ONLENGTH_300MS - 300 ms
 *              - 0x05 - DTMF_ONLENGTH_350MS - 350 ms
 *              - 0x06 - DTMF_ONLENGTH_SMS - SMS Tx special pulse width
 *
 *  \param  pOffLength(optional)
 *          - DTMF Interdigit Interval
 *              - 0x00 - DTMF_OFFLENGTH_60MS - 60 ms
 *              - 0x01 - DTMF_OFFLENGTH_100MS - 100 ms
 *              - 0x02 - DTMF_OFFLENGTH_150MS - 150 ms
 *              - 0x03 - DTMF_OFFLENGTH_200MS - 200 ms
 *
 * \note    None
 *
 */
typedef struct
{
    DTMFInfo DTMFInformation;
    BYTE     *pOnLength;
    BYTE     *pOffLength;
} voiceDTMFEventInfo;

/**
 *  Preferred DTMF event indication callback.
 *
 *  \param  pVoiceDTMFEventInfo
 *          - See \ref voiceDTMFEventInfo for more information.
 *
 *  \note   Technology Supported: CDMA/GSM\n
 *          Device Supported: SL9090\n
 */
typedef void ( * tFNDTMFEvent)
    ( voiceDTMFEventInfo *pVoiceDTMFEventInfo );

/**
 *  Enables/disables the DTMF Event callback function.
 *  The most recent successfully subscribed callback function will be the only
 *  function that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0-Disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA/GSM\n
 *          Device Supported: SL9090\n
 *          Timeout: 2 seconds\n
 *          This callback communicates that a DTMF event has been received.
 *
 */
ULONG SLQSVoiceSetDTMFEventCallBack( tFNDTMFEvent pCallback );

/**
 *  This structure contains the parameters passed for SLQSVoiceSetSUPSCallBack
 *  by the device.
 *
 *  \param  SUPSInformation(mandatory)
 *          - See \ref SUPSInfo for more information.
 *
 *  \param  pSvcClass(optional)
 *          - Service class is a combination (sum) of information class
 *            constants (optional)
 *          - See qaGobiApiTableSupServiceInfoClasses.h for service classes.
 *
 *  \param  pReason(optional)
 *          - See qaGobiApiTableCallControlReturnReasons.h for return reasons.
 *
 *  \param  pCallFWNum(optional)
 *          - Call forwarding number to be registered with the network.
 *          - ASCII String, NULL terminated.
 *
 *  \param  pCallFWTimerVal(optional)
 *          - Call Forwarding No Reply Timer.
 *              - Range: 5 to 30 in steps of 5.
 *
 *  \param  pUSSInfo(optional)
 *          - See \ref USSInfo for more information.
 *
 *  \param  pCallID(optional)
 *          - Call identifier of the voice call that has been modified to a
 *            supplementary service as a result of call control.
 *
 *  \param  pAlphaIDInfo(optional)
 *          - See \ref alphaIDInfo for more information.
 *
 *  \param  pCallBarPasswd(optional)
 *          - Password is required if call barring is provisioned using a
 *            password.
 *              - Password consists of 4 ASCII digits.
 *              - Range: 0000 to 9999.
 *          - This also serves as the old password in the register password
 *            scenario.
 *
 *  \param  pNewPwdData(optional)
 *          - See \ref newPwdData for more information.
 *
 *  \param  pDataSrc(optional)
 *          - Sups Data Source.
 *          - Used to distinguish between the supplementary service data sent
 *            to the network and the response received from the network.
 *          - If absent, the supplementary service data in this indication can
 *            be assumed as a request sent to the network.
 *
 *  \param  pFailCause(optional)
 *          - Supplementary services failure cause.
 *          - See \ref qaGobiApiTableVoiceCallEndReasons.h for more information.
 *
 *  \param  pCallFwdInfo(optional)
 *          - See \ref getCallFWInfo for more information.
 *
 *  \param  pCLIRstatus(optional)
 *          - See \ref CLIRResp for more information.
 *
 *  \param  pCLIPstatus(optional)
 *          - See \ref CLIPResp for more information.
 *
 *  \param  pCOLPstatus(optional)
 *          - See \ref COLPResp for more information.
 *
 *  \param  pCOLRstatus(optional)
 *          - See \ref COLRResp for more information.
 *
 *  \param  pCNAPstatus(optional)
 *          - See \ref CNAPResp for more information.
 *
 * \note    None
 *
 */
typedef struct
{
    SUPSInfo       SUPSInformation;
    BYTE           *pSvcClass;
    BYTE           *pReason;
    BYTE           *pCallFWNum;
    BYTE           *pCallFWTimerVal;
    struct USSInfo *pUSSInfo;
    BYTE           *pCallID;
    alphaIDInfo    *pAlphaIDInfo;
    BYTE           *pCallBarPasswd;
    newPwdData     *pNewPwdData;
    BYTE           *pDataSrc;
    WORD           *pFailCause;
    getCallFWInfo  *pCallFwdInfo;
    CLIRResp       *pCLIRstatus;
    CLIPResp       *pCLIPstatus;
    COLPResp       *pCOLPstatus;
    COLRResp       *pCOLRstatus;
    CNAPResp       *pCNAPstatus;
} voiceSUPSInfo;

/**
 *  Preferred SUPS indication callback.
 *
 *  \param  pVoiceSUPSInfo
 *          - See \ref voiceSUPSInfo for more information.
 *
 *  \note   Technology Supported: GSM\n
 *          Device Supported: SL9090\n
 */
typedef void ( * tFNSUPSInfo)
    ( voiceSUPSInfo *pVoiceSUPSInfo );

/**
 *  Enables/disables the SUPS callback function.
 *  The most recent successfully subscribed callback function will be the only
 *  function that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0-Disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: GSM\n
 *          Device Supported: SL9090\n
 *          Timeout: 2 seconds\n
 *          This callback notifies clients about the modem-originated
 *          supplementary service requests and the responses received from the
 *          network.
 *
 */
ULONG SLQSVoiceSetSUPSCallBack( tFNSUPSInfo pCallback );

/**
 *  Structure for storing the nasSysInfo indication parameters.
 *
 *  \param  pCDMASrvStatusInfo
 *          - See \ref SrvStatusInfo for more information.
 *
 *  \param  pHDRSrvStatusInfo
 *          - See \ref SrvStatusInfo for more information.
 *
 *  \param  pGSMSrvStatusInfo
 *          - See \ref GSMSrvStatusInfo for more information.
 *
 *  \param  pWCDMASrvStatusInfo
 *          - See \ref GSMSrvStatusInfo for more information.
 *
 *  \param  pLTESrvStatusInfo
 *          - See \ref GSMSrvStatusInfo for more information.
 *
 *  \param  pCDMASysInfo
 *          - See \ref CDMASysInfo for more information.
 *
 *  \param  pHDRSysInfo
 *          - See \ref HDRSysInfo for more information.
 *
 *  \param  pGSMSysInfo
 *          - See \ref GSMSysInfo for more information.
 *
 *  \param  pWCDMASysInfo
 *          - See \ref WCDMASysInfo for more information.
 *
 *  \param  pLTESysInfo
 *          - See \ref LTESysInfo for more information.
 *
 *  \param  pAddCDMASysInfo
 *          - See \ref AddCDMASysInfo for more information.
 *
 *  \param  pAddHDRSysInfo
 *          - System table index referencing the beginning of the geo in which
 *            the current serving system is present.
 *          - When the system index is not known, 0xFFFF is used.
 *
 *  \param  pAddGSMSysInfo
 *          - See \ref AddSysInfo for more information.
 *
 *  \param  pAddWCDMASysInfo
 *          - See \ref AddSysInfo for more information.
 *
 *  \param  pAddLTESysInfo
 *          - System table index referencing the beginning of the geo in which
 *            the current serving system is present.
 *          - When the system index is not known, 0xFFFF is used.
 *
 *  \param  pGSMCallBarringSysInfo
 *          - See \ref CallBarringSysInfo for more information.
 *
 *  \param  pWCDMACallBarringSysInfo
 *          - See \ref CallBarringSysInfo for more information.
 *
 *  \param  pLTEVoiceSupportSysInfo
 *          - Indicates voice support status on LTE.
 *              - 0x00 - Voice is not supported
 *              - 0x01 - Voice is supported
 *
 *  \param  pGSMCipherDomainSysInfo
 *          - Ciphering on the service domain.
 *              - 0x00 - No service
 *              - 0x01 - Circuit-switched only
 *              - 0x02 - Packet-switched only
 *              - 0x03 - Circuit-switched and packet-switched
 *
 *  \param  pWCDMACipherDomainSysInfo
 *          - Ciphering on the service domain.
 *              - 0x00 - No service
 *              - 0x01 - Circuit-switched only
 *              - 0x02 - Packet-switched only
 *              - 0x03 - Circuit-switched and packet-switched
 *
 *  \param  pSysInfoNoChange
 *          - System Info No Change.
 *          - Flag used to notify clients that a request to select a network
 *            ended with no change in the PLMN.
 *              - 0x01 - No change in system information
 *
 */
typedef struct
{
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
    BYTE               *pSysInfoNoChange;
} nasSysInfo;

/**
 *  System Information indication callback.
 *
 *  \param  pNasSysInfo
 *          - See \ref nasSysInfo for more information.
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC77XX\n
 */
typedef void ( *tFNSysInfo )
    ( nasSysInfo *pNasSysInfo ) ;

/**
 *  Enables/disables the Sys Info callback function.
 *  The most recent successfully subscribed callback function will be the only
 *  function that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0-Disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC77XX\n
 *          Timeout: 2 seconds\n
 *          This callback provides current serving system information,
 *          including registration information and system property. The
 *          serving system information of the radio interfaces specified in
 *          mode_pref are included in the response message. When any value
 *          in the sys_info message changes, an indication message is sent.
 *          Indications contain all the values for all active RATs.
 *
 */
ULONG SLQSNasSysInfoCallBack( tFNSysInfo pCallback );

/**
 * This structure contains the parameters for Universal Time Information.
 *
 *  \param  year
 *          - Year.
 *
 *  \param  month
 *          - Month.
 *              - 1 is January and 12 is December.
 *
 *  \param  day
 *          - Day.
 *              - Range 1 to 31.
 *
 *  \param  hour
 *          - Hour.
 *              - Range 0 to 59.
 *
 *  \param  minute
 *          - Minute.
 *              - Range 0 to 59.
 *
 *  \param  second
 *          - Second.
 *              - Range 0 to 59.
 *
 *  \param  dayOfWeek
 *          - Day of the Week.
 *              - 0 is Monday and 6 is Sunday.
 *
 */
typedef struct
{
    WORD year;
    BYTE month;
    BYTE day;
    BYTE hour;
    BYTE minute;
    BYTE second;
    BYTE dayOfWeek;
} UniversalTime;

/**
 *  Structure for storing the nasSysInfo indication parameters.
 *
 *  \param  universalTime
 *          - See \ref UniversalTime for more information.
 *
 *  \param  pTimeZone
 *          - Time Zone.
 *          - Offset from Universal time, i.e., the difference between local
 *            time and Universal time, in increments of 15 min (signed value).
 *
 *  \param  pDayltSavAdj
 *          - Daylight Saving Adjustment.
 *          - Daylight saving adjustment in hr.
 *              - Possible values: 0, 1, and 2.
 *
 */
typedef struct
{
    UniversalTime universalTime;
    BYTE          *pTimeZone;
    BYTE          *pDayltSavAdj;
} nasNetworkTime;

/**
 *  Network Time indication callback.
 *
 *  \param  pNasNetworkTime
 *          - See \ref nasNetworkTime for more information.
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC77XX\n
 */
typedef void ( *tFNNetworkTime )
    ( nasNetworkTime *pNasNetworkTime );

/**
 *  Enables/disables the Network Time callback function.
 *  The most recent successfully subscribed callback function will be the only
 *  function that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0-Disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC77XX\n
 *          Timeout: 2 seconds\n
 *          This callback is sent when the 3GPP or 3GPP2 network sends time
 *          information to the User Equipment.
 *
 */
ULONG SLQSNasNetworkTimeCallBack( tFNNetworkTime pCallback );

/**
 * \internal
 * eQMI_SWIOMA_DM_EVENT_IND TLVs defined below
 */

/**
 * This structure will hold the SwiOmaDmFota session parameters information.
 *
 *  \param  state
 *          - 0x01 - No Firmware available
 *          - 0x02 - Query Firmware Download
 *          - 0x03 - Firmware Downloading
 *          - 0x04 - Firmware downloaded
 *          - 0x05 - Query Firmware Update
 *          - 0x06 - Firmware updating
 *          - 0x07 - Firmware updated
 *
 *  \param  user_input_req - Bit mask of available user inputs
 *          - 0x00 - No user input required.Informational indication
 *          - 0x01 - Accept
 *          - 0x02 - Reject
 *
 *  \param  user_input_timeout
 *          - Timeout for user input in minutes.
 *             A value of 0 means no time-out
 *
 *  \param  fw_dload_size
 *          - The size (in bytes) of the firmware update package
 *
 *  \param  fw_dload_complete
 *          - The number of bytes downloaded. Need to determine how
 *            often to send this message for progress bar notification.
 *            Every 500ms or 5% increment.
 *
 *  \param  update_complete_status
 *          - See table below.
 *
 *  \param  severity
 *          - 0x01 - Mandatory
 *          - 0x02 - Optional
 *
 *  \param  versionlength
 *          - Length of FW Version string in bytes
 *
 *  \param  version
 *          - FW Version string in ASCII (Max 256 characters)
 *
 *  \param  namelength
 *          - Length Package Name string in bytes
 *
 *  \param  package_name
 *          - Package Name in UCS2 (Max 256 characters)
 *
 *  \param  descriptionlength
 *          - Length of description in bytes
 *
 *  \param  description
 *          - Description of Update Package in USC2 (Max 256 characters)
 */
struct omaDmFotaTlv
{
    BYTE   state;
    BYTE   userInputReq;
    USHORT userInputTimeout;
    ULONG  fwdloadsize;
    ULONG  fwloadComplete;
    USHORT updateCompleteStatus;
    BYTE   severity;
    USHORT versionlength;
    BYTE   version[256];
    USHORT namelength;
    BYTE   package_name[256];
    USHORT descriptionlength;
    BYTE   description[256];
};

/**
 * This structure will hold the SwiOmaDmConfig session parameters information.
 *
 *  \param  state
 *          - 0x01 - OMA-DM Read Request
 *          - 0x02 - OMA-DM Change Request
 *          - 0x03 - OMA-DM Config Complete
 *
 *  \param  user_input_req - Bit mask of available user inputs
 *          - 0x00 - No user input required.Informational indication
 *          - 0x01 - Accept
 *          - 0x02 - Reject
 *
 *  \param  user_input_timeout
 *          - Timeout for user input in minutes.
 *            A value of 0 means no time-out
 *
 *  \param  alertmsglength
 *          - Length of Alert message string in bytes
 *
 *  \param  alertmsg
 *          - Alert message in UCS2  (Max 256 characters)
 */
struct omaDmConfigTlv
{
    BYTE   state;
    BYTE   userInputReq;
    USHORT userInputTimeout;
    USHORT alertmsglength;
    BYTE   alertmsg[256];
};

/**
 * This union sessionInfo consist of omaDmFotaTlv and omaDmConfigTlv,
 * out of which one will be unpacked against psessionTypeFields.
 *
 */
typedef union sessionInfo
{
    struct omaDmFotaTlv   omaDmFota;
    struct omaDmConfigTlv omaDmConfig;
} sessionInformation;

/**
 *  This structure holds information related to memory
 *
 *  \param  storageType
 *          - Indicates the type of memory storage
 *              0x00 - STORAGE_TYPE_UIM
 *              0x01 - STORAGE_TYPE_NV
 *  \param  messageMode
 *          - Indicates the type of memory mode
 *              0x00 - MESSAGE_MODE_CDMA - CDMA
 *              0x01 - MESSAGE_MODE_GW - GW
 */
typedef struct
{
    BYTE storageType;
    BYTE messageMode;
} SMSMemoryInfo;

/**
 *  SMS Memory related callback function.
 *
 *  \param  pSMSMemoryFullInfo[OUT]
 *          - pointer to SMSMemoryInfo.
 *          - see \ref SMSMemoryInfo for details.
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750,MC83xx,MC7700/10\n
 *
 */
typedef void (* tFNMemoryFull)( SMSMemoryInfo *pSMSMemoryFullInfo );

/**
 *  Enables/disables the event related to memory full status callback function.
 *  The most recent successfully subscribed callback function will be the only
 *  function that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0-Disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750,MC83xx,MC7700/10\n
 *
 */
ULONG SLQSWmsMemoryFullCallBack( tFNMemoryFull pCallback );

/**
 * This structure consist of OTASP or OTAPA event params
 *
 *  \param  callID
 *          - Call identifier for the call.
 *
 *  \param  OTASPStatus
 *          - OTASP status for the OTASP call.
 *            Values:
 *            - 0x00 - OTASP_STATUS_SPL_UNLOCKED.SPL unlocked; only for
 *                     user-initiated OTASP
 *            - 0x01 - OTASP_STATUS_SPRC_RETRIES_EXCEEDED. SPC retries exceeded;
 *                     only for user-initiated OTASP
 *            - 0x02 - OTASP_STATUS_AKEY_EXCHANGED.A-key exchanged;
 *                     only for user-initiated OTASP
 *            - 0x03 - OTASP_STATUS_SSD_UPDATED. SSD updated; for both\n
 *                     user-initiated OTASP and network-initiated OTASP (OTAPA)
 *            - 0x04 - OTASP_STATUS_NAM_DOWNLOADED - NAM downloaded;
 *                     only for user-initiated OTASP
 *            - 0x05 - OTASP_STATUS_MDN_DOWNLOADED - MDN downloaded;
 *                     only for user-initiated OTASP
 *            - 0x06 - OTASP_STATUS_IMSI_DOWNLOADED - IMSI downloaded;
 *                     only for user-initiated OTASP
 *            - 0x07 - OTASP_STATUS_PRL_DOWNLOADED - PRL downloaded;
 *                     only for user-initiated OTASP
 *            - 0x08 - OTASP_STATUS_COMMITTED - Commit successful;
 *                     only for user-initiated OTASP
 *            - 0x09 - OTASP_STATUS_OTAPA_STARTED - OTAPA started;
 *                     only for network-initiated OTASP(OTAPA)
 *            - 0x0A - OTASP_STATUS_OTAPA_STOPPED - OTAPA stopped;
 *                     only for network-initiated OTASP(OTAPA)
 *            - 0x0B - OTASP_STATUS_OTAPA_ABORTED - OTAPA aborted;
 *                     only for network-initiated OTASP(OTAPA)
 *            - 0x0C - OTASP_STATUS_OTAPA_COMMITTED - OTAPA committed;
 *                     only for network-initiated OTASP(OTAPA)
 */
typedef struct
{
    BYTE callID;
    BYTE OTASPStatus;
}voiceOTASPStatusInfo;

/**
 * OTASP or OTAPA event Indication Callback function
 *
 *  \param  pVoiceOTASPStatusInfo
 *          - OTASP Status Information.
 *          - See \ref voiceOTASPStatusInfo for more information
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: SL9090\n
 */
typedef void ( * tFNOTASPStatus )
    ( voiceOTASPStatusInfo *pVoiceOTASPStatusInfo );

/**
 *  Enables/disables OTASP(Over-The-Air Service Provisioning) or
 *  OTAPA(Over-The-Air Parameter Administration) event CallBack Funtion
 *  (applicable only for 3GPP2).
 *  The most recent successfully subscribed callback function will be the
 *  only function that is invoked when the corresponding event occurs.
 *
 *  \param  pCallback[IN]
 *          - a valid function pointer to enable OTASP or OTAPA event
 *            Indication notification
 *          - NULL to disable OTASP or OTAPA event, Indication notification
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported:  SL9090\n
 *          Timeout: 10 seconds\n
 *          This indication communicates the occurrence of an OTASP or OTAPA
 *          event.This indication is only applicable for 3GPP2 devices.
 */
ULONG SLQSVoiceSetOTASPStatusCallBack(
    tFNOTASPStatus pCallback );

/**
 *  This structure contains Signal Information
 *
 *  \param  signalType
 *          - Call identifier for the call.
 *
 *  \param  alertPitch
 *          - Signal Information
 *
 *  \param  signal
 *          - Caller ID Information
 *
 */
typedef struct
{
    BYTE signalType;
    BYTE alertPitch;
    BYTE signal;
} signalInfo;

/**
 *  This structure contains Caller ID Information
 *
 *  \param  PI
 *          - Presentation indicator; refer to [S1, Table 2.7.4.4-1]
 *            for valid values.
 *
 *  \param  callerIDLen
 *          - Number of sets of following elements
 *            - Caller Id
 *
 *  \param  pCallerID
 *          - Caller ID in ASCII string.
 *
 */
typedef struct
{
    BYTE PI;
    BYTE callerIDLen;
    BYTE callerID[255];
} callerIDInfo;

/**
 *  This structure contains Calling party Number Information
 *
 *  \param  PI
 *          - Presentation indicator; refer to [S1, Table 2.7.4.4-1]
 *            for valid values.
 *
 *  \param  SI
 *          - Number of sets of following elements
 *            - Caller Id
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
    BYTE PI;
    BYTE SI;
    BYTE numType;
    BYTE numPlan;
    BYTE numLen;
    BYTE number[255];
} callingPartyInfo;

/**
 *  This structure contains Called party Number Information
 *
 *  \param  PI
 *          - Presentation indicator; refer to [S1, Table 2.7.4.4-1]
 *            for valid values.
 *
 *  \param  SI
 *          - Number of sets of following elements
 *            - Caller Id
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
    BYTE PI;
    BYTE SI;
    BYTE numType;
    BYTE numPlan;
    BYTE numLen;
    BYTE number[255];
} calledPartyInfo;

/**
 *  This structure contains Redirecting Number Information
 *
 *  \param  PI
 *          - Presentation indicator; refer to [S1, Table 2.7.4.4-1]
 *            for valid values.
 *
 *  \param  SI
 *          - Number of sets of following elements
 *            - Caller Id
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
 *  \param  reason
 *          -Redirecting reason; refer to [S1, Table 3.7.5.11-1] for\n
 *           valid values
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
    BYTE PI;
    BYTE SI;
    BYTE numType;
    BYTE numPlan;
    BYTE reason;
    BYTE numLen;
    BYTE number[255];
} redirNumInfo;

/**
 *  This structure contains National Supplementary Services - Audio Control
 *  Information
 *
 *  \param  upLink
 *          - Values as per[ S24, 4.10 Reservation response].
 *
 *  \param  downLink
 *          - Values as per[ S24, 4.10 Reservation response].
 */
typedef struct
{
    BYTE upLink;
    BYTE downLink;
} NSSAudioCtrl;

/**
 *  This structure contains Line Control Information
 *
 *  \param  polarityIncluded
 *          - Included Polarity; Boolean Value
 *
 *  \param  toggleMode
 *          - Toggle mode; Boolean Value
 *
 *  \param  revPolarity
 *          - Reverse Polarity; Boolean Value
 *
 *  \param  pwrDenialTime
 *          - Power denial time; refer to [S1, Section 3.7.5.15 Line Control]
 *            for valid values
 */
typedef struct
{
    BYTE polarityIncluded;
    BYTE toggleMode;
    BYTE revPolarity;
    BYTE pwrDenialTime;
} lineCtrlInfo;

/**
 *  This structure contains Line Control Information
 *
 *  \param  dispType
 *          - Values are per [S1, Table 3.7.5.16-1].
 *
 *  \param  extDispInfoLen
 *          - Number of sets of the following elements:
 *            - ext_display_info
 *
 *  \param  extDispInfo
 *          - Extended display information buffer containing the display
 *            record; refer to [S1, Section 3.7.5.16] for the format
 *            information of the buffer contents.
 */
typedef struct
{
    BYTE dispType;
    BYTE extDispInfoLen;
    BYTE extDispInfo[255];
} extDispRecInfo;

/**
 *  This structure contains Voice record Information
 *
 *  \param  callID [Mandatory]
 *          - Call identifier for the call.
 *
 *  \param  pSignalInfo[Optional]
 *          - Signal Information
 *          - See \ref signalInfo for more information
 *
 *  \param  pCallerIDInfo[Optional]
 *          - Caller ID Information
 *          - See \ref callerIDInfo for more information
 *
 *  \param  pDispInfo[Optional]
 *          - Display Information
 *
 *  \param  pExtDispInfo[Optional]
 *          - Extended Display Information
 *
 *
 *  \param  pCallerNameInfo[Optional]
 *          - Caller Name Information
 *
 *  \param  pCallWaitInd[Optional]
 *          - Call Waiting Indicator
 *
 *  \param  pConnectNumInfo[Optional]
 *          - Connected Number Information
 *          - see \ref connectNumInfo for more information
 *
 *  \param  pCallingPartyInfo[Optional]
 *          - Calling Party Number Information
 *          - This structure is having exactly same elements as connectNumInfo
 *          - see \ref connectNumInfo for more information
 *
 *  \param  pCalledPartyInfo[Optional]
 *          - Called Party Number Information
 *          - see \ref calledPartyInfo for more information
 *
 * \param   pRedirNumInfo[Optional]
 *          - Redirecting Number Information
 *          - see \ref redirNumInfo for more information
 *
 *  \param  pCLIRCause[Optional]
 *          - National Supplementary Services - CLIR
 *          - see \ref NSSAudioCtrl for more information
 *
 *  \param  pNSSAudioCtrl[Optional]
 *          - National Supplementary Services - Audio Control
 *
 *  \param  pNSSRelease[Optional]
 *          - National Supplementary Services - Release
 *
 *  \param  pLineCtrlInfo[Optional]
 *          - Line Control Information
 *          - see \ref lineCtrlInfo for more information
 *
 *  \param  pExtDispRecInfo[Optional]
 *          - Extended Display Record Information
 *          - see \ref extDispRecInfo for more information
 *
 */
typedef struct
{
    BYTE            callID;
    signalInfo      *pSignalInfo;
    callerIDInfo    *pCallerIDInfo;
    BYTE            *pDispInfo;
    BYTE            *pExtDispInfo;
    BYTE            *pCallerNameInfo ;
    BYTE            *pCallWaitInd;
    connectNumInfo  *pConnectNumInfo;
    connectNumInfo  *pCallingPartyInfo;
    calledPartyInfo *pCalledPartyInfo;
    redirNumInfo    *pRedirNumInfo;
    BYTE            *pCLIRCause;
    NSSAudioCtrl    *pNSSAudioCtrl;
    BYTE            *pNSSRelease;
    lineCtrlInfo    *pLineCtrlInfo;
    extDispRecInfo  *pExtDispRecInfo;
} voiceInfoRec;

/**
 *  Voice Information Record callback.
 *
 *  \param  pVoiceInfoRec
 *          - See \ref voiceInfoRec for more information.
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC7750\n
 */
typedef void ( * tFNInfoRec) ( voiceInfoRec *pVoiceInfoRec );

/**
 *  Enables/disables the Voice information Record callback function.
 *  The most recent successfully subscribed callback function will be the only
 *  function that is invoked when the corresponding event occurs.
 *  (Applicable only for 3GPP2)
 *
 *  \param  pCallback[IN]
 *          - Callback function pointer (0-Disable)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC7750\n
 *          Timeout: 2 seconds
 */
ULONG SLQSVoiceInfoRecCallback( tFNInfoRec pCallback );


#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_CBK_H__ */
