/*
 * \ingroup cbk
 *
 * \file    qaCbkVoiceSLQSVoicePrivacyInd.c
 *
 * \brief   Contains UnPacking routines for the QMI_VOICE_PRIVACY_IND
 *          message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkVoiceSLQSVoicePrivacyInd.h"

/* Functions */
/*
 * This function unpacks the Voice Privacy information TLV from the
 * QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvVoicePrivacyInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voicePrivacyInfo *lResponse = (voicePrivacyInfo *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the Call ID */
    eRCode = GetByte( pTlvData, &(lResponse->callID) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the Voice Privacy */
    return GetByte( pTlvData, &(lResponse->voicePrivacy) );
}

/*
 * This function unpacks the Privacy Indication message to a
 * user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkVoicePrivacyInd(
    BYTE             *pMdmResp,
    voicePrivacyInfo *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_VOICE_PRIVACY_INFO, &UnpackCbkTlvVoicePrivacyInfo},
        { eTLV_TYPE_INVALID,       NULL } /* Important. Sentinel.
                                           * Signifies last item in map.
                                           */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_PRIVACY_IND );
    return eRCode;
}
