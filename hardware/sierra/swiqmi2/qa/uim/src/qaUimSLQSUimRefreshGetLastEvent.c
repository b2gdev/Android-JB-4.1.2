/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimRefreshGetLastEvent.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_UIM_REFRESH_GET_LAST_EVENT message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaUimSLQSUimRefreshGetLastEvent.h"
#include "qaGobiApiUimCommon.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Session Information Last Evenet parameter to the QMI
 * message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSessionInfoRefLastEvent( BYTE *pBuf, BYTE *pParam )
{
    UIMSessionInformation *pReq =
          &(( UIMRefreshGetLastEventReq *)pParam)->sessionInfo;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Insert Session Information */
    return PackTlvCommonSessionInfo( pBuf, pReq );
}

/*
 * This function packs the SLQS RefreshGetLastEvent request parameters to
 * the QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaUimSLQSUimRefreshGetLastEvent.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiUimSlqsRefreshGetLastEvent(
    WORD                      *pMlength,
    BYTE                      *pParamField,
    UIMRefreshGetLastEventReq *pUIMRefreshGetLastEventReq )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SESSION_REF_GET_LAST_EVENT, &BuildTlvSessionInfoRefLastEvent },
        { eTLV_TYPE_INVALID,            NULL } /* Important. Sentinel.
                                                * Signifies last item in map
                                                */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pUIMRefreshGetLastEventReq,
                      map,
                      eQMI_UIM_REFRESH_GET_LAST_EVENT,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the unpack refresh Evenet state from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError UnpackTlvRefreshEvent( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiUimSLQSRefreshGetLastEventResp *lResp =
        ( struct QmiUimSLQSRefreshGetLastEventResp *)pResp;
    BYTE lCount, lIcount;

    UIMRefreshEvent *lResponse =
           lResp->pUIMRefreshGetLastEventResp->pRefreshEvent;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = GetByte( pTlvData, &lResponse->stage );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResponse->mode );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResponse->sessionType );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResponse->aidLength );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0; lCount < lResponse->aidLength; lCount++ )
    {
        eRCode = GetByte( pTlvData, &lResponse->aid[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    eRCode = GetWord( pTlvData, &lResponse->numOfFiles );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0; lCount < lResponse->numOfFiles; lCount++ )
    {
        fileInfo *pTemp = (fileInfo *)&lResponse->arrfileInfo[lCount];

        eRCode = GetWord( pTlvData, &pTemp->fileID);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pTemp->pathLen);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for ( lIcount = 0; lIcount < pTemp->pathLen; lIcount++ )
        {
            eRCode = GetWord( pTlvData, &pTemp->path[lIcount] );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }
    }

    return eRCode;
}

/*
 * This function unpacks the SLQS RefreshGetLastEvent response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiUimSlqsRefreshGetLastEvent(
    BYTE                                     *pMdmResp,
    struct QmiUimSLQSRefreshGetLastEventResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,   &qmUnpackTlvResultCode },
        { eTLV_REFRESH_EVENT, &UnpackTlvRefreshEvent },
        { eTLV_TYPE_INVALID,  NULL } /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_UIM_REFRESH_GET_LAST_EVENT );
    return eRCode;
}
