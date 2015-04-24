/*************
 *
 * Filename:  qmudefs.h
 *
 * Purpose:   User definitions for qm package
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

#ifndef QMUDEFS_H
#define QMUDEFS_H

/*
    QMI REQUEST PACKET SENT OVER IPC CHANNEL AND RECEIVED BY QMRR TASK
    -------------------------------------------
    | QMUXSDU PREAMBLE  | QMI SERVICE MESSAGE |
    -------------------------------------------
bytes:      7               m

    QMUX SDU PREAMBLE
    -------------------------------------------------------------------
    | QMI Service Type | QMI Request Timeout | QMI Transaction Length |
    -------------------------------------------------------------------
bytes:      1               4                           2

    QMI SERVICE MESSAGE
    --------------------------------------
    | QMI SVC MSG HDR | QMI SVC MSG TLVs |
    --------------------------------------
bytes:      4               n

    QMI SVC MSG HDR
    ------------------------
    | MSG ID  | MSG LENGTH |
    ------------------------
bytes:   2          2

*/

/* Includes */
#include "aa/aaglobal.h"

/* QMI Error storage array size. This is used to size the array
 * on the API side for storing the last error received from the
 * modem when the QMI Error Flag is set in a response header.
 * The size is the same as is defined in the Windows SDK
 */
#define QMFWERRORSZ    200

/*******************************
 *  QMI DRIVER IOCTL COMMANDS  *
 ******************************/
/* Device I/O control code for setting QMI service */
#define QMI_GET_SERVICE_FILE_IOCTL  0x8BE0 + 1

/* Device I/O control code for obtaining device VIDPID */
#define QMI_GET_VIDPID_IOCTL        0x8BE0 + 2

/* Device I/O control code for obtaining device MEID */
#define QMI_GET_MEID_IOCTL          0x8BE0 + 3

/* Device path size - last octet reserved for null termination */
#define QMI_DEVICE_PATH_MAX_SIZE    256

/* Device MEID size - last octet reserved for null termination */
#define QMI_MEID_BUF_SIZE           15

/* USB port name size - last octet reserved for null termination */
#define QMI_USB_PORT_NAME_SIZE      32

/* Device I/O control code for releasing a QMI service client */
#define QMI_RELEASE_SERVICE_FILE_IOCTL  (0x8BE0 + 4)

/**********************
 *  QMUX SDU DEFINES  *
 **********************/
/* The preamble contains any data the SDK QM task requires to carry out the QMI
 * request that does not form the QMI (request) Message sent to the device.
 * Current preamble fields include:
 *  QMI service pertaining to th QMI request (swi_uint8)
 *  QMI request timeout (swi_uint32)
 *  QMI Transaction length (swi_uint16)
 */
#define QMUXSDU_PREAMBLE    7

/* QMI Parameter field (QMUX SDU) maximum size */
#define QMPARAMMAX          4 * 1024

/*** QMI Protocol Field definitions ***/

/* QMI Service Message definitions */
#define QMISVC_MSGID_SZ             2   /* QMI service message, message ID size */
#define QMISVC_MSGLENSZ             2   /* QMI service message, message length */
#define QMISVC_MSGHDR_SZ            (QMISVC_MSGID_SZ + QMISVC_MSGLENSZ)
#define QMISVC_MSGID_OFFSET         0
#define QMISVC_MSGLEN_OFFSET        (QMISVC_MSGID_OFFSET + QMISVC_MSGID_SZ)

/* QMI Service Transaction (QMUX SDU) definitions */
#define QMUXSDU_CTRLFLGS_SZ         1   /* QMI service transaction control flags size */
#define QMUXSDU_XACTIONID_SZ        2   /* QMI service transaction transaction ID size */
#define QMUXSDU_HDR_SZ              (QMUXSDU_CTRLFLGS_SZ +\
                                     QMUXSDU_XACTIONID_SZ)

#define QMUXSDU_CTRLFLGS_OFFSET     0
#define QMUXSDU_XACTIONID_OFFSET    (QMUXSDU_CTRLFLGS_OFFSET +\
                                     QMUXSDU_CTRLFLGS_SZ)
#define QMUXSDU_QMISVCMSG_OFFSET    (QMUXSDU_XACTIONID_OFFSET +\
                                     QMUXSDU_XACTIONID_SZ)

/* QMI TLV definitions */
#define QMITLV_TYPE_SZ               1   /* QMI TLV, type size */
#define QMITLV_LENGTH_SZ             2   /* QMI TLV,  TLV length */
#define QMITLV_HDR_SZ                (QMITLV_TYPE_SZ + QMITLV_LENGTH_SZ)

/* Invalid QMI transaction ID */
#define QMI_TRANSACTION_ID_INVALID   0
#define QMI_MSG_ID_INVALID          -1

/* supported QMI clients */
#define QMIWDSSVCNAM     "WDS"
#define QMIDMSSVCNAM     "DMS"
#define QMINASSVCNAM     "NAS"
#define QMIQOSSVCNAM     "QOS"
#define QMIWMSSVCNAM     "WMS"
#define QMIPDSSVCNAM     "PDS"
#define QMIAUTSVCNAM     "AUT"
#define QMIUIMSVCNAM     "UIM"
#define QMIRMSSVCNAM     "RMS"
#define QMIOMASVCNAM     "OMA"
#define QMISWIOMASVCNAM  "SWIOMA"
#define QMIPBMSVCNAM     "PBM"
#define QMICATSVCNAM     "CAT"
#define QMIDCSSVCNAM     "DCS"
#define QMIFMSSVCNAM     "FMS"
#define QMISARSVCNAM     "SAR"
#define QMIVOICESVCNAM   "VOICE"
#define QMIINVSVCNAM     "INV"  /* invalid service */

/* WDS service defines */
#define IPv4_FAMILY_PREFERENCE  0x04
#define IPv6_FAMILY_PREFERENCE  0x06

/*************
 *
 * Name:    qmisupportedclients
 *
 * Purpose: QMI service clients
 *
 * Members: QMI<QMI Service>CLNT
 *
 * Notes:   DCS/FMS clients are serviced by QMI services provided by the SDK.
 *          Hence, the corresponding requests are processed locally by the SDK,
 *          and are therefore not sent out to the device.
 *
 **************/
enum qmisupportedclients
{
    QMI_SUPPORTED_CLNT_MIN,
    QMIWDSCLNT = QMI_SUPPORTED_CLNT_MIN,
    QMIDMSCLNT,
    QMINASCLNT,
    QMIQOSCLNT,
    QMIWMSCLNT,
    QMIPDSCLNT,
    QMIAUTCLNT,
    QMIUIMCLNT,
    QMICATCLNT,
    QMIRMSCLNT,
    QMIOMACLNT,
    QMIPBMCLNT,
    QMIDCSCLNT, /* see Notes in header above */
    QMISWIOMACLNT,
    QMIFMSCLNT,
    QMISARCLNT,
    QMIVOICECLNT,
    QMIWDSV6CLNT,
    /* add new clients above this line */
    QMI_SUPPORTED_CLNT_MAX,
    QMIUNSUPPORTEDCLNT
};

/* QMI Service Enumeration */
enum eQMIService
{
   eQMI_SVC_ENUM_BEGIN = -1,
   eQMI_SVC_CONTROL,          // 000 Control service
   eQMI_SVC_RANGE0_START,
   eQMI_SVC_WDS =             // 001 Wireless data service
        eQMI_SVC_RANGE0_START,
   eQMI_SVC_DMS,              // 002 Device management service
   eQMI_SVC_NAS,              // 003 Network access service
   eQMI_SVC_QOS,              // 004 Quality of service, err, service
   eQMI_SVC_WMS,              // 005 Wireless messaging service
   eQMI_SVC_PDS,              // 006 Position determination service
   eQMI_SVC_AUTH,             // 007 Authentication service
   eQMI_SVC_RANGE0_STOP =
        eQMI_SVC_AUTH,
   eQMI_SVC_VOICE = 9,
   eQMI_SVC_UIM = 0x0B,
   eQMI_SVC_SAR = 17,

   eQMI_SVC_CAT = 224,        // 224 Card application toolkit service
   eQMI_SVC_RANGE1_START =
        eQMI_SVC_CAT,
   eQMI_SVC_RMS,              // 225 Remote management service
   eQMI_SVC_OMA,              // 226 Open mobile alliance dev mgmt service
   eQMI_SVC_SWIOMA = 240,     // 240 SWI Open mobile alliance dev mgmt service
   eQMI_SVC_RANGE1_STOP =
       eQMI_SVC_SWIOMA,
   eQMI_SVC_ENUM_END,
   /* Services below are internal to the SDK. Hence, the corresponding
    * QMI requests are processed locally by the SDK and therefore are not
    * sent to the device.
    */
   eQMI_SVC_DCS,
   eQMI_SVC_FMS
};

/* QMI Control Service Type Message ID Enumeration */
enum eQMIMessageCTL
{
   eQMI_CTL_ENUM_BEGIN = -1,
   eQMI_CTL_SET_INSTANCE_ID = 32,   // 32 Set the unique link instance ID
   eQMI_CTL_GET_VERSION_INFO,       // 33 Get supported service version info
   eQMI_CTL_GET_CLIENT_ID,          // 34 Get a unique client ID
   eQMI_CTL_RELEASE_CLIENT_ID,      // 35 Release the unique client ID
   eQMI_CTL_REVOKE_CLIENT_ID_IND,   // 36 Indication of client ID revocation
   eQMI_CTL_INVALID_CLIENT_ID,      // 37 Indication of invalid client ID
   eQMI_CTL_SET_DATA_FORMAT,        // 38 Set host driver data format
   eQMI_CTL_SYNC,                   // 39 Synchronize client/server
   eQMI_CTL_SYNC_IND = 39,          // 39 Synchronize indication
   eQMI_CTL_SET_EVENT,              // 40 Set event report conditions
   eQMI_CTL_EVENT_IND = 40,         // 40 Event report indication
   eQMI_CTL_SET_POWER_SAVE_CFG,     // 41 Set power save config
   eQMI_CTL_SET_POWER_SAVE_MODE,    // 42 Set power save mode
   eQMI_CTL_GET_POWER_SAVE_MODE,    // 43 Get power save mode
   eQMI_CTL_ENUM_END
};

/* QMI WDS Service Type Message ID Enumeration */
enum eQMIMessageWDS
{
   eQMI_WDS_ENUM_BEGIN = -1,

   eQMI_WDS_RESET,                // 00 Reset WDS service state variables
   eQMI_WDS_SET_EVENT,            // 01 Set connection state report conditions
   eQMI_WDS_EVENT_IND = 1,        // 01 Connection state report indication
   eQMI_WDS_ABORT,                // 02 Abort previously issued WDS command

   eQMI_WDS_START_NET = 32,       // 32 Start WDS network interface
   eQMI_WDS_STOP_NET,             // 33 Stop WDS network interface
   eQMI_WDS_GET_PKT_STATUS,       // 34 Get packet data connection status
   eQMI_WDS_PKT_STATUS_IND = 34,  // 34 Packet data connection status indication
   eQMI_WDS_GET_RATES,            // 35 Get current bit rates of the connection
   eQMI_WDS_GET_STATISTICS,       // 36 Get the packet data transfer statistics
   eQMI_WDS_G0_DORMANT,           // 37 Go dormant
   eQMI_WDS_G0_ACTIVE,            // 38 Go active
   eQMI_WDS_CREATE_PROFILE,       // 39 Create profile with specified settings
   eQMI_WDS_MODIFY_PROFILE,       // 40 Modify profile with specified settings
   eQMI_WDS_DELETE_PROFILE,       // 41 Delete the specified profile
   eQMI_WDS_GET_PROFILE_LIST,     // 42 Get all profiles
   eQMI_WDS_GET_PROFILE,          // 43 Get the specified profile
   eQMI_WDS_GET_DEFAULTS,         // 44 Get the default data session settings
   eQMI_WDS_GET_SETTINGS,         // 45 Get the runtime data session settings
   eQMI_WDS_SET_MIP,              // 46 Get the mobile IP setting
   eQMI_WDS_GET_MIP,              // 47 Set the mobile IP setting
   eQMI_WDS_GET_DORMANCY,         // 48 Get the dormancy status

   eQMI_WDS_GET_AUTOCONNECT = 52, // 52 Get the NDIS autoconnect setting
   eQMI_WDS_GET_DURATION,         // 53 Get the duration of data session
   eQMI_WDS_GET_MODEM_STATUS,     // 54 Get the modem status
   eQMI_WDS_MODEM_IND = 54,       // 54 Modem status indication
   eQMI_WDS_GET_DATA_BEARER,      // 55 Get the data bearer type
   eQMI_WDS_GET_MODEM_INFO,       // 56 Get the modem info
   eQMI_WDS_MODEM_INFO_IND = 56,  // 56 Modem info indication

   eQMI_WDS_GET_ACTIVE_MIP = 60,  // 60 Get the active mobile IP profile
   eQMI_WDS_SET_ACTIVE_MIP,       // 61 Set the active mobile IP profile
   eQMI_WDS_GET_MIP_PROFILE,      // 62 Get mobile IP profile settings
   eQMI_WDS_SET_MIP_PROFILE,      // 63 Set mobile IP profile settings
   eQMI_WDS_GET_MIP_PARAMS,       // 64 Get mobile IP parameters
   eQMI_WDS_SET_MIP_PARAMS,       // 65 Set mobile IP parameters
   eQMI_WDS_GET_LAST_MIP_STATUS,  // 66 Get last mobile IP status
   eQMI_WDS_GET_AAA_AUTH_STATUS,  // 67 Get AN-AAA authentication status
   eQMI_WDS_GET_CUR_DATA_BEARER,  // 68 Get current data bearer
   eQMI_WDS_GET_CALL_LIST,        // 69 Get the call history list
   eQMI_WDS_GET_CALL_ENTRY,       // 70 Get an entry from the call history list
   eQMI_WDS_CLEAR_CALL_LIST,      // 71 Clear the call history list
   eQMI_WDS_GET_CALL_LIST_MAX,    // 72 Get maximum size of call history list

   eQMI_WDS_SET_IP_FAMILY = 77,   // 77 Set the client IP family preference

   eQMI_WDS_SET_AUTOCONNECT = 81, // 81 Set the NDIS autoconnect setting
   eQMI_WDS_GET_DNS,              // 82 Get the DNS setting
   eQMI_WDS_SET_DNS,              // 83 Set the DNS setting

   eQMI_WDS_SWI_CREATE_PROFILE = 0x5558,    // Create profile 3GPP2 Profile
   eQMI_WDS_SWI_GET_3GPP_CFG_ITEM = 0x5559, // Get 3GPP Config Item
   eQMI_WDS_SWI_SET_3GPP_CFG_ITEM = 0x555A, // Set 3GPP Config Item
   eQMI_WDS_SWI_PDP_RUNTIME_SETTINGS = 0x555B, // Get PDP Context
   eQMI_WDS_ENUM_END
};

/* QMI DMS Service Type Message ID Enumeration */
enum eQMIMessageDMS
{
   eQMI_DMS_ENUM_BEGIN = -1,

   eQMI_DMS_RESET,               // 00 Reset DMS service state variables
   eQMI_DMS_SET_EVENT,           // 01 Set connection state report conditions
   eQMI_DMS_EVENT_IND = 1,       // 01 Connection state report indication

   eQMI_DMS_GET_CAPS = 32,       // 32 Get the device capabilities
   eQMI_DMS_GET_MANUFACTURER,    // 33 Get the device manfacturer
   eQMI_DMS_GET_MODEL_ID,        // 34 Get the device model ID
   eQMI_DMS_GET_REV_ID,          // 35 Get the device revision ID
   /* Change eQMI_DMS_GET_NUMBER to eQMI_DMS_GET_MSISDN */
   //eQMI_DMS_GET_NUMBER,          // 36 Get the assigned voice number
   eQMI_DMS_GET_MSISDN,          // 36 Get the assigned voice number
   eQMI_DMS_GET_IDS,             // 37 Get the ESN/IMEI/MEID
   eQMI_DMS_GET_POWER_STATE,     // 38 Get the get power state
   eQMI_DMS_UIM_SET_PIN_PROT,    // 39 UIM - Set PIN protection
   eQMI_DMS_UIM_PIN_VERIFY,      // 40 UIM - Verify PIN
   eQMI_DMS_UIM_PIN_UNBLOCK,     // 41 UIM - Unblock PIN
   eQMI_DMS_UIM_PIN_CHANGE,      // 42 UIM - Change PIN
   eQMI_DMS_UIM_GET_PIN_STATUS,  // 43 UIM - Get PIN status
   /* Change eQMI_DMS_MSM_ID to eQMI_DMS_GET_HARDWARE_REV */
   //eQMI_DMS_GET_MSM_ID = 44,     // 44 Get MSM ID
   eQMI_DMS_GET_HARDWARE_REV = 44, // 44 Get MSM ID
   eQMI_DMS_GET_OPERATING_MODE,  // 45 Get the operating mode
   eQMI_DMS_SET_OPERATING_MODE,  // 46 Set the operating mode
   eQMI_DMS_GET_TIME,            // 47 Get timestamp from the device
   eQMI_DMS_GET_PRL_VERSION,     // 48 Get the PRL version
   eQMI_DMS_GET_ACTIVATED_STATE, // 49 Get the activation state
   eQMI_DMS_ACTIVATE_AUTOMATIC,  // 50 Perform an automatic activation
   eQMI_DMS_ACTIVATE_MANUAL,     // 51 Perform a manual activation
   eQMI_DMS_GET_USER_LOCK_STATE, // 52 Get the lock state
   eQMI_DMS_SET_USER_LOCK_STATE, // 53 Set the lock state
   eQMI_DMS_SET_USER_LOCK_CODE,  // 54 Set the lock PIN
   eQMI_DMS_READ_USER_DATA,      // 55 Read user data
   eQMI_DMS_WRITE_USER_DATA,     // 56 Write user data
   eQMI_DMS_READ_ERI_FILE,       // 57 Read the enhanced roaming indicator file
   eQMI_DMS_FACTORY_DEFAULTS,    // 58 Reset to factory defaults
   eQMI_DMS_VALIDATE_SPC,        // 59 Validate service programming code
   eQMI_DMS_UIM_GET_ICCID,       // 60 Get UIM ICCID
   eQMI_DMS_GET_FIRWARE_ID,      // 61 Get firmware ID
   eQMI_DMS_SET_FIRMWARE_ID,     // 62 Set firmware ID
   eQMI_DMS_GET_HOST_LOCK_ID,    // 63 Get host lock ID
   eQMI_DMS_UIM_GET_CK_STATUS,   // 64 UIM - Get control key status
   eQMI_DMS_UIM_SET_CK_PROT,     // 65 UIM - Set control key protection
   eQMI_DMS_UIM_UNBLOCK_CK,      // 66 UIM - Unblock facility control key
   eQMI_DMS_GET_IMSI,            // 67 Get the IMSI
   eQMI_DMS_UIM_GET_STATE,       // 68 UIM - Get the UIM state
   eQMI_DMS_GET_BAND_CAPS,       // 69 Get the device band capabilities
   eQMI_DMS_GET_FACTORY_ID,      // 70 Get the device factory ID
   eQMI_DMS_GET_FIRMWARE_PREF,   // 71 Get firmware preference
   eQMI_DMS_SET_FIRMWARE_PREF,   // 72 Set firmware preference
   eQMI_DMS_LIST_FIRMWARE,       // 73 List all stored firmware
   eQMI_DMS_DELETE_FIRMWARE,     // 74 Delete specified stored firmware
   eQMI_DMS_SET_TIME,            // 75 Set device time
   eQMI_DMS_GET_FIRMWARE_INFO,   // 76 Get stored firmware info
   eQMI_DMS_GET_ALT_NET_CFG,     // 77 Get alternate network config
   eQMI_DMS_SET_ALT_NET_CFG,     // 78 Set alternate network config
   eQMI_DMS_GET_IMG_DLOAD_MODE,  // 79 Get next image download mode
   eQMI_DMS_SET_IMG_DLOAD_MODE,  // 80 Set next image download mode
   eQMI_DMS_GET_CURRENT_PRL_INFO = 83,// 83 Get Current PRL Info
   eQMI_DMS_GET_CWE_SPKGS_INFO = 0x5556, // CWE Sierra Package String info
   eQMI_DMS_SWI_GET_CUST_FEATURES = 0x6557, // Query custom feature settings
   eQMI_DMS_SWI_SET_CUST_FEATURES = 0x6558, // Change custom feature settings
   eQMI_DMS_ENUM_END
};

/* QMI NAS Service Type Message ID Enumeration */
enum eQMIMessageNAS
{
   eQMI_NAS_ENUM_BEGIN = -1,

   eQMI_NAS_RESET,               // 00 Reset NAS service state variables
   eQMI_NAS_ABORT,               // 01 Abort previously issued NAS command
   eQMI_NAS_SET_EVENT,           // 02 Set NAS state report conditions
   eQMI_NAS_EVENT_IND = 2,       // 02 Connection state report indication
   eQMI_NAS_SET_REG_EVENT,       // 03 Set NAS registration report conditions

   eQMI_NAS_GET_RSSI = 32,       // 32 Get the signal strength
   eQMI_NAS_SCAN_NETS,           // 33 Scan for visible network
   eQMI_NAS_REGISTER_NET,        // 34 Initiate a network registration
   eQMI_NAS_ATTACH_DETACH,       // 35 Initiate an attach or detach action
   eQMI_NAS_GET_SS_INFO,         // 36 Get info about current serving system
   eQMI_NAS_SS_INFO_IND = 36,    // 36 Current serving system info indication
   eQMI_NAS_GET_HOME_INFO,       // 37 Get info about home network
   eQMI_NAS_GET_NET_PREF_LIST,   // 38 Get the list of preferred networks
   eQMI_NAS_SET_NET_PREF_LIST,   // 39 Set the list of preferred networks
   eQMI_NAS_GET_NET_BAN_LIST,    // 40 Get the list of forbidden networks
   eQMI_NAS_SET_NET_BAN_LIST,    // 41 Set the list of forbidden networks
   eQMI_NAS_SET_TECH_PREF,       // 42 Set the technology preference
   eQMI_NAS_GET_TECH_PREF,       // 43 Get the technology preference
   eQMI_NAS_GET_ACCOLC,          // 44 Get the Access Overload Class
   eQMI_NAS_SET_ACCOLC,          // 45 Set the Access Overload Class
   eQMI_NAS_GET_SYSPREF,         // 46 Get the CDMA system preference
   eQMI_NAS_GET_NET_PARAMS,      // 47 Get various network parameters
   eQMI_NAS_SET_NET_PARAMS,      // 48 Set various network parameters
   eQMI_NAS_GET_RF_INFO,         // 49 Get the SS radio/band channel info
   eQMI_NAS_GET_AAA_AUTH_STATUS, // 50 Get AN-AAA authentication status
   eQMI_NAS_SET_SYS_SELECT_PREF, // 51 Set system selection preference
   eQMI_NAS_GET_SYS_SELECT_PREF, // 52 Get system selection preference
   eQMI_NAS_SYS_SELECT_IND = 52, // 52 System selection pref indication

   eQMI_NAS_SET_DDTM_PREF = 55,  // 55 Set DDTM preference
   eQMI_NAS_GET_DDTM_PREF,       // 56 Get DDTM preference
   eQMI_NAS_DDTM_IND = 56,       // 56 DDTM preference indication
   eQMI_NAS_GET_OPERATOR_NAME = 57, // 57 Get Operator Name Data
   eQMI_NAS_GET_PLMN_MODE = 59,  // 59 Get PLMN mode bit from CSP
   eQMI_NAS_PLMN_MODE_IND,       // 60 CSP PLMN mode bit indication
   eQMI_NAS_GET_3GPP2_SUBSCRIPTION = 62, // Get 3GPP2 subscription info
   eQMI_NAS_GET_CELL_LOCATION_INFO = 67, // 67 Get Cell Location Info
   eQMI_NAS_GET_PLMN_NAME      = 68, // 68 Get operator name for specified network
   eQMI_NAS_NETWORK_TIME_IND   = 76, // 76 Network Time Indication.
   eQMI_NAS_GET_SYS_INFO       = 77, // 77 Get System Information.
   eQMI_NAS_SYS_INFO_IND       = 78, // 78 System Information Indication.
   eQMI_NAS_GET_SIG_INFO       = 79, // 79 Get information regarding signal strength
   eQMI_NAS_GET_HDR_COLOR_CODE = 87, // 87 Get HDR Color Code
   eQMI_NAS_GET_TX_RX_INFO     = 90, // 90 Get detailed Tx/Rx information
   eQMI_NAS_SWI_MODEM_STATUS   = 0x5556, // Get current status of Modem

   eQMI_NAS_ENUM_END
};

/* QMI WMS Service Type Message ID Enumeration */
enum eQMIMessageWMS
{
   eQMI_WMS_ENUM_BEGIN = -1,

   eQMI_WMS_RESET,                  // 00 Reset WMS service state variables
   eQMI_WMS_SET_EVENT,              // 01 Set new message report conditions
   eQMI_WMS_EVENT_IND = 1,          // 01 New message report indication

   eQMI_WMS_RAW_SEND = 32,          // 32 Send a raw message
   eQMI_WMS_RAW_WRITE,              // 33 Write a raw message to the device
   eQMI_WMS_RAW_READ,               // 34 Read a raw message from the device
   eQMI_WMS_MODIFY_TAG,             // 35 Modify message tag on the device
   eQMI_WMS_DELETE,                 // 36 Delete message by index/tag/memory

   eQMI_WMS_GET_MSG_PROTOCOL = 48,  // 48 Get the current message protocol
   eQMI_WMS_GET_MSG_LIST,           // 49 Get list of messages from the device
   eQMI_WMS_SET_ROUTES,             // 50 Set routes for message memory storage
   eQMI_WMS_GET_ROUTES,             // 51 Get routes for message memory storage
   eQMI_WMS_GET_SMSC_ADDR,          // 52 Get SMSC address
   eQMI_WMS_SET_SMSC_ADDR,          // 53 Set SMSC address
   eQMI_WMS_GET_MSG_LIST_MAX,       // 54 Get maximum size of SMS storage
   eQMI_WMS_SEND_ACK,               // 55 Send ACK
   eQMI_WMS_SET_RETRY_PERIOD,       // 56 Set retry period
   eQMI_WMS_SET_RETRY_INTERVAL,     // 57 Set retry interval
   eQMI_WMS_SET_DC_DISCO_TIMER,     // 58 Set DC auto-disconnect timer
   eQMI_WMS_SET_MEMORY_STATUS,      // 59 Set memory storage status
   eQMI_WMS_SET_BC_ACTIVATION,      // 60 Set broadcast activation
   eQMI_WMS_SET_BC_CONFIG,          // 61 Set broadcast config
   eQMI_WMS_GET_BC_CONFIG,          // 62 Get broadcast config
   eQMI_WMS_MEMORY_FULL_IND,        // 63 Memory full indication
   eQMI_WMS_GET_DOMAIN_PREF,        // 64 Get domain preference
   eQMI_WMS_SET_DOMAIN_PREF,        // 65 Set domain preference
   eQMI_WMS_MEMORY_SEND,            // 66 Send message from memory store
   eQMI_WMS_GET_MSG_WAITING,        // 67 Get message waiting info
   eQMI_WMS_MSG_WAITING_IND,        // 68 Message waiting indication
   eQMI_WMS_SET_PRIMARY_CLIENT,     // 69 Set client as primary client
   eQMI_WMS_SMSC_ADDR_IND,          // 70 SMSC address indication

   eQMI_WMS_SET_IND_REGISTER      = 71,  // 71 Set indication register
   eQMI_WMS_GET_TRANS_LAYER_INFO  = 72,  // 72 Get Transport Layer Information
   eQMI_WMS_TRANS_LAYER_INFO_IND  = 73,  // 73 Transport Layer Info Indication
   eQMI_WMS_GET_TRANS_NW_REG_INFO = 74,  // 74 Get Transport NW Reg Info
   eQMI_WMS_TRANS_NW_REG_INFO_IND = 75,  // 75 Transport NW Reg Info Indication
   eQMI_WMS_GET_IND_REGISTER      = 77,  // 77 Get indication register
   eQMI_WMS_ENUM_END
};

/* QMI PDS Service Type Message ID Enumeration */
enum eQMIMessagePDS
{
   eQMI_PDS_ENUM_BEGIN = -1,

   eQMI_PDS_RESET,                // 00 Reset PDS service state variables
   eQMI_PDS_SET_EVENT,            // 01 Set PDS report conditions
   eQMI_PDS_EVENT_IND = 1,        // 01 PDS report indication

   eQMI_PDS_GET_STATE = 32,       // 32 Return PDS service state
   eQMI_PDS_STATE_IND = 32,       // 32 PDS service state indication
   eQMI_PDS_SET_STATE,            // 33 Set PDS service state
   eQMI_PDS_START_SESSION,        // 34 Start a PDS tracking session
   eQMI_PDS_GET_SESSION_INFO,     // 35 Get PDS tracking session info
   eQMI_PDS_FIX_POSITION,         // 36 Manual tracking session position
   eQMI_PDS_END_SESSION,          // 37 End a PDS tracking session
   eQMI_PDS_GET_NMEA_CFG,         // 38 Get NMEA sentence config
   eQMI_PDS_SET_NMEA_CFG,         // 39 Set NMEA sentence config
   eQMI_PDS_INJECT_TIME,          // 40 Inject a time reference
   eQMI_PDS_GET_DEFAULTS,         // 41 Get default tracking session config
   eQMI_PDS_SET_DEFAULTS,         // 42 Set default tracking session config
   eQMI_PDS_GET_XTRA_PARAMS,      // 43 Get the GPS XTRA parameters
   eQMI_PDS_SET_XTRA_PARAMS,      // 44 Set the GPS XTRA parameters
   eQMI_PDS_FORCE_XTRA_DL,        // 45 Force a GPS XTRA database download
   eQMI_PDS_GET_AGPS_CONFIG,      // 46 Get the AGPS mode configuration
   eQMI_PDS_SET_AGPS_CONFIG,      // 47 Set the AGPS mode configuration

   eQMI_PDS_GET_SVC_AUTOTRACK,    // 48 Get the service auto-tracking state
   eQMI_PDS_SET_SVC_AUTOTRACK,    // 49 Set the service auto-tracking state
   eQMI_PDS_GET_COM_AUTOTRACK,    // 50 Get COM port auto-tracking config
   eQMI_PDS_SET_COM_AUTOTRACK,    // 51 Set COM port auto-tracking config
   eQMI_PDS_RESET_DATA,           // 52 Reset PDS service data
   eQMI_PDS_SINGLE_FIX,           // 53 Request single position fix
   eQMI_PDS_GET_VERSION,          // 54 Get PDS service version
   eQMI_PDS_INJECT_XTRA,          // 55 Inject XTRA data
   eQMI_PDS_INJECT_POSITION,      // 56 Inject position data
   eQMI_PDS_INJECT_WIFI,          // 57 Inject Wi-Fi obtained data
   eQMI_PDS_GET_SBAS_CONFIG,      // 58 Get SBAS config
   eQMI_PDS_SET_SBAS_CONFIG,      // 59 Set SBAS config
   eQMI_PDS_SEND_NI_RESPONSE,     // 60 Send network initiated response
   eQMI_PDS_INJECT_ABS_TIME,      // 61 Inject absolute time
   eQMI_PDS_INJECT_EFS,           // 62 Inject EFS data
   eQMI_PDS_GET_DPO_CONFIG,       // 63 Get DPO config
   eQMI_PDS_SET_DPO_CONFIG,       // 64 Set DPO config
   eQMI_PDS_GET_ODP_CONFIG,       // 65 Get ODP config
   eQMI_PDS_SET_ODP_CONFIG,       // 66 Set ODP config
   eQMI_PDS_CANCEL_SINGLE_FIX,    // 67 Cancel single position fix
   eQMI_PDS_GET_GPS_STATE,        // 68 Get GPS state

   eQMI_PDS_GET_METHODS = 80,     // 80 Get GPS position methods state
   eQMI_PDS_SET_METHODS,          // 81 Set GPS position methods state

   eQMI_PDS_ENUM_END
};

/* QMI AUTH Service Type Message ID Enumeration */
enum eQMIMessageAUTH
{
   eQMI_AUTH_ENUM_BEGIN = -1,

   eQMI_AUTH_START_EAP = 32,        // 32 Start the EAP session
   eQMI_AUTH_SEND_EAP,              // 33 Send and receive EAP packets
   eQMI_AUTH_EAP_RESULT_IND,        // 34 EAP session result indication
   eQMI_AUTH_GET_EAP_KEYS,          // 35 Get the EAP session keys
   eQMI_AUTH_END_EAP,               // 36 End the EAP session

   eQMI_AUTH_ENUM_END
};
/* QMI CAT Service Type Message ID Enumeration */
enum eQMIMessageCAT
{
   eQMI_CAT_ENUM_BEGIN = -1,

   eQMI_CAT_RESET,                  // 00 Reset CAT service state variables
   eQMI_CAT_SET_EVENT,              // 01 Set new message report conditions
   eQMI_CAT_EVENT_IND = 1,          // 01 New message report indication

   eQMI_CAT_GET_STATE = 32,         // 32 Get service state information
   eQMI_CAT_SEND_TERMINAL,          // 33 Send a terminal response
   eQMI_CAT_SEND_ENVELOPE,          // 34 Send an envelope command

   eQMI_CAT_ENUM_END
};

/* QMI RMS Service Type Message ID Enumeration */
enum eQMIMessageRMS
{
   eQMI_RMS_ENUM_BEGIN = -1,

   eQMI_RMS_RESET,                  // 00 Reset RMS service state variables

   eQMI_RMS_GET_SMS_WAKE = 32,      // 32 Get SMS wake settings
   eQMI_RMS_SET_SMS_WAKE,           // 33 Set SMS wake settings

   eQMI_RMS_ENUM_END
};

/* QMI OMA-DM Service Type Message ID Enumeration */
enum eQMIMessageOMA
{
   eQMI_OMA_ENUM_BEGIN = -1,

   eQMI_OMA_RESET,                  // 00 Reset OMA service state variables
   eQMI_OMA_SET_EVENT,              // 01 Set OMA report conditions
   eQMI_OMA_EVENT_IND = 1,          // 01 OMA report indication

   eQMI_OMA_START_SESSION = 32,     // 32 Start client inititated session
   eQMI_OMA_CANCEL_SESSION,         // 33 Cancel session
   eQMI_OMA_GET_SESSION_INFO,       // 34 Get session information
   eQMI_OMA_SEND_SELECTION,         // 35 Send selection for net inititated msg
   eQMI_OMA_GET_FEATURES,           // 36 Get feature settings
   eQMI_OMA_SET_FEATURES,           // 37 Set feature settings

   eQMI_OMA_ENUM_END
};

/* SWI QMI OMA-DM Service Type Message ID Enumeration */
enum eQMIMessageSWIOMA
{
   eQMI_SWIOMA_ENUM_BEGIN = -1,

   eQMI_SWIOMA_START_SESSION = 1,     // 1 Start client inititated session
   eQMI_SWIOMA_CANCEL_SESSION,        // 2 Cancel session
   eQMI_SWIOMA_GET_SESSION_INFO,      // 3 Get session information
   eQMI_SWIOMA_EVENT_IND,             // 4 OMA report indication
   eQMI_SWIOMA_SEND_SELECTION,        // 5 Send selection for net inititated msg
   eQMI_SWIOMA_GET_SETTINGS,          // 6 Get feature settings
   eQMI_SWIOMA_SET_SETTINGS,          // 7 Set feature settings
   eQMI_SWIOMA_SET_EVENT,             // 8 Set OMA report conditions

   eQMI_SWIOMA_ENUM_END
};

/* QMI Device Connectivity Service (DCS) Type Message ID Enumeration */
enum eQMIMessageDCS
{
   eQMI_DCS_ENUM_BEGIN = -1,
   eQMI_DCS_ENUMERATE_DEVICES,          // QCWWANEnumerateDevices API QMI message
   eQMI_DCS_CONNECT,                    // QCWWANConnect                -"-
   eQMI_DCS_DISCONNECT,                 // QCWWANDisconnect             -"-
   eQMI_DCS_GET_CONNECTED_DEVICE_ID,    // QCWWANGetConnectedDeviceIP   -"-
   eQMI_DCS_CANCEL,                     // QCWWANCancel                 -"-
   eQMI_DCS_GET_USB_PORT_NAMES,         // SLQSGetUsbPortNames          -"-
   eQMI_DCS_GET_DEVICE_MODE,            // SLQSGetDeviceMode            -"-
   eQMI_DCS_EVENT_IND,
   eQMI_DCS_ENUM_END
};

/* QMI SAR Service Type Message ID Enumeration */
enum eQMIMessageSAR
{
   eQMI_SAR_ENUM_BEGIN = -1,
   eQMI_SAR_RF_SET_STATE = 1,
   eQMI_SAR_RF_GET_STATE,
   eQMI_SAR_ENUM_END
};

/* QMI VOICE Service Type Message ID Enumeration */
enum eQMIMessageVOICE
{
   eQMI_VOICE_ENUM_BEGIN = -1,
   eQMI_VOICE_INDICATION_REGISTER       =  3, //Voice Indication register QMI message
   eQMI_VOICE_DIAL_CALL                 = 32, //SLQSVoiceDialCall API QMI message
   eQMI_VOICE_END_CALL                  = 33, //SLQSVoiceEndCall API QMI message
   eQMI_VOICE_ANSWER_CALL               = 34, //SLQSAnswerCall API
   eQMI_VOICE_GET_CALL_INFO             = 36, //SLQSVoiceGetCallInfo API QMI message
   eQMI_VOICE_OTASP_STATUS_IND          = 37, //SLQSVoiceSetOTASPStatusCallback
   eQMI_VOICE_INFO_REC_IND              = 38, //SLQSVoiceInfoRecCallback
   eQMI_VOICE_SEND_FLASH                = 39, //SLQSVoiceSendFlash API QMI message
   eQMI_VOICE_BURST_DTMF                = 40, //SLQSVoiceBurstDTMF
   eQMI_VOICE_START_CONT_DTMF           = 41, //SLQSVoiceStartContDTMF
   eQMI_VOICE_STOP_CONT_DTMF            = 42, //SLQSVoiceStopContDTMF
   eQMI_VOICE_DTMF_IND                  = 43, //SLQSVoiceSetDTMFEventCallBack QMI message
   eQMI_VOICE_SET_PREFERRED_PRIVACY     = 44, //SLQSVoiceSetPreferredPrivacy API QMI message
   eQMI_VOICE_PRIVACY_IND               = 45, //SLQSVoiceSetPrivacyChangeCallBack
   eQMI_VOICE_ALL_CALL_STATUS_IND       = 46, //SLQSVoiceSetAllCallStatusCallBack
   eQMI_VOICE_GET_ALL_CALL_INFO         = 47, //SLQSVoiceGetAllCallInfo API QMI message
   eQMI_VOICE_MANAGE_CALLS              = 49, //SLQSVoiceManageCalls
   eQMI_VOICE_SUPS_NOTIFICATION_IND     = 50, //SLQSVoiceSetSUPSNotificationCallBack API QMI message
   eQMI_VOICE_SET_SUPS_SERVICE          = 51, //SLQSVoiceSetSUPSService API QMI message
   eQMI_VOICE_GET_CALL_WAITING          = 52, //SLQSVoiceGetCallWaiting API QMI message
   eQMI_VOICE_GET_CALL_BARRING          = 53, //SLQSVoiceGetCallBarring API QMI message
   eQMI_VOICE_GET_CLIP                  = 54, //SLQSVoiceGetCLIP API QMI message
   eQMI_VOICE_GET_CLIR                  = 55, //SLQSVoiceGetCLIR API QMI message
   eQMI_VOICE_GET_CALL_FORWARDING       = 56, //SLQSVoiceGetCallForwarding API QMI message
   eQMI_VOICE_SET_CALL_BARRING_PASSWORD = 57, //SLQSVoiceSetCallBarringPassword API QMI message
   eQMI_VOICE_ORIGINATE_USSD            = 58, //OrignateUSSD API QMI message
   eQMI_VOICE_ANSWER_USSD               = 59, //AnswerUSSD API QMI message
   eQMI_VOICE_CANCEL_USSD               = 60, //CancelUSSD API QMI message
   eQMI_VOICE_USSD_RELEASE_IND          = 61, //Release Indication
   eQMI_VOICE_USSD_IND                  = 62, //USSD Indication
   eQMI_VOICE_SET_CONFIG                = 64, //SLQSVoiceSetConfig API QMI message
   eQMI_VOICE_GET_CONFIG                = 65, //SLQSVoiceGetConfig API QMI message
   eQMI_VOICE_SUPS_IND                  = 66, //SLQSVoiceSetSUPSCallBack QMI message
   eQMI_VOICE_ORIG_USSD_NO_WAIT         = 67, //SLQSVoiceOrigUSSDNoWait  API QMI message
   eQMI_VOICE_BIND_SUBSCRIPTION         = 68, //SLQSVoiceBindSubscription API QMI message
   eQMI_VOICE_ALS_SET_LINE_SWITCHING    = 69, //SLQSVoiceALSSetLineSwitching API QMI message
   eQMI_VOICE_ALS_SELECT_LINE           = 70, //SLQSVoiceALSSelectLine API QMI message
   eQMI_VOICE_GET_COLP                  = 75, //SLQSVoiceGetCOLP API QMI message
   eQMI_VOICE_GET_COLR                  = 76, //SLQSVoiceGetCOLR API QMI message
   eQMI_VOICE_GET_CNAP                  = 77, //SLQSVoiceGetCNAP API QMI message
   eQMI_VOICE_ENUM_END
};

enum eQMI_DCS_ENUMERATE_TLVs
{
    eTLV_DCS_ENUMERATE_DEVICE_NODE,
    eTLV_DCS_ENUMERATE_DEVICE_KEY
};

enum eQMI_DCS_CONNECT_TLVs
{
    eTLV_DCS_CONNECT_DEVICE_NODE,
    eTLV_DCS_CONNECT_DEVICE_KEY
};

enum eQMI_DCS_DISCONNECT_TLVs
{
    eTLV__DCS_DISCONNECT_TLVs_PLACEHOLDER
};

enum eQMI_DCS_CONNECTED_DEVICEID_TLVs
{
    eTLV_DCS_CONNECTED_DEVICEID_DEVICE_NODE,
    eTLV_DCS_CONNECTED_DEVICEID_DEVICE_KEY
};

enum eQMI_DCS_CANCEL_TLVs
{
    eTLV_DCS_CANCEL_TLVs_PLACEHOLDER
};

enum eQMI_DCS_GET_USB_PORT_NAME_TLVs
{
    eTLV_DCS_GET_USB_PORT_NAME_ATCMDPORT =0x10,
    eTLV_DCS_GET_USB_PORT_NAME_NMEAPORT,
    eTLV_DCS_GET_USB_PORT_NAME_DMPORT
};

enum eQMI_DCS_GET_DEVICE_MODE_TLVs
{
    eTLV_DCS_GET_DEVICE_MODE =0x10
};

enum eQMI_DCS_EVENT_INDICATION_TLVs
{
    eTLV_DCS_IND_DEVICE_STATE   = 0x10,
    eTLV_DCS_IND_SDK_TERMINATED = 0x11
};

/* QMI Firmware Management Service (FMS) Type Message ID Enumeration */
enum eQMIMessageFMS
{
   eQMI_FMS_ENUM_BEGIN = -1,
   eQMI_FMS_SET_IMAGE_PATH,     /* support for UpgradeFirmware2k() API */
   eQMI_FMS_GET_CWE_SPKGS_INFO, /* support for SLQSGetImageInfo() API */
   eQMI_FMS_GET_MBN_INFO,       /* support for GetImageInfo() API */
   eQMI_FMS_EVENT_IND,          /* FMS notifications to host */
   eQMI_FMS_ENUM_END
};

enum eQMI_FMS_SET_IMAGE_PATH_TLVs
{
    eTLV_FMS_IMAGE_PATH
};

enum eQMI_FMS_EVENT_INDICATION_TLVs
{
    eTLV_FMS_IND_FW_DWLD_COMPLETE   = 0x10
};

enum eQMI_FMS_GET_CWE_SPKGS_INFO_TLVs
{
    /* request TLVs */
    eTLV_FMS_GET_CWE_SPKGS_INFO_IMGDIR_PATH,
    /* response TLVs */
    eTLV_FMS_GET_CWE_SPKGS_INFO_SKUID_STR = 0x10,
    eTLV_FMS_GET_CWE_SPKGS_INFO_MODELID_STR,
    eTLV_FMS_GET_CWE_SPKGS_INFO_FWVERSION_STR,
    eTLV_FMS_GET_CWE_SPKGS_INFO_CARRIER_STR,
    eTLV_FMS_GET_CWE_SPKGS_INFO_PRIVERSION_STR,
    eTLV_FMS_GET_CWE_SPKGS_INFO_PKGVERSION_STR
};

enum eQMI_FMS_GET_MBN_INFO_TLVs
{
    /* request TLVs */
    eTLV_FMS_GET_MBN_INFO_IMGDIR_PATH,
    eTLV_FMS_GET_MBN_INFO_IMGTYPE,
    /* response TLVs */
    eTLV_FMS_GET_MBN_INFO_FIRMWAREID = 0x10,
    eTLV_FMS_GET_MBN_INFO_FWVERSION_STR,
    eTLV_FMS_GET_MBN_INFO_FIRMWAREID_STR
};

/* QMI UIM Service Type Message ID Enumeration */
enum eQMIMessageUIM
{
   eQMI_UIM_ENUM_BEGIN             = -1,
   eQMI_UIM_RESET                  =  0, //SLQSUIMReset API QMI message
   eQMI_UIM_GET_FILE_ATTRIBUTES    = 36, //SLQSUIMGetFileAttributes API QMI message
   eQMI_UIM_SET_PIN_PROTECTION     = 37, //SLQSUIMSetPinProtection API QMI message
   eQMI_UIM_VERIFY_PIN             = 38, //SLQSUIMVerifyPin API QMI message
   eQMI_UIM_UNBLOCK_PIN            = 39, //SLQSUIMUnblockPin API QMI message
   eQMI_UIM_CHANGE_PIN             = 40, //SLQSUIMChangePin API QMI message
   eQMI_UIM_DEPERSONALIZATION      = 41, //SLQSUIMDepersonalization API QMI message
   eQMI_UIM_REFRESH_REGISTER       = 42, //SLQSUIMRefreshRegister
   eQMI_UIM_REFRESH_OK             = 43, //SLQSUIMRefreshOK
   eQMI_UIM_REFRESH_COMPLETE       = 44, //SLQSUIMRefreshComplete
   eQMI_UIM_REFRESH_GET_LAST_EVENT = 45, //SLQSUIMRefreshRegister
   eQMI_UIM_EVENT_REG              = 46, //SLQSUIMEventRegister API QMI message
   eQMI_UIM_GET_CARD_STATUS        = 47, //SLQSUIMGetCardStatus API QMI message
   eQMI_UIM_POWER_DOWN             = 48, //SLQSUIMPowerDown API QMI message
   eQMI_UIM_STATUS_CHANGE_IND      = 50, //SLQSUIMSetStatusChangeCallBack
   eQMI_UIM_REFRESH_IND            = 51, //SLQSUIMSetRefreshCallBack
   eQMI_UIM_AUTHENTICATE           = 52  //SLQSUIMAuthenticate API QMI message
};

/* QMI Call End Reason Enumeration */
enum eQMICallEndReason
{
    eQMI_CALL_END_REASON_BEGIN = -1,

   // General
   eQMI_CALL_END_REASON_UNSPECIFIED = 1,           // 1
   eQMI_CALL_END_REASON_CLIENT_END,                // 2
   eQMI_CALL_END_REASON_NO_SRV,                    // 3
   eQMI_CALL_END_REASON_FADE,                      // 4
   eQMI_CALL_END_REASON_REL_NORMAL,                // 5
   eQMI_CALL_END_REASON_ACC_IN_PROG,               // 6
   eQMI_CALL_END_REASON_ACC_FAIL,                  // 7
   eQMI_CALL_END_REASON_REDIR_OR_HANDOFF,          // 8
   eQMI_CALL_END_REASON_CLOSE_IN_PROGRESS,         // 9
   eQMI_CALL_END_REASON_AUTH_FAILED,               // 10
   eQMI_CALL_END_REASON_INTERNAL,                  // 11

   // CDMA
   eQMI_CALL_END_REASON_CDMA_LOCK = 500,           // 500
   eQMI_CALL_END_REASON_INTERCEPT,                 // 501
   eQMI_CALL_END_REASON_REORDER,                   // 502
   eQMI_CALL_END_REASON_REL_SO_REJ,                // 503
   eQMI_CALL_END_REASON_INCOM_CALL,                // 504
   eQMI_CALL_END_REASON_ALERT_STOP,                // 505
   eQMI_CALL_END_REASON_ACTIVATION,                // 506
   eQMI_CALL_END_REASON_MAX_ACCESS_PROBE,          // 507
   eQMI_CALL_END_REASON_CCS_NOT_SUPPORTED_BY_BS,   // 508
   eQMI_CALL_END_REASON_NO_RESPONSE_FROM_BS,       // 509
   eQMI_CALL_END_REASON_REJECTED_BY_BS,            // 510
   eQMI_CALL_END_REASON_INCOMPATIBLE,              // 511
   eQMI_CALL_END_REASON_ALREADY_IN_TC,             // 512
   eQMI_CALL_END_REASON_USER_CALL_ORIG_DURING_GPS, // 513
   eQMI_CALL_END_REASON_USER_CALL_ORIG_DURING_SMS, // 514
   eQMI_CALL_END_REASON_NO_CDMA_SRV,               // 515

   // GSM/WCDMA
   eQMI_CALL_END_REASON_CONF_FAILED = 1000,        // 1000
   eQMI_CALL_END_REASON_INCOM_REJ,                 // 1001
   eQMI_CALL_END_REASON_NO_GW_SRV,                 // 1002
   eQMI_CALL_END_REASON_NETWORK_END,               // 1003

   eQMI_CALL_END_REASON_LLC_SNDCP_FAILURE,         // 1004
   eQMI_CALL_END_REASON_INSUFFICIENT_RESOURCES,    // 1005
   eQMI_CALL_END_REASON_OPTION_TEMP_OOO,           // 1006
   eQMI_CALL_END_REASON_NSAPI_ALREADY_USED,        // 1007
   eQMI_CALL_END_REASON_REGULAR_DEACTIVATION,      // 1008
   eQMI_CALL_END_REASON_NETWORK_FAILURE,           // 1009
   eQMI_CALL_END_REASON_UMTS_REATTACH_REQ,         // 1010
   eQMI_CALL_END_REASON_UMTS_PROTOCOL_ERROR,       // 1011
   eQMI_CALL_END_REASON_OPERATOR_BARRING,          // 1012
   eQMI_CALL_END_REASON_UNKNOWN_APN,               // 1013
   eQMI_CALL_END_REASON_UNKNOWN_PDP,               // 1014
   eQMI_CALL_END_REASON_GGSN_REJECT,               // 1015
   eQMI_CALL_END_REASON_ACTIVATION_REJECT,         // 1016
   eQMI_CALL_END_REASON_OPTION_NOT_SUPPORTED,      // 1017
   eQMI_CALL_END_REASON_OPTION_UNSUBSCRIBED,       // 1018
   eQMI_CALL_END_REASON_QOS_NOT_ACCEPTED,          // 1019
   eQMI_CALL_END_REASON_TFT_SEMANTIC_ERROR,        // 1020
   eQMI_CALL_END_REASON_TFT_SYNTAX_ERROR,          // 1021
   eQMI_CALL_END_REASON_UNKNOWN_PDP_CONTEXT,       // 1022
   eQMI_CALL_END_REASON_FILTER_SEMANTIC_ERROR,     // 1023
   eQMI_CALL_END_REASON_FILTER_SYNTAX_ERROR,       // 1024
   eQMI_CALL_END_REASON_PDP_WITHOUT_ACTIVE_TFT,    // 1025
   eQMI_CALL_END_REASON_INVALID_TRANSACTION_ID,    // 1026
   eQMI_CALL_END_REASON_MESSAGE_SEMANTIC_ERROR,    // 1027
   eQMI_CALL_END_REASON_INVALID_MANDATORY_INFO,    // 1028
   eQMI_CALL_END_REASON_TYPE_UNSUPPORTED,          // 1029
   eQMI_CALL_END_REASON_MSG_TYPE_WRONG_FOR_STATE,  // 1030
   eQMI_CALL_END_REASON_UNKNOWN_INFO_ELEMENT,      // 1031
   eQMI_CALL_END_REASON_CONDITIONAL_IE_ERROR,      // 1032
   eQMI_CALL_END_REASON_MSG_WRONG_FOR_PROTOCOL,    // 1033
   eQMI_CALL_END_REASON_APN_TYPE_CONFLICT,         // 1034
   eQMI_CALL_END_REASON_NO_GPRS_CONTEXT,           // 1035
   eQMI_CALL_END_REASON_FEATURE_NOT_SUPPORTED,     // 1036

   // CDMA 1xEV-DO (HDR)
   eQMI_CALL_END_REASON_CD_GEN_OR_BUSY = 1500,     // 1500
   eQMI_CALL_END_REASON_CD_BILL_OR_AUTH,           // 1501
   eQMI_CALL_END_REASON_CHG_HDR,                   // 1502
   eQMI_CALL_END_REASON_EXIT_HDR,                  // 1503
   eQMI_CALL_END_REASON_HDR_NO_SESSION ,           // 1504
   eQMI_CALL_END_REASON_HDR_ORIG_DURING_GPS_FIX,   // 1505
   eQMI_CALL_END_REASON_HDR_CS_TIMEOUT ,           // 1506
   eQMI_CALL_END_REASON_HDR_RELEASED_BY_CM,        // 1507

   eQMI_CALL_END_REASON_END
};

/*************
 *
 * Name:    eQMITlvCommon
 *
 * Purpose: enumerated common TLV operation codes
 *
 * Members:
 *
 * Notes:
 *
 **************/
enum eQMITlvCommon
{
    eTLV_RESULT_CODE  = 0x02,
    eTLV_TYPE_INVALID = 0xFF
};

/*************
 *
 * Name:    eQMITlvCommonLength
 *
 * Purpose: Common TLV Length codes for responses/indications
 *
 * Members:
 *
 * Notes:
 *
 **************/
enum eQMITlvCommonLength
{
    eTLV_LENGTH_VARIABLE = -1
};

/*************
 *
 * Name:    eQMITimeout
 *
 * Purpose: enumerated timeout values used for call to APIs.
 *
 * Members:
 *
 * Notes:   Don't change the order or the enumeration of this list
 *
 **************/
enum eQMITimeout
{
    eQMI_TIMEOUT_2_S        = 2000,
    eQMI_TIMEOUT_5_S        = 5000,
    eQMI_TIMEOUT_8_S        = 8000,
    eQMI_TIMEOUT_10_S       = 10000,
    eQMI_TIMEOUT_20_S       = 20000,
    eQMI_TIMEOUT_30_S       = 30000,
    eQMI_TIMEOUT_60_S       = 60000,
    eQMI_TIMEOUT_300_S      = 300000,
    eQMI_TIMEOUT_DEFAULT    = eQMI_TIMEOUT_8_S
};

/*************
 *
 * Name:    qmTBuffer
 *
 * Purpose: QM pack/unpack helper structure
 *
 * Members: Size        - Maximum size of QMI request/response Value (TLVs)
 *          Index       - QMI request/response Message buffer index used during pack/unpack
 *          Datap       - pointer to QMI message Value buffer (TLVs) for pack/unpack
 *
 * Notes:
 *
 **************/
struct qmTBuffer
{
    swi_uint16  Size;  /* Max number of swi_uint8s in data */
    swi_uint16  Index; /* Location wrt start of data for next read */
    swi_uint8   *Datap; /* pointer to contiguous data */
};

/*************
 *
 * Name:    qmMessage
 *
 * Purpose: QMI service request message structure
 *
 * Members: MessageID   - QMI Message ID
 *          Length      - QMI Message Value length
 *          Bufferp     - pointer to QMI message Value (TLVs)
 *
 * Notes:
 *
 **************/
struct qmMessage
{
    swi_uint16          MessageId;
    swi_uint16          Length;
    struct qmTBuffer    *Bufferp;
};

/*************
 *
 * Name:    qmTlvBuilderItem
 *
 * Purpose: QM Builder message information
 *
 * Members: tlvType     - QMI Message TLV's Type field value
 *          Builder     - Function invoked to pack the QMI message TLV for the
 *                        specified TLV Type.
 *
 * Notes:
 *
 **************/
struct qmTlvBuilderItem
{
    swi_uint8         tlvType;
    enum eQCWWANError (*Builder)(swi_uint8*, swi_uint8*);
};

/*************
 *
 * Name:    qmTlvUnpackerItem
 *
 * Purpose: QM Unpack message information
 *
 * Members: tlvType     - QMI Message TLV's Type field value
 *          Unpackere   - Function invoked to unpack the QMI message TLV for
 *                        the specified TLV Type.
 *
 * Notes:
 *
 **************/
struct qmTlvUnpackerItem
{
    swi_uint8         tlvType;
    enum eQCWWANError (*Unpacker)(swi_uint8*, swi_uint8*) ;
};

/*************
 *
 * Name:    qmTlvResult
 *
 * Purpose: QMI result TLV structure
 *
 * Members: TlvResultCode   - SDK QMI request processing result code
 *          DevicuResult    - Device result code
 *          DeivceError     - Device error code
 *
 * Notes:
 *
 **************/
struct qmTlvResult
{
    swi_uint32  TlvResultCode;
    swi_uint16  DeviceResult;
    swi_uint16  DeviceError;
};

#include "qm/qmuproto.h"

#endif /* QMUDEFS_H */
