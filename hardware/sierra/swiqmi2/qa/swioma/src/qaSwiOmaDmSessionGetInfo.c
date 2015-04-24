/*
 * \ingroup swioma
 *
 * \file    qaSwiOmaDmSessionGetInfo.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_SWIOMA_DM_SESSION_GET_INFO message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSwiOmaDmSessionGetInfo.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SLQSOMADMGetSessionInfo parameters to the QMI message
 * SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaGobiApiSwiOmadms.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiSwiOmaDmSessionGetInfo(
    WORD  *pMlength,
    BYTE  *pParamField )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID,     NULL } /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_SWIOMA_GET_SESSION_INFO,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SLQSOMADMGetSessionInfo Session Info from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvSessionInfo( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiSwiOmaDmSessionGetInfoResp *lResp =
        (struct QmiSwiOmaDmSessionGetInfoResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              lByte  = 0;
    BYTE              lSessionType;

    if ( !lResp->pSessionType ||
         !lResp->pSessionState )
         return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                          eTLV_GET_OMA_DM_SESSION_INFO_LENGTH );

    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        eRCode = GetByte( pTlvData, &lSessionType );
        if ( eQCWWAN_ERR_NONE == eRCode )
        {
            if( lSessionType == *(lResp->pSessionType) )
            {
            eRCode = GetByte( pTlvData, &lByte );
            if ( eQCWWAN_ERR_NONE == eRCode )
                *(lResp->pSessionState) = lByte;
            }
            else
                return eQCWWAN_ERR_INVALID_ARG;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the SLQSOMADMGetSessionInfo response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp     [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp     [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiSwiOmaDmSessionGetInfo(
    BYTE                                 *pMdmResp,
    struct QmiSwiOmaDmSessionGetInfoResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,             &qmUnpackTlvResultCode },
        { eTLV_GET_OMA_DM_SESSION_INFO, &UnpackTlvSessionInfo },
        { eTLV_TYPE_INVALID,            NULL } /* Important. Sentinel.
                                                * Signifies last item in map.
                                                */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_SWIOMA_GET_SESSION_INFO );
    return eRCode;
}
