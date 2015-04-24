/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceOrigUSSDNoWait.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_ORIG_USSD_NO_WAIT message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceOrigUSSDNoWait.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the USS Info parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvOrigUSSDNWUSSInfo( BYTE *pBuf, BYTE *pParam )
{
    struct USSInfo *pReq =
                  &(( voiceOrigUSSDNoWaitInfo *)pParam)->USSInformation;
    enum eQCWWANError eRCode;
    WORD              lCount;

    /* Insert USS Info param */
    eRCode = PutByte( pBuf, pReq->ussDCS );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->ussLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < pReq->ussLen ; lCount++ )
    {
        eRCode = PutByte( pBuf, pReq->ussData[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function packs the SLQS Originate USSD No Wait request parameters to
 * the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSVoiceOrigUSSDNoWait.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsOrigUSSDNoWait(
    WORD                    *pMlength,
    BYTE                    *pParamField,
    voiceOrigUSSDNoWaitInfo *pVoiceOrigUSSDNoWaitInfo )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_ORIG_USSD_NW_USS_INFO,  &BuildTlvOrigUSSDNWUSSInfo },
        { eTLV_TYPE_INVALID,           NULL } /* Important. Sentinel.
                                               * Signifies last item in map
                                               */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pVoiceOrigUSSDNoWaitInfo,
                      map,
                      eQMI_VOICE_ORIG_USSD_NO_WAIT,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the SLQS Originate USSD No Wait response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsOrigUSSDNoWait(
    BYTE                                  *pMdmResp,
    struct QmiVoiceSLQSOrigUSSDNoWaitResp *pApiResp )
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
                           eQMI_VOICE_ORIG_USSD_NO_WAIT );
    return eRCode;
}
