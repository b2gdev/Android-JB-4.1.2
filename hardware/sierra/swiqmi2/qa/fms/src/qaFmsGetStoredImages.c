/*
 * \ingroup fms
 *
 * \file    qaFmsGetStoredImages.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_DMS_LIST_STORED_IMAGES message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"
#include "qaFmsGetStoredImages.h"

#define MAX_BUILD_ID_LEN 16

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the Get Stored Image Parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError PkQmiFmsGetStoredImages(
    WORD *pMlength,
    BYTE *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        /* There is no mandatory tlv hence map is empty */
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel
                                     * Signifies last item in map
                                     */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_DMS_LIST_FIRMWARE,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the Stored Image List from the
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
enum eQCWWANError UnpackTlvStoredImageList( BYTE *pTlvData, BYTE *pResp )
{
    struct ImageIDEntries *pImageIDEntries = NULL;
    struct ImageIdElement *pImageIDElement = NULL;
    enum   eQCWWANError   eRCode           = eQCWWAN_ERR_NONE;
    ULONG                 lImageListSize   = 0;
    WORD                  lQmiRspLen       = 0;
    BYTE                  lListIdx         = 0, lImageIDElements = 0, idx = 0;

    struct QmiFmsGetStoredImagesResp *lResp =
                       ( struct QmiFmsGetStoredImagesResp *)pResp;

    /* Extract the length of the recieved QMI response */
    GetStringLen( pTlvData, &lQmiRspLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* If QMI response size is greater than user provide buffer,return error */
    if( lQmiRspLen > *( lResp->pImagelistSize ) )
        return eQCWWAN_ERR_BUFFER_SZ;

    /* Extract list size */
    eRCode  = GetByte( pTlvData, &lResp->pImageList->listSize );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /*
     * Increment the no. of bytes copied, each time we copy data into user array
     */
    lImageListSize++;

    /* Fill the list_entry*/
    while ( lListIdx < lResp->pImageList->listSize )
    {
        pImageIDEntries = ( struct ImageIDEntries *)&lResp->pImageList->
                                                    imageIDEntries[lListIdx];
        /* Extract Image type */
        eRCode = GetByte( pTlvData, &pImageIDEntries->imageType );
        if ( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
        lImageListSize++;

        /* Extract Max Images */
        eRCode = GetByte( pTlvData, &pImageIDEntries->maxImages );
        if ( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
        lImageListSize++;

        /* Extract Executing image */
        eRCode = GetByte( pTlvData, &pImageIDEntries->executingImage );
        if ( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
        lImageListSize++;

        /* Extract Image ID size */
        eRCode = GetByte( pTlvData, &pImageIDEntries->imageIDSize );
        if ( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
        lImageListSize++;
        lImageIDElements = 0;

        /* Extract Image id list elements*/
        while( lImageIDElements < pImageIDEntries->imageIDSize )
        {
            pImageIDElement = ( struct ImageIdElement * )&(pImageIDEntries->
                              imageIDElement[lImageIDElements]);

            /* Extract Storage index */
            eRCode = GetByte( pTlvData, &pImageIDElement->storageIndex );
            if ( eQCWWAN_ERR_NONE != eRCode )
                return eRCode;
            lImageListSize++;

            /* Extract Failure count */
            eRCode = GetByte( pTlvData, &pImageIDElement->failureCount );
            if ( eQCWWAN_ERR_NONE != eRCode )
                return eRCode;
            lImageListSize++;

            /* Extract Unique ID */
            for( idx = 0; idx < MAX_BUILD_ID_LEN; idx++ )
            {
                eRCode = GetByte( pTlvData, &pImageIDElement->imageID[idx] );
                if ( eQCWWAN_ERR_NONE != eRCode )
                    return eRCode;
                lImageListSize++;
            }

            /* Extract Build ID Length */
            eRCode = GetByte( pTlvData, &pImageIDElement->buildIDLength );
            if ( eQCWWAN_ERR_NONE != eRCode )
                return eRCode;
            lImageListSize++;

            /* Extract Build ID String only if length is non-zero */
            if( 0 != pImageIDElement->buildIDLength )
            {
                 for( idx = 0; idx < pImageIDElement->buildIDLength; idx++ )
                 {
                      eRCode = GetByte( pTlvData,
                                     (BYTE *)&pImageIDElement->buildID[idx]);
                      if ( eQCWWAN_ERR_NONE != eRCode )
                          return eRCode;
                 }
                 lImageListSize += slstrlen( pImageIDElement->buildID );
            }
            lImageIDElements++;
        }
        lListIdx++;
    }
    /* Update the image list size to be returned to the user */
    *( lResp->pImagelistSize ) = lImageListSize;
    return eRCode;
}

/*
 * This function unpacks the QMI_DMS_LIST_STORED_IMAGES response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError UpkQmiFmsGetStoredImages(
    BYTE                             *pMdmResp,
    struct QmiFmsGetStoredImagesResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,       &qmUnpackTlvResultCode },
        { eTLV_STORED_IMAGE_LIST, &UnpackTlvStoredImageList},
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map.
                                          */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_LIST_FIRMWARE );
    return eRCode;
}

