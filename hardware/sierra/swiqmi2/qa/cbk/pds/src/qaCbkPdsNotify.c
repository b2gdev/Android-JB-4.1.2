/*
 * \ingroup cbk
 *
 * \file    qaCbkPdsNotify.c
 *
 * \brief   Contains routines for the PDS Notifications.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiCbk.h"
#include "qaCbkPdsEventReportInd.h"
#include "qaQmiNotify.h"

/* Functions */

/*************
 *
 * Name:    qaQmiPdsEventNotify
 *
 * Purpose: To notify all the Callbacks associated with the PDS Event Report
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
local void qaQmiPdsEventNotify(
    struct QmiNotification *pQmiIndication )
{
    void                   *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_NMEA;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        CHAR *CbkParamOne;
        struct PositionDataNMEATlv *pResp =
           &pQmiIndication->QmiInd.qaQmiCbkPdsEventStatusReportInd.PDNMEATlv;

        /* The TLV was present in the Indication - hence process */
        /* Included for TLVs which are not implemented */
        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            CbkParamOne = pResp->PositionDataNMEA;

            /* Invoke the callback */
            ((tFNNewNMEA)pCallback) ( (LPCSTR)CbkParamOne );
        }
    }
}

/*************
 *
 * Name:    qaQmiPdsGpsServiceStateNotify
 *
 * Purpose: Unpacks the recevied PDS indication and invokes the approriate
 *          callback based on the QMI message type.
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
package void qaQmiPdsGpsServiceStateNotify(
    struct QmiNotification *pQmiIndication )
{
    void *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_PDS_STATE;
    pCallback = qaQmiGetCallback( CbkIndex );
    if(pCallback)
    {
        ULONG CbkParamOne;
        ULONG CbkParamTwo;
        struct GPSServiceStateTlv *pResp =
               &pQmiIndication->QmiInd.qaQmiCbkPdsGpsServiceStatusInd.GPSSSTlv;

        /* invoke the callback if required response TLVs are present */
        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            CbkParamOne = pResp->EnabledStatus;
            CbkParamTwo = pResp->TrackingStatus;

            /* Invoke the callback */
            ((tFNPDSState)pCallback)( CbkParamOne, CbkParamTwo );
        }
    }
}

/*************
 *
 * Name:    UpkQmiCbkPdsNotification
 *
 * Purpose: Unpacks the recevied PDS indication and invokes the approriate
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
package enum eQCWWANError UpkQmiCbkPdsNotification(
    USHORT                  QmiMsgID,
    BYTE                    *pMdmResp,
    struct QmiNotification  *pNotifResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    switch (QmiMsgID)
    {
        case eQMI_PDS_EVENT_IND:
        {
            struct QmiCbkPdsEventStatusReportInd *pResp =
                &pNotifResp->QmiInd.qaQmiCbkPdsEventStatusReportInd;

            /* Set all the TLVs to be in Not-Present State */
            pResp->PDNMEATlv.TlvPresent = FALSE;

            /* Unpack the PDS Event Indication */
            eRCode = UpkQmiCbkPdsEventReportInd( pMdmResp,
                                                 pResp );
            /* Notify to the Callbacks associated */
            qaQmiPdsEventNotify( pNotifResp );
            break;
        }
        case eQMI_PDS_STATE_IND:
        {
            struct QmiCbkPdsGpsServiceStatusInd *pResp =
                &pNotifResp->QmiInd.qaQmiCbkPdsGpsServiceStatusInd;

            /* initialize response TLV presence flag */
            pResp->GPSSSTlv.TlvPresent = FALSE;

            /* Unpack the PDS Event Indication */
            eRCode = UpkQmiCbkPdsGpsServiceStateInd( pMdmResp,
                                                     pResp );
            /* trigger the registered callbacks */
            qaQmiPdsGpsServiceStateNotify( pNotifResp );
            break;
        }
        default:
            break;
    }
    return eRCode;
}

