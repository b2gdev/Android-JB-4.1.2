/**************
 *
 *  Filename: qatesthelper.h
 *
 *  Purpose:  Contains the common elements used by WDS, NAS and
 *            DMS Test Applications
 *
 * Copyright: © 2011 Sierra Wireless, Inc., all rights reserved
 *
 **************/

#ifndef __TEST_HELPER_H__
#define __TEST_HELPER_H__

#ifdef ARM
#define fprintf( fp, format, args... ) fprintf(stdout, format , ##args)
#endif

struct errorcodemap
{
    enum eQCWWANError code;
    char *desc;
};

static const struct errorcodemap codemap[] =
{
    {eQCWWAN_ERR_ENUM_BEGIN,        "Invalid error code"},
    {eQCWWAN_ERR_NONE,              "Success"},
    {eQCWWAN_ERR_GENERAL,           "General error"},
    {eQCWWAN_ERR_INTERNAL,          "Internal error"},
    {eQCWWAN_ERR_MEMORY,            "Memory error"},
    {eQCWWAN_ERR_INVALID_ARG,       "Invalid argument"},
    {eQCWWAN_ERR_BUFFER_SZ,         "Buffer too small"},
    {eQCWWAN_ERR_NO_DEVICE,         "Unable to detect WWAN device"},
    {eQCWWAN_ERR_INVALID_DEVID,     "Invalid WWAN device ID"},
    {eQCWWAN_ERR_NO_CONNECTION,     "No connection to WWAN device"},
    {eQCWWAN_ERR_QMI_IFACE,         "Unable to obtain QMI interace"},
    {eQCWWAN_ERR_QMI_CONNECT,       "Unable to connect to QMI interface"},
    {eQCWWAN_ERR_QMI_REQ_SCH,       "Unable to schedule QMI request"},
    {eQCWWAN_ERR_QMI_REQ,           "Error sending QMI request"},
    {eQCWWAN_ERR_QMI_RSP,           "Error receiving QMI response"},
    {eQCWWAN_ERR_QMI_REQ_TO,        "Timeout while sending QMI request"},
    {eQCWWAN_ERR_QMI_RSP_TO,        "Timeout while receiving QMI response"},
    {eQCWWAN_ERR_MALFORMED_QMI_RSP, "Malformed QMI response received"},
    {eQCWWAN_ERR_INVALID_QMI_RSP,   "Invalid QMI response received"},
    {eQCWWAN_ERR_INVALID_FILE,      "Invalid file path"},
    {eQCWWAN_ERR_FILE_OPEN,         "Unable to open file"},
    {eQCWWAN_ERR_FILE_COPY,         "Unable to copy file"},
    {21,                            "Not Used"},
    {22,                            "Not Used"},
    {23,                            "Not Used"},
    {24,                            "Not Used"},
    {25,                            "Not Used"},
    {26,                            "Not Used"},
    {eQCWWAN_ERR_OFFLINE,           "Unable to set WWAN device offline"},
    {eQCWWAN_ERR_RESET,             "Unable to reset WWAN device"},
    {eQCWWAN_ERR_NO_SIGNAL,         "No available signal"},
    {eQCWWAN_ERR_MULTIPLE_DEVICES,  "Multiple WWAN devices detected"},
    {eQCWWAN_ERR_DRIVER,            "Error interfacing to driver"},
    {eQCWWAN_ERR_NO_CANCELABLE_OP,  "No cancelable operation is pending"},
    {eQCWWAN_ERR_CANCEL_OP,         "Error canceling outstanding operation"},
    {eQCWWAN_ERR_ENUM_END,          "No more error codes beyond this"}
};
static const struct errorcodemap swicmcodemap[] =
{
    {eQCWWAN_ERR_SWICM_START,
     "Invalid error code"},

    {eQCWWAN_ERR_SWICM_NOT_IMPLEMENTED,
     "API yet to be implemented"},

    {eQCWWAN_ERR_SWICM_QMI_SVC_NOT_SUPPORTED,
     "QMI Service not supported"},

    {eQCWWAN_ERR_SWICM_QMI_CLNT_NOT_SUPPORTED,
     "QMI client not supported"},

    {eQCWWAN_ERR_SWICM_TIMEOUT,
     "SDK timed out"},

    {eQCWWAN_ERR_SWICM_SOCKET_IN_USE,
     "socket is in use"},

    {eQCWWAN_ERR_SWICM_AM_VERS_ERROR,
     "AM Message Header version mismatch"},

    {eQCWWAN_ERR_SWICM_FAILED_TO_KILL_SDK_PROCESS,
     "Failed to kill SDK"},

    {eQCWWAN_ERR_SWICM_CALL_IN_PROGRESS,
     "Cannot complete operation while device is in a call"},

    {eQCWWAN_ERR_SWICM_V4DWN_V6DWN,
     "V4 session inactive, V6 session inactive"},

    {eQCWWAN_ERR_SWICM_V4DWN_V6UP,
     "V4 session inactive, V6 session active"},

    {eQCWWAN_ERR_SWICM_V4UP_V6DWN,
     "V4 session active, V6 session inactive"},

    {eQCWWAN_ERR_SWICM_V4UP_V6UP,
     "V4 session active, V6 session active"},

    {eQCWWAN_ERR_SWICM_END,
     "No more CM error codes beyond this"}
};

static const struct errorcodemap swiimcodemap[] =
{
    {eQCWWAN_ERR_SWIIM_START,             "Invalid error code"},
    {eQCWWAN_ERR_SWIIM_INVALID_PATH,      "Invalid directory path"},
    {eQCWWAN_ERR_SWIIM_OPENING_DIR,       "Unable to open the directory"},
    {eQCWWAN_ERR_SWIIM_FILE_NOT_FOUND,    "No Firmware image present in the path"},
    {eQCWWAN_ERR_SWIIM_OPENING_FILE,      "Unable to open the file"},
    {eQCWWAN_ERR_SWIIM_CORRUPTED_FW_IMAGE,"Firmware image is corrupt"},
    {eQCWWAN_ERR_SWIIM_END,               "No more IM error codes beyond this"}
};

static const struct errorcodemap swismscodemap[] =
{
    {eQCWWAN_ERR_SWISMS_START,                 "Invalid error code"},
    {eQCWWAN_ERR_SWISMS_MSG_LEN_TOO_LONG,      "Message too long"},
    {eQCWWAN_ERR_SWISMS_MSG_CORRUPTED,         "Message corrupt octet length too high"},
    {eQCWWAN_ERR_SWISMS_SMSC_NUM_CORRUPTED,    "SMSC number missing or corrupt"},
    {eQCWWAN_ERR_SWISMS_BEARER_DATA_NOT_FOUND, "No Bearer data present"},
    {eQCWWAN_ERR_SWISM_END,                    "No more SMS error codes beyond this"}
};

static const struct errorcodemap qmicodemap[] =
{
    {eQCWWAN_ERR_QMI_MALFORMED_MSG, "Malformed or Correupted QMI msg"},

    {eQCWWAN_ERR_QMI_NO_MEMORY,\
        "Device could not allocate memory for QMI Resp"},

    {eQCWWAN_ERR_QMI_INTERNAL,\
        "Unexpected error occurred during processing"},

    {eQCWWAN_ERR_QMI_ABORTED,\
        "Processing aborted"},

    {eQCWWAN_ERR_QMI_CLIENT_IDS_EXHAUSTED,\
        "QMI client IDs have been exhausted"},

    {eQCWWAN_ERR_QMI_UNABORTABLE_TRANSACTION,\
        "Unable to abort QMI transaction"},

    {eQCWWAN_ERR_QMI_INVALID_CLIENT_ID,\
        "Invalid QMI client ID"},

    {eQCWWAN_ERR_QMI_NO_THRESHOLDS,\
        "No thresholds were provided"},

    {eQCWWAN_ERR_QMI_INVALID_HANDLE,\
        "Invalid Handle provided in the QMI request"},

    {eQCWWAN_ERR_QMI_INVALID_PROFILE,\
        "Profile specified is invalid"},

    {eQCWWAN_ERR_QMI_INVALID_PINID,\
        "Invalid PIN ID specified"},

    {eQCWWAN_ERR_QMI_INCORRECT_PIN,\
        "Incorrect PIN ID specified"},

    {eQCWWAN_ERR_QMI_NO_NETWORK_FOUND,\
        "No network found"},

    {eQCWWAN_ERR_QMI_CALL_FAILED,\
        "Call failed"},

    {eQCWWAN_ERR_QMI_OUT_OF_CALL,\
        "Device is not in a call"},

    {eQCWWAN_ERR_QMI_NOT_PROVISIONED,\
        "Requested information element not provisioned on device"},

    {eQCWWAN_ERR_QMI_MISSING_ARG,\
        "Mandatory QMI TLV not provided"},

    {1018, "Not Used"},

    {eQCWWAN_ERR_QMI_ARG_TOO_LONG,\
        "Arg passed in QMI TLV larger than available storage in device"},

    {1020, "Not Used"},
    {1021, "Not Used"},

    {eQCWWAN_ERR_QMI_INVALID_TX_ID,\
        "Invalid TX ID specified"},

    {eQCWWAN_ERR_QMI_DEVICE_IN_USE,\
        "Device currently in a call"},

    {eQCWWAN_ERR_QMI_OP_NETWORK_UNSUPPORTED,\
        "The selected operation is not supported by the network"},

    {eQCWWAN_ERR_QMI_OP_DEVICE_UNSUPPORTED,\
        "The selected operation is not supported by the device"},

    {eQCWWAN_ERR_QMI_NO_EFFECT,\
        "Requested operation would have no effect"},

    {eQCWWAN_ERR_QMI_NO_FREE_PROFILE,\
        "No space for a profile is available"},

    {eQCWWAN_ERR_QMI_INVALID_PDP_TYPE,\
        "Invalid PDP type specified"},

    {eQCWWAN_ERR_QMI_INVALID_TECH_PREF,\
        "Invalid technology preference specified"},

    {eQCWWAN_ERR_QMI_INVALID_PROFILE_TYPE,\
        "Invalid profile type specified"},

    {eQCWWAN_ERR_QMI_INVALID_SERVICE_TYPE,\
        "Invalid service type specified"},

    {eQCWWAN_ERR_QMI_INVALID_REGISTER_ACTION,
        "Invalid register action specified"},

    {eQCWWAN_ERR_QMI_INVALID_PS_ATTACH_ACTION,\
        "Invalid PS attach/detach action specified"},

    {eQCWWAN_ERR_QMI_AUTHENTICATION_FAILED,\
        "Authentication of supplied information element failed"},

    {eQCWWAN_ERR_QMI_PIN_BLOCKED,\
        "PIN is blocked; an unblock operation needs to be issued"},

    {eQCWWAN_ERR_QMI_PIN_PERM_BLOCKED,\
        "PIN is permanently blocked; the UIM is unusable"},

    {eQCWWAN_ERR_QMI_SIM_NOT_INITIALIZED,\
        "UIM initialized has not completed"},

    {eQCWWAN_ERR_QMI_MAX_QOS_REQUESTS_IN_USE,\
        " Maximum QOS Requests in Use"},

    {eQCWWAN_ERR_QMI_INCORRECT_FLOW_FILTER,\
        "Incorrect Flow Filter"},

    {eQCWWAN_ERR_QMI_NETWORK_QOS_UNAWARE,\
        "Network QOS Unaware"},

    {eQCWWAN_ERR_QMI_INVALID_ID,\
        "Invalid ID"},

    {eQCWWAN_ERR_QMI_REQUESTED_NUM_UNSUPPORTED,\
        "Requested Num not supported"},

    {eQCWWAN_ERR_QMI_INTERFACE_NOT_FOUND,\
        "Interface Not Found"},

    {eQCWWAN_ERR_QMI_FLOW_SUSPENDED,\
        "Flow Suspended"},

    {eQCWWAN_ERR_QMI_INVALID_DATA_FORMAT,\
        "Invalid Data Format"},

    {eQCWWAN_ERR_QMI_GENERAL,\
        "General error"},

    {eQCWWAN_ERR_QMI_UNKNOWN,\
        "Unknown error"},

    {eQCWWAN_ERR_QMI_INVALID_ARG,\
        "A specified argument is invalid"},

    {eQCWWAN_ERR_QMI_INVALID_INDEX,\
        "A specified index is invalid"},

    {eQCWWAN_ERR_QMI_NO_ENTRY,\
        "No information element exists at specified memory designation"},

    {eQCWWAN_ERR_QMI_DEVICE_STORAGE_FULL,\
        "The memory storage specified in the request is full"},

    {eQCWWAN_ERR_QMI_DEVICE_NOT_READY,\
        "Device not in a ready state"},

    {eQCWWAN_ERR_QMI_NETWORK_NOT_READY,\
        "Network not in a ready state"},

    {eQCWWAN_ERR_QMI_CAUSE_CODE,\
        "Error provided in SMS cause code"},

    {eQCWWAN_ERR_QMI_MESSAGE_NOT_SENT,\
        "The message could not be sent"},

    {eQCWWAN_ERR_QMI_MESSAGE_DELIVERY_FAILURE,\
        "The message could not be delivered"},

    {eQCWWAN_ERR_QMI_INVALID_MESSAGE_ID,\
        "The message ID specified for the message is invalid"},

    {eQCWWAN_ERR_QMI_ENCODING,\
        "The message is not encoded properly"},

    {eQCWWAN_ERR_QMI_AUTHENTICATION_LOCK,\
        "Maximum number of authentication failures has been reached"},

    {eQCWWAN_ERR_QMI_INVALID_TRANSITION,\
        "Operating mode transition from the current mode is invalid"},

    {eQCWWAN_ERR_QMI_NOT_A_MCAST_IFACE,\
        "Not a MCAST Interface"},

    {eQCWWAN_ERR_QMI_MAX_MCAST_REQUESTS_IN_USE,\
        "Maximum MCAST Requests in use"},

    {eQCWWAN_ERR_QMI_INVALID_MCAST_HANDLE,\
        "Invalid MCAST Handle"},

    {eQCWWAN_ERR_QMI_INVALID_IP_FAMILY_PREF,\
        "Invalid IP family Preference"},

    {eQCWWAN_ERR_QMI_SESSION_INACTIVE,\
        "No tracking session has been started"},

    {eQCWWAN_ERR_QMI_SESSION_INVALID,\
        "Current session does not allow this operation"},

    {eQCWWAN_ERR_QMI_SESSION_OWNERSHIP,\
        "Current tracking session not started by this QMI control point"},

    {eQCWWAN_ERR_QMI_INSUFFICIENT_RESOURCES,\
        "Device GPS service resources insufficient for request"},

    {eQCWWAN_ERR_QMI_DISABLED,\
        "Device GPS service disabled"},

    {eQCWWAN_ERR_QMI_INVALID_OPERATION, \
        "Invalid operation specified"},

    {eQCWWAN_ERR_QMI_INVALID_QMI_CMD,\
        "Invalid/unknown QMI command specified"},

    {eQCWWAN_ERR_QMI_TPDU_TYPE,\
        "Message contains TPDU type that cannot be read as raw message"},

    {eQCWWAN_ERR_QMI_SMSC_ADDR,\
        "The SMSC address specified is invalid"},

    {eQCWWAN_ERR_QMI_INFO_UNAVAILABLE,\
        "Information element is unavailable at this point"},

    {eQCWWAN_ERR_QMI_SEGMENT_TOO_LONG,\
        "Segment size too large"},

    {eQCWWAN_ERR_QMI_SEGMENT_ORDER,\
        "Segment order is incorrect"},

    {eQCWWAN_ERR_QMI_BUNDLING_NOT_SUPPORTED,\
        "Bundling not supported"},

    {eQCWWAN_ERR_QMI_OP_PARTIAL_FAILURE,\
        "Operation Partial Failure"},

    {eQCWWAN_ERR_QMI_POLICY_MISMATCH,\
        "Policy Msmatch"},

    {eQCWWAN_ERR_QMI_SIM_FILE_NOT_FOUND,\
        "SIM file not found"},

    {eQCWWAN_ERR_QMI_EXTENDED_INTERNAL,\
        "Extended Internal"},

    {eQCWWAN_ERR_QMI_ACCESS_DENIED,\
        "Access to a required entity is not available"},

    {eQCWWAN_ERR_QMI_HARDWARE_RESTRICTED,\
        "Selected operating mode is invalid with current hardware setting"},

    {eQCWWAN_ERR_QMI_ACK_NOT_SENT,\
        "Acknowledgement Not Sent"},

    {eQCWWAN_ERR_QMI_INJECT_TIMEOUT,\
        "Inject Time Out"},

    {1086, "Not Used"},
    {1087, "Not Used"},
    {1088, "Not Used"},
    {1089, "Not Used"},

    /* Sups Errors */
    {eQCWWAN_ERR_QMI_INCOMPATIBLE_STATE,\
        "Incompatible State"},

    {eQCWWAN_ERR_QMI_FDN_RESTRICT,\
       "FDN Restrict"},

    {eQCWWAN_ERR_QMI_SUPS_FAILURE_CAUSE,\
        "SUPS Failure Cause"},

    {eQCWWAN_ERR_QMI_NO_RADIO,\
        "No Radio"},

    {eQCWWAN_ERR_QMI_NOT_SUPPORTED,\
        "Not Supported"},

    {eQCWWAN_ERR_QMI_NO_SUBSCRIPTION,\
        "No Subscription"},

    {eQCWWAN_ERR_QMI_CARD_CALL_CONTROL_FAILED,\
        "Card Call Control Failed"},

    {eQCWWAN_ERR_QMI_NETWORK_ABORTED,\
        "Network Aborted"},

    {eQCWWAN_ERR_QMI_MSG_BLOCKED,\
        "Message Blocked"},

    /* Error 99 is open */
    {1099, "Not Used"},

    {eQCWWAN_ERR_QMI_MAX,\
        "QMI Error Boundary"},
};

static const struct errorcodemap qmicatcodemap[] =
{
    {eQCWWAN_ERR_QMI_EVENT_REG_FAILED,\
        "CAT event registration failed"},

    {eQCWWAN_ERR_QMI_INVALID_TERMINAL_RSP,\
        "Invalid terminal response"},

    {eQCWWAN_ERR_QMI_INVALID_ENVELOPE_CMD,\
        "Invalid envelope command"},

    {eQCWWAN_ERR_QMI_CARD_BUSY_RSP,\
        "Card busy response for envelope command"},

    {eQCWWAN_ERR_QMI_ENVELOPE_CMD_FAILURE, \
        "Envelope command failure"},

};

#endif /* __TEST_HELPER_H__  */
