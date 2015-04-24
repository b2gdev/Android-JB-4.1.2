/*
 * \ingroup fms
 *
 * \file    qaFmsSetImagesPreference.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_DMS_SET_FIRMWARE_PREF message.
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
#include "qaFmsSetImagesPreference.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * Packs the SetImagesPreference Image Preference List field to the QMI
 * message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvImagePrefList( BYTE *pBuf, BYTE *pParam )
{
    struct QmiFmsSetImagesPreferenceReq *pReq =
        (struct QmiFmsSetImagesPreferenceReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE  lTemp, imageListSize = 0, index = 0, buildIdLength = 0, idx = 0;

    /* Add TLV data */
    imageListSize = pReq->pImageList->listSize;

    eRCode = PutByte( pBuf, imageListSize);
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        while ( imageListSize-- )
        {
            lTemp  = pReq->pImageList->listEntries[idx].imageType;
            eRCode = PutByte( pBuf, lTemp );
            if ( eQCWWAN_ERR_NONE == eRCode )
            {
                for ( index = 0; index < MBN_IMG_ID_STR_LEN; index++ )
                {
                    lTemp  = pReq->pImageList->listEntries[idx].imageId[index];
                    eRCode = PutByte( pBuf, lTemp );
                    if ( eQCWWAN_ERR_NONE != eRCode )
                        return eRCode;
                }
            }
            if ( eQCWWAN_ERR_NONE == eRCode )
            {
                buildIdLength  = pReq->pImageList->listEntries[idx].buildIdLength;
                eRCode = PutByte( pBuf, buildIdLength );
            }
            if ( eQCWWAN_ERR_NONE == eRCode )
            {
                for ( index = 0; index < buildIdLength ; index++ )
                {
                    lTemp  = pReq->pImageList->listEntries[idx].buildId[index];
                    eRCode = PutByte( pBuf, lTemp );
                    if( eQCWWAN_ERR_NONE != eRCode )
                        return eRCode;
                }
            }
            idx++;
        }
    }
    return eRCode;
}

/*
 * Packs the SetImagesPreference Download Override field to the QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvDownloadOverride( BYTE *pBuf, BYTE *pParam )
{
    struct QmiFmsSetImagesPreferenceReq *pReq =
        (struct QmiFmsSetImagesPreferenceReq *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->bForceDownload );
}

/*
 * Packs the SetImagesPreference Download Modem Storage Index
 * field to the QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvMdmStorageIndex( BYTE *pBuf, BYTE *pParam )
{
    struct QmiFmsSetImagesPreferenceReq *pReq =
        (struct QmiFmsSetImagesPreferenceReq *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->modemIndex );
}

/*
 * Packs the SetImagesPreference parameters to the QMI message SDU
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
enum eQCWWANError PkQmiFmsSetImagesPreference(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG imageListSize,
    BYTE  *pImageList,
    ULONG bForceDownload,
    BYTE  modemIndex )
{
    static struct qmTlvBuilderItem map_no_mdmidx[] =
    {
        { eTLV_SET_IMAGES_PREF_LIST,  &BuildTlvImagePrefList },
        { eTLV_SET_DOWNLOAD_OVERRIDE, &BuildTlvDownloadOverride },
        { eTLV_TYPE_INVALID,          NULL } /* Important. Sentinel.
                                              * Signifies last item in map
                                              */
    };

    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_IMAGES_PREF_LIST,  &BuildTlvImagePrefList },
        { eTLV_SET_DOWNLOAD_OVERRIDE, &BuildTlvDownloadOverride },
        { eTLV_SET_MDM_STORAGE_INDEX, &BuildTlvMdmStorageIndex },
        { eTLV_TYPE_INVALID,          NULL } /* Important. Sentinel.
                                              * Signifies last item in map
                                              */
    };

    struct QmiFmsSetImagesPreferenceReq req;
    slmemset((CHAR *)&req, 0, sizeof(struct QmiFmsSetImagesPreferenceReq));
    req.imageListSize  = imageListSize;
    req.pImageList     = (struct PrefImageList *)pImageList;
    req.bForceDownload = bForceDownload;
    req.modemIndex     = modemIndex;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    if( modemIndex == 0xFF )
    {
        eRCode = qmbuild( pBuffer,
                          (BYTE *)&req,
                          map_no_mdmidx,
                          eQMI_DMS_SET_FIRMWARE_PREF,
                          pMlength );
    }
    else
    {
        eRCode = qmbuild( pBuffer,
                          (BYTE *)&req,
                          map,
                          eQMI_DMS_SET_FIRMWARE_PREF,
                          pMlength );
    }

    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * Unpacks the GetImagesPreference Image Download List from the QMI response
 * message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvImageDwnldList( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiFmsSetImagesPreferenceResp *lResp =
        (struct QmiFmsSetImagesPreferenceResp *)pResp;
    BYTE listLen = 0, idx = 0;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pImageTypesSize ||
         !lResp->pImageTypes )
         return eRCode;

    eRCode = GetByte( pTlvData, &listLen );
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        // check for buffer undersize error
        if ( *(lResp->pImageTypesSize) < listLen )
            return eQCWWAN_ERR_BUFFER_SZ;

        *(lResp->pImageTypesSize) = listLen;
        while ( listLen )
        {
            /* Extract the ImageList Elements*/
            eRCode = GetByte( pTlvData, &lResp->pImageTypes[idx] );
            if ( eQCWWAN_ERR_NONE != eRCode )
                return eRCode;

            listLen--;
            idx++;
        }
    }
    return eRCode;
}
/*
 * Unpacks the SetImagesPreference response message to a user-provided response
 * structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiFmsSetImagesPreference(
    BYTE                                 *pMdmResp,
    struct QmiFmsSetImagesPreferenceResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,          &qmUnpackTlvResultCode },
        { eTLV_SET_IMAGE_DWNLD_LIST, &UnpackTlvImageDwnldList },
        { eTLV_TYPE_INVALID,         NULL } /* Important. Sentinel.
                                             * Signifies last item in map.
                                             */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_SET_FIRMWARE_PREF );
    return eRCode;
}
