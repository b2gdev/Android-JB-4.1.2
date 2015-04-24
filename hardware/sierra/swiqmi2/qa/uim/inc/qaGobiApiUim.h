/**
 * \ingroup uim
 *
 * \file    qaGobiApiUim.h
 *
 * \brief   Uim Service API function prototypes
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __GOBI_API_UIM_H__
#define __GOBI_API_UIM_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_DESCRIPTION_LENGTH 255
#define MAX_CONTENT_LENGTH     1024
#define MAX_NO_OF_APPLICATIONS 10
#define MAX_NO_OF_SLOTS        5
#define MAX_PUK_LENGTH         8
#define MAX_PATH_LENGTH        255

/**
 *  This API resets the issuing control points state kept by the service.
 *
 *  \param  None
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5/MC77x0/SL909x\n
 *          Timeout: 30 Secs
 *
 *          The list of events to which the client is registered is emptied.
 *          The client must register again using the SLQSUIMEventReg API
 *          to start receiving the events again. This would mean that the
 *          callback registrations would be reset after this API.
 *
 */
ULONG SLQSUIMReset();

/**
 *  This structure contains information of the request parameters associated
 *  with a Power Down.
 *
 *  \param  slot
 *          - Indicates the slot to be used.
 *              - 1 - Slot 1
 *              - 2 - Slot 2
 *
 */
typedef struct
{
    BYTE slot;
} UIMPowerDownReq;

/**
 *  This API power downs the SIM card.
 *
 *  \param  pUIMPowerDownReq[IN]
 *          - See \ref UIMPowerDownReq for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5/MC77x0/SL909x\n
 *          Timeout: 30 Secs
 *
 *          This function powers down the card.
 *          This is usually performed when the phone is switched off or when
 *          it is set to Airplane mode.
 *
 */
ULONG SLQSUIMPowerDown ( UIMPowerDownReq *pUIMPowerDownReq );

/**
 *  This structure contains paramaters for file Information
 *
 *  \param  fileID
 *          - This is Identifier to SIM files;
 *            e.g. in UIM "6F07" is Identifier of IMSI File
 *
 *  \param  pathLen
 *          - Length of file Path
 *
 *  \param  path
 *          - Path value. This value must be the complete
 *            path of the file, which is a sequence block of 2
 *            bytes (e.g., 0x3F00 0x7FFF).
 */
typedef struct {
    WORD  fileID;
    BYTE  pathLen;
    WORD  path[MAX_PATH_LENGTH];
} fileInfo;

/**
 *  This structure contains information of parameters associated with
 *  the Refresh Event.
 *
 *  \param  stage
 *          - Indicates the stage of the Refresh procedure.
 *            - 0 - Waiting for OK to refresh
 *            - 1 - Refresh started
 *            - 2 - Refresh ended successfully
 *            - 3 - Refresh failed
 *
 *  \param  mode
 *          - Indicates the Refresh mode.
 *            - 0 - Reset
 *            - 1 - Init
 *            - 2 - Init and FCN
 *            - 3 - FCN
 *            - 4 - Init and Full FCN
 *            - 5 - Application reset
 *            - 6 - 3G session reset
 *
 *  \param  sessionType
 *          - Indicates the session type.
 *            - 0 - Primary GW provisioning
 *            - 1 - Primary 1X provisioning
 *            - 2 - Secondary GW provisioning
 *            - 3 - Secondary 1X provisioning
 *            - 4 - Nonprovisioning on slot 1
 *            - 5 - Nonprovisioning on slot 2
 *            - 6 - Card on slot 1
 *            - 7 - Card on slot 2
 *            - 8 - Logical channel on slot 1
 *            - 9 - Logical channel on slot 2
 *
 *  \param  aidLength
 *          - Number of sets of the following elements
 *            - Application Identifier
 *
 *  \param  aid
 *          - Application identifier value or channel ID. This
 *            value is required for non-provisioning and for
 *            logical channel session types. It is ignored in all
 *            other cases
 *
 *  \param  numFiles
 *          - Number of sets of the following elements:
 *            - file_id
 *            - path_len
 *            - path
 *
 *  \param  arrfileInfo
 *          - Array of file Information struct
 */
typedef struct
{
    BYTE     stage;
    BYTE     mode;
    BYTE     sessionType;
    BYTE     aidLength;
    BYTE     aid[MAX_DESCRIPTION_LENGTH];
    WORD     numOfFiles;
    fileInfo arrfileInfo[MAX_DESCRIPTION_LENGTH];
} UIMRefreshEvent;

/**
 *  This structure contains Application Status Information loaded on the card.
 *
 *  \param  appType
 *          - Indicates the type of the application.
 *              - 0 - Unknown
 *              - 1 - SIM card
 *              - 2 - USIM application
 *              - 3 - RUIM card
 *              - 4 - CSIM application
 *              - 5 - ISIM application
 *          - Other values are reserved for the future and are
 *            to be handled as "Unknown".
 *
 *  \param  appState
 *          - Indicates the state of the application.
 *              - 0 - Unknown
 *              - 1 - Detected
 *              - 2 - PIN1 or UPIN is required
 *              - 3 - PUK1 or PUK for UPIN is required
 *              - 4 - Personalization state must be checked
 *              - 5 - PIN1 is blocked
 *              - 6 - Illegal
 *              - 7 - Ready
 *
 *  \param  persoState
 *          - Indicates the state of the personalization for the application.
 *              - 0 - Unknown
 *              - 1 - Personalization operation is in progress
 *              - 2 - Ready
 *              - 3 - Personalization code is required
 *              - 4 - PUK for personalization code is required
 *              - 5 - Permanently blocked
 *
 *  \param  persoFeature
 *          - Indicates the personalization feature.
 *          - This applies only when a personalization code is required to
 *            deactivate or unblock personalization.
 *              - 0 - GW network personalization
 *              - 1 - GW network subset personalization
 *              - 2 - GW service provider personalization
 *              - 3 - GW corporate personalization
 *              - 4 - GW UIM personalization
 *              - 5 - 1X network type 1 personalization
 *              - 6 - 1X network type 2 personalization
 *              - 7 - 1X HRPD personalization
 *              - 8 - 1X service provider personalization
 *              - 9 - 1X corporate personalization
 *              - 10 - 1X RUIM personalization
 *              - 11 - Unknown
 *
 *  \param  persoRetries
 *          - Indicates the number of retries remaining to disable the
 *            personalization.
 *
 *  \param  persoUnblockRetries
 *          - Indicates the number of retries remaining to unblock the
 *            personalization.
 *
 *  \param  aidLength
 *          - Number of sets of the following elements. i.e. aidVal
 *          - If zero(0) then no aidVal information exists.
 *
 *  \param  aidVal[MAX_DESCRIPTION_LENGTH]
 *          - Application identifier value.
 *
 *  \param  univPin
 *          - Indicates whether UPIN replaces PIN1.
 *              - 0 - PIN1 is used
 *              - 1 - UPIN replaces PIN1
 *
 *  \param  pin1State
 *          - Indicates the state of PIN1.
 *              - 0 - Unknown
 *              - 1 - Enabled and not verified
 *              - 2 - Enabled and verified
 *              - 3 - Disabled
 *              - 4 - Blocked
 *              - 5 - Permanently blocked
 *
 *  \param  pin1Retries
 *          - Indicates the number of retries remaining to verify PIN1.
 *
 *  \param  puk1Retries
 *          - Indicates the number of retries remaining to unblock PIN1.
 *
 *  \param  pin2State
 *          - Indicates the state of PIN2.
 *              - 0 - Unknown
 *              - 1 - Enabled and not verified
 *              - 2 - Enabled and verified
 *              - 3 - Disabled
 *              - 4 - Blocked
 *              - 5 - Permanently blocked
 *
 *  \param  pin2Retries
 *          - Indicates the number of retries remaining to verify PIN2.
 *
 *  \param  puk2Retries
 *          - Indicates the number of retries remaining to unblock PIN2.
 *
 */
typedef struct
{
    BYTE appType;
    BYTE appState;
    BYTE persoState;
    BYTE persoFeature;
    BYTE persoRetries;
    BYTE persoUnblockRetries;
    BYTE aidLength;
    BYTE aidVal[MAX_DESCRIPTION_LENGTH];
    BYTE univPin;
    BYTE pin1State;
    BYTE pin1Retries;
    BYTE puk1Retries;
    BYTE pin2State;
    BYTE pin2Retries;
    BYTE puk2Retries;
} appStatus;

/**
 *  This structure contains information about the SLOTS present.
 *
 *  \param  cardState
 *          - Indicates the state of the card for each slot.
 *              - 0 - Absent
 *              - 1 - Present
 *              - 2 - Error
 *
 *  \param  upinState
 *          - Indicates the state of UPIN.
 *              - 0 - Unknown
 *              - 1 - Enabled and not verified
 *              - 2 - Enabled and verified
 *              - 3 - Disabled
 *              - 4 - Blocked
 *              - 5 - Permanently blocked
 *              - 0xFF - Not Available
 *
 *  \param  upinRetries
 *          - Indicates the number of retries remaining to verify the UPIN.
 *          - If 0xFF, information not available.
 *
 *  \param  upukRetries
 *          - Indicates the number of retries remaining to unblock the UPIN.
 *          - If 0xFF, information not available.
 *
 *  \param  errorState
 *          - Indicates the reason for the card error, and is valid only when
 *            the card state is Error
 *              - 0 - Unknown
 *              - 1 - Power down
 *              - 2 - Poll error
 *              - 3 - No ATR received
 *              - 4 - Volt mismatch
 *              - 5 - Parity error
 *              - 6 - Unknown; possibly removed
 *              - 7 - Card returned technical problems
 *              - 0xFF - Not Available
 *          - Other values are possible and reserved for future use.
 *          - When an unknown value is received, it is to be handled as "Unknown".
 *
 *  \param  numApp
 *          - Indicates the number of applications available on the card.
 *          - The following block is repeated for each application. i.e. AppStatus.
 *          - If zero(0) then no AppStatus information exists.
 *
 *  \param  AppStatus[MAX_NO_OF_APPLICATIONS]
 *          - See \ref appStatus for more information.
 *
 */
typedef struct
{
    BYTE      cardState;
    BYTE      upinState;
    BYTE      upinRetries;
    BYTE      upukRetries;
    BYTE      errorState;
    BYTE      numApp;
    appStatus AppStatus[MAX_NO_OF_APPLICATIONS];
} slotInfo;

/**
 *  This structure contains Card Status Information.
 *
 *  \param  indexGwPri
 *          - Index of the primary GW provisioning application.
 *          - The most significant byte indicates the slot (starting from 0),
 *            while the least significant byte indicates the application for
 *            that slot (starting from 0).
 *          - The value 0xFFFF identifies when the session does not exist.
 *
 *  \param  index1xPri
 *          - Index of the primary 1X provisioning application.
 *          - The most significant byte indicates the slot (starting from 0),
 *            while the least significant byte indicates the application for
 *            that slot (starting from 0).
 *          - The value 0xFFFF identifies when the session does not exist.
 *
 *  \param  indexGwSec
 *          - Index of the secondary GW provisioning application.
 *          - The most significant byte indicates the slot (starting from 0),
 *            while the least significant byte indicates the application for
 *            that slot (starting from 0).
 *          - The value 0xFFFF identifies when the session does not exist.
 *
 *  \param  index1xSec
 *          - Index of the secondary GW provisioning application.
 *          - The most significant byte indicates the slot (starting from 0),
 *            while the least significant byte indicates the application for
 *            that slot (starting from 0).
 *          - The value 0xFFFF identifies when the session does not exist.
 *
 *  \param  numSlot
 *          - Indicates the number of slots available on the device.
 *          - The following block is repeated for each slot. i.e. cardState
 *          - If zero(0) then no cardState information exists.
 *
 *  \param  SlotInfo[MAX_NO_OF_SLOTS]
 *          - See \ref slotInfo for more information.
 *
 */
typedef struct
{
    WORD     indexGwPri;
    WORD     index1xPri;
    WORD     indexGwSec;
    WORD     index1xSec;
    BYTE     numSlot;
    slotInfo SlotInfo[MAX_NO_OF_SLOTS];
} cardStatus;

/**
 *  This structure contains Hot Swap Status Information.
 *
 *  \param  hotSwapLength
 *          - Number of sets of the following elements. i.e. hot_swap
 *
 *  \param  hotSwap[MAX_DESCRIPTION_LENGTH]
 *          - Indicates the status of the hot-swap switch.
 *              - 0 - Hot-swap is not supported
 *              - 1 - Hot-swap is supported, but the status of the switch
 *                    is not supported
 *              - 2 - Switch indicates that the card is present
 *              - 3 - Switch indicates that the card is not present
 *
 */
typedef struct
{
    BYTE hotSwapLength;
    BYTE hotSwap[MAX_DESCRIPTION_LENGTH];
} hotSwapStatus;

/**
 *  This structure contains information of the response parameters associated
 *  with a Get Card Status API.
 *
 *  \param  pCardStatus(optional)
 *          - See \ref cardStatus for more information.
 *
 *  \param  pHotSwapStatus(optional)
 *          - See \ref hotSwapStatus for more information.
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not returned.
 */
typedef struct
{
    cardStatus    *pCardStatus;
    hotSwapStatus *pHotSwapStatus;
} UIMGetCardStatusResp;

/**
 *  This API retrieves the current status of the card.
 *
 *  \param  pUIMGetCardStatusResp[OUT]
 *          - See \ref UIMGetCardStatusResp for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5/MC77x0/SL909x\n
 *          Timeout: 30 Secs
 *
 *          This function retrieves the current status of the card and the
 *          status of all applications available on the card.
 *          The function also returns support information for the hot-swap
 *          feature and the status of the switch used to detect a card
 *          removal/insertion.
 *
 */
ULONG SLQSUIMGetCardStatus ( UIMGetCardStatusResp *pUIMGetCardStatusResp );

/**
 *  This structure contains the Session Information.
 *
 *  \param  sessionType
 *          - Indicates the session type.
 *              - 0 - Primary GW provisioning
 *              - 1 - Primary 1X provisioning
 *              - 2 - Secondary GW provisioning
 *              - 3 - Secondary 1X provisioning
 *              - 4 - Non-provisioning on slot 1
 *              - 5 - Non-provisioning on slot 2
 *              - 6 - Card on slot 1
 *              - 7 - Card on slot 2
 *              - 8 - Logical channel on slot 1
 *              - 9 - Logical channel on slot 2
 *
 *  \param  aidLength
 *          - Length of the following elements i.e. Application Identifier.
 *
 *  \param  aid
 *          - Application identifier value or channel ID.
 *          - This value is required for non-provisioning and for logical
 *            channel session types. It is ignored in all other cases.
 *
 */
typedef struct
{
    BYTE   sessionType;
    BYTE   aidLength;
    BYTE   aid[MAX_DESCRIPTION_LENGTH];
} UIMSessionInformation ;

/**
 *  This structure contains the information about the pin protection parameters
 *  that need to be set.
 *
 *  \param  pinID
 *          - Indicates the PIN ID to be enabled or disabled.
 *              - 1 - PIN1 (also called PIN)
 *              - 2 - PIN2
 *              - 3 - Universal PIN
 *              - 4 - Hidden key
 *
 *  \param  pinOperation
 *          - Indicates whether the PIN is enabled or disabled.
 *              - 0 - Disable the PIN
 *              - 1 - Enable the PIN
 *
 *  \param  pinLength
 *          - Length of the following elements i.e. pin value.
 *
 *  \param  pinValue[MAX_DESCRIPTION_LENGTH]
 *          - PIN value.
 *          - This value is a sequence of ASCII characters.
 *
 */
typedef struct
{
    BYTE pinID;
    BYTE pinOperation;
    BYTE pinLength;
    BYTE pinValue[MAX_DESCRIPTION_LENGTH];
} setPINProtection;

/**
 *  This structure contains information of the request parameters associated
 *  with a set pin protection API.
 *
 *  \param  sessionInfo
 *          - See \ref UIMSessionInformation for more information.
 *
 *  \param  pinProtection
 *          - See \ref setPINProtection for more information.
 *
 *  \param  pKeyReferenceID(optional)
 *          - Indicates the PIN key reference ID.
 *          - Indicates the PIN key reference ID. Valid values are from 1 to 8,
 *            respectively, for application 1 to application 8.
 *          - This TLV is used only for PIN1 and PIN2 and is ignored in all
 *            other cases.
 *
 *  \param  pIndicationToken(optional)
 *          - Response in Indication.
 *          - When this TLV is present, it indicates that the result must be
 *            provided in a subsequent indication.
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not added to the request.
 */
typedef struct
{
    UIMSessionInformation sessionInfo;
    setPINProtection      pinProtection;
    BYTE                  *pKeyReferenceID;
    ULONG                 *pIndicationToken;
} UIMSetPinProtectionReq ;

/**
 *  This structure contains the information about the retries remaining.
 *
 *  \param  verifyLeft
 *          - Number of remaining attempts to verify the PIN.
 *          - 0xFF, if unavailable.
 *
 *  \param  unblockLeft
 *          - Number of remaining attempts to unblock the PIN.
 *          - 0xFF, if unavailable.
 *
 *  \note   This value is returned only when the enable/disable operation has
 *          failed.
 *          This information is not sent for a hidden key PIN type.
 *
 */
typedef struct
{
    BYTE verifyLeft;
    BYTE unblockLeft;
} remainingRetries;

/**
 *  This structure contains the encrypted PIN1 Information.
 *
 *  \param  pin1Len
 *          - Number of sets of the following elements ie encrypted PIN1 value.
 *          - If zero(0), no information follows.
 *
 *  \param  pin1Val
 *          - Encrypted PIN1 value.
 *
 *  \note   This value is returned only when PIN1 is enabled successfully
 *          and the feature is supported.
 *
 */
typedef struct
{
    BYTE pin1Len;
    BYTE pin1Val[MAX_DESCRIPTION_LENGTH];
} encryptedPIN1;

/**
 *  This structure contains information of the response parameters associated
 *  with a set of PIN related API's.
 *
 *  \param  pRemainingRetries(optional)
 *          - See \ref remainingRetries for more information.
 *
 *  \param  pEncryptedPIN1(optional)
 *          - See \ref encryptedPIN1 for more information.
 *
 *  \param  pIndicationToken(optional)
 *          - Response in Indication.
 *          - When this TLV is present, it indicates that the result is
 *            provided in a subsequent indication.
 *          - 0xFFFFFFFF, if unavailable
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not returned.
 */
typedef struct
{
    remainingRetries *pRemainingRetries;
    encryptedPIN1    *pEncryptedPIN1;
    ULONG            *pIndicationToken;
} UIMPinResp;

/**
 *  This API enables or disables the protection of the UIM contents by a
 *  specific PIN.
 *
 *  \param  pUIMSetPinProtectionReq[IN]
 *          - See \ref UIMSetPinProtectionReq for more information.
 *
 *  \param  pUIMSetPinProtectionResp[OUT]
 *          - See \ref UIMPinResp for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5/MC77x0/SL909x\n
 *          Timeout: 30 Secs
 *
 *          This API enables or disables the protection of UIM contents by a
 *          specific PIN.
 *          The same PIN can be used by multiple sessions (i.e., the PIN is
 *          shared between GSM and RUIM in an ICC card).
 *          The PIN is automatically set for all the sessions when the API
 *          is executed.
 *          The client can pass a token in the request to receive the result
 *          in a subsequent SLQSUIMSetPinProtectionCallback indication.
 *
 */
ULONG SLQSUIMSetPinProtection (
                UIMSetPinProtectionReq *pUIMSetPinProtectionReq,
                UIMPinResp             *pUIMSetPinProtectionResp );

/**
 *  This structure contains the information about the pin parameters that need
 *  to be verified.
 *
 *  \param  pinID
 *          - Indicates the PIN ID to be verified.
 *              - 1 - PIN1 (also called PIN)
 *              - 2 - PIN2
 *              - 3 - Universal PIN
 *              - 4 - Hidden key
 *
 *  \param  pinLen
 *          - Length of the following elements i.e. pin value.
 *
 *  \param  pinVal[MAX_DESCRIPTION_LENGTH]
 *          - PIN value.
 *          - This value is a sequence of ASCII characters.
 *
 */
typedef struct{
    BYTE pinID;
    BYTE pinLen;
    BYTE pinVal[MAX_DESCRIPTION_LENGTH];
} verifyUIMPIN;

/**
 *  This structure contains information of the request parameters associated
 *  with a verify PIN API.
 *
 *  \param  sessionInfo
 *          - See \ref UIMSessionInformation for more information.
 *
 *  \param  verifyPIN
 *          - See \ref verifyUIMPIN for more information.
 *
 *  \param  pEncryptedPIN1(optional)
 *          - See \ref encryptedPIN1 for more information.
 *
 *  \param  pKeyReferenceID(optional)
 *          - Indicates the PIN key reference ID.
 *          - Indicates the PIN key reference ID. Valid values are from 1 to 8,
 *            respectively, for application 1 to application 8.
 *          - This TLV is used only for PIN1 and PIN2 and is ignored in all
 *            other cases.
 *
 *  \param  pIndicationToken(optional)
 *          - Response in Indication.
 *          - When this TLV is present, it indicates that the result must be
 *            provided in a subsequent indication.
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not added to the request.
 */
typedef struct
{
    UIMSessionInformation sessionInfo;
    verifyUIMPIN          verifyPIN;
    encryptedPIN1         *pEncryptedPIN1;
    BYTE                  *pKeyReferenceID;
    ULONG                 *pIndicationToken;
} UIMVerifyPinReq;

/**
 *  This API verifies the PIN before the card content is accessed.
 *
 *  \param  pUIMVerifyPinReq[IN]
 *          - See \ref UIMVerifyPinReq for more information.
 *
 *  \param  pUIMVerifyPinResp[OUT]
 *          - See \ref UIMPinResp for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5/MC77x0/SL909x\n
 *          Timeout: 30 Secs
 *
 *          This API verifies the PIN before the card content is accessed.
 *          The same PIN can be used by multiple sessions (i.e., the PIN is
 *          shared between GSM and RUIM in an ICC card).
 *          The PIN is automatically set for all the sessions when the API
 *          is executed.
 *          The client can pass a token in the request to receive the result
 *          in a subsequent SLQSUIMVerifyPinCallback.
 *
 */
ULONG SLQSUIMVerifyPin ( UIMVerifyPinReq *pUIMVerifyPinReq,
                         UIMPinResp      *pUIMVerifyPinResp );

/**
 *  This structure contains the information about the pin parameters that need
 *  to be verified.
 *
 *  \param  pinID
 *          - Indicates the PIN ID to be changed.
 *              - 1 - PIN1 (also called PIN)
 *              - 2 - PIN2
 *              - 3 - Universal PIN
 *              - 4 - Hidden key
 *
 *  \param  oldPINLen
 *          - Length of the following elements i.e. old pin value.
 *
 *  \param  oldPINVal[MAX_DESCRIPTION_LENGTH]
 *          - Old PIN value.
 *          - This value is a sequence of ASCII characters.
 *
 *  \param  pinLen
 *          - Length of the following elements i.e. new pin value.
 *
 *  \param  pinVal[MAX_DESCRIPTION_LENGTH]
 *          - New PIN value.
 *          - This value is a sequence of ASCII characters.
 *
 */
typedef struct{
    BYTE pinID;
    BYTE oldPINLen;
    BYTE oldPINVal[MAX_DESCRIPTION_LENGTH];
    BYTE pinLen;
    BYTE pinVal[MAX_DESCRIPTION_LENGTH];
} changeUIMPIN;

/**
 *  This structure contains information of the request parameters associated
 *  with a Change PIN API.
 *
 *  \param  sessionInfo
 *          - See \ref UIMSessionInformation for more information.
 *
 *  \param  changePIN
 *          - See \ref changeUIMPIN for more information.
 *
 *  \param  pKeyReferenceID(optional)
 *          - Indicates the PIN key reference ID.
 *          - Indicates the PIN key reference ID. Valid values are from 1 to 8,
 *            respectively, for application 1 to application 8.
 *          - This TLV is used only for PIN1 and PIN2 and is ignored in all
 *            other cases.
 *
 *  \param  pIndicationToken(optional)
 *          - Response in Indication.
 *          - When this TLV is present, it indicates that the result must be
 *            provided in a subsequent indication.
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not added to the request.
 */
typedef struct
{
    UIMSessionInformation sessionInfo;
    changeUIMPIN          changePIN;
    BYTE                  *pKeyReferenceID;
    ULONG                 *pIndicationToken;
} UIMChangePinReq;

/**
 *  This API changes the value of the specified PIN.
 *
 *  \param  pUIMChangePinReq[IN]
 *          - See \ref UIMChangePinReq for more information.
 *
 *  \param  pUIMChangePinResp[OUT]
 *          - See \ref UIMPinResp for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5/MC77x0/SL909x\n
 *          Timeout: 30 Secs
 *
 *          This API changes the value of the specified PIN.
 *          The application must pass both the new and the old values of the
 *          PIN to complete the operation
 *          The same PIN can be used by multiple sessions (i.e., the PIN is
 *          shared between GSM and RUIM in an ICC card).
 *          The PIN is automatically set for all the sessions when the API
 *          is executed.
 *          The client can pass a token in the request to receive the result
 *          in a subsequent SLQSUIMChangePinCallback.
 *
 */
ULONG SLQSUIMChangePin ( UIMChangePinReq *pUIMChangePinReq,
                         UIMPinResp      *pUIMChangePinResp );

/**
 *  This structure contains the information about the unblock pin parameters.
 *
 *  \param  pinID
 *          - Indicates the PIN ID to be changed.
 *              - 1 - PIN1 (also called PIN)
 *              - 2 - PIN2
 *              - 3 - Universal PIN
 *
 *  \param  pukLen
 *          - Length of the following elements i.e. puk value.
 *
 *  \param  pukVal[MAX_PUK_LENGTH]
 *          - PIN Unlock Key value.
 *          - This value is a sequence of ASCII characters.
 *
 *  \param  pinLen
 *          - Length of the following elements i.e. new pin value.
 *
 *  \param  pinVal[MAX_DESCRIPTION_LENGTH]
 *          - New PIN value.
 *          - This value is a sequence of ASCII characters.
 *
 */
typedef struct{
    BYTE pinID;
    BYTE pukLen;
    BYTE pukVal[MAX_DESCRIPTION_LENGTH];
    BYTE newPINLen;
    BYTE newPINVal[MAX_DESCRIPTION_LENGTH];
} unblockUIMPIN;

/**
 *  This structure contains information of the request parameters associated
 *  with a Unblock PIN API.
 *
 *  \param  sessionInfo
 *          - See \ref UIMSessionInformation for more information.
 *
 *  \param  unblockPIN
 *          - See \ref unblockUIMPIN for more information.
 *
 *  \param  pKeyReferenceID(optional)
 *          - Indicates the PIN key reference ID.
 *          - Indicates the PIN key reference ID. Valid values are from 1 to 8,
 *            respectively, for application 1 to application 8.
 *          - This TLV is used only for PIN1 and PIN2 and is ignored in all
 *            other cases.
 *
 *  \param  pIndicationToken(optional)
 *          - Response in Indication.
 *          - When this TLV is present, it indicates that the result must be
 *            provided in a subsequent indication.
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not added to the request.
 */
typedef struct
{
    UIMSessionInformation sessionInfo;
    unblockUIMPIN         unblockPIN;
    BYTE                  *pKeyReferenceID;
    ULONG                 *pIndicationToken;
} UIMUnblockPinReq;

/**
 *  This API unblocks a blocked PIN using the PUK code.
 *
 *  \param  pUIMUnblockPinReq[IN]
 *          - See \ref UIMUnblockPinReq for more information.
 *
 *  \param  pUIMUnblockPinResp[OUT]
 *          - See \ref UIMPinResp for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5/MC77x0/SL909x\n
 *          Timeout: 30 Secs
 *
 *          This API unblocks a blocked PIN using the PUK code.
 *          The client must pass PUK1 to unblock PIN1 or PUK2 to unblock PIN2.
 *          The same PIN can be used by multiple sessions (i.e., the PIN is
 *          shared between GSM and RUIM in an ICC card).
 *          The PIN is automatically set for all the sessions when the API
 *          is executed.
 *          The client can pass a token in the request to receive the result
 *          in a subsequent SLQSUIMUnblockPinCallback.
 *
 */
ULONG SLQSUIMUnblockPin ( UIMUnblockPinReq *pUIMUnblockPinReq,
                          UIMPinResp       *pUIMUnblockPinResp );

/**
 *  This structure contains information of the request parameters associated
 *  with a SLQSUIMEventRegister.
 *
 *  \param  eventMask(Mandatory)
 *          - Bitmask of the events that were successfully enabled.
 *            This result can be different from the mask used in the request
 *            when notifications are not supported. Additional bits are
 *            reserved for future use.
 *            - Bit 0 - Card status
 *            - Bit 1 - SAP connection
 */
typedef struct
{
    ULONG eventMask;
} UIMEventRegisterReqResp;

/**
 *  This API Registers for event notifications from the card.
 *
 *  \param  pUIMEventRegisterReqResp[IN/OUT]
 *          - See \ref UIMEventRegisterReqResp for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5/MC77x0/SL909x\n
 *          Timeout: 30 Secs
 *
 *          This function registers for event notifications from the card.
 *          The client must verify the mask in the response to determine
 *          which events were registered successfully. Events not supported
 *          correctly are not registered. The client can deregister from
 *          all event notifications by passing "0x00000000" bitmask in the
 *          request.
 *
 */
ULONG SLQSUIMEventRegister (
    UIMEventRegisterReqResp *pUIMEventRegisterReqResp );

/**
 *  This structure contains Parameters of the Session Information
 *
 *  \param  sessionInfo
 *          - Session Information
 *          - See \ref UIMSessionInformation for more information
 *
 *  \param  OKtoRefresh
 *          - Indicates whether a refresh is OK. Valid values:
 *            - 0 - Not OK to refresh
 *            - 1 - OK to refresh
 */
typedef struct
{
    UIMSessionInformation sessionInfo;
    BYTE                  OKtoRefresh;
} UIMRefreshOKReq;

/**
 *  This API Enables the client to indicate whether it is OK to start the
 *  Refresh procedure.
 *
 *  \param  pUIMRefreshOKReq
 *          - Consist of parameters for SLQSUIMRefreshOK.
 *            Please see /ref UIMRefreshOKReq for details.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5/MC77x0/SL909x\n 
 *          Timeout: 30 Secs
 *
 *          This function enables the client to indicate whether it is OK to
 *          start the Refresh procedure. This command is used only after a
 *          refresh event is received, which indicates the need to vote.
 */
ULONG SLQSUIMRefreshOK (
    UIMRefreshOKReq *pUIMRefreshOKReq );

/**
 *  This structure contains paramaters of refresh Information
 *
 *  \param  registerFlag
 *          - Flag that indicates whether to register or
 *            deregister for refresh indications. Valid values:
 *            - 0 - Deregister
 *            - 1 - Register
 *
 *  \param  voteForInit
 *          - Flag that indicates whether to vote for the init
 *            when there is a refresh. Valid values:
 *            - 0 - Client does not vote for initialization
 *            - 1 - Client votes for initialization
 *
 *  \param  numFiles
 *          - Number of sets of the following elements:
 *            - file_id
 *            - path_len
 *            - path
 *
 *  \param  arrfileInfo
 *          - Array of file Information structure.
 *          - See /ref fileInfo for more information
 */
typedef struct
{
    BYTE     registerFlag;
    BYTE     voteForInit;
    WORD     numFiles;
    fileInfo arrfileInfo[MAX_DESCRIPTION_LENGTH];
} registerRefresh;

/**
 *  This structure contains information of the request parameters associated
 *  with a Refresh Register.
 *
 *  \param  sessionInfo
 *          - Session Information params
 *          - See \ref UIMSessionInformation for more information
 *
 *  \param  regRefresh
 *          - Register Refresh parameters
 *          - See \ref registerRefresh for more information
 *
 */
typedef struct
{
    UIMSessionInformation sessionInfo;
    registerRefresh       regRefresh;
} UIMRefreshRegisterReq;

/**
 *  This API Registers for file change notifications triggered by the card.
 *
 *  \param  pUIMRefreshRegisterReq[IN]
 *          - See \ref UIMRefreshRegisterReq for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5/MC77x0/SL909x\n 
 *          Timeout: 30 Secs
 *
 *          This function registers for file change notifications triggered
 *          by the card. The client can specify a list of files. The client
 *          is notified only when one of the files is modified by the Refresh
 *          procedure. This function can be invoked multiple times for each
 *          session type. If the function is invoked twice with the same session
 *          type, the new values overwrite the previous values. The client can
 *          also use this function to stop receiving indications of the refresh.
 *          This API should be invoked prior to the invocation of the
 *          SLQSUIMSetRefreshCallBack for the events to be registered.
 *
 */
ULONG SLQSUIMRefreshRegister(
    UIMRefreshRegisterReq *pUIMRefreshRegisterReq );

/**
 *  This structure contains information of the request parameters associated
 *  with a SLQSUIMRefreshComplete.
 *
 *  \param  sessionInfo(Mandatory)
 *          - See \ref UIMSessionInformation for more information.
 * 
 *  \param  refreshComplete(Mandatory)
 *          - Indicates whether the refresh was successful.
 *            Valid values:
 *            - 0 - Refresh was not completed successfully
 *            - 1 - Refresh was completed successfully 
 *
 */
typedef struct
{
    UIMSessionInformation  sessionInfo;
    BYTE                   refreshComplete;
} UIMRefreshCompleteReq;


/**
 *  This API invoked when the client has finished the Refresh procedure. 
 *
 *  \param  pUIMRefreshCompleteReq[IN]
 *          - See \ref UIMRefreshCompleteReq for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5/MC77x0/SL909x\n
 *          Timeout: 30 Secs
 *
 *          This function is invoked when the client has finished the Refresh
 *          procedure (has reread all the cached files) and communicates this
 *          to the modem. This function enables the terminal response to be
 *          sent to the card
 */
ULONG SLQSUIMRefreshComplete (
    UIMRefreshCompleteReq *pUIMRefreshCompleteReq );

/**
 *  This structure contains information of the response parameters associated
 *  with a SLQSUIMRefreshGetLastEvent.
 *
 *  \param  refreshEvent(Optional)
 *          - See \ref UIMRefreshEvent for more information.
 */
typedef struct
{
    UIMRefreshEvent *pRefreshEvent;
} UIMRefreshGetLastEventResp;

/**
 *  This structure contains information of the request parameters associated
 *  with a SLQSUIMRefreshGetLastEvent.
 *
 *  \param  sessionInfo(Mandatory)
 *          - See \ref UIMSessionInformation for more information.
 *
 */
typedef struct
{
    UIMSessionInformation sessionInfo;
} UIMRefreshGetLastEventReq;

/**
 *  This API provides the ability to retrieve the last refresh event.
 *
 *  \param  pUIMRefreshGetLastEventReq[IN]
 *          - See \ref UIMRefreshGetLastEventReq for more information.
 *
 *  \param  pUIMRefreshGetLastEventResp[OUT]
 *          - See \ref UIMRefreshGetLastEventResp for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5/MC77x0/SL909x\n
 *          Timeout: 30 Secs
 *      
 *          This function provides the ability to retrieve the last refresh
 *          event. The event information is usually passed in as an indication
 *          from the QMI to the application and is saved by the application at
 *          that time. If the event information is not saved, the client can
 *          retrieve the last refresh event. 
 *          Details regarding the Refresh procedure (i.e., the stages and
 *          actions that an application must complete) are described in
 *          document: 80-VM566-1(NAA Refresh High Level Guide )
 */
ULONG SLQSUIMRefreshGetLastEvent (
    UIMRefreshGetLastEventReq  *pUIMRefreshGetLastEventReq,
    UIMRefreshGetLastEventResp *pUIMRefreshGetLastEventResp );

/**
 *  This structure contains information of the request parameters associated
 *  with a Get File Attributes API.
 *
 *  \param  sessionInfo
 *          - See \ref UIMSessionInformation for more information.
 *
 *  \param  fileIndex
 *          - See \ref fileInfo for more information.
 *
 *  \param  pIndicationToken(optional)
 *          - Response in Indication.
 *          - When this TLV is present, it indicates that the result must be
 *            provided in a subsequent indication.
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not added to the request.
 */
typedef struct
{
    UIMSessionInformation sessionInfo;
    fileInfo              fileIndex;
    ULONG                 *pIndicationToken;
} UIMGetFileAttributesReq;

/**
 *  This structure contains the information about the card result.
 *
 *  \param  sw1
 *          - SW1 received from the card.
 *
 *  \param  sw2
 *          - SW2 received from the card.
 *
 */
typedef struct
{
    BYTE sw1;
    BYTE sw2;
} cardResult;

/**
 *  This structure contains the information about the File Attributes.
 *
 *  \param  fileSize
 *          - Indicates the size of the file.
 *
 *  \param  fileID
 *          - Indicates the ID of the file.
 *
 *  \param  fileType
 *          - Indicates the type of the file.
 *              - 0 - Transparent
 *              - 1 - Cyclic
 *              - 2 - Linear fixed
 *              - 3 - Dedicated file
 *              - 4 - Master file
 *
 *  \param  recordSize
 *          - Indicates the size of the records.
 *          - Only for cyclic and linear fixed files
 *
 *  \param  recordCount
 *          - Indicates the total no. of the records.
 *          - Only for linear fixed files
 *
 *  \param  secRead
 *          - Read security attributes.
 *              - 0 - Always
 *              - 1 - Never
 *              - 2 - AND condition
 *              - 3 - OR condition
 *              - 4 - Single condition
 *
 *  \param  secReadMask
 *          - Mask with read security attributes.
 *          - This field is valid only when required by security attributes.
 *              - Bit 0 - PIN1
 *              - Bit 1 - PIN2
 *              - Bit 2 - UPIN
 *              - Bit 3 - ADM
 *
 *  \param  secWrite
 *          - Write security attributes.
 *              - 0 - Always
 *              - 1 - Never
 *              - 2 - AND condition
 *              - 3 - OR condition
 *              - 4 - Single condition
 *
 *  \param  secWriteMask
 *          - Mask with write security attributes.
 *          - This field is valid only when required by security attributes.
 *              - Bit 0 - PIN1
 *              - Bit 1 - PIN2
 *              - Bit 2 - UPIN
 *              - Bit 3 - ADM
 *
 *  \param  secIncrease
 *          - Increase security attributes.
 *              - 0 - Always
 *              - 1 - Never
 *              - 2 - AND condition
 *              - 3 - OR condition
 *              - 4 - Single condition
 *
 *  \param  secIncreaseMask
 *          - Mask with increase security attributes.
 *          - This field is valid only when required by security attributes.
 *              - Bit 0 - PIN1
 *              - Bit 1 - PIN2
 *              - Bit 2 - UPIN
 *              - Bit 3 - ADM
 *
 *  \param  secDeactivate
 *          - Deactivate security attributes.
 *              - 0 - Always
 *              - 1 - Never
 *              - 2 - AND condition
 *              - 3 - OR condition
 *              - 4 - Single condition
 *
 *  \param  secDeactivateMask
 *          - Mask with deactivate security attributes.
 *          - This field is valid only when required by security attributes.
 *              - Bit 0 - PIN1
 *              - Bit 1 - PIN2
 *              - Bit 2 - UPIN
 *              - Bit 3 - ADM
 *
  *  \param  secActivate
 *          - Activate security attributes.
 *              - 0 - Always
 *              - 1 - Never
 *              - 2 - AND condition
 *              - 3 - OR condition
 *              - 4 - Single condition
 *
 *  \param  secActivateMask
 *          - Mask with activate security attributes.
 *          - This field is valid only when required by security attributes.
 *              - Bit 0 - PIN1
 *              - Bit 1 - PIN2
 *              - Bit 2 - UPIN
 *              - Bit 3 - ADM
 *
 *  \param  rawLen
 *          - Length of the following elements i.e. raw value.
 *
 *  \param  rawValue[MAX_DESCRIPTION_LENGTH]
 *          - Raw value of file attributes.
 *
 */
typedef struct
{
    WORD fileSize;
    WORD fileID;
    BYTE fileType;
    WORD recordSize;
    WORD recordCount;
    BYTE secRead;
    WORD secReadMask;
    BYTE secWrite;
    WORD secWriteMask;
    BYTE secIncrease;
    WORD secIncreaseMask;
    BYTE secDeactivate;
    WORD secDeactivateMask;
    BYTE secActivate;
    WORD secActivateMask;
    WORD rawLen;
    BYTE rawValue[MAX_DESCRIPTION_LENGTH];
} fileAttributes;

/**
 *  This structure contains information of the response parameters associated
 *  with a Get File Attributes API.
 *
 *  \param  pCardResult(optional)
 *          - See \ref cardResult for more information.
 *
 *  \param  pFileAttributes(optional)
 *          - See \ref fileAttributes for more information.
 *
 *  \param  pIndicationToken(optional)
 *          - Response in Indication.
 *          - When this TLV is present, it indicates that the result must be
 *            provided in a subsequent indication.
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not returned.
 */
typedef struct
{
    cardResult     *pCardResult;
    fileAttributes *pFileAttributes;
    ULONG          *pIndicationToken;
} UIMGetFileAttributesResp;

/**
 *  This API retrieves the file attributes for any Elementary File(EF) or
 *  Dedicated File(DF) in the card and provides access by the path.
 *
 *  \param  pUIMGetFileAttributesReq[IN]
 *          - See \ref UIMGetFileAttributesReq for more information.
 *
 *  \param  pUIMGetFileAttributesResp[OUT]
 *          - See \ref UIMGetFileAttributesResp for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5/MC77x0/SL909x\n
 *          Timeout: 30 Secs
 *
 *          This API retrieves the file attributes for any Elementary File(EF)
 *          or Dedicated File(DF) in the card and provides access by the path.
 *          The response contains the status code received from the card
 *          (SW1 and SW2) when the card responded to the select request.
 *          The client can pass a token in the request to receive the result
 *          in a subsequent SLQSUIMGetFileAttributesCallback.
 *
 */
ULONG SLQSUIMGetFileAttributes (
                      UIMGetFileAttributesReq  *pUIMGetFileAttributesReq,
                      UIMGetFileAttributesResp *pUIMGetFileAttributesResp );

/**
 *  This structure contains the Depersonalization Information.
 *
 *  \param  feature
 *          - Indicates the personalization feature to de-activate or unblock.
 *              - 0 - GW network personalization
 *              - 1 - GW network subset personalization
 *              - 2 - GW service provider personalization
 *              - 3 - GW corporate personalization
 *              - 4 - GW UIM personalization
 *              - 5 - 1X network type 1 personalization
 *              - 6 - 1X network type 2 personalization
 *              - 7 - 1X HRPD personalization
 *              - 8 - 1X service provider personalization
 *              - 9 - 1X corporate personalization
 *              - 10 - 1X RUIM personalization
 *
 *  \param  operation
 *          - Indicates the operation to perform.
 *              - 0 - Deactivate personalization.
 *              - 1 - Unblock personalization.
 *
 *  \param  ckLen
 *          - Length of the following elements i.e. control key value.
 *
 *  \param  ckVal[MAX_DESCRIPTION_LENGTH]
 *          - Control key value.
 *          - This value is a sequence of ASCII characters.
 *
 */
typedef struct{
    BYTE  feature;
    BYTE  operation;
    BYTE  ckLen;
    BYTE  ckVal[MAX_DESCRIPTION_LENGTH];
} depersonalizationInformation;

/**
 *  This structure contains information of the request parameters associated
 *  with a Depersonalization API.
 *
 *  \param  depersonilisationInfo
 *          - See \ref depersonalizationInformation for more information.
 *
 */
typedef struct
{
    depersonalizationInformation depersonilisationInfo;
} UIMDepersonalizationReq;

/**
 *  This structure contains information of the response parameters associated
 *  with a Depersonalization API.
 *
 *  \param  pRemainingRetries(optional)
 *          - See \ref remainingRetries for more information.
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not returned.
 */
typedef struct
{
    remainingRetries *pRemainingRetries;
} UIMDepersonalizationResp;

/**
 *  This API de-activates or unblocks the personalization on the phone.
 *
 *  \param  pUIMDepersonilizationReq[IN]
 *          - See \ref UIMDepersonalizationReq for more information.
 *
 *  \param  pUIMDepersonilizationResp[OUT]
 *          - See \ref UIMDepersonalizationResp for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5/MC77x0/SL909x\n
 *          Timeout: 30 Secs
 *
 *          This API deactivates or unblocks the personalization on the phone.
 *          Each feature can be deactivated/unblocked independently of the
 *          other features.
 *
 */
ULONG SLQSUIMDepersonalization (
                      UIMDepersonalizationReq  *pUIMDepersonilizationReq,
                      UIMDepersonalizationResp *pUIMDepersonilizationResp );

/**
 *  This structure contains the Session Information.
 *
 *  \param  context
 *          - Authenticate context.
 *              - 0 - Runs the GSM algorithm (valid only on a 2GSIM card)
 *              - 1 - Runs the CAVE algorithm (valid only on a RUIM card)
 *              - 2 - GSM security context (valid only on a USIM application)
 *              - 3 - 3G security context (valid only on a USIM application)
 *              - 4 - VGCS/VBS security context
 *                  (valid only on aUSIM application)
 *              - 5 - GBA security context, Bootstrapping mode
 *                  (valid only on a USIM or ISIM application)
 *              - 6 - GBA security context, NAF Derivation mode
 *                  (valid only on a USIM or ISIM application)
 *              - 7 - MBMS security context, MSK Update mode
 *                  (valid only on a USIM application)
 *              - 8 - MBMS security context, MTK Generation mode
 *                  (valid only on a USIM application)
 *              - 9 - MBMS security context, MSK Deletion mode
 *                  (valid only on a USIM application)
 *              - 10 - MBMS security context, MUK Deletion mode
 *                  (valid only on a USIM application)
 *              - 11 - IMS AKA security context
 *                  (valid only on aISIM application)
 *              - 12 - HTTP-digest security context
 *                  (valid only onan ISIM application)
 *              - 13 - Compute IP authentication, CHAP
 *                  (valid onlyon RUIM or CSIM)
 *              - 14 - Compute IP authentication, MN-HA authenticator
 *                  (valid only on RUIM or CSIM)
 *              - 15 - Compute IP authentication, MIP-RRQ hash
 *                  (valid only on RUIM or CSIM)
 *              - 16 - Compute IP authentication, MN-AAA authenticator
 *                  (valid only on RUIM or CSIM)
 *              - 17 - Compute IP authentication, HRPD access authenticator
 *                  (valid only on RUIM or CSIM)
 *          - Other values are possible and reserved for future use.
 *
 *  \param  dataLen
 *          - Length of the following elements i.e. data.
 *
 *  \param  data[MAX_DESCRIPTION_LENGTH]
 *          - Authenticate Data.
 *
 */
typedef struct
{
    BYTE context;
    WORD dataLen;
    BYTE data[MAX_CONTENT_LENGTH];
} authenticationData;

/**
 *  This structure contains information of the request parameters associated
 *  with a Authenticate API.
 *
 *  \param  sessionInfo
 *          - See \ref UIMSessionInformation for more information.
 *
 *  \param  authData
 *          - See \ref authenticationData for more information.
 *
 *  \param  pIndicationToken(optional)
 *          - Response in Indication.
 *          - When this TLV is present, it indicates that the result must be
 *            provided in a subsequent indication.
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not added to the request.
 */
typedef struct
{
    UIMSessionInformation sessionInfo;
    authenticationData    authData;
    ULONG                 *pIndicationToken;
} UIMAuthenticateReq;

/**
 *  This structure contains the information about the authenticate result.
 *
 *  \param  contentLen
 *          - Length of the following elements i.e. content.
 *
 *  \param  content[MAX_CONTENT_LENGTH]
 *          - Authenticate data.
 *          - This value is a sequence of bytes returned from the card.
 *
 */
typedef struct
{
    WORD contentLen;
    BYTE content[MAX_CONTENT_LENGTH];
} authenticateResult;

/**
 *  This structure contains information of the response parameters associated
 *  with a Authenticate API.
 *
 *  \param  pCardResult(optional)
 *          - See \ref cardResult for more information.
 *
 *  \param  pAuthenticateResult(optional)
 *          - See \ref authenticateResult for more information.
 *
 *  \param  pIndicationToken(optional)
 *          - Response in Indication.
 *          - When this TLV is present, it indicates that the result must be
 *            provided in a subsequent indication.
 *
 *  \note   Using NULL for the pointers would make sure that the parameter is
 *          not returned.
 */
typedef struct
{
    cardResult         *pCardResult;
    authenticateResult *pAuthenticateResult;
    ULONG              *pIndicationToken;
} UIMAuthenticateResp;

/**
 *  This API executes the authentication algorithm on the card.
 *
 *  \param  pUIMAuthenticateReq[IN]
 *          - See \ref UIMAuthenticateReq for more information.
 *
 *  \param  pUIMAuthenticateResp[OUT]
 *          - See \ref UIMAuthenticateResp for more information.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5/MC77x0/SL909x\n
 *          Timeout: 30 Secs
 *
 *          This API executes a security command on the card that depends on
 *          the card type.
 *          The response contains the status code received from the card
 *          (SW1 and SW2) when the card responded to the read request.
 *          The client can pass a token in the request to receive the result
 *          in a subsequent SLQSUIMAuthenticateCallback
 *
 */
ULONG SLQSUIMAuthenticate ( UIMAuthenticateReq  *pUIMAuthenticateReq,
                            UIMAuthenticateResp *pUIMAuthenticateResp);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_UIM_H__ */
