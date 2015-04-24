/*
 * \ingroup cbk
 *
 * \file    qaCbkFmsNotify.c
 *
 * \brief   Contains routines for the FMS Notifications.
 *
 * Copyright: ©  2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiCbk.h"
#include "qaCbkFmsEventReportInd.h"
#include "qaQmiNotify.h"

/*************
 *
 * Name:    qaQmiFmsEventNotify
 *
 * Purpose: To notify all the Callbacks associated with the Fms Event Report
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
local void qaQmiFmsEventNotify(
    struct QmiNotification *pQmiIndication )
{
    void *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_FMS_FW_DWLD_STATUS;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        eDevState CbkParamOne;  /* Device state */
        struct FwDldCompletionTlv *pResp =
            &pQmiIndication->QmiInd.qaQmiCbkFmsEventStatusReportInd.FDCTlv;

        /* The TLV was present in the Indication - hence process */
        /* Included for TLVs which are not implemented */
        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            CbkParamOne = pResp->FwDldCompletionStatus;

            /* Invoke the callback */
            ((tFNDeviceStateChange)pCallback) ( CbkParamOne );
        }
    }
}

/*************
 *
 * Name:    UpkQmiCbkFmsNotification
 *
 * Purpose: Unpacks the received FMS indication and invokes the approriate
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
package enum eQCWWANError UpkQmiCbkFmsNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    switch(QmiMsgID)
    {
        case eQMI_FMS_EVENT_IND:
        {
            struct QmiCbkFmsEventStatusReportInd *pResp =
                &pNotifResp->QmiInd.qaQmiCbkFmsEventStatusReportInd;

            /* Set all the TLVs to be in Not-Present State */
            pResp->FDCTlv.TlvPresent = FALSE;

            /* Unpack the FMS Event Indication */
            eRCode = UpkQmiCbkFmsEventReportInd( pMdmResp,
                                                 pResp );

            /* Notify Callbacks */
            qaQmiFmsEventNotify( pNotifResp );
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
