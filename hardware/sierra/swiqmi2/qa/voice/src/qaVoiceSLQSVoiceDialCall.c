/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceDialCall.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_DIAL_CALL message.
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
#include "qaVoiceSLQSVoiceDialCall.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the Calling Number for the call to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvCallingNumber( BYTE *pBuf, BYTE *pParam )
{
    voiceCallRequestParams *pReq =
        ((struct QmiVoiceSLQSDialCallReq *)pParam)->pCallRequestParams;

    enum eQCWWANError eRCode;
    BYTE              lcount = 0;

    /* Insert Calling Number */
    while ( pReq->callNumber[lcount]!= '\0' )
    {
        /* Check index of calling number */
        if ( lcount > CALLING_NUMBER_MAX_INDEX )
        {
            break;
        }

        eRCode = PutByte( pBuf, pReq->callNumber[lcount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
        lcount++;
    }

    return eRCode;
}

/*
 * This function packs the Call Type for the call to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvCallType( BYTE *pBuf, BYTE *pParam )
{
    voiceCallRequestParams *pReq =
        ((struct QmiVoiceSLQSDialCallReq *)pParam)->pCallRequestParams;

    if ( NULL == pReq->pCallType )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Call Type */
    return PutByte( pBuf, *pReq->pCallType );
}

/*
 * This function packs the CLIR type values for the call to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvClirTmpMode( BYTE *pBuf, BYTE *pParam )
{
    voiceCallRequestParams *pReq =
        ((struct QmiVoiceSLQSDialCallReq *)pParam)->pCallRequestParams;

    if ( NULL == pReq->pCLIRType )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert CLIR Type */
    return PutByte( pBuf, *pReq->pCLIRType );
}

/*
 * This function packs the UUS Info for the call to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvUUSInfo( BYTE *pBuf, BYTE *pParam )
{
    UUSInfo *pReq =
        ((struct QmiVoiceSLQSDialCallReq *)pParam)->pCallRequestParams->pUUSInFo;
    enum eQCWWANError eRCode;
    BYTE              lcount;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert UUS Info */
    eRCode = PutByte( pBuf, pReq->UUSType );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->UUSDcs );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->UUSDatalen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lcount = 0 ; lcount < pReq->UUSDatalen ; lcount++ )
    {
        eRCode = PutByte( pBuf, pReq->UUSData[lcount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function packs the CUG Info for the call to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvCUGInfo( BYTE *pBuf, BYTE *pParam )
{
    CUGInfo *pReq = ((struct QmiVoiceSLQSDialCallReq *)
                        pParam)->pCallRequestParams->pCUGInfo;
    enum eQCWWANError eRCode;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Check if CUGIndex within permitted range or not */
    if ( pReq->CUGIndex > CUG_INDEX_MAX )
    {
        return eQCWWAN_ERR_QMI_INVALID_INDEX;
    }

    /* Insert CUG Info */
    eRCode = PutWord( pBuf, pReq->CUGIndex );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->SuppPrefCUG );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->SuppOA );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function packs the Emergency Category values for the call
 * to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvEmergencyCategory( BYTE *pBuf, BYTE *pParam )
{
    voiceCallRequestParams *pReq =
        ((struct QmiVoiceSLQSDialCallReq *)pParam)->pCallRequestParams;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( NULL == pReq->pEmergencyCategory ||
         NULL == pReq->pCallType )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Emergency Category only if CallType is Emergency */
    if ( CALL_TYPE_EMERGENCY == *pReq->pCallType )
    {
        eRCode = PutByte( pBuf, *pReq->pEmergencyCategory );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }
    return eRCode;
}

/*
 * This function packs the Called Party Subaddress Info for the call to
 * the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvCalledPartySubaddress( BYTE *pBuf, BYTE *pParam )
{
    calledPartySubAdd *pReq = ((struct QmiVoiceSLQSDialCallReq *)
                                 pParam)->pCallRequestParams->pCallPartySubAdd;
    enum eQCWWANError eRCode;
    BYTE              lcount;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Called Party Subaddress Info */
    eRCode = PutByte( pBuf, pReq->extBit );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->subAddrType );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->oddEvenInd );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->subAddrLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lcount = 0 ; lcount < pReq->subAddrLen ; lcount++ )
    {
        eRCode = PutByte( pBuf, pReq->subAddr[lcount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function packs the Service type values for the call to the
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
enum eQCWWANError BuildTlvServiceType( BYTE *pBuf, BYTE *pParam )
{
    voiceCallRequestParams *pReq =
            ((struct QmiVoiceSLQSDialCallReq *)pParam)->pCallRequestParams;

    if( NULL == pReq->pSvcType )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Service Type */
    return PutLong( pBuf, *(pReq->pSvcType) );
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
enum eQCWWANError PkQmiVoiceSlqsDialCall(
    WORD                   *pMlength,
    BYTE                   *pParamField,
    voiceCallRequestParams *pcallRequestParams )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_CALLING_NUMBER,  &BuildTlvCallingNumber },
        { eTLV_CALL_TYPE,       &BuildTlvCallType },
        { eTLV_CLIR_TMP_MODE,   &BuildTlvClirTmpMode },
        { eTLV_UUS_INFO,        &BuildTlvUUSInfo },
        { eTLV_CUG_INFO,        &BuildTlvCUGInfo },
        { eTLV_EMERGENCY_CAT,   &BuildTlvEmergencyCategory },
        { eTLV_CALLED_PARTY_SA, &BuildTlvCalledPartySubaddress },
        { eTLV_SERVICE_TYPE,    &BuildTlvServiceType },
        { eTLV_TYPE_INVALID,    NULL } /* Important. Sentinel.
                                        * Signifies last item in map
                                        */
    };

    struct QmiVoiceSLQSDialCallReq req;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiVoiceSLQSDialCallReq));
    req.pCallRequestParams = pcallRequestParams;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_VOICE_DIAL_CALL,
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
enum eQCWWANError UnpackTlvCallID(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceCallResponseParams *lResp = 
            ((struct QmiVoiceSLQSDialCallResp *)pResp)->pCallResponseParams;

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
enum eQCWWANError UnpackTlvAlphaId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    alphaIDInfo *lResp = ((struct QmiVoiceSLQSDialCallResp *)
                                     pResp)->pCallResponseParams->pAlphaIDInfo;
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
enum eQCWWANError UnpackTlvCCResultType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceCallResponseParams *lResp =
        ((struct QmiVoiceSLQSDialCallResp *)pResp)->pCallResponseParams;

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
enum eQCWWANError UnpackTlvCCSUPSType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceCallResponseParams *lResp =
        ((struct QmiVoiceSLQSDialCallResp *)pResp)->pCallResponseParams;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCCSUPSType )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* If Voice, Supplementary Services do not exist */
    if ( NULL != lResp->pCCResultType )
    {
        if ( CC_RESULT_TYPE_VOICE == *lResp->pCCResultType )
        {
            return eRCode;
        }
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
 * This function unpacks the SLQS Dial Call response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsDialCall(
    BYTE                            *pMdmResp,
    struct QmiVoiceSLQSDialCallResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,    &qmUnpackTlvResultCode },
        { eTLV_CALL_ID,        &UnpackTlvCallID },
        { eTLV_ALPHA_ID,       &UnpackTlvAlphaId },
        { eTLV_CC_RESULT_TYPE, &UnpackTlvCCResultType },
        { eTLV_CC_SUP_S_TYPE,  &UnpackTlvCCSUPSType },
        { eTLV_TYPE_INVALID,   NULL } /* Important. Sentinel.
                                       * Signifies last item in map.
                                       */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_DIAL_CALL );
    return eRCode;
}
