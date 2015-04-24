/*
 * \ingroup cbk
 *
 * \file    qaCbkWdsNotify.c
 *
 * \brief   Contains routines for the WDS Notifications.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiCbk.h"
#include "qaCbkWdsEventReportInd.h"
#include "qaCbkWdsGetPktSrvcStatusInd.h"
#include "qaQmiNotify.h"

/* Functions */

/*************
 *
 * Name:    qaQmiWdsPktStatusNotify
 *
 * Purpose: Unpacks the recevied WDS indication and invokes the approriate
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
package void qaQmiWdsPktStatusNotify(
    struct QmiNotification *pQmiIndication )
{
    void *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_SESSION_STATE;
    pCallback = qaQmiGetCallback( CbkIndex );
    if(pCallback)
    {
        ULONG CbkParamOne;
        ULONG CbkParamTwo;

        /* Extract the Parameters */
        CbkParamOne =
            pQmiIndication->QmiInd.qaQmiCbkWdsSessionStateInd.state;

        CbkParamTwo =
            pQmiIndication->QmiInd.qaQmiCbkWdsSessionStateInd.sessionEndReason;

        /* Invoke the callback */
        ((tFNSessionState)pCallback)( CbkParamOne, CbkParamTwo );
    }
}

/*************
 *
 * Name:    qaQmiWdsEventNotify
 *
 * Purpose: To notify all the Callbacks associated with the WDS Event Report
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
local void qaQmiWdsEventNotify(
    struct QmiNotification *pQmiIndication )
{
    void                   *pCallback;
    enum eQMICallbackIndex CbkIndex;
    ULONG                  CbkParamOne;
    ULONGLONG              BTCbkParamOne, BTCbkParamTwo;

    CbkIndex  = eQMI_CB_DATA_BEARER;
    pCallback = qaQmiGetCallback( CbkIndex );

    if ( pCallback )
    {
        struct DataBearerTechTlv *pResp =
            &pQmiIndication->QmiInd.qaQmiCbkWdsEventStatusReportInd.DBTechTlv;

        struct CurrentDataBearerTechTlv *pResp1 =
            &pQmiIndication->QmiInd.qaQmiCbkWdsEventStatusReportInd.CDBTechTlv;

        /* The TLV was present in the Indication - hence process */
        if( pResp->TlvPresent || pResp1->TlvPresent )
        {
            /* Extract the Parameters */
            /* If both TLV's are present in the indication use current data
               bearer technology TLV */
            CbkParamOne = (pResp1->TlvPresent)?
                pResp1->RATMask : pResp->DataBearerTechnology;

            /* Invoke the callback */
            ((tFNDataBearer)pCallback) ( CbkParamOne );
        }
    }

    CbkIndex  = eQMI_CB_DORMANCY_STATUS;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        struct DormancyStatusTlv *pResp =
                &pQmiIndication->QmiInd.qaQmiCbkWdsEventStatusReportInd.DSTlv;

        /* The TLV was present in the Indication - hence process */
        /* Included for TLVs which are not implemented */

        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            CbkParamOne = pResp->DormancyStatus;

            /* Invoke the callback */
            ((tFNDormancyStatus)pCallback) ( CbkParamOne );
        }
    }

    CbkIndex  = eQMI_CB_MOBILE_IP;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        struct MobileIPStatusTlv *pResp =
                &pQmiIndication->QmiInd.qaQmiCbkWdsEventStatusReportInd.MSTlv;

        /* The TLV was present in the Indication - hence process */
        /* Included for TLVs which are not implemented */

        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            CbkParamOne = pResp->MipStatus;

            /* Invoke the callback */
            ((tFNMobileIPStatus)pCallback) ( CbkParamOne );
        }
    }

    CbkIndex  = eQMI_CB_BYTE_TOTALS;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        struct ByteTotalsTlv *pResp =
               &pQmiIndication->QmiInd.qaQmiCbkWdsEventStatusReportInd.BTTlv;

        /* The TLV was present in the Indication - hence process */
        /* Included for TLVs which are not implemented */
        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            BTCbkParamOne = pResp->TxTotalBytes;

            BTCbkParamTwo = pResp->RxTotalBytes;

            /* Invoke the callback */
            ( (tFNByteTotals)pCallback) ( BTCbkParamOne, BTCbkParamTwo );
        }
    }
}

/*************
 *
 * Name:    UpkQmiCbkWdsNotification
 *
 * Purpose: Unpacks the recevied WDS indication and invokes the approriate
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
package enum eQCWWANError UpkQmiCbkWdsNotification(
    USHORT                  QmiMsgID,
    BYTE                    *pMdmResp,
    struct QmiNotification  *pNotifResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    switch (QmiMsgID)
    {
        case eQMI_WDS_EVENT_IND:
        {
            struct QmiCbkWdsEventStatusReportInd *pResp =
                &pNotifResp->QmiInd.qaQmiCbkWdsEventStatusReportInd;

            /* Set all the TLVs to be in Not-Present State */
            pResp->DBTechTlv.TlvPresent  = FALSE;
            pResp->DSTlv.TlvPresent      = FALSE;
            pResp->MSTlv.TlvPresent      = FALSE;
            pResp->BTTlv.TlvPresent      = FALSE;
            pResp->CDBTechTlv.TlvPresent = FALSE;

            /* Unpack the WDS Event Indication */
            eRCode = UpkQmiCbkWdsEventReportInd( pMdmResp,
                                                 pResp );
            /* Notify to the Callbacks associated */
            qaQmiWdsEventNotify( pNotifResp );
            break;
        }
        case eQMI_WDS_PKT_STATUS_IND:
        {
            struct QmiCbkWdsSessionStateInd *pResp =
                &pNotifResp->QmiInd.qaQmiCbkWdsSessionStateInd;

            /* Unpack the WDS Packet Status Indication */
            eRCode = UpkQmiCbkWdsGetPktSrvcStatusInd( pMdmResp,
                                                      pResp );
            /* Notify to the Callbacks associated */
            qaQmiWdsPktStatusNotify( pNotifResp );
            break;
        }
        default:
            break;
    }
    return eRCode;
}

