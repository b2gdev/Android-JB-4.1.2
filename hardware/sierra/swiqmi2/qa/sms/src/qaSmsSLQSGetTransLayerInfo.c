/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSGetTransLayerInfo.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_WMS_GET_TRANSPORT_LAYER_INFO message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSmsSLQSGetTransLayerInfo.h"

/*****************************************************************************
 * Request handling
 *****************************************************************************/

/*
 * This function packs the SLQS Get Transport Layer Info request parameters to
 * the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiSmsSLQSGetTransLayerInfo(
    WORD *pMlength,
    BYTE *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID,    NULL } /* Important. Sentinel.
                                        * Signifies last item in map
                                        */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_WMS_GET_TRANS_LAYER_INFO,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Transport layer registered indication from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRegInd(
    BYTE *pTlvData,
    BYTE *pResp )
{
    getTransLayerInfoResp *lResp =
        ((struct QmiSmsSLQSGetTransLayerInfoResp *)pResp)->pGetTransLayerResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pRegInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pRegInd );
}

/*
 * This function unpacks the transport layer info from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvTransLayerInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    getTransLayerInfoResp *tLInfoResp =
        ((struct QmiSmsSLQSGetTransLayerInfoResp *) pResp)->pGetTransLayerResp;

    transLayerInfo *lResp = tLInfoResp->pTransLayerInfo;

    enum eQCWWANError eRCode;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Transport Type response into the structure */
    eRCode = GetByte( pTlvData, &lResp->TransType );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Copy the Transport capabilities into the structure */
    eRCode = GetByte( pTlvData, &lResp->TransCap );
    return eRCode;
}

/*
 * This function unpacks the SLQS Get Transport Layer Info response to
 * a user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiSmsSLQSGetTransLayerInfo(
    BYTE                                   *pMdmResp,
    struct QmiSmsSLQSGetTransLayerInfoResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,          &qmUnpackTlvResultCode },
        { eTLV_TRANS_LAYER_REG_INFO, &UnpackTlvRegInd },
        { eTLV_TRANS_LAYER_INFO,     &UnpackTlvTransLayerInfo },
        { eTLV_TYPE_INVALID,         NULL } /* Important. Sentinel.
                                             * Signifies last item in map.
                                             */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WMS_GET_TRANS_LAYER_INFO );
    return eRCode;
}
