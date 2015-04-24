/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceSetConfig.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_SET_CONFIG message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceSetConfig.h"

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
enum eQCWWANError BuildTlvSetAutoAnswer( BYTE *pBuf, BYTE *pParam )
{
    voiceSetConfigReq *pReq = ( voiceSetConfigReq *)pParam;

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
enum eQCWWANError BuildTlvSetAirTimer( BYTE *pBuf, BYTE *pParam )
{
    airTimer *pReq = (( voiceSetConfigReq *)pParam)->pAirTimerConfig;
    enum eQCWWANError eRCode;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Air Timer values */
    eRCode = PutByte( pBuf, pReq->namID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return PutLong( pBuf, pReq->airTimerValue );
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
enum eQCWWANError BuildTlvSetRoamTimer( BYTE *pBuf, BYTE *pParam )
{
    roamTimer *pReq = (( voiceSetConfigReq *)pParam)->pRoamTimerConfig;
    enum eQCWWANError eRCode;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Roam Timer values */
    eRCode = PutByte( pBuf, pReq->namID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return PutLong( pBuf, pReq->roamTimerValue );
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
enum eQCWWANError BuildTlvSetTTYMode( BYTE *pBuf, BYTE *pParam )
{
    voiceSetConfigReq *pReq = ( voiceSetConfigReq *)pParam;

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
enum eQCWWANError BuildTlvSetPrefVSo( BYTE *pBuf, BYTE *pParam )
{
    prefVoiceSO *pReq = (( voiceSetConfigReq *)pParam)->pPrefVoiceSO;
    enum eQCWWANError eRCode;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Voice Service Option values */
    eRCode = PutByte( pBuf, pReq->namID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->evrcCapability );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutWord( pBuf, pReq->homePageVoiceSO );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutWord( pBuf, pReq->homeOrigVoiceSO );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return PutWord( pBuf, pReq->roamOrigVoiceSO );
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
enum eQCWWANError BuildTlvSetPrefVDomain( BYTE *pBuf, BYTE *pParam )
{
    voiceSetConfigReq *pReq = ( voiceSetConfigReq *)pParam;

    if ( NULL == pReq->pPrefVoiceDomain )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Voice Domain param */
    return PutByte( pBuf, *pReq->pPrefVoiceDomain );
}

/*
 * This function packs the SLQS Set Configuration request parameters to the QMI
 * message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSVoiceSetConfig.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsSetConfig(
    WORD              *pMlength,
    BYTE              *pParamField,
    voiceSetConfigReq *pVoiceSetConfigReq )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_AUTO_ANSWER,  &BuildTlvSetAutoAnswer },
        { eTLV_SET_AIR_TIMER,    &BuildTlvSetAirTimer },
        { eTLV_SET_ROAM_TIMER,   &BuildTlvSetRoamTimer },
        { eTLV_SET_TTY_MODE,     &BuildTlvSetTTYMode },
        { eTLV_SET_PREF_VSO,     &BuildTlvSetPrefVSo },
        { eTLV_SET_PREF_VDOMAIN, &BuildTlvSetPrefVDomain },
        { eTLV_TYPE_INVALID,    NULL } /* Important. Sentinel.
                                        * Signifies last item in map
                                        */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pVoiceSetConfigReq,
                      map,
                      eQMI_VOICE_SET_CONFIG,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Auto Answer OutCome from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvAutoAnswerOC(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceSetConfigResp *lResp =
            ((struct QmiVoiceSLQSSetConfigResp *)pResp)->pVoiceSetConfigResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pAutoAnsStatus )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pAutoAnsStatus );
}

/*
 * This function unpacks the Air Timer OutCome from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvAirTimerOC(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceSetConfigResp *lResp =
            ((struct QmiVoiceSLQSSetConfigResp *)pResp)->pVoiceSetConfigResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pAirTimerStatus )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pAirTimerStatus );
}

/*
 * This function unpacks the Roam Timer OutCome from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRoamTimerOC(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceSetConfigResp *lResp =
            ((struct QmiVoiceSLQSSetConfigResp *)pResp)->pVoiceSetConfigResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pRoamTimerStatus )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pRoamTimerStatus );
}

/*
 * This function unpacks the TTY Mode OutCome from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvTTYModeOC(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceSetConfigResp *lResp =
            ((struct QmiVoiceSLQSSetConfigResp *)pResp)->pVoiceSetConfigResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pTTYConfigStatus )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pTTYConfigStatus );
}

/*
 * This function unpacks the Preferred Voice Service Option OutCome from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvPrefVSOOC(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceSetConfigResp *lResp =
            ((struct QmiVoiceSLQSSetConfigResp *)pResp)->pVoiceSetConfigResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pPrefVoiceSOStatus )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pPrefVoiceSOStatus );
}
/*
 * This function unpacks the Preferred Voice Domain OutCome from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvPrefVDomainOC(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceSetConfigResp *lResp =
            ((struct QmiVoiceSLQSSetConfigResp *)pResp)->pVoiceSetConfigResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pVoiceDomainPrefStatus )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pVoiceDomainPrefStatus );
}

/*
 * This function unpacks the SLQS Set Configuration response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsSetConfig(
    BYTE                             *pMdmResp,
    struct QmiVoiceSLQSSetConfigResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,     &qmUnpackTlvResultCode },
        { eTLV_AUTO_ANSWER_OC,  &UnpackTlvAutoAnswerOC },
        { eTLV_AIR_TIMER_OC,    &UnpackTlvAirTimerOC},
        { eTLV_ROAM_TIMER_OC,   &UnpackTlvRoamTimerOC },
        { eTLV_TTY_MODE_OC,     &UnpackTlvTTYModeOC },
        { eTLV_PREF_VSO_OC,     &UnpackTlvPrefVSOOC },
        { eTLV_PREF_VDOMAIN_OC, &UnpackTlvPrefVDomainOC },
        { eTLV_TYPE_INVALID,    NULL } /* Important. Sentinel.
                                        * Signifies last item in map.
                                        */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_SET_CONFIG );
    return eRCode;
}
