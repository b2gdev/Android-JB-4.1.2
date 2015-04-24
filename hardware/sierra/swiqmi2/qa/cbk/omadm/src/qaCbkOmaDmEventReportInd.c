/*
 * \ingroup cbk
 *
 * \file    qaCbkOmaDmEventReportInd.c
 *
 * \brief   Contains UnPacking routines for the
 *          QMI_OMA_DM_EVENT_REPORT_IND message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkOmaDmEventReportInd.h"

/* Functions */

/*
 * This function unpacks the OMA-DM session state TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvOmaDMSessionState(
    BYTE *pTlvData,
    BYTE *pResp )
{

    /* Get the required TLV structure to the local pointer */
    struct sessionStateTlv *lResponse =
        &((struct QmiCbkOmaDmEventStatusReportInd *)pResp)->sSTlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lByte;

    /* Extract the paramters from OMA-DM sessionState  Tlv */
    /* Extract the session state */
    eRCode = GetByte( pTlvData, &lByte );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    lResponse->sessionState = lByte;
    lResponse->TlvPresent = TRUE;

    return eRCode;
}

/*
 * This function unpacks the OMA-DM Extended Session Info TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvOmaDMExtSessionInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{

    /* Get the required TLV structure to the local pointer */
    struct sessionStateTlv *lResponse =
        &((struct QmiCbkOmaDmEventStatusReportInd *)pResp)->sSTlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lByte;

    /* Extract the Oma-DM Extended Session Info TLV parameter */
    /* Extract the failure reason */
    eRCode = GetByte( pTlvData, &lByte );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    lResponse->sessionFailReason = lByte;
    lResponse->TlvPresent = TRUE;

    return eRCode;
}

/*
 * This function unpacks the Event Report Indication message to a
 * user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkOmaDmEventReportInd(
    BYTE                                   *pMdmResp,
    struct QmiCbkOmaDmEventStatusReportInd *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_IND_OMA_DM_SessionState,  &UnpackCbkTlvOmaDMSessionState },
        { eTLV_IND_OMA_DM_FailureReason, &UnpackCbkTlvOmaDMExtSessionInfo },
        { eTLV_TYPE_INVALID,             NULL } /* Important. Sentinel.
                                                 * Signifies last item in map.
                                                 */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_OMA_EVENT_IND );
    return eRCode;
}
