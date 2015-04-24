/*
 * \ingroup cbk
 *
 * \file    qaCbkUimNotify.c
 *
 * \brief   Contains routines for the Uim Notifications
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiCbk.h"
#include "qaQmiNotify.h"

/*************
 *
 * Name:   qaQmiUIMSetRefreshCallBack
 *
 * Purpose: To notify all the Callbacks associated with the UIM Refresh
 *          Indication.
 *
 * Notes:   none
 *
 **************/
local void qaQmiUIMSetRefreshCallBack(
    UIMRefreshEvent *pResp )
{
    void                   *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_UIM_REFRESH_IND;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        /* Invoke the callback */
       ((tFNUIMRefresh)pCallback) ( pResp );
    }
}

/*************
 *
 * Name:    qaQmiUIMSetStatusChangeCallBack
 *
 * Purpose: To notify all the Callbacks associated with the UIM Status Change
 *          Indication.
 *
 * Notes:   none
 *
 **************/
local void qaQmiUIMSetStatusChangeCallBack(
    UIMStatusChangeInfo *pResp )
{
    void                   *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_UIM_STATUS_CHANGE_IND;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        /* Invoke the callback */
       ((tFNUIMStatusChangeInfo)pCallback) ( pResp );
    }
}

/*************
 *
 * Name:   UpkQmiCbkUIMNotification
 *
 * Purpose: Unpacks the received UIM indication and invokes the approriate
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
package enum eQCWWANError UpkQmiCbkUIMNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    switch(QmiMsgID)
    {
        case eQMI_UIM_REFRESH_IND:
        {
            UIMRefreshEvent *pResp =
             &pNotifResp->QmiInd.qaQmiCbkUIMRefreshInd;

            /* Unpack the UIM refresh Event Indication */
            eRCode = UpkQmiCbkUIMSetRefreshInd( pMdmResp,
                                                pResp );

            /* Notify to the Callbacks associated */
            qaQmiUIMSetRefreshCallBack( pResp );
            break;
        }
        case eQMI_UIM_STATUS_CHANGE_IND:
        {
            UIMStatusChangeInfo *pResp =
             &pNotifResp->QmiInd.qaQmiCbkUIMStatusChangeInd;

            /* Unpack the UIM refresh Event Indication */
            eRCode = UpkQmiCbkUIMSetStatusChangeInd( pMdmResp,
                                                     pResp );

            /* Notify to the Callbacks associated */
            qaQmiUIMSetStatusChangeCallBack( pResp );
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
