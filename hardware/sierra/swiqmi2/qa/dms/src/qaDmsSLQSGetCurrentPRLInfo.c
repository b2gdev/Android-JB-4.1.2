/*
 * \ingroup dms
 *
 * \file    qaDmsSLQSGetCurrentPRLInfo.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_DMS_GET_CURRENT_PRL_INFO message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiDms.h"
#include "qaDmsSLQSGetCurrentPRLInfo.h"

/*****************************************************************************
 * Request handling
 *****************************************************************************/
/*
 * This function packs the SLQS Get Current PRL request parameters to
 * the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed data
 *                              will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiDmsSLQSGetCurrentPRLInfo(
    WORD *pMlength,
    BYTE *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }/* Important. Sentinel.
                                    * Signifies last item in map
                                    */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode =  qmbuild( pParamField,
                       NULL,
                       map,
                       eQMI_DMS_GET_CURRENT_PRL_INFO,
                       pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 *****************************************************************************/
/*
 * Unpacks the PRL Version from the QMI response message to a user
 * provided response structure
 *
 * \param       pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]    - Pointer to structure containing storage
 *                                  to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvPRLVersionInfo( BYTE *pTlvData, BYTE *pResp )
{
    dmsCurrentPRLInfo *lResp =
        ( ( struct QmiDMSSLQSGetCurrentPRLInfoResp * )pResp )->pCurrentPRLInfo;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pPRLVersion )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the parameters into the structure */
    return GetWord( pTlvData, lResp->pPRLVersion );
}

/*
 * Unpacks the PRL Preference from the QMI response message to a user
 * provided response structure
 *
 * \param       pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]    - Pointer to structure containing storage
 *                                  to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvPRLPreference( BYTE *pTlvData, BYTE *pResp )
{
    dmsCurrentPRLInfo *lResp =
        ( ( struct QmiDMSSLQSGetCurrentPRLInfoResp * )pResp )->pCurrentPRLInfo;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pPRLPreference )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the parameters into the structure */
    return GetByte( pTlvData, lResp->pPRLPreference );
}

/*
 * This function unpacks the SLQSGetCurrentPRLInfo response to a user-provided
 * response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiDmsSLQSGetCurrentPRLInfo(
    BYTE                                   *pMdmResp,
    struct QmiDMSSLQSGetCurrentPRLInfoResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,        &qmUnpackTlvResultCode },
        { eTLV_DMS_PRL_VERSION,    &UnpackTlvPRLVersionInfo },
        { eTLV_DMS_PRL_PREFERENCE, &UnpackTlvPRLPreference },
        { eTLV_TYPE_INVALID,       NULL } /* Important. Sentinel.
                                           * Signifies last item in map.
                                           */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_GET_CURRENT_PRL_INFO );
    return eRCode;
}
