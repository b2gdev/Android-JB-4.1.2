/**
 *
 * @ingroup swiril
 *
 * @file 
 * Provides main QMI-specific Sierra specific functions
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#include <telephony/ril.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <alloca.h>
#include <getopt.h>
#include <sys/socket.h>
#include <cutils/sockets.h>
#include <termios.h>
#include <cutils/properties.h>
#include <stdbool.h>

#define LOG_TAG "RIL"
#include "swiril_log.h"

#include "swiril_oem_qmi.h"
#include "swiril_misc_qmi.h"
#include "swiril_pdp_qmi.h"
#include "swiril_sim_qmi.h"
#include "swiril_network_qmi.h"
#include "swiril_network.h"
#include "swiril_main_qmi.h"
#include "swiril_sms_qmi.h"
#include "swiril_callhandling.h"
#include "swiril_services.h"
#include "swiril_misc.h"

/* For QMI */
#include "qmerrno.h"
#include "SWIWWANCMAPI.h"

#define PROPERTY_PERSIST_SIERRA_NOTIFICATION "persist.sierra.notification"

/**
 * Local definitions for SLQS related data structures
 */

/**
 * Used for:
 *     QCWWAN2kEnumerateDevices()
 *     QCWWAN2kConnect()
 */
typedef struct device_info_param{
  CHAR deviceNode[256];
  CHAR deviceKey[16];
} device_info_t;


static CHAR deviceNodeOld[NUM_MAX_STR_LEN];
static CHAR deviceKeyOld[NUM_MAX_STR_LEN];
static bool sierra_radio_notification = false;

/**
 *
 * Return the PROPERTY_PERSIST_SIERRA_NOTIFICATION setting 
 *
 * @return
 *     true  - when the PROPERTY_PERSIST_SIERRA_NOTIFICATION set
 *     false - otherwise
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     None
 */
bool getPropertyRadioNotifiction(void)
{
    return sierra_radio_notification;
}

/**
 *
 * Fill in the QMI RIL command handler function pointer
 *
 * @param[out] rilsolreqhandlerp 
 *     Pointer to UMTS/GSM handler structure 
 * @param[out] rilsolreqhandlercdmap 
 *     Pointer to CDMA handler structure 
 * @param voicEnable 
 *     voice call enabled or not
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
void initRILFunctionHandlerQMI(RIL_RequestHandlerFunc *rilsolreqhandlerp,
                               RIL_RequestHandlerFunc *rilsolreqhandlercdmap)
{
    /* GPRS, UMTS initialization */
    rilsolreqhandlerp[RIL_REQUEST_RADIO_POWER] = requestRadioPowerUMTS;
    rilsolreqhandlerp[RIL_REQUEST_GET_IMEI] = requestGetIMEIQMI;
    rilsolreqhandlerp[RIL_REQUEST_GET_IMEISV] = requestGetIMEISVQMI;
    rilsolreqhandlerp[RIL_REQUEST_GET_IMSI] = requestGetIMSIQMI;
    rilsolreqhandlerp[RIL_REQUEST_SETUP_DATA_CALL] = requestSetupDefaultPDPQMI;
    rilsolreqhandlerp[RIL_REQUEST_DEACTIVATE_DATA_CALL] = requestDeactivateDefaultPDPQMI;
#ifndef SWI_RIL_VERSION_6
    rilsolreqhandlerp[RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE] = requestLastPDPFailCauseQMI;
#endif
    rilsolreqhandlerp[RIL_REQUEST_DATA_CALL_LIST] = requestPDPContextListQMI;
    rilsolreqhandlerp[RIL_REQUEST_BASEBAND_VERSION] = requestBasebandVersionQMI;
    rilsolreqhandlerp[RIL_REQUEST_SIGNAL_STRENGTH] = requestSignalStrengthQMI;
#ifndef SWI_RIL_VERSION_6
    rilsolreqhandlerp[RIL_REQUEST_REGISTRATION_STATE] = requestRegistrationStateUMTS;
    rilsolreqhandlerp[RIL_REQUEST_GPRS_REGISTRATION_STATE] = requestGPRSRegistrationStateUMTS;
#else
    rilsolreqhandlerp[RIL_REQUEST_VOICE_REGISTRATION_STATE] = requestVoiceRegistrationStateUMTS;
    rilsolreqhandlerp[RIL_REQUEST_DATA_REGISTRATION_STATE] = requestDataRegistrationStateUMTS;
#endif
    rilsolreqhandlerp[RIL_REQUEST_OPERATOR] = requestOperatorUMTS;
    rilsolreqhandlerp[RIL_REQUEST_DEVICE_IDENTITY] = requestGetDeviceIdentityQMI;
    rilsolreqhandlerp[RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE] = requestQueryNetworkSelectionModeUMTS;
    rilsolreqhandlerp[RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC] = requestSetNetworkSelectionAutomaticUMTS;
    rilsolreqhandlerp[RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL] = requestSetNetworkSelectionManual;
    rilsolreqhandlerp[RIL_REQUEST_QUERY_AVAILABLE_NETWORKS] = requestQueryAvailableNetworks;
    rilsolreqhandlerp[RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE] = requestSetPreferredNetworkTypeQMI;
    rilsolreqhandlerp[RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE] = requestGetPreferredNetworkTypeQMI;
    rilsolreqhandlerp[RIL_REQUEST_GET_CURRENT_CALLS] = requestCurrentCallsQMI;
    rilsolreqhandlerp[RIL_REQUEST_OEM_HOOK_STRINGS] = requestOEMHookStrings;
    rilsolreqhandlerp[RIL_REQUEST_SEND_SMS] = requestSendSMSUMTS;
    rilsolreqhandlerp[RIL_REQUEST_SMS_ACKNOWLEDGE] = requestSMSAcknowledgeUMTS;
    rilsolreqhandlerp[RIL_REQUEST_WRITE_SMS_TO_SIM] = requestWriteSMSToSimUMTS;
    rilsolreqhandlerp[RIL_REQUEST_DELETE_SMS_ON_SIM] = requestDeleteSmsOnSimUMTS;
    rilsolreqhandlerp[RIL_REQUEST_GET_SMSC_ADDRESS] = requestGetSMSCAddressUMTS;
    rilsolreqhandlerp[RIL_REQUEST_SET_SMSC_ADDRESS] = requestSetSMSCAddressUMTS;
    rilsolreqhandlerp[RIL_REQUEST_SET_LOCATION_UPDATES] = requestSetLocationUpdatesQMI;
    rilsolreqhandlerp[RIL_REQUEST_SCREEN_STATE] = requestScreenStateQMI;
    rilsolreqhandlerp[RIL_REQUEST_RESET_RADIO] = requestResetRadioQMI;
    rilsolreqhandlerp[RIL_REQUEST_QUERY_AVAILABLE_BAND_MODE] = requestQueryAvailableBandModeQMI;
    rilsolreqhandlerp[RIL_REQUEST_SET_BAND_MODE] = requestSetBandModeQMI;

    /* SIM request initialization */
    rilsolreqhandlerp[RIL_REQUEST_ENTER_SIM_PIN]   = requestEnterSIMPINQMI;
    rilsolreqhandlerp[RIL_REQUEST_ENTER_SIM_PIN2]  = requestEnterSIMPIN2QMI;
    rilsolreqhandlerp[RIL_REQUEST_ENTER_SIM_PUK]   = requestEnterSIMPUKQMI;
    rilsolreqhandlerp[RIL_REQUEST_ENTER_SIM_PUK2]  = requestEnterSIMPUK2QMI;
    rilsolreqhandlerp[RIL_REQUEST_CHANGE_SIM_PIN]  = requestChangeSIMPINQMI;
    rilsolreqhandlerp[RIL_REQUEST_CHANGE_SIM_PIN2] = requestChangeSIMPIN2QMI;

    /* for Verizon LTE which use CDMA format SMS */
    rilsolreqhandlerp[RIL_REQUEST_CDMA_SEND_SMS] = requestSendSMSCDMA;
    rilsolreqhandlerp[RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE] = requestSMSAcknowledgeCDMA;
    rilsolreqhandlerp[RIL_REQUEST_CDMA_WRITE_SMS_TO_RUIM] = requestWriteSMSToRuimCDMA;
    rilsolreqhandlerp[RIL_REQUEST_CDMA_DELETE_SMS_ON_RUIM] = requestDeleteSmsOnRuimCDMA;


    /* CDMA initialization */
    rilsolreqhandlercdmap[RIL_REQUEST_GET_IMSI] = requestGetIMSIQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_RADIO_POWER] = requestRadioPowerCDMA;
    rilsolreqhandlercdmap[RIL_REQUEST_GET_IMEI] = requestGetIMEIQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_GET_IMEISV] = requestGetIMEISVQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_GET_CURRENT_CALLS] = requestCurrentCallsQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_OPERATOR] = requestOperatorCDMA;
    rilsolreqhandlercdmap[RIL_REQUEST_SIGNAL_STRENGTH] = requestSignalStrengthQMI;
#ifndef SWI_RIL_VERSION_6
    rilsolreqhandlercdmap[RIL_REQUEST_REGISTRATION_STATE] = requestRegistrationStateCDMA;
#else
    rilsolreqhandlercdmap[RIL_REQUEST_VOICE_REGISTRATION_STATE] = requestVoiceRegistrationStateCDMA;
    rilsolreqhandlercdmap[RIL_REQUEST_DATA_REGISTRATION_STATE] = requestDataRegistrationStateCDMA;
#endif
    rilsolreqhandlercdmap[RIL_REQUEST_BASEBAND_VERSION] = requestBasebandVersionQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_CDMA_SET_SUBSCRIPTION] = requestCDMASetSubscriptionQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_CDMA_SUBSCRIPTION] = requestCDMASubscriptionCDMA;
    rilsolreqhandlercdmap[RIL_REQUEST_DEVICE_IDENTITY] = requestGetDeviceIdentityQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE] = requestGetCDMARoamingPreferenceCDMA;
    rilsolreqhandlercdmap[RIL_REQUEST_CDMA_SET_ROAMING_PREFERENCE] = requestSetCDMARoamingPreferenceCDMA;
    rilsolreqhandlercdmap[RIL_REQUEST_SETUP_DATA_CALL] = requestSetupDefaultPDPQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_DEACTIVATE_DATA_CALL] = requestDeactivateDefaultPDPQMI;
#ifndef SWI_RIL_VERSION_6
    rilsolreqhandlercdmap[RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE] = requestLastPDPFailCauseQMI;
#endif
    rilsolreqhandlercdmap[RIL_REQUEST_DATA_CALL_LIST] = requestPDPContextListQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE] = requestSetPreferredNetworkTypeQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE] = requestGetPreferredNetworkTypeQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_OEM_HOOK_STRINGS] = requestOEMHookStrings;
    rilsolreqhandlercdmap[RIL_REQUEST_CDMA_SEND_SMS] = requestSendSMSCDMA;
    rilsolreqhandlercdmap[RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE] = requestSMSAcknowledgeCDMA;
    rilsolreqhandlercdmap[RIL_REQUEST_CDMA_WRITE_SMS_TO_RUIM] = requestWriteSMSToRuimCDMA;
    rilsolreqhandlercdmap[RIL_REQUEST_CDMA_DELETE_SMS_ON_RUIM] = requestDeleteSmsOnRuimCDMA;
    rilsolreqhandlercdmap[RIL_REQUEST_SET_LOCATION_UPDATES] = requestSetLocationUpdatesQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_SCREEN_STATE] = requestScreenStateQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_RESET_RADIO] = requestResetRadioQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_QUERY_AVAILABLE_BAND_MODE] = requestQueryAvailableBandModeQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_SET_BAND_MODE] = requestSetBandModeQMI;

    /* SIM request initialization */
    rilsolreqhandlercdmap[RIL_REQUEST_ENTER_SIM_PIN]   = requestEnterSIMPINQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_ENTER_SIM_PIN2]  = requestEnterSIMPIN2QMI;
    rilsolreqhandlercdmap[RIL_REQUEST_ENTER_SIM_PUK]   = requestEnterSIMPUKQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_ENTER_SIM_PUK2]  = requestEnterSIMPUK2QMI;
    rilsolreqhandlercdmap[RIL_REQUEST_CHANGE_SIM_PIN]  = requestChangeSIMPINQMI;
    rilsolreqhandlercdmap[RIL_REQUEST_CHANGE_SIM_PIN2] = requestChangeSIMPIN2QMI;

    /* voice call related */
    if (isVoiceEnabled()) {
        /* For GSM/UMTS */ 
        /* Basic Voice Call Requests */
        rilsolreqhandlerp[RIL_REQUEST_GET_CURRENT_CALLS] =
                                     requestGetCurrentCalls;
        rilsolreqhandlerp[RIL_REQUEST_LAST_CALL_FAIL_CAUSE] =
                                     requestLastCallFailCause;
        rilsolreqhandlerp[RIL_REQUEST_DIAL] = requestDial;
        rilsolreqhandlerp[RIL_REQUEST_HANGUP] = requestHangup;
        rilsolreqhandlerp[RIL_REQUEST_ANSWER] = requestAnswer;
        rilsolreqhandlerp[RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND] =
                                     requestHangupForegroundResumeBackground;
        rilsolreqhandlerp[RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND] =
                                     requestHangupWaitingOrBackground;
        rilsolreqhandlerp[RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE] =
                                     requestSwitchWaitingOrHoldingAndActive;
        rilsolreqhandlerp[RIL_REQUEST_CONFERENCE] = requestConference;
        rilsolreqhandlerp[RIL_REQUEST_SEPARATE_CONNECTION] =
                                     requestSeparateConnection;
        rilsolreqhandlerp[RIL_REQUEST_EXPLICIT_CALL_TRANSFER] =
                                     requestExplicitCallTransfer;

        /* Advanced Voice Call Requests */
        rilsolreqhandlerp[RIL_REQUEST_GET_CLIR] = requestGetCLIR;
        rilsolreqhandlerp[RIL_REQUEST_SET_CLIR] = requestSetCLIR;
        rilsolreqhandlerp[RIL_REQUEST_QUERY_CLIP] = requestQueryClip;
        rilsolreqhandlerp[RIL_REQUEST_UDUB] = requestUDUB;
        rilsolreqhandlerp[RIL_REQUEST_SET_CALL_WAITING] = requestSetCallWaiting;
        rilsolreqhandlerp[RIL_REQUEST_QUERY_CALL_FORWARD_STATUS] = requestQueryCallForwardStatus;
        rilsolreqhandlerp[RIL_REQUEST_SET_CALL_FORWARD] = requestSetCallForward;
        rilsolreqhandlerp[RIL_REQUEST_QUERY_CALL_WAITING] = requestQueryCallWaiting;

        /*  For CDMA */
        /* Basic Voice Call Requests */
        rilsolreqhandlercdmap[RIL_REQUEST_GET_CURRENT_CALLS] =
                                                    requestGetCurrentCalls;
        rilsolreqhandlercdmap[RIL_REQUEST_DIAL] = requestDial;
        rilsolreqhandlercdmap[RIL_REQUEST_HANGUP] = requestHangup;
        rilsolreqhandlercdmap[RIL_REQUEST_ANSWER] = requestAnswer;
        rilsolreqhandlercdmap[RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND] =
                                     requestHangupForegroundResumeBackground;
        rilsolreqhandlercdmap[RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND] =
                                     requestHangupWaitingOrBackground;
        rilsolreqhandlercdmap[RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE] =
                                     requestSwitchWaitingOrHoldingAndActive;
        rilsolreqhandlercdmap[RIL_REQUEST_CONFERENCE] = requestConference;
        rilsolreqhandlercdmap[RIL_REQUEST_SEPARATE_CONNECTION] =
                                     requestSeparateConnection;
        rilsolreqhandlercdmap[RIL_REQUEST_EXPLICIT_CALL_TRANSFER] =
                                     requestExplicitCallTransfer;

        /* Advanced Voice Call Requests */
        rilsolreqhandlercdmap[RIL_REQUEST_UDUB] = requestUDUB;
    }
}

void qmiSLQSRestart()
{
    ULONG nRet;
    BYTE DevicesSize = 1;
    static device_info_t devices[1] = { { {'\0'}, {'\0'} } };
    static device_info_t *pdev = &devices[0];

    /* SLQSStart() could return an error if the SLQS daemon is restarted, so
       try again in this case.
     */

start:
    while ( (nRet = SLQSStart()) != eQCWWAN_ERR_NONE) {
        LOGI ("SLQSStart failed.. %lx\n", nRet);
        sleep(1);
    }

    while ( (nRet = QCWWAN2kEnumerateDevices(&DevicesSize, (unsigned char *)pdev)) != eQCWWAN_ERR_NONE ) {
        LOGI ("\nQCWWAN2kEnumerateDevices unable to find device.. %lx\n", nRet);
        /**
         * SWI_TBD - After suspend-resume with external GPS enabled the SDK
         * daemon is blocked on a semaphore call and does not get execution
         * time for around 20 seconds. This leads to the API side to timeout
         * leading to the QMI requests and responses to be out of sync. The
         * workaround here kills the SDK process when the API times out and
         * restarts the SDK daemon
         */
        if (nRet == eQCWWAN_ERR_SWICM_TIMEOUT) {
            nRet = SLQSKillSDKProcess();
            LOGI ("\nSLQSKillSDKProcess %lx\n", nRet);
            sleep(1);
            goto start;
        }
        DevicesSize = 1;
        sleep(1);
    }

    LOGD("#devices: %d\ndeviceNode: %s\ndeviceKey: %s\n",
        DevicesSize,
        pdev->deviceNode,
        pdev->deviceKey  );

    /* connect to QMI device */
    while (QCWWAN2kConnect(pdev->deviceNode, pdev->deviceKey) != eQCWWAN_ERR_NONE) {
        LOGE ("\nUnable to find device..\n");
        sleep(1);
    }

    LOGD ("\nQCWWAN2kConnect found device\n");
}
        
void qmiDeviceConnect()
{
    CHAR deviceNode[NUM_MAX_STR_LEN];
    CHAR deviceKey[NUM_MAX_STR_LEN];
    ULONG nRet;
    SWI_FW_INFO_TYPE tech;
    enum eGobiImageCarrier image;
    CHAR propValue[PROPERTY_VALUE_MAX];

    /* Check if the property is defined and value is 1 */
    if ( (property_get(PROPERTY_PERSIST_SIERRA_NOTIFICATION, propValue, NULL) > 0) && 
         (0 == strcmp(propValue, "1")) ) {
        LOGI(">>> %s set: debug radio notifcation enabled", PROPERTY_PERSIST_SIERRA_NOTIFICATION);
        sierra_radio_notification = true;
    }
    else {
        LOGI(">>> %s NOT set: debug radio notifcation disabled", PROPERTY_PERSIST_SIERRA_NOTIFICATION);
    }

    /* Start the SLQS SDK */
    SetSDKImagePath("/system/bin/slqssdk");
    qmiSLQSRestart();       

    while (QCWWAN2kGetConnectedDeviceID(sizeof(deviceNode),
                                        &deviceNode[0],
                                        sizeof(deviceKey),
                                        &deviceKey[0]) != eQCWWAN_ERR_NONE) {
        LOGE ("\nUnable to get connected device ID..\n");
        sleep(1);
    }
    strcpy(deviceNodeOld, deviceNode);
    strcpy(deviceKeyOld, deviceKey);
    
    /* poll firmware info */
    initFirmwareInfoQMI();
        
    /* register callbacks */
    registerSignalStrengthCallbackQMI();
    registerSessionStateCallbackQMI();
    registerNewSMSCallbackQMI();
    registerSLQSSetServingSystemCallbackQMI();
    registerSetBandPreferenceCallbackQMI();
    registerDataBearerCallback();

    /* Register to voice Callback */
    if (isVoiceEnabled()) {
        registerVoiceCallbackQMI();
    }

    /* CDMA only callbacks */
    if (getFirmwareInfoTechQMI(&tech) && (tech == SWI_FW_INFO_TYPE_GOBI_CDMA)) {
        /* SWI_TBD there might be a chance firmware info invalid, so CDMA callbacks don't get registered */
        registerActivationStatusCallbackCDMA();
        /* SWI_TBD only register OMA-DM callback for Sprint image */
        if (getFirmwareInfoCarrierQMI(&image) && (image == eGOBI_IMG_CAR_SPRINT)) {
            registerSetOMADMStateCallbackCDMA();
        }
    }
    /* Debug purpose radio notfications */
    if (sierra_radio_notification) {
        registerRFInfoCallbackQMI();
        registerDormancyStatusCallbackQMI(true);
        registerByteTotalsCallbackQMI();
    }
    /* Network related initialization */
    networkServiceInit();

    /* SMS related initialization */
    smsServiceInit();
}

void qmiDeviceDisconnect(void)
{
    if (QCWWANDisconnect() != eQCWWAN_ERR_NONE ) {
        LOGE ("\nUnable to Disconnect from SDK..\n");
    }
    /* clear firmware info cache */
    clearFirmwareInfoQMI();
}

int qmiGetConnectedDeviceID(void)
{
    CHAR deviceNode[NUM_MAX_STR_LEN];
    CHAR deviceKey[NUM_MAX_STR_LEN];
    ULONG nRet;
    int iret = -1;
   
    nRet = QCWWAN2kGetConnectedDeviceID(sizeof(deviceNode),
                                        &deviceNode[0],
                                        sizeof(deviceKey),
                                        &deviceKey[0]);
    
    if (nRet == eQCWWAN_ERR_NONE ) {
        if ((strcmp(deviceNode, deviceNodeOld) == 0)
         && (strcmp(deviceKey, deviceKeyOld) == 0)) {
            iret = 0;
        }
    }
    else {
        LOGE ("QCWWAN2kGetConnectedDeviceID failed %lu ..\n", nRet);
    }
    
    return iret;
}

void clearDeviceNodeAndKey(void)
{
    memset(deviceNodeOld, 0, sizeof(deviceNodeOld));
    memset(deviceKeyOld, 0, sizeof(deviceKeyOld));
}

