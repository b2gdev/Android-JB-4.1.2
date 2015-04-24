/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceSetCallBarringPassword.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_VOICE_SET_CALL_BARRING_PASSWORD message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceSetCallBarringPassword.h"
#include "qaGobiApiVoiceCommon.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Call Barring password Information to the
 * QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvCBPWDInfo( BYTE *pBuf, BYTE *pParam )
{
    voiceSetCallBarringPwdInfo *pReq = (voiceSetCallBarringPwdInfo *)pParam;
    enum eQCWWANError eRCode;
    BYTE              lcount;

    /* Insert Call Barring Password Info */
    eRCode = PutByte( pBuf, pReq->Reason );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for( lcount = 0 ; lcount < PASSWORD_LENGTH ; lcount++ )
    {
        eRCode = PutByte( pBuf, pReq->oldPasswd[lcount]);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    for( lcount = 0 ; lcount < PASSWORD_LENGTH ; lcount++ )
    {
        eRCode = PutByte( pBuf, pReq->newPasswd[lcount]);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    for( lcount = 0 ; lcount < PASSWORD_LENGTH ; lcount++ )
    {
        eRCode = PutByte( pBuf, pReq->newPasswdAgain[lcount]);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function packs the SLQS Set Call Barring Password request parameters
 * to the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSVoiceSetCallBarringPassword.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsSetCallBarringPwd(
    WORD                       *pMlength,
    BYTE                       *pParamField,
    voiceSetCallBarringPwdInfo *pVoiceSetCallFWReq  )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_CBPWD_INFO,   &BuildTlvCBPWDInfo },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map
                                     */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pVoiceSetCallFWReq,
                      map,
                      eQMI_VOICE_SET_CALL_BARRING_PASSWORD,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Call Barring Password Failure Cause from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCBPWDFailureCause(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceSetCallBarringPwdResp *lResp =
    ((struct QmiVoiceSLQSSetCallBarringPwdResp *)pResp)->pSetCallBarringPwdResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pFailCause )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetWord( pTlvData, lResp->pFailCause );
}

/*
 * This function unpacks the Call Barring Password Alpha Identifier Info from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCBPWDAlphaId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    alphaIDInfo *lResp = ((struct QmiVoiceSLQSSetCallBarringPwdResp *)
                                  pResp)->pSetCallBarringPwdResp->pAlphaIDInfo;

    return UnpackTlvCommonAlphaIdentifier( pTlvData, lResp );
}

/*
 * This function unpacks the Call Barring Password Result Type from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCBPWDResultType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceSetCallBarringPwdResp *lResp =
    ((struct QmiVoiceSLQSSetCallBarringPwdResp *)pResp)->pSetCallBarringPwdResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCCResType )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCCResType );
}

/*
 * This function unpacks the Call Barring Password Call Identifier from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCBPWDCallId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceSetCallBarringPwdResp *lResp =
    ((struct QmiVoiceSLQSSetCallBarringPwdResp *)pResp)->pSetCallBarringPwdResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCallID )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCallID );
}

/*
 * This function unpacks the Call Barring Password Call Control Supplementary Service
 * Type from the QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCBPWDccSupsType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    ccSUPSType *lResp = ((struct QmiVoiceSLQSSetCallBarringPwdResp *)
                                pResp)->pSetCallBarringPwdResp->pCCSUPSType;

    return UnpackTlvCommonccSupsType( pTlvData, lResp );
}

/*
 * This function unpacks the SLQS Get Call Barring Password response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsSetCallBarringPwd(
    BYTE                                     *pMdmResp,
    struct QmiVoiceSLQSSetCallBarringPwdResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,         &qmUnpackTlvResultCode },
        { eTLV_CBPWD_FAILURE_CAUSE, &UnpackTlvCBPWDFailureCause },
        { eTLV_CBPWD_ALPHA_ID,      &UnpackTlvCBPWDAlphaId },
        { eTLV_CBPWD_RESULT_TYPE,   &UnpackTlvCBPWDResultType },
        { eTLV_CBPWD_CALL_ID,       &UnpackTlvCBPWDCallId },
        { eTLV_CBPWD_CC_SUP_S_TYPE, &UnpackTlvCBPWDccSupsType },
        { eTLV_TYPE_INVALID,        NULL } /* Important. Sentinel.
                                            * Signifies last item in map.
                                            */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_SET_CALL_BARRING_PASSWORD );
    return eRCode;
}
