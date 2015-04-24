/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceSetPreferredPrivacy.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_VOICE_SET_PREFERRED_PRIVACY message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceSetPreferredPrivacy.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Set Voice Privacy Preference to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSetVoicePrivacyPref( BYTE *pBuf, BYTE *pParam )
{
    voiceSetPrefPrivacy *pReq =(voiceSetPrefPrivacy *)pParam;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Add Call Id */
    return PutByte( pBuf, pReq->privacyPref );
}

/*
 * This function packs the SLQS Set Preferred Privacy request parameters to
 * the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSVoiceSetPreferredPrivacy.h for remaining parameter
 *     descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsSetPrefPrivacy(
    WORD                *pMlength,
    BYTE                *pParamField,
    voiceSetPrefPrivacy *pSetPrefPrivacy )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_VOICE_PRIVACY_PREF, &BuildTlvSetVoicePrivacyPref },
        { eTLV_TYPE_INVALID,           NULL } /* Important. Sentinel.
                                               * Signifies last item in map
                                               */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pSetPrefPrivacy,
                      map,
                      eQMI_VOICE_SET_PREFERRED_PRIVACY,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the SLQS Set Preferred Privacy response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsSetPrefPrivacy(
    BYTE                                  *pMdmResp,
    struct QmiVoiceSLQSSetPrefPrivacyResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_SET_PREFERRED_PRIVACY );
    return eRCode;
}

