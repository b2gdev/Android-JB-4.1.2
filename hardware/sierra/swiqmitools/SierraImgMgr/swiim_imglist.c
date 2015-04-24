/**
 *
 * @ingroup swiim
 *
 * @file
 * image list fuction of image management 
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
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <stdbool.h>

#include "SwiDataTypes.h"
#include "qmerrno.h"
#define  LOG_TAG  "swi_imgmgr"
#include "swiril_log.h"
#include "swiim_img.h" 

struct ImageList devImgList;
static struct imgInfoVb sImgInfoVb;
static int imageIndex = 0;

extern char *configFilePath;
struct ImageFinalList devImgFinalList[50];
struct ImageFinalList ModemImgFinalList[50];

/* ImageID String for Fail Safe AMSS image */
const char imgIddefAMSSImage[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const ULONG imageIDLen = 0x10;

/* Image information table */
struct imgInfoTable imgDetailInfo[MAX_IMAGES] =
{
    { "D3600", 'S', eGOBI_IMG_CAR_SPRINT,        eGOBI_IMG_REG_NA,     eGOBI_IMG_TECH_CDMA },
    { "D3600", 'V', eGOBI_IMG_CAR_VERIZON,       eGOBI_IMG_REG_NA,     eGOBI_IMG_TECH_CDMA },
    { "D3200", 'V', eGOBI_IMG_CAR_VODAFONE,      eGOBI_IMG_REG_EU,     eGOBI_IMG_TECH_UMTS },
    { "D3200", 'A', eGOBI_IMG_CAR_ATT,           eGOBI_IMG_REG_NA,     eGOBI_IMG_TECH_UMTS },
    { "D3200", 'L', eGOBI_IMG_CAR_TMOBILE,       eGOBI_IMG_REG_EU,     eGOBI_IMG_TECH_UMTS },
    { "D3200", 'G', eGOBI_IMG_CAR_GENERIC,       eGOBI_IMG_REG_GLOBAL, eGOBI_IMG_TECH_UMTS },
    { "D3200", 'H', eGOBI_IMG_CAR_TELEFONICA,    eGOBI_IMG_REG_EU,     eGOBI_IMG_TECH_UMTS },
    { "D3200", 'I', eGOBI_IMG_CAR_TELCOM_ITALIA, eGOBI_IMG_REG_EU,     eGOBI_IMG_TECH_UMTS },
    { "D3200", 'O', eGOBI_IMG_CAR_ORANGE,        eGOBI_IMG_REG_EU,     eGOBI_IMG_TECH_UMTS },
    { "D3200", 'U', eGOBI_IMG_CAR_GENERIC,       eGOBI_IMG_REG_GLOBAL, eGOBI_IMG_TECH_UMTS },
};

/**
 *
 * Stub function for GetImageInfo
 *
 * @param [in] pPath 
 *     Folder selected for firmware image
 * @param [out] pFirmwareID
 *    Firmware ID obtained from the firmware image
 * @param [out] pTechnology
 *    Technology obtained from the firmware image
 * @param [out] pCarrier
 *   Carrier obtained from the firmware image
 * @param [out] pRegion
 *    Region obtained from the firmware image
 * @param [out] pGPSCapability
 *    GPS capability obtained from the firmware image
 *
 * @return
 *     true if function succeed
 *     false if funciton failed
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */       
local ULONG GetImageInfoStub(
    CHAR  *pPath,
    ULONG *pFirmwareID,
    ULONG *pTechnology,
    ULONG *pCarrier,
    ULONG *pRegion,
    ULONG *pGPSCapability )
{
    LOGD("GetImageInfoStub entered. \n");
    if (!pPath || !pFirmwareID || !pTechnology || !pCarrier || !pRegion || !pGPSCapability)
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    *pFirmwareID = ULONG_MAX;
    *pTechnology = TECHNOLOGY_CDMA;
    *pCarrier = CARRIER_VERIZON;
    *pRegion =REGION_NORTH_AMERICA;
    *pGPSCapability =  ULONG_MAX;   

    LOGD("GetImageInfoStub exit. \n");
    return eQCWWAN_ERR_NONE;
}

/**
 *
 * Get Carrier Name from Carrier ID.
 *
 * @param [in] ID 
 *     Image ID of the image
 * @param [out] CarrierName 
 *     Carrier name of the image
 * @param [in] size 
 *     Size of carrier name
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
local void GetCarrierName(swi_uint32 ID, char *CarrierName, swi_uint32 size)
{
    LOGD("GetCarrierName entered. \n");
    switch(ID)
    {
        case CARRIER_GENERIC:
            snprintf(CarrierName, size, "    Generic    ");
            break;
        case CARRIER_FACTORY:
            snprintf(CarrierName, size, "    Factory    ");
            break;
        case CARRIER_VERIZON:
            snprintf(CarrierName, size, "    Verizon    ");
            break;
        case CARRIER_SPRINT:
            snprintf(CarrierName, size, "    Sprint     ");
            break;
        case CARRIER_ATT:
            snprintf(CarrierName, size, "     AT&T      ");
            break;
        case CARRIER_TMOBILE:
            snprintf(CarrierName, size, "   T-Mobile    ");
            break;
        case CARRIER_TELEFONICA:
            snprintf(CarrierName, size, "  Telefonica   ");
            break;
        case eGOBI_IMG_CAR_TELCOM_ITALIA:
            snprintf(CarrierName, size, " Telecom-Italia" );
            break;
        case eGOBI_IMG_CAR_ORANGE:
            snprintf(CarrierName, size, "    Orange     " );
            break;
        case eGOBI_IMG_CAR_VODAFONE:
            snprintf(CarrierName, size, "   Vodafone    " );
            break;
        default:
            snprintf(CarrierName, size, "    Unknown    " );
            break;

    }
    LOGD("GetCarrierName exit\n");
    return;
}

/**
 *
 * Get technology from technology type.
 *
 * @param [in] technology 
 *     technology of the image
 * @param [out] chTechnology 
 *     technology string of the image
 * @param [in] size 
 *     Size of technology string
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
local void GetTechnology(swi_uint32 technology, char *chTechnology, swi_uint32 size)
{

    LOGD("GetTechnology entered. \n");
    switch(technology)
    {
        case TECHNOLOGY_CDMA:
            snprintf(chTechnology, size, "CDMA");
            break;
        case TECHNOLOGY_UMTS:
            snprintf(chTechnology, size, "UMTS");
            break;
        case TECHNOLOGY_LTE:
            snprintf(chTechnology, size, "LTE ");
            break;
        default:
            snprintf(chTechnology, size, "UMTS");
            break;
    }
    LOGD("GetTechnology exit. \n");
    return;
}

/**
 *
 * Get region from region type.
 *
 * @param [in] region 
 *     region of the image
 * @param [out] chRegion 
 *     region string of the image
 * @param [in] size 
 *     Size of region string
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
local void GetRegion(swi_uint32 region, char *chRegion, swi_uint32 size)
{
    LOGD("GetRegion entered. \n");
    switch(region)
    {
        case REGION_NORTH_AMERICA:
            snprintf(chRegion, size, "     NA  ");
            break;
        case REGION_LATIN_AMERICA:
            snprintf(chRegion, size, "     LA  ");
            break;
        case REGION_EUROPE:
            snprintf(chRegion, size, "     EU  ");
            break;
        case REGION_ASIA:
            snprintf(chRegion, size, "     AS  ");
            break;
        case REGION_AUSTRILIA:
            snprintf(chRegion, size, "     AU  ");
            break;
        case REGION_GLOBAL:
            snprintf(chRegion, size, "   Global");
            break;
        default:
            snprintf(chRegion, size, "  Unknown");
            break;
    }
    LOGD("GetRegion exit \n");
    return;
}

/**
 *
 * Get whether this image is active.
 *
 * @param [in] PathImagId 
 *     Firmware ID of the image on host HD
 * @param [out] fActive 
 *     - true  if the image is curreently actived
 *     - false if the image is curreently inactived 
 *
 * @return
 *     true if function succeed
 *     false if funciton failed
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */       
bool GetImageActive(swi_uint32 PathImagId, bool *fActive)
{
    LOGD("GetImageActive entered. \n");
    swi_uint32 nRet = 0;
    struct qmifwinfo_s FwInfo;

    if(!fActive)
    {
        LOGE("fActive is NULL\n");
        return false;
    }
    nRet = SLQSGetFirmwareInfo( &FwInfo );

    if (nRet != 0)
    {
        LOGE("GetFirmwareInfo failed: %lu", nRet);
        return false;
    }
    LOGD("GetFirmwareInfo firmwareId:%08x, tec:%lu, carrier:%lu, region:%lu \n", 
        (unsigned int)FwInfo.dev.g.FirmwareID, FwInfo.dev.g.Technology,
        FwInfo.dev.g.Carrier, FwInfo.dev.g.Region);
 
    if(PathImagId == FwInfo.dev.g.FirmwareID) 
        *fActive = true;
    else 
        *fActive = false;
    
   LOGD("GetImageActive exit. \n");
   return true;
}

/**
 *
 * Get Carrier, Technology, GPS, Region infor from directory provided
 *
 * @param [in] chListPath 
 *     qualified path to image file
 * @param [in] ID 
 *     Image ID of image path
 *
 * @return
 *     true if function succeed
 *     false if funciton failed
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */       
local bool GetImgInfor(const char *chListPath, const char * ID)
{
    LOGD("listimages entered. \n");
    struct qmifwinfo_s FwInfo;
    bool   ret        = false;
    char newpathp[MAXPATHSIZE] = "";
    
    snprintf(newpathp, sizeof(newpathp), "%s/%s", chListPath, ID);

    swi_uint32 result = SLQSGetImageInfo((char *)newpathp, &FwInfo);

    if(!result)
    {
        char chCarrier[MAXPATHSIZE] = "";
        char chTechnology[MAXPATHSIZE] = "";
        char chRegion[MAXPATHSIZE] = "";
        bool fActive = false;
        char chActive[MAXPATHSIZE] = "";
       
        LOGD("%s %lu %lu %lu %08x \n",
             newpathp,
             FwInfo.dev.g.Carrier,
             FwInfo.dev.g.Technology,
             FwInfo.dev.g.Region,
             (unsigned int)FwInfo.dev.g.FirmwareID);

        GetCarrierName(FwInfo.dev.g.Carrier, chCarrier, sizeof(chCarrier)-1);
        GetTechnology(FwInfo.dev.g.Technology, chTechnology, sizeof(chTechnology)-1);
        GetRegion(FwInfo.dev.g.Region, chRegion, sizeof(chRegion)-1);
        if(!GetImageActive(FwInfo.dev.g.FirmwareID, &fActive))
        {
            LOGE("GetImageActive failed.\n");
            snprintf(chActive, MAXPATHSIZE, "Unknown");
        }
        else
        {
            if(fActive)
                snprintf(chActive, MAXPATHSIZE, "Active");
            else
                snprintf(chActive, MAXPATHSIZE, "Inactive");
        }
        
        printf("%s,%s,  %s,   %s, %08x, %s\n", ID, chCarrier, chTechnology,
               chRegion, (unsigned int)FwInfo.dev.g.FirmwareID, chActive);
        ret = true;
    }
    else
    {
        LOGE("GetImageInfo for %s failed: %lu \n", newpathp, result);
        ret = false;
    }

    LOGD("listimages exit. \n");
    return ret;
}

/**
 *
 * Image management with "--list" option
 *
 * @param [in] chListPath 
 *     qualified path to image file
 *
 * @return
 *     true if function succeed
 *     false if funciton failed
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */       
bool ListImage(char *chListPath)
{
    LOGD("ListImage Selected. \n");
    DIR     *dirp;
    struct  dirent *dep;

    dirp = opendir(chListPath);
    if (dirp == NULL)
    {
        LOGE("opendir for %s error", chListPath);
        return false;
    }

    printf("Id,   Carrier,    Technology,  Region,  Version,  Status \n");
    
    for ( ; ; )
    {
        dep = readdir(dirp);

        if (dep == NULL)
            break;

        if (dep->d_name[0] != '\0' && dep->d_name[0] != '.') /* not ".." etc. */
        {
            LOGD("folder name : %s \n", dep->d_name);
            if (!GetImgInfor(chListPath, dep->d_name))
            {
                LOGE("listimages error for %s/%s \n", chListPath, dep->d_name);
            }
        }
    }
    LOGD("ListImage Selected finished");
    return true;
}

/**
 * Print the information(passed as input) of the firmware image.
 *
 * @param [in] printActiveStatus 
 *     To know whether the current image is active on the device 
 * @param [in] pImgInfo 
 *     Image information structure
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
void DisplayImageInfo(
    BYTE               printActiveStatus,
    struct qmifwinfo_s *pImgInfo )
{
    /* Intialize the Image info buffer */
    memset( (void *)&sImgInfoVb, 0, sizeof( sImgInfoVb ) );


    GetCarrierName( pImgInfo->dev.g.Carrier,
                    sImgInfoVb.carrierName,
                    sizeof(sImgInfoVb.carrierName) );

    GetTechnology( pImgInfo->dev.g.Technology,
                   sImgInfoVb.technologyName,
                   sizeof(sImgInfoVb.technologyName) );

    GetRegion( pImgInfo->dev.g.Region,
               sImgInfoVb.regionName,
               sizeof(sImgInfoVb.regionName) );

    /* Update the activation status */
    switch( printActiveStatus )
    {
        case eIMG_INACTIVE:
            strcpy( sImgInfoVb.activeStatus, " Inactive" );
            break;

        case eIMG_ACTIVE:
            strcpy( sImgInfoVb.activeStatus, " Active" );
            break;

        default:
            strcpy( sImgInfoVb.activeStatus, " Unknown");
            break;
    }

    /* 
     * Displaying the image information from the device. The Firmware ID is not
     * available for the device image and hence updating it as "n/a"
     */
    printf( "%d,%s,  %s,   %s, %s ,%s\n",
            imageIndex++,
            sImgInfoVb.carrierName,
            sImgInfoVb.technologyName,
            sImgInfoVb.regionName,
            "  n/a ",
            sImgInfoVb.activeStatus );
}

/*
 * Name:     ExtractPRIImgInfo
 *
 * Purpose:  Extract the Carrier, Technology, region and GPS information from
 *           the received build id.
 *
 * Return:   true  - If the image is successfully identified
 *           false - If the image is not identified
 *
 * Notes:    none
 */
bool ExtractPRIImgInfo(
    char               *pBuildId,
    struct qmifwinfo_s *pImgInfo,
    int                 BuildIdLen )
{
    BYTE idx = 0;

    /* Extract the information from the Image Info table */
    for( idx = 0; idx < MAX_IMAGES; idx++ )
    {
        /* If there ia a match for technology string */
        if( NULL != strstr( pBuildId, imgDetailInfo[idx].pTechnology ) )
        {
            if( pBuildId[GOBI33_BUILD_ID_VER_INITIALS_UQCN] == '3' &&
                pBuildId[GOBI33_BUILD_ID_VER_INITIALS_UQCN + 1] > '0')
            {
                if ( pBuildId[BuildIdLen - GOBI33_CARRIER_INIT_OFFSET_FROM_END] ==
                      imgDetailInfo[idx].carrierIntials )
                {
                        pImgInfo->dev.g.Carrier = imgDetailInfo[idx].carrier;
                        pImgInfo->dev.g.Region =  imgDetailInfo[idx].region;
                        pImgInfo->dev.g.Technology = imgDetailInfo[idx].technology;
                        return true;
                }
            }
            /* If there is a match for carrier names's initial */
            else if( pBuildId[GOBI_BUILD_ID_CARRIER_INITIALS_OFFSET] ==
                         imgDetailInfo[idx].carrierIntials )
            {
                pImgInfo->dev.g.Carrier = imgDetailInfo[idx].carrier;
                pImgInfo->dev.g.Region =  imgDetailInfo[idx].region;
                pImgInfo->dev.g.Technology = imgDetailInfo[idx].technology;
                return true;
            }
        }
    }
    return false;
}

/*
 * Name:     ExtractModemImgInfo
 *
 * Purpose:  Extract the Carrier, Technology, region and GPS information from
 *           the received build id.
 *
 * Return:   true  - If the image is successfully identified
 *           false - If the image is not identified
 *
 * Notes:    none
 */
bool ExtractModemImgInfo(
    char               *pBuildId,
    struct qmifwinfo_s *pImgInfo,
    int                 BuildIdLen )
{
    BYTE idx = 0;

    /* Extract the information from the Image Info table */
    for( idx = 0; idx < MAX_IMAGES; idx++ )
    {
        /* If there is a match for technology string */
        if( NULL != strstr( pBuildId, imgDetailInfo[idx].pTechnology ) )
        {
            if( IsSierraGobiDevice() &&
                pBuildId[SL9090_BUILD_ID_CARRIER_INITIALS_OFFSET] ==
                imgDetailInfo[idx].carrierIntials )
            {
                pImgInfo->dev.g.Carrier = imgDetailInfo[idx].carrier;
                pImgInfo->dev.g.Region =  imgDetailInfo[idx].region;
                pImgInfo->dev.g.Technology = imgDetailInfo[idx].technology;
                return true;
            }
            else
            {
                if( pBuildId[GOBI33_BUILD_ID_VER_INITIALS_AMSS] == '3' &&
                    pBuildId[GOBI33_BUILD_ID_VER_INITIALS_AMSS + 1] > '0')
                {
                    if (pBuildId[BuildIdLen - GOBI33_CARRIER_INIT_OFFSET_FROM_END] ==
                      imgDetailInfo[idx].carrierIntials )
                    {
                        pImgInfo->dev.g.Carrier = imgDetailInfo[idx].carrier;
                        pImgInfo->dev.g.Region =  imgDetailInfo[idx].region;
                        pImgInfo->dev.g.Technology = imgDetailInfo[idx].technology;
                        return true;
                     }

                }
                /* If there is a match for carrier names's initial */
                else if( pBuildId[GOBI_BUILD_ID_CARRIER_INITIALS_OFFSET] ==
                         imgDetailInfo[idx].carrierIntials )
                {
                    pImgInfo->dev.g.Carrier = imgDetailInfo[idx].carrier;
                    pImgInfo->dev.g.Region =  imgDetailInfo[idx].region;
                    pImgInfo->dev.g.Technology = imgDetailInfo[idx].technology;
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * Gets the PRI image index from the list.
 *
 * @param [out] pImgListIdx 
 *     The list index of the PRI image list
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
 *
 */
bool GetPRIImageIdx( BYTE *pImgListIdx)
{
    bool retVal = false;    
    /* Figure out the index of PRI image type in the received image list */
    if( IMG_TYPE_PRI == devImgList.imageIDEntries[0].imageType )
    {
        *pImgListIdx = 0;
        retVal = true;
    }
    else if( 2 == devImgList.listSize )
    {
        /* If the PRI image is not located at the first index in the list and
         * sizeof the retrieved list is 2, then next image index is the PRI
         * image index.
         */
        *pImgListIdx = 1;
        retVal = true;
    }
    return retVal;
}

/**
 * Return the status of the PRI Image based on the rest of the images on the
 * host
 *
 * @param [out] pPRIImgBuildID
 *     The build ID of the PRI image
 *
 * @param [out] pOldPRIImgBuildID
 *     The build ID used to compare whether the current Image is new
 *
 * @param [out] PRIBuildIdLen
 *     The build ID length of the PRI image
 *
 * @param [out] OldPRIBuildLen
 *     The build ID length used to compare whether the current Image is new
 *
 * @return
 *     FW_DIFFERENT_CARRIER if the carrier image is not present
 *     FW_SAME_CARRIER_OLD if no new carrier of the same image is present
 *     FW_SAME_CARRIER_NEW if new carrier of the same image is present
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
enum carrierImgStatus IsFwNew( CHAR *pPRIImgBuildID,
                               CHAR *pOldPRIImgBuildID,
                               ULONG PRIBuildIdLen,
                               ULONG OldPRIBuildLen )
{

    CHAR PRIImgStr[100], OldPRIImgStr[100];
    CHAR *pPRIBuildIDVer, *pOldPRIBuildIDVer;
    struct qmifwinfo_s ImgInfo, OldImgInfo;
    CHAR BuildIdVerNew[MAX_VER_STR_LEN];
    CHAR BuildIdVerOld[MAX_VER_STR_LEN];
    BYTE count = 0, idx = 0, DigitsInOldVer = 0 ,DigitsInNewVer = 0;
    int diff = 0;
    CHAR DigitsAfterString[3] = {'\0','\0','\0'};

    strcpy( PRIImgStr, pPRIImgBuildID );
    strcpy( OldPRIImgStr, pOldPRIImgBuildID );

    /* Extract the image informations */
    if ( !ExtractPRIImgInfo(pPRIImgBuildID, &ImgInfo, PRIBuildIdLen ) ||
         !ExtractPRIImgInfo(pOldPRIImgBuildID, &OldImgInfo, OldPRIBuildLen ))
    {
        return FW_DIFFERENT_CARRIER;
    }

    /* Make sure that the carrier, technology and region are the same */
    if ((ImgInfo.dev.g.Carrier    != OldImgInfo.dev.g.Carrier) ||
        (ImgInfo.dev.g.Region     != OldImgInfo.dev.g.Region) ||
        (ImgInfo.dev.g.Technology != OldImgInfo.dev.g.Technology))
    {
        return FW_DIFFERENT_CARRIER;
    }

    /* Get the PRI Image version */
    pPRIBuildIDVer = strtok ( PRIImgStr, "-" );
    pPRIBuildIDVer = strtok ( NULL, "-" );
    pPRIBuildIDVer = strtok ( NULL, "-" );

    /* Get the old PRI Image version */
    pOldPRIBuildIDVer = strtok ( OldPRIImgStr, "-" );
    pOldPRIBuildIDVer = strtok ( NULL, "-" );
    pOldPRIBuildIDVer = strtok ( NULL, "-" );

    /* Check if AMSS iamge is Gobi3.3 or higher
     * (to allow Gobi5.5 down the line)
     */
    if( pPRIImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_UQCN] == '3' &&
        pPRIImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_UQCN + 1] > '0' )
    {
        if( pOldPRIImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_UQCN] == '3' &&
            pOldPRIImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_UQCN + 1] > '0' )
        {
            /* Read and store digits in NULL terminated Version string */
            for ( idx = 0; pOldPRIBuildIDVer[idx] != '\0'; idx++ )
            {
                if (pOldPRIBuildIDVer[idx] >= '0' &&
                    pOldPRIBuildIDVer[idx] <= '9')
                {
                    BuildIdVerOld[DigitsInOldVer] = pOldPRIBuildIDVer[idx];
                    DigitsInOldVer++;
                }
            }

            for ( idx = 0; pPRIBuildIDVer[idx] != '\0'; idx++ )
            {
                if ( pPRIBuildIDVer[idx] >= '0' && pPRIBuildIDVer[idx] <= '9')
                {
                    BuildIdVerNew[DigitsInNewVer] = pPRIBuildIDVer[idx];
                    DigitsInNewVer++;
                }
            }

            /* If digits in version string are not equal do Normalization */
            diff = DigitsInOldVer -DigitsInNewVer;
            if ( diff > 0 )
            {
                /* Save digits "zz" after YY string i.e. carrier string */
                DigitsAfterString[0] = BuildIdVerNew[DigitsInNewVer-2];
                DigitsAfterString[1] = BuildIdVerNew[DigitsInNewVer-1];

                for( count = 0; diff > count; count++)
                {
                    BuildIdVerNew[DigitsInNewVer -2 + count]= '0';
                }
                /* Terminate buffer with NULL */
                BuildIdVerNew[DigitsInNewVer-2 + count ]= '\0';
                /* Append digits "zz" in buffer after Normalization */
                strcat(BuildIdVerNew,DigitsAfterString);
            }

            if( diff < 0 )
            {
                /* To make diff a positive number */
                diff *= -1;
                DigitsAfterString[0] = BuildIdVerOld[DigitsInOldVer-2];
                DigitsAfterString[1] = BuildIdVerOld[DigitsInOldVer-1];
                for( count = 0; diff > count; count++)
                {
                    BuildIdVerOld[DigitsInOldVer -2 + count]= '0';
                }
                BuildIdVerOld[DigitsInOldVer-2 + count ]= '\0';
                strcat(BuildIdVerOld,DigitsAfterString);
            }

            /**
             * Check to see which of the FW is newer:
             * only if AMSS Images are Gobi3.3 or higher
             */
            if (atol(BuildIdVerNew) > atol(BuildIdVerOld))
            {
                return FW_SAME_CARRIER_NEW;
            }
            else
            {
                return FW_SAME_CARRIER_OLD;
            }
        }
        else
        {
             /* Check to see which of the FW is newer */
             return FW_SAME_CARRIER_NEW;
        }
    }

    /* Check to see which of the FW is newer: Only if AMSS Images Gobi1.7 */
    if (atoi(pPRIBuildIDVer) > atoi(pOldPRIBuildIDVer))
    {
        return FW_SAME_CARRIER_NEW;
    }
    return FW_SAME_CARRIER_OLD;
}

/**
 * Name:     IsFwModemImageNew
 *
 * Purpose:  Return the status of the Modem Image based on the rest of the
 *           images on the host
 *
 * param     [out] pModemImgBuildID
 *           The build ID of the Modem image
 *
 * param     [out] pOldModemImgBuildID
 *           The build ID used to compare whether the current Image is new
 *
 * return
 *      FW_DIFFERENT_CARRIER if the carrier image is not present
 *      FW_SAME_CARRIER_OLD if no new carrier of the same image is present
 *      FW_SAME_CARRIER_NEW if new carrier of the same image is present
 *
 * note
 *     none
 */
enum carrierImgStatus IsFwModemImageNew( CHAR *pModemImgBuildID,
                                         CHAR *pOldModemImgBuildID,
                                         ULONG ModemBuildIdLen,
                                         ULONG OldModemBuildLen )
{
    CHAR ModemImgStr[100], OldModemImgStr[100];
    CHAR *pModemBuildIDVer, *pOldModemBuildIDVer;
    CHAR newModemBuildIDVer[10];
    CHAR oldModemBuildIDVer[10];
    BYTE offset, newImageCount = 0, oldImageCount = 0;
    struct qmifwinfo_s ImgInfo, OldImgInfo;
    CHAR BuildIdVerNew[MAX_VER_STR_LEN];
    CHAR BuildIdVerOld[MAX_VER_STR_LEN];
    BYTE count = 0, idx = 0, DigitsInOldVer = 0 ,DigitsInNewVer = 0;
    int diff = 0;
    CHAR DigitsAfterString[3] = {'\0','\0','\0'};

    strcpy( ModemImgStr, pModemImgBuildID );
    strcpy( OldModemImgStr, pOldModemImgBuildID );

    /* Extract the image informations */
    if (!ExtractModemImgInfo(pModemImgBuildID, &ImgInfo,ModemBuildIdLen ) ||
        !ExtractModemImgInfo(pOldModemImgBuildID, &OldImgInfo, OldModemBuildLen))
    {
        return FW_DIFFERENT_CARRIER;
    }

    /* Make sure that the carrier, technology and region are the same */
    if ((ImgInfo.dev.g.Carrier    != OldImgInfo.dev.g.Carrier) ||
        (ImgInfo.dev.g.Region     != OldImgInfo.dev.g.Region) ||
        (ImgInfo.dev.g.Technology != OldImgInfo.dev.g.Technology))
    {
        return FW_DIFFERENT_CARRIER;
    }

    /* Get the Modem Image version */
    pModemBuildIDVer = strtok ( ModemImgStr, "-" );
    pModemBuildIDVer = strtok ( NULL, "-" );
    pModemBuildIDVer = strtok ( NULL, "-" );

    /* Get the old Modem Image version */
    pOldModemBuildIDVer = strtok ( OldModemImgStr, "-" );
    pOldModemBuildIDVer = strtok ( NULL, "-" );
    pOldModemBuildIDVer = strtok ( NULL, "-" );

    if(IsSierraGobiDevice())
    {
        for (offset = VERSION_ID_OFFSET;
             offset <= strlen(pModemBuildIDVer);
             offset++)
        {
            newModemBuildIDVer[newImageCount] = pModemBuildIDVer[offset];
            newImageCount++;
        }
        for (offset = VERSION_ID_OFFSET;
             offset<= strlen(pOldModemBuildIDVer);
             offset++)
        {
            oldModemBuildIDVer[oldImageCount] = pOldModemBuildIDVer[offset];
            oldImageCount++;
        }
        if(atol(newModemBuildIDVer) > atol(oldModemBuildIDVer))
        {
            return FW_SAME_CARRIER_NEW;
        }
        else
        {
            return FW_SAME_CARRIER_OLD;
        }
    }
    else
    {
        /* Check if AMSS iamge is Gobi3.3 or higher
         * (to allow Gobi5.5 down the line)
         */
         if( pModemImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_AMSS] == '3' &&
             pModemImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_AMSS + 1] > '0' )
         {
             if( pOldModemImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_AMSS] == '3' &&
                 pOldModemImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_AMSS + 1] > '0' )
             {
                 /* Read and store digits in NULL terminated Version string */
                 for (idx = 0;
                      pOldModemBuildIDVer[idx] != '\0';
                      idx++ )
                 {
                    if (pOldModemBuildIDVer[idx] >= '0' &&
                        pOldModemBuildIDVer[idx] <= '9' )
                    {
                        BuildIdVerOld[DigitsInOldVer] = pOldModemBuildIDVer[idx];
                        DigitsInOldVer++;
                    }
                 }

                 for ( idx = 0; pModemBuildIDVer[idx] != '\0'; idx++ )
                 {
                     if (pModemBuildIDVer[idx] >= '0' &&
                         pModemBuildIDVer[idx] <= '9')
                     {
                         BuildIdVerNew[DigitsInNewVer] = pModemBuildIDVer[idx];
                         DigitsInNewVer++;
                     }
                 }

                 /* If digits in version str are not equal do Normalization */
                 diff = DigitsInOldVer -DigitsInNewVer;
                 if ( diff > 0 )
                 {
                     /* Save digits "zz" after YY string i.e. carrier string */
                     DigitsAfterString[0] = BuildIdVerNew[DigitsInNewVer-2];
                     DigitsAfterString[1] = BuildIdVerNew[DigitsInNewVer-1];

                     for( count = 0; diff > count; count++)
                     {
                         BuildIdVerNew[DigitsInNewVer -2 + count]= '0';
                     }
                     /* Terminate buffer with NULL */
                     BuildIdVerNew[DigitsInNewVer-2 + count ]= '\0';
                     /* Append digits "zz" in buffer after Normalization */
                     strcat(BuildIdVerNew,DigitsAfterString);
                 }

                 if( diff < 0 )
                 {
                     /* To make diff a positive number */
                     diff *= -1;
                     DigitsAfterString[0] = BuildIdVerOld[DigitsInOldVer-2];
                     DigitsAfterString[1] = BuildIdVerOld[DigitsInOldVer-1];
                     for( count = 0; diff > count; count++)
                     {
                         BuildIdVerOld[DigitsInOldVer -2 + count]= '0';
                     }
                     BuildIdVerOld[DigitsInOldVer-2 + count ]= '\0';
                     strcat(BuildIdVerOld,DigitsAfterString);
                 }

                 /*
                  * Check to see which of the FW is newer:
                  * Only of AMSS Images are Gobi3.3 or higher
                  */
                 if (atol(BuildIdVerNew) > atol(BuildIdVerOld))
                 {
                     return FW_SAME_CARRIER_NEW;
                 }
                 else
                 {
                     return FW_SAME_CARRIER_OLD;
                 }
             }
             else
             {
                  /* Check to see which of the FW is newer */
                  return FW_SAME_CARRIER_NEW;
             }
         }
         /* Check to see which of the FW is newer: Only if AMSS Images Gobi1.7 */
         if (atoi(pModemBuildIDVer) > atoi(pOldModemBuildIDVer))
         {
             return FW_SAME_CARRIER_NEW;
         }
         return FW_SAME_CARRIER_OLD;
    }
}

/**
 * Get the carrier string format present in the config file
 *
 * @param [in] carrier 
 *    The carrier of the Modem image
 *
 * @return
 *     The carrier substring as present in the config file
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *      None
 */
char *GetCarrierString (ULONG carrier)
{
    switch (carrier)
    {
        case eGOBI_IMG_CAR_SPRINT:
            return "SP=";
        case eGOBI_IMG_CAR_VERIZON:
            return "VE=";
        case eGOBI_IMG_CAR_VODAFONE:
            return "VO=";
        case eGOBI_IMG_CAR_ATT:
            return "AT=";
        case eGOBI_IMG_CAR_TMOBILE:
            return "TM=";
        case eGOBI_IMG_CAR_GENERIC:
            return "GE=";
        case eGOBI_IMG_CAR_TELEFONICA:
            return "TE=";
        case eGOBI_IMG_CAR_TELCOM_ITALIA:
            return "TI=";
        case eGOBI_IMG_CAR_ORANGE:
            return "OR=";
        default:
            return NULL;
    }
    return NULL;
}

/**
 * Name:    Scanfile
 *
 * Purpose: Scans carrierImageOverride file line by line
 *
 * param   [in] filename
 *          File Path
 *
 * param   [in] Buffer
 *          Buffer to store carrierImageOverride file content.
 *
 * return
 *         The carrier substring as present in the carrierImageOverride file.
 *
 * note
 *         None
 */
bool Scanfile( char *filename, char *buffer )
{
    FILE *file = fopen ( filename, "r" );
    char *pch;
    if ( file != NULL )
    {
        /* Assume Maximum line length is not more than 100 characters */
        char line[100];

        memset( line, 0, sizeof(line));

        /* Read a line */
        while ( fgets ( line, sizeof line, file ) != NULL )
        {
            /* Discard the content if read line is comment */
            pch = strstr(line,"#");
            if( !pch )
            {
                strcat(buffer, line);
            }
        }
        /* Close file */
        fclose ( file );
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Check whether the modem image has a version according to the config file
 *
 * @param [in] modemImdIdx 
 *    Image index of the Modem image
 *
 * @param [in] carrier 
 *    The carrier of the Modem image
 *
 * @param [in] version 
 *    The version of the Modem image
 *
 * @return
 *     true if function succeed
 *     false if function failed
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *      None
 */
bool IsAptModemImagePresent( BYTE modemImdIdx, ULONG carrier, ULONG version )
{
    char buffer[1000];
    char *pVersion, *pCarrier;
    bool nRet = false,fileScan = false;

    /* Initialize path for carrierImageOverride.txt file
     * to point at directory where we fire build for application */
    if(IsSierraGobiDevice())
    {
        configFilePath = "./carrierImageOverrideSierraGobi.txt.";
    }
    else
    {
        configFilePath = "./carrierImageOverride.txt";
    }

    /* Scan file line by line */
    fileScan = Scanfile( configFilePath, buffer );

    /*
     * If file scan fails, Default to normal image switching.
     * This could be a case when file is not present in make directory
     */
    if( !fileScan )
    {
        /* If modem image at present index is latest? */
        if ( !ModemImgFinalList[modemImdIdx].IsLatest)
        {
            /* Check for other images present in modem */
            return false;
        }
        else
        {
            /* If modem Image latest activate this */
            return true;
        }
    }

    /* If the file is present, check for the configure file for compatibility */
    if ( fileScan )
    {
        pCarrier = GetCarrierString(carrier);

        /* If the carrier information is present in the file, extract version */
        if( pCarrier )
        {
            pVersion = strstr(buffer, pCarrier);

            if( pVersion )
            {
                strtok(pVersion, "=");
                pVersion = strtok(NULL, "\n");
                nRet = true;
            }
            else
            {
                /* If modem image at present index is latest? */
                if ( !ModemImgFinalList[modemImdIdx].IsLatest)
                {
                    /* Check for other images present in modem */
                    return false;
                }
                else
                {
                    /* If modem Image latest activate this */
                    return true;
                }
            }
        }

        /*
         * Once the version is extracted, compare with the version of the
         * image on the device
         */
        if ( nRet )
        {
            if ( (ULONG)atol(pVersion) == version )
            {
                nRet = true;
            }
            else
            {
                nRet = false;
            }
        }
    /* If the file is not present select the first image found, hence true */
    }
    else
    {
        nRet = true;
    }
    return nRet;
}

/**
 * Check whether the corresponding (with respect to the PRI image in the
 * mentioned imageIndex) modem image is present
 *
 * @param [in] imageIndex
 *    The image index of the PRI image
 *
 * @param [in] pModemImgBuildID
 *    The build ID of the Modem image
 *
 * @return
 *     true if function succeed
 *     false if function failed
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *      None
 */
bool CheckModemImage(
BYTE imageIndex, BYTE modemImdIdx, CHAR *pModemImgBuildID, ULONG BuildIDlen )
{
    struct qmifwinfo_s ModemImgInfo, *pPRIImgInfo;
    CHAR *pModemBuildIDVer, *pNewModemBuildIDVer = NULL;
    CHAR ModemImgStr[100];

    strcpy( ModemImgStr, pModemImgBuildID );
    pPRIImgInfo = &devImgFinalList[imageIndex].imgInfo;
    ExtractModemImgInfo( pModemImgBuildID, &ModemImgInfo, BuildIDlen );

    /* Get the modem build ID Image version */
    pModemBuildIDVer = strtok ( ModemImgStr, "-" );
    pModemBuildIDVer = strtok ( NULL, "-" );
    pModemBuildIDVer = strtok ( NULL, "-" );

    /*
     * If the carrier, region and technology are the same; check whether modem
     * image has a similar or higher version present in the configuration file
     */
    if (((ModemImgInfo.dev.g.Technology == eGOBI_IMG_TECH_UMTS) &&
         (ModemImgInfo.dev.g.Carrier    == eGOBI_IMG_CAR_GENERIC) &&
         (ModemImgInfo.dev.g.Technology == pPRIImgInfo->dev.g.Technology)) ||
        ((ModemImgInfo.dev.g.Technology == eGOBI_IMG_TECH_CDMA) &&
         (ModemImgInfo.dev.g.Carrier    == pPRIImgInfo->dev.g.Carrier) &&
         (ModemImgInfo.dev.g.Region     == pPRIImgInfo->dev.g.Region) &&
         (ModemImgInfo.dev.g.Technology == pPRIImgInfo->dev.g.Technology)))
    {
        if(IsSierraGobiDevice())
        {
            pNewModemBuildIDVer = pModemBuildIDVer + VERSION_ID_OFFSET;
            return IsAptModemImagePresent ( modemImdIdx,
                     pPRIImgInfo->dev.g.Carrier, atol(pNewModemBuildIDVer));
        }
        else
        {
            if( pModemImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_AMSS] == '3' &&
            pModemImgBuildID[GOBI33_BUILD_ID_VER_INITIALS_AMSS +1 ] > '0')
            {
                /* If modem image at present index is latest? */
                if ( !ModemImgFinalList[modemImdIdx].IsLatest)
                {
                /* Check for other images present in modem */
                return false;
                }
                else
                {
                /* If modem Image latest activate this */
                return true;
                }
            }
            else
            {
                return IsAptModemImagePresent ( modemImdIdx,
                                             pPRIImgInfo->dev.g.Carrier,
                                             atoi(pModemBuildIDVer));
            }
        }
    }
    return false;
}

/**
 * Gets the Final PRI image index from the list.
 *
 * @param [out] pPriImgEntry
 *    The list of PRI images on the device
 *
 * @return
 *     None
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *      The list which needs to be displayed to the user is based on two
 *      conditions:
 *          - The Image is the lastest Image (based on build ID version )
 *          - The Image is ACTIVE if not the latest 
 */
void GetFinalImageList( struct ImageIDEntries *pPriImgEntry )
{
    BYTE idx = 0, idx_old = 0;

    /* Clear the Image list structure */
    memset( (void *)&devImgFinalList, 0, sizeof(devImgFinalList) );

    /* Extract the information from each PRI image */
    for( idx = 0; idx < pPriImgEntry->imageIDSize; idx++ )
    {
        struct ImageIdElement *pImgElement = &pPriImgEntry->imageIDElement[idx];

        /* Set this by default to true */
        devImgFinalList[idx].IsLatest = true;

        if ( !ExtractPRIImgInfo( pImgElement->buildID,
                                 &devImgFinalList[idx].imgInfo,
                                 pImgElement->buildIDLength ) )
        {
            LOGE( "Unknown Image : %s\n",
                  pPriImgEntry->imageIDElement[idx].buildID );
            devImgFinalList[idx].IsLatest = false;
            continue;
        }

        /* 
         * Cross check with the other PRI Images and proceed to display only
         * when the FW is new or if the FW is currently active one on the host
         */
        for( idx_old = 0; idx_old < idx; idx_old++ )
        {
            struct ImageIdElement *pOldImgElement =
                         &pPriImgEntry->imageIDElement[idx_old];

            /* Check to see if we found a new FW, of the same carrier */
            switch (IsFwNew (pImgElement->buildID,
                             pOldImgElement->buildID,
                             pImgElement->buildIDLength,
                             pOldImgElement->buildIDLength))
            {
                case FW_SAME_CARRIER_NEW:
                     /* Change the status if this is not an executing image */
                    if( idx_old != pPriImgEntry->executingImage ) {
                        devImgFinalList[idx_old].IsLatest = false;
                    }
                    break;
                case FW_SAME_CARRIER_OLD:
                    /* Change the status if this is not an executing image */
                    if( idx != pPriImgEntry->executingImage ) {
                        devImgFinalList[idx].IsLatest = false;
                    }
                    break;
                case FW_DIFFERENT_CARRIER:
                default:
                    break;
            }

            /* 
             * Break from the loop if the current Image is neither the latest
             * nor the active one on the host
             */ 
            if( !devImgFinalList[idx].IsLatest )
                break;
        }
    }
}

/**
 * Name:    GetFinalModemImageList
 *
 * Purpose: Gets the Final PRI image index from the list.
 *
 * param    [out] pPriImgEntry
 *          The list of PRI images on the device
 *
 * return
 *          None
 *
 * note
 *      The list which needs to be displayed to the user is based on two
 *      conditions:
 *          - The Image is the lastest Image (based on build ID version )
 *          - The Image is ACTIVE if not the latest
 */
void GetFinalModemImageList( struct ImageIDEntries *pModemImgEntry )
{
    BYTE idx = 0, idx_old = 0;

    /* Clear the Image list structure */
    memset( (void *)&ModemImgFinalList, 0, sizeof(ModemImgFinalList) );

    /* Extract the information from each PRI image */
    for( idx = 0; idx < pModemImgEntry->imageIDSize; idx++ )
    {
        struct ImageIdElement *pImgElement = &pModemImgEntry->imageIDElement[idx];

        /* Set this by default to true */
        ModemImgFinalList[idx].IsLatest = true;

        if ( !ExtractModemImgInfo( pImgElement->buildID,
                                   &ModemImgFinalList[idx].imgInfo,
                                   pImgElement->buildIDLength) )
        {
            if ( 'X' != pImgElement->buildID[GOBI_BUILD_ID_CARRIER_INITIALS_OFFSET] )
            {
                LOGE( "Unknown Image : %s\n", pModemImgEntry->imageIDElement[idx].buildID );
            }
            ModemImgFinalList[idx].IsLatest = false;
            continue;
        }

        /* Check if the image ID is zero*/
        if( strncmp( (const char *)pImgElement->imageID,
                     imgIddefAMSSImage,
                     imageIDLen ) == 0 )
        {
            /* Avoid setting the fail safe AMSS image as latest image */
            ModemImgFinalList[idx].IsLatest = false;
            continue;
        }

        /*
         * Cross check with the other PRI Images and proceed to display only
         * when the FW is new or if the FW is currently active one on the host
         */
        for( idx_old = 0; idx_old < idx; idx_old++ )
        {
            struct ImageIdElement *pOldImgElement =
                         &pModemImgEntry->imageIDElement[idx_old];
            /* Check if the image ID is zero*/
            if( strncmp( (const char *)pOldImgElement->imageID,
                        imgIddefAMSSImage,
                        imageIDLen ) == 0 )
            {
                /* Ignore the fail safe AMSS image for cross checking for latest image */
                continue;
            }
            /* Check to see if we found a new FW, of the same carrier */
            switch( IsFwModemImageNew( pImgElement->buildID,
                                       pOldImgElement->buildID,
                                       pImgElement->buildIDLength,
                                       pOldImgElement->buildIDLength ) )
            {
                case FW_SAME_CARRIER_NEW:
                     /* Change the status if not latest image */
                        ModemImgFinalList[idx_old].IsLatest = false;
                    break;
                case FW_SAME_CARRIER_OLD:
                    /* Change the status if this is not latest image */
                        ModemImgFinalList[idx].IsLatest = false;
                    break;
                case FW_DIFFERENT_CARRIER:
                default:
                    break;
            }

            /*
             * Break from the loop if the current Image is neither the latest
             * nor the active one on the host
             */
            if( !ModemImgFinalList[idx].IsLatest )
                break;
        }
    }
}

/**
 * Display the list of carrier images loaded in the device.
 *
 * @param [in] displayStatus 
 *     To know whether the image has to be listed in the shell
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
 *
 */
bool ListDeviceImages( bool displayStatus )
{
    struct qmifwinfo_s    imgInfo;
    struct ImageIDEntries *pPriImgEntry, *pModemImgEntry = NULL;
    ULONG            resultCode, size = 0;
    BYTE             imgListIdx = 0;
    BYTE             activeImageFound = eIMG_INACTIVE, idx = 0;

    size = sizeof( devImgList );

    /* Clear the Image list structure */
    memset( (void *)&devImgList, 0, size );

    /* Retrieve the information about the images loaded in device */
    resultCode = GetStoredImages( &size,
                                  (BYTE *)&devImgList );

    /* Return if we receive an error */
    if( resultCode != 0 )
    {
        LOGE("ListDeviceImages failed: Failure code: %lu", resultCode);
        return false;
    }

    /* Figure out the index of PRI image type in the received image list */
    /* If no PRI image exist on the device */
    if( !GetPRIImageIdx( &imgListIdx ) )
    {
        LOGE("FAIL SAFE Image Active!" );
        return false;
    }

    /* Store the pointer to PRI and MODEM image list */
    pPriImgEntry   = &( devImgList.imageIDEntries[imgListIdx] );
    pModemImgEntry = &( devImgList.imageIDEntries[ (imgListIdx+1)%2 ] );

    /* Get final image list of UQCN(PRI) Images */
    GetFinalImageList( pPriImgEntry );

    /* Get Final list of AMSS(MODEM) Images */
    GetFinalModemImageList( pModemImgEntry );

    /* If the list does not need to be displayed, exit */
    if ( !displayStatus ) {
        return true;
    }

    printf("Id,   Carrier,    Technology,  Region, Version, Status \n");

    imageIndex = 1;
    /* Extract the information from each PRI image */
    for( idx = 0; idx < pPriImgEntry->imageIDSize; idx++ )
    {
        /* Consider image as inactive by default */
        activeImageFound = eIMG_INACTIVE;

        /*  If fail safe image is active, do not consider PRI image */
        if( FAIL_SAFE_IMG_IDX != pModemImgEntry->executingImage )
        {
             /* If the current image is executing image */
            if( idx == pPriImgEntry->executingImage )
            {
                /* Set the value to display the image as active */
                activeImageFound = eIMG_ACTIVE;
            }
            else
            {
                activeImageFound = eIMG_INACTIVE;
            }
        }

        /* 
         * Need to display only the one which is the latest image
         * if otherwise, the image has to be the active image on the device
         */
        if (!devImgFinalList[idx].IsLatest)
            continue;

        /* Display the image information */
        DisplayImageInfo( activeImageFound, &devImgFinalList[idx].imgInfo );
    }

    /* If no image is active, then fail safe image will be executing */
    if( ( UNKNOWN_EXECUTING_IMG_IDX == pPriImgEntry->executingImage ) ||
        ( FAIL_SAFE_IMG_IDX == pModemImgEntry->executingImage ) )
    {
        LOGE("FAIL SAFE Image Active!!");
    }
    return true;
}

