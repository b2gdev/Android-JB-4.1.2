/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceSetSUPSService.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_SET_SUPS_SERVICE message.
 *
 * Copyright: © 2011-12 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiVoice.h"
#include "qaVoiceSLQSVoiceSetSUPSService.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the call forwarding type and plan to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvCallForwardingTypePlan( BYTE *pBuf, BYTE *pParam )
{
    callFwdTypeAndPlan *pReq = ((struct QmiVoiceSLQSSetSUPSServiceReq *)
            pParam)->pVoiceSetSUPSServiceReq->pCallFwdTypeAndPlan;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /*Call forwarding typ and plan are optional parameters */
    if( NULL == pReq )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert call forwarding number type and plan parameters */
    eRCode = PutByte( pBuf, pReq->numberType );
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        eRCode = PutByte( pBuf, pReq->numberPlan );
    }
    return eRCode;
}

/*
 * This function packs the call forwarding number to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvCallForwardingNumber( BYTE *pBuf, BYTE *pParam )
{
    voiceSetSUPSServiceReq *pReq =
    ((struct QmiVoiceSLQSSetSUPSServiceReq *)pParam)->pVoiceSetSUPSServiceReq;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lCount = 0;

    /* Call forwarding number is an optional parameter */
    if( NULL == pReq->pCallForwardingNumber )
    {
        return eQCWWAN_ERR_NONE;
    }

    while ( pReq->pCallForwardingNumber[lCount]!= '\0' )
    {
        /* Check index of call forwarding number */
        if ( lCount > QMI_VOICE_CALL_FORWARDING_LEN )
        {
            break;
        }
        eRCode = PutByte( pBuf, pReq->pCallForwardingNumber[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
        lCount++;
    }
    return eRCode;
}

/*
 * This function packs the call barring password to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvCallBarringPwd( BYTE *pBuf, BYTE *pParam )
{
    voiceSetSUPSServiceReq *pReq =
    ((struct QmiVoiceSLQSSetSUPSServiceReq *)pParam)->pVoiceSetSUPSServiceReq;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lCount = 0;

    /* Call barring password is an optional parameter */
    if( NULL == pReq->pCallBarringPasswd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert call barring password */
    for (lCount=0; lCount < QMI_VOICE_CALL_BARRING_PWD_LEN; lCount++)
    {
        eRCode = PutByte( pBuf, pReq->pCallBarringPasswd[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
                return eRCode;
        }
    }
    return eRCode;
}

/*
 * This function packs the call forwarding no reply timer value to the QMI
 * message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvCallForwardingNoReply( BYTE *pBuf, BYTE *pParam )
{
    voiceSetSUPSServiceReq *pReq =
    ((struct QmiVoiceSLQSSetSUPSServiceReq *)pParam)->pVoiceSetSUPSServiceReq;

    /* Service Class is an optional parameter */
    if( NULL == pReq->pTimerVal )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert service class value */
    return PutByte( pBuf, *(pReq->pTimerVal) );
}

/*
 * This function packs the service class value to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvServiceClass( BYTE *pBuf, BYTE *pParam )
{
    voiceSetSUPSServiceReq *pReq =
    ((struct QmiVoiceSLQSSetSUPSServiceReq *)pParam)->pVoiceSetSUPSServiceReq;

    /* Service Class is an optional parameter */
    if( NULL == pReq->pServiceClass )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert service class value */
    return PutByte( pBuf, *(pReq->pServiceClass) );
}

/*
 * This function packs the supplementary service values to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSupplymentaryService( BYTE *pBuf, BYTE *pParam )
{
    voiceSetSUPSServiceReq *pReq =
    ((struct QmiVoiceSLQSSetSUPSServiceReq *)pParam)->pVoiceSetSUPSServiceReq;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Voice Service is a mandatory parameter, hence only validating pointer */
    if( NULL == pReq )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Voice Service and reason parameters */
    eRCode = PutByte( pBuf, pReq->voiceSvc );
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        eRCode = PutByte( pBuf, pReq->reason );
    }
    return eRCode;
}

/*
 * This function packs the SLQS Dial Call request parameters to the QMI
 * message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSDialCall.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsSetSUPSService(
    WORD                   *pMlength,
    BYTE                   *pParamField,
    voiceSetSUPSServiceReq *pVoiceSetSUPSServiceReq )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SUPS_SERVICE_INFO,            &BuildTlvSupplymentaryService },
        { eTLV_SERVICE_CLASS,                &BuildTlvServiceClass },
        { eTLV_CALL_BARRING_PWD,             &BuildTlvCallBarringPwd },
        { eTLV_CALL_FORWARDING_NUMBER,       &BuildTlvCallForwardingNumber },
        { eTLV_CALL_FORWARDING_NO_REPLY,     &BuildTlvCallForwardingNoReply },
        { eTLV_CALL_FORWARDING_NU_TYPE_PLAN, &BuildTlvCallForwardingTypePlan },
        { eTLV_TYPE_INVALID,    NULL } /* Important. Sentinel.
                                        * Signifies last item in map
                                        */
    };

    struct QmiVoiceSLQSSetSUPSServiceReq req;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiVoiceSLQSSetSUPSServiceReq));
    req.pVoiceSetSUPSServiceReq = pVoiceSetSUPSServiceReq;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_VOICE_SET_SUPS_SERVICE,
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
enum eQCWWANError UnpackTlvSUPSCallID(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceSetSUPSServiceResp *lResp =
    ((struct QmiVoiceSLQSSetSUPSServiceResp *)pResp)->pVoiceSetSUPSServiceResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCallID )
    {
        return eQCWWAN_ERR_NONE;
    }
    return GetByte( pTlvData, lResp->pCallID );
}

/*
 * This function unpacks the Alpha Identifier Info from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSUPSAlphaId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    alphaIDInfo *lResp = ((struct QmiVoiceSLQSSetSUPSServiceResp *)
                            pResp)->pVoiceSetSUPSServiceResp->pAlphaIDInfo;
    enum eQCWWANError eRCode;
    WORD              lcount;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Alpha Identifier responses into the list */
    eRCode = GetByte( pTlvData, &lResp->alphaDcs );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->alphaLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for( lcount = 0 ; lcount < lResp->alphaLen ; lcount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->alphaText[lcount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }
    return eRCode;
}

/*
 * This function unpacks the Call Control Result Type from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSUPSCCResultType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceSetSUPSServiceResp *lResp =
    ((struct QmiVoiceSLQSSetSUPSServiceResp *)pResp)->pVoiceSetSUPSServiceResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCCResultType )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCCResultType );
}

/*
 * This function unpacks the Call Control Supplementary Service Type from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSUPSCCSUPSType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceSetSUPSServiceResp *lResp =
    ((struct QmiVoiceSLQSSetSUPSServiceResp *)pResp)->pVoiceSetSUPSServiceResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCCSUPSType )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the CC Supplementary Service responses into the list */
    eRCode = GetByte( pTlvData, &lResp->pCCSUPSType->svcType );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }
    return GetByte( pTlvData, &lResp->pCCSUPSType->reason );
}

/*
 * This function unpacks the failure cause from the QMI response message to a
 * user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvFailureCause( BYTE *pTlvData, BYTE *pResp )
{
    voiceSetSUPSServiceResp *lResp =
    ((struct QmiVoiceSLQSSetSUPSServiceResp *)pResp)->pVoiceSetSUPSServiceResp;

    /* Check whether the parmeter is required */
    if ( NULL == lResp->pFailCause )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the failure casue into the list */
    return GetWord( pTlvData, lResp->pFailCause );
}

/*
 * This function unpacks the SLQS Set supplimentary service response message
 * to a user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsSetSUPSService(
    BYTE                                  *pMdmResp,
    struct QmiVoiceSLQSSetSUPSServiceResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,         &qmUnpackTlvResultCode },
        { eTLV_SUPS_FAILURE_CAUSE,  &UnpackTlvFailureCause },
        { eTLV_SUPS_ALPHA_ID,       &UnpackTlvSUPSAlphaId },
        { eTLV_SUPS_CALL_ID,        &UnpackTlvSUPSCallID },
        { eTLV_SUPS_CC_RESULT_TYPE, &UnpackTlvSUPSCCResultType },
        { eTLV_SUPS_CC_SUP_S_TYPE,  &UnpackTlvSUPSCCSUPSType },
        { eTLV_TYPE_INVALID,   NULL } /* Important. Sentinel.
                                       * Signifies last item in map.
                                       */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_SET_SUPS_SERVICE );
    return eRCode;
}
