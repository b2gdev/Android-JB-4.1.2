/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCallInfo.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_GET_CALL_INFO message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceGetCallInfo.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the Call ID for the call to the QMI message SDU.
 * Mandatory TLV.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvCallID( BYTE *pBuf, BYTE *pParam )
{
	voiceCallInfoReq *pReq = (voiceCallInfoReq *)pParam;

    /* Insert Call ID */
    return PutByte( pBuf, pReq->callID );
}

/*
 * This function packs the SLQS Get Call Info request parameters to the
 * QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSGetCallInfo.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsGetCallInfo(
    WORD             *pMlength,
    BYTE             *pParamField,
    voiceCallInfoReq *pGetCallInfoReq  )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_GET_CALL_ID,  &BuildTlvCallID },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map
                                     */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pGetCallInfoReq,
                      map,
                      eQMI_VOICE_GET_CALL_INFO,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Call Info from the QMI response message
 * to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCallInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    callInfo *lResp =
    ((struct QmiVoiceSLQSGetCallInfoResp *)pResp)->pGetCallInfoResp->pCallInfo;
    enum eQCWWANError eRCode;

    /* Check For Invalid Parameter */
    if ( !lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Call Info responses into the list */
    eRCode = GetByte( pTlvData, &lResp->callID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->callState );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->callType );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->direction );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->mode );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the Remote Party Number from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRemotePartyNum(
    BYTE *pTlvData,
    BYTE *pResp )
{
    remotePartyNum *lResp = ((struct QmiVoiceSLQSGetCallInfoResp *)
                                     pResp)->pGetCallInfoResp->pRemotePartyNum;
    enum eQCWWANError eRCode;
    BYTE              lcount;

    /* Check For Invalid Parameter */
    if ( !lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Remote Party Number responses into the list */
    eRCode = GetByte( pTlvData, &lResp->presentationInd);
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->numLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for( lcount = 0 ; lcount < lResp->numLen ; lcount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->remPartyNumber[lcount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the Service Option from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvServiceOption(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceCallInfoResp *lResp =
              ((struct QmiVoiceSLQSGetCallInfoResp *)pResp)->pGetCallInfoResp;

    /* Check For Invalid Parameter */
    if ( !lResp->pSrvOpt )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetWord( pTlvData, lResp->pSrvOpt );
}

/*
 * This function unpacks the Voice Privacy Info from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvVoicePrivacy(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceCallInfoResp *lResp =
              ((struct QmiVoiceSLQSGetCallInfoResp *)pResp)->pGetCallInfoResp;

    /* Check For Invalid Parameter */
    if ( !lResp->pVoicePrivacy )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pVoicePrivacy );
}

/*
 * This function unpacks the OTASP Status from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvOTASPStatus(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceCallInfoResp *lResp =
              ((struct QmiVoiceSLQSGetCallInfoResp *)pResp)->pGetCallInfoResp;

    /* Check For Invalid Parameter */
    if ( !lResp->pOTASPStatus )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pOTASPStatus );
}

/*
 * This function unpacks the Remote Party Name from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRemotePartyName(
    BYTE *pTlvData,
    BYTE *pResp )
{
    remotePartyName *lResp = ((struct QmiVoiceSLQSGetCallInfoResp *)
                                     pResp)->pGetCallInfoResp->pRemotePartyName;
    enum eQCWWANError eRCode;
    BYTE              lcount;

    /* Check For Invalid Parameter */
    if ( !lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Remote Party Name responses into the list */
    eRCode = GetByte( pTlvData, &lResp->namePI );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->codingScheme );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->nameLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for( lcount = 0 ; lcount < lResp->nameLen ; lcount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->callerName[lcount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the UUS Information from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvUUSInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UUSInfo *lResp =
     ((struct QmiVoiceSLQSGetCallInfoResp *)pResp)->pGetCallInfoResp->pUUSInfo;
    enum eQCWWANError eRCode;
    BYTE              lcount;

    /* Check For Invalid Parameter */
    if ( !lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the UUS Info responses into the list */
    eRCode = GetByte( pTlvData, &lResp->UUSType );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->UUSDcs );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->UUSDatalen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for( lcount = 0 ; lcount < lResp->UUSDatalen ; lcount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->UUSData[lcount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the Alerting Type from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvAlertingType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceCallInfoResp *lResp =
              ((struct QmiVoiceSLQSGetCallInfoResp *)pResp)->pGetCallInfoResp;

    /* Check For Invalid Parameter */
    if ( !lResp->pAlertType )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pAlertType );
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
enum eQCWWANError UnpackTlvAlphaID(
    BYTE *pTlvData,
    BYTE *pResp )
{
    alphaIDInfo *lResp = ((struct QmiVoiceSLQSGetCallInfoResp *)
                                     pResp)->pGetCallInfoResp->pAlphaIDInfo;
    enum eQCWWANError eRCode;
    WORD              lcount;

    /* Check For Invalid Parameter */
    if ( !lResp )
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
 * This function unpacks the Connected Number Information from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvConnectNumInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    connectNumInfo *lResp = ((struct QmiVoiceSLQSGetCallInfoResp *)
                                     pResp)->pGetCallInfoResp->pConnectNumInfo;
    enum eQCWWANError eRCode;
    BYTE              lcount;

    /* Check For Invalid Parameter */
    if ( !lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Connected Number Information responses into the list */
    eRCode = GetByte( pTlvData, &lResp->numPresInd );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->screeningInd );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->numType );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->numPlan );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->callerIDLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for( lcount = 0 ; lcount < lResp->callerIDLen ; lcount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->callerID[lcount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the Diagnostic Information from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvDiagnosticInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    diagInfo *lResp = ((struct QmiVoiceSLQSGetCallInfoResp *)
                                     pResp)->pGetCallInfoResp->pDiagInfo;
    enum eQCWWANError eRCode;
    BYTE              lcount;

    /* Check For Invalid Parameter */
    if ( !lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Diagnostic Information responses into the list */
    eRCode = GetByte( pTlvData, &lResp->diagInfoLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for( lcount = 0 ; lcount < lResp->diagInfoLen ; lcount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->diagnosticInfo[lcount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the Alerting Pattern from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvAlertingPattern(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceCallInfoResp *lResp =
              ((struct QmiVoiceSLQSGetCallInfoResp *)pResp)->pGetCallInfoResp;

    /* Check For Invalid Parameter */
    if ( !lResp->pAlertingPattern )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetLong( pTlvData, lResp->pAlertingPattern );
}

/*
 * This function unpacks the SLQS Get Call Info response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsGetCallInfo(
    BYTE                               *pMdmResp,
    struct QmiVoiceSLQSGetCallInfoResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,       &qmUnpackTlvResultCode },
        { eTLV_CALL_INFO,         &UnpackTlvCallInfo },
        { eTLV_REMOTE_PARTY_NUM,  &UnpackTlvRemotePartyNum },
        { eTLV_SERVICE_OPTION,    &UnpackTlvServiceOption },
        { eTLV_VOICE_PRIVACY,     &UnpackTlvVoicePrivacy },
        { eTLV_OTASP_STATUS,      &UnpackTlvOTASPStatus },
        { eTLV_REMOTE_PARTY_NAME, &UnpackTlvRemotePartyName },
        { eTLV_GET_UUS_INFO,      &UnpackTlvUUSInfo },
        { eTLV_ALERTING_TYPE,     &UnpackTlvAlertingType },
        { eTLV_ALPHA_IDENTIFIER,  &UnpackTlvAlphaID },
        { eTLV_CONNECT_NUM_INFO,  &UnpackTlvConnectNumInfo },
        { eTLV_DIAGNOSTIC_INFO,   &UnpackTlvDiagnosticInfo },
        { eTLV_ALERTING_PATTERN,  &UnpackTlvAlertingPattern },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map.
                                          */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_GET_CALL_INFO );
    return eRCode;
}
