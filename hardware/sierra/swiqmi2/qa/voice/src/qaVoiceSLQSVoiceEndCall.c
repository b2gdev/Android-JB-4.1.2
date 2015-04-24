/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceEndCall.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_END_CALL message.
 *
 * Copyright: © 2011-12 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceEndCall.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the Unique Call Identifier Information fields to the
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
enum eQCWWANError BuildTlvCallId( BYTE *pBuf, BYTE *pParam )
{
    struct QmiVoiceSLQSEndCallReq *pReq =
        (struct QmiVoiceSLQSEndCallReq *)pParam;

    return PutByte( pBuf, *pReq->pCallId );
}

/*
 * This function packs the SLQSEndCall request parameters to the QMI message SDU
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
enum eQCWWANError PkQmiVoiceSlqsEndCall(
    WORD *pMlength,
    BYTE *pParamField,
    BYTE *pCallId )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_CALL_ID_REQ,  &BuildTlvCallId },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map
                                     */
    };

    struct QmiVoiceSLQSEndCallReq req;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiVoiceSLQSEndCallReq));
    req.pCallId = pCallId;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_VOICE_END_CALL,
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
enum eQCWWANError UnpackTlvCallId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct QmiVoiceSLQSEndCallResp *lResp =
               (struct QmiVoiceSLQSEndCallResp *)pResp;

    return GetByte( pTlvData, lResp->pCallId );
}

/*
 * This function unpacks the SLQSEndCall response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsEndCall(
    BYTE                           *pMdmResp,
    struct QmiVoiceSLQSEndCallResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_CALL_ID_RESP, &UnpackTlvCallId },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_END_CALL );
    return eRCode;
}
