/*
 * \ingroup cbk
 *
 * \file    qaCbkWmsNotify.c
 *
 * \brief   Contains routines for the WMS Notifications.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiCbk.h"
#include "qaQmiNotify.h"
#include "sludefs.h"

/* Functions */

/*************
 *
 * Name:    qaQmiWmsEventNotify
 *
 * Purpose: To notify all the Callbacks associated with the WMS Event Report
 *          Notification.
 *
 * Parms:   pQmiIndication  [IN] - pointer to structure used to store all QMI
 *                                 Notification parameters.
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   Must be called after qminit() has been called
 *
 **************/
local void qaQmiWmsEventNotify(
    struct QmiNotification *pQmiIndication )
{
    void                   *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_NEW_SMS;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        struct NewMTMessageTlv *pResp =
               &pQmiIndication->QmiInd.qaQmiCbkWmsEventReportInd.NewMMTlv;
        ULONG CbkParamOne;
        ULONG CbkParamTwo;

        /* invoke the callback if required response TLVs are present */
        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            CbkParamOne = pResp->MTMessageInfo.storageType;
            CbkParamTwo = pResp->MTMessageInfo.messageIndex;

            /* Invoke the callback */
            (( tFNNewSMS )pCallback) ( CbkParamOne, CbkParamTwo );
        }
    }

    CbkIndex  = eQMI_CB_SMS_EVENT;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        /* Initialize the SMS event info parameters */
        SMSEventInfo SMSEvents;
        slmemset((char *)&SMSEvents, 0, sizeof(SMSEventInfo));

        /* Check whether a mobile terminated SMS event is received
         * Initialize the structure with the information
         */
        struct NewMTMessageTlv *pRespOne =
               &pQmiIndication->QmiInd.qaQmiCbkWmsEventReportInd.NewMMTlv;
        if ( pRespOne->TlvPresent )
        {
            SMSEvents.smsEventType |= SMS_EVENT_MT_MESSAGE;
            SMSEvents.pMTMessageInfo = &pRespOne->MTMessageInfo;
        }

        /* Check whether a message mode event is received
         * Initialize the structure with the information
         */
        struct MessageModeTlv *pRespTwo =
               &pQmiIndication->QmiInd.qaQmiCbkWmsEventReportInd.MMTlv;
        if ( pRespTwo->TlvPresent )
        {
            SMSEvents.smsEventType |= SMS_EVENT_MESSAGE_MODE;
            SMSEvents.pMessageModeInfo = &pRespTwo->MessageModeInfo;
        }

        /* Check whether a transfer route mobile termintaed SMS event 
         * is received, initialize the structure with the information
         */
        struct TransferRouteMessageTlv *pRespThree =
               &pQmiIndication->QmiInd.qaQmiCbkWmsEventReportInd.TRMessageTlv;
        if ( pRespThree->TlvPresent )
        {
            SMSEvents.smsEventType |= SMS_EVENT_TRANSFER_ROUTE_MT_MESSAGE;
            SMSEvents.pTransferRouteMTMessageInfo =
                                        &pRespThree->TransferRouteMTMessageInfo;
        }

        /* Check whether a ETWS SMS event is received,
         * initialize the structure with the information
         */
        struct SMSEtwsMessageTlv *pRespFour =
               &pQmiIndication->QmiInd.qaQmiCbkWmsEventReportInd.ETWSTlv;
        if ( pRespFour->TlvPresent )
        {
            SMSEvents.smsEventType |= SMS_EVENT_ETWS;
            SMSEvents.pEtwsMessageInfo = &pRespFour->EtwsMessageInfo;
        }

        /* Check whether a ETWS PLMN event is received,
         * initialize the structure with the information
         */
        struct ETWSPLMNInfoTlv *pRespFive =
               &pQmiIndication->QmiInd.qaQmiCbkWmsEventReportInd.ETWSPLMNTlv;
        if ( pRespFive->TlvPresent )
        {
            SMSEvents.smsEventType |= SMS_EVENT_ETWS_PLMN;
            SMSEvents.pEtwsPlmnInfo = &pRespFive->ETWSPLMNInfo;
        }

        /* Check whether a SMSC address event is received,
         * initialize the structure with the information
         */
        struct SMSCAddressTlv *pRespSix =
               &pQmiIndication->QmiInd.qaQmiCbkWmsEventReportInd.SMSCTlv;
        if ( pRespSix->TlvPresent )
        {
            SMSEvents.smsEventType |= SMS_EVENT_SMSC_ADDRESS;
            SMSEvents.pSMSCAddressInfo = &pRespSix->SMSCInfo;
        }

        /* Check whether a SMS on IMS event is received,
         * initialize the structure with the information
         */
        struct SMSOnIMSTlv *pRespSeven =
               &pQmiIndication->QmiInd.qaQmiCbkWmsEventReportInd.IMSTlv;
        if ( pRespSeven->TlvPresent )
        {
            SMSEvents.smsEventType |= SMS_EVENT_SMS_ON_IMS;
            SMSEvents.pSMSOnIMSInfo = &pRespSeven->IMSInfo;
        }

        /* Invoke the callback */
        (( tFNSMSEvents )pCallback) ( &SMSEvents );
    }
}

/*************
 *
 * Name:    qaQmiWmsTransLayerNotify
 *
 * Purpose: To notify the callback associated with the WMS Transport layer
 *          Info indication
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
local void qaQmiWmsTransLayerNotify(
    transLayerNotification *pResp )
{
    void                   *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_WMS_TRANS_LAYER_INFO_IND;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        /* Invoke the callback */
        (( tFNtransLayerInfo )pCallback) ( pResp );
    }
}

/*************
 *
 * Name:    qaQmiQmsTransNWRegNotify
 *
 * Purpose: To notify the callback associated with the WMS Transport NW Reg
 *          Info indication
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
local void qaQmiWmsTransNWRegNotify(
    transNWRegInfoNotification *pResp )
{
    void                   *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_WMS_TRANS_NW_REG_INFO_IND;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        /* Invoke the callback */
        (( tFNtransNWRegInfo )pCallback) ( pResp );
    }
}

/*************
 *
 * Name:    qaQmiWmsMemoryFullInd
 *
 * Purpose: To notify the callback associated with the WMS Memory full
 *          status Info indication
 *
 * Parms:   pResp  [IN] - pointer to structure used to store all QMI
 *                                 Notification parameters.
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void qaQmiWmsMemoryFullInd(
    SMSMemoryInfo *pResp )
{
    void                   *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_WMS_MEMORY_FULL;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        /* Invoke the callback */
        (( tFNMemoryFull )pCallback) ( pResp );
    }
}

/*************
 *
 * Name:    UpkQmiCbkWmsNotification
 *
 * Purpose: Unpacks the received WMS indication and invokes the appropriate
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
package enum eQCWWANError UpkQmiCbkWmsNotification(
    USHORT                  QmiMsgID,
    BYTE                    *pMdmResp,
    struct QmiNotification  *pNotifResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    switch (QmiMsgID)
    {
        case eQMI_WMS_EVENT_IND:
        {
            struct QmiCbkWmsEventReportInd *pResp =
                &pNotifResp->QmiInd.qaQmiCbkWmsEventReportInd;

            /* initialize response TLV presence flag */
            pResp->NewMMTlv.TlvPresent     = FALSE;
            pResp->MMTlv.TlvPresent        = FALSE;
            pResp->TRMessageTlv.TlvPresent = FALSE;
            pResp->ETWSTlv.TlvPresent      = FALSE;
            pResp->ETWSPLMNTlv.TlvPresent  = FALSE;
            pResp->SMSCTlv.TlvPresent      = FALSE;
            pResp->IMSTlv.TlvPresent       = FALSE;

            /* Unpack the WMS Event Indication */
            eRCode = UpkQmiCbkWmsEventReportInd( pMdmResp,
                                                 pResp );

            /* trigger the registered callbacks */
            qaQmiWmsEventNotify( pNotifResp );
            break;
        }
        case eQMI_WMS_TRANS_LAYER_INFO_IND:
        {

            transLayerNotification *pResp =
                &pNotifResp->QmiInd.qaQmiCbkTransLayerNotificationInd;

            //Set the Optional Tlv to not present state
            pResp->pTransLayerInfo = NULL;

            /* Unpack the WMS Transport Layer Info indication */
            eRCode = UpkQmiCbkWmsTransLayerInfoInd( pMdmResp,
                                                    pResp );

            /* Trigger the registered callback */
            qaQmiWmsTransLayerNotify( pResp );

            break;
        }
        case eQMI_WMS_TRANS_NW_REG_INFO_IND:
        {
            transNWRegInfoNotification *pResp =
                &pNotifResp->QmiInd.qaQmiCbkTransNWRegNotificationInd;

            //Unpack the WMS transport NW registration info indication */
            eRCode = UpkQmiCbkWmsTransNWRegInfoInd( pMdmResp,
                                                    pResp );

            /* Trigger the registered callback */
            qaQmiWmsTransNWRegNotify( pResp );

            break;
        }
        case eQMI_WMS_MEMORY_FULL_IND:
        {
            SMSMemoryInfo *pResp =
                &pNotifResp->QmiInd.qaQmiCbkWmsMemoryFull;

            /* Unpack the WMS Memory Full status Info indication */
            eRCode = UpkQmiCbkWmsMemoryFullInfoInd( pMdmResp,
                                                    pResp );

            /* Trigger the registered callback */
            qaQmiWmsMemoryFullInd( pResp );

            break;
        }

        default:
            break;
    }
    return eRCode;
}
