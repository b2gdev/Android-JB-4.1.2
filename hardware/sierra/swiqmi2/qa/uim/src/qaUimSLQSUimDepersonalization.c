/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimDepersonalization.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_UIM_DEPERSONALIZATION message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaUimSLQSUimDepersonalization.h"
#include "qaGobiApiUimCommon.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Depersonalization parameters to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvDepersonalization( BYTE *pBuf, BYTE *pParam )
{
    depersonalizationInformation *pReq =
                  &(( UIMDepersonalizationReq *)pParam)->depersonilisationInfo;
    enum eQCWWANError eRCode;
    WORD              lCount;

    /* Check For Invalid Parameter */
    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Insert Depersonalization Info */
    eRCode = PutByte( pBuf, pReq->feature );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->operation );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->ckLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount=0 ; lCount < pReq->ckLen ; lCount++ )
    {
        eRCode = PutByte( pBuf, pReq->ckVal[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function packs the SLQS Depersonalization request parameters to the QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaUimSLQSUimDepersonalization.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiUimSlqsDepersonalization(
    WORD                    *pMlength,
    BYTE                    *pParamField,
    UIMDepersonalizationReq *pUIMDepersonalizationReq )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_DEPERSONALIZATION, &BuildTlvDepersonalization },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map
                                          */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pUIMDepersonalizationReq,
                      map,
                      eQMI_UIM_DEPERSONALIZATION,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Retries Remaining Values from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvDPRetriesRem(
    BYTE *pTlvData,
    BYTE *pResp )
{
    remainingRetries *lResp = ((struct QmiUimSLQSDepersonalizationResp *)
                               pResp)->pUIMDepersonalizationResp->pRemainingRetries;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Retries Remaining Values into the List */
    return UnpackTlvCommonRetriesRem( pTlvData, lResp );
}

/*
 * This function unpacks the SLQS Depersonalization response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiUimSlqsDepersonalization(
    BYTE                                   *pMdmResp,
    struct QmiUimSLQSDepersonalizationResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,          &qmUnpackTlvResultCode },
        { eTLV_DP_RETRIES_REMAINING, &UnpackTlvDPRetriesRem },
        { eTLV_TYPE_INVALID,         NULL } /* Important. Sentinel.
                                             * Signifies last item in map.
                                             */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_UIM_DEPERSONALIZATION );
    return eRCode;
}
