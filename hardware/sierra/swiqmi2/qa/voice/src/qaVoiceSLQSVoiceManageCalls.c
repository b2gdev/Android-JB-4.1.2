/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceManageCalls.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_MANAGE_CALLS message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceManageCalls.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the supplementary Service type fields to the
 * QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvsupplementaryInfo( BYTE *pBuf, BYTE *pParam )
{
    voiceManageCallsReq *pReq = (voiceManageCallsReq *)pParam;
    /* Add TLV data */
    return PutByte( pBuf, pReq->SUPSType);
}

/*
 * This function packs the CallID type fields to the QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvCallidInfo( BYTE *pBuf, BYTE *pParam )
{
    voiceManageCallsReq *pReq = (voiceManageCallsReq *)pParam;

    /* Optional parameter check against NULL */
    if ( !pReq->pCallID )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *pReq->pCallID );
}

/*
 * This function packs the Manage Calls request parameters to the QMI
 * message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaGobiApiVoice.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceManageCalls(
    WORD                *pMlength,
    BYTE                *pParamField,
    voiceManageCallsReq *pVoiceManageCallsReq )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SUPS_TYPE,      &BuildTlvsupplementaryInfo },
        { eTLV_CALL_ID_MANAGE, &BuildTlvCallidInfo },
        { eTLV_TYPE_INVALID,   NULL } /* Important. Sentinel.
                                       * Signifies last item in map
                                       */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pVoiceManageCallsReq,
                      map,
                      eQMI_VOICE_MANAGE_CALLS,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/**
 * This function unpacks the Failure Cause from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE
 */
enum eQCWWANError UnpackTlvFailureCuase(BYTE *pTlvData, BYTE *pResp)
{
    voiceManageCallsResp *lResp =
        ((struct QmiVoiceManageCallsResp *)pResp)->pVoiceManageCallsResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pFailCause )
    {
        return eRCode;
    }

    eRCode = GetWord(pTlvData, lResp->pFailCause);
    return eRCode;
}

/*
 * This function unpacks the Manage Calls response message to a user-provided
 * response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceManageCalls(
    BYTE                           *pMdmResp,
    struct QmiVoiceManageCallsResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,   &qmUnpackTlvResultCode },
        { eTLV_FAILURE_CAUSE, &UnpackTlvFailureCuase },
        { eTLV_TYPE_INVALID,  NULL } /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_MANAGE_CALLS );
    return eRCode;
}
