/*
 * \ingroup wds
 *
 * \file qaWdsGetDefaultSettings.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_WDS_GET_DEFAULTS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "amudefs.h"

#include "qaQmiBasic.h"
#include "qaWdsGetDefaultSettings.h"

#define WDS_QMI_MSG_IPV6_LEN            16
#define WDS_QMI_MSG_PRIMARY_DNSV6_LEN   16
#define WDS_QMI_MSG_SECONDARY_DNSV6_LEN 16

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetDefaultProfile profileType field
 *                 to the QMI message SDU
 *
 * \param pBuf    - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvProfileType(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsGetDefaultSettingsReq *pReq =
                        (struct QmiWdsGetDefaultSettingsReq *)pParam;

    /* Add TLV data */
    return PutByte(pBuf, pReq->profileType);
}

/*
 * This function packs the GetDefaultProfile parameters
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
enum eQCWWANError PkQmiWdsGetDefaultSettings(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG profileType )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_PROFILE_TYPE      ,&BuildTlvProfileType },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;
    struct QmiWdsGetDefaultSettingsReq req;
    slmemset((char *)&req, 0, sizeof(struct QmiWdsGetDefaultSettingsReq));
    req.profileType   = profileType;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_WDS_GET_DEFAULTS,
                      pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the GetDefaultProfile PDPType from the
 *                QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvPDPType( BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetDefaultSettingsResp *lResp =
                        (struct QmiWdsGetDefaultSettingsResp *)pResp;
    BYTE lTemp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pPDPType)
        return eRCode;

    /* pPDPType is a ULONG * but its size asper the document is 1 byte */
    eRCode = GetByte(pTlvData, &lTemp );
    *(lResp->pPDPType) = lTemp;

    return eRCode;
}

/*
 * This function unpacks the GetDefaultProfile IpAddress from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvIpAddress(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetDefaultSettingsResp *lResp =
                        (struct QmiWdsGetDefaultSettingsResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pIPAddress)
        return eRCode;

    eRCode = GetLong(pTlvData, lResp->pIPAddress);

    return eRCode;
}

/*
 * This function unpacks the GetDefaultProfile PrimaryDNS from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvPrimaryDNS(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetDefaultSettingsResp *lResp =
                        (struct QmiWdsGetDefaultSettingsResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pPrimaryDNS)
        return eRCode;

    eRCode = GetLong(pTlvData, lResp->pPrimaryDNS);

    return eRCode;
}

/*
 * This function unpacks the GetDefaultProfile SecondaryDNS from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvSecondaryDNS(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetDefaultSettingsResp *lResp =
                        (struct QmiWdsGetDefaultSettingsResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pSecondaryDNS)
        return eRCode;

    eRCode = GetLong(pTlvData, lResp->pSecondaryDNS);

    return eRCode;
}

/*
 * This function unpacks the GetDefaultProfileLTE IpAddressv6 from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvIpAddressv6(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetDefaultSettingsResp *lResp =
                        (struct QmiWdsGetDefaultSettingsResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx;

    if (!lResp->pIPAddressv6)
        return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           WDS_QMI_MSG_IPV6_LEN );
    if (eRCode != eQCWWAN_ERR_NONE)
        return eRCode;

    for (idx = 0; idx < 8; idx++)
    {
        eRCode = GetWordBe(pTlvData, (lResp->pIPAddressv6)++);
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the GetDefaultProfileLTE PrimaryDNSv6 from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvPrimaryDNSv6(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetDefaultSettingsResp *lResp =
                        (struct QmiWdsGetDefaultSettingsResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx;

    if (!lResp->pPrimaryDNSv6)
        return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           WDS_QMI_MSG_PRIMARY_DNSV6_LEN );
    if (eRCode != eQCWWAN_ERR_NONE)
        return eRCode;

    for (idx = 0; idx < 8; idx++)
    {
        eRCode = GetWord(pTlvData, (lResp->pPrimaryDNSv6)++);
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the GetDefaultProfileLTE SecondaryDNSv6 from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvSecondaryDNSv6(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetDefaultSettingsResp *lResp =
                        (struct QmiWdsGetDefaultSettingsResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx;

    if (!lResp->pSecondaryDNSv6)
        return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *) pTlvData,
                                           WDS_QMI_MSG_SECONDARY_DNSV6_LEN );
    if (eRCode != eQCWWAN_ERR_NONE)
        return eRCode;

    for (idx = 0; idx < 8; idx++)
    {
        eRCode = GetWord(pTlvData, (lResp->pSecondaryDNSv6)++);
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the GetDefaultProfile Authentication from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvAuthentication(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetDefaultSettingsResp *lResp =
                        (struct QmiWdsGetDefaultSettingsResp *)pResp;
    BYTE lTemp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pAuthentication)
        return eRCode;

    /*
     *  pAuthentication is a ULONG * but its size as
     *  per the document is 1 byte
     */
    eRCode = GetByte(pTlvData, &lTemp);
    *(lResp->pAuthentication) = lTemp;

    return eRCode;
}

/*
 * This function unpacks the GetDefaultProfile ProfileName from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvName(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetDefaultSettingsResp *lResp =
                        (struct QmiWdsGetDefaultSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pName )
        return eQCWWAN_ERR_NONE; 
    
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pName,
                                 lResp->nameSize );

    return eRCode;
}

/*
 * This function unpacks the GetDefaultProfile APNname from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvAPNName(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetDefaultSettingsResp *lResp =
                        (struct QmiWdsGetDefaultSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pName )
        return eQCWWAN_ERR_NONE; 

    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pAPNName,
                                 lResp->apnSize );

    return eRCode;
}

/*
 * This function unpacks the GetDefaultProfile UserName from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvUserName(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetDefaultSettingsResp *lResp =
                        (struct QmiWdsGetDefaultSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pUserName )
        return eQCWWAN_ERR_NONE; 

    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pUserName,
                                 lResp->userSize );
    return eRCode;
}

/*
 * This function unpacks the GetDefaultProfile response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UpkQmiWdsGetDefaultSettings(
    BYTE   *pMdmResp,
    struct QmiWdsGetDefaultSettingsResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,      &qmUnpackTlvResultCode },
        { eTLV_PDP_TYPE,         &UnpackTlvPDPType},
        { eTLV_IP_ADDR,          &UnpackTlvIpAddress},
        { eTLV_PRIMARY_DNS,      &UnpackTlvPrimaryDNS},
        { eTLV_SECONDARY_DNS,    &UnpackTlvSecondaryDNS},
        { eTLV_IP_ADDR_V6,       &UnpackTlvIpAddressv6},
        { eTLV_PRIMARY_DNS_V6,   &UnpackTlvPrimaryDNSv6},
        { eTLV_SECONDARY_DNS_V6, &UnpackTlvSecondaryDNSv6},
        { eTLV_AUTHENTICATION,   &UnpackTlvAuthentication},
        { eTLV_PROFILE_NAME,     &UnpackTlvName},
        { eTLV_APNNAME,          &UnpackTlvAPNName},
        { eTLV_USER_NAME,        &UnpackTlvUserName},
        { eTLV_TYPE_INVALID,     NULL } /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };

    eRCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_WDS_GET_DEFAULTS );
    return eRCode;
}
