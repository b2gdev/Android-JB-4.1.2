/*
 * \ingroup wds
 *
 * \file qaWdsGetRuntimeSettings.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_WDS_GET_RUNTIME_SETTINGS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaWdsGetRuntimeSettings.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the GetIPAddress Requested Settings field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvRequestedSettings(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsGetRuntimeSettingsReq *pReq =
                (struct QmiWdsGetRuntimeSettingsReq *)pParam;

    if (pReq->pRequestedSettings == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutLong(pBuf, *(pReq->pRequestedSettings));
}

/*
 * This function packs the GetIPAddress parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 * \sa     qaGobiApiWds.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiWdsGetRuntimeSettings(
    WORD    *pMlength,
    BYTE    *pParamField,
    ULONG    *pRequestedSettings)
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_REQUESTED_SETTINGS, &BuildTlvRequestedSettings },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    struct QmiWdsGetRuntimeSettingsReq req;
    enum eQCWWANError eRCode;

    slmemset((char *)&req, 0, sizeof (struct QmiWdsGetRuntimeSettingsReq));
    req.pRequestedSettings = pRequestedSettings;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_WDS_GET_SETTINGS,
                      pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the GetIPAddress IPAddress from the
 * QMI response message to a user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                               to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvIPAddress(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!pLresp->pIPAddress)
        return eRCode;

    eRCode = GetLong(pTlvData, pLresp->pIPAddress);
    return eRCode;
}

/*
 * This function unpacks the GetIPAddress IPAddressV6 from the
 * QMI response message to a user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                               to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvIPAddressV6(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx = 0;

    if (!pLresp->pIPAddressV6 )
        return eRCode;

    for( idx = 0; idx < 8 ; idx++ )
    {
        if( eQCWWAN_ERR_NONE != 
            ( eRCode = GetWordBe(pTlvData,(pLresp->pIPAddressV6 )++) ) )
        {
            return eRCode;
        }
    }

    if (!pLresp->pIPv6prefixlen)
        return eRCode;

    eRCode = GetByte(pTlvData, pLresp->pIPv6prefixlen);
    return eRCode;
}

/*
 * This function unpacks the StartDataSession2 response message to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UpkQmiWdsGetRuntimeSettings(
    BYTE      *pMdmResp,
    struct    QmiWdsGetRuntimeSettingsResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,     &qmUnpackTlvResultCode },
        { eTLV_IP_ADDRESS,      &UnpackTlvIPAddress},
        { eTLV_IP_ADDRESS_V6,   &UnpackTlvIPAddressV6},
        { eTLV_TYPE_INVALID,    NULL }  /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WDS_GET_SETTINGS );
    return eRCode;
}
