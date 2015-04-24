/*
 * \ingroup wds
 *
 * \file qaWdsSLQSWdsSwiPDPRuntimeSettings.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         eQMI_WDS_SWI_PDP_RUNTIME_SETTINGS message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc., all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaWdsSLQSWdsSwiPDPRuntimeSettings.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the context identifier field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvPDPRSContextId(BYTE *pBuf, BYTE *pParam)
{
    swiPDPRuntimeSettingsReq *pReq = (swiPDPRuntimeSettingsReq *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->contextId );
}

/*
 * This function packs the SLQSWdsSwiPDPRuntimeSettings parameters
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
 */
enum eQCWWANError PkQmiWdsSLQSSwiPDPRuntimeSettings(
    WORD                     *pMlength,
    BYTE                     *pParamField,
    swiPDPRuntimeSettingsReq *pPDPRuntimeSettingsReq )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_PDP_RS_CONTEXT_ID, &BuildTlvPDPRSContextId },
        { eTLV_TYPE_INVALID,      NULL }  /* Important. Sentinel.
                                           * Signifies last item in map.
                                           */
    };

    enum eQCWWANError                      eRCode;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pPDPRuntimeSettingsReq,
                      map,
                      eQMI_WDS_SWI_PDP_RUNTIME_SETTINGS,
                      pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the Context Id from the QMI response message to a
 * user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                 to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvContextID( BYTE *pTlvData, BYTE *pResp )
{
    swiPDPRuntimeSettingsResp *lResp =
    ((struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *)
                                     pResp)->pPDPRuntimeSettingsResp;
   
    /* Check For Invalid Parameter */
    if ( NULL == lResp->pContextId )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData ,lResp->pContextId );
}

/*
 * This function unpacks the Bearer Id from the QMI response message to a
 * user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                 to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvBearerID( BYTE *pTlvData, BYTE *pResp )
{
    swiPDPRuntimeSettingsResp *lResp =
    ((struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *)
                                     pResp)->pPDPRuntimeSettingsResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pBearerId )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData ,lResp->pBearerId );
}

/*
 * This function unpacks the APN Name from the QMI response message to a
 * user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                               to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvRSAPNName( BYTE *pTlvData, BYTE *pResp )
{
    swiPDPRuntimeSettingsResp *lResp =
    ((struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *)
                                     pResp)->pPDPRuntimeSettingsResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    swi_uint16        tlvLength;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pAPNName )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Get the Length of string as the string might not be NULL terminated */
    eRCode = GetStringLen( (swi_uint8 *)pTlvData, &tlvLength );
    if( eQCWWAN_ERR_NONE == eRCode )
    {
        /* Extract the String Parameter */
        eRCode = qmQmiExtractString( pTlvData,
                                     lResp->pAPNName,
                                     tlvLength + 1 );
    }
    return eRCode;
}

/*
 * This function unpacks the IPv4 Address from the QMI response message to a
 * user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRSIPv4Address(BYTE *pTlvData, BYTE *pResp)
{
    swiPDPRuntimeSettingsResp *lResp =
    ((struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *)
                                     pResp)->pPDPRuntimeSettingsResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pIPv4Address )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the IPv4 address */
    return GetLong( pTlvData, lResp->pIPv4Address );
}

/*
 * This function unpacks the IPv4 Gateway Address from the QMI response message
 * to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRSIPv4GWAddress(BYTE *pTlvData, BYTE *pResp)
{
    swiPDPRuntimeSettingsResp *lResp =
    ((struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *)
                                     pResp)->pPDPRuntimeSettingsResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pIPv4GWAddress )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the IPv4 Gateway address */
    return GetLong( pTlvData, lResp->pIPv4GWAddress );
}

/*
 * This function unpacks the Primary DNS IPv4 Address from the QMI response
 * message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRSPrDNSIPv4Address(BYTE *pTlvData, BYTE *pResp)
{
    swiPDPRuntimeSettingsResp *lResp =
    ((struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *)
                                     pResp)->pPDPRuntimeSettingsResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pPrDNSIPv4Address )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the Primary DNS IPv4 Address */
    return GetLong( pTlvData, lResp->pPrDNSIPv4Address );
}

/*
 * This function unpacks the Secondary DNS IPv4 Address from the QMI response
 * message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRSSeDNSIPv4Address(BYTE *pTlvData, BYTE *pResp)
{
    swiPDPRuntimeSettingsResp *lResp =
    ((struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *)
                                     pResp)->pPDPRuntimeSettingsResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pSeDNSIPv4Address )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the Secondary DNS IPv4 Address */
    return GetLong( pTlvData, lResp->pSeDNSIPv4Address );
}

/*
 * This function unpacks the IPv6 Address from the QMI response message to a
 * user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRSIPv6Address(BYTE *pTlvData, BYTE *pResp)
{
    struct IPV6AddressInfo *lResp =
    ((struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *)
                             pResp)->pPDPRuntimeSettingsResp->pIPv6Address;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              lCount;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the IPV6 address */
    for( lCount = 0; lCount < IPV6_ADDRESS_ARRAY_SIZE ; lCount++ )
    {
        eRCode = GetWordBe( pTlvData, &lResp->IPAddressV6[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    /* Extract the IPV6 address length */
    return GetByte( pTlvData, &lResp->IPV6PrefixLen );
}

/*
 * This function unpacks the IPv6 Gateway Address from the QMI response message
 * to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRSIPv6GWAddress(BYTE *pTlvData, BYTE *pResp)
{
    struct IPV6AddressInfo *lResp =
    ((struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *)
                             pResp)->pPDPRuntimeSettingsResp->pIPv6GWAddress;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              lCount;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the IPV6 Gateway address */
    for( lCount = 0; lCount < IPV6_ADDRESS_ARRAY_SIZE ; lCount++ )
    {
        eRCode = GetWordBe( pTlvData, &lResp->IPAddressV6[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    /* Extract the IPV6 Gateway address length */
    return GetByte( pTlvData, &lResp->IPV6PrefixLen );
}

/*
 * This function unpacks the Primary DNS IPv6 Address from the QMI response
 * message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRSPrDNSIPv6Address(BYTE *pTlvData, BYTE *pResp)
{
    swiPDPRuntimeSettingsResp *lResp =
    ((struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *)
                                     pResp)->pPDPRuntimeSettingsResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              lCount;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pPrDNSIPv6Address )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the Primary DNS IPv6 Address */
    for( lCount = 0; lCount < IPV6_ADDRESS_ARRAY_SIZE ; lCount++ )
    {
        eRCode = GetWordBe( pTlvData, &lResp->pPrDNSIPv6Address[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the Secondary DNS IPv6 Address from the QMI response
 * message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRSSeDNSIPv6Address(BYTE *pTlvData, BYTE *pResp)
{
    swiPDPRuntimeSettingsResp *lResp =
    ((struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *)
                                     pResp)->pPDPRuntimeSettingsResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              lCount;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pSeDNSIPv6Address )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the Secondary DNS IPv6 Address */
    for( lCount = 0; lCount < IPV6_ADDRESS_ARRAY_SIZE ; lCount++ )
    {
        eRCode = GetWordBe( pTlvData, &lResp->pSeDNSIPv6Address[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the Primary PCSCF IPv4 Address from the QMI response
 * message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRSPrPCSCFIPv4Address(BYTE *pTlvData, BYTE *pResp)
{
    swiPDPRuntimeSettingsResp *lResp =
    ((struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *)
                                     pResp)->pPDPRuntimeSettingsResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pPrPCSCFIPv4Address )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the Primary PCSCF IPv4 Address */
    return GetLong( pTlvData, lResp->pPrPCSCFIPv4Address );
}

/*
 * This function unpacks the Secondary PCSCF IPv4 Address from the QMI response
 * message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRSSePCSCFIPv4Address(BYTE *pTlvData, BYTE *pResp)
{
    swiPDPRuntimeSettingsResp *lResp =
    ((struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *)
                                     pResp)->pPDPRuntimeSettingsResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pSePCSCFIPv4Address )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the Secondary DNS IPv4 Address */
    return GetLong( pTlvData, lResp->pSePCSCFIPv4Address );
}

/*
 * This function unpacks the Primary PCSCF IPv6 Address from the QMI response
 * message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRSPrPCSCFIPv6Address(BYTE *pTlvData, BYTE *pResp)
{
    swiPDPRuntimeSettingsResp *lResp =
    ((struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *)
                                     pResp)->pPDPRuntimeSettingsResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              lCount;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pPrPCSCFIPv6Address )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the Primary PCSCF IPv6 Address */
    for( lCount = 0; lCount < IPV6_ADDRESS_ARRAY_SIZE ; lCount++ )
    {
        eRCode = GetWordBe( pTlvData, &lResp->pPrPCSCFIPv6Address[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the Secondary PCSCF IPv6 Address from the QMI response
 * message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRSSePCSCFIPv6Address(BYTE *pTlvData, BYTE *pResp)
{
    swiPDPRuntimeSettingsResp *lResp =
    ((struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *)
                                     pResp)->pPDPRuntimeSettingsResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              lCount;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pSePCSCFIPv6Address )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the Secondary PCSCF IPv6 Address */
    for( lCount = 0; lCount < IPV6_ADDRESS_ARRAY_SIZE ; lCount++ )
    {
        eRCode = GetWordBe( pTlvData, &lResp->pSePCSCFIPv6Address[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the SLQSWdsSwiPDPRuntimeSettings response message to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UpkQmiWdsSLQSSwiPDPRuntimeSettings(
    BYTE                                       *pMdmResp,
    struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,                  &qmUnpackTlvResultCode },
        { eTLV_PDP_RS_CONTEXT_ID_RESP,       &UnpackTlvContextID},
        { eTLV_PDP_RS_BEARER_ID,             &UnpackTlvBearerID},
        { eTLV_PDP_RS_APN_NAME,              &UnpackTlvRSAPNName},
        { eTLV_PDP_RS_IPV4_ADDRESS,          &UnpackTlvRSIPv4Address},
        { eTLV_PDP_RS_IPV4_GW_ADDRESS,       &UnpackTlvRSIPv4GWAddress},
        { eTLV_PDP_RS_PR_DNS_IPV4_ADDRESS,   &UnpackTlvRSPrDNSIPv4Address},
        { eTLV_PDP_RS_SE_DNS_IPV4_ADDRESS,   &UnpackTlvRSSeDNSIPv4Address},
        { eTLV_PDP_RS_IPV6_ADDRESS,          &UnpackTlvRSIPv6Address},
        { eTLV_PDP_RS_IPV6_GW_ADDRESS,       &UnpackTlvRSIPv6GWAddress},
        { eTLV_PDP_RS_PR_DNS_IPV6_ADDRESS,   &UnpackTlvRSPrDNSIPv6Address},
        { eTLV_PDP_RS_SE_DNS_IPV6_ADDRESS,   &UnpackTlvRSSeDNSIPv6Address},
        { eTLV_PDP_RS_PR_PCSCF_IPV4_ADDRESS, &UnpackTlvRSPrPCSCFIPv4Address},
        { eTLV_PDP_RS_SE_PCSCF_IPV4_ADDRESS, &UnpackTlvRSSePCSCFIPv4Address},
        { eTLV_PDP_RS_PR_PCSCF_IPV6_ADDRESS, &UnpackTlvRSPrPCSCFIPv6Address},
        { eTLV_PDP_RS_SE_PCSCF_IPV6_ADDRESS, &UnpackTlvRSSePCSCFIPv6Address},
        { eTLV_TYPE_INVALID,                 NULL } /* Important. Sentinel.
                                                     * Signifies last item
                                                     * in map.
                                                     */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WDS_SWI_PDP_RUNTIME_SETTINGS );
    return eRCode;
}
