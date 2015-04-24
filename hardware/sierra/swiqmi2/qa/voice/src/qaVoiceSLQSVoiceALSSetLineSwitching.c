/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceALSSetLineSwitching.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_ALS_SET_LINE_SWITCHING message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceALSSetLineSwitching.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Switch option parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvALSSetLineSwitchOption( BYTE *pBuf, BYTE *pParam )
{
    voiceALSSetLineSwitchInfo *pReq = ( voiceALSSetLineSwitchInfo *)pParam;

    /* Insert Switch option param */
    return PutByte( pBuf, pReq->switchOption );
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
 * \sa qaVoiceSLQSVoiceALSSetLineSwitching.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsALSSetLineSwitching(
    WORD                      *pMlength,
    BYTE                      *pParamField,
    voiceALSSetLineSwitchInfo *pVoiceALSSetLineSwitchInfo )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_ALS_SET_LINE_SWITCH_OPTION, &BuildTlvALSSetLineSwitchOption },
        { eTLV_TYPE_INVALID,               NULL } /* Important. Sentinel.
                                                   * Signifies last item in map
                                                   */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pVoiceALSSetLineSwitchInfo,
                      map,
                      eQMI_VOICE_ALS_SET_LINE_SWITCHING,
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
enum eQCWWANError UpkQmiVoiceSlqsALSSetLineSwitching(
    BYTE                                       *pMdmResp,
    struct QmiVoiceSLQSALSSetLineSwitchingResp *pApiResp )
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
                           eQMI_VOICE_ALS_SET_LINE_SWITCHING );
    return eRCode;
}
