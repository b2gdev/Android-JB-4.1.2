/*
 * \ingroup nas
 *
 * \file qaNasGetDeviceConfig.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         eQMI_NAS_GET_NET_PARAMS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaNasGetDeviceConfig.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetDeviceConfig parameters
 * to the QMI message SDU
 *
 * \param       pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param       pMlength    [OUT] - Total length of built message.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, Access beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiNasGetDeviceConfig(
    WORD *pMlength,
    BYTE *pParamField )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    enum eQCWWANError eRCode;
    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_NAS_GET_NET_PARAMS,
                      pMlength );
    return eRCode;
}


/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the Slot Cycle Index from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UnpackTlvGetSCI( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetDeviceConfigResp *lResp =
        (struct QmiNasGetDeviceConfigResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->pSCI == NULL )
        return eQCWWAN_ERR_NONE;

    eRCode = GetByte( pTlvData, lResp->pSCI );
    return eRCode;
}

/*
 * This function unpacks the Station Class Mark from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UnpackTlvGetSCM( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetDeviceConfigResp *lResp =
        (struct QmiNasGetDeviceConfigResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->pSCM == NULL )
        return eQCWWAN_ERR_NONE;

    eRCode = GetByte( pTlvData, lResp->pSCM );
    return eRCode;
}

/*
 * This function unpacks the Registration Parameters from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UnpackTlvGetRegParams( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetDeviceConfigResp *lResp =
        (struct QmiNasGetDeviceConfigResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->pRegHomeSID    == NULL ||
         lResp->pRegForeignSID == NULL ||
         lResp->pRegForeignNID == NULL )
         return eQCWWAN_ERR_NONE;

    eRCode = GetByte( pTlvData, lResp->pRegHomeSID );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;
    eRCode = GetByte( pTlvData, lResp->pRegForeignSID );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;
    eRCode = GetByte( pTlvData, lResp->pRegForeignNID );

    return eRCode;
}

/*
 * This function unpacks the Force HDR Revision from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UnpackTlvGetForceHdrRev( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetDeviceConfigResp *lResp =
        (struct QmiNasGetDeviceConfigResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->pForceRev0 == NULL )
        return eQCWWAN_ERR_NONE;

    eRCode = GetByte( pTlvData, lResp->pForceRev0 );

    return eRCode;
}

/*
 * This function unpacks the HDR SCP Custom Config from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UnpackTlvGetHdrSpcCustomConfig( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetDeviceConfigResp *lResp =
        (struct QmiNasGetDeviceConfigResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->pCustomSCP   == NULL ||
         lResp->pProtocol    == NULL ||
         lResp->pBroadcast   == NULL ||
         lResp->pApplication == NULL )
         return eQCWWAN_ERR_NONE;

    eRCode = GetByte( pTlvData, lResp->pCustomSCP );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;
    eRCode = GetLong( pTlvData, lResp->pProtocol );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;
    eRCode = GetLong( pTlvData, lResp->pBroadcast );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;
    eRCode = GetLong( pTlvData, lResp->pApplication );

    return eRCode;
}

/*
 * This function unpacks the Roaming Preference from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UnpackTlvGetRoamPref( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetDeviceConfigResp *lResp =
        (struct QmiNasGetDeviceConfigResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lTemp;

    if ( lResp->pRoaming == NULL )
        return eQCWWAN_ERR_NONE;

    /* pRoaming is a ULONG * but its size as per the document is 1 byte */
    eRCode             = GetByte( pTlvData, &lTemp );
    *(lResp->pRoaming) = lTemp;

    return eRCode;
}

/*
 * This function unpacks the GetDeviceConfig response message to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 * \return    eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UpkQmiNasGetDeviceConfig(
    BYTE                             *pMdmResp,
    struct QmiNasGetDeviceConfigResp *pApiResp)
{
	enum eQCWWANError rCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,               &qmUnpackTlvResultCode },
        { eTLV_GET_SCI,                   &UnpackTlvGetSCI },
        { eTLV_GET_SCM,                   &UnpackTlvGetSCM },
        { eTLV_REG_PARAMS,                &UnpackTlvGetRegParams },
        { eTLV_GET_FORCE_HDR_REV,         &UnpackTlvGetForceHdrRev },
        { eTLV_GET_HDR_SPC_CUSTOM_CONFIG, &UnpackTlvGetHdrSpcCustomConfig },
        { eTLV_GET_ROAM_PREF,             &UnpackTlvGetRoamPref },
        { eTLV_TYPE_INVALID,              NULL } /* Important. Sentinel.
                                                  * Signifies last item in map.
                                                  */
    };
    rCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_NAS_GET_NET_PARAMS );
    return rCode;
}
