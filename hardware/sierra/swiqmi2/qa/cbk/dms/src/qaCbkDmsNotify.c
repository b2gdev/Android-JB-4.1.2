/*
 * \ingroup cbk
 *
 * \file    qaCbkDmsNotify.c
 *
 * \brief   Contains routines for the DMS Notifications.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiCbk.h"
#include "qaCbkDmsEventReportInd.h"
#include "qaQmiNotify.h"


/*************
 *
 * Name:    qaQmiDmsEventNotify
 *
 * Purpose: To notify all the Callbacks associated with the DMS Event Report
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
local void qaQmiDmsEventNotify(
    struct QmiNotification *pQmiIndication )
{
    void  *pCallback;
    enum  eQMICallbackIndex CbkIndex;
    ULONG CbkParamOne;

    CbkIndex  = eQMI_CB_POWER;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        struct OperatingModeTlv *pResp =
                &pQmiIndication->QmiInd.qaQmiCbkDmsEventStatusReportInd.OMTlv;

        /* The TLV was present in the Indication - hence process */
        /* Included for TLVs which are not implemented */
        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            CbkParamOne = pResp->operatingMode;

            /* Invoke the callback */
            ((tFNPower)pCallback) ( CbkParamOne );
        }
    }

    CbkIndex  = eQMI_CB_ACTIVATION_STATUS;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        struct ActivationStatusTlv *pResp =
                &pQmiIndication->QmiInd.qaQmiCbkDmsEventStatusReportInd.ASTlv;

        /* The TLV was present in the Indication - hence process */
        /* Included for TLVs which are not implemented */
        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            CbkParamOne = pResp->activationStatus;

            /* Invoke the callback */
            ((tFNActivationStatus)pCallback) ( CbkParamOne );
        }
    }
}

/*************
 *
 * Name:    UpkQmiCbkDmsNotification
 *
 * Purpose: Unpacks the recevied DMS indication and invokes the approriate
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
package enum eQCWWANError UpkQmiCbkDmsNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    switch(QmiMsgID)
    {
        case eQMI_DMS_EVENT_IND:
        {
            struct QmiCbkDmsEventStatusReportInd *pResp =
                &pNotifResp->QmiInd.qaQmiCbkDmsEventStatusReportInd;

            /* Set all the TLVs to be in Not-Present State */
            pResp->OMTlv.TlvPresent = FALSE;
            pResp->ASTlv.TlvPresent = FALSE;

            /* Unpack the WDS Event Indication */
            eRCode = UpkQmiCbkDmsEventReportInd( pMdmResp,
                                                 pResp );
            /* Notify to the Callbacks associated */
            qaQmiDmsEventNotify( pNotifResp );
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

