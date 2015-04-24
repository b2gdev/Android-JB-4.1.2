/*
 * \ingroup wds
 *
 * \file    qaWdsStartNetworkInterface.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_WDS_START_NET message
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaWdsStartNetworkInterface.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the StartDataSession TechPreference field to the
 * QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvTechPreference(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsStartNetworkInterfaceReq *pReq =
                      (struct QmiWdsStartNetworkInterfaceReq *)pParam;

    if (pReq->pTechnology == NULL)
        return eQCWWAN_ERR_NONE;

    if( QMI_WDS_TECH_UMTS == *(pReq->pTechnology) )
    {
        *(pReq->pTechnology) = QMI_WDS_TECH_UMTS_VALUE;
    }

    if( QMI_WDS_TECH_CDMA == *(pReq->pTechnology) )
    {
        *(pReq->pTechnology) = QMI_WDS_TECH_CDMA_VALUE;
    }

    if( QMI_WDS_TECH_eMBMS == *(pReq->pTechnology) )
    {
        *(pReq->pTechnology) = QMI_WDS_TECH_eMBMS_VALUE;
    }

    if( QMI_WDS_TECH_mLINK == *(pReq->pTechnology) )
    {
        *(pReq->pTechnology) = QMI_WDS_TECH_mLINK_VALUE;
    }
    /* Add TLV data */
    return PutWord(pBuf, (WORD)*(pReq->pTechnology));
}

/*
 * This function packs the StartDataSession PrimaryDnsPreference field
 * to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvPrimaryDnsPreference(BYTE *pBuf, BYTE  *pParam)
{
    struct QmiWdsStartNetworkInterfaceReq *pReq =
                       (struct QmiWdsStartNetworkInterfaceReq *)pParam;

    if (pReq->pPrimaryDNS == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutLong(pBuf, *(pReq->pPrimaryDNS));
}

/*
 * This function packs the StartDataSession PrimaryDnsPreferencev6 field
 * to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvPrimaryDnsPreferencev6(BYTE *pBuf, BYTE  *pParam)
{
    struct QmiWdsStartNetworkInterfaceReq *pReq =
                       (struct QmiWdsStartNetworkInterfaceReq *)pParam;

    if (pReq->pPrimaryDNSv6 == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutLong(pBuf, *(pReq->pPrimaryDNSv6));
}

/*
 * This function packs the StartDataSession SecondaryDnsPreference
 * field to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvSecondaryDnsPreference(BYTE *pBuf, BYTE  *pParam)
{
    struct QmiWdsStartNetworkInterfaceReq *pReq =
                       (struct QmiWdsStartNetworkInterfaceReq *)pParam;

    if (pReq->pSecondaryDNS == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutLong(pBuf, *(pReq->pSecondaryDNS));
}

/*
 * This function packs the StartDataSession SecondaryDnsPreferencev6
 * field to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvSecondaryDnsPreferencev6(BYTE *pBuf, BYTE  *pParam)
{
    struct QmiWdsStartNetworkInterfaceReq *pReq =
                       (struct QmiWdsStartNetworkInterfaceReq *)pParam;

    if (pReq->pSecondaryDNSv6 == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutLong(pBuf, *(pReq->pSecondaryDNSv6));
}

/*
 * This function packs the StartDataSession PrimaryNbnsPreference
 * field to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvPrimaryNbnsPreference(BYTE *pBuf, BYTE  *pParam)
{
    struct QmiWdsStartNetworkInterfaceReq *pReq =
                       (struct QmiWdsStartNetworkInterfaceReq *)pParam;

    if (pReq->pPrimaryNBNS == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutLong(pBuf, *(pReq->pPrimaryNBNS));
}

/*
 * This function packs the StartDataSession SecondaryNbnsPreference
 * field to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvSecondaryNbnsPreference(BYTE *pBuf, BYTE  *pParam)
{
    struct QmiWdsStartNetworkInterfaceReq *pReq =
                       (struct QmiWdsStartNetworkInterfaceReq *)pParam;

    if (pReq->pSecondaryNBNS == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutLong(pBuf, *(pReq->pSecondaryNBNS));
}

/*
 * This function packs the StartDataSession BuildTlvApnName
 * field to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvApnName(BYTE *pBuf, BYTE  *pParam)
{
    struct QmiWdsStartNetworkInterfaceReq *pReq =
                       (struct QmiWdsStartNetworkInterfaceReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (pReq->pAPNName == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    while( *(pReq->pAPNName) != EOS)
    {
        PutByte(pBuf, *(pReq->pAPNName)++);
        if( eQCWWAN_ERR_NONE != eRCode )
            break;
    }
    return eRCode;
}

/*
 * This function packs the StartDataSession AddrPreference
 * field to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvAddrPreference(BYTE *pBuf, BYTE  *pParam)
{
    struct QmiWdsStartNetworkInterfaceReq *pReq =
                       (struct QmiWdsStartNetworkInterfaceReq *)pParam;

    if (pReq->pIPAddress == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutLong(pBuf, *(pReq->pIPAddress));
}

/*
 * This function packs the StartDataSession AddrPreferencev6
 * field to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvAddrPreferencev6(BYTE *pBuf, BYTE  *pParam)
{
    struct QmiWdsStartNetworkInterfaceReq *pReq =
                       (struct QmiWdsStartNetworkInterfaceReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx;

    if (pReq->pIPAddressv6 == NULL)
        return eRCode;

    /* Add TLV data */
    for ( idx = 0; idx < 8; idx++ )
    {
        eRCode = PutWordBe( pBuf, *(pReq->pIPAddressv6 + idx) );
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;
    }

    return eRCode;
}

/*
 * This function packs the StartDataSession AuthPreference
 * field to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvAuthPreference(BYTE *pBuf, BYTE  *pParam)
{
    struct QmiWdsStartNetworkInterfaceReq *pReq =
                       (struct QmiWdsStartNetworkInterfaceReq *)pParam;

    if (pReq->pAuthentication == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutByte(pBuf, *(pReq->pAuthentication));
}

/*
 * This function packs the StartDataSession Username
 * field to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvUsername(BYTE *pBuf, BYTE  *pParam)
{
    struct QmiWdsStartNetworkInterfaceReq *pReq =
                       (struct QmiWdsStartNetworkInterfaceReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (pReq->pUsername == NULL)
        return eQCWWAN_ERR_NONE;

    while( *(pReq->pUsername) != EOS)
    {
        PutByte(pBuf, *(pReq->pUsername)++);
        if( eQCWWAN_ERR_NONE != eRCode )
            break;
    }
    return eRCode;
}

/*
 * This function packs the StartDataSession Password
 * field to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvPassword(BYTE *pBuf, BYTE  *pParam)
{
    struct QmiWdsStartNetworkInterfaceReq *pReq =
                       (struct QmiWdsStartNetworkInterfaceReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (pReq->pPassword == NULL)
        return eQCWWAN_ERR_NONE;

    while( *(pReq->pPassword) != EOS )
    {
        eRCode = PutByte(pBuf, *(pReq->pPassword)++);
        if( eQCWWAN_ERR_NONE != eRCode )
            break;
    }
    return eRCode;
}

/*
 * This function packs the StartDataSession enableAutoconnect
 * field to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvEnableAutoconnect(BYTE *pBuf, BYTE  *pParam)
{
    struct QmiWdsStartNetworkInterfaceReq *pReq =
                       (struct QmiWdsStartNetworkInterfaceReq *)pParam;

    /* Add TLV data */
    return PutByte(pBuf, pReq->enableAutoconnect);
}

/*
 * This function packs the StartDataSession IP Family Preference
 * field to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvIPFamilyPreference(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsStartNetworkInterfaceReq *pReq =
                      (struct QmiWdsStartNetworkInterfaceReq *)pParam;

    if (pReq->pIPFamilyPreference == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutByte(pBuf, *(pReq->pIPFamilyPreference));
}

/*
 * This function packs the SLQSStartStopDataSession Technology Preference field
 * to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvTechnologyPreference(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsStartNwkInterfaceReq *pReq =
                      (struct QmiWdsStartNwkInterfaceReq *)pParam;

    if ( pReq->pTechnology == NULL )
        return eQCWWAN_ERR_NONE;

    if( QMI_WDS_TECH_UMTS == *(pReq->pTechnology) )
    {
        *(pReq->pTechnology) = QMI_WDS_TECH_UMTS_VALUE;
    }

    if( QMI_WDS_TECH_CDMA == *(pReq->pTechnology) )
    {
        *(pReq->pTechnology) = QMI_WDS_TECH_CDMA_VALUE;
    }

    if( QMI_WDS_TECH_eMBMS == *(pReq->pTechnology) )
    {
        *(pReq->pTechnology) = QMI_WDS_TECH_eMBMS_VALUE;
    }

    if( QMI_WDS_TECH_mLINK == *(pReq->pTechnology) )
    {
        *(pReq->pTechnology) = QMI_WDS_TECH_mLINK_VALUE;
    }

    /* Add TLV data */
    return PutWord( pBuf, (WORD)*( pReq->pTechnology ) );
}

/*
 * This function packs the 3GPP Configured Profile Identifier Profile Index
 * field to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlv3gppProfileID(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsStartNwkInterfaceReq *pReq =
                      (struct QmiWdsStartNwkInterfaceReq *)pParam;

    if ( pReq->pProfileId3GPP == NULL )
         return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutByte( pBuf, *( pReq->pProfileId3GPP ) );
}

/*
 * This function packs the 3GPP2 Configured Profile Identifier Profile Index
 * field to the QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlv3gpp2ProfileID(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsStartNwkInterfaceReq *pReq =
                      (struct QmiWdsStartNwkInterfaceReq *)pParam;

    if ( pReq->pProfileId3GPP2 == NULL )
         return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutByte( pBuf, *( pReq->pProfileId3GPP2 ) );
}

/*
 * This function packs the StartDataSession parameters
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
enum eQCWWANError PkQmiWdsStartNetworkInterface(
    WORD   *pMlength,
    BYTE   *pParamField,
    ULONG  *pTechnology,
    ULONG  *pPrimaryDNS,
    ULONG  *pSecondaryDNS,
    ULONG  *pPrimaryNBNS,
    ULONG  *pSecondaryNBNS,
    USHORT *pPrimaryDNSv6,
    USHORT *pSecondaryDNSv6,
    CHAR   *pApnName,
    ULONG  *pIpAddressPreference,
    USHORT *pIPAddressv6,
    ULONG  *pAuthenticationPreference,
    CHAR   *pUserName,
    CHAR   *pPassword,
    ULONG  *pSessionId,
    ULONG  *pFailureReason,
    BYTE   *pIPFamilyPreference,
    BYTE   enableAutoconnect )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TECH_PREFERENCE,             &BuildTlvTechPreference },
        { eTLV_PRIMARY_DNS_PREFERENCE,      &BuildTlvPrimaryDnsPreference },
        { eTLV_SECONDARY_DNS_PREFERENCE,    &BuildTlvSecondaryDnsPreference },
        { eTLV_PRIMARY_NBNS_PREFERENCE,     &BuildTlvPrimaryNbnsPreference },
        { eTLV_SECONDARY_NBNS_PREFERENCE,   &BuildTlvSecondaryNbnsPreference },
        { eTLV_PRIMARY_DNS_PREFERENCE_V6,   &BuildTlvPrimaryDnsPreferencev6 },
        { eTLV_SECONDARY_DNS_PREFERENCE_V6, &BuildTlvSecondaryDnsPreferencev6 },
        { eTLV_APN_NAME,                    &BuildTlvApnName },
        { eTLV_ADDR_PREFERENCE,             &BuildTlvAddrPreference },
        { eTLV_ADDR_PREFERENCE_V6,          &BuildTlvAddrPreferencev6 },
        { eTLV_AUTH_PREFERENCE,             &BuildTlvAuthPreference },
        { eTLV_USERNAME,                    &BuildTlvUsername },
        { eTLV_PASSWORD,                    &BuildTlvPassword },
#if 0
        { eTLV_IP_FAMILY_PREFERENCE,        &BuildTlvIPFamilyPreference },
#endif
        { eTLV_ENABLE_AUTOCONNECT,          &BuildTlvEnableAutoconnect },
        { eTLV_TYPE_INVALID,                NULL } /* Important. Sentinel.
                                                  * Signifies last item in map.
                                                  */
    };

    struct QmiWdsStartNetworkInterfaceReq req;
    enum eQCWWANError eRCode;

    slmemset((char *)&req, 0, sizeof(struct QmiWdsStartNetworkInterfaceReq));
    req.pTechnology         = pTechnology;
    req.pPrimaryDNS         = pPrimaryDNS;
    req.pSecondaryDNS       = pSecondaryDNS;
    req.pPrimaryNBNS        = pPrimaryNBNS;
    req.pSecondaryNBNS      = pSecondaryNBNS;
    req.pPrimaryDNSv6       = pPrimaryDNSv6;
    req.pSecondaryDNSv6     = pSecondaryDNSv6;
    req.pAPNName            = pApnName;
    req.pIPAddress          = pIpAddressPreference;
    req.pIPAddressv6        = pIPAddressv6;
    req.pAuthentication     = pAuthenticationPreference;
    req.pUsername           = pUserName;
    req.pPassword           = pPassword;
    req.pSessionId          = pSessionId;
    req.pFailureReason      = pFailureReason;
    req.pIPFamilyPreference = pIPFamilyPreference;
    req.enableAutoconnect   = enableAutoconnect;

    eRCode = qmbuild( pParamField, (BYTE *)&req, map,
                      eQMI_WDS_START_NET, pMlength);

    return eRCode;
}

/*
 * This function packs the Start/Stop Data Session parameters
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
enum eQCWWANError PkQmiWdsStartNetworkInterface2(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG *pTechnology,
    ULONG *pProfileId3GPP,
    ULONG *pProfileId3GPP2 )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TECH_PREFERENCE,         &BuildTlvTechnologyPreference },
        { eTLV_3GPP_CONFIG_PROFILE_ID,  &BuildTlv3gppProfileID },
        { eTLV_3GPP2_CONFIG_PROFILE_ID, &BuildTlv3gpp2ProfileID },
        { eTLV_TYPE_INVALID,            NULL } /* Important. Sentinel.
                                                * Signifies last item in map.
                                                */
    };

    struct QmiWdsStartNwkInterfaceReq req;
    enum eQCWWANError eRCode;

    slmemset( (char *)&req,
              0,
              sizeof(struct QmiWdsStartNwkInterfaceReq) );

    req.pTechnology     = pTechnology;
    req.pProfileId3GPP  = pProfileId3GPP;
    req.pProfileId3GPP2 = pProfileId3GPP2;

    eRCode = qmbuild( pParamField, (BYTE *)&req, map,
                      eQMI_WDS_START_NET, pMlength);

    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the StartDataSession PacketDataHandle from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]     - Pointer to structure containing storage
 *                               to return data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvPacketDataHandle(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsStartNetworkInterfaceResp *lResp =
                       (struct QmiWdsStartNetworkInterfaceResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pPktDataHandle)
        return eRCode;

    eRCode = GetLong(pTlvData, lResp->pPktDataHandle);

    return eRCode;
}

/*
 * This function unpacks the StartDataSession CallEndReason from the
 * QMI response message to a user provided response structure.
 *
 * \param pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]     - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE
 *
 */
enum eQCWWANError UnpackTlvCallEndReason(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsStartNetworkInterfaceResp *pLresp =
                       (struct QmiWdsStartNetworkInterfaceResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD lWord;

    if (!pLresp->pFailureReason)
        return eRCode;

    eRCode = GetWord(pTlvData, &lWord);
    *pLresp->pFailureReason = lWord;

    return eRCode;
}

/*
 * This function unpacks the StartDataSession VerboseCallEndReason from the
 * QMI response message to a user provided response structure.
 *
 * \param pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]     - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE
 *
 */
enum eQCWWANError UnpackTlvVerboseCallEndReason(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsStartNetworkInterfaceResp *pLresp =
                       (struct QmiWdsStartNetworkInterfaceResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD lWord;

    if (!pLresp->pFailureReason)
        return eRCode;

    eRCode = GetWord(pTlvData, &lWord);
    eRCode = GetWord(pTlvData, &lWord);
    *pLresp->pFailureReason = lWord;

    return eRCode;
}

/*
 * This function unpacks the StartDataSession2 response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success
 *
 */
enum eQCWWANError UpkQmiWdsStartNetworkInterface2(
    BYTE                                   *pMdmResp,
    struct QmiWdsStartNetworkInterfaceResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,             &qmUnpackTlvResultCode },
        { eTLV_PACKET_DATA_HANDLE,      &UnpackTlvPacketDataHandle },
        { eTLV_CALL_END_REASON,         &UnpackTlvCallEndReason },
#if 0
        { eTLV_VERBOSE_CALL_END_REASON, &UnpackTlvVerboseCallEndReason },
#endif
        { eTLV_TYPE_INVALID,            NULL } /* Important. Sentinel.
                                                * Signifies last item in map.
                                                */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WDS_START_NET );

    return eRCode;
}
