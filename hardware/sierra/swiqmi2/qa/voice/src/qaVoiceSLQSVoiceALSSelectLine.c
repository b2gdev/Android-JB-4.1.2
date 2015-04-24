/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceALSSelectLine.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_ALS_SELECT_LINE message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceALSSelectLine.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Line Value parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvALSSetLineValue( BYTE *pBuf, BYTE *pParam )
{
    voiceALSSelectLineInfo *pReq = ( voiceALSSelectLineInfo *)pParam;

    /* Insert Switch option param */
    return PutByte( pBuf, pReq->lineValue );
}

/*
 * This function packs the SLQS ALS Set Line Switching request parameters to
 * the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSVoiceALSSelectLine.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsALSSelectLine(
    WORD                   *pMlength,
    BYTE                   *pParamField,
    voiceALSSelectLineInfo *pVoiceALSSelectLineInfo )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_ALS_SET_LINE_VALUE, &BuildTlvALSSetLineValue },
        { eTLV_TYPE_INVALID,       NULL } /* Important. Sentinel.
                                           * Signifies last item in map
                                           */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pVoiceALSSelectLineInfo,
                      map,
                      eQMI_VOICE_ALS_SELECT_LINE,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the SLQS ALS Set Line Switching response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsALSSelectLine(
    BYTE                                 *pMdmResp,
    struct QmiVoiceSLQSALSSelectLineResp *pApiResp )
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
                           eQMI_VOICE_ALS_SELECT_LINE );
    return eRCode;
}
