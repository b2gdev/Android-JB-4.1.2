/*
 * Filename:  qmerrno.h
 *
 * Purpose:   QMI error numbers
 *
 * Copyright: C 2011 Sierra Wireless Inc., all rights reserved
 *
 */

/* Pragmas */
#pragma once

/* Includes */

/**
 * <b>QMI Error Code Enumeration</b>
 */
enum eQCWWANError
{
    eQCWWAN_ERR_ENUM_BEGIN = -1,
    /** 00 - Success */
    eQCWWAN_ERR_NONE,
    /** 01 - General error */
    eQCWWAN_ERR_GENERAL,
    /** 02 - Internal error */
    eQCWWAN_ERR_INTERNAL,
    /** 03 - Memory error */
    eQCWWAN_ERR_MEMORY,
    /** 04 - Invalid argument */
    eQCWWAN_ERR_INVALID_ARG,
    /** 05 - Buffer too small */
    eQCWWAN_ERR_BUFFER_SZ,
    /** 06 - Unable to detect WWAN device */
    eQCWWAN_ERR_NO_DEVICE,
    /** 07 - Invalid WWAN device ID */
    eQCWWAN_ERR_INVALID_DEVID,
    /** 08 - No connection to WWAN device */
    eQCWWAN_ERR_NO_CONNECTION,
    /** 09 - Unable to obtain QMI interace */
    eQCWWAN_ERR_QMI_IFACE,
    /** 10 - Unable to connect to QMI interface */
    eQCWWAN_ERR_QMI_CONNECT,
    /** 11 - Unable to schedule QMI request */
    eQCWWAN_ERR_QMI_REQ_SCH,
    /** 12 - Error sending QMI request */
    eQCWWAN_ERR_QMI_REQ,
    /** 13 - Error receiving QMI response */
    eQCWWAN_ERR_QMI_RSP,
    /** 14 - Timeout while sending QMI request */
    eQCWWAN_ERR_QMI_REQ_TO,
    /** 15 - Timeout while receiving QMI response */
    eQCWWAN_ERR_QMI_RSP_TO,
    /** 16 - Malformed QMI response received */
    eQCWWAN_ERR_MALFORMED_QMI_RSP,
    /** 17 - Invalid QMI response received */
    eQCWWAN_ERR_INVALID_QMI_RSP,
    /** 18 - Invalid file path */
    eQCWWAN_ERR_INVALID_FILE,
    /** 19 - Unable to open file */
    eQCWWAN_ERR_FILE_OPEN,
    /** 20 - Unable to copy file */
    eQCWWAN_ERR_FILE_COPY,
    /** 27 - Unable to set WWAN device offline */
    eQCWWAN_ERR_OFFLINE = 27,
    /** 28 - Unable to reset WWAN device */
    eQCWWAN_ERR_RESET,
    /** 29 - No available signal */
    eQCWWAN_ERR_NO_SIGNAL,
    /** 30 - Multiple WWAN devices detected */
    eQCWWAN_ERR_MULTIPLE_DEVICES,
    /** 31 - Error interfacing to driver */
    eQCWWAN_ERR_DRIVER,
    /** 32 - No cancelable operation is pending */
    eQCWWAN_ERR_NO_CANCELABLE_OP,
    /** 33- Error canceling outstanding operation */
    eQCWWAN_ERR_CANCEL_OP,

    /** End of SLQS SDK specific error codes */
    eQCWWAN_ERR_ENUM_END,

    /** 1000 - This is not an error code but the offset from which
     *  mapped QMI error codes start from
     */
    eQCWWAN_ERR_QMI_OFFSET = 1000,

    /** 1001 - Malformed or Corrupted QMI msg */
    eQCWWAN_ERR_QMI_MALFORMED_MSG = 1001,
    /** 1002 - Device could not allocate memory for QMI Resp */
    eQCWWAN_ERR_QMI_NO_MEMORY,
    /** 1003 - Unexpected error occurred during processing */
    eQCWWAN_ERR_QMI_INTERNAL,
    /** 1004 - Processing aborted */
    eQCWWAN_ERR_QMI_ABORTED,
    /** 1005 - QMI client IDs have been exhausted */
    eQCWWAN_ERR_QMI_CLIENT_IDS_EXHAUSTED,
    /** 1006 - Unable to abort QMI transaction */
    eQCWWAN_ERR_QMI_UNABORTABLE_TRANSACTION,
    /** 1007 - Invalid QMI client ID */
    eQCWWAN_ERR_QMI_INVALID_CLIENT_ID,
    /** 1008 - No thresholds were provided */
    eQCWWAN_ERR_QMI_NO_THRESHOLDS,
    /** 1009 - Invalid Handle provided in the QMI request */
    eQCWWAN_ERR_QMI_INVALID_HANDLE,
    /** 1010 - Profile specified is invalid */
    eQCWWAN_ERR_QMI_INVALID_PROFILE,
    /** 1011 - Invalid PIN ID specified */
    eQCWWAN_ERR_QMI_INVALID_PINID,
    /** 1012 - Incorrect PIN ID specified */
    eQCWWAN_ERR_QMI_INCORRECT_PIN,
    /** 1013 - No network found */
    eQCWWAN_ERR_QMI_NO_NETWORK_FOUND,
    /** 1014 - Call failed */
    eQCWWAN_ERR_QMI_CALL_FAILED,
    /** 1015 - Device is not in a call */
    eQCWWAN_ERR_QMI_OUT_OF_CALL,
    /** 1016 - Requested information element not provisioned on device */
    eQCWWAN_ERR_QMI_NOT_PROVISIONED,
    /** 1017 - Mandatory QMI TLV not provided */
    eQCWWAN_ERR_QMI_MISSING_ARG,
    /** 1019 - Arg passed in QMI TLV larger than available storage in device */
    eQCWWAN_ERR_QMI_ARG_TOO_LONG = 1019,
    /**  1022 - Invalid TX ID specified */
    eQCWWAN_ERR_QMI_INVALID_TX_ID = 1022,
    /** 1023 - Device currently in a call */
    eQCWWAN_ERR_QMI_DEVICE_IN_USE,
    /** 1024 - The selected operation is not supported by the network */
    eQCWWAN_ERR_QMI_OP_NETWORK_UNSUPPORTED,
    /** 1025 - The selected operation is not supported by the device */
    eQCWWAN_ERR_QMI_OP_DEVICE_UNSUPPORTED,
    /** 1026 - Requested operation would have no effect */
    eQCWWAN_ERR_QMI_NO_EFFECT,
    /** 1027 - No space for a profile is available */
    eQCWWAN_ERR_QMI_NO_FREE_PROFILE,
    /** 1028 - Invalid PDP type specified */
    eQCWWAN_ERR_QMI_INVALID_PDP_TYPE,
    /** 1029 - Invalid technology preference specified */
    eQCWWAN_ERR_QMI_INVALID_TECH_PREF,
    /** 1030 - Invalid profile type specified */
    eQCWWAN_ERR_QMI_INVALID_PROFILE_TYPE,
    /** 1031 - Invalid service type specified */
    eQCWWAN_ERR_QMI_INVALID_SERVICE_TYPE,
    /** 1032 - Invalid register action specified */
    eQCWWAN_ERR_QMI_INVALID_REGISTER_ACTION,
    /** 1033 - Invalid PS attach/detach action specified */
    eQCWWAN_ERR_QMI_INVALID_PS_ATTACH_ACTION,
    /** 1034 - Authentication of supplied information element failed */
    eQCWWAN_ERR_QMI_AUTHENTICATION_FAILED,
    /** 1035 - PIN is blocked; an unblock operation needs to be issued */
    eQCWWAN_ERR_QMI_PIN_BLOCKED,
    /** 1036 - PIN is permanently blocked; the UIM is unusable */
    eQCWWAN_ERR_QMI_PIN_PERM_BLOCKED,
    /** 1037 - UIM initialization has not completed */
    eQCWWAN_ERR_QMI_SIM_NOT_INITIALIZED,
    /** 1038 - Max QOS requests are used */
    eQCWWAN_ERR_QMI_MAX_QOS_REQUESTS_IN_USE,
    /** 1039 - The Flow filter is incorrect */
    eQCWWAN_ERR_QMI_INCORRECT_FLOW_FILTER,
    /** 1040 - Network unaware of the QOS requested */
    eQCWWAN_ERR_QMI_NETWORK_QOS_UNAWARE,
    /** 1041 - Invalid QOS ID */
    eQCWWAN_ERR_QMI_INVALID_ID,
    /** 1041 - Invalid QOS ID */
    eQCWWAN_ERR_QMI_INVALID_QOS_ID = eQCWWAN_ERR_QMI_INVALID_ID,
    /** 1042 - The request number is not supported */
    eQCWWAN_ERR_QMI_REQUESTED_NUM_UNSUPPORTED,
    /** 1043 - Unable to find the interface */
    eQCWWAN_ERR_QMI_INTERFACE_NOT_FOUND,
    /** 1044-  Flow suspended */
    eQCWWAN_ERR_QMI_FLOW_SUSPENDED,
    /** 1045 - Data format is invalid */
    eQCWWAN_ERR_QMI_INVALID_DATA_FORMAT,
    /** 1046 - General error */
    eQCWWAN_ERR_QMI_GENERAL,
    /** 1047 - Unknown error */
    eQCWWAN_ERR_QMI_UNKNOWN,
    /** 1048 - A specified argument is invalid */
    eQCWWAN_ERR_QMI_INVALID_ARG,
    /** 1049 - A specified index is invalid */
    eQCWWAN_ERR_QMI_INVALID_INDEX,
    /** 1050 - No information element exists at specified memory designation */
    eQCWWAN_ERR_QMI_NO_ENTRY,
    /** 1051 - The memory storage specified in the request is full */
    eQCWWAN_ERR_QMI_DEVICE_STORAGE_FULL,
    /** 1052 - Device not in a ready state */
    eQCWWAN_ERR_QMI_DEVICE_NOT_READY,
    /** 1053 - Network not in a ready state */
    eQCWWAN_ERR_QMI_NETWORK_NOT_READY,
    /** 1054 - Error provided in SMS cause code */
    eQCWWAN_ERR_QMI_CAUSE_CODE,
    /** 1055 - The message could not be sent */
    eQCWWAN_ERR_QMI_MESSAGE_NOT_SENT,
    /** 1056 - The message could not be delivered */
    eQCWWAN_ERR_QMI_MESSAGE_DELIVERY_FAILURE,
    /** 1057 - The message ID specified for the message is invalid */
    eQCWWAN_ERR_QMI_INVALID_MESSAGE_ID,
    /** 1058 - The message is not encoded properly */
    eQCWWAN_ERR_QMI_ENCODING,
    /** 1059 - Maximum number of authentication failures has been reached */
    eQCWWAN_ERR_QMI_AUTHENTICATION_LOCK,
    /** 1060 - Operating mode transition from the current mode is invalid */
    eQCWWAN_ERR_QMI_INVALID_TRANSITION,
    /** 1061 - The intercace is not muticast */
    eQCWWAN_ERR_QMI_NOT_A_MCAST_IFACE,
    /** 1062 - Maximum requests in use */
    eQCWWAN_ERR_QMI_MAX_MCAST_REQUESTS_IN_USE,
    /** 1063 - Invalid muticast handle */
    eQCWWAN_ERR_QMI_INVALID_MCAST_HANDLE,
    /** 1064 - Invalid IP family preference */
    eQCWWAN_ERR_QMI_INVALID_IP_FAMILY_PREF,
    /** 1065 - No tracking session has been started */
    eQCWWAN_ERR_QMI_SESSION_INACTIVE,
    /** 1066 - Current session does not allow this operation */
    eQCWWAN_ERR_QMI_SESSION_INVALID,
    /** 1067 - Current tracking session not started by this QMI control point */
    eQCWWAN_ERR_QMI_SESSION_OWNERSHIP,
    /** 1068 - Device GPS service resources insufficient for request */
    eQCWWAN_ERR_QMI_INSUFFICIENT_RESOURCES,
    /** 1069 - Device GPS service disabled */
    eQCWWAN_ERR_QMI_DISABLED,
    /** 1070 - Invalid operation specified */
    eQCWWAN_ERR_QMI_INVALID_OPERATION,
    /** 1071 - Invalid/unknown QMI command specified */
    eQCWWAN_ERR_QMI_INVALID_QMI_CMD,
    /** 1072 - Message contains TPDU type that cannot be read as raw message */
    eQCWWAN_ERR_QMI_TPDU_TYPE,
    /** 1073 - The SMSC address specified is invalid */
    eQCWWAN_ERR_QMI_SMSC_ADDR,
    /** 1074 - Information element is unavailable at this point */
    eQCWWAN_ERR_QMI_INFO_UNAVAILABLE,
    /** 1075 - Segment size too large */
    eQCWWAN_ERR_QMI_SEGMENT_TOO_LONG,
    /** 1076 - Segment order is incorrect */
    eQCWWAN_ERR_QMI_SEGMENT_ORDER,
    /** 1077 - Bundling not supported */
    eQCWWAN_ERR_QMI_BUNDLING_NOT_SUPPORTED,
    /** 1078 - The operation failed partially */
    eQCWWAN_ERR_QMI_OP_PARTIAL_FAILURE,
    /** 1079 - Policy mismatch */
    eQCWWAN_ERR_QMI_POLICY_MISMATCH,
    /** 1080 - SIM file not found */
    eQCWWAN_ERR_QMI_SIM_FILE_NOT_FOUND,
    /** 1081 - Extended internal error */
    eQCWWAN_ERR_QMI_EXTENDED_INTERNAL,
    /** 1082 - Access to a required entity is not available */
    eQCWWAN_ERR_QMI_ACCESS_DENIED,
    /** 1083 - Selected operating mode is invalid with current hardware setting */
    eQCWWAN_ERR_QMI_HARDWARE_RESTRICTED,
    /** 1084 - ACK not sent */
    eQCWWAN_ERR_QMI_ACK_NOT_SENT,
    /** 1084 - Inject a timeout for the request */
    eQCWWAN_ERR_QMI_INJECT_TIMEOUT,

    /** 1090 - Incompatible state */
    eQCWWAN_ERR_QMI_INCOMPATIBLE_STATE = 1090,
    /** 1091 - FDN Restrict */
    eQCWWAN_ERR_QMI_FDN_RESTRICT,
    /** 1092 - SUPS failure cause */
    eQCWWAN_ERR_QMI_SUPS_FAILURE_CAUSE,
    /** 1093 - No Radio */
    eQCWWAN_ERR_QMI_NO_RADIO,
    /** 1094 - Not Supported */
    eQCWWAN_ERR_QMI_NOT_SUPPORTED,
    /** 1095 - No Subscription */
    eQCWWAN_ERR_QMI_NO_SUBSCRIPTION,
    /** 1096 - Card call control failed */
    eQCWWAN_ERR_QMI_CARD_CALL_CONTROL_FAILED,
    /** 1097 - Network Aborted */
    eQCWWAN_ERR_QMI_NETWORK_ABORTED,
    /** 1098 - Open Error  */
    eQCWWAN_ERR_QMI_MSG_BLOCKED,
    /** Error - End of QMI specific defines */
    eQCWWAN_ERR_QMI_MAX,

    /** <b>Vendor defines - Connection Manager error codes</b> */
    eQCWWAN_ERR_SWICM_START = 0xE000,
    /** 0xE001 - The API is yet to be implemented */
    eQCWWAN_ERR_SWICM_NOT_IMPLEMENTED,
    /** 0xE002 - The service is not supported */
    eQCWWAN_ERR_SWICM_QMI_SVC_NOT_SUPPORTED,
    /** 0xE003 - The client is not supported */
    eQCWWAN_ERR_SWICM_QMI_CLNT_NOT_SUPPORTED,
    /** 0xE004 - API Timeout */
    eQCWWAN_ERR_SWICM_TIMEOUT,
    /** 0xE005 - The communication socket is in use */
    eQCWWAN_ERR_SWICM_SOCKET_IN_USE,
    /** 0xE006 - SLQS API and SDK version mismatch */
    eQCWWAN_ERR_SWICM_AM_VERS_ERROR,
    /** 0xE007 - Failed to kill SDK process */
    eQCWWAN_ERR_SWICM_FAILED_TO_KILL_SDK_PROCESS,
    /** 0xE008 - Call in progress */
    eQCWWAN_ERR_SWICM_CALL_IN_PROGRESS,
    /** 0xE009 - IPV4 and IPV6 is down */
    eQCWWAN_ERR_SWICM_V4DWN_V6DWN,
    /** 0xE00A - IPV4 is down and IPV6 is up */
    eQCWWAN_ERR_SWICM_V4DWN_V6UP,
    /** 0xE00B - IPV4 is up and IPV6 is up */
    eQCWWAN_ERR_SWICM_V4UP_V6DWN,
    /** 0xE00C - IPV4 and IPV6 is up */
    eQCWWAN_ERR_SWICM_V4UP_V6UP,
    /** 0xE00D - End of connection manager specific codes */
    eQCWWAN_ERR_SWICM_END,

    /** <b>Vendor defines - SMS Error codes</b> */
    eQCWWAN_ERR_SWISMS_START = 0xE100,
    /** 0xE101 - SMS message length is long */
    eQCWWAN_ERR_SWISMS_MSG_LEN_TOO_LONG,
    /** 0xE102 - The SMS message is corrupted (encoding wrong) */
    eQCWWAN_ERR_SWISMS_MSG_CORRUPTED,
    /** 0xE103 - The SMS number is corrupted (incorrect number) */
    eQCWWAN_ERR_SWISMS_SMSC_NUM_CORRUPTED,
    /** 0xE104 - The SMS bearer data is not available */
    eQCWWAN_ERR_SWISMS_BEARER_DATA_NOT_FOUND,
    eQCWWAN_ERR_SWISM_END,

    /** <b>Vendor defines - Image Management error codes</b> */
    eQCWWAN_ERR_SWIIM_START = 0xE800,
    /** 0xE801 - Invalid directory path */
    eQCWWAN_ERR_SWIIM_INVALID_PATH,
    /** 0xE802 - Unable to open the directory */
    eQCWWAN_ERR_SWIIM_OPENING_DIR,
    /** 0xE803 - No Firmware image present in the path */
    eQCWWAN_ERR_SWIIM_FILE_NOT_FOUND,
    /** 0xE804 - Unable to open the file */
    eQCWWAN_ERR_SWIIM_OPENING_FILE,
    /** 0xE805 - Firmware image is corrupted */
    eQCWWAN_ERR_SWIIM_CORRUPTED_FW_IMAGE,
/* RILSTART */
    /** 0xE806 - No Firmware image download needed */
    eQCWWAN_ERR_SWIIM_FIRMWARE_NOT_DOWNLOADED,
/* RILSTOP */
    eQCWWAN_ERR_SWIIM_END,

    /** <b>Vendor defines - Device Connectivity error codes</b> */
    eQCWWAN_ERR_SWIDCS_START = 0xE900,
    /** 0xE901 - IO Control error */
    eQCWWAN_ERR_SWIDCS_IOCTL_ERR,
    /** 0xE902 - file open/read/write error */
    eQCWWAN_ERR_SWIDCS_FILEIO_ERR,
    /** 0xE903 - The device is not found */
   eQCWWAN_ERR_SWIDCS_DEVNODE_NOT_FOUND,
    /** 0xE903 - Application is disconnected from SDK */
    eQCWWAN_ERR_SWIDCS_APP_DISCONNECTED,
    eQCWWAN_ERR_SWIDCS_END,

    /** <b>QMI errors related to CAT</b> */
    eQCWWAN_ERR_QMI_CAT_START = 0xF3E9,
    /** 62441 - CAT event registration failed */
    eQCWWAN_ERR_QMI_EVENT_REG_FAILED = eQCWWAN_ERR_QMI_CAT_START,
    /** 62442 - Invalid terminal response */
    eQCWWAN_ERR_QMI_INVALID_TERMINAL_RSP,
    /** 62443 - Invalid envelope command */
    eQCWWAN_ERR_QMI_INVALID_ENVELOPE_CMD,
    /** 62444 - Card busy response for envelope command */
    eQCWWAN_ERR_QMI_CARD_BUSY_RSP,
    /** 62445 - Envelope command failure */
    eQCWWAN_ERR_QMI_ENVELOPE_CMD_FAILURE,
    eQCWWAN_ERR_QMI_CAT_END = eQCWWAN_ERR_QMI_ENVELOPE_CMD_FAILURE,

    /** 0xFFFF - Not an error, represent the end of QMI errors  */
    eQCWWAN_ERR_QMI_WIDTH = 0xFFFF
};

/**
 * <b>WDS DS profile extented error codes</b>
 */
enum qm_wds_ds_profile_extended_err_codes
{
    /** 1 - General Failure */
    eWDS_ERR_PROFILE_REG_RESULT_FAIL = 1,

    /** 2 - The request contains an invalid profile handle */
    eWDS_ERR_PROFILE_REG_RESULT_ERR_INVAL_HNDL,

    /** 3 - An invalid operation was requested. */
    eWDS_ERR_PROFILE_REG_RESULT_ERR_INVAL_OP,

    /** 4 - The request contains an invalid technology type */
    eWDS_ERR_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_TYPE,

    /** 5 - The request contains an invalid profile number */
    eWDS_ERR_PROFILE_REG_RESULT_ERR_INVAL_PROFILE_NUM,

    /** 6 - The request contains an invalid profile identifier */
    eWDS_ERR_PROFILE_REG_RESULT_ERR_INVAL_IDENT,

    /** 7 - The request contains an invalid argument other than profile number and
     * profile identifier received.
     */
    eWDS_ERR_PROFILE_REG_RESULT_ERR_INVAL,

    /** 8 - Profile registry has not been initialized yet */
    eWDS_ERR_PROFILE_REG_RESULT_ERR_LIB_NOT_INITED,

    /** 9 - The request contains a parameter with invalid length. */
    eWDS_ERR_PROFILE_REG_RESULT_ERR_LEN_INVALID,

    /** 10 - End of the profile list was reached while searching for the requested
     * profile.
     */
    eWDS_ERR_PROFILE_REG_RESULT_LIST_END,

    /** 11 - The request contains an invalid subscrition identifier. */
    eWDS_ERR_PROFILE_REG_RESULT_ERR_INVAL_SUBS_ID,

    /** 12 - The request contains an invalid profile family. */
    eWDS_ERR_PROFILE_REG_INVAL_PROFILE_FAMILY,

    /** 1001 - The request contains an invalid 3GPP profile family. */
    eWDS_ERR_PROFILE_REG_3GPP_INVAL_PROFILE_FAMILY = 1001,

    /** 1002 - An error was encountered while accessing the 3GPP profiles.*/
    eWDS_ERR_PROFILE_REG_3GPP_ACCESS_ERR,

    /** 1003 - The given 3GPP profile doesn't have a valid context. */
    eWDS_ERR_PROFILE_REG_3GPP_CONTEXT_NOT_DEFINED,

    /** 1004 - The given 3GPP profile is marked invalid. */
    eWDS_ERR_PROFILE_REG_3GPP_VALID_FLAG_NOT_SET,

    /** 1005 - The given 3GPP profile is marked read-only. */
    eWDS_ERR_PROFILE_REG_3GPP_READ_ONLY_FLAG_SET,

    /** 1006 - Creation of a new 3GPP profile failed because the limit of 16
     * profiles has already been reached.
     */
    eWDS_ERR_PROFILE_REG_3GPP_ERR_OUT_OF_PROFILES,

    /** 1101 - An invalid profile identifier was received as part of the 3GPP2
     * profile modification request.
     */
    eWDS_ERR_PROFILE_REG_3GPP2_ERR_INVALID_IDENT_FOR_PROFILE = 1101,

    eWDS_ERR_PROFILE_REG_END
};

/*----------------------
  DMS error codes
------------------------*/
/*----------------------
  NAS error codes
------------------------*/
/*----------------------
  QOS error codes
------------------------*/
/*----------------------
  WMS error codes
------------------------*/
/*----------------------
  PDS error codes
------------------------*/
/*----------------------
  AUT error codes
------------------------*/
/*----------------------
  UIM error codes
------------------------*/
/*----------------------
  CAT error codes
------------------------*/
/*----------------------
  RMS error codes
------------------------*/
/*----------------------
  OMA error codes
------------------------*/
/*----------------------
  PBM error codes
------------------------*/
/*----------------------
  DCS error codes
------------------------*/
/*-----------------------
  SWI-OMA error codes
------------------------*/
/*----------------------
  FMS error codes
------------------------*/
/*----------------------
  SAR error codes
------------------------*/
/*----------------------
  VOICE error codes
------------------------*/
