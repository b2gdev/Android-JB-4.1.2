/*
 * \ingroup pds
 *
 * \file    qaPdsResetGPSServicedata.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_PDS_RESET_GPS_SERVICE_DATA message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "sludefs.h"
#include "qaPdsResetGPSServicedata.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * Packs the ResetPDSData pGPSDataMask field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvGPSSrvDataMask( BYTE *pBuf, BYTE *pParam )
{
    struct QmiPdsResetGPSServicedataReq *pReq =
        (struct QmiPdsResetGPSServicedataReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pGPSDataMask )
        return eRCode;

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pGPSDataMask) );
}

/*
 * Packs the ResetPDSData pCellDataMask field to the QMI message SDU
 *
 * \param   pBuf[OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam[IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvGPSSrvCellDbMask ( BYTE *pBuf, BYTE *pParam )
{
    struct QmiPdsResetGPSServicedataReq *pReq =
        (struct QmiPdsResetGPSServicedataReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pCellDataMask )
        return eRCode;

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pCellDataMask) );
}

/*
 * Packs the ResetPDSData parameters to the QMI message SDU
 *
 * \param  pParamField[OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength[OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError PkQmiPdsResetGPSServicedata(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG *pGPSDataMask,
    ULONG *pCellDataMask )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_GPS_SRV_DATA_MASK,    &BuildTlvGPSSrvDataMask },
        { eTLV_GPS_SRV_CELL_DB_MASK, &BuildTlvGPSSrvCellDbMask },
        { eTLV_TYPE_INVALID,         NULL } /* Important. Sentinel.
                                             * Signifies last item in map.
                                             */
    };

    struct QmiPdsResetGPSServicedataReq req;

    enum eQCWWANError eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiPdsResetGPSServicedataReq));
    req.pGPSDataMask  = pGPSDataMask;
    req.pCellDataMask = pCellDataMask;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_PDS_RESET_DATA,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * Unpacks the ResetPDSData response message to a user-provided response
 * structure.
 *
 * \param  pMdmResp[IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp[OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError UpkQmiPdsResetGPSServicedata(
    BYTE                                 *pMdmResp,
    struct QmiPdsResetGPSServicedataResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_PDS_RESET_DATA );
    return eRCode;
}
