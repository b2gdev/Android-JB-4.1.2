/*
 * \ingroup wds
 *
 * \file qaWdsModifyProfile.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_WDS_MODIFY_PROFILE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaWdsModifyProfile.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the SetDefaultProfile ProfileType field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvProfileTypeM (BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyProfileReq *pReq =
        (struct QmiWdsModifyProfileReq *)pParam;
    enum eQCWWANError eRCode;

    /* Add TLV data */
    eRCode = PutByte(pBuf, pReq->profileType);

    if ( eQCWWAN_ERR_NONE == eRCode )
        /* Add TLV data */
        eRCode = PutByte(pBuf, pReq->profileID);

    return eRCode;
}

/*
 * This function packs the SetDefaultProfile PDPType field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvPDPType (BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyProfileReq *pReq =
        (struct QmiWdsModifyProfileReq *)pParam;

    if (pReq->pPDPType == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutByte(pBuf, *(pReq->pPDPType));
}

/*
 * This function packs the SetDefaultProfile IPAddress field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvIPAddress(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyProfileReq *pReq =
        (struct QmiWdsModifyProfileReq *)pParam;

    if (pReq->pIPAddress == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutLong(pBuf, *(pReq->pIPAddress));
}

/*
 * This function packs the SetDefaultProfile PrimaryDNS field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvPrimaryDNS(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyProfileReq *pReq =
        (struct QmiWdsModifyProfileReq *)pParam;

    if (pReq->pPrimaryDNS == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutLong(pBuf, *(pReq->pPrimaryDNS));
}

/*
 * This function packs the SetDefaultProfile SecondaryDNS field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvSecondaryDNS(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyProfileReq *pReq =
        (struct QmiWdsModifyProfileReq *)pParam;

    if (pReq->pSecondaryDNS == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutLong(pBuf, *(pReq->pSecondaryDNS));
}

/*
 * This function packs the SetDefaultProfile IPAddressV6 field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvIPAddressV6(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyProfileReq *pReq =
        (struct QmiWdsModifyProfileReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx;

    if (pReq->pIPAddressV6 == NULL)
        return eRCode;
    
    /* Add TLV data */
    for ( idx = 0; idx < 8; idx++ )
    {
        eRCode = PutWordBe( pBuf, *(pReq->pIPAddressV6 + idx) );
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;
    }
    
    return eRCode;
}

/*
 * This function packs the SetDefaultProfile PrimaryDNSV6 field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvPrimaryDNSV6(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyProfileReq *pReq =
        (struct QmiWdsModifyProfileReq *)pParam;

    if (pReq->pPrimaryDNSV6 == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutWord(pBuf, *(pReq->pPrimaryDNSV6));
}

/*
 * This function packs the SetDefaultProfile SecondaryDNSV6 field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvSecondaryDNSV6(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyProfileReq *pReq =
        (struct QmiWdsModifyProfileReq *)pParam;

    if (pReq->pSecondaryDNSV6 == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutWord(pBuf, *(pReq->pSecondaryDNSV6));
}
/*
 * This function packs the SetDefaultProfile Authentication field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvAuthentication(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyProfileReq *pReq =
        (struct QmiWdsModifyProfileReq *)pParam;

    if (pReq->pAuthentication == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutByte(pBuf, *(pReq->pAuthentication));
}

/*
 * This function packs the SetDefaultProfile ProfileName field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvName(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyProfileReq *pReq =
        (struct QmiWdsModifyProfileReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (pReq->pName == NULL)
        return eRCode;

    /* Add TLV data */
    while (*(pReq->pName) != EOS )
    {
        eRCode = PutByte(pBuf, *(pReq->pName)++);
        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;    
    }
    return eRCode;
}

/*
 * This function packs the SetDefaultProfile APNName field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvAPNName(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyProfileReq *pReq =
        (struct QmiWdsModifyProfileReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (pReq->pAPNName == NULL)
        return eRCode;

    /* Add TLV data */
    while (*(pReq->pAPNName) != EOS )
    {
        eRCode = PutByte(pBuf, *(pReq->pAPNName)++);
        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;    
    }
    return eRCode;
}

/*
 * This function packs the SetDefaultProfile Username field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvUsernameM(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyProfileReq *pReq =
        (struct QmiWdsModifyProfileReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (pReq->pUsername == NULL)
        return eRCode;

    /* Add TLV data */
    while (*(pReq->pUsername) != EOS )
    {
        eRCode = PutByte(pBuf, *(pReq->pUsername)++);
        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;    
    }
    return eRCode;
}

/*
 * This function packs the SetDefaultProfile Password field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvPasswordM(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyProfileReq *pReq =
        (struct QmiWdsModifyProfileReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (pReq->pPassword == NULL)
        return eRCode;

    /* Add TLV data */
    while (*(pReq->pPassword) != EOS )
    {
        eRCode = PutByte(pBuf, *(pReq->pPassword)++);
        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;    
    }
    return eRCode;
}

/*
 * This function packs the SetDefaultProfile parameters
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
enum eQCWWANError PkQmiWdsModifyProfile(
        WORD      *pMlength,
        BYTE      *pParamField,
        ULONG     profileType,
        BYTE      profileID,
        ULONG     *pPDPType,
        ULONG     *pIPAddress,
        ULONG     *pPrimaryDNS,
        ULONG     *pSecondaryDNS,
        USHORT    *pIPAddressv6,
        USHORT    *PrimaryDNSv6,
        USHORT    *pSecondaryDNSv6,
        ULONG     *pAuthentication,
        CHAR      *pName,
        CHAR      *pAPNName,
        CHAR      *pUsername,
        CHAR      *pPassword )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_PROF_TYPE,       &BuildTlvProfileTypeM },
        { eTLV_NAME,            &BuildTlvName },
        { eTLV_PDP_TYP,         &BuildTlvPDPType },
        { eTLV_APN_NAM,         &BuildTlvAPNName },
        { eTLV_PRIM_DNS,        &BuildTlvPrimaryDNS },
        { eTLV_SECOND_DNS,      &BuildTlvSecondaryDNS },
#if QMI_NO_LTE_FW_SUPPORT
        { eTLV_PRIM_DNS_V6,     &BuildTlvPrimaryDNSV6 },
        { eTLV_SECOND_DNS_V6,   &BuildTlvSecondaryDNSV6 },
        { eTLV_IP_ADD_V6,       &BuildTlvIPAddressV6 },
#endif
        { eTLV_USERNM,          &BuildTlvUsernameM },
        { eTLV_PASSWD,          &BuildTlvPasswordM },
        { eTLV_AUTH_PREF,       &BuildTlvAuthentication },
        { eTLV_IP_ADD,          &BuildTlvIPAddress },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    struct QmiWdsModifyProfileReq req;
    enum eQCWWANError eRCode;

    slmemset((char *)&req, 0, sizeof(struct QmiWdsModifyProfileReq));
    req.profileType     = profileType;
    req.profileID       = profileID;
    req.pPDPType        = pPDPType;
    req.pIPAddress      = pIPAddress;
    req.pPrimaryDNS     = pPrimaryDNS;
    req.pSecondaryDNS   = pSecondaryDNS;
    req.pIPAddressV6    = pIPAddressv6;
    req.pPrimaryDNSV6   = PrimaryDNSv6;
    req.pSecondaryDNSV6 = pSecondaryDNSv6;
    req.pAuthentication = pAuthentication;
    req.pName           = pName;
    req.pAPNName        = pAPNName;
    req.pUsername       = pUsername;
    req.pPassword       = pPassword;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_WDS_MODIFY_PROFILE,
                      pMlength);
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the SetDefaultProfile response message to a
 * user-provided response structure.
 *
 * \param	pMdmResp - Pointer to packed response from the modem.
 *
 * \param	pApiResp - Pointer to storage to unpack into.
 *
 * \return	eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UpkQmiWdsModifyProfile(
    BYTE *pMdmResp,
    struct QmiWdsModifyProfileResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,     &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID,    NULL }  /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };
    eRCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_WDS_MODIFY_PROFILE );
    return eRCode;
}

