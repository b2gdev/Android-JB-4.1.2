/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSAnswerCall.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_ANSWER_CALL message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceAnswerCall.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the Unique Call Identifier for the call that must
 * be answered to the QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvAnswerCallId( BYTE *pBuf, BYTE *pParam )
{
    voiceAnswerCall *pReq = (voiceAnswerCall *)pParam;

    return PutByte( pBuf, *pReq->pCallId );
}

/*
 * This function packs the SLQS Answer Call request parameters to the QMI
 * message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSAnswerCall.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsAnswerCall(
    WORD            *pMlength,
    BYTE            *pParamField,
    voiceAnswerCall *pVoiceAnswerCall )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_ANSWER_CALL_ID_REQ, &BuildTlvAnswerCallId },
        { eTLV_TYPE_INVALID,       NULL } /* Important. Sentinel.
                                           * Signifies last item in map
                                           */
    };

    //struct QmiVoiceSLQSAnswerCallReq req;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    //slmemset((CHAR *)&req, 0, sizeof(struct QmiVoiceSLQSAnswerCallReq));
    //req.pVoiceAnswerCall = pVoiceAnswerCall;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pVoiceAnswerCall,
                      map,
                      eQMI_VOICE_ANSWER_CALL,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Unique Call Identifier from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvAnswerCallId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct QmiVoiceSLQSAnswerCallResp *lResp =
               (struct QmiVoiceSLQSAnswerCallResp *)pResp;

    return GetByte( pTlvData, lResp->pCallId );
}

/*
 * This function unpacks the SLQS Answer Call response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsAnswerCall(
    BYTE                              *pMdmResp,
    struct QmiVoiceSLQSAnswerCallResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,         &qmUnpackTlvResultCode },
        { eTLV_ANSWER_CALL_ID_RESP, &UnpackTlvAnswerCallId },
        { eTLV_TYPE_INVALID, NULL   } /* Important. Sentinel.
                                       * Signifies last item in map.
                                       */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_ANSWER_CALL );
    return eRCode;
}
