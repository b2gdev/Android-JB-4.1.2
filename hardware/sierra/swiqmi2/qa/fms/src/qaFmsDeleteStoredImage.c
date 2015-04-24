/*
 * \ingroup fms
 *
 * \file    qaFmsDeleteStoredImage.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_DMS_DELETE_STORED_IMAGE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiFms.h"
#include "qaFmsDeleteStoredImage.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * Packs the DeleteStoredImage Image Info list field to the QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvImageInfoList( BYTE *pBuf, BYTE *pParam )
{
    struct QmiFmsDeleteStoredImageReq *pReq =
        (struct QmiFmsDeleteStoredImageReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE idx = 0;

    /* Pack Image Type */
    eRCode = PutByte( pBuf, pReq->pImageInfo->imageType );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Pack Image ID */
    for ( idx = 0; idx < MBN_IMG_ID_STR_LEN; idx++ )
    {
        eRCode = PutByte( pBuf, pReq->pImageInfo->imageId[idx] );
        if ( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
    }

    /* Pack Build ID length */
    eRCode = PutByte( pBuf, pReq->pImageInfo->buildIdLength );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Return if no Build ID is provided */
    if ( 0 >= pReq->pImageInfo->buildIdLength )
    {
        return eRCode;
    }
    else if( MBN_BUILD_ID_STR_LEN < pReq->pImageInfo->buildIdLength )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    for ( idx = 0; idx < pReq->pImageInfo->buildIdLength; idx++ )
    {
        eRCode = PutByte( pBuf, pReq->pImageInfo->buildId[idx] );
        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
    }

    return eRCode;
}

/*
 * Packs the DeleteStoredImage parameters to the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaGobiApifms.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiFmsDeleteStoredImage(
    WORD  *pMlength,
    BYTE  *pBuffer,
    BYTE  *pImageInfo )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_STORED_IMAGE_INFO,  &BuildTlvImageInfoList },
        { eTLV_TYPE_INVALID,       NULL } /* Important. Sentinel.
                                           * Signifies last item in map
                                           */
    };

    struct QmiFmsDeleteStoredImageReq req;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiFmsDeleteStoredImageReq));
    req.pImageInfo     = (struct ImageElement *)pImageInfo;

    eRCode = qmbuild( pBuffer,
                      (BYTE *)&req,
                      map,
                      eQMI_DMS_DELETE_FIRMWARE,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the DeleteStoredImage response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaGobiApifms.h for remaining parameter descriptions.
 */
enum eQCWWANError UpkQmiFmsDeleteStoredImage(
    BYTE                               *pMdmResp,
    struct QmiFmsDeleteStoredImageResp *pApiResp )
{
    enum eQCWWANError eRCode;

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
                           eQMI_DMS_DELETE_FIRMWARE );
    return eRCode;
}
