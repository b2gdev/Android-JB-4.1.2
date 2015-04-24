/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimGetFileAttributes.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_UIM_GET_FILE_ATTRIBUTES message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaUimSLQSUimGetFileAttributes.h"
#include "qaGobiApiUimCommon.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Session Information parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvGetFASessInfo( BYTE *pBuf, BYTE *pParam )
{
    UIMSessionInformation *pReq =
                       &(( UIMGetFileAttributesReq *)pParam)->sessionInfo;

    /* Check For Invalid Parameter */
    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Insert Session Information */
    return PackTlvCommonSessionInfo( pBuf, pReq );
}

/*
 * This function packs the File ID parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvGetFAFileID( BYTE *pBuf, BYTE *pParam )
{
    fileInfo *pReq = &(( UIMGetFileAttributesReq *)pParam)->fileIndex;
    enum eQCWWANError eRCode;
    WORD              lCount;

    /* Check For Invalid Parameter */
    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Insert File ID */
    eRCode = PutWord( pBuf, pReq->fileID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->pathLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount=0 ; lCount < pReq->pathLen ; lCount++ )
    {
        eRCode = PutWord( pBuf, pReq->path[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function packs the Response Indication parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvGetFARespIndication( BYTE *pBuf, BYTE *pParam )
{
    UIMGetFileAttributesReq *pReq = ( UIMGetFileAttributesReq *)pParam;

    /* Check For Invalid Parameter */
    if ( NULL == pReq->pIndicationToken )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Session Information */
    return PutLong( pBuf, *pReq->pIndicationToken );
}

/*
 * This function packs the SLQS GetFileAttributes request parameters to the QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaUimSLQSUimGetFileAttributes.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiUimSlqsGetFileAttributes(
    WORD                    *pMlength,
    BYTE                    *pParamField,
    UIMGetFileAttributesReq *pUIMGetFileAttributesReq )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_GET_FA_SESS_INFO, &BuildTlvGetFASessInfo },
        { eTLV_GET_FA_FILE_ID,   &BuildTlvGetFAFileID },
        { eTLV_GET_FA_RESP_IND,  &BuildTlvGetFARespIndication },
        { eTLV_TYPE_INVALID,     NULL } /* Important. Sentinel.
                                         * Signifies last item in map
                                         */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pUIMGetFileAttributesReq,
                      map,
                      eQMI_UIM_GET_FILE_ATTRIBUTES,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Card Result Values from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvFACardResult(
    BYTE *pTlvData,
    BYTE *pResp )
{
    cardResult *lResp = ((struct QmiUimSLQSGetFileAttributesResp *)
                               pResp)->pUIMGetFileAttributesResp->pCardResult;
    enum eQCWWANError eRCode;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the card result values into the List */
    eRCode = GetByte( pTlvData, &lResp->sw1 );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &lResp->sw2 );
}

/*
 * This function unpacks the File Attribute Values from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvFileAttributes(
    BYTE *pTlvData,
    BYTE *pResp )
{
    fileAttributes *lResp = ((struct QmiUimSLQSGetFileAttributesResp *)
                            pResp)->pUIMGetFileAttributesResp->pFileAttributes;
    enum eQCWWANError eRCode;
    BYTE              lcount;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the file attribute values into the List */
    eRCode = GetWord( pTlvData, &lResp->fileSize );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->fileID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->fileType );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->recordSize );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->recordCount );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->secRead );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->secReadMask );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->secWrite );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->secWriteMask );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->secIncrease );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->secIncreaseMask );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->secDeactivate );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->secDeactivateMask );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->secActivate );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->secActivateMask );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->rawLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lcount=0; lcount < lResp->rawLen ; lcount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->rawValue[lcount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the Indication Token from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvFARespIndication(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UIMGetFileAttributesResp *lResp =
    ((struct QmiUimSLQSGetFileAttributesResp *)pResp)->pUIMGetFileAttributesResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pIndicationToken )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Indication Token into the List */
    return GetLong( pTlvData, lResp->pIndicationToken );
}

/*
 * This function unpacks the SLQS Get Card Status response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiUimSlqsGetFileAttributes(
    BYTE                                   *pMdmResp,
    struct QmiUimSLQSGetFileAttributesResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,          &qmUnpackTlvResultCode },
        { eTLV_GET_FA_CARD_RESULT,   &UnpackTlvFACardResult },
        { eTLV_GET_FILE_ATTRIBUTES,  &UnpackTlvFileAttributes },
        { eTLV_GET_FA_RESP_IND_RESP, &UnpackTlvFARespIndication },
        { eTLV_TYPE_INVALID,         NULL } /* Important. Sentinel.
                                             * Signifies last item in map.
                                             */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_UIM_GET_FILE_ATTRIBUTES );
    return eRCode;
}
