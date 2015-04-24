/*
 * \ingroup cbk
 *
 * \file    qaCbkDcsNotify.c
 *
 * \brief   Contains routines for the DCS Notifications.
 *
 * Copyright: ©  2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiCbk.h"
#include "qaCbkDcsEventReportInd.h"
#include "qaQmiNotify.h"

/*************
 *
 * Name:    qaQmiDcsEventNotify
 *
 * Purpose: To notify all the Callbacks associated with the Dcs Event
 *          Report Notification.
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
local void qaQmiDcsEventNotify(
    struct QmiNotification *pQmiIndication )
{
    void *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex = eQMI_CB_DCS_DEVICE_STATE_CHANGE;
    pCallback = qaQmiGetCallback( CbkIndex );

    struct deviceStateChangeTlv *pResp =
            &pQmiIndication->QmiInd.qaQmiCbkDcsEventStatusReportInd.DSCTlv;

    if ( pResp->TlvPresent )
    {
        /* Extract the Parameters */
        eDevState CbkParamOne;  /* Device state */
        CbkParamOne = pResp->deviceState;

        /* Notify the callback thread for device state change */
        qaQmiDeviceStateChangeCbk( CbkParamOne );

        /* Invoke the callback subscribed by the user*/
        if ( pCallback )
        {
            ((tFNDeviceStateChange)pCallback) ( CbkParamOne );
        }
    }

    CbkIndex = eQMI_CB_DCS_SDK_TERMINATED;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        struct SDKTerminatedTlv *pResp =
                &pQmiIndication->QmiInd.qaQmiCbkDcsEventStatusReportInd.SDKTlv;

        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            BYTE *CbkParamTwo;
            CbkParamTwo = pResp->Data;

            /* Invoke the callback */
            ((tFNSDKTerminated)pCallback) ( CbkParamTwo );
        }
    }

}

/*************
 *
 * Name:    UpkQmiCbkDcsNotification
 *
 * Purpose: Unpacks the received DCS indication and invokes the approriate
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
package enum eQCWWANError UpkQmiCbkDcsNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    switch(QmiMsgID)
    {
        case eQMI_DCS_EVENT_IND:
        {
            struct QmiCbkDcsEventStatusReportInd *pResp =
                &pNotifResp->QmiInd.qaQmiCbkDcsEventStatusReportInd;

            /* Set all the TLVs to be in Not-Present State */
            pResp->DSCTlv.TlvPresent = FALSE;

            /* Unpack the DCS Event Indication */
            eRCode = UpkQmiCbkDcsEventReportInd( pMdmResp,
                                                 pResp );

            /* Notify Callbacks */
            qaQmiDcsEventNotify( pNotifResp );
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
