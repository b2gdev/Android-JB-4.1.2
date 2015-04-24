/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimRefreshOK.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_UIM_REFRESH_OK message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaUimSLQSUimRefreshOK.h"
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
enum eQCWWANError BuildTlvSessionInfoRefOK( BYTE *pBuf, BYTE *pParam )
{
    UIMSessionInformation *pReq = &(( UIMRefreshOKReq *)pParam)->sessionInfo;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Insert Session Information */
    return PackTlvCommonSessionInfo( pBuf, pReq );
}

/*
 * This function packs the SetPortAutomaticTracking bAuto field to the
 * QMI message SDU
 *
 * \param  pBuf         [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param  pParam       [IN]  - Pointer to structure containing data
 *                              for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvOKForRefresh( BYTE *pBuf, BYTE *pParam )
{
     UIMRefreshOKReq *pReq = (UIMRefreshOKReq *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->OKtoRefresh );
}

/*
 * This function packs the SLQS RefreshOK request parameters to the QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaUimSLQSUimRefreshOK.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiUimSlqsRefreshOK(
    WORD            *pMlength,
    BYTE            *pParamField,
    UIMRefreshOKReq *pUIMRefreshOKReq )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SESSION_INFO_REFRESH_OK, &BuildTlvSessionInfoRefOK },
        { eTLV_OK_FOR_REFRESH,          &BuildTlvOKForRefresh },
        { eTLV_TYPE_INVALID,            NULL } /* Important. Sentinel.
                                                * Signifies last item in map
                                                */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pUIMRefreshOKReq,
                      map,
                      eQMI_UIM_REFRESH_OK,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the SLQS RefreshOK response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiUimSlqsRefreshOK(
    BYTE                           *pMdmResp,
    struct QmiUimSLQSRefreshOKResp *pApiResp )
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
                           eQMI_UIM_REFRESH_OK );
    return eRCode;
}
