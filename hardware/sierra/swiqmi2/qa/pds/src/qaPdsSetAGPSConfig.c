/*
 * \ingroup pds
 *
 * \file    qaPdsSetAGPSConfig.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_PDS_SET_AGPS_CONFIG message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaPdsSetAGPSConfig.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * packs the SetAGPSConfig location server field to the
 * QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvLocationServer( BYTE *pBuf, BYTE *pParam )
{
    struct QmiPdsSetAGPSConfigReq *pReq =
        (struct QmiPdsSetAGPSConfigReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if( (pReq->pServerAddress == NULL) || (pReq->pServerPort == NULL) )
        return eQCWWAN_ERR_NONE;

    /* Add server address in IPv4 format */
    eRCode = PutLong( pBuf, *(pReq->pServerAddress) );

    /* Add server port */
    if ( eRCode == eQCWWAN_ERR_NONE )
        eRCode = PutLong( pBuf, *(pReq->pServerPort) );

    return eRCode;
}

/*
 * packs the SetAGPSConfig location server URL field to the
 * QMI message SDU
 *
 * \param  pBuf         [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param  pParam       [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvLocationServerUrl( BYTE *pBuf, BYTE *pParam )
{
    struct QmiPdsSetAGPSConfigReq *pReq =
        (struct QmiPdsSetAGPSConfigReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE urlLength;
    BYTE *pServerUrl;

    if( (pReq->pServerURL == NULL) || (pReq->pServerURLLength == NULL) )
        return eQCWWAN_ERR_NONE;

    urlLength  = *(pReq->pServerURLLength);
    pServerUrl = pReq->pServerURL;

    /* Add server URL length  */
    eRCode = PutByte( pBuf, *(pReq->pServerURLLength) );

    /* Add server URL  */
    while(urlLength--)
    {
        if ( eRCode == eQCWWAN_ERR_NONE )
            eRCode = PutByte( pBuf, *(pServerUrl++) );
    }
    return eRCode;
}

/*
 * This function packs the SetAGPSConfig parameters to the QMI message SDU
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiPdsSetAGPSConfig(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG *pServerAddress,
    ULONG *pServerPort,
    BYTE  *pServerURL,
    BYTE  *pServerURLLength )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_AGPS_LOCATION_SERVER,     &BuildTlvLocationServer },
        { eTLV_SET_AGPS_LOCATION_SERVER_URL, &BuildTlvLocationServerUrl },
        { eTLV_TYPE_INVALID,                 NULL }/* Important. Sentinel.
                                                    * Signifies last item in map
                                                    */
    };

    struct QmiPdsSetAGPSConfigReq req;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiPdsSetAGPSConfigReq));
    req.pServerAddress   = pServerAddress;
    req.pServerPort      = pServerPort;
    req.pServerURL       = pServerURL;
    req.pServerURLLength = pServerURLLength;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_PDS_SET_AGPS_CONFIG,
                      pMlength );
    return eRCode;
}


/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SetAGPSConfig response message to a user-provided
 * response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiPdsSetAGPSConfig(
    BYTE                           *pMdmResp,
    struct QmiPdsSetAGPSConfigResp *pApiResp )
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
                           eQMI_PDS_SET_AGPS_CONFIG );
    return eRCode;
}
