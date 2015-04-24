/*
 * \ingroup pds
 *
 * \file    qaPdsGetAutoTrackingState.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_PDS_GET_AUTO_TRACKING_STATE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaPdsGetAutoTrackingState.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetServiceAutomaticTracking parameters to the
 * QMI message SDU
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success; eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiPdsGetAutoTrackingState(
    WORD *pMlength,
    BYTE *pParamField )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_PDS_GET_SVC_AUTOTRACK,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the GetServiceAutomaticTracking state from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success; eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvAutoTrackingState( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiPdsGetAutoTrackingStateResp *lResp =
        (struct QmiPdsGetAutoTrackingStateResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    BYTE lByte = 0;

    if ( !lResp->pbAuto )
         return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           eTLV_AUTO_TRACKING_STATE_LENGTH );
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        /* pbAuto is a ULONG * but its size as per document is 1 byte */
        eRCode = GetByte( pTlvData, &lByte );
        if ( eQCWWAN_ERR_NONE == eRCode )
            *(lResp->pbAuto) = lByte;
    }
    return eRCode;
}

/*
 * This function unpacks the GetServiceAutomaticTracking response message
 * to a user-provided response structure.
 *
 * \param  pMdmResp     [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp     [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success; eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiPdsGetAutoTrackingState(
    BYTE                                  *pMdmResp,
    struct QmiPdsGetAutoTrackingStateResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,         &qmUnpackTlvResultCode },
        { eTLV_AUTO_TRACKING_STATE, &UnpackTlvAutoTrackingState },
        { eTLV_TYPE_INVALID,        NULL } /* Important. Sentinel.
                                            * Signifies last item in map.
                                            */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_PDS_GET_SVC_AUTOTRACK );
    return eRCode;
}
