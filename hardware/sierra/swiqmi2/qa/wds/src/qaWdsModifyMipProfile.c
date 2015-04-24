/*
 * \ingroup wds
 *
 * \file qaWdsModifyMipProfile.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_WDS_MODIFY_MIP_PROFILE message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaWdsModifyMipProfile.h"

#define MAX_SPC_LENGTH 6

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SetMobileIPProfile index field to the QMI message SDU
 *
 * \param  pBuf    - Pointer to storage into which the packed
 *                   data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvMIPIndex(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyMipProfileReq *pReq =
                     (struct QmiWdsModifyMipProfileReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    int i = 0;

    /* Add 6 byte SPC and Index in the message */
    for (i=0; i < MAX_SPC_LENGTH; i++)
    {
        eRCode = PutByte( pBuf, pReq->pSPC[i] );
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;
    }
    return PutByte( pBuf, pReq->index );
}

/*
 * This function packs the SetMobileIPProfile State field to the QMI message SDU
 *
 * \param  pBuf    - Pointer to storage into which the packed
 *                   data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvMIPState(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyMipProfileReq *pReq =
                     (struct QmiWdsModifyMipProfileReq *)pParam;

    /* Set the MIP State */
    if (pReq->pEnabled)
    {
        return PutByte( pBuf, *(pReq->pEnabled) );
    }
    return eQCWWAN_ERR_NONE;
}

/*
 * This function packs the SetMobileIPProfile Home address field to the QMI
 * message SDU
 *
 * \param  pBuf    - Pointer to storage into which the packed
 *                   data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvMIPHomeAddress(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyMipProfileReq *pReq =
                     (struct QmiWdsModifyMipProfileReq *)pParam;

    /* Set the MIP State */
    if (pReq->pAddress)
    {
        return PutLong( pBuf, *(pReq->pAddress) );
    }
    return eQCWWAN_ERR_NONE;
}

/*
 * This function packs the SetMobileIPProfile Primary Home Agent address field
 * to the QMI message SDU
 *
 * \param  pBuf    - Pointer to storage into which the packed
 *                   data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvMIPAgentPrimary(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyMipProfileReq *pReq =
                     (struct QmiWdsModifyMipProfileReq *)pParam;

    /* Set primary HA */
    if (pReq->pPrimaryHA)
    {
        return PutLong( pBuf, *(pReq->pPrimaryHA) );
    }
    return eQCWWAN_ERR_NONE;
}

/*
 * This function packs the SetMobileIPProfile Secondary Home Agent address field
 * to the QMI message SDU
 *
 * \param  pBuf    - Pointer to storage into which the packed
 *                   data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvMIPAgentSecondary(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyMipProfileReq *pReq =
                     (struct QmiWdsModifyMipProfileReq *)pParam;

    /* Set secondary HA  */
    if (pReq->pSecondaryHA)
    {
        return PutLong( pBuf, *(pReq->pSecondaryHA) );
    }
    return eQCWWAN_ERR_NONE;
}

/*
 * This function packs the SetMobileIPProfile tuning preference field
 * to the QMI message SDU
 *
 * \param  pBuf    - Pointer to storage into which the packed
 *                   data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvMIPTunPref(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyMipProfileReq *pReq =
                     (struct QmiWdsModifyMipProfileReq *)pParam;

    /* Set reverse tunnelling */
    if (pReq->pRevTunneling)
    {
        return PutByte( pBuf, *(pReq->pRevTunneling) );
    }
    return eQCWWAN_ERR_NONE;
}

/*
 * This function packs the SetMobileIPProfile tuning preference field
 * to the QMI message SDU
 *
 * \param  pBuf    - Pointer to storage into which the packed
 *                   data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvMIPNAI(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyMipProfileReq *pReq =
                     (struct QmiWdsModifyMipProfileReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    int i = 0;

    /* Set Network Access Identifier */
    if (pReq->pNAI)
    {
        while (pReq->pNAI[i] != EOS)
        {
            eRCode = PutByte( pBuf, pReq->pNAI[i++] );
            if (eRCode != eQCWWAN_ERR_NONE)
                return eRCode;
        }
    }
    return eRCode;
}

/*
 * This function packs the SetMobileIPProfile tuning preference field
 * to the QMI message SDU
 *
 * \param  pBuf    - Pointer to storage into which the packed
 *                   data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvMIPHASpi(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyMipProfileReq *pReq =
                     (struct QmiWdsModifyMipProfileReq *)pParam;

    /* Set HA Security Parameter Index */
    if (pReq->pHASPI)
    {
        return PutLong( pBuf, *(pReq->pHASPI) );
    }
    return eQCWWAN_ERR_NONE;
}

/*
 * This function packs the SetMobileIPProfile tuning preference field
 * to the QMI message SDU
 *
 * \param  pBuf    - Pointer to storage into which the packed
 *                   data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvMIPAAASpi(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyMipProfileReq *pReq =
                     (struct QmiWdsModifyMipProfileReq *)pParam;

    /* Set AAA Security Parameter Index */
    if (pReq->pAAASPI)
    {
        return PutLong( pBuf, *(pReq->pAAASPI) );
    }
    return eQCWWAN_ERR_NONE;
}

/*
 * This function packs the SetMobileIPProfile tuning preference field
 * to the QMI message SDU
 *
 * \param  pBuf    - Pointer to storage into which the packed
 *                   data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvMIPHAKeyState(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyMipProfileReq *pReq =
                     (struct QmiWdsModifyMipProfileReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    int i = 0;

    /* Set MN-HA key */
    if (pReq->pMNHA)
    {
        while (pReq->pMNHA[i] != EOS)
        {
            eRCode = PutByte( pBuf, pReq->pMNHA[i++] );
            if (eRCode != eQCWWAN_ERR_NONE)
                return eRCode;
        }
    }
    return eRCode;
}

/*
 * This function packs the SetMobileIPProfile tuning preference field
 * to the QMI message SDU
 *
 * \param  pBuf    - Pointer to storage into which the packed
 *                   data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvMIPAAAKeyState(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsModifyMipProfileReq *pReq =
                     (struct QmiWdsModifyMipProfileReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    int i = 0;

    /* Set MN-AAA key */
    if (pReq->pMNAAA)
    {
        while (pReq->pMNAAA[i] != EOS)
        {
            eRCode = PutByte( pBuf, pReq->pMNAAA[i++] );
            if (eRCode != eQCWWAN_ERR_NONE)
                return eRCode;
        }
    }
    return eRCode;
}

/*
 * This function packs the SetMobileIPProfile parameters to the QMI message SDU
 *
 * \param  pMlength [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param  pMlength [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 * \sa     qaGobiApiWds.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiWdsModifyMipProfile(
    WORD  *pMlength,
    BYTE  *pBuffer,
    CHAR  *pSPC,
    BYTE  index,
    BYTE  *pEnabled,
    ULONG *pAddress,
    ULONG *pPrimaryHA,
    ULONG *pSecondaryHA,
    BYTE  *pRevTunneling,
    CHAR  *pNAI,
    ULONG *pHASPI,
    ULONG *pAAASPI,
    CHAR  *pMNHA,
    CHAR  *pMNAAA )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_MIP_ID,               &BuildTlvMIPIndex },
        { eTLV_MIP_STATE,            &BuildTlvMIPState },
        { eTLV_MIP_HOME_ADDRESS,     &BuildTlvMIPHomeAddress },
        { eTLV_MIP_HOME_AGENT_PRI,   &BuildTlvMIPAgentPrimary },
        { eTLV_MIP_HOME_AGENT_SEC,   &BuildTlvMIPAgentSecondary },
        { eTLV_MIP_REV_TUN_PREF,     &BuildTlvMIPTunPref },
        { eTLV_MIP_NAI,              &BuildTlvMIPNAI },
        { eTLV_MIP_MN_HA_SPI,        &BuildTlvMIPHASpi },
        { eTLV_MIP_MN_AAA_SPI,       &BuildTlvMIPAAASpi },
        { eTLV_MIP_MN_HA_KEY_STATE,  &BuildTlvMIPHAKeyState },
        { eTLV_MIP_MN_AAA_KEY_STATE, &BuildTlvMIPAAAKeyState },
        { eTLV_TYPE_INVALID,         NULL }  /* Important. Sentinel.
                                              * Signifies last item in map.
                                              */
    };

    struct QmiWdsModifyMipProfileReq req;
    enum eQCWWANError eRCode;

    slmemset((char *)&req, 0, sizeof(struct QmiWdsModifyMipProfileReq));
    req.index         = index;
    req.pSPC          = pSPC;
    req.pEnabled      = pEnabled;
    req.pAddress      = pAddress;
    req.pPrimaryHA    = pPrimaryHA;
    req.pSecondaryHA  = pSecondaryHA;
    req.pRevTunneling = pRevTunneling;
    req.pNAI          = pNAI;
    req.pHASPI        = pHASPI;
    req.pAAASPI       = pAAASPI;
    req.pMNHA         = pMNHA;
    req.pMNAAA        = pMNAAA;

    eRCode = qmbuild( pBuffer,
                      (BYTE *)&req,
                      map,
                      eQMI_WDS_SET_MIP_PROFILE,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

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
enum eQCWWANError UpkQmiWdsModifyMipProfile(
    BYTE   *pMdmResp,
    struct QmiWdsModifyMipProfileResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,     &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID,    NULL } /* Important. Sentinel.
                                        * Signifies last item in map.
                                        */
    };
    eRCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_WDS_SET_MIP_PROFILE);
    return eRCode;
}
