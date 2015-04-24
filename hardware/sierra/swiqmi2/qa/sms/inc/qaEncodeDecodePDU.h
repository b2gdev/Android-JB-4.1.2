/*
 * \file    qaMakePdu.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaMakePdu.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/****************************************************************
*                       Defines
****************************************************************/
#define ASC2HEX(x) ( ((x >= '0') && (x <= '9')) ? \
                                      (x - '0') : \
                     ((x >= 'A') && (x <= 'F')) ? \
                                 (x - 'A' + 10) : \
                     ((x >= 'a') && (x <= 'f')) ? \
                             (x - 'a' + 10) : 0 )

#define HEX(x, i) (BYTE)(((x[i/2] >> ((i & 1) ? 0 : 4)) & 0x0F))
#define MAXIMUM_PDU       250
#define NUMBERFORMAT      145
#define NATIONALFORMAT    129
#define ENTER_KEY         0x0A
#define nMaxStrLen        0xFF
#define DEV_NODE_SZ       256
#define DEV_KEY_SZ        16
#define MAXSMS_SIZE        160
#define ENTER_KEY         0x0A
#define ESCAPE_KEY        0x1B
#define OPTION_LEN          4
#define NUMBERFORMAT      145
#define NATIONALFORMAT    129
#define STORAGEINDEX_UIM  0
#define STORAGEINDEX_NV   1
#define MAXIMUM_PDU       250
#define MAXTEXT           2048
#define UMTS              0x01
#define CDMA              0x00

/******************************************************************************
CDMA SERVICE MACRO MASK_B

DESCRIPTION
   Masks the number of bits give by length starting at the given offset.
   Unlike MASK and MASK_AND_SHIFT, this macro only creates that mask, it
   does not operate on the data buffer.
******************************************************************************/
#define MASK_B(offset, len) \
  ((0xff >> offset) & (0xff << (8 - (offset + len))))

#define MAX_SMS_MSG_LEN                             160

/* PROTOCOL DEFINES */
#define POINT_TO_POINT_MSG                          0

#define TELESERVICE_IDENTIFIER                      0
#define TELESERVICE_ID_VALUE                        4098
#define TELESERVICE_HDR_LEN                         2

#define SERVICE_CATEGORY_IDENIFIER                  1
#define SERVICE_CATEGORY_HDR_LEN                    2

#define ORIGADDR_IDENTIFIER                         2
#define ORIGADDR_HDR_LEN                            7

#define ORIGSUBADDR_IDENTIFIER                      3

#define DESTADDR_IDENTIFIER                         4
#define DESTADDR_HDR_LEN                            7

#define DESTSUBADDR_IDENTIFIER                      5

#define BEARER_REPLY_IDENTIFIER                     6
#define BEARER_REPLY_HDR_LEN                        1

#define CAUSECODES_IDENTIFIER                       7

#define BEARER_DATA_IDENTIFIER                      8

/* Sub indetifiers for Bearer Data */
#define MSG_SUB_IDENTIFIER_ID                       0
#define MSG_SUB_IDENTIFIER_HDR_LEN                  3

#define USER_DATA_SUB_IDENTIFIER                    1
#define USER_DATA_MSG_ENCODING_7BIT_ASCII           2

#define USER_RESP_CODE_SUB_IDENTIFIER               2

#define MSG_TIMESTAMP_SUB_INDETIFIER                3
#define MSG_TIMESTAMP_SUB_INDETIFIER_HDR_LEN        6

#define ABS_VAL_PERIOD_SUB_IDENTIFIER               4
#define ABS_VAL_PERIOD_SUB_IDENTIFIER_HDR_LEN       6

#define REL_VAL_PERIOD_SUB_IDENTIFIER               5
#define REL_VAL_PERIOD_SUB_IDENTIFIER_HDR_LEN       1

#define DEF_DELIVERY_TIME_ABS_SUB_IDENTIFIER        6

#define DEF_DELIVERY_TIME_REL_SUB_IDENTIFIER        7

#define PRIORIY_SUB_IDENTIFIER                      8
#define PRIORIY_SUB_IDENTIFIER_HDR_LEN              1

#define PRIVACY_SUB_IDENTIFIER                      9

#define REPLY_OPTION_SUB_IDENTIFIER                 10
#define REPLY_OPTION_SUB_IDENTIFIERHDR_LEN          1

#define NUM_OF_MSGS_SUB_IDENTIFIER                  11

#define ALERT_MSG_DELIVERY_SUB_IDENTIFIER           12

#define LANGUAGE_INDICATOR_SUB_INDETIFIER           13
#define LANGUAGE_INDICATOR_SUB_INDETIFIER_HDR_LEN   1

#define CALLBACK_NUM_SUB_IDENTIFIER                 14

#define MSG_DISP_MODE_SUB_IDENTIFIER                15

#define MULTI_ENCODING_USER_DATA_SUB_IDENTIFIER     16

#define MSG_DEPOSIT_INDEX_SUB_IDENTIFIER            17

#define SERVICE_CAT_PROGRAM_DATA_SUB_IDENTIFIER     18

#define SERVICE_CAT_PROGRAM_RESULT_SUB_IDENTIFIER   19

#define MSG_STATUS_SUB_IDENTIFIER                   20

/* CDMA SMS decoding defines */
#define  MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define _T(x) L ## x

/**********************************************************
MACRO bitsize

DESCRIPTION
   Computes size in bits of the specified data type.
**********************************************************/
#define bitsize(type) (sizeof(type) * 8)
#define MAX_SMSI_TL_ADDRESS  50 // Maximum SMS address length
#define MAX_SMSI_CALL_BACK   32 // Maximum SMS callback length

/**********************************************************
MACRO copymask

DESCRIPTION
   Creates a mask of bits sized to the number of bits in the given type.
**********************************************************/
#define copymask(type) ((0xffffffff) >> (32 - bitsize(type)))

/******************************************************************************
MACRO MASK

DESCRIPTION
   Masks the bits in data at the given offset for given number of width bits.
******************************************************************************/
#define MASK(width, offset, data) \
    /*lint -e701 shift left  of signed quantity  */  \
    /*lint -e702 shift right of signed quantity  */  \
    /*lint -e572 Excessive shift value           */  \
    /*lint -e573 Signed-unsigned mix with divide */  \
    /*lint -e506 Constant value boolean          */  \
    /*lint -e649 Sign fill during constant shift */  \
                                                     \
   (((width) == bitsize(data)) ? (data) :   \
   ((((copymask(data) << (bitsize(data) - ((width) % bitsize(data)))) \
    & copymask(data)) >>  (offset)) & (data))) \
                     \
    /*lint +e701 */  \
    /*lint +e702 */  \
    /*lint +e572 */  \
    /*lint +e573 */  \
    /*lint +e506 */  \
    /*lint +e649 */


/******************************************************************************
MACRO MASK_AND_SHIFT

DESCRIPTION
   Same as the macro MASK except also shifts the data in the result by the
   given number of shift bits.
*******************************************************************************/
#define MASK_AND_SHIFT(width, offset, shift, data)  \
    /*lint -e504 Unusual shifter value */  \
                  ((((signed) (shift)) < 0) ?       \
                    MASK((width), (offset), (data)) << -(shift) :  \
                    MASK((width), (offset), (data)) >>  (((unsigned) (shift)))) \
    /*lint +e504 */

/*****************************************************************************/


/*****************************************************************************
*                       Enums
******************************************************************************/
/*
 * An enumeration of QCWWAN_TYPE_SMS_ParamMask.
 * This identifies the meanings of the bits in the
 * QCWWAN_STRUCT_SMS_CdmaHeader parameter mask of available fields.
 *
 */
typedef enum
{
    QCWWAN_SMS_PARAMASK_NO_PARAMS,
    QCWWAN_SMS_PARAMASK_BEARER_REPLY,
    QCWWAN_SMS_PARAMASK_VALID_ABS,
    QCWWAN_SMS_PARAMASK_VALID_REL     = 0x00000004,
    QCWWAN_SMS_PARAMASK_DEFER_ABS     = 0x00000008,
    QCWWAN_SMS_PARAMASK_DEFER_REL     = 0x00000010,
    QCWWAN_SMS_PARAMASK_USER_RESP     = 0x00000020,
    QCWWAN_SMS_PARAMASK_ADDRESS       = 0x00000040,
    QCWWAN_SMS_PARAMASK_SUBADDR       = 0x00000080,
    QCWWAN_SMS_PARAMASK_USER_DATA     = 0x00000100,
    QCWWAN_SMS_PARAMASK_PRIORITY      = 0x00000200,
    QCWWAN_SMS_PARAMASK_PRIVACY       = 0x00000400,
    QCWWAN_SMS_PARAMASK_REPLY_OPT     = 0x00000800,
    QCWWAN_SMS_PARAMASK_NUM_OF_MSG    = 0x00001000,
    QCWWAN_SMS_PARAMASK_ALERT         = 0x00002000,
    QCWWAN_SMS_PARAMASK_LANGUAGE      = 0x00004000,
    QCWWAN_SMS_PARAMASK_CALL_BACK     = 0x00008000,
    QCWWAN_SMS_PARAMASK_MC_TIME_STAMP = 0x00010000,
    QCWWAN_SMS_PARAMASK_DISP_MODE     = 0x00020000
}QCWWAN_TYPE_SMS_ParamMask;

/*
 * An enumeration of QCWWAN_TYPE_SMS_MsgCategory.
 *
 */
typedef enum
{
    QCWWAN_SMS_MSGCAT_POINT_TO_POINT,
    QCWWAN_SMS_MSGCAT_BROADCAST_MSG,
    QCWWAN_SMS_MSGCAT_ACK_MSG
}QCWWAN_TYPE_SMS_MsgCategory;

/*
 * An enumeration of QCWWAN_TYPE_SMS_DataEncodeType -
 * SMS Data Encoding enumeration. This enumeration identifies
 * the possible data encoding methods used on the SMS message body.
 *
 */
typedef enum
{
    QCWWAN_SMS_DATAENCODE_8bitAscii = 0,
    QCWWAN_SMS_DATAENCODE_IS91EP,
    QCWWAN_SMS_DATAENCODE_7bitAscii,
    QCWWAN_SMS_DATAENCODE_IA5,
    QCWWAN_SMS_DATAENCODE_Unicode,
    QCWWAN_SMS_DATAENCODE_Shift_Jis,
    QCWWAN_SMS_DATAENCODE_Korean,
    QCWWAN_SMS_DATAENCODE_LatinHebrew,
    QCWWAN_SMS_DATAENCODE_Latin,
    QCWWAN_SMS_DATAENCODE_GSM7BitDefault,
   QCWWAN_SMS_DATAENCODE_MAX32 = 0x10000000
}QCWWAN_TYPE_SMS_DataEncodeType;

/*
 * An enumeration of QCWWAN_TYPE_SMS_MsgIdType
 * - SMS Message ID Type enumeration.Defines the SMS Message ID Type.
 *
 */
typedef enum
{
    QCWWAN_SMS_MSGIDTYPE_NOTUSED,
    QCWWAN_SMS_MSGIDTYPE_DELIVER,
    QCWWAN_SMS_MSGIDTYPE_SUBMIT,
    QCWWAN_SMS_MSGIDTYPE_CANCEL,
    QCWWAN_SMS_MSGIDTYPE_DELIVERY_ACK,
    QCWWAN_SMS_MSGIDTYPE_USER_ACK
}QCWWAN_TYPE_SMS_MsgIdType;

/*
 *
 * An enumeration of QCWWAN_TYPE_SMS_Teleservice
 * - SMS Message Teleservice Type enumeration. Defines the SMS teleservice type.
 *   Refer to TIA/EIA-41-D Table 175 for further information.
 *
 */
typedef enum
{
    QCWWAN_SMS_TELESRV_CELL_PAGING,
    QCWWAN_SMS_TELESRV_CELL_MESSAGING,
    QCWWAN_SMS_TELESRV_VOICE_MAIL_NOT,
    QCWWAN_SMS_TELESRV_CLI_ORDER,
    QCWWAN_SMS_TELESRV_VOICE_IND,
    QCWWAN_SMS_TELESRV_SHORT_MSG,
    QCWWAN_SMS_TELESRV_ALERT_INFO,
    QCWWAN_SMS_TELESRV_WAP
}QCWWAN_TYPE_SMS_Teleservice;

/*
 *
 * An enumeration of QCWWAN_TYPE_SMS_Language - SMS Language type enumeration.
 * Defines the language used in the message body
 *
 */
typedef enum
{
    QCWWAN_SMS_LANGUAGE_UNSPECIFIED,
    QCWWAN_SMS_LANGUAGE_ENGLISH,
    QCWWAN_SMS_LANGUAGE_FRENCH,
    QCWWAN_SMS_LANGUAGE_SPANISH,
    QCWWAN_SMS_LANGUAGE_JAPANESE,
    QCWWAN_SMS_LANGUAGE_KOREAN,
    QCWWAN_SMS_LANGUAGE_CHINESE,
    QCWWAN_SMS_LANGUAGE_HEBREW
}QCWWAN_TYPE_SMS_Language;

/*
 * An enumeration of QCWWAN_TYPE_SMS_Priority - SMS Message Priority enumeration
 * Defines the priority of the SMS Message.
 *
 */
typedef enum
{
        QCWWAN_SMS_PRIORITY_NORMAL,
        QCWWAN_SMS_PRIORITY_INTERACTIVE,
        QCWWAN_SMS_PRIORITY_URGENT,
        QCWWAN_SMS_PRIORITY_EMERGENCY
} QCWWAN_TYPE_SMS_Priority;

/*
 * An enumeration of QCWWAN_TYPE_SMS_Privacy - SMS Message Privacy enumeration
 * Defines the privacy level of the SMS message
 *
 */
typedef enum
{
    QCWWAN_SMS_PRIVACY_NOT_RESTR,
    QCWWAN_SMS_PRIVACY_RESTRICTED,
    QCWWAN_SMS_PRIVACY_CONFIDENTIAL,
    QCWWAN_SMS_PRIVACY_SECRET
}QCWWAN_TYPE_SMS_Privacy;

/*
 * An enumeration of QCWWAN_TYPE_SMS_MsgDispMode - SMS Message
 * Display Mode enumeration. Specifies how the SMS message should be displayed
 *
 */
typedef enum
{
        QCWWAN_SMS_MSGDISPMODE_IMMEDIATE,
        QCWWAN_SMS_MSGDISPMODE_DEFAULT,
        QCWWAN_SMS_MSGDISPMODE_USER_INVOKE
}QCWWAN_TYPE_SMS_MsgDispMode;

/*
 * All function calls will return one of the return codes. In the event you
 * receive a return code not enumerated in this list, please check for
 * available SDK updates from Sierra Wireless Inc.
 */
typedef enum
{
    /*! The function is defined for other technologies and is not supported on
        this particular product. */
    eQCWWAN_APP_ERR_NOT_SUP = 1,
    eQCWWAN_APP_ERR_BUFFER_SZ,

    /*! The function failed to execute correctly.  Any return data should be
        ignored. A retry may yield better results. */
    eQCWWAN_APP_ERR_FAILED = 5,

    /*! The function is returning data without detecting a processing fault,
        but the data does not match the expected range or format.  A retry of
        the function may yield better results. */
    eQCWWAN_APP_ERR_UNEXPECTED_RESP,
    eQCWWAN_APP_ERR_IMPOSSIBLE_ENCODING
} eQCWWAN_APP_ERR;

/****************************************************************
*                       Data Structures
****************************************************************/
/* Device information structure */
typedef struct device_info_param{
   CHAR deviceNode[DEV_NODE_SZ];
   CHAR deviceKey[DEV_KEY_SZ];
}device_info_t;

typedef struct messageList{
    ULONG messageIndex;
    BYTE  messageTag;
} messageLists;

typedef struct indexMap{
    CHAR  *memoryType;
    ULONG originalIndex;
    ULONG mappedIndex;
    ULONG messageTag;
} indexMapMessage;

/*
 * QCWWAN_STRUCT_SMS_CdmaHeader - CDMA SMS Header
 *
 * this structure defines the header of incoming
 * SMS messages. Most of the data items are
 * defined in the CDMA specification and are not
 * explained here. The enumerated types are
 * described following this structure.
 *
 * sizeStruct    - size of this structure
 *
 * eParamMask    - Parameter Mask indicating which of the below fields
 *                are valid.  See QCWWAN_TYPE_SMS_ParamMask.
 *
 * eMsgCategory  - Message Category
 *                 See QCWWAN_TYPE_SMS_MsgCategory
 *
 * eTeleservice  - Message Teleservice
 *                 See QCWWAN_TYPE_SMS_Teleservice
 *
 * serviceCategory - Service Category (News, Restaraunts, etc.)
 *                   Refer to TIA/EIA-637 Section 3.4.3.2 for more detail.
 *
 * tlAckReq        - Indicates if the client needs to confirm
 *                   whether the message is received successfully or not.
 *
 * eMsgIdType      - Message ID Type
 *                   See QCWWAN_TYPE_SMS_MsgIdType
 *
 * addrDigitMode   - Address Digit Mode
 *
 * addrNumMode     - Address number mode
 *
 * addrNumType     - Address number type:
 *                   GSM SMS: addr value is GSM 7-bit chars
 *                   if NumMode == 0
 *                   WMS_NUMBER_UNKNOWN        = 0,
 *                   WMS_NUMBER_INTERNATIONAL  = 1,
 *                   WMS_NUMBER_NATIONAL       = 2,
 *                   WMS_NUMBER_NETWORK        = 3,
 *                   WMS_NUMBER_SUBSCRIBER     = 4,
 *                   WMS_NUMBER_ALPHANUMERIC   = 5,
 *                   WMS_NUMBER_ABBREVIATED    = 6,
 *                   WMS_NUMBER_RESERVED_7     = 7,
 *                   if NumMode == 1
 *                   WMS_NUMBER_DATA_IP        = 1
 *                   WMS_NUMBER_INTERNET_EMAIL = 2
 *
 * addrNumPlan      - Address number plan
 *                    CCITT E.164 and E.163, including ISDN plan
 *                    WMS_NUMBER_PLAN_UNKNOWN     = 0,
 *                    WMS_NUMBER_PLAN_TELEPHONY   = 1,
 *                    WMS_NUMBER_PLAN_RESERVED_2  = 2,
 *                    WMS_NUMBER_PLAN_DATA        = 3, :CCITT X.121
 *                    WMS_NUMBER_PLAN_TELEX       = 4, :CCITT F.69
 *                    WMS_NUMBER_PLAN_RESERVED_5  = 5,
 *                    WMS_NUMBER_PLAN_RESERVED_6  = 6,
 *                    WMS_NUMBER_PLAN_RESERVED_7  = 7,
 *                    WMS_NUMBER_PLAN_RESERVED_8  = 8,
 *                    WMS_NUMBER_PLAN_PRIVATE     = 9,
 *
 * addrLen          - Length of Address
 *
 * szAddress        - Destination address if we are sending out a MO SMS
 *                    Origination address if we are retrieving a MT SMS
 *
 * subType          - Sub address type
 *
 * subOdd           - Sub Address Odd byte
 *
 * subLen           - Length of the sub-address
 *
 * szSubAddress     - Destination sub-address if we are sending out a MO SMS
 *                    Origination sub-address if we are retrieving a MT SMS
 *
 * callBackLen      - Length of the Callback number
 *
 * szCallBack       - String containing the Call Back number with a 32 maximum characters
 *
 * ePriority        - Message Priority
 *                    See QCWWAN_TYPE_SMS_Priority
 *
 * ePrivacy         - Message Privacy Level
 *                    See QCWWAN_TYPE_SMS_Privacy
 *
 * eLanguage        - Message Language
 *                    See QCWWAN_TYPE_SMS_Language
 *
 * user_ack_req     - Specify whether user Acknowledgement is required
 *                    True/False
 *
 * dak_req          - Specify whether delivery acknowledge is required.
 *
 * userResp         - User Response Code
 *
 * szNumMsg         - Number of Voice messages (SMSI_VMN_95) on the network
 *
 * deliveryAlert    - Alert on Delivery
 *
 * dataEncoding     - Message Body Data Encoding.
 *                   Refer to QCWWAN_TYPE_SMS_DataEncodeType.
 *
 * ts_yr            - Time Stamp Year - the value is presented
 *                    in decimal format.
 *
 * ts_mon           - Time Stamp Month - the value is presented
 *                    in hexadecimal format.
 *                    E.g. December will be interpreted as 0x12
 *
 * ts_day           - Time Stamp Day- the value is presented in
 *                    hexadecimal format.
 *                    E.g. 30th will be interpreted as 0x30.
 *
 * ts_hrs           - Time Stamp Hour - the value is presented in
 *                    hexadecimal format.
 *                    E.g. eleven o'clock is presented as 0x0x11.
 *                    Represented in 24-hour time.
 *
 * ts_min           - Time Stamp Minute - the value is presented
 *                    in hexadecimal format.
 *                    E.g. thirty minute is presented as 0x30.
 *
 * ts_sec           - Time Stamp Second - the value is presented
 *                    in hexadecimal format.
 *                    E.g. thirty minute is presented as 0x30.
 *
 * abVal_yr         - Absolute Validity Year - the value is
 *                    presented in decimal format.
 *
 * abVal_mon        - Absolute Validity Month - the value is
 *                    presented in hexadecimal format.
 *                    E.g. December will be interpreted as 0x12.
 *
 * abVal_day        - Absolute Validity Day- the value is presented
 *                    in hexadecimal format.
 *                    E.g. 30th will be interpreted as 0x30.
 *
 * abVal_hrs        - Absolute Validity Hour - the value is
 *                    presented in hexadecimal format.
 *                    E.g. eleven o'clock is presented as 0x0x11.
 *                    Represented in 24-hour time.
 *
 * abVal_min        - Absolute Validity Minute - the value is
 *                    presented in hexadecimal format.
 *                    Thus, thirty minute is presented as 0x30.
 *
 * abVal_sec        - Absolute Validity Second - the value is
 *                    presented in hexadecimal format.
 *                    E.g. thirty minute is presented as 0x30.
 *
 * relVal_time      - Relative Validity Time of a SMS transmission.
 *                    See TIA/EIA 637-A Section 4.5.6 for more detail.
 *
 * abDef_yr         - Absolute Delivery Deferral Year - the value is
 *                    presented in decimal format.
 *
 * abDef_mon        - Absolute Delivery Deferral Month - the value is
 *                    presented in hexadecimal format.
 *                    E.g. December will be interpreted as 0x12.
 *
 * abDef_day        - Absolute Delivery Deferral Day - the value is presented
 *                    in hexadecimal format.
 *                     E.g. 30th will be interpreted as 0x30.
 *
 * abDef_hrs        - Absolute Delivery Deferral Hour - the value is
 *                    presented in hexadecimal format.
 *                    E.g. eleven o'clock is presented as 0x0x11.
 *                    Represented in 24-hour time.
 *
 * abDef_min        - Absolute Delivery Deferral Minute - the value is
 *                    presented in hexadecimal format.
 *                    E.g. thirty minute is presented as 0x30.
 *
 * abDef_sec        - Absolute Delivery Deferral Second - the value is
 *                    presented in hexadecimal format.
 *                    E.g. thirty minute is presented as 0x30.
 *
 * relDef_time      - Relative Delivery Deferral Time of a SMS transmission.
 *                    See Reference 1 Section 4.5.8 for more detail.
 *
 * eMsgDispMode     - Message Display Mode:
 *                    See QCWWAN_TYPE_SMS_MsgDispMode
 *
 */
typedef struct
{
    ULONG                        sizeStruct;
    QCWWAN_TYPE_SMS_ParamMask    eParamMask;
    QCWWAN_TYPE_SMS_MsgCategory  eMsgCategory;
    QCWWAN_TYPE_SMS_Teleservice  eTeleservice;
    WORD                         serviceCategory;
    BYTE                         tlAckReq;
    QCWWAN_TYPE_SMS_MsgIdType    eMsgIdType;
    BYTE                         addrDigitMode;
    BYTE                         addrNumMode;
    BYTE                         addrNumType;
    BYTE                         addrNumPlan;
    BYTE                         addrLen;
    WORD                         szAddress[MAX_SMSI_TL_ADDRESS];
    BYTE                         subType;
    BYTE                         subOdd;
    BYTE                         subLen;
    WORD                         szSubAddress[MAX_SMSI_TL_ADDRESS];
    BYTE                         callBackLen;
    WORD                         szCallBack[MAX_SMSI_CALL_BACK];
    QCWWAN_TYPE_SMS_Priority     ePriority;
    QCWWAN_TYPE_SMS_Privacy      ePrivacy;
    QCWWAN_TYPE_SMS_Language     eLanguage;
    BYTE                         user_ack_req;
    BYTE                         dak_req;
    BYTE                         userResp;
    WORD                         szNumMsg[2];
    BYTE                         deliveryAlert;
    QCWWAN_TYPE_SMS_DataEncodeType  dataEncoding;
    BYTE                         ts_yr;
    BYTE                         ts_mon;
    BYTE                         ts_day;
    BYTE                         ts_hrs;
    BYTE                         ts_min;
    BYTE                         ts_sec;
    BYTE                         abVal_yr;
    BYTE                         abVal_mon;
    BYTE                         abVal_day;
    BYTE                         abVal_hrs;
    BYTE                         abVal_min;
    BYTE                         abVal_sec;
    BYTE                         relVal_time;
    BYTE                         abDef_yr;
    BYTE                         abDef_mon;
    BYTE                         abDef_day;
    BYTE                         abDef_hrs;
    BYTE                         abDef_min;
    BYTE                         abDef_sec;
    BYTE                         relDef_time;
    QCWWAN_TYPE_SMS_MsgDispMode  eMsgDispMode;
}QCWWAN_STRUCT_SMS_CdmaHeader;

/*
 * QCWWAN_STRUCT_CDMA_SMS_RetrieveSms - SMS Retrieve structure
 * This structure is used to provide input to, and
 * receive output from the SwiRetrieveSMSMessage function.
 *
 * sizeStruct       - size of this structure
 *
 * sHeader          - The SMS header for this message.
 *
 * nSMSId           - The message ID number assigned by
 *                      the network.
 *
 * cntRemainingSMS  - The number of SMS
 *                      messages remaining in the modem (all three
 *                      queues combined).
 *
 * pMessage         - A pointer provided by the application
 *                      in which to place the body of the SMS
 *                      message.
 *
 * sizeBuffer       - The length in bytes of the buffer
 *                    supplied to receive the body of the SMS message.
 *                    This should be based on the maximum size of
 *                    message supported by the network. The API
 *                    currently supports up to 240 bytes, although
 *                    most networks limit this to a lower value.
 *                    retrievedStatus For future use; ignore.
 *                    On return , this parameter will be set to the
 *                    no. of bytes copied into the pMessage buffer
 *
 */
typedef struct
{
    ULONG                     sizeStruct;
    BOOL                      bUnreadSMS;      /* Not used */
    QCWWAN_STRUCT_SMS_CdmaHeader sHeader;
    WORD                      nSMSId;
    ULONG                     cntRemainingSMS;
    BYTE                      *pMessage;
    ULONG                     sizeBuffer;
    ULONG                     retrievedStatus;  /* Not used */
}QCWWAN_STRUCT_CDMA_SMS_RetrieveSms;

/*
 * Prototypes
 */

extern ULONG SwiCDMAEncodeMOTextMsg(
    BYTE    *pMessageSize,
    BYTE    *pMessage,
    BYTE    messageId,
    CHAR    *pDestAddr,
    CHAR    *pCallbackAddr,
    ULONG   textMsgLength,
    wchar_t *pTextMsg,
    BYTE    *pPriority,
    BYTE    *pEncodingAlphabet,
    BYTE    *pRelValidity );

extern ULONG SwiCDMADecodeMTTextMsg(
    ULONG  messageLength,
    BYTE*  pMessage,
    ULONG* pMessageID,
    BYTE*  pSenderAddrLength,
    CHAR*  pSenderAddr,
    BYTE*  pTextMsgLength,
    WORD*  pTextMsg,
    BYTE*  pPriority,
    BYTE*  pPrivacy,
    BYTE*  pLanguage,
    BYTE   mcTimeStamp[7],
    BYTE   absoluteValidity[7],
    BYTE*  pRelativeValidity,
    BYTE*  pDisplayMode,
    BOOL*  pUserAcknowledgementReq,
    BOOL*  pReadAcknowledgementReq,
    BYTE*  pAlertPriority,
    BYTE*  pCallbkAddrLength,
    CHAR*  pCallbkAddr );

extern ULONG SwiMakePdu(
    CHAR  *pNumber,
    CHAR  *pMessage,
    USHORT messagelen,
    CHAR  *pPdu,
    USHORT validity );

extern void SwiAsc2Hex(
    BYTE *hexa,
    CHAR *str );

extern ULONG SwiExtractpdu(
    CHAR *pPdu,
    BYTE *pSenderAddrLength,
    CHAR *pSenderAddr,
    BYTE *pTextMsgLength,
    BYTE *pScAddrLength,
    CHAR *pDate,
    CHAR *pTime,
    CHAR *pTextMsg,
    CHAR *pScAddr );

extern void SwiHex2Asc(
    BYTE   *pStr,
    BYTE   *pHexa,
    USHORT len );

extern ULONG WCDMAEncodeMOTextMsg(
    BYTE*     pMessageListSize,
    BYTE*     pMessageList,
    BYTE      messageRefNum,
    CHAR*     pDestAddr,
    ULONG     textMsgLength,
    wchar_t*  pTextMsg,
    CHAR*     pScAddr,
    swi_bool* pRejectDuplicates,
    swi_bool* pStatusReportReq,
    swi_bool* pReplyPath,
    BYTE*     pValidityFormat,
    BYTE      validityPeriod[7],
    BYTE*     pEncodingAlphabet );
