/**
 *
 * @ingroup swiim
 *
 * @file
 * image update fuction of image management 
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
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <stdbool.h>

#include "SwiDataTypes.h"
#define  LOG_TAG  "swi_imgmgr"
#include "swiril_log.h"
#include "swiim_img.h" 

struct sImageList lstDeviceAppImages;
struct sImageList lstDevicePriImages;

/* Update Active Image Parameters */
swi_bool updateActiveImage = FALSE;
int  selectID = 0;

/**
 *
 * Search a data buffer for the first occurence of a specified
 * sequence of data (starting from the end of the buffer)
 *
 * @param [in] pBuffer 
 *     Buffer being search
 * @param [in] bufferLen 
 *     Size of above buffer
 * @param [in] pData 
 *     Data to be searched for
 * @param [in] dataLen 
 *     Size of data to be searched for
 *
 * @return
 *     BYTE * - pointer of seartch result
 *    
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */       
const BYTE * ReverseBinaryDataSearch( 
   const BYTE *                pBuffer,
   ULONG                       bufferLen,
   const BYTE *                pData, 
   ULONG                       dataLen ) 
{ 
   /* Handle empty cases */
   if (pBuffer == 0 || bufferLen == 0 || pData ==0 || dataLen == 0)
   {
      return 0;
   }

   /* Handle impossible case */
   if (dataLen > bufferLen)
   {
      return 0;
   }

   const BYTE * pTmp = pBuffer + (bufferLen - dataLen);
   while (pTmp > pBuffer)
   {
      int res = memcmp( (const void *)pTmp, 
                          (const void *)pData, 
                          (size_t)dataLen );

      if (res == 0)
      {
         return pTmp;
      }

      pTmp--;
   }

   return 0;
}

/**
 *
 * Get the image build version information for the image specified by the given 
 * ID and qualified path
 *
 * @param [in] iSelectID 
 *      Image ID of image path
 * @param [in] pFilePath 
 *     qualified path to image file
 * @param [in] versionSize 
 *     The maximum number of characters including the NULL terminator 
 * @param [out] pVersionApp 
 *     NULL-terminated string representing the AMSS version
 * @param [out] pVersionPri 
 *     NULL-terminated string representing the UQCN version
 *
 * @return
 *     TRUE if function succeed
 *     FASLE if funciton failed
 *    
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */       
swi_bool GetHostImageVersion( 
    int                        iSelectID,
    const char *               pFilePath, 
    USHORT                     versionSize,
    char *                     pVersionApp,
    char *                     pVersionPri )
{
    LOGD("GetHostImageVersion Enter. \n");
  
    ULONG    build = MBN_BUILD_MAGIC;
    //glob_t   files;
    BYTE     read[256] = {0};
    ULONG    t = 0;
    char     chNewPath[MAXPATHSIZE] = "";
    char     chFilePath[MAXPATHSIZE] = "";
    char     chIdPath[MAXPATHSIZE] = "";
    int      ret = 0;
    FILE     *pAppHandle = NULL;
    FILE     *pPriHandle = NULL;
    swi_bool result = TRUE;
    
    if (!pFilePath || !pVersionApp || !pVersionPri ||(versionSize == 0))
    {
        LOGE("Invalid argument. \n");
        result = FALSE;
        goto EXIT;
    }

    strcpy(chNewPath, pFilePath);
    int folderLen = strlen(chNewPath);
    if (chNewPath[folderLen - 1] != '/')
    {
        strcat(chNewPath, "/");
    }
    snprintf(chIdPath, MAXPATHSIZE, "%d/", iSelectID);
    strcat(chNewPath, chIdPath);
#if 0
    strcat(chNewPath, "amss*.mbn");
    ret = glob(chNewPath, 0, NULL, &files );
    if (ret != 0)
    {
        /* Glob error file not found, use folder 6 for UMTS App image */
        strcpy(chNewPath, pFilePath);
        int folderLen = strlen(chNewPath);
        if (chNewPath[folderLen - 1] != '/')
        {
            strcat(chNewPath, "/");
        }
        snprintf(chIdPath, MAXPATHSIZE, "%d/", 6);
        strcat(chNewPath, chIdPath);
        strcat(chNewPath, "amss*.mbn");
        ret = glob(chNewPath, 0, NULL, &files );
        if (ret != 0)
        {
            LOGE("Unable to find app file %s. \n", chNewPath);
            result = FALSE;
            goto EXIT;
        }
    }
    /* open file */
    pAppHandle = fopen(files.gl_pathv[0], "r");

#endif
    strcat(chNewPath, "amss.mbn");
    pAppHandle = fopen(chNewPath, "r");
    if(pAppHandle == NULL)
    {
        /* file not found, use folder 6 for UMTS App image */
        strcpy(chNewPath, pFilePath);
        int folderLen = strlen(chNewPath);
        if (chNewPath[folderLen - 1] != '/')
        {
            strcat(chNewPath, "/");
        }
        snprintf(chIdPath, MAXPATHSIZE, "%d/", 6);
        strcat(chNewPath, chIdPath);
        strcat(chNewPath, "amss.mbn");
    }

    /* open file */
    pAppHandle = fopen(chNewPath, "r");
    if(pAppHandle == NULL)
    {
        LOGE("Unable to open file %s. \n",chNewPath);
        result = FALSE;
        goto EXIT;
    }

    /* move file pointer to the last 256 bytes of the file */
    ret = fseek(pAppHandle, -256, SEEK_END);
    if(ret != 0)
    {
        LOGE("Failed to seek image file\n");
        result = FALSE;
        goto EXIT;
    }

    /* read last 256 bytes of the file */
    if (!fread(read, 256, 1, pAppHandle))
    {
        LOGE("Failed to read image file \n");
        goto EXIT;
    }

    /* Search for the build ID */
    BYTE * pTmp = 0;

    pTmp = (BYTE *)ReverseBinaryDataSearch((const BYTE *)read,
                                   256,
                                   (const BYTE *)&build,
                                   (ULONG)sizeof(ULONG));
    if (pTmp == 0)
    {
        LOGE("Invalid Gobi Image file: 0x%4lx missing. \n", MBN_BUILD_MAGIC);
        result = FALSE;
        goto EXIT;
    }

    memset(pVersionApp, 0, versionSize);

    /* Copy the MBN_BUILD_MAGIC ID */
    struct sMBNBuildIDRecord * pRec = (struct sMBNBuildIDRecord *)pTmp;
    for (t = 0; t < MBN_BUILD_ID_LEN; t++)
    {
        if (t >= versionSize)
        {
            LOGE("Invalid Gobi Image file: build ID length error.. \n");
            result = FALSE;
            goto EXIT;
        }

        pVersionApp[t] = (char)pRec->mBuildID[t];
        if (pRec->mBuildID[t] == 0)
        {
            break;
        }
    }

    strcpy(chNewPath, pFilePath);
    folderLen = strlen(chNewPath);
    if (chNewPath[folderLen - 1] != '/')
    {
        strcat(chNewPath, "/");
    }
    snprintf(chIdPath, MAXPATHSIZE, "%d/", iSelectID);
    strcat(chNewPath, chIdPath);

    /* open file */
    strcpy(chFilePath, chNewPath);
    strcat(chNewPath, "uqcn.mbn");
    pPriHandle = fopen(chNewPath, "r");
    if(pPriHandle == NULL)
    {
        /* uqcn file was not found, use uqcn_nogps.mbn */
        strcpy(chNewPath, chFilePath);
        strcat(chNewPath, "uqcn_nogps.mbn");
        pPriHandle = fopen(chNewPath, "r");
        if(pPriHandle == NULL)
        {
            LOGE("Unable to open both uqcn.mbn and uqcn_nogps.mbn \n");
            result = FALSE;
            goto EXIT;
        }
    }

    /* move file pointer to the last 256 bytes of the file */
    ret = fseek(pPriHandle, -256, SEEK_END);
    if(ret != 0)
    {
        LOGE("Failed to seek image file. \n");
        result = FALSE;
        goto EXIT;
    }

    /* read last 256 bytes of the file */
    if (!fread(read, 256, 1, pPriHandle))
    {
        LOGE("Failed to read image file. \n");
        goto EXIT;
    }

    /* Search for the build ID */
    pTmp = (BYTE *)ReverseBinaryDataSearch( read,
                                   256,
                                   (const BYTE *)&build,
                                   (ULONG)sizeof( ULONG ) );

    if (pTmp == 0)
    {
        LOGE("Invalid Gobi Image file: 0x%4lx missing. \n", MBN_BUILD_MAGIC);
        result = FALSE;
        goto EXIT;
    }

    memset( &pVersionPri[0], 0, versionSize * sizeof( char ) );

    /* Copy the MBN_BUILD_MAGIC ID */
    pRec = (struct sMBNBuildIDRecord *)pTmp;
    for (t = 0; t < MBN_BUILD_ID_LEN; t++)
    {
        if (t >= versionSize)
        {
            LOGE("Invalid Gobi Image file: build ID length error. \n");
            result = FALSE;
            goto EXIT;
        }

        pVersionPri[t] = (char)pRec->mBuildID[t];
        if (pRec->mBuildID[t] == 0)
        {
            break;
        }
    }
    
EXIT:
    if(pAppHandle)
    {
        fclose(pAppHandle);
        pAppHandle = NULL;
    }
    if(pPriHandle)
    {
        fclose(pPriHandle);
        pPriHandle = NULL;
    }
    LOGD("GetHostImageVersion Exit. \n");
    return result;
}

/**
 *
 * Compare the image from host with that from module to find which one is newer
 *
 * @param [in] chVersion 
 *      Image ID of image path
 * @param [in] eType 
 *      Image type (AMSS or UQCN)
 *
 * @return
 *      - TRUE  if image in HD is newer
 *      - FALSE if image in module is newer *         
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */       
swi_bool IsHostFWNewer(const char *chVersion, enum eGobiMBNType eType)
{
    LOGD("IsHostFWNewer Enter. \n");

    char     chHostVersion[NUM_MAX_STR_LEN] = "";
    char     chHostCarrier[NUM_MAX_STR_LEN] = "";
    char     chDevCarrier[NUM_MAX_STR_LEN] = "";
    ULONG    ulHostVersion = 0, ulDevVersion = 0;
    struct   sImageList *pImageList = NULL;
    enum carrierImgStatus eImgStatus = FW_DIFFERENT_CARRIER;
 
    strcpy(chHostVersion, chVersion);

    if (eType == eGOBI_MBN_TYPE_MODEM)
    {
        pImageList = &lstDeviceAppImages;
    }
    else if (eType == eGOBI_MBN_TYPE_PRI)
    {
        pImageList = &lstDevicePriImages;
    }
    else
    {
        LOGE("Unsupported image type: %d. \n", eType);
        LOGD("IsHostFWNewer Exit. \n");
        return FALSE;
    }

    /* 
     * After the while loop execution, the download would be don only if a newer
     * FW was not present in the device
     */
    while(pImageList)
    {
        switch(IsFwNew(chHostVersion,
               pImageList->BuildId,
               strlen(chHostVersion),
               strlen(pImageList->BuildId)))
        {
            /* Set Image to NEW if newer images were not present in device */
            case FW_SAME_CARRIER_NEW:
                if(eImgStatus != FW_SAME_CARRIER_OLD) {
                    eImgStatus = FW_SAME_CARRIER_NEW;
                }
                break;
            case FW_SAME_CARRIER_OLD:
                eImgStatus = FW_SAME_CARRIER_OLD;
                break;
            case FW_DIFFERENT_CARRIER:
            default:
                break;
        }
        pImageList = pImageList->mpNext;
    }

    /* modem did not have the image,*/
    if (eImgStatus == FW_DIFFERENT_CARRIER) {
        LOGD("Device does not have host image: %s. \n", chHostCarrier);
    }

    if (eImgStatus == FW_SAME_CARRIER_NEW) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 *
 * Compare both AMSS and UQCN images to find whether image on host HD is newer
 *
 * @param [in] chHostAppVersion 
 *      Host AMSS Image build ID
 * @param [in] chHostPriVersion 
 *      Host UQCN Image build ID
 *
 * @return
 *      - TRUE  if image in HD is newer
 *      - FALSE if image in module is newer *         
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */       
swi_bool IsFWNewer(char *chHostAppVersion, char *chHostPriVersion)
{
    LOGD("IsFWNewer Enter. \n");
    swi_bool fRet = TRUE;

    if (strlen(chHostAppVersion) == 0 || strlen(chHostPriVersion) == 0)
    {
        LOGE("Failed to retrieve firmware versions on the host. \n");
        LOGE("IsFWNewer Exit. \n");
        return FALSE;
    }

    /* Check if the firmware (either modem or pri) is newer on the host.  If any
     of these 2 firmware is newer, we will update the firmware. */

    /* First, check if modem firmware version is newer on the host side */
    fRet = IsHostFWNewer(chHostAppVersion, eGOBI_MBN_TYPE_MODEM);

    if (fRet == FALSE) {
        /* Second, check if the pri firmware version is newer on the host */
        fRet = IsHostFWNewer(chHostPriVersion, eGOBI_MBN_TYPE_PRI);

        if (fRet) {
            LOGD("Host pri firmware is newer. \n");
        }
    } else {
        LOGD("Host modem firmware is newer. \n");
    }

    LOGD("IsFWNewer Exit. \n");
    return fRet;
}

/**
 *
 * Get all the images from module and create 2 lists for the image information
 *
 * @param [in/out] lstAppImg 
 *      Device AMSS Image list
 * @param [in/out] lstPriImg 
 *      Device UQCN Image list
 *
 * @return
 *      - TRUE  if succeed
 *      - FALSE if failed         
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */       
swi_bool LoadDeviceImages(
    struct sImageList *lstAppImg,
    struct sImageList *lstPriImg )
{
    LOGD("LoadDeviceImages Enter. \n");
    int   i = 0, j = 0;
    ULONG nRet = 0;
    struct ImageList imageList;
    ULONG nImageListSize = MAX_IMAGE_LIST_SIZE;

    nRet = GetStoredImages(&nImageListSize, (BYTE*)&imageList);

    if (!nRet)
    {
        BYTE nListSize = imageList.listSize;
        LOGD("ImageListSize (BYTES): %lu, ListSize (elements): %d. \n", 
            nImageListSize, nListSize);

        struct STRUCT_ModemImageListDetails
        {
            BYTE ImageType;
            BYTE MaxImages;
            BYTE ExecutingImage;
            BYTE NumStoredImages;
        };

        struct STRUCT_ModemImageListDetails m_arrModemImgListDetails[3];
        
        struct sImageList *pLocalImageList = NULL;
        
        /* cycle through image list elements */
        for (i=0; i<nListSize; i++)
        {
            BYTE ImageType = imageList.imageIDEntries[i].imageType;
            
            /* determine which list to add to (PRI or Application) */
            switch (ImageType)
            {
                case 0:	/* application */
                    pLocalImageList = lstAppImg;
                    break;
                case 1: /* PRI */
                    pLocalImageList = lstPriImg;
                    break;
                default:
                    LOGE("Unsupported image type: %d. \n", ImageType);
                    nRet = -1;
                    goto EXIT;
            }

            m_arrModemImgListDetails[ImageType].ImageType = ImageType;
            m_arrModemImgListDetails[ImageType].MaxImages = imageList.imageIDEntries[i].maxImages;
            m_arrModemImgListDetails[ImageType].ExecutingImage = imageList.imageIDEntries[i].executingImage;
            m_arrModemImgListDetails[ImageType].NumStoredImages = imageList.imageIDEntries[i].imageIDSize;

            LOGD("ImageType: %d, MaxImages: %d, ExecutingImage: %d, ImageIdSize: %d. \n", 
                ImageType, 
                m_arrModemImgListDetails[ImageType].MaxImages, 
                m_arrModemImgListDetails[ImageType].ExecutingImage, 
                m_arrModemImgListDetails[ImageType].NumStoredImages);
            
            /* cycle through image ID list elements */
            for (j=0; j<m_arrModemImgListDetails[ImageType].NumStoredImages; j++)
            {
                LOGD("***** %d *****", j);
                
                struct sImageList *pImageElement = NULL;
                
                pImageElement = malloc(sizeof(struct sImageList));
                memset(pImageElement, 0, sizeof(struct sImageList));
                pImageElement->ImageType = ImageType;
                pImageElement->StorageIndex = imageList.imageIDEntries[i].imageIDElement[j].storageIndex;
                pImageElement->FailureCount = imageList.imageIDEntries[i].imageIDElement[j].failureCount;

                memcpy( pImageElement->ImageId,
                        imageList.imageIDEntries[i].imageIDElement[j].imageID,
                        LEN_IMAGE_ID );

                pImageElement->BuildIdLen = imageList.imageIDEntries[i].imageIDElement[j].buildIDLength;

                memcpy( pImageElement->BuildId,
                        imageList.imageIDEntries[i].imageIDElement[j].buildID,
                        pImageElement->BuildIdLen);
                
                LOGD("Index: %d, BuildId: %s. \n", 
                    pImageElement->StorageIndex, 
                    pImageElement->BuildId);
                
                if(j == 0)
                {
                    memcpy(pLocalImageList, pImageElement, sizeof(struct sImageList));
                }
                else
                {
                    pLocalImageList->mpNext = pImageElement;
                    pLocalImageList = pLocalImageList->mpNext;
                }
            }
        }
    }
    else
    {
        LOGE("GetStoredImages returned error: %lu. \n", nRet);
    }

EXIT:
    LOGD("LoadDeviceImages Exit. \n");
    return (nRet==0?TRUE:FALSE);
}

/**
 *
 * Release image list memory
 *
 * @param [in] lstAppImg 
 *      pointer to device AMSS Image list to be released
 * @param [in] lstPriImg 
 *      pointer to device UQCN Image list to be released
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */       
void ReleaseDeviceImages( 
    struct sImageList *lstAppImg,
    struct sImageList *lstPriImg )
{
     LOGD("ReleaseDeviceImages Enter. \n");
     struct sImageList *pImageList = NULL;
     struct sImageList *pTempImageList = NULL;
   
     /* Free modem image list */
     pImageList = lstAppImg->mpNext;
     while(pImageList)
     {
        pTempImageList = pImageList->mpNext;
        free(pImageList);
        pImageList = pTempImageList;
     }
     
     /* Free Pri image list */
     pImageList = lstPriImg->mpNext;
     while(pImageList)
     {
        pTempImageList = pImageList->mpNext;
        free(pImageList);
        pImageList = pTempImageList;
     }
     LOGD("ReleaseDeviceImages Exit. \n");
}

/**
 *
 * Check the image from host then update firmware
 *
 * @param [in] iSelectID 
 *      Image ID of image path
 * @param [in] chUpdatePath 
 *      qualified path to image file
 *
 * @return
 *      - TRUE  if succeed
 *      - FALSE if failed         
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */       
swi_bool UpdateImage(int iSelectID, const char *chUpdatePath)
{
    LOGD("UpdateImage Entered. \n");

    swi_bool ret = FALSE;
    USHORT   versionSz = MBN_BUILD_ID_LEN;
    char     versionStrApp[MBN_BUILD_ID_LEN] = { 0 };
    char     versionStrPri[MBN_BUILD_ID_LEN] = { 0 };

    swi_bool result = GetHostImageVersion(iSelectID,
                                        (const char *)chUpdatePath,
                                        versionSz,
                                        &versionStrApp[0],
                                        &versionStrPri[0]);
    if(result == TRUE)
    {
        LOGD("GetHostImageVersion for %s succeed . \n", chUpdatePath);
        LOGD("App Version: %s, Pri Version: %s. \n", versionStrApp, versionStrPri);
        ret = TRUE;
    }
    else
    {
        LOGE("GetHostImageVersion for %s failed. . \n", chUpdatePath);
        ret = FALSE;
        return ret;
    }
    
    if(IsFWNewer(&versionStrApp[0], &versionStrPri[0]) == TRUE)
    {
        LOGD("Select :: iSelectID : %d : chUpdatePath : %s\n",iSelectID, chUpdatePath);
        ret = SelectImage(iSelectID, (char *)chUpdatePath);
        if(ret == TRUE)
            LOGD("UpgradeFirmware for %s succeed. \n", chUpdatePath);
        else
            LOGE("UpgradeFirmware for %s failed. \n", chUpdatePath);
    }
    
    LOGD("UpdateImage Exit. \n");
    return ret;
}

local bool GetActiveImage(const char *chListPath, const char * ID)
{
    struct qmifwinfo_s FwInfo;
    bool ret = true;
    char newpathp[MAXPATHSIZE] = "";
    
    snprintf(newpathp, sizeof(newpathp), "%s/%s", chListPath, ID);

    swi_uint32 result = SLQSGetImageInfo((char *)newpathp, &FwInfo);
    if(!result)
    {
        LOGE("%s %lu %lu %lu %08x \n",
             newpathp,
             FwInfo.dev.g.Carrier,
             FwInfo.dev.g.Technology,
             FwInfo.dev.g.Region,
             (unsigned int)FwInfo.dev.g.FirmwareID);

        if(!GetImageActive(FwInfo.dev.g.FirmwareID, &ret))
            LOGE("GetImageActive failed");
    }
    else
    {
        LOGE("GetImageInfo for %s failed: %lu", newpathp, result);
        ret = false;
    }

    LOGD("listimages exit");
    return ret;
}

/**
 *
 * Image management with "--update" option
 *
 * @param [in] chUpdatePath 
 *     qualified path to image file
 *
 * @return
 *     TRUE if function succeed
 *     FASLE if funciton failed
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */
 bool UpdateAllImages(char *chUpdatePath)
{
    LOGD("UpdateAllImages Selected. \n");
    updateActiveImage = false;
    DIR     *dirp;
    struct  dirent *dep;
    
    if(LoadDeviceImages(&lstDeviceAppImages, &lstDevicePriImages) == FALSE)
    {
        LOGE("LoadDeviceImages Failed. \n");
        return false;
    }

    dirp = opendir(chUpdatePath);
    if (dirp == NULL)
    {
        LOGE("opendir error. \n");
        return false;
    }

    for ( ; ; )
    {
        dep = readdir(dirp);

        if (dep == NULL)
            break;

        if (dep->d_name[0] != '\0' && dep->d_name[0] != '.') /* not ".." etc. */
        {
            LOGD( "folder name : %s", dep->d_name);
            if (!GetActiveImage(chUpdatePath, dep->d_name))
            {
                LOGD( "GetActiveImage error %s/%s", chUpdatePath, dep->d_name);
            }
            else
            {
                LOGD( "GetActiveImage Success %d", atoi(dep->d_name));
                updateActiveImage = TRUE;
                selectID = atoi(dep->d_name);
                break;
            }
        }
    }

    /*close the directory for re-scanning */
    closedir(dirp);

    dirp = opendir(chUpdatePath);
    if (dirp == NULL)
    {
        LOGE("opendir error");
        return false;
    }

    for ( ; ; )
    {
        dep = readdir(dirp);

        if (dep == NULL)
            break;

        if (dep->d_name[0] != '\0' && dep->d_name[0] != '.') /* not ".." etc. */
        {
            LOGD("folder name : %s. \n", dep->d_name);
            if (UpdateImage(atoi(dep->d_name), (const char *)chUpdatePath) == FALSE)
            {
                LOGE("UpdateImage error for %s. \n", chUpdatePath);
            }
        }
    }

    /* Update the previous active image */
    if( updateActiveImage == TRUE )
    {
        LOGD( "UpdateImage Active %d", selectID);
        UpdateImage( selectID, chUpdatePath );
    }

    ReleaseDeviceImages(&lstDeviceAppImages, &lstDevicePriImages);

    LOGD("UpdateAllImages Selected finished. \n");
    return true;
}

