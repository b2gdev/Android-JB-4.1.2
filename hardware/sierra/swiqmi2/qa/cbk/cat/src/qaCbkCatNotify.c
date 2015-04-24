/*
 * \ingroup cbk
 *
 * \file    qaCbkCatNotify.c
 *
 * \brief   Contains routines for the CAT Notifications.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiCbk.h"
#include "qaCbkCatEventReportInd.h"
#include "qaQmiNotify.h"

/* Functions */

/*************
 *
 * Name:    qaQmiCatEventNotify
 *
 * Purpose: To notify all the Callbacks associated with the CAT Event Report
 *          Notification.
 *
 * Parms:   pQmiIndication  [IN] - pointer to structure used to store all QMI
 *                                 Notification parameters.
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   Must be called after qminit() has been called
 *
 **************/
local void qaQmiCatEventNotify(
    struct QmiNotification *pQmiIndication )
{
    void                   *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_CAT_EVENT;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        ULONG CbkParamOne;
        ULONG CbkParamTwo;
        BYTE *CbkParamThree;
        BYTE lIndex, lMaxIndex;
        struct CatCommonEventTlv *pResp =
           pQmiIndication->QmiInd.qaQmiCbkCatEventStatusReportInd.CCETlv;

        /* Extract the Number of Events Received from the Indication */
        lMaxIndex =
            pQmiIndication->QmiInd.qaQmiCbkCatEventStatusReportInd.event_Index;

        /* Invode the Callback for all of the TLVs received in the Indication */
        for ( lIndex = 0; lIndex < lMaxIndex; lIndex++ )
        {
            /* The TLV was present in the Indication - hence process */
            /* Included for TLVs which are not implemented */
            if ( pResp[lIndex].TlvPresent )
            {
                /* Extract the Parameters */
                CbkParamOne   = pResp[lIndex].EventID;
                CbkParamTwo   = pResp[lIndex].EventLength;
                if( CbkParamOne == eTLV_DISPLAY_TEXT         ||
                    CbkParamOne == eTLV_GET_IN_KEY           ||
                    CbkParamOne == eTLV_GET_INPUT            ||
                    CbkParamOne == eTLV_SETUP_MENU           ||
                    CbkParamOne == eTLV_SELECT_ITEM          ||
                    CbkParamOne == eTLV_SETUP_IDLE_MODE_TEXT ||
                    CbkParamOne == eTLV_LANGUAGE_NOTIFICATION )
                {
                    CbkParamThree = (BYTE *)&pResp[lIndex].CatEvent.CatEvIDData;
                }
                else if( CbkParamOne == eTLV_ALPHA_IDENTIFIER )
                    CbkParamThree = (BYTE *)&pResp[lIndex].CatEvent.\
                                                            CatAlphaIdtfr;
                else if( eTLV_SETUP_EVENT_LIST )
                    CbkParamThree = (BYTE *)&pResp[lIndex].CatEvent.CatEventLst;
                else if( eTLV_REFRESH )
                    CbkParamThree = (BYTE *)&pResp[lIndex].CatEvent.CatRefresh;
                else if( eTLV_END_PROACTIVE_SESSION )
                    CbkParamThree = (BYTE *)&pResp[lIndex].CatEvent.CatEndPS;

                /* Invoke the callback */
                ((tFNCATEvent)pCallback) ( CbkParamOne,
                                           CbkParamTwo,
                                           CbkParamThree );
            }
        }
    }
}


/*************
 *
 * Name:    UpkQmiCbkCatNotification
 *
 * Purpose: Unpacks the recevied CAT indication and invokes the approriate
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
package enum eQCWWANError UpkQmiCbkCatNotification(
    USHORT                  QmiMsgID,
    BYTE                    *pMdmResp,
    struct QmiNotification  *pNotifResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lIndx;
    switch (QmiMsgID)
    {
        case eQMI_CAT_EVENT_IND:
        {
            struct QmiCbkCatEventStatusReportInd *pResp =
                &pNotifResp->QmiInd.qaQmiCbkCatEventStatusReportInd;

            /* initialize response TLV presence flag */
            for( lIndx = 0; lIndx < QMI_CAN_COMMON_EVENT_TLV_NUMBER; lIndx++ )
                pResp->CCETlv[lIndx].TlvPresent = FALSE;

            /* Unpack the CAT Event Indication */
            eRCode = UpkQmiCbkCatEventReportInd( pMdmResp,
                                                 pResp );

            /* trigger the registered callbacks */
            qaQmiCatEventNotify( pNotifResp );
            break;
        }

        default:
            break;
    }
    return eRCode;
}

