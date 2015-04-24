/*
 * \ingroup cbk
 *
 * \file    qaCbkFmsEventReportInd.c
 *
 * \brief   unpacking routines for the QMI_FMS_EVENT_REPORT_IND message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkFmsEventReportInd.h"

/*
 * Unpacks the firmware download completion TLV from the QMI indication message
 * to a user provided response structure.
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage to return data
 *                     for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvFwDldCompletion(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct FwDldCompletionTlv *lResp =
            &((struct QmiCbkFmsEventStatusReportInd *)pResp)->FDCTlv;

    /* Extract the firmware download completion status */
    enum eQCWWANError eRCode;
    /* RILSTART */
    eRCode = GetLong( pTlvData, &(lResp->FwDldCompletionStatus) );
    /* RILSTOP */

    if ( eRCode == eQCWWAN_ERR_NONE )
        lResp->TlvPresent = TRUE;

    return eRCode;
}

/*
 * Unpacks the Event Report Indication message to a user-provided response
 * structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkFmsEventReportInd(
    BYTE                                 *pMdmResp,
    struct QmiCbkFmsEventStatusReportInd *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        {   eTLV_FMS_IND_FW_DWLD_COMPLETE, &UnpackCbkTlvFwDldCompletion },
        {   eTLV_TYPE_INVALID, NULL } /* signifies last item in map */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_FMS_EVENT_IND );
    return eRCode;
}
