/*
 *
 * \ingroup : cbk
 *
 * \file    : qaCbkVoiceIndicationRegister.c
 *
 * \brief   : Contains Packing and UnPacking routines for the
 *            VOICE_INDICATION_REGISTER message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkVoiceIndicationRegister.h"

/* Functions */

/* Request handlers */

/*
 * This function packs the supplementary service notification field to the
 * QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Increment beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvSUPSNotificationEvents(
    BYTE    *pBuf,
    BYTE    *pParam )
{
    struct QmiCbkVoiceIndicationRegisterReq *pReq =
        (struct QmiCbkVoiceIndicationRegisterReq *)pParam;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eDTMFEvents)
        return eQCWWAN_ERR_NONE;

    return PutByte ( pBuf, pReq->eDTMFEvents );
}

/*
 * This function packs the voice privacy events field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Increment beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvPrivacyEvents(
    BYTE    *pBuf,
    BYTE    *pParam )
{
    struct QmiCbkVoiceIndicationRegisterReq *pReq =
        (struct QmiCbkVoiceIndicationRegisterReq *)pParam;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eSUPSNotificationEvents)
        return eQCWWAN_ERR_NONE;

    return PutByte ( pBuf, pReq->eSUPSNotificationEvents );
}

/*
 * This function packs the DTMF events field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Increment beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvDTMFEvents(
    BYTE    *pBuf,
    BYTE    *pParam )
{
    struct QmiCbkVoiceIndicationRegisterReq *pReq =
        (struct QmiCbkVoiceIndicationRegisterReq *)pParam;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eDTMFEvents)
        return eQCWWAN_ERR_NONE;

    return PutByte ( pBuf, pReq->eDTMFEvents );
}

/*
 * This function packs the WDS Set Event Report parameters to the
 * QMI message SDU
 *
 * \param  pMlength[OUT]
 *         - Total length of built message.
 *
 * \param  eDTMFEvents[IN]
 *         - Subscribe to DTMF events
 *
 * \param  eVoicePrivacyEvents[IN]
 *         - Subscribe to Voice privacy events
 *
 * \param  eSUPSNotificationEvents[IN]
 *         - Subscribe to supplementary service notification events
 *
 * \param  pParamField[OUT]
 *         - Pointer to storage into which the packed
 *         data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Increment beyond allowed size attempted
 *
 */
package enum eQCWWANError PkQmiCbkVoiceIndicationRegister(
    WORD                  *pMlength,
    enum eQmiCbkSetStatus eDTMFEvents,
    enum eQmiCbkSetStatus eVoicePrivacyEvents,
    enum eQmiCbkSetStatus eSUPSNotificationEvents,
    BYTE                  *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
         { eTLV_DTMF_EVENTS,              BuildTlvDTMFEvents },
         { eTLV_VOICE_PRIVACY_EVENTS,     BuildTlvPrivacyEvents },
         { eTLV_SUPS_NOTIFICATION_EVENTS, BuildTlvSUPSNotificationEvents },
         { eTLV_TYPE_INVALID,             NULL }  /* Important. Sentinel.
                                                   * Signifies last item in map.
                                                   */
    };

    struct QmiCbkVoiceIndicationRegisterReq req;
    slmemset( (char *)&req,
              0,
              sizeof (struct QmiCbkVoiceIndicationRegisterReq) );

    req.eDTMFEvents             = eDTMFEvents;
    req.eVoicePrivacyEvents     = eVoicePrivacyEvents;
    req.eSUPSNotificationEvents = eSUPSNotificationEvents;

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_VOICE_INDICATION_REGISTER,
                      pMlength );
    return eRCode;
}

/* Response handlers */

/*
 * This function unpacks the VOICE_INDICATION_REGISTER response
 * message to a user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 *
 */
package enum eQCWWANError UpkQmiCbkVoiceIndicationRegister(
    BYTE    *pMdmResp,
    struct  QmiCbkVoiceIndicationRegisterResp *pApiResp)
{
    enum eQCWWANError eRCode;

    /* The unpack of the Callback contains only result code */
    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE       ,&qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID      ,NULL }  /* Important. Sentinel.
                                           * Signifies last item in map.
                                           */
    };
    eRCode =  qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_VOICE_INDICATION_REGISTER);
    return eRCode;
}

