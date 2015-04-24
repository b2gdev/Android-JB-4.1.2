/*
 * \ingroup wds
 *
 * \file qaWdsSlqsDeleteProfile.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_WDS_DELETE_PROFILE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "aa/aaglobal.h"
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "amudefs.h"

#include "qaQmiBasic.h"
#include "qaWdsSLQSDeleteProfile.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SLQSDeleteProfile profileType field to the QMI
 * message SDU.
 *
 * \param  pBuf   - Pointer to storage into which the packed data will be
 *                  placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
local enum eQCWWANError BuildTlvDelProfileIdentifier(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsSlqsDeleteProfileReq *pReq =
                        (struct QmiWdsSlqsDeleteProfileReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Add Profile Type */
    eRCode = PutByte( pBuf, pReq->profileType );

    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        /* Add Profile Index */
        eRCode = PutByte( pBuf, pReq->profileIndex );
    }
    return eRCode;
}

/*
 * This function packs the SLQSDeleteProfile parameters to the QMI
 * message SDU.
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \sa     qaGobiApiWds.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiWdsSlqsDeleteProfile(
    WORD *pMlength,
    BYTE *pParamField,
    BYTE profileType,
    BYTE profileIndex )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_DELETE_PROFILE_IDENTIFIER, &BuildTlvDelProfileIdentifier },
        { eTLV_TYPE_INVALID,              NULL } /* Important. Sentinel.
                                                  * Signifies last item in map.
                                                  */
    };

    enum eQCWWANError eRCode;
    struct QmiWdsSlqsDeleteProfileReq req;
    slmemset( (char *)&req, 0, sizeof( struct QmiWdsSlqsDeleteProfileReq ) );
    req.profileType  = profileType;
    req.profileIndex = profileIndex;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_WDS_DELETE_PROFILE,
                      pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SLQSDeleteProfile extended error code from the
 * QMI response message to a user provided response structure.
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 */
local enum eQCWWANError UnpackTlvDelExtErrorCode( BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSlqsDeleteProfileResp *lResp =
                        (struct QmiWdsSlqsDeleteProfileResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* If lResp->pExtendedErrorCode is NULL, return */
    if( NULL == lResp->pExtendedErrorCode )
    {
        return eRCode;
    }

    return GetWord( pTlvData, lResp->pExtendedErrorCode );
}

/*
 * This function unpacks the SLQSDeleteProfile response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp     [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp     [OUT] - Pointer to storage to unpack into.
 *
  * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
enum eQCWWANError UpkQmiWdsSlqsDeleteProfile(
    BYTE                               *pMdmResp,
    struct QmiWdsSlqsDeleteProfileResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,                   &qmUnpackTlvResultCode },
        { eTLV_DELETE_PROFILE_EXTENDED_ERROR, &UnpackTlvDelExtErrorCode },
        { eTLV_TYPE_INVALID,                  NULL } /* Important. Sentinel.
                                                      * Signifies last item in map.
                                                      */
    };

    eRCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_WDS_DELETE_PROFILE );
    return eRCode;
}
