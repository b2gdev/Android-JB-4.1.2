/**
 *
 * @ingroup swiril
 *
 * @file 
 * Provides miscellaneous Sierra specific functions
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#include <telephony/ril.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <cutils/properties.h>

#include "swiril_main.h"
#include "swiril_cache.h"
#include "swiril_sim_qmi.h"
#include "swiril_misc_qmi.h"
#include "swiril_device_qmi.h"
#include "swiril_network_qmi.h"

#include "at_channel.h"
#include "at_tok.h"

#include "aa/aaglobal.h"
#include "qmerrno.h"
#include "qmudefs.h"
#include "SWIWWANCMAPI.h"

#define LOG_TAG "RIL"
#include "swiril_log.h"

#define IMSI_STRING_LENGTH 16       /**< IMSI string length including NULL */
#define IMEI_SV_STRING_LENGTH 2     /**< IMEI SV string length including NULL */

/* SIM related defines */
#define SIM_PIN1 1
#define SIM_PIN2 2
#define SIM_PUK1 1
#define SIM_PUK2 2

static const struct timeval TIMEVAL_SIMPOLL = { 1, 0 };

/* Should SIM_READY be sent.  This is used as part of the logic to delay
 * sending SIM_READY so that it does not conflict with registration.
 */
static bool s_pendingSIMReady = false;


/**
 * Enter the SIM PIN.
 *
 * @param simPINId
 *     the SIM PIN Id carrying value of either 1 or 2
 * @param [in] pSIMPIN
 *     the SIM PIN to be entered
 * @param t
 *     the RIL token identifier 
 *
 * @return
 *     none
 */
static void enterSIMPIN( ULONG simPINId, CHAR *pSIMPIN, RIL_Token t )
{
    ULONG     nRet;
    ULONG     verifyRetriesLeft = 0;
    ULONG     unblockRetriesLeft = 0;
    RIL_Errno errNumber = RIL_E_GENERIC_FAILURE;

    /* Default is 1 second, same as TIMEVAL_SIMPOLL */
    struct timeval timeval_simpoll = { 1, 0 };

    /* If the property exists, overwrite the default */
    CHAR propValue[PROPERTY_VALUE_MAX];
    int intpropvalue;

    if (property_get("persist.sierra.pin_delay", propValue, NULL) > 0) {
    	intpropvalue = atoi(propValue);
        LOGI("%s: pindelay=%i", __func__, intpropvalue);
        timeval_simpoll.tv_sec = intpropvalue;
    }

    /* Verify the requested PIN value */
    nRet = UIMVerifyPIN ( simPINId,
                          pSIMPIN, /* PIN value */
                          &verifyRetriesLeft,
                          &unblockRetriesLeft );
    /* Success */
    if (nRet == eQCWWAN_ERR_NONE) {
        LOGD( "enterSIMPIN Success" );

        /* Got OK, so return success and start SIM polling process so that the
           RADIO state can be updated once the SIM is ready. While not actually
           required, wait a short time before starting the polling process,
           since the SIM won't be ready right away anyways, and thus we avoid
           unnecessary polling.
         */
        resetpollSIMRetry();
        enqueueRILEvent( RIL_EVENT_QUEUE_PRIO,
                         pollSIMState,
                         NULL,
                         &timeval_simpoll );

        /* Return the successful response with number of retries left */
        RIL_onRequestComplete( t,
                               RIL_E_SUCCESS,
                               (int *)&verifyRetriesLeft,
                               sizeof(int *) );
    }
    /* Error */
    else {
        LOGE("enterSIMPIN error: %lu", nRet);

        /* Set the appropriate error code */
        if (nRet ==  eQCWWAN_ERR_QMI_INCORRECT_PIN){
            errNumber = RIL_E_PASSWORD_INCORRECT;
            RIL_onRequestComplete( t, errNumber, &verifyRetriesLeft, sizeof(int *));
        }
        else
            RIL_onRequestComplete( t, errNumber, NULL, 0);
    }
}

/**
 * Enter the SIM PUK.
 *
 * @param simPUKId
 *     the SIM PUK Id carrying value of either 1 or 2
 * @param [in] pSIMPUK
 *     the SIM PUK to be entered
 * @param [in] pSIMPIN
 *     the SIM PIN to be entered
 * @param t
 *     the RIL token identifier 
 *
 * @return
 *     none
 */
static void enterSIMPUK(
    ULONG     simPUKId,
    CHAR      *pSIMPUK,
    CHAR      *pSIMPIN,
    RIL_Token t )
{
    ULONG     nRet;
    ULONG     verifyRetriesLeft = 0;
    ULONG     unblockRetriesLeft = 0;
    RIL_Errno errNumber = RIL_E_GENERIC_FAILURE;

    /* Verify the requested PUK value */
    nRet = UIMUnblockPIN ( simPUKId,
                           pSIMPUK, /* PUK value */
                           pSIMPIN, /* PIN value */
                           &verifyRetriesLeft,
                           &unblockRetriesLeft );
    /* Success */
    if (nRet == eQCWWAN_ERR_NONE) {
        LOGD( "enterSIMPUK Success" );

        /* Got OK, so return success and start SIM polling process so that the
           RADIO state can be updated once the SIM is ready. While not actually
           required, wait a short time before starting the polling process,
           since the SIM won't be ready right away anyways, and thus we avoid
           unnecessary polling.
         */
        resetpollSIMRetry(); 
        enqueueRILEvent( RIL_EVENT_QUEUE_PRIO,
                         pollSIMState,
                         NULL,
                         &TIMEVAL_SIMPOLL );

        /* Return the successful response with number of retries left */
        RIL_onRequestComplete( t,
                               RIL_E_SUCCESS,
                               (int *)&unblockRetriesLeft,
                               sizeof(int *) );
    }
    /* Error */
    else {
        LOGE("enterSIMPUK error: %lu", nRet);

        /* Set the appropriate error code */
        if (nRet == eQCWWAN_ERR_QMI_INCORRECT_PIN){
            errNumber = RIL_E_PASSWORD_INCORRECT;
            RIL_onRequestComplete( t, errNumber, &unblockRetriesLeft, sizeof(int *));
        }
        else
            RIL_onRequestComplete( t, errNumber, NULL, 0);
    }
}

/**
 * Change the SIM PIN.
 *
 * @param simPINId
 *     the SIM PIN Id carrying value of either 1 or 2
 * @param [in] pSIMPINOld
 *     the old SIM PIN
 * @param [in] pSIMPINNew
 *     the new SIM PIN
 * @param t
 *     the RIL token identifier 
 *
 * @return
 *     none
 */
static void changeSIMPIN(
    ULONG     simPINId,
    CHAR      *pSIMPINOld,
    CHAR      *pSIMPINNew,
    RIL_Token t )
{
    ULONG     nRet;
    ULONG     verifyRetriesLeft = 0;
    ULONG     unblockRetriesLeft = 0;
    RIL_Errno errNumber = RIL_E_GENERIC_FAILURE;

    /* Verify the requested PUK value */
    nRet = UIMChangePIN ( simPINId,
                          pSIMPINOld, /* old PIN value */
                          pSIMPINNew, /* new PIN value */
                          &verifyRetriesLeft,
                          &unblockRetriesLeft );
    /* Success */
    if (nRet == eQCWWAN_ERR_NONE) {
        LOGD( "changeSIMPIN Success" );
        /* Return the successful response with number of retries left */
        RIL_onRequestComplete( t,
                               RIL_E_SUCCESS,
                               (int *)&verifyRetriesLeft,
                               sizeof(int *) );
    }
    /* Error */
    else {
        LOGE("changeSIMPIN error: %lu", nRet);

        /* Set the appropriate error code */
        if (nRet ==  eQCWWAN_ERR_QMI_INCORRECT_PIN){
            errNumber = RIL_E_PASSWORD_INCORRECT;
            RIL_onRequestComplete( t, errNumber, &verifyRetriesLeft, sizeof(int *));
        }
        else
            RIL_onRequestComplete( t, errNumber, NULL, 0);
    }
}


/**
 * RIL_REQUEST_GET_IMSI
 */
void requestGetIMSIQMI(void *data, size_t datalen, RIL_Token t)
{
    ULONG nRet;
    char imsiString[IMSI_STRING_LENGTH];
    char imsiStringPrint[IMSI_STRING_LENGTH];
        
    memset(imsiStringPrint, 'x', IMSI_STRING_LENGTH - 1);
    imsiStringPrint[IMSI_STRING_LENGTH - 1] = 0;

    nRet = GetIMSI(sizeof(imsiString), &imsiString[0]);
    if (nRet == eQCWWAN_ERR_NONE) {
        /* only print out first 6 digits */
        strncpy(imsiStringPrint, imsiString, 6);
        LOGD("requestGetIMSIQMI IMSI: %s\n", imsiStringPrint);
        /* Return the successful response and cache the value for next time */
        swiril_cache_RIL_onRequestComplete(t, RIL_E_SUCCESS, 
                                           &imsiString[0],
                                           sizeof(char *),
                                           RIL_REQUEST_GET_IMSI);
    }
    else {
        LOGE("%s GetIMSI error: %lu", __func__, nRet);
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
        /* SWI_TBD when GetIMSI failed with eQCWWAN_ERR_QMI_SIM_NOT_INITIALIZED,
           GetIMSI will always failed with same return, just reset modem for now */
        if ((nRet == eQCWWAN_ERR_QMI_SIM_NOT_INITIALIZED)&&(isDualModeModule())) {
            LOGE("%s reset modem", __func__);
            setPowerOff();
         }
    }
}

/**
 * RIL_REQUEST_GET_IMEI
 */
void requestGetIMEIQMI(void *data, size_t datalen, RIL_Token t)
{
    ULONG nRet;
    CHAR  ESNString[ESN_STRING_LENGTH];
    CHAR  IMEIString[IMEI_STRING_LENGTH];
    CHAR  MEIDString[MEID_STRING_LENGTH];
        
    nRet = GetSerialNumbers(sizeof(ESNString),
                            &ESNString[0],
                            sizeof(IMEIString),
                            &IMEIString[0],
                            sizeof(MEIDString),
                            &MEIDString[0]); 
    if (nRet == eQCWWAN_ERR_NONE) {
        LOGD("requestGetIMEIQMI ESN: %s, IMEI: %s, MEID: %s\n", ESNString, IMEIString, MEIDString);
        /* Return the successful response and cache the value for next time */
        swiril_cache_RIL_onRequestComplete(t, RIL_E_SUCCESS,
                                           &IMEIString[0],
                                           sizeof(char *),
                                           RIL_REQUEST_GET_IMEI);
    } else {
        LOGE("requestGetIMEIQMI: GetSerialNumbers() error: %lu", nRet);
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    }
}

/**
 * RIL_REQUEST_GET_IMEISV
 */
void requestGetIMEISVQMI(void *data, size_t datalen, RIL_Token t)
{
    int err;
    char *response = NULL;
        
    err = getIMEISV(&response);
    if (err < 0) {
        LOGE("%s:: getIMEISV failed, err number: %d", __func__, err);
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE,
                              NULL,
                              0);
    } else {
        swiril_cache_RIL_onRequestComplete(t, RIL_E_SUCCESS,
                                       response,
                                       sizeof(char *),
                                       RIL_REQUEST_GET_IMEISV);
    }
    if (response != NULL)
        free(response);
}

/**
 * RIL_REQUEST_DEVICE_IDENTITY
 */
void requestGetDeviceIdentityQMI(void *data, size_t datalen, RIL_Token t)
{
    ULONG nRet;
    CHAR  ESNString[ESN_STRING_LENGTH];
    CHAR  IMEIString[IMEI_STRING_LENGTH];
    CHAR  MEIDString[MEID_STRING_LENGTH];
    char* response[4];
    int   err;
        
    nRet = GetSerialNumbers(sizeof(ESNString),
                            &ESNString[0],
                            sizeof(IMEIString),
                            &IMEIString[0],
                            sizeof(MEIDString),
                            &MEIDString[0]); 
    if (nRet == eQCWWAN_ERR_NONE) {
        LOGD("requestGetDeviceIdentityQMI ESN: %s, IMEI: %s, MEID: %s\n", ESNString, IMEIString, MEIDString);
        /* Return the successful response and cache the value for next time */
        
        /* UMTS related */
        response[0] = &IMEIString[0];

        response[1] = NULL;
        err = getIMEISV(&response[1]);
        if (err < 0) {
            LOGE("%s:: getIMEISV failed, err number: %d", __func__, err);
            goto error;
        }
        
        /* CDMA related */
        response[2] = &ESNString[0];
        response[3] = &MEIDString[0];
        swiril_cache_RIL_onRequestComplete(t, RIL_E_SUCCESS, 
                                           &response,
                                           sizeof(response),
                                           RIL_REQUEST_DEVICE_IDENTITY);
    } else {
        LOGE("requestGetDeviceIdentityQMI error: %lu", nRet);
        goto error;
    }

finally:
    if (response[1] != NULL)
        free(response[1]);
    return;
error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    goto finally;
}

/**
 * RIL_REQUEST_ENTER_SIM_PIN
 */
void requestEnterSIMPINQMI(void *data, size_t datalen, RIL_Token t)
{
    enterSIMPIN(SIM_PIN1,
                ((CHAR **)data)[0], /* SIM PIN */
                t);
}

/**
 * RIL_REQUEST_ENTER_SIM_PIN2
 */
void requestEnterSIMPIN2QMI(void *data, size_t datalen, RIL_Token t)
{
    enterSIMPIN( SIM_PIN2,
                 ((CHAR **)data)[0], /* SIM PIN 2 */
                 t );
}

/**
 * RIL_REQUEST_ENTER_SIM_PUK
 */
void requestEnterSIMPUKQMI(void *data, size_t datalen, RIL_Token t)
{
    enterSIMPUK( SIM_PUK1,
                 ((CHAR **)data)[0], /* PUK 1 */
                 ((CHAR **)data)[1], /* PIN 1 */
                 t );
}

/**
 * RIL_REQUEST_ENTER_SIM_PUK2
 */
void requestEnterSIMPUK2QMI(void *data, size_t datalen, RIL_Token t)
{
    enterSIMPUK( SIM_PUK2,
                 ((CHAR **)data)[0], /* PUK 2 */
                 ((CHAR **)data)[1], /* PIN 2 */
                 t );
}

/**
 * RIL_REQUEST_CHANGE_SIM_PIN
 */
void requestChangeSIMPINQMI(void *data, size_t datalen, RIL_Token t)
{
    changeSIMPIN( SIM_PIN1,
                  ((CHAR **)data)[0], /* current SIM PIN */
                  ((CHAR **)data)[1], /* new SIM PIN */
                  t );
}

/**
 * RIL_REQUEST_CHANGE_SIM_PIN2
 */
void requestChangeSIMPIN2QMI(void *data, size_t datalen, RIL_Token t)
{
    changeSIMPIN( SIM_PIN2,
                  ((CHAR **)data)[0], /* current SIM PIN 2 */
                  ((CHAR **)data)[1], /* new SIM PIN 2 */
                  t );
}


/**
 * Call-back function to send delayed SIM_READY notification.
 */
static void delayedSIMReady(void *param)
{
    if (s_pendingSIMReady) {
        s_pendingSIMReady = false;

        LOGD("%s: set RADIO_STATE_SIM_READY", __func__);
        setRadioState(RADIO_STATE_SIM_READY);
    } else {
        LOGV("%s: SIM_READY already sent", __func__);
    }
}

/**
 * Determine whether a delayed SIM_READY notification should be used
 */
bool useDelayedSIMReady(void)
{
    ULONG nRet = 0;
    struct getServingNetworkPara servingNetworkPara;

    /* Assume there will be no delayed SIM_READY */
    s_pendingSIMReady = false;

    /* Check the current registration state.  If the modem is already
     * registered, SIM_READY can be sent right away.
     */
    memset(&servingNetworkPara, 0, sizeof(servingNetworkPara));
    servingNetworkPara.radioIfacesSize = RAT_MAX;
        
    nRet = GetServingNetwork(&servingNetworkPara.registrationState,
                             &servingNetworkPara.CSDomain,
                             &servingNetworkPara.PSDomain,
                             &servingNetworkPara.RAN,
                             &servingNetworkPara.radioIfacesSize,
                             &servingNetworkPara.radioIfaces[0],
                             &servingNetworkPara.roamingInd,
                             &servingNetworkPara.MCC,
                             &servingNetworkPara.MNC,
                             sizeof(servingNetworkPara.name),
                             &servingNetworkPara.name[0]);


    if (nRet == eQCWWAN_ERR_NONE) {
        LOGD("%s registrationState  : %lu\n", __func__, servingNetworkPara.registrationState);
        if(servingNetworkPara.registrationState == QMI_REG_STATE_REGISTERED)
            return false;
    } else {
        /* Log an error and assume not currently registered */
        LOGE("%s: GetServingNetwork error: %lu", __func__, nRet);
    }


    /* If the property exists, delay the SIM_READY notification
     * by the specified time.
     */
    char propValue[PROPERTY_VALUE_MAX];

    if (property_get("persist.sierra.sim_ready_delay", propValue, NULL) > 0) {
        /* Default is 0 second */
        struct timeval timeval_simready = { 0, 0 };
        int intpropvalue;

        intpropvalue = atoi(propValue);
        LOGI("%s: sim_ready_delay=%i", __func__, intpropvalue);
        timeval_simready.tv_sec = intpropvalue;

        enqueueRILEvent(RIL_EVENT_QUEUE_PRIO, delayedSIMReady, NULL,
                    &timeval_simready);

        s_pendingSIMReady = true;
        return true;
    }

    return false;
}

/*
 * If there is still a pending SIM_READY notification, then sent it now.
 * Normally called when modem becomes registered.
 */
void sendPendingSIMREADY(void)
{
    delayedSIMReady(NULL);
}

/**
 * Try using QMI API to get retry count for PIN.
 *
 * Currently only support PIN1.  May be expanded to other values later.
 */
int getPINRetryCountQMI(void)
{
    int num_retries = -1;
    ULONG nRet;
    ULONG Status;
    ULONG VerifyRetriesLeft;
    ULONG UnblockRetriesLeft;

    nRet = UIMGetPINStatus ( 1, &Status, &VerifyRetriesLeft, &UnblockRetriesLeft);
    if (nRet == eQCWWAN_ERR_NONE) {
        num_retries = VerifyRetriesLeft;
        LOGI("%s: success, retry count = %d", __func__, num_retries);
    } else
        LOGE("%s: error, nRet = %lu", __func__, nRet);

    return num_retries;
}

/** 
 * Reset s_pendingSIMReady
 *
 */
void ResetPendingSIMReady(void)
{
    s_pendingSIMReady = false;
}

