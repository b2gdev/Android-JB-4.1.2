/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceSendFlash.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_VOICE_SEND_FLASH message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceSendFlash.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Send Flash Call Id to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSFCallId( BYTE *pBuf, BYTE *pParam )
{
    voiceFlashInfo *pReq =(voiceFlashInfo *)pParam;

    if ( NULL == pReq->pCallID )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Add Call Id */
    return PutByte( pBuf, *pReq->pCallID );
}

/*
 * This function packs the Send Flash Pay Load values to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSFPayload( BYTE *pBuf, BYTE *pParam )
{
    voiceFlashInfo *pReq =(voiceFlashInfo *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              lCount = 0;

    if ( NULL == pReq->pFlashPayLd )
    {
        return eQCWWAN_ERR_NONE;
    }

    while ( pReq->pFlashPayLd[lCount]!= EOS )
    {
        eRCode = PutByte( pBuf, pReq->pFlashPayLd[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
        lCount++;
    }

    return eRCode;
}

/*
 * This function packs the Send Flash Type values to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSFType( BYTE *pBuf, BYTE *pParam )
{
    voiceFlashInfo *pReq =(voiceFlashInfo *)pParam;

    if ( NULL == pReq->pFlashType )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add Flash Type */
    return PutByte( pBuf, *pReq->pFlashType );
}

/*
 * This function packs the SLQS Send Flash request parameters to
 * the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSVoiceSendFlash.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsSendFlash(
    WORD           *pMlength,
    BYTE           *pParamField,
    voiceFlashInfo *pFlashInfo )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SEND_FLASH_CALL_ID, &BuildTlvSFCallId },
        { eTLV_SEND_FLASH_PAYLOAD, &BuildTlvSFPayload },
        { eTLV_SEND_FLASH_TYPE,    &BuildTlvSFType },
        { eTLV_TYPE_INVALID,       NULL } /* Important. Sentinel.
                                           * Signifies last item in map
                                           */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pFlashInfo,
                      map,
                      eQMI_VOICE_SEND_FLASH,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Send Flash Call Id from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSFCallIdRsp(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct QmiVoiceSLQSSendFlashResp *lResp =
                             ((struct QmiVoiceSLQSSendFlashResp *)pResp);

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCallId )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCallId );
}

/*
 * This function unpacks the SLQS Send Flash response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsSendFlash(
    BYTE                             *pMdmResp,
    struct QmiVoiceSLQSSendFlashResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,            &qmUnpackTlvResultCode },
        { eTLV_SEND_FLASH_CALL_ID_RSP, &UnpackTlvSFCallIdRsp },
        { eTLV_TYPE_INVALID,           NULL } /* Important. Sentinel.
                                               * Signifies last item in map.
                                               */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_SEND_FLASH );
    return eRCode;
}
