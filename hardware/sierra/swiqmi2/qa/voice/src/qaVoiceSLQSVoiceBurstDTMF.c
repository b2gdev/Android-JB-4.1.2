/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceBurstDTMF.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_BURST_DTMF message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiVoice.h"
#include "qaVoiceSLQSVoiceBurstDTMF.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the Burst DTMF Information fields to the
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
enum eQCWWANError BuildTlvBurstDTMFInfo( BYTE *pBuf, BYTE *pParam )
{
    voiceBurstDTMFInfo *lReq = (voiceBurstDTMFInfo *)pParam;
    burstDTMFInfo *pReq = &lReq->BurstDTMFInfo;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              lByte  = 0;

    /* Mandatory parameter check against NULL */
    if ( !pReq->pCallID )
    {
         return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Add TLV data */
    eRCode = PutByte( pBuf, *pReq->pCallID);
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }
    eRCode = PutByte( pBuf, pReq->digitCnt );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    lByte = pReq->digitCnt;
    while(lByte-- )
    {
        eRCode = PutByte( pBuf, pReq->pDigitBuff[lByte] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }
    return eRCode;
}
/*
 * This function packs the Burst DTMF Information fields to the
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
enum eQCWWANError BuildTlvDTMFLength( BYTE *pBuf, BYTE *pParam )
{
    voiceBurstDTMFInfo *lReq = (voiceBurstDTMFInfo *)pParam;
    DTMFLengths        *pReq = lReq->pBurstDTMFLengths;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Optional parameter check against NULL */
    if ( !pReq )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    eRCode = PutByte( pBuf, pReq->DTMFPulseWidth );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }
    return PutByte( pBuf, pReq->DTMFInterdigitInterval );
}

/*
 * This function packs the DTMF Length request parameters to the QMI message
 * SDU
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
enum eQCWWANError PkQmiVoiceSlqsBurstDTMF(
    WORD               *pMlength,
    BYTE               *pParamField,
    voiceBurstDTMFInfo *pBurstDTMFInfo )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_BURST_DTMF_INFO_REQ, &BuildTlvBurstDTMFInfo },
        { eTLV_DTMF_LENGTH_REQ,     &BuildTlvDTMFLength },
        { eTLV_TYPE_INVALID,        NULL } /* Important. Sentinel.
                                            * Signifies last item in map
                                            */
    };


    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pBurstDTMFInfo,
                      map,
                      eQMI_VOICE_BURST_DTMF,
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
enum eQCWWANError UnpackTlvDTMFCallId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct QmiVoiceSLQSBurstDTMFResp *lResp =
        (struct QmiVoiceSLQSBurstDTMFResp *)pResp;

    /* Mandatory parameter check against NULL */
    if ( !lResp->pCallId )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return GetByte( pTlvData, lResp->pCallId );
}

/*
 * This function unpacks the BurstDTMF response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSLQSBurstDTMF(
    BYTE                             *pMdmResp,
    struct QmiVoiceSLQSBurstDTMFResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,       &qmUnpackTlvResultCode },
        { eTLV_DTMF_CALL_ID_RESP, &UnpackTlvDTMFCallId },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map.
                                          */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_BURST_DTMF );
    return eRCode;
}
