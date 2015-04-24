/*************
 *
 * Filename: qaQmiNotify.c
 *
 * Purpose:  QMI notification Callback registration API and supporting functions
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/* include files */
#include "SwiDataTypes.h"
#include "qaQmiNotify.h"
#include "qaGobiApiCbk.h"
#include "amudefs.h"
#include "swi_osapi.h"
#include "qmerrno.h"
#include "sludefs.h"
#include "pi/piudefs.h"

/* Local Definitions */

/* Local Storage */

/* QMI Notification callback table */
local struct qaCallbackInfo qaCallbackList[eQMI_CB_END] =
{
    /*-----------------------------------------------------------------------
     * NOTE: The location (index) of each entry (row) in this table, must
     * exactly coincide with the numerical value of the corresponding 
     * callback's enumeration ( see enum eQMICallbackIndex in qaQmiNotify.h) 
     * or the SDK's notification mechanism will be broken. Therefore, when
     * adding a new entry, ensure that its location in the table corresponds
     * to its corresponding value in enum eQMICallbackIndex. Similarly, if
     * an entry is to be removed(relocated), ensure that the corresponding
     * enum is also removed(relocated).
     *-----------------------------------------------------------------------*/
                       /* pCallback  pSetCallback                                    pCallbackCache */
/* SESSION_STATE */      { NULL,     NULL,                                           NULL },
/* DATA_BEARER */        { NULL,     (pCbkType)SetDataBearerCallback,                NULL },
/* DORMANCY_STATUS */    { NULL,     (pCbkType)SetDormancyStatusCallback,            NULL },
/* MOBILE_IP */          { NULL,     (pCbkType)SetMobileIPStatusCallback,            NULL },
/* BYTE_TOTALS */        { NULL,     (pCbkType)iSetByteTotalsCallback,               NULL },
/* ACTVATION_STATUS */   { NULL,     (pCbkType)SetActivationStatusCallback,          NULL },
/* POWER */              { NULL,     (pCbkType)SetPowerCallback,                     NULL },
/* ROAMING_INDICATOR */  { NULL,     NULL,                                           NULL },
/* DATA_CAPABILITIES */  { NULL,     NULL,                                           NULL },
/* SIGNAL_STRENGTH */    { NULL,     (pCbkType)iSetSignalStrengthCallback,           NULL },
/* RF_INFO */            { NULL,     (pCbkType)SetRFInfoCallback,                    NULL },
/* RSSI_INFO */          { NULL,     (pCbkType)iSLQSSetSignalStrengthsCallback,      NULL },
/* LU_REJECT */          { NULL,     (pCbkType)SetLURejectCallback,                  NULL },
/* SERVING_SYSTEM */     { NULL,     NULL,                                           NULL },
/* BAND_PREF */          { NULL,     NULL,                                           NULL },
/* SYS_SEL_PREF */       { NULL,     NULL,                                           NULL },
/* SYS_INFO_IND */       { NULL,     (pCbkType)SLQSNasSysInfoCallBack,               NULL },
/* NETWORK_TIME_IND */   { NULL,     (pCbkType)SLQSNasNetworkTimeCallBack,           NULL },
/* NMEA */               { NULL,     (pCbkType)SetNMEACallback,                      NULL },
/* NMEA_PLUS */          { NULL,     NULL,                                           NULL },
/* PDS_STATE */          { NULL,     NULL,                                           NULL },
/* NEW_SMS */            { NULL,     (pCbkType)SetNewSMSCallback,                    NULL },
/* Trans Layer Info */   { NULL,     NULL,                                           NULL },
/* Trans NW REG Info */  { NULL,     NULL,                                           NULL },
/* SMS_EVENT */          { NULL,     (pCbkType)SLQSSetSMSEventCallback,              NULL },
/* MEMORY_FULL_IND */    { NULL,     (pCbkType)SLQSWmsMemoryFullCallBack,            NULL },
/* CAT_EVENT */          { NULL,     (pCbkType)iSetCATEventCallback,                 NULL },
/* OMADM_ALERT */        { NULL,     NULL,                                           NULL },
/* OMADM_STATE */        { NULL,     (pCbkType)SetOMADMStateCallback,                NULL },
/* SWIOMADM_ALERT */     { NULL,     (pCbkType)SetSLQSOMADMAlertCallback,            NULL },
/* SWIOMADM_STATE */     { NULL,     NULL,                                           NULL },
/* DEVIC_STATE_CHANGE */ { NULL,     NULL,                                           NULL },
/* SDK TERMINATED */     { NULL,     NULL,                                           NULL },
/* FW_DWLD_STATUS */     { NULL,     NULL,                                           NULL },
/* USSD_RELEASE */       { NULL,     NULL,                                           NULL },
/* USSD_Notification */  { NULL,     NULL,                                           NULL },
/* SUPS_Notification */  { NULL,     (pCbkType)SLQSVoiceSetSUPSNotificationCallback, NULL },
/* ALL_CALL_STATUS*/     { NULL,     (pCbkType)SLQSVoiceSetAllCallStatusCallBack,    NULL },
/* VOICE_PRIVACY_IND*/   { NULL,     (pCbkType)SLQSVoiceSetPrivacyChangeCallBack,    NULL },
/* DTMF_IND*/            { NULL,     (pCbkType)SLQSVoiceSetDTMFEventCallBack,        NULL },
/* SUPS_IND*/            { NULL,     (pCbkType)SLQSVoiceSetSUPSCallBack,             NULL },
/* OTASP_STATUS_IND */   { NULL,     NULL,                                           NULL },
/* INFO_REC_IND */       { NULL,     NULL,                                           NULL },
/* STATUS_CHANGE_IND*/   { NULL,     (pCbkType)SLQSUIMSetStatusChangeCallBack,       NULL },
/* REFRESH_IND */        { NULL,     NULL,                                           NULL },
};

/* List of notifications for which registered callbacks are to persist
 * as long as the application is running
 */
static enum eQMICallbackIndex qaPersistentAppCbkList[eQMI_CB_END] =
{
    eQMI_CB_DCS_DEVICE_STATE_CHANGE,
    eQMI_CB_DCS_SDK_TERMINATED
};

/* Functions */

/*************
 *
 * Name:    qaQmiGetCallbackMsgID
 *
 * Purpose: To get the QMI message ID of the received indication
 *
 * Parms:   packetpp [IN]  - double pointer to incoming QMI indication
 *
 * Return:  QMI message ID
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local USHORT qaQmiGetCallbackMsgID(
    BYTE **packetpp)
{
    BYTE *pktp;

    pktp = *(BYTE **)packetpp;
    return piget16(&pktp);
}

/*************
 *
 * Name:    UpkQmiNotification
 *
 * Purpose: Calls the appropriate QMI Service handlers for unpacking the received
 *          QMI indication.
 *
 * Parms:   pMdmResp   [IN] - pointer to packed indication from the modem
 *          pRespparms [IN] - pointer to received response parameters
 *
 * Return:  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local enum eQCWWANError UpkQmiNotification(
    BYTE             *pMdmResp,
    struct amrrparms *pRespparms)
{
    enum   eQCWWANError    eRCode = eQCWWAN_ERR_NONE;
    USHORT                 QmiMsgID;
    USHORT                 QmiSvcType;
    struct QmiNotification NotifResp;

    slmemset( (char *)&NotifResp,
              0,
              sizeof (struct QmiNotification) );

    /* Get the Message ID from the Message */
    QmiMsgID   = qaQmiGetCallbackMsgID(&pMdmResp);
    QmiSvcType = pRespparms->amparm.amqmi.amqmisvctype;

    /* Based on the Message ID call appropriate Ind Unpack routines */
    switch(QmiSvcType)
    {
        case eQMI_SVC_WDS:
            eRCode = UpkQmiCbkWdsNotification( QmiMsgID,
                                               pMdmResp,
                                               &NotifResp );
            break;

        case eQMI_SVC_DMS:
            eRCode = UpkQmiCbkDmsNotification( QmiMsgID,
                                               pMdmResp,
                                               &NotifResp );
            break;

        case eQMI_SVC_NAS:
            eRCode = UpkQmiCbkNasNotification( QmiMsgID,
                                               pMdmResp,
                                               &NotifResp );
            break;

        case eQMI_SVC_PDS:
            eRCode = UpkQmiCbkPdsNotification( QmiMsgID,
                                               pMdmResp,
                                               &NotifResp );
            break;
        case eQMI_SVC_WMS:
            eRCode = UpkQmiCbkWmsNotification( QmiMsgID,
                                               pMdmResp,
                                               &NotifResp );
            break;
        case eQMI_SVC_CAT:
            eRCode = UpkQmiCbkCatNotification( QmiMsgID,
                                               pMdmResp,
                                               &NotifResp );
            break;
        case eQMI_SVC_OMA:
            eRCode = UpkQmiCbkOmaDmNotification( QmiMsgID,
                                                 pMdmResp,
                                                 &NotifResp );
            break;
        case eQMI_SVC_SWIOMA:
            eRCode = UpkQmiCbkSwiOmaDmNotification( QmiMsgID,
                                                    pMdmResp,
                                                    &NotifResp );
            break;
        case eQMI_SVC_DCS:
            eRCode = UpkQmiCbkDcsNotification( QmiMsgID,
                                               pMdmResp,
                                               &NotifResp );
            break;

        case eQMI_SVC_FMS:
            eRCode = UpkQmiCbkFmsNotification( QmiMsgID,
                                               pMdmResp,
                                               &NotifResp );
            break;

        case eQMI_SVC_VOICE:
            eRCode = UpkQmiCbkVoiceNotification( QmiMsgID,
                                                 pMdmResp,
                                                 &NotifResp );
            break;

        case eQMI_SVC_UIM:
            eRCode = UpkQmiCbkUIMNotification( QmiMsgID,
                                               pMdmResp,
                                               &NotifResp );
            break;

        default:
            /* Should not get here */
            break;
    }
    return eRCode;
}

/*************
 *
 * Name:    qaQmiListenNotify
 *
 * Purpose: Start a user application thread to listen on the QMI notification
 *          socket. Thread will persist for life time of user application.
 *
 * Parms:   pCallback [IN]  - Callback funcion pointer to be added to the
 *                            callback list.
 *
 *          eCbkIndex [IN]  - Index of the Callback List Array
 *
 * Return:  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void qaQmiListenNotify(void)
{
    BYTE *pInBuf;    /* pointer to buffer allocated for AM QM notification */

    /* initialize pointer to QMI Indication buffer */
    pInBuf = (BYTE *)amgetNotifbufp();

    /* QMI Indication Request Response structure */
    struct amrrparms reqparms;
    reqparms.amparmtype = SWI_PARM_QMI;
    reqparms.amtimeout  = 0;    /* wait indefinitely */

    /* value-result parameter. Request maximum buffer size worth of octets,
     * return value specifies number of octets actually read.
     */
    ULONG lBufLength;

    /* pointer to QMI message rx'd from modem */
    BYTE *pInParam;
    while(1)
    {
        /* set the requested read buffer length */
        lBufLength = AMMAXNOTIFBKLEN;
        pInParam = NULL;

        /* Wait for an Indication */
        if( TRUE == amapiwaitnotif( pInBuf,
                                    &lBufLength,
                                    &reqparms,
                                    &pInParam,
                                    (ULONG)reqparms.amtimeout ) )
        {
            /* Call the Notification Unpacking routine */
            (void)UpkQmiNotification(pInParam, &reqparms);
        }
    }
}

/*************
 *
 * Name:    qaQmiRemoveAllCallbacks
 *
 * Purpose: De-register all QMI notification callbacks
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
package void qaQmiRemoveAllCallbacks(void)
{
    enum eQMICallbackIndex i;
    ULONG m,n;
    BOOL remove;

    m = sizeof(qaPersistentAppCbkList)/sizeof(qaPersistentAppCbkList[0]);

    for( i = eQMI_CB_START ; i < eQMI_CB_END ; i++ )
    {
        n = m;
        remove = TRUE;
        while( n-- && remove )
        {
            if( i == qaPersistentAppCbkList[n] )
            {
                remove = FALSE;
            }
        }
        if( remove )
        {
            /* deregister the application's callback */
            qaQmiSetCallback( i, NULL );
        }
    }
}

/*
 * Name:    qaNotifyInit
 *
 * Purpose: QA notification initialization routine
 *
 * Parms:   none
 *
 * Return:  none
 */
global void qaNotifyInit(void)
{
    /* Create API notification thread. Notifications are received from the
     * SDK and registered notifications are further relayed to the application.
     * This thread runs in the application's process space.
     */
    swi_osapithreadcreate( qaQmiListenNotify );
}

/*
 * Name:    qaQmiGetCallback
 *
 * Purpose: Get the Callback associated with the Callback Index
 *
 * Parms:   eCbkIndex - Callback Index
 *
 * Return:  Callback associated with the Index
 */
void *qaQmiGetCallback( enum eQMICallbackIndex eCbkIndex )
{
    /* Get the Callback pointer to the Callback list */
    return qaCallbackList[eCbkIndex].pCallback;
}

/*
 * Name:    qaQmiSetCallback
 *
 * Purpose: Set the Callback to the associated Callback Index
 *
 * Parms:   eCbkIndex - Callback Index
 *          pCallback - Pointer to the Callback function
 *
 * Return:  none
 */
void qaQmiSetCallback(
    enum eQMICallbackIndex eCbkIndex,
    void                   *pCallback )
{
    /* Set the Callback pointer to the Callback list */
    qaCallbackList[eCbkIndex].pCallback = pCallback;
}

/*
 * Name:    qaCbkDeviceStateChange
 *
 * Purpose: Device State Change event handler
 *
 * Parms:   device_state - current state of the device
 *
 * Return:  none
 */
void qaQmiDeviceStateChangeCbk(
    BYTE deviceState )
{
    enum eQMICallbackIndex eCbkIndex;

    if( DEVICE_STATE_DISCONNECTED == deviceState )
    {
        /* To ensure application level callback persistance, the SDK
         * must register for notifications with the device every time a
         * device connection event follows a device disconnection event.
         * Currently, this requires that we cache references to the user
         * supplied callback functions.
         */
        for( eCbkIndex = eQMI_CB_START; eCbkIndex < eQMI_CB_END; eCbkIndex++ )
        {
            /* If the callback is not subscribed for, or if callback
             * registration does not require sending a request to the device,
             * then do not cache a reference to the user supplied callback
             */
            if( ( NULL == qaCallbackList[eCbkIndex].pCallback ) ||
                ( NULL == qaCallbackList[eCbkIndex].pSetCallback ) )
            {
                continue;
            }
            qaCallbackList[eCbkIndex].pCallbackCache =
                                      qaCallbackList[eCbkIndex].pCallback;
            qaCallbackList[eCbkIndex].pCallback = NULL;
        }
    }
    else if( DEVICE_STATE_READY == deviceState )
    {
        /* If the device is connected, subscribe to all the callbacks saved at
         * the time of device disconnection.
         */
        for( eCbkIndex = eQMI_CB_START; eCbkIndex < eQMI_CB_END; eCbkIndex++ )
        {
            /* If the callback is not cached, or if callback registration does
             * not require sending a request to the device, then do not attempt
             * device registeration for the corresponding notification.
             */
             if( ( NULL == qaCallbackList[eCbkIndex].pCallbackCache ) ||
                 ( NULL == qaCallbackList[eCbkIndex].pSetCallback ) )
            {
                continue;
            }

            /* register with the device to receive the corresponding QMI
             * notification.
             */
            ( qaCallbackList[eCbkIndex].pSetCallback)(
                                        qaCallbackList[eCbkIndex].pCallbackCache );
            qaCallbackList[eCbkIndex].pCallbackCache = NULL;
        }
    }
}
