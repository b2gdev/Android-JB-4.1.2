/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCallForwardingStatus.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_VOICE_GET_CALL_FORWARDING message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceGetCallForwardingStatus.h"
#include "qaGobiApiVoiceCommon.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Call Forwarding Reason to the
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
enum eQCWWANError BuildTlvCallFWSReason( BYTE *pBuf, BYTE *pParam )
{
    voiceGetCallFWReq *pReq = (voiceGetCallFWReq *)pParam;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Reason */
    return PutByte( pBuf, pReq->Reason );
}

/*
 * This function packs the Call Forwarding Service Class values to the
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
enum eQCWWANError BuildTlvCFWSServClass( BYTE *pBuf, BYTE *pParam )
{
	voiceGetCallFWReq *pReq = (voiceGetCallFWReq *)pParam;

    if ( NULL == pReq->pSvcClass )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Service Class */
    return PutByte( pBuf, *pReq->pSvcClass );
}

/*
 * This function packs the SLQS Get Call Forwarding Status request parameters
 * to the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSVoiceGetCallForwardingStatus.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsGetCallFWStatus(
    WORD              *pMlength,
    BYTE              *pParamField,
    voiceGetCallFWReq *pVoiceGetCallFWReq  )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_CFWS_REASON,        &BuildTlvCallFWSReason },
        { eTLV_CFWS_SERVICE_CLASS, &BuildTlvCFWSServClass },
        { eTLV_TYPE_INVALID,       NULL } /* Important. Sentinel.
                                           * Signifies last item in map
                                           */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pVoiceGetCallFWReq,
                      map,
                      eQMI_VOICE_GET_CALL_FORWARDING,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Call Forwarding Information from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCFWSInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    getCallFWInfo *lResp = ((struct QmiVoiceSLQSGetCallFWResp *)
                                   pResp)->pVoiceGetCallFWResp->pGetCallFWInfo;
    enum eQCWWANError eRCode;
    BYTE              lCount, lIcount;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    eRCode = GetByte( pTlvData, &lResp->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < lResp->numInstances ; lCount++ )
    {
        callFWInfo *lresp = &lResp->CallFWInfo[lCount];
        /* Copy the call forwarding info responses into the list */
        eRCode = GetByte( pTlvData, &lresp->SvcStatus );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->SvcClass );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->numLen );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for ( lIcount = 0 ; lIcount < lresp->numLen ; lIcount++ )
        {
            eRCode = GetByte( pTlvData, &lresp->number[lIcount]);
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }

        eRCode = GetByte( pTlvData, &lresp->noReplyTimer );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }
    return eRCode;
}

/*
 * This function unpacks the Call Forwarding Failure Cause from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCFWSFailureCause(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCallFWResp *lResp =
           ((struct QmiVoiceSLQSGetCallFWResp *)pResp)->pVoiceGetCallFWResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pFailCause )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetWord( pTlvData, lResp->pFailCause );
}

/*
 * This function unpacks the Call Forwarding Alpha Identifier Info from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCFWSAlphaId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    alphaIDInfo *lResp = ((struct QmiVoiceSLQSGetCallFWResp *)
                                   pResp)->pVoiceGetCallFWResp->pAlphaIDInfo;

    return UnpackTlvCommonAlphaIdentifier( pTlvData, lResp );
}

/*
 * This function unpacks the Call Forwarding Result Type from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCFWSResultType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCallFWResp *lResp =
           ((struct QmiVoiceSLQSGetCallFWResp *)pResp)->pVoiceGetCallFWResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCCResType )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCCResType );
}

/*
 * This function unpacks the Call Forwarding Call Identifier from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCFWSCallId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCallFWResp *lResp =
           ((struct QmiVoiceSLQSGetCallFWResp *)pResp)->pVoiceGetCallFWResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCallID )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCallID );
}

/*
 * This function unpacks the Call Forwarding Call Control Supplementary Service
 * Type from the QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCFWSccSupsType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    ccSUPSType *lResp = ((struct QmiVoiceSLQSGetCallFWResp *)
                                pResp)->pVoiceGetCallFWResp->pCCSUPSType;

    return UnpackTlvCommonccSupsType( pTlvData, lResp );
}

/*
 * This function unpacks the Call Forwarding Extended Information from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCFWSExtInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    getCallFWExtInfo *lResp = ((struct QmiVoiceSLQSGetCallFWResp *)
                                pResp)->pVoiceGetCallFWResp->pGetCallFWExtInfo;
    enum eQCWWANError eRCode;
    BYTE              lCount, lIcount;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    eRCode = GetByte( pTlvData, &lResp->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < lResp->numInstances ; lCount++ )
    {
        callFWExtInfo *lresp = &lResp->CallFWExtInfo[lCount];
        /* Copy the call forwarding extended info responses into the list */
        eRCode = GetByte( pTlvData, &lresp->SvcStatus );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->SvcClass );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->noReplyTimer );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->PI );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->SI );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->numType );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->numPlan );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->numLen );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for ( lIcount = 0 ; lIcount < lresp->numLen ; lIcount++ )
        {
            eRCode = GetByte( pTlvData, &lresp->number[lIcount]);
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }
    }
    return eRCode;
}

/*
 * This function unpacks the SLQS Get Call Forwarding response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsGetCallFWStatus(
    BYTE                             *pMdmResp,
    struct QmiVoiceSLQSGetCallFWResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,        &qmUnpackTlvResultCode },
        { eTLV_CFWS_INFO,          &UnpackTlvCFWSInfo },
        { eTLV_CFWS_FAILURE_CAUSE, &UnpackTlvCFWSFailureCause },
        { eTLV_CFWS_ALPHA_ID,      &UnpackTlvCFWSAlphaId },
        { eTLV_CFWS_RESULT_TYPE,   &UnpackTlvCFWSResultType },
        { eTLV_CFWS_CALL_ID,       &UnpackTlvCFWSCallId },
        { eTLV_CFWS_CC_SUP_S_TYPE, &UnpackTlvCFWSccSupsType },
        { eTLV_CFWS_EXT_INFO,      &UnpackTlvCFWSExtInfo },
        { eTLV_TYPE_INVALID,       NULL } /* Important. Sentinel.
                                           * Signifies last item in map.
                                           */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_GET_CALL_FORWARDING );
    return eRCode;
}
