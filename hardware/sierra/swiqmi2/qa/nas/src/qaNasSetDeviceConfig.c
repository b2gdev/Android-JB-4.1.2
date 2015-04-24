/*
 * \ingroup : nas
 *
 * \file    : qaNasSetDeviceConfig.c
 *
 * \brief   : Contains Packing and UnPacking routines for the
 *            eQMI_NAS_SET_NET_PARAMS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaNasSetDeviceConfig.h"


/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the qaNasSetDeviceConfig SPC field to the
 * QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE  on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvSetSPC( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetDeviceConfigReq *pReq =
                     (struct QmiNasSetDeviceConfigReq *)pParam;
    enum eQCWWANError eRCode;
    BYTE              count;

    if ( pReq->pSpc == NULL )
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    for ( count = 0; count < 6; count++ )
    {
        eRCode = PutByte( pBuf, *(pReq->pSpc)++ );
        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
    }
    return eRCode;
}

/*
 * This function packs the qaNasSetDeviceConfig HDRRev field to the
 * QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE  on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvForceHdrRev( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetDeviceConfigReq *pReq =
                     (struct QmiNasSetDeviceConfigReq *)pParam;

    if ( pReq->pForceHdrRev == NULL )
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pForceHdrRev) );
}

/*
 * This function packs the qaNasSetDeviceConfig HDRSPCCustomConfig field to the
 * QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE  on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvHdrScpCustomConfig( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetDeviceConfigReq *pReq =
                     (struct QmiNasSetDeviceConfigReq *)pParam;
    enum eQCWWANError eRCode;

    if ( pReq->pState       == NULL ||
         pReq->pProtocol    == NULL ||
         pReq->pBroadcast   == NULL ||
         pReq->pApplication == NULL )
        return eQCWWAN_ERR_NONE;
    /* Add TLV data */
    eRCode = PutByte( pBuf, *(pReq->pState) );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;
    eRCode = PutLong( pBuf, *(pReq->pProtocol) );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;
    eRCode = PutLong( pBuf, *(pReq->pBroadcast) );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;
    eRCode = PutLong( pBuf, *(pReq->pApplication) );
    return eRCode;
}

/*
 * This function packs the qaNasSetDeviceConfig RoamingPreference field to the
 * QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE  on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError BuildTlvRoamPref( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetDeviceConfigReq *pReq =
                     (struct QmiNasSetDeviceConfigReq *)pParam;

    if ( pReq->pRoaming == NULL )
         return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pRoaming) );
}

/*
 * This function packs the SetDeviceConfig parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE  on success, eQCWWAN_ERR_XXX on error
 *
 * \sa     qaGobiApiNas.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiNasSetDeviceConfig(
    WORD  *pMlength,
    BYTE  *pParamField,
    CHAR  *pSpc,
    BYTE  *pForceHdrRev,
    BYTE  *pState,
    ULONG *pProtocol,
    ULONG *pBroadcast,
    ULONG *pApplication,
    ULONG *pRoaming)
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_SPC,               &BuildTlvSetSPC },
        { eTLV_FORCE_HDR_REV,         &BuildTlvForceHdrRev },
        { eTLV_HDR_SCP_CUSTOM_CONFIG, &BuildTlvHdrScpCustomConfig },
        { eTLV_ROAM_PREF,             &BuildTlvRoamPref },
        { eTLV_TYPE_INVALID,          NULL } /* Important. Sentinel.
                                              * Signifies last item in map.
                                              */
    };

    struct QmiNasSetDeviceConfigReq req;
    enum   eQCWWANError             eRCode;

    /* Initialize the structure and populate the values */
    slmemset((char *)&req, 0, sizeof(struct QmiNasSetDeviceConfigReq));
    req.pSpc         = pSpc;
    req.pForceHdrRev = pForceHdrRev;
    req.pState       = pState;
    req.pProtocol    = pProtocol;
    req.pBroadcast   = pBroadcast;
    req.pApplication = pApplication;
    req.pRoaming     = pRoaming;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_NAS_SET_NET_PARAMS,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 *****************************************************************************/

/*
 * This function unpacks the eQMI_NAS_SET_NET_PARAMS response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE  on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError UpkQmiNasSetDeviceConfig(
    BYTE                             *pMdmResp,
    struct QmiNasSetDeviceConfigResp *pApiResp)
{
    enum eQCWWANError eRCode;

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
                           eQMI_NAS_SET_NET_PARAMS );
    return eRCode;
}

