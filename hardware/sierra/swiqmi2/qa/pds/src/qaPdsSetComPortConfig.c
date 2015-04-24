/*
 * \ingroup pds
 *
 * \file    qaPdsSetComPortConfig.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_PDS_SET_COM_PORT_CONFIG message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaPdsSetComPortConfig.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SetPortAutomaticTracking bAuto field to the
 * QMI message SDU
 *
 * \param  pBuf         [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param  pParam       [IN]  - Pointer to structure containing data
 *                              for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvComPortConfig( BYTE *pBuf, BYTE *pParam )
{
    struct QmiPdsSetComPortConfigReq *pReq =
        (struct QmiPdsSetComPortConfigReq *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->bAuto );
}

/*
 * This function packs the SetPortAutomaticTracking parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError PkQmiPdsSetComPortConfig(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG bAuto )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_COM_PORT_CONFIG, &BuildTlvComPortConfig },
        { eTLV_TYPE_INVALID,        NULL } /* Important. Sentinel.
                                            * Signifies last item in map
                                            */
    };

    struct QmiPdsSetComPortConfigReq req;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiPdsSetComPortConfigReq));
    req.bAuto = bAuto;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_PDS_SET_COM_AUTOTRACK,
                      pMlength );
    return eRCode;
}


/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the PdsSetComportConfig response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError UpkQmiPdsSetComPortConfig(
    BYTE                              *pMdmResp,
    struct QmiPdsSetComPortConfigResp *pApiResp )
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
                           eQMI_PDS_SET_COM_AUTOTRACK );
    return eRCode;
}
