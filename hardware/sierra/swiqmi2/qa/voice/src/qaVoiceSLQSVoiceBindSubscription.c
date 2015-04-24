/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceBindSubscription.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_BIND_SUBSCRIPTION message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceBindSubscription.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Subscription Type parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvBindSubscriptionType( BYTE *pBuf, BYTE *pParam )
{
    voiceBindSubscriptionInfo *pReq = ( voiceBindSubscriptionInfo *)pParam;

    /* Insert Subscription Type param */
    return PutByte( pBuf, pReq->subsType );
}

/*
 * This function packs the SLQS Bind Subscription request parameters to
 * the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSVoiceBindSubscription.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsBindSubscription(
    WORD                      *pMlength,
    BYTE                      *pParamField,
    voiceBindSubscriptionInfo *pVoiceBindSubscriptionInfo )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_BIND_SUBSCRIPTION_TYPE,  &BuildTlvBindSubscriptionType },
        { eTLV_TYPE_INVALID,            NULL } /* Important. Sentinel.
                                                * Signifies last item in map
                                                */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pVoiceBindSubscriptionInfo,
                      map,
                      eQMI_VOICE_BIND_SUBSCRIPTION,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the SLQS Bind Subscription response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsBindSubscription(
    BYTE                                    *pMdmResp,
    struct QmiVoiceSLQSBindSubscriptionResp *pApiResp )
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
                           eQMI_VOICE_BIND_SUBSCRIPTION );
    return eRCode;
}
