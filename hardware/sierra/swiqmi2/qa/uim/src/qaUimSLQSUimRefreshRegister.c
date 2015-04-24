/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimRefreshRegister.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_UIM_REFRESH_REGISTER message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaUimSLQSUimRefreshRegister.h"
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
enum eQCWWANError BuildTlvSessionInfo( BYTE *pBuf, BYTE *pParam )
{
    UIMSessionInformation *pReq = &(( UIMRefreshRegisterReq *)pParam)->sessionInfo;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Insert Session Information */
    return PackTlvCommonSessionInfo( pBuf, pReq );
}

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
enum eQCWWANError BuildTlvRegisterRefresh( BYTE *pBuf, BYTE *pParam )
{
    registerRefresh *pReq = &(( UIMRefreshRegisterReq *)pParam)->regRefresh;

    enum eQCWWANError eRCode;
    BYTE lCount,lIcount;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Add Register flag */
    eRCode = PutByte( pBuf, pReq->registerFlag );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Add Vote for Init */
    eRCode = PutByte( pBuf, pReq->voteForInit );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Add Number of files */
    eRCode = PutWord( pBuf, pReq->numFiles );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for( lCount = 0; lCount < pReq->numFiles; lCount++ )
    {
        /* Add file ID */
        eRCode = PutWord( pBuf, pReq->arrfileInfo[lCount].fileID );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        /* Add pathLen */
        eRCode = PutByte( pBuf, pReq->arrfileInfo[lCount].pathLen );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for ( lIcount = 0; lIcount < pReq->arrfileInfo[lCount].pathLen; lIcount++ )
        {
            eRCode = PutWord( pBuf, pReq->arrfileInfo[lCount].path[lIcount]);
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }
    }
    return eRCode;
}

/*
 * This function packs the SLQS RefreshRegister request parameters to the QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaUimSLQSUimRefreshRegister.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiUimSlqsRefreshRegister(
    WORD                  *pMlength,
    BYTE                  *pParamField,
    UIMRefreshRegisterReq *pUIMRefreshRegisterReq )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SESSION_INFO,     &BuildTlvSessionInfo },
        { eTLV_REGISTER_REFRESH, &BuildTlvRegisterRefresh },
        { eTLV_TYPE_INVALID,     NULL } /* Important. Sentinel.
                                         * Signifies last item in map
                                         */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pUIMRefreshRegisterReq,
                      map,
                      eQMI_UIM_REFRESH_REGISTER,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the SLQSRefreshRegister response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiUimSlqsRefreshRegister(
    BYTE                                 *pMdmResp,
    struct QmiUimSLQSRefreshRegisterResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_UIM_REFRESH_REGISTER );
    return eRCode;
}
