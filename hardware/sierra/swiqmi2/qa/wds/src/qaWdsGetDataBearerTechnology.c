/*
 * \ingroup wds
 *
 * \file qaWdsGetDataBearerTechnology.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_WDS_GET_DATA_BEARER message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaWdsGetDataBearerTechnology.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the GetDataBearerTechnology parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_NO_MEMORY, on error
 *
 */
enum eQCWWANError PkQmiWdsGetDataBearerTechnology(
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

    eRCode = qmbuild (pParamField,
                      NULL,
                      map,
                      eQMI_WDS_GET_DATA_BEARER,
                      pMlength);

    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the GetDataBearerTechnology CurrentDataBearer from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvDataBearer(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetDataBearerResp *lResp =
                (struct QmiWdsGetDataBearerResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              lTemp = 0;

    if (!lResp->pCurrentDataBearer)
       return eRCode;

    eRCode = GetByte(pTlvData, &lTemp);
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        *(lResp->pCurrentDataBearer) = lTemp;
    }
    return eRCode;
}

/*
 * This function unpacks the GetDataBearerTechnology response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UpkQmiWdsGetDataBearerTechnology(
    BYTE   *pMdmResp,
    struct QmiWdsGetDataBearerResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_DATABEARER,   &UnpackTlvDataBearer },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    eRCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_WDS_GET_DATA_BEARER );
    return eRCode;
}
