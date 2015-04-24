/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetAllCallInfo.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_GET_ALL_CALL_INFO message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceGetAllCallInfo.h"
#include "qaGobiApiVoiceCommon.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the SLQS Get All Call Info request parameters to the
 * QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSGetAllCallInfo.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsGetAllCallInfo(
    WORD *pMlength,
    BYTE *pParamField )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map
                                     */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_VOICE_GET_ALL_CALL_INFO,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the array of Call Info from the QMI response message
 * to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvArrayCallInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    arrCallInfo *lResp = ((struct QmiVoiceSLQSGetAllCallInfoResp *)
                                     pResp)->pGetAllCallInfoResp->pArrCallInfo;

    return UnpackTlvCommonArrCallInfo( pTlvData, lResp );
}

/*
 * This function unpacks the array of Remote party numbers from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvArrayRemotePartyNum(
    BYTE *pTlvData,
    BYTE *pResp )
{
    arrRemotePartyNum *lResp = ((struct QmiVoiceSLQSGetAllCallInfoResp *)
                                   pResp)->pGetAllCallInfoResp->pArrRemotePartyNum;

    return UnpackTlvCommonArrRemPartyNum( pTlvData, lResp );
}

/*
 * This function unpacks the array of Remote party names from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvArrayRemotePartyName(
    BYTE *pTlvData,
    BYTE *pResp )
{
    arrRemotePartyName *lResp = ((struct QmiVoiceSLQSGetAllCallInfoResp *)
                                   pResp)->pGetAllCallInfoResp->pArrRemotePartyName;

    return UnpackTlvCommonArrRemPartyName( pTlvData, lResp );
}

/*
 * This function unpacks the array of Alerting Type from the QMI response message
 * to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvArrayAlertingType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    arrAlertingType *lResp = ((struct QmiVoiceSLQSGetAllCallInfoResp *)
                                     pResp)->pGetAllCallInfoResp->pArrAlertingType;

    return UnpackTlvCommonArrAlertType( pTlvData, lResp );
}

/*
 * This function unpacks the array of UUS Information from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvArrayUUSInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    arrUUSInfo *lResp = ((struct QmiVoiceSLQSGetAllCallInfoResp *)
                                     pResp)->pGetAllCallInfoResp->pArrUUSInfo;
    enum eQCWWANError eRCode;
    BYTE              lcount , lIcount;

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

    for ( lcount = 0 ; lcount < lResp->numInstances ; lcount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->AllCallsUUSInfo[lcount].callID );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
        UUSInfo *lresp = &lResp->AllCallsUUSInfo[lcount].uusInfo;

        /* Copy the UUS Info responses into the list */
        eRCode = GetByte( pTlvData, &lresp->UUSType );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->UUSDcs );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->UUSDatalen );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for( lIcount = 0 ; lIcount < lresp->UUSDatalen ; lIcount++ )
        {
            eRCode = GetByte( pTlvData, &lresp->UUSData[lIcount] );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }
    }
    return eRCode;
}

/*
 * This function unpacks the array of Service option from the QMI response message
 * to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvArrayServiceOption(
    BYTE *pTlvData,
    BYTE *pResp )
{
    arrSvcOption *lResp = ((struct QmiVoiceSLQSGetAllCallInfoResp *)
                                     pResp)->pGetAllCallInfoResp->pArrSvcOption;

    return UnpackTlvCommonArrServiceOpt( pTlvData, lResp );
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
enum eQCWWANError UnpackTlvAllOTASPStatus(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetAllCallInfo *lResp =
         ((struct QmiVoiceSLQSGetAllCallInfoResp *)pResp)->pGetAllCallInfoResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pOTASPStatus )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pOTASPStatus );
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
enum eQCWWANError UnpackTlvAllVoicePrivacy(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetAllCallInfo *lResp =
         ((struct QmiVoiceSLQSGetAllCallInfoResp *)pResp)->pGetAllCallInfoResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pVoicePrivacy )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pVoicePrivacy );
}

/*
 * This function unpacks the array of Call End Reasons from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvArrayCallEndReason(
    BYTE *pTlvData,
    BYTE *pResp )
{
    arrCallEndReason *lResp = ((struct QmiVoiceSLQSGetAllCallInfoResp *)
                                     pResp)->pGetAllCallInfoResp->pArrCallEndReason;

    return UnpackTlvCommonArrCallEndReason( pTlvData, lResp );
}

/*
 * This function unpacks the array of Alpha Identifier Info from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvArrayAlphaID(
    BYTE *pTlvData,
    BYTE *pResp )
{
    arrAlphaID *lResp = ((struct QmiVoiceSLQSGetAllCallInfoResp *)
                                     pResp)->pGetAllCallInfoResp->pArrAlphaID;

    return UnpackTlvCommonArrAlphaID( pTlvData, lResp );
}

/*
 * This function unpacks the array of Connected Party Numbers from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvArrayConPartyNum(
    BYTE *pTlvData,
    BYTE *pResp )
{
    arrConnectPartyNum *lResp = ((struct QmiVoiceSLQSGetAllCallInfoResp *)
                                     pResp)->pGetAllCallInfoResp->pArrConnectPartyNum;

    return UnpackTlvCommonArrConnPartyNum( pTlvData, lResp );
}

/*
 * This function unpacks the array of Diagnostic Info from the QMI response message
 * to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvArrayDiagInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    arrDiagInfo *lResp = ((struct QmiVoiceSLQSGetAllCallInfoResp *)
                                     pResp)->pGetAllCallInfoResp->pArrDiagInfo;

    return UnpackTlvCommonArrDiagInfo( pTlvData, lResp );
}

/*
 * This function unpacks the array of Called Party Numbers from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvArrayCalledPartyNum(
    BYTE *pTlvData,
    BYTE *pResp )
{
    arrCalledPartyNum *lResp = ((struct QmiVoiceSLQSGetAllCallInfoResp *)
                                     pResp)->pGetAllCallInfoResp->pArrCalledPartyNum;

    return UnpackTlvCommonArrCalledPartyNum( pTlvData, lResp );
}

/*
 * This function unpacks the array of Redirected Party Numbers from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvArrayRedirPartyNum(
    BYTE *pTlvData,
    BYTE *pResp )
{
    arrRedirPartyNum *lResp = ((struct QmiVoiceSLQSGetAllCallInfoResp *)
                                pResp)->pGetAllCallInfoResp->pArrRedirPartyNum;

    return UnpackTlvCommonArrRedPartyNum( pTlvData, lResp );
}

/*
 * This function unpacks the array of Alerting Pattern from the QMI response message
 * to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvArrayAlertingPattern(
    BYTE *pTlvData,
    BYTE *pResp )
{
    arrAlertingPattern *lResp = ((struct QmiVoiceSLQSGetAllCallInfoResp *)
                                     pResp)->pGetAllCallInfoResp->pArrAlertingPattern;

    return UnpackTlvCommonArrAlertPattern( pTlvData, lResp );
}

/*
 * This function unpacks the SLQS Get All Call Info response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsGetAllCallInfo(
    BYTE                                  *pMdmResp,
    struct QmiVoiceSLQSGetAllCallInfoResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,             &qmUnpackTlvResultCode },
        { eTLV_ARRAY_CALL_INFO,         &UnpackTlvArrayCallInfo },
        { eTLV_ARRAY_REMOTE_PARTY_NUM,  &UnpackTlvArrayRemotePartyNum },
        { eTLV_ARRAY_REMOTE_PARTY_NAME, &UnpackTlvArrayRemotePartyName },
        { eTLV_ARRAY_ALERTING_TYPE,     &UnpackTlvArrayAlertingType },
        { eTLV_ARRAY_UUS_INFO,          &UnpackTlvArrayUUSInfo },
        { eTLV_ARRAY_SERVICE_OPTION,    &UnpackTlvArrayServiceOption },
        { eTLV_ALL_OTASP_STATUS,        &UnpackTlvAllOTASPStatus },
        { eTLV_ALL_VOICE_PRIVACY,       &UnpackTlvAllVoicePrivacy },
        { eTLV_ARRAY_CALL_END_REASON,   &UnpackTlvArrayCallEndReason },
        { eTLV_ARRAY_ALPHA_IDENTIFIER,  &UnpackTlvArrayAlphaID },
        { eTLV_ARRAY_CON_PARTY_NUM,     &UnpackTlvArrayConPartyNum },
        { eTLV_ARRAY_DIAGNOSTIC_INFO,   &UnpackTlvArrayDiagInfo },
        { eTLV_ARRAY_CALLED_PARTY_NUM,  &UnpackTlvArrayCalledPartyNum },
        { eTLV_ARRAY_REDIR_PARTY_NUM,   &UnpackTlvArrayRedirPartyNum },
        { eTLV_ARRAY_ALERTING_PATTERN,  &UnpackTlvArrayAlertingPattern },
        { eTLV_TYPE_INVALID,            NULL } /* Important. Sentinel.
                                                * Signifies last item in map.
                                                */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_GET_ALL_CALL_INFO );
    return eRCode;
}
