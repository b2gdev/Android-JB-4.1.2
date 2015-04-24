/*
 * \ingroup cbk
 *
 * \file    qaCbkVoiceNotify.c
 *
 * \brief   Contains routines for the VOICE\USSD Notifications
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiCbk.h"
#include "qaCbkVoiceUssdInd.h"
#include "qaQmiNotify.h"

/*************
 *
 * Name:   qaQmiVoiceUSSDReleaseNotify
 *
 * Purpose: To notify all the Callbacks associated with the USSD Release Event
 *          Report Notification.
 *
 * Notes:   none
 *
 **************/
local void qaQmiVoiceUSSDReleaseNotify( )
{
    void                   *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_VOICE_USSD_RELEASE_IND;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        /* No TLVs are associated with this indication */
        /* Invoke the callback */
        ((tFNUSSDRelease)pCallback) ( );
    }
}

/*************
 *
 * Name:   qaQmiVoiceAllCallStatusNotify
 *
 * Purpose: To notify all the Callbacks associated with the All Call Status
 *          Indication.
 *
 * Notes:   none
 *
 **************/
local void qaQmiVoiceAllCallStatusNotify(
    voiceSetAllCallStatusCbkInfo *pResp )
{
    void                   *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_VOICE_ALL_CALL_STATUS;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        /* Invoke the callback */
       ((tFNAllCallStatus)pCallback) ( pResp );
    }
}

/*************
 *
 * Name:    qaQmiVoiceEventNotify
 *
 * Purpose: To notify all the Callbacks associated with the USSD Event
 *          Report Notification.
 *
 * Parms:   pQmiIndication  [IN] - pointer to structure used to store all QMI
 *                                 Notification parameters.
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void qaQmiVoiceEventNotify(
    struct QmiNotification *pQmiIndication )
{
    void  *pCallback;
    enum  eQMICallbackIndex CbkIndex;
    ULONG CbkParamOne;  /* Notification type */
    BYTE  *CbkParamTwo; /* Network Information */

    CbkIndex  = eQMI_CB_VOICE_USSD_IND;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        struct VoiceUssdTlv *pResp =
            &pQmiIndication->QmiInd.qaQmiCbkVoiceUssdInd.VSTlv;

        /* The TLV was present in the Indication - hence process */
        /* Included for TLVs which are not implemented */
        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            CbkParamOne = pResp->Notification_Type;
            CbkParamTwo = (BYTE *)(&(pResp->NetworkInfo));

            /* Invoke the callback */
            ((tFNUSSDNotification)pCallback) ( CbkParamOne, CbkParamTwo );
        }
    }
}

/*************
 *
 * Name:    qaQmiVoiceSUPSNotify
 *
 * Purpose: To notify all the Callbacks associated with the SUPS Notification
 *          event.
 *
 * Parms:   pQmiIndication  [IN] - pointer to structure used to store all QMI
 *                                 Notification parameters.
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void qaQmiVoiceSUPSNotify(
    voiceSUPSNotification *pResp )
{
    void  *pCallback;
    enum  eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_VOICE_SUPS_NOTIFICATION_IND;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        /* Invoke the callback */
        ((tFNSUPSNotification)pCallback) ( pResp );
    }
}

/*************
 *
 * Name:    qaQmiVoiceDTMFNotify
 *
 * Purpose: To notify all the Callbacks associated with the DTMF event.
 *
 * Parms:   voiceDTMFEventInfo  [IN] - pointer to structure used to store all
 *                                     voice DTMF events
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void qaQmiVoiceDTMFNotify(
    voiceDTMFEventInfo *pResp )
{
    void  *pCallback;
    enum  eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_VOICE_DTMF_IND;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        /* Invoke the callback */
        ((tFNDTMFEvent)pCallback) ( pResp );
    }
}

/*************
 *
 * Name:    qaQmiVoiceSUPSIndNotify
 *
 * Purpose: To notify all the Callbacks associated with the SUPS Info event.
 *
 * Parms:   voiceSUPSInfo  [IN] - pointer to structure used to store all voice
 *                                SUPS Information
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void qaQmiVoiceSUPSIndNotify(
    voiceSUPSInfo *pResp )
{
    void  *pCallback;
    enum  eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_VOICE_SUPS_IND;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        /* Invoke the callback */
        ((tFNSUPSInfo)pCallback) ( pResp );
    }
}

/*************
 *
 * Name:    qaQmiVoicePrivacyNotify
 *
 * Purpose: To notify all the Callbacks associated with the Privacy Change
 *          event.
 *
 * Notes:   none
 *
 **************/
local void qaQmiVoicePrivacyNotify(
    voicePrivacyInfo *pResp )
{
    void  *pCallback;
    enum  eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_VOICE_PRIVACY_IND;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        /* Invoke the callback */
        ((tFNPrivacyChange)pCallback) ( pResp );
    }
}

/*************
 *
 * Name:   qaQmiVoiceOTASPStatusNotify
 *
 * Purpose: To notify all the Callbacks associated with the OTASP or OTAPA
 *          event Indication.
 *
 * Notes:   none
 *
 **************/
local void qaQmiVoiceOTASPStatusNotify(
    voiceOTASPStatusInfo *pResp )
{
    void                   *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_VOICE_OTASP_STATUS_IND;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        /* Invoke the callback */
       ((tFNOTASPStatus)pCallback) ( pResp );
    }
}

/*************
 *
 * Name:   qaQmiVoiceInfoRecNotify
 *
 * Purpose: To notify all the Callbacks associated with the information record
 *          event Indication.
 *
 * Notes:   none
 *
 **************/
local void qaQmiVoiceInfoRecNotify(
    voiceInfoRec *pResp )
{
    void                   *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_VOICE_INFO_REC_IND;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        /* Invoke the callback */
       ((tFNInfoRec)pCallback) ( pResp );
    }
}

/*************
 *
 * Name:   UpkQmiCbkVoiceNotification
 *
 * Purpose: Unpacks the received Voice indication and invokes the approriate
 *          callback based on the QMI message type.
 *
 * Parms:   QmiMsgID        - QMI Message ID
 *          pMdmResp   [IN] - Pointer to packed response from the modem.
 *          pNotifResp [IN] - Notification Structure to fill in the parameters.
 *
 * Return:  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
package enum eQCWWANError UpkQmiCbkVoiceNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    switch(QmiMsgID)
    {
        case eQMI_VOICE_USSD_RELEASE_IND:
        {
            /* Notify to the Callbacks associated */
            qaQmiVoiceUSSDReleaseNotify( );
            break;
        }
        case eQMI_VOICE_USSD_IND:
        {
            struct QmiCbkVoiceUssdInd *pResp =
                &pNotifResp->QmiInd.qaQmiCbkVoiceUssdInd;

            /* Set all the TLVs to be in Not-Present State */
            pResp->VSTlv.TlvPresent = FALSE;

            /* Unpack the Voice USSD Event Indication */
            eRCode = UpkQmiCbkVoiceUssdInd ( pMdmResp,
                                             pResp );

            /* Notify to the Callbacks associated */
            qaQmiVoiceEventNotify( pNotifResp );
            break;
        }
        case eQMI_VOICE_SUPS_NOTIFICATION_IND:
        {
            voiceSUPSNotification *pResp =
                &pNotifResp->QmiInd.qaQmiCbkVoiceSUPSNotificationInd;

            /* Set all the TLVs to be in Not-Present State */
            pResp->pCUGIndex = NULL;
            pResp->pECTNum   = NULL;

            /* Unpack the Voice SUPS Event Indication */
            eRCode = UpkQmiCbkVoiceSUPSNotificationInd ( pMdmResp,
                                                         pResp );

            /* Notify to the Callbacks associated */
            qaQmiVoiceSUPSNotify( pResp );
            break;
        }
        case eQMI_VOICE_ALL_CALL_STATUS_IND:
        {
             voiceSetAllCallStatusCbkInfo *pResp =
              &pNotifResp->QmiInd.qaQmiCbkVoiceAllCallStatusInd;

             /* Set all the TLVs to be in Not-Present State */
             pResp->pArrRemotePartyNum  = NULL;
             pResp->pArrRemotePartyName = NULL;
             pResp->pArrAlertingType    = NULL;
             pResp->pArrSvcOption       = NULL;
             pResp->pArrCallEndReason   = NULL;
             pResp->pArrAlphaID         = NULL;
             pResp->pArrConnectPartyNum = NULL;
             pResp->pArrDiagInfo        = NULL;
             pResp->pArrCalledPartyNum  = NULL;
             pResp->pArrRedirPartyNum   = NULL;
             pResp->pArrAlertingPattern = NULL;

             /* Unpack the Voice Call Status Event Indication */
             eRCode = UpkQmiCbkVoiceAllCallStatusInd( pMdmResp,
                                                      pResp );

             /* Notify to the Callbacks associated */
             qaQmiVoiceAllCallStatusNotify( pResp );
             break;
        }

        case eQMI_VOICE_PRIVACY_IND:
        {
            voicePrivacyInfo *pResp =
                     &pNotifResp->QmiInd.qaQmiCbkVoicePrivacyInd;

            /* Unpack the Voice USSD Event Indication */
            eRCode = UpkQmiCbkVoicePrivacyInd( pMdmResp,
                                               pResp );

            /* Notify to the Callbacks associated */
            qaQmiVoicePrivacyNotify( pResp );
            break;
        }

        case eQMI_VOICE_DTMF_IND:
        {
            voiceDTMFEventInfo *pResp =
                &pNotifResp->QmiInd.qaQmiCbkVoiceDTMFEventInd;

            /* Set all the TLVs to be in Not-Present State */
            pResp->pOffLength = NULL;
            pResp->pOnLength  = NULL;

            /* Unpack the Voice SUPS Event Indication */
            eRCode = UpkQmiCbkVoiceDTMFEventInd ( pMdmResp,
                                                  pResp );

            /* Notify to the Callbacks associated */
            qaQmiVoiceDTMFNotify( pResp );
            break;
        }

        case eQMI_VOICE_SUPS_IND:
        {
            voiceSUPSInfo *pResp = &pNotifResp->QmiInd.qaQmiCbkVoiceSupsInd;

            /* Set all the TLVs to be in Not-Present State */
            pResp->pSvcClass       = NULL;
            pResp->pReason         = NULL;
            pResp->pCallFWNum      = NULL;
            pResp->pCallFWTimerVal = NULL;
            pResp->pUSSInfo        = NULL;
            pResp->pCallID         = NULL;
            pResp->pAlphaIDInfo    = NULL;
            pResp->pCallBarPasswd  = NULL;
            pResp->pNewPwdData     = NULL;
            pResp->pFailCause      = NULL;
            pResp->pCallFwdInfo    = NULL;
            pResp->pCLIRstatus     = NULL;
            pResp->pCLIPstatus     = NULL;
            pResp->pCOLPstatus     = NULL;
            pResp->pCOLRstatus     = NULL;
            pResp->pCNAPstatus     = NULL;

            /* Unpack the Voice SUPS Event Indication */
            eRCode = UpkQmiCbkVoiceSUPSInd ( pMdmResp,
                                             pResp );

            /* Notify to the Callbacks associated */
            qaQmiVoiceSUPSIndNotify( pResp );
            break;
        }

       case eQMI_VOICE_OTASP_STATUS_IND:
        {   
            voiceOTASPStatusInfo *pResp =
              &pNotifResp->QmiInd.qaQmiCbkVoiceOTASPStatusInd;

             /* Unpack the OTASP or OTAPA event Indication */
             eRCode = UpkQmiCbkVoiceSetOTASPStatusInd( pMdmResp,
                                                       pResp );

             /* Notify to the Callbacks associated */
             qaQmiVoiceOTASPStatusNotify( pResp );
             break;
        }
   
        case eQMI_VOICE_INFO_REC_IND:
        {
            voiceInfoRec *pResp =
              &pNotifResp->QmiInd.qaQmiCbkVoiceInfoRecInd;

             /* Set all the TLVs to be in Not-Present State */
             pResp->pSignalInfo       = NULL;
             pResp->pCallerIDInfo     = NULL;
             pResp->pDispInfo         = NULL;
             pResp->pExtDispInfo      = NULL;
             pResp->pCallerNameInfo   = NULL;
             pResp->pCallWaitInd      = NULL;
             pResp->pConnectNumInfo   = NULL;
             pResp->pCallingPartyInfo = NULL;
             pResp->pCalledPartyInfo  = NULL;
             pResp->pRedirNumInfo     = NULL;
             pResp->pCLIRCause        = NULL;
             pResp->pNSSAudioCtrl     = NULL;
             pResp->pNSSRelease       = NULL;
             pResp->pLineCtrlInfo     = NULL;
             pResp->pExtDispRecInfo   = NULL;

             /* Unpack the Voice Call Status Event Indication */
             eRCode = UpkQmiCbkVoiceInfoRecInd( pMdmResp,
                                                pResp );

             /* Notify to the Callbacks associated */
             qaQmiVoiceInfoRecNotify( pResp );
             break;
        }
        
        default:
        {
            eRCode = eQCWWAN_ERR_INVALID_ARG;
            break;
        }
    }

    return eRCode;
}
