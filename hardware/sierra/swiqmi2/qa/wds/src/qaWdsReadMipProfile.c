/*
 * \ingroup wds
 *
 * \file qaWdsReadMipProfile.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_WDS_READ_MIP_PROFILE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaWdsReadMipProfile.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetMobileIPProfile index field to the
 * QMI message SDU
 *
 * \param  pBuf    - Pointer to storage into which the packed
 *                   data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvIndex(BYTE *pBuf, BYTE *pParam)
 {
    struct QmiWdsReadMipProfileReq *pReq =
                     (struct QmiWdsReadMipProfileReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Add TLV data */
    eRCode = PutByte( pBuf, pReq->index );
    return eRCode;
 }

/*
 * This function packs the GetMobileIPProfile parameters
 * to the QMI message SDU
 *
 * \param  pMlength    [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE      - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Accessed beyond allowed size attempted
 *
 * \sa     qaGobiApiWds.h for remaining parameter descriptions.
 *
 */

enum eQCWWANError PkQmiWdsReadMipProfile(
    WORD  *pMlength,
    BYTE  *pBuffer,
    BYTE  index )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_INDEX,        &BuildTlvIndex },
        { eTLV_TYPE_INVALID, NULL }/* Important. Sentinel.
                                              * Signifies last item in map.
                                              */
    };

    struct QmiWdsReadMipProfileReq req;
    enum eQCWWANError eRCode;

    slmemset((char *)&req, 0, sizeof(struct QmiWdsReadMipProfileReq));
    req.index = index;

    eRCode = qmbuild( pBuffer,
                      (BYTE *)&req,
                      map,
                      eQMI_WDS_GET_MIP_PROFILE,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the GetMobileIPProfile naiSize from the
 * QMI response message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, upon success
 */
enum eQCWWANError UnpackTlvNai(
    BYTE* pTlvData,
    BYTE *pResp )
{
    struct QmiWdsReadMipProfileResp *lResp =
    		         (struct QmiWdsReadMipProfileResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the String Parameter.
     * The size of the string is stored to check for buffer size in API
     */
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pNAI,
                                 lResp->naiSize );
    return eRCode;
}

/*
 * This function unpacks the AAA Server Security Parameter Index from the
 * QMI response message to a user provided response structure.
 *
 * \param pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]     - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvMnAaaSpi(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsReadMipProfileResp *pLresp =
                       (struct QmiWdsReadMipProfileResp* )pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pLresp->pAAASPI )
         return eRCode;

    eRCode = GetLong( pTlvData, pLresp->pAAASPI );
    return eRCode;
}

/*
 * This function unpacks the Home Agent Security Parameter Index from
 * the QMI response message to a user provided response structure.
 *
 * \param pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]     - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvMnHaSpi(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsReadMipProfileResp *pLresp =
                       (struct QmiWdsReadMipProfileResp* )pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pLresp->pHASPI )
        return eRCode;

    eRCode = GetLong( pTlvData, pLresp->pHASPI );
    return eRCode;
}

/*
 * This function unpacks the GetMobileIPProfile RevTunneling from the
 * QMI response message to a user provided response structure.
 *
 * \param pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]     - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvRTPref(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsReadMipProfileResp *pLresp =
                       (struct QmiWdsReadMipProfileResp* )pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pLresp->pRevTunneling )
        return eRCode;

    eRCode = GetByte( pTlvData, pLresp->pRevTunneling );
    return eRCode;
}

/*
 * This function unpacks the GetMobileIPProfile SecondaryHA from the
 * QMI response message to a user provided response structure.
 *
 * \param pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]     - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvHASec(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsReadMipProfileResp *pLresp =
                       (struct QmiWdsReadMipProfileResp* )pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pLresp->pSecondaryHA )
        return eRCode;

    eRCode = GetLong( pTlvData, pLresp->pSecondaryHA );
    return eRCode;
}

/*
 * This function unpacks the GetMobileIPProfile PrimaryHA from the
 * QMI response message to a user provided response structure.
 *
 * \param pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]     - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvHAPri(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsReadMipProfileResp *pLresp =
                       (struct QmiWdsReadMipProfileResp* )pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pLresp->pPrimaryHA )
        return eRCode;

    eRCode = GetLong( pTlvData, pLresp->pPrimaryHA );
    return eRCode;
}

/*
 * This function unpacks the GetMobileIPProfile Address  from the
 * QMI response message to a user provided response structure.
 *
 * \param pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]     - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvHAddress (BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsReadMipProfileResp *pLresp =
                       (struct QmiWdsReadMipProfileResp* )pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pLresp->pAddress )
        return eRCode;

    eRCode = GetLong( pTlvData, pLresp->pAddress );
    return eRCode;
}

/*
 * This function unpacks the GetMobileIPProfile Enabled from the
 * QMI response message to a user provided response structure.
 *
 * \param pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]     - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvState(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsReadMipProfileResp *pLresp =
                       (struct QmiWdsReadMipProfileResp* )pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pLresp->pEnabled )
        return eRCode;

    eRCode = GetByte( pTlvData, pLresp->pEnabled );
    return eRCode;
}

/*
 * This function unpacks the GetMobileIPProfile response message to to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UpkQmiWdsReadMipProfile(
    BYTE   *pMdmResp,
    struct QmiWdsReadMipProfileResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,     &qmUnpackTlvResultCode },
        { eTLV_GSTATE,          &UnpackTlvState },
        { eTLV_GHOME_ADDRESS,   &UnpackTlvHAddress },
        { eTLV_GHOME_AGENT_PRI, &UnpackTlvHAPri },
        { eTLV_GHOME_AGENT_SEC, &UnpackTlvHASec },
        { eTLV_GREV_TUN_PREF,   &UnpackTlvRTPref },
        { eTLV_GNAI,            &UnpackTlvNai },
        { eTLV_GMN_HA_SPI,      &UnpackTlvMnHaSpi },
        { eTLV_GMN_AAA_SPI,     &UnpackTlvMnAaaSpi },
        { eTLV_TYPE_INVALID,    NULL }/* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };
    eRCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_WDS_GET_MIP_PROFILE);
    return eRCode;
}
