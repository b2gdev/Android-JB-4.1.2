/*
 * \ingroup sar
 *
 * \file    qaSarSLQSRFGetState.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_SAR_RF_GET_STATE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSarSLQSGetRFSarState.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SarSLQSRFGetState parameters to the QMI
 * message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaGobiApiSar.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiSarSLQSRFGetState(
    WORD  *pMlength,
    BYTE  *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        /* There is no mandatory TLV hence map is empty */
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map
                                     */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_SAR_RF_GET_STATE,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function packs the SarSLQSRFGetState SAR RF State Type field to the
 * QMI message SDU
 *
 * \param  pTlvData   [OUT] - Pointer to storage into which the packed
 *                            data will be placed by this function.
 *
 * \param  pResp       [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvSarSLQSRFGetState( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiSarSLQSRFGetStateResp *lResp=
        (struct QmiSarSLQSRFGetStateResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pRfSarState )
    {
        return eRCode;
    }

    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                            eTLV_SAR_RF_SATATE_LENGTH );
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        /* Add TLV data */
        eRCode = GetLong( pTlvData, lResp->pRfSarState );
    }
    return eRCode;
}

/*
 * This function unpacks the SarSLQSRFGetState response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiSarSLQSRFGetState(
    BYTE                            *pMdmResp,
    struct QmiSarSLQSRFGetStateResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,      &qmUnpackTlvResultCode },
        { eTLV_SAR_RF_GET_STATE, &UnpackTlvSarSLQSRFGetState },
        { eTLV_TYPE_INVALID,     NULL } /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_SAR_RF_GET_STATE );

    return eRCode;
}
