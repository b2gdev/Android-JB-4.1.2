/*
 * \ingroup oma
 *
 * \file    qaOmaDmGetSessionInfo.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_OMA_DM_GET_SESSION_INFO message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaOmaDmGetSessionInfo.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the OMADMGetSessionInfo parameters to the QMI message
 * SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaGobiApiOmadms.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiOmaDmGetSessionInfo(
    WORD *pMlength,
    BYTE *pParamField )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_OMA_GET_SESSION_INFO,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the OMADMGetSessionInfo Session Info from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvSessionInformation( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiOmaDmGetSessionInfoResp *lResp =
        (struct QmiOmaDmGetSessionInfoResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              lByte  = 0;

    if ( !lResp->pSessionState ||
         !lResp->pSessionType )
         return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                            eTLV_OMA_DM_SESSION_INFO_LENGTH );

    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        /* Session State */
        eRCode = GetByte( pTlvData, &lByte );
        if ( eQCWWAN_ERR_NONE == eRCode )
        {
            *(lResp->pSessionState) = lByte;
            /* Session Type */
            eRCode = GetByte( pTlvData, &lByte );
            if ( eQCWWAN_ERR_NONE == eRCode )
                *(lResp->pSessionType) = lByte;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the OMADMGetSessionInfo Failure Reason from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvFailureReason( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiOmaDmGetSessionInfoResp *lResp =
        (struct QmiOmaDmGetSessionInfoResp *)pResp;

    enum eQCWWANError eRCode         = eQCWWAN_ERR_NONE;
    BYTE              lFailureReason = 0;

    if ( !lResp->pFailureReason )
        return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                            eTLV_OMA_DM_FAILURE_REASON_LENGTH);

    if( eQCWWAN_ERR_NONE == eRCode )
    {
        /* Session Failure Reason*/
        eRCode = GetByte( pTlvData, &lFailureReason );
        if ( eQCWWAN_ERR_NONE == eRCode )
        {
            *(lResp->pFailureReason) = lFailureReason;
        }
    }
    return eRCode;
}

/*
 * This function unpacks the OMADMGetSessionInfo Retry Info from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvRetryInfo( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiOmaDmGetSessionInfoResp *lResp =
        (struct QmiOmaDmGetSessionInfoResp *)pResp;

    enum eQCWWANError eRCode      = eQCWWAN_ERR_NONE;
    WORD              lByte       = 0;
    BYTE              lRetryCount = 0;

    if ( !lResp->pRetryCount ||
         !lResp->pSessionPause ||
         !lResp->pTimeRemaining )
        return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                            eTLV_OMA_DM_RETRY_INFO_LENGTH );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Session Retry Count*/
    eRCode = GetByte( pTlvData, &lRetryCount );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    *(lResp->pRetryCount) = lRetryCount;
    /* Session Pause timer*/
    eRCode = GetWord( pTlvData, &lByte );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;    
                
    *(lResp->pSessionPause) = lByte;
    /* Pause Time remaining */
    eRCode = GetWord( pTlvData, &lByte );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;
    
    *(lResp->pTimeRemaining) = lByte;
    return eRCode;
}

/*
 * This function unpacks the OMADMGetPendingNIA Retry Info from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvNetworkInitiatedAlert( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiOmaDmGetSessionInfoResp *lResp =
                (struct QmiOmaDmGetSessionInfoResp *)pResp;

    enum eQCWWANError eRCode       = eQCWWAN_ERR_NONE;
    WORD              lSessionID   = 0;
    BYTE              lSessionType = 0;

    if ( !lResp->pNIASessionType ||
         !lResp->pNIASessionID  )
    {
        return eRCode;
    }

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                  eTLV_OMA_DM_NETWORK_INITIATED_ALERT_LENGTH );

    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        eRCode = GetByte( pTlvData, &lSessionType );
        if ( eQCWWAN_ERR_NONE == eRCode )
        {
            /* Extract NIASessionType */
            *(lResp->pNIASessionType) = lSessionType;
            eRCode = GetWord( pTlvData, &lSessionID );
            if ( eQCWWAN_ERR_NONE == eRCode )
            {
                /* Extract NIASessionID*/
                *(lResp->pNIASessionID) = lSessionID;
            }
        }
    }

    return eRCode;
}

/*
 * This function unpacks the OMADMGetSessionInfo response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiOmaDmGetSessionInfo(
    BYTE                              *pMdmResp,
    struct QmiOmaDmGetSessionInfoResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,                    &qmUnpackTlvResultCode },
        { eTLV_OMA_DM_GET_SESSION_INFO,        &UnpackTlvSessionInformation },
        { eTLV_OMA_DM_SESSION_FAILURE_REASON,  &UnpackTlvFailureReason },
        { eTLV_OMA_DM_RETRY_INFO,              &UnpackTlvRetryInfo },
        { eTLV_OMA_DM_NETWORK_INITIATED_ALERT, &UnpackTlvNetworkInitiatedAlert},
        { eTLV_TYPE_INVALID,                   NULL } /* Important. Sentinel.
                                                      * Signifies last item in map.
                                                      */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_OMA_GET_SESSION_INFO );
    return eRCode;
}
