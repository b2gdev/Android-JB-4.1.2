/*
 * \ingroup pds
 *
 * \file    qaPdsGetDefaultTrackingSession.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_PDS_GET_DEFAULT_TRACKING_SESSION message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaPdsGetDefaultTrackingSession.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * Packs the GetPDSDefaults parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiPdsGetDefaultTrackingSession(
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
    enum eQCWWANError eRCode;
    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_PDS_GET_DEFAULTS,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * Unpacks the GetPDSDefaults operation, timeout, interval and accuracy fields
 * from the QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvDefaultSessionInfo( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiPdsGetDefaultTrackingSessionResp *lResp =
        (struct QmiPdsGetDefaultTrackingSessionResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              lTemp;

    if ( !lResp->pOperation ||
         !lResp->pInterval  ||
         !lResp->pTimeout   ||
         !lResp->pAccuracy )
       return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           eTLV_GET_DEFAULT_SESSION_INFO_LEN );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* operating mode is ULONG * but its size as per document is 1 byte */
    eRCode = GetByte( pTlvData, &lTemp );
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        *(lResp->pOperation) = lTemp;
        /* Max. Time to work on each Fix */
         eRCode = GetByte( pTlvData, lResp->pTimeout );
        /* Interval between Fix requests */
         if ( eQCWWAN_ERR_NONE == eRCode )
         {
             eRCode = GetLong( pTlvData, lResp->pInterval );
             /* Desired Accuracy threshold */
             if ( eQCWWAN_ERR_NONE == eRCode )
                 eRCode = GetLong( pTlvData, lResp->pAccuracy );
         }
    }
    return eRCode;
}

/*
 * Unpacks the GetPDSDefaults response message to a user-provided
 * response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiPdsGetDefaultTrackingSession(
    BYTE                                       *pMdmResp,
    struct QmiPdsGetDefaultTrackingSessionResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,              &qmUnpackTlvResultCode },
        { eTLV_GET_DEFAULT_SESSION_INFO, &UnpackTlvDefaultSessionInfo },
        { eTLV_TYPE_INVALID,             NULL } /* Important. Sentinel.
                                                 * Signifies last item in map.
                                                 */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_PDS_GET_DEFAULTS );
    return eRCode;
}
