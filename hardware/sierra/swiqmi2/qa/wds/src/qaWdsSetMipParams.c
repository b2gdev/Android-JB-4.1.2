/*
 * \ingroup wds
 *
 * \file qaWdsSetMipParams.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_WDS_SET_MIP_PROFILE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaWdsSetMipParams.h"

#define  SPCCOUNT 6

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SetMobileIpParameters SPC field to the
 * QMI message SDU
 *
 * \param   pBuf   - Pointer to storage into which the packed
 *                   data will be placed by this function.
 *
 * \param   pParam - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvSPC(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsSetMipParamsReq *pReq =
                     (struct QmiWdsSetMipParamsReq *)pParam;
    CHAR              spcLen;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    spcLen = slstrlen( pReq->pSPC );

    if( SPCCOUNT != spcLen)
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }
    else
    {
        /* Add TLV data */
        while ( spcLen-- )
        {
             eRCode = PutByte(pBuf, *(pReq->pSPC));
             if( eQCWWAN_ERR_NONE != eRCode )
             {
                 return eRCode;
             }
            ( pReq->pSPC )++;
        }
    }
    return eRCode;
}

/*
 * This function packs the SetMobileIpParameters Mode field to the
 * QMI message SDU
 *
 * \param pBuf    - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvMode(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsSetMipParamsReq *pReq =
                     (struct QmiWdsSetMipParamsReq *)pParam;

    if (pReq->pMode == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
      return PutByte(pBuf, *(pReq->pMode));
}

/*
 * This function packs the SetMobileIpParameters RettryLimit field
 * to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvRetryLimit(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsSetMipParamsReq *pReq =
                     (struct QmiWdsSetMipParamsReq *)pParam;

    if (pReq->pRetryLimit == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutByte(pBuf, *(pReq->pRetryLimit));
}

/*
 * This function packs the SetMobileIpParameters RetryInterval field
 * to the QMI message SDU
 *
 * \param pBuf    - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvRetryInterval(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsSetMipParamsReq *pReq =
                     (struct QmiWdsSetMipParamsReq *)pParam;

    if (pReq->pRetryInterval == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutByte(pBuf, *(pReq->pRetryInterval));
}

/*
 * This function packs the SetMobileIpParameters ReRegperiod field
 * to the QMI message SDU
 *
 * \param pBuf    - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvReRegPeriod(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsSetMipParamsReq *pReq =
                     (struct QmiWdsSetMipParamsReq *)pParam;

    if (pReq->pReRegPeriod == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutByte(pBuf, *(pReq->pReRegPeriod));
}

/*
 * This function packs the SetMobileIpParameters ReRegTraffic field
 * to the QMI message SDU
 *
 * \param pBuf    - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvReRegTraffic(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsSetMipParamsReq *pReq =
                     (struct QmiWdsSetMipParamsReq *)pParam;

    if (pReq->pReRegTraffic == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutByte(pBuf, *(pReq->pReRegTraffic));
}

/*
 * This function packs the SetMobileIpParameters HAAuthenticator field
 * to the QMI message SDU
 *
 * \param pBuf    - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvHAAuthenticator(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsSetMipParamsReq *pReq =
                     (struct QmiWdsSetMipParamsReq *)pParam;

    if (pReq->pHAAuthenticator == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutByte(pBuf, *(pReq->pHAAuthenticator));
}

/*
 * This function packs the SetMobileIpParameters HA2002bis field to the
 * QMI message SDU
 *
 * \param pBuf    - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvHA2002bis(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsSetMipParamsReq *pReq =
                     (struct QmiWdsSetMipParamsReq *)pParam;

    if (pReq->pHA2002bis == NULL)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutByte(pBuf, *(pReq->pHA2002bis));
}

/*
 * This function packs the SetMobileIpParameters parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_MEMORY, on error
 *
 * \sa     qaGobiApiWds.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiWdsSetMipParams(
    WORD  *pMlength,
    BYTE  *pParamField,
    CHAR  *pSPC,
    ULONG *pMode,
    BYTE  *pRetryLimit,
    BYTE  *pRetryInterval,
    BYTE  *pReRegPeriod,
    BYTE  *pReRegTraffic,
    BYTE  *pHAAuthenticator,
    BYTE  *pHA2002bis )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_MIP_SPC,                &BuildTlvSPC },
        { eTLV_MIP_MODE,               &BuildTlvMode },
        { eTLV_MIP_REG_RETRY_COUNT,    &BuildTlvRetryLimit },
        { eTLV_MIP_REG_RETRY_INTERVAL, &BuildTlvRetryInterval },
        { eTLV_MIP_RE_REG_PERIOD,      &BuildTlvReRegPeriod },
        { eTLV_MIP_RE_REG_IF_TRAF,     &BuildTlvReRegTraffic },
        { eTLV_MIP_QC_HANDSOFF,        &BuildTlvHAAuthenticator },
        { eTLV_MIP_RFC2002BIS,         &BuildTlvHA2002bis },
        { eTLV_TYPE_INVALID,           NULL }/* Important. Sentinel.
                                              * Signifies last item in map.
                                              */
    };

    struct QmiWdsSetMipParamsReq req;

    enum eQCWWANError eRCode;

    slmemset((char *)&req, 0, sizeof(struct QmiWdsSetMipParamsReq));
    req.pSPC             = pSPC;
    req.pMode            = pMode;
    req.pRetryLimit      = pRetryLimit;
    req.pRetryInterval   = pRetryInterval;
    req.pReRegPeriod     = pReRegPeriod;
    req.pReRegTraffic    = pReRegTraffic;
    req.pHAAuthenticator = pHAAuthenticator;
    req.pHA2002bis       = pHA2002bis;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_WDS_SET_MIP_PARAMS,
                      pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SetMobileIpParameters response message to to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UpkQmiWdsSetMipParams(
    BYTE   *pMdmResp,
    struct QmiWdsSetMipParamsResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID, NULL }/* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };
    eRCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_WDS_SET_MIP_PARAMS );
    return eRCode;
}
