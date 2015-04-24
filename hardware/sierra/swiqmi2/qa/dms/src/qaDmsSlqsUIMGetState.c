/**
 * \ingroup dms
 *
 * \file    qaDmsUIMGetState.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_DMS_UIM_GET_STATE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"
#include "qaDmsSlqsUIMGetState.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/**
 * This function packs the UIMGetState parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on failure
 *
 */
enum eQCWWANError PkQmiDmsUIMGetState(
    WORD *pMlength,
    BYTE *pParamField )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_DMS_UIM_GET_STATE,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/**
 * This function unpacks the UIMGetState UIMState from the QMI
 * response  message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on failure
 */
enum eQCWWANError UnpackTlvUIMState( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiDmsUIMGetStateResp *lResp =
        (struct QmiDmsUIMGetStateResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lByte;

    if ( lResp->pUIMState == NULL )
        return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                            eTLV_UIM_GET_STATE_RESP_LEN );
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        /* pUIMState is a ULONG * but its size as per document is 1 byte */
        eRCode = GetByte( pTlvData, &lByte );
        if ( eRCode == eQCWWAN_ERR_NONE )
            *(lResp->pUIMState) = lByte;
    }
    return eRCode;
}

/**
 *
 * This function unpacks the UIMGetState response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on failure
 */
enum eQCWWANError UpkQmiDmsUIMGetState(
    BYTE                         *pMdmResp,
    struct QmiDmsUIMGetStateResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,       &qmUnpackTlvResultCode },
        { eTLV_UIM_GET_STATE,     &UnpackTlvUIMState },
        { eTLV_TYPE_INVALID,      NULL }  /* Important. Sentinel.
                                           * Signifies last item in map.
                                           */
    };

    eRCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_DMS_UIM_GET_STATE);
    return eRCode;
}

