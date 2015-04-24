/*
 * \ingroup wds
 *
 * \file qaWdsGetDormancyState.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_WDS_GET_DORMANCY message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaWdsGetDormancyState.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the GetDormancyState parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError PkQmiWdsGetDormancyState(
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
                     eQMI_WDS_GET_DORMANCY,
                     pMlength);
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the GetDormancyState from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvDormancyState(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetDormancyResp *lResp = (struct QmiWdsGetDormancyResp *)pResp;
    BYTE lTemp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pDormancyState)
        return eRCode;

    /* pDormancyState is a ULONG * but its size asper the document is 1 byte */
    eRCode = GetByte(pTlvData, &lTemp);
    *(lResp->pDormancyState) = lTemp;
    return eRCode;
}

/*
 * This function unpacks the GetDormancyState response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UpkQmiWdsGetDormancyState(
    BYTE   *pMdmResp,
    struct QmiWdsGetDormancyResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,     &qmUnpackTlvResultCode },
        { eTLV_DORMANCY_STATE,  &UnpackTlvDormancyState },
        { eTLV_TYPE_INVALID,    NULL }  /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };

    eRCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_WDS_GET_DORMANCY );
    return eRCode;
}
