/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceStopContDTMF.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_STOP_CONT_DTMF message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceStopContDTMF.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the continuous DTMF CallID fields to the
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
enum eQCWWANError BuildTlvStopContDTMFInfo( BYTE *pBuf, BYTE *pParam )
{
    voiceStopContDTMFinfo *pReq = (voiceStopContDTMFinfo *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->callID );
}

/*
 * This function packs the Continuous DTMF request parameters to the QMI message
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
enum eQCWWANError PkQmiVoiceSlqsStopContDTMF(
    WORD                  *pMlength,
    BYTE                  *pParamField,
    voiceStopContDTMFinfo *pVoiceStopContDTMFinfo )
{
    static struct qmTlvBuilderItem map[] =
    {
        {eTLV_STOP_CONT_DTMF_REQ, &BuildTlvStopContDTMFInfo },
        {eTLV_TYPE_INVALID,       NULL } /* Important. Sentinel.
                                          * Signifies last item in map
                                          */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pVoiceStopContDTMFinfo,
                      map,
                      eQMI_VOICE_STOP_CONT_DTMF,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the  Continuous Call Identifier from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvStopContDTMFCallId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct QmiVoiceSLQSStopContDTMFResp *lResp =
        (struct QmiVoiceSLQSStopContDTMFResp *)pResp;

    /* Mandatory parameter check against NULL */
    if ( !lResp )
    {
         return eQCWWAN_ERR_NONE;
    }
    /* Add TLV data */
    return GetByte( pTlvData, &lResp->callId );
}

/*
 * This function unpacks the SLQSStopContDTMF response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSLQSStopContDTMF(
    BYTE                                 *pMdmResp,
    struct QmiVoiceSLQSStopContDTMFResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,                 &qmUnpackTlvResultCode },
        { eTLV_STOP_CONT_DTMF_CALL_ID_RESP, &UnpackTlvStopContDTMFCallId },
        { eTLV_TYPE_INVALID,                 NULL } /* Important. Sentinel.
                                                     * Signifies last item
                                                     * in map.
                                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_STOP_CONT_DTMF );
    return eRCode;
}
