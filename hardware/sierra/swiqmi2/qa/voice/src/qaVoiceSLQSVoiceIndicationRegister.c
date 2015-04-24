/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSIndicationRegister.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_BURST_DTMF message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiVoice.h"
#include "qaVoiceSLQSVoiceIndicationRegister.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the DTMF Event Information fields to the
 * QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvVoiceDTMFEvents( BYTE *pBuf, BYTE *pParam )
{
    voiceIndicationRegisterInfo *pReq = (voiceIndicationRegisterInfo*)pParam;

    /* Optional parameter check against NULL */
    if ( !pReq->pRegDTMFEvents )
    {
         return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *pReq->pRegDTMFEvents);
}

/*
 * This function packs the Voice Privacy Event Information fields to the
 * QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvVoicePrivacyEvents( BYTE *pBuf, BYTE *pParam )
{
    voiceIndicationRegisterInfo *pReq = (voiceIndicationRegisterInfo*)pParam;

    /* Optional parameter check against NULL */
    if ( !pReq->pRegVoicePrivacyEvents )
    {
         return eQCWWAN_ERR_NONE ;
    }

    /* Add TLV data */
    return PutByte( pBuf, *pReq->pRegVoicePrivacyEvents );
}

/*
 * This function packs the SUPS Event Information fields to the
 * QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvVoiceSuppsNotiEvents( BYTE *pBuf, BYTE *pParam )
{
    voiceIndicationRegisterInfo *pReq = (voiceIndicationRegisterInfo*)pParam;

    /* Optional parameter check against NULL */
    if ( !pReq->pSuppsNotifEvents )
    {
         return eQCWWAN_ERR_NONE ;
    }

    /* Add TLV data */
    return PutByte( pBuf, *pReq->pSuppsNotifEvents );
}

/*
 * This function packs the DTMF Length request parameters to the QMI message
 * SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaGobiApiVoice.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSLQSIndicationRegister(
    WORD                        *pMlength,
    BYTE                        *pParamField,
    voiceIndicationRegisterInfo *pVoiceIndicationRegisterInfo )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_VOICE_DTMF_EVENTS,               &BuildTlvVoiceDTMFEvents },
        { eTLV_VOICE_PRIVACY_EVENTS,            &BuildTlvVoicePrivacyEvents },
        { eTLV_VOICE_SUPPS_NOTIFICATION_EVENTS, &BuildTlvVoiceSuppsNotiEvents },
        { eTLV_TYPE_INVALID,                    NULL } /* Important. Sentinel.
                                                        * Signifies last ite
                                                        * in map
                                                        */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pVoiceIndicationRegisterInfo,
                      map,
                      eQMI_VOICE_INDICATION_REGISTER,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the IndicationRegister response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSLQSIndicationRegister(
    BYTE                                      *pMdmResp,
    struct QmiVoiceSLQSIndicationRegisterResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,       &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map.
                                          */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_INDICATION_REGISTER );
    return eRCode;
}
