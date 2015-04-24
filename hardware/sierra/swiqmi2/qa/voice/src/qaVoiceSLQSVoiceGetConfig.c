/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetConfig.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_GET_CONFIG message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceGetConfig.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Auto Answer parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvGetAutoAnswer( BYTE *pBuf, BYTE *pParam )
{
    voiceGetConfigReq *pReq = ( voiceGetConfigReq *)pParam;

    if ( NULL == pReq->pAutoAnswer )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Auto Answer param */
    return PutByte( pBuf, *pReq->pAutoAnswer );
}

/*
 * This function packs the Air Timer parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvGetAirTimer( BYTE *pBuf, BYTE *pParam )
{
    voiceGetConfigReq *pReq = ( voiceGetConfigReq *)pParam;

    if ( NULL == pReq->pAirTimer )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Air Timer param */
    return PutByte( pBuf, *pReq->pAirTimer );
}

/*
 * This function packs the Roam Timer parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvGetRoamTimer( BYTE *pBuf, BYTE *pParam )
{
    voiceGetConfigReq *pReq = ( voiceGetConfigReq *)pParam;

    if ( NULL == pReq->pRoamTimer )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Roam Timer param */
    return PutByte( pBuf, *pReq->pRoamTimer );
}

/*
 * This function packs the TTY mode parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvGetTTYMode( BYTE *pBuf, BYTE *pParam )
{
    voiceGetConfigReq *pReq = ( voiceGetConfigReq *)pParam;

    if ( NULL == pReq->pTTYMode)
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert TTY mode param */
    return PutByte( pBuf, *pReq->pTTYMode );
}

/*
 * This function packs the Preferred Voice Service Option parameter
 * to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvGetPrefVSo( BYTE *pBuf, BYTE *pParam )
{
    voiceGetConfigReq *pReq = ( voiceGetConfigReq *)pParam;

    if ( NULL == pReq->pPrefVoiceSO )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Preferred Voice Service Option mode param */
    return PutByte( pBuf, *pReq->pPrefVoiceSO );
}

/*
 * This function packs the AMR status parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvGetAMRStatus( BYTE *pBuf, BYTE *pParam )
{
    voiceGetConfigReq *pReq = ( voiceGetConfigReq *)pParam;

    if ( NULL == pReq->pAMRStatus )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert AMR status param */
    return PutByte( pBuf, *pReq->pAMRStatus );
}

/*
 * This function packs the Preferred Voice Privacy Option parameter
 * to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvGetVoicePrivacyPref( BYTE *pBuf, BYTE *pParam )
{
    voiceGetConfigReq *pReq = ( voiceGetConfigReq *)pParam;

    if ( NULL == pReq->pPrefVoicePrivacy )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Preferred Voice Privacy Option param */
    return PutByte( pBuf, *pReq->pPrefVoicePrivacy );
}

/*
 * This function packs the Preferred Voice Service Option parameter
 * to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvGetNameId( BYTE *pBuf, BYTE *pParam )
{
    voiceGetConfigReq *pReq = ( voiceGetConfigReq *)pParam;

    if ( NULL == pReq->pNamID )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Name Id param */
    return PutByte( pBuf, *pReq->pNamID );
}

/*
 * This function packs the Preferred Voice Domain parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvGetPrefVDomain( BYTE *pBuf, BYTE *pParam )
{
    voiceGetConfigReq *pReq = ( voiceGetConfigReq *)pParam;

    if ( NULL == pReq->pVoiceDomainPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Voice Domain preference param */
    return PutByte( pBuf, *pReq->pVoiceDomainPref );
}

/*
 * This function packs the SLQS Get Configuration request parameters to the QMI
 * message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSVoiceGetConfig.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsGetConfig(
    WORD              *pMlength,
    BYTE              *pParamField,
    voiceGetConfigReq *pVoiceGetConfigReq )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_GET_AUTO_ANSWER,        &BuildTlvGetAutoAnswer },
        { eTLV_GET_AIR_TIMER,          &BuildTlvGetAirTimer },
        { eTLV_GET_ROAM_TIMER,         &BuildTlvGetRoamTimer },
        { eTLV_GET_TTY_MODE,           &BuildTlvGetTTYMode },
        { eTLV_GET_PREF_VSO,           &BuildTlvGetPrefVSo },
        { eTLV_GET_AMR_STATUS,         &BuildTlvGetAMRStatus },
        { eTLV_GET_VOICE_PRIVACY_PREF, &BuildTlvGetVoicePrivacyPref },
        { eTLV_GET_NAM_ID,             &BuildTlvGetNameId },
        { eTLV_GET_PREF_VDOMAIN,       &BuildTlvGetPrefVDomain },
        { eTLV_TYPE_INVALID,           NULL } /* Important. Sentinel.
                                               * Signifies last item in map
                                               */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pVoiceGetConfigReq,
                      map,
                      eQMI_VOICE_GET_CONFIG,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Auto Answer values from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvGetAutoAnswer(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetConfigResp *lResp =
            ((struct QmiVoiceSLQSGetConfigResp *)pResp)->pVoiceGetConfigResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pAutoAnswerStat )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pAutoAnswerStat );
}

/*
 * This function unpacks the Air Timer values from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvGetAirTimer(
    BYTE *pTlvData,
    BYTE *pResp )
{
    airTimer *lResp = ((struct QmiVoiceSLQSGetConfigResp *)
                                pResp)->pVoiceGetConfigResp->pAirTimerCnt;
    enum eQCWWANError eRCode;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }
    eRCode = GetByte( pTlvData, &lResp->namID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetLong( pTlvData, &lResp->airTimerValue);
}

/*
 * This function unpacks the Roam Timer Values from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvGetRoamTimer(
    BYTE *pTlvData,
    BYTE *pResp )
{
    roamTimer *lResp = ((struct QmiVoiceSLQSGetConfigResp *)
                                pResp)->pVoiceGetConfigResp->pRoamTimerCnt;

    enum eQCWWANError eRCode;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }
    eRCode = GetByte( pTlvData, &lResp->namID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetLong( pTlvData, &lResp->roamTimerValue );
}

/*
 * This function unpacks the TTY Mode Values from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvGetTTYMode(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetConfigResp *lResp =
            ((struct QmiVoiceSLQSGetConfigResp *)pResp)->pVoiceGetConfigResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCurrTTYMode )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCurrTTYMode );
}

/*
 * This function unpacks the Preferred Voice Service Option Values from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvGetCurPrefVSO(
    BYTE *pTlvData,
    BYTE *pResp )
{
    prefVoiceSO *lResp = ((struct QmiVoiceSLQSGetConfigResp *)
                            pResp)->pVoiceGetConfigResp->pCurPrefVoiceSO;
    enum eQCWWANError eRCode;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Voice Service Option values */
    eRCode = GetByte( pTlvData, &lResp->namID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->evrcCapability );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->homePageVoiceSO );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->homeOrigVoiceSO );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetWord( pTlvData, &lResp->roamOrigVoiceSO );
}

/*
 * This function unpacks the AMR Configuration Values from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvGetAMRConfig(
    BYTE *pTlvData,
    BYTE *pResp )
{
    curAMRConfig *lResp = ((struct QmiVoiceSLQSGetConfigResp *)
                                pResp)->pVoiceGetConfigResp->pCurAMRConfig;
    enum eQCWWANError eRCode;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    eRCode = GetByte( pTlvData, &lResp->gsmAmrStat );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &lResp->wcdmaAmrStat );
}

/*
 * This function unpacks the Preferred Voice Privacy Preference from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvGetPrivacyPreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetConfigResp *lResp =
            ((struct QmiVoiceSLQSGetConfigResp *)pResp)->pVoiceGetConfigResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCurVoicePrivacyPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCurVoicePrivacyPref );
}

/*
 * This function unpacks the Preferred Voice Domain Values from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvGetPrefVDomain(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetConfigResp *lResp =
            ((struct QmiVoiceSLQSGetConfigResp *)pResp)->pVoiceGetConfigResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCurVoiceDomainPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCurVoiceDomainPref );
}

/*
 * This function unpacks the SLQS Get Configuration response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsGetConfig(
    BYTE                             *pMdmResp,
    struct QmiVoiceSLQSGetConfigResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,                &qmUnpackTlvResultCode },
        { eTLV_GET_AUTO_ANSWER_RESP,       &UnpackTlvGetAutoAnswer },
        { eTLV_GET_CUR_AIR_TIMER,          &UnpackTlvGetAirTimer},
        { eTLV_GET_CUR_ROAM_TIMER,         &UnpackTlvGetRoamTimer },
        { eTLV_GET_CUR_TTY_MODE,           &UnpackTlvGetTTYMode },
        { eTLV_GET_CUR_PREF_VSO,           &UnpackTlvGetCurPrefVSO},
        { eTLV_GET_CUR_AMR_CONFIG,         &UnpackTlvGetAMRConfig },
        { eTLV_GET_CUR_VOICE_PRIVACY_PREF, &UnpackTlvGetPrivacyPreference },
        { eTLV_GET_CUR_VOICE_DOMAIN_PREF,  &UnpackTlvGetPrefVDomain },
        { eTLV_TYPE_INVALID,               NULL } /* Important. Sentinel.
                                                   * Signifies last item in map.
                                                   */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_GET_CONFIG );
    return eRCode;
}
