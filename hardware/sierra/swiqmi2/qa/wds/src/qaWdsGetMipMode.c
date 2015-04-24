/**
 * \ingroup wds
 *
 * \file qaWdsGetMipMode.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_WDS_GET_MIP_MODE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaWdsGetMipMode.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/**
 * This function packs the GetMobileIP parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQMI_ERR_NONE
 * \return eQMI_ERR_NO_MEMORY
 *
 */
enum eQCWWANError PkQmiWdsGetMipMode(
    WORD    *pMlength,
    BYTE    *pParamField)
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;
    eRCode = qmbuild(pParamField,
                     NULL,
                     map,
                     eQMI_WDS_GET_MIP,
                     pMlength);
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/**
 * This function unpacks the GetMobileIP from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE
 */
enum eQCWWANError UnpackTlvMipMode(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetMipModeResp *lResp = (struct QmiWdsGetMipModeResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pMipMode)
        return eRCode;

    eRCode = GetByte(pTlvData, (BYTE *)lResp->pMipMode);
    return eRCode;
}

/**
 * This function unpacks the GetMobileIP response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UpkQmiWdsGetMipMode(
    BYTE   *pMdmResp,
    struct QmiWdsGetMipModeResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_GET_MIP_MODE, &UnpackTlvMipMode },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };

    eRCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_WDS_GET_MIP);
    return eRCode;
}
