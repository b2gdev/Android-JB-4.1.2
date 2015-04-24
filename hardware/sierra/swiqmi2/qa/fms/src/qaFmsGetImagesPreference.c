/*
 * \ingroup fms
 *
 * \file    qaFmsGetImagesPreference.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_DMS_GET_FIRMWARE_PREFERENCE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiFms.h"
#include "qaFmsGetImagesPreference.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * Packs the GetImagesPreference parameters to the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiFmsGetImagesPreference(
    WORD *pMlength,
    BYTE *pParamField )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_DMS_GET_FIRMWARE_PREF,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * Unpacks the GetImagesPreference Image Preference List from the QMI
 * response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvImagePrefList( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiFmsGetImagesPreferenceResp *lResp =
        (struct QmiFmsGetImagesPreferenceResp *)pResp;
    BYTE lTemp = 0 , listLen = 0, idx = 0, buildIdLength = 0, index = 0;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pImageListSize ||
         !lResp->pImageList )
         return eQCWWAN_ERR_QMI_NO_MEMORY;
    /* Extract the TLV length */
    lTemp = qmQmiGetResponseTLVlength((struct qmTBuffer *)pTlvData);

    /* Check if memory allocated to out buffer is sufficient for response */
    if ( (lTemp > 0) && (*(lResp->pImageListSize) < lTemp) )
        return eQCWWAN_ERR_BUFFER_SZ;

    *(lResp->pImageListSize) = lTemp;
    eRCode = GetByte( pTlvData, &listLen );

    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        lResp->pImageList->listSize = listLen;
        while ( listLen-- )
        {
            buildIdLength = 0;
            /* Extract the ImageList Elements*/
            eRCode = GetByte( pTlvData, &lResp->pImageList->
                                        listEntries[idx].imageType );
            if ( eQCWWAN_ERR_NONE == eRCode )
            {
                for ( index = 0; index < GOBI_MBN_IMG_ID_STR_LEN; index++ )
                {
                    eRCode = GetByte( pTlvData, &lResp->
                                      pImageList->listEntries[idx].
                                      imageId[index] );
                    if( eQCWWAN_ERR_NONE != eRCode )
                        return eRCode;
                }
            }
            if ( eQCWWAN_ERR_NONE == eRCode )
                eRCode = GetByte( pTlvData, &lResp->pImageList->
                                   listEntries[idx].buildIdLength );
            if ( eQCWWAN_ERR_NONE == eRCode )
            {
                buildIdLength = lResp->pImageList->listEntries[idx].
                                                               buildIdLength;
                for ( index = 0; index < buildIdLength; index++ )
                {
                    eRCode = GetByte( pTlvData, &lTemp);
                    if ( eQCWWAN_ERR_NONE != eRCode )
                        return eRCode;
                    lResp->pImageList->listEntries[idx].buildId[index] =
                                                                 (CHAR)lTemp;
                }
            }
            idx++;
        }
    }
    return eRCode;
}

/*
 * Unpacks the GetImagesPreference response message to a user-provided response
 * structure.
 *
 * \param  pMdmResp     [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp     [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiFmsGetImagesPreference(
    BYTE                                 *pMdmResp,
    struct QmiFmsGetImagesPreferenceResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,          &qmUnpackTlvResultCode },
        { eTLV_GET_IMAGES_PREF_LIST, &UnpackTlvImagePrefList },
        { eTLV_TYPE_INVALID,         NULL } /* Important. Sentinel.
                                             * Signifies last item in map.
                                             */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_GET_FIRMWARE_PREF );
    return eRCode;
}
