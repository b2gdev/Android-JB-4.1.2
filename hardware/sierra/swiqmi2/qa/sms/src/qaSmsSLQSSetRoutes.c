/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSSetRoutes.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_WMS_SET_ROUTES message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSmsSLQSSetRoutes.h"

/*****************************************************************************
 * Request handling
 *****************************************************************************/

/*
 * Packs the route list field for SLQSSetRoutes API to the QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvRouteList( BYTE *pBuf, BYTE *pParam )
{
    smsSetRoutesReq *pReq = (smsSetRoutesReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD lTemp = 0, index = 0;

    if (pReq == NULL)
    {
        return eRCode;
    }

    lTemp = pReq->numOfRoutes;

    /* Add Number of Routes */
    eRCode = PutWord( pBuf, pReq->numOfRoutes );
    if (eRCode != eQCWWAN_ERR_NONE)
        return eRCode;

    while (lTemp--)
    {
        /* Add message type parameter */
        eRCode = PutByte( pBuf, pReq->routeList[index].messageType );
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;

        /* Add message class parameter */
        eRCode = PutByte( pBuf, pReq->routeList[index].messageClass );
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;

        /* Add route storage parameter */
        eRCode = PutByte( pBuf, pReq->routeList[index].routeStorage );
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;

        /* Add receipt action */
        eRCode = PutByte( pBuf, pReq->routeList[index++].receiptAction );
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;
    }

    return eRCode;
}

/*
 * Packs the transfer status report events field for SLQSSetRoutes API to 
 * the QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvTransferSR( BYTE *pBuf, BYTE *pParam )
{
    smsSetRoutesReq *pReq = (smsSetRoutesReq *)pParam;

    if (pReq == NULL)
    {
       return eQCWWAN_ERR_NONE;
    }
    else if (pReq->pTransferStatusReport == NULL)
    {
       return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pTransferStatusReport) );
}

/*
 * This function packs the SLQS Set Routes request parameters to
 * the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed data
 *                              will be placed by this function.
 *
 * \param pSetRoutesReq [IN]  - structure with values to be packed
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiSmsSLQSSetRoutes(
    WORD            *pMlength,
    BYTE            *pParamField,
    smsSetRoutesReq *pSetRoutesReq )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_ROUTE_LIST,   &BuildTlvRouteList },
        { eTLV_TRANSFER_SR,  &BuildTlvTransferSR },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel. Signifies last item
                                     * in map
                                     */
    };

    return qmbuild( pParamField,
                    (BYTE *)pSetRoutesReq,
                    map,
                    eQMI_WMS_SET_ROUTES,
                    pMlength );
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SLQS Set Routes response to
 * a user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiSmsSLQSSetRoutes(
    BYTE                           *pMdmResp,
    struct QmiSmsSLQSSetRoutesResp *pApiResp )
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
                           eQMI_WMS_SET_ROUTES );
    return eRCode;
}
