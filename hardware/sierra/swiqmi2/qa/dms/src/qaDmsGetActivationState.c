/**
 * \ingroup dms
 *
 * \file    qaDmsGetActivationState.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_DMS_GET_ACTIVATED_STATE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"
#include "qaDmsGetActivationState.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/**
 * This function packs the GetActivationState parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on failure
 *
 */
enum eQCWWANError PkQmiDmsGetActivationState(
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
                      eQMI_DMS_GET_ACTIVATED_STATE,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/**
 * This function unpacks the GetActivationState ActivationState from the QMI
 * response  message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on failure
 */
enum eQCWWANError UnpackTlvActivationState( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiDmsGetActivationStateResp *lResp =
        (struct QmiDmsGetActivationStateResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD tempWord;

    if ( lResp->pActivationState == NULL )
        return eRCode;

    eRCode = GetWord( pTlvData, &tempWord );
    *(lResp->pActivationState) = tempWord;

    return eRCode;
}

/**
 *
 * This function unpacks the GetActivationState response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on failure
 */
enum eQCWWANError UpkQmiDmsGetActivationState(
    BYTE                                *pMdmResp,
    struct QmiDmsGetActivationStateResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,       &qmUnpackTlvResultCode },
        { eTLV_ACTIVATION_STATE,  &UnpackTlvActivationState },
        { eTLV_TYPE_INVALID,      NULL }  /* Important. Sentinel.
                                           * Signifies last item in map.
                                           */
    };

    eRCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_DMS_GET_ACTIVATED_STATE);
    return eRCode;
}

