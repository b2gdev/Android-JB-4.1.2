/**
 *
 * @ingroup swiim
 *
 * @file
 * image select fuction of image management 
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

/* include files */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <stdbool.h>

#include "SwiDataTypes.h"
#define  LOG_TAG  "swi_imgmgr"
#include "swiril_log.h"
#include "swiim_img.h" 
#include "qmerrno.h"

extern struct ImageList devImgList;
static bool fwDwlComplete = false;
extern struct ImageFinalList devImgFinalList[50];

/**
 *
 * Call back function to notify FW download completion
 *
 * @param [in] status 
 *     Call back parameter indicate FW download completion status
 *
 * @return
 *    none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */ 
void FirmwareDwlCbk(ULONG status)
{
    if (eQCWWAN_ERR_NONE == status) {
        LOGI("Firmware Download Completed");
    } else {
        LOGE( "Firmware Not Downloaded");
    }
    /* set firmware complete to true */
    fwDwlComplete = true;

    /* Unsubscribe from the callback */
    SetFwDldCompletionCbk(NULL);
}


/**
 *
 * Register call back function
 *
 *
 * @return
 *     - true  if succeed
 *     - false if failed
 *         
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */       
bool SetFwDwldCompletionCallback()
{
    ULONG ret = 0;

    ret = SetFwDldCompletionCbk(FirmwareDwlCbk);

    if( (0 != ret) &&
        (4 != ret) )
        LOGE("SetFwDldCompletionCbk failed, ret : %lu. ", ret);

    return ((ret==0)||(ret==4));
}


/**
 *
 * Check whether the image from host exist in the module to
 *
 * @param [in] chBuildId 
 *     Image ID of downloaded image 
 * @param [in] pImageList 
 *     Device Image list to be compared 
 *
 * @return
 *     - true  if exist
 *     - false if not exist
 *         
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */       
bool IsImageExistInList(const char *chBuildId, struct sImageList *pImageList)
{
    LOGD("IsImageExistInList Entered.");
    bool fRet = false;

    LOGD("donwloaded image build id %s.", chBuildId);

    while(pImageList)
    {
        if (strcmp(chBuildId, pImageList->BuildId) == 0)
        {
            fRet = true;
            LOGD("donwloaded image found.");
            break;
        }
        pImageList = pImageList->mpNext;
    }

    LOGD("IsImageExistInList Exit.");
    return fRet;
}

/**
 *
 * Check whethere download image is succesful
 *
 * @param 
 *     none
 *
 * @return
 *     - true  if succeed
 *     - false if failed
 *         
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */       
bool CheckDownload()
{
    LOGD("CheckDownload Entered.");
    char szBuildID[MAXPATHSIZE] = "";
    bool fRet = true;
    int i = 0;
    BYTE nBuildIdLength = 0;
    ULONG nImageListSize = sizeof(struct PrefImageList);
    struct PrefImageList imageList;
    struct sImageList lstUpdatedDeviceAppImages;
    struct sImageList lstUpdatedDevicePriImages;
    struct sImageList *psImageList = NULL;

    /* Initialize the structure, so that pointers are initialized to NULL */ 
    memset(&lstUpdatedDeviceAppImages, 0, sizeof(lstUpdatedDeviceAppImages));
    memset(&lstUpdatedDevicePriImages, 0, sizeof(lstUpdatedDevicePriImages));

    /* construct image list from firmware images*/
    if(!LoadDeviceImages(&lstUpdatedDeviceAppImages, &lstUpdatedDevicePriImages))
    {
        LOGE("LoadDeviceImages Failed.");
        return false;
    }
    
    /* Get the image ID just downloaded*/
    ULONG result = GetImagesPreference(&nImageListSize, (BYTE *)&imageList);

    if (!result)
    {
        BYTE nListSize = imageList.listSize;

        // cycle through image list elements
        for (i=0; i< nListSize; i++)
        {
            BYTE ImageType = imageList.listEntries[i].imageType;

            nBuildIdLength = imageList.listEntries[i].buildIdLength;
            memset(szBuildID, 0, sizeof(szBuildID));
            strncpy( (char *)szBuildID,
                     (char *)imageList.listEntries[i].buildId,
                     nBuildIdLength);
            if (ImageType == eGOBI_MBN_TYPE_MODEM)
            {
                psImageList = &lstUpdatedDeviceAppImages;;
            }
            else if (ImageType == eGOBI_MBN_TYPE_PRI)
            {
                psImageList = &lstUpdatedDevicePriImages;
            }
            else
            {
                LOGE("Unsupported image type: %d.", ImageType);
                return false;
            }

            fRet = IsImageExistInList(szBuildID, psImageList);
            if (!fRet)
            {
                LOGE("Downloaded image did not exist in image list.");
                break;
            }
        }
    }
    else
    {
        LOGE("GetImagesPreference failed: %lu.", result);
    }

    /* deconstruct image list */
    ReleaseDeviceImages(&lstUpdatedDeviceAppImages, &lstUpdatedDevicePriImages);
    LOGD("CheckDownload Exit.");
    return fRet;
}

/**
 *
 * Image management with "--select" option
 *
 * @param [in] iSelectID 
 *     ID selected for image path
 * @param [in] chListPath 
 *     qualified path to image file
 *
 * @return
 *     true if function succeed
 *     FASLE if funciton failed
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */       
bool SelectImage(int iSelectID, char *chSelectPath)
{
    LOGD("SelectImage Entered.");
    char newpathp[MAXPATHSIZE] = "";
    char chIdPath[MAXPATHSIZE] = "";
    bool ret = false;
    int folderLen;

    /*1 Set callback function for firmware download completion */
    if (!SetFwDwldCompletionCallback())
    {
        LOGE("SetFwDwldCompletionCallback failed.");
        ret = false;
        goto EXIT;
    }
    
    /*3 Upgrade Firmware */

    /* check for AMSS file */
    strcpy(newpathp, chSelectPath);
    folderLen = strlen(newpathp);
    if (newpathp[folderLen - 1] != '/')
    {
        strcat(newpathp, "/");
    }
    snprintf(chIdPath, MAXPATHSIZE, "%d/", iSelectID);
    strcat(newpathp, chIdPath);

    swi_uint32 result = 0;
    result = UpgradeFirmware2k( newpathp );

    snprintf(newpathp, sizeof(newpathp), "%s/%d", chSelectPath, iSelectID);
    LOGD( "select folder : %s", newpathp);

    if(!result)
    {
        LOGD("UpgradeFirmware for %s succeed: %lu.", newpathp, result);
        ret = true;
    }
    else
    {
        LOGE("UpgradeFirmware for %s failed: %lu.", newpathp, result);
        ret = false;
        goto EXIT;
    }

    /*4 Fimware Download call back */
    LOGD( "Activating Selected Image. . ." );
    fwDwlComplete = false;
    while( !fwDwlComplete )
    {
        sleep(1);
    }

    sleep(5); /* wait for download complete*/
    

    /*4 Check whether downloading is succeful*/
    if (!CheckDownload())
    {
        LOGE("CheckDownload Failed, Download failed .");
        ret = false;
        goto EXIT;
    }
    LOGD("Firmware download for %s succeed.", newpathp);
    ret = true;

EXIT:
    LOGD("SelectImage Exit.");
    return ret;
}

/**
 * Calculate the Image index corresponding the actual PRI Image list
 *
 * @param [out] imageIndex 
 *     Index of the Image in the device to be selected
 *
 * @return
 *     true if function succeed
 *     false if function failed
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
bool ComputeImageIndex ( BYTE *imageIdx )
{
    struct ImageIDEntries *pPriImgEntry;
    BYTE idx = 0, priImageIdx = 0;

    /* Figure out the index of PRI image type in the received image list */
    /* If no PRI image exist on the device */
    if( !GetPRIImageIdx( &priImageIdx ) ) {
        LOGE("FAIL SAFE Image Active!" );
    }

    /* Store the pointer to PRI image list */
    pPriImgEntry   = &devImgList.imageIDEntries[priImageIdx];
    if (*imageIdx > pPriImgEntry->imageIDSize) {
        LOGE( "Invalid Image Location selected");
        return false;
    }

    /* Compute the PRI Image Index with respect to the ID which is selected */
    for( idx = 0; idx < pPriImgEntry->imageIDSize; idx++ ) {
        if( devImgFinalList[idx].IsLatest ) {
            /* Condition to check the valid Image Index */
            if( !(--(*imageIdx)) ) {
                *imageIdx = idx;
                return true;
            }
        }
    }
    return false;
}

/**
 * Activate the image on the device selected by the user.
 *
 * @param [in] imageIndex 
 *     Index of the Image in the device to be selected
 *
 * @return
 *     true if function succeed
 *     false if function failed
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
bool SelectDeviceImage( BYTE imageIndex )
{
    struct PrefImageList prefImageList;
    struct ImageIdElement *pActPRIImg = NULL;
    struct ImageIDEntries *pActModemImg = NULL;
    ULONG resultCode = 0, imageListSize = 0, imageTypeSize = 0;
    int modemImageFound = false;
    BYTE modemImdIdx = 0, priImageIdx = 0;
    BYTE imageType[IMG_BUF_SIZE];

    /* Populate the device Image structure without displaying */
    if (!ListDeviceImages (false))
    {
        return false;
    }

    while(1)
    {
        /* Calculate the Image index corresponding the actual PRI Image list */
        if (!ComputeImageIndex( &imageIndex )) {
            return false;
        }

        /* Get the PRI image index from the Image list */
        if( !GetPRIImageIdx( &priImageIdx ) ) {
            return false;
        }

        /* Store the pointer to PRI image list */
        pActPRIImg = &( devImgList.imageIDEntries[priImageIdx].\
                          imageIDElement[imageIndex] );

        /* Fill the PrefImageList structure with PRI image info */
        prefImageList.listSize = 1;
        prefImageList.listEntries[0].imageType = IMG_TYPE_PRI;
        memcpy( (void *)prefImageList.listEntries[0].imageId,
                (void *)pActPRIImg->imageID,
                GOBI_MBN_IMG_ID_STR_LEN );
        prefImageList.listEntries[0].buildIdLength = pActPRIImg->buildIDLength;
        if( 0 != pActPRIImg->buildIDLength )
        {
            strcpy( prefImageList.listEntries[0].buildId,
                    pActPRIImg->buildID );
        }

        /* Store the pointer to Modem image list */
        pActModemImg = &( devImgList.imageIDEntries[!priImageIdx] );

        /* Check if the corresponding MODEM image exist on device.
         * If the CDMA image is selected by the user then Modem image with same
         * build id should exist
         */
        for(modemImdIdx = 0;
            modemImdIdx < pActModemImg->imageIDSize;
            modemImdIdx++)
        {
            CHAR *pbuildID = pActModemImg->imageIDElement[modemImdIdx].buildID;
            ULONG buildIDlen =
                     pActModemImg->imageIDElement[modemImdIdx].buildIDLength;

            /* If the corresponding MODEM image is found */
            if (CheckModemImage(imageIndex,
                                modemImdIdx,
                                pbuildID,
                                buildIDlen)) {
                modemImageFound = true;
                break;
            }
        }

        /* If corresponding MODEM image does not exist on the device, return */
        if( !modemImageFound )
        {
            LOGE( "Failed to activate selected image"
                  ": MODEM image does not exist on the device");
            return false;
        }

        /* Reset the flag to false */
        modemImageFound = false;

        /* MODEM image exist, retrieve the information */
        prefImageList.listSize++;
        prefImageList.listEntries[1].imageType = IMG_TYPE_MODEM;
        memcpy( (void *)prefImageList.listEntries[1].imageId,
                (void *)pActModemImg->imageIDElement[modemImdIdx].imageID,
                GOBI_MBN_IMG_ID_STR_LEN );
        prefImageList.listEntries[1].buildIdLength =
             pActModemImg->imageIDElement[modemImdIdx].buildIDLength;

        if( 0 != pActModemImg->imageIDElement[modemImdIdx].buildIDLength )
        {
            strcpy( prefImageList.listEntries[1].buildId,
                    pActModemImg->imageIDElement[modemImdIdx].buildID );
        }

        imageListSize = sizeof( prefImageList );
        imageTypeSize = IMG_BUF_SIZE;
        memset( (void *)imageType, 0, imageTypeSize );

        /** 
         * Set the Image Path - Not required
         * Workaround for the issue in SLQS as the FW Download callback would
         * not be invoked without setting the image path.
         * Set to some path which does not exist
         */
        resultCode = SetImagePath( "Generic" );

        if( resultCode )
        {
            LOGE( "Failed to set Image path"
                             "Failure Code : %lu", resultCode );
            continue;
        }

        /* Set the preference for selected image in device */
        resultCode = SetImagesPreference( imageListSize,
                                          (BYTE *)&prefImageList,
                                          0,
                                          0xFF,
                                          &imageTypeSize,
                                          imageType );

        if( resultCode )
        {
            LOGE( "Failed to activate selected image"
                             "Failure Code : %lu", resultCode );
            continue;
        }

        /* Subscribe to Device State Change callback */
        if (!SetFwDwldCompletionCallback())
        {
            LOGE("SetFwDwldCompletionCallback failed.");
            return false;
        }

        /* Reset the device to activate the image */
        resultCode = SetPower( RESET_MODEM );
        if( resultCode )
        {
            LOGE( "Failed to reset: Failure Code : %lu", resultCode );
            continue;
        }

        /* Wait until selected image gets activated */
        LOGD( "Activating Selected Image. . ." );
        fwDwlComplete = false;
        while( !fwDwlComplete )
        {
            sleep(1);
        }
        sleep(3);
   
         /* If we fail to list the device images, return */
        if( !ListDeviceImages(false) )
        {
            return false;
        }

        if( (UNKNOWN_EXECUTING_IMG_IDX == 
                devImgList.imageIDEntries[priImageIdx].executingImage) ||
            (FAIL_SAFE_IMG_IDX == 
                devImgList.imageIDEntries[!priImageIdx].executingImage) )
        {
            LOGE("FAIL SAFE Image Active!!");
            return false;
        }

        imageIndex = ( devImgList.imageIDEntries[priImageIdx].executingImage );
        pActPRIImg = &( devImgList.imageIDEntries[priImageIdx].\
                          imageIDElement[imageIndex] );


        /* Check if the selected image gets activated */
        if( !strcmp( pActPRIImg->buildID,
                               prefImageList.listEntries[0].buildId ) )
        {
            LOGD("Selected Image activated successfully");
            return true;
        }
        else
        {
            LOGE("Failed to activate selected image");
            return false;
        }
    }
    return false;
}
