/*
 * \ingroup cbk
 *
 * \file    qaCbkOmaDmNotify.c
 *
 * \brief   Contains routines for the OMA DM Notifications.
 *
 * Copyright: ©  2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiCbk.h"
#include "qaCbkOmaDmEventReportInd.h"
#include "qaQmiNotify.h"

/*************
 *
 * Name:    qaQmiOmaDmEventNotify
 *
 * Purpose: To notify all the Callbacks associated with the OMA-DM Event
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
local void qaQmiOmaDmEventNotify(
    struct QmiNotification *pQmiIndication )
{
    void  *pCallback;
    enum  eQMICallbackIndex CbkIndex;
    ULONG CbkParamOne; /* Session State */
    ULONG CbkParamTwo; /* Session Failure Reason */

    CbkIndex  = eQMI_CB_OMADM_STATE;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        struct sessionStateTlv *pResp =
            &pQmiIndication->QmiInd.qaQmiCbkOmaDmEventStatusReportInd.sSTlv;

        /* The TLV was present in the Indication - hence process */
        /* Included for TLVs which are not implemented */
        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            CbkParamOne = pResp->sessionState;
            CbkParamTwo = pResp->sessionFailReason;

            /* Invoke the callback */
            ((tFNOMADMState)pCallback) ( CbkParamOne, CbkParamTwo );
        }
    }
}

/*************
 *
 * Name:    UpkQmiCbkOmaDmNotification
 *
 * Purpose: Unpacks the received OMA-DM indication and invokes the appropriate
 *          callback based on the QMI message type.
 *
 * Parms:   QmiMsgID   - QMI Message ID
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
package enum eQCWWANError UpkQmiCbkOmaDmNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    switch(QmiMsgID)
    {
        case eQMI_OMA_EVENT_IND:
        {
            struct QmiCbkOmaDmEventStatusReportInd *pResp =
                &pNotifResp->QmiInd.qaQmiCbkOmaDmEventStatusReportInd;

            /* Set all the TLVs to be in Not-Present State */
            pResp->sSTlv.TlvPresent = FALSE;

            /* Unpack the OMA-DM Event Indication */
            eRCode = UpkQmiCbkOmaDmEventReportInd( pMdmResp,
                                                   pResp );

            /* Notify to the Callbacks associated */
            qaQmiOmaDmEventNotify( pNotifResp );
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
