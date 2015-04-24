 /**************
 *
 *  Filename: qaGobiApiFmsTest.c
 *
 *  Purpose:  Firmware Management Service (FMS) API test driver function
 *            definitions
 *
 * Copyright: © 2011 Sierra Wireless, Inc., all rights reserved
 *
 **************/

/* Linux definitions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* include files */
#include "SwiDataTypes.h"
#include "qmerrno.h"
#include "qmudefs.h"
#include "sludefs.h"

#include "qaGobiApiFms.h"
#include "qaFmsSlqsGetImageInfo.h"
#include "qaFmsGetStoredImages.h"
#include "qaGobiApiFmsTest.h"
#include "qatesthelper.h"
#include "qatestproto.h"
#include "qaGobiApiCbk.h"
#include "qaGobiApiDms.h"

/*
 * Name:     doGetImageStore
 *
 * Purpose:  Perform the tests that call the GetImageStore() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetImageStore( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    CHAR  imageStorePath;
    FILE  *fp=NULL;

    fp = tfopen("../../fms/test/results/qafmsstubs.txt", "a");
    fprintf(fp, "<GetImagestore>\n");

    if (fp)
    {
        while ( tCaseNum < MAX_GET_IMAGE_STORE_TESTCASE_NUM )
        {
            fprintf(fp,"Test Case %d / %d : %s\n",
                    (tCaseNum+ 1),
                    MAX_GET_IMAGE_STORE_TESTCASE_NUM,
                    GetImageStoreTestCases[tCaseNum].desc );

            nRet = GetImageStore( GetImageStoreTestCases[tCaseNum]. \
                                                imageStorePathSize,
                                  &imageStorePath );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "GetImagestore Successful\n");
            }
            tCaseNum++;
        }
   }
   if (fp)
       tfclose(fp);
}

void doSLQSGetFirmwareInfo( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp=NULL;

    struct qmifwinfo_s ImageInfo;
    BYTE stringsize = 128;
    CHAR ModelId[stringsize];
    ULONG  resultCode;
    CHAR *pstr = NULL;

    resultCode = GetModelID(stringsize,
                            ModelId );
    if( eQCWWAN_ERR_NONE != resultCode )
    {
        fprintf( stderr, "Failed to get device info" );
        doprintreason( stderr, resultCode );
        return;
    }

    /* Define the test cases to be executed */
    SLQSGetFWInfoTestCase_t
        SLQSGetFWInfoTestCases[MAX_SLQS_GET_FW_INFO_TESTCASE_NUM] =
    {
        { eQCWWAN_ERR_INVALID_ARG,  1, NULL, "Sending OUT param as NULL" },
        { eQCWWAN_ERR_NONE,         2, &ImageInfo, "All Valid" }
    };

    fp = tfopen("../../fms/test/results/slqsGetFirmwareInfo.txt","w");

    if (fp)
    {
        while ( tCaseNum < MAX_SLQS_GET_FW_INFO_TESTCASE_NUM )
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum+ 1),
                        MAX_SLQS_GET_FW_INFO_TESTCASE_NUM,
                        SLQSGetFWInfoTestCases[tCaseNum].desc);

            nRet = SLQSGetFirmwareInfo(
                       SLQSGetFWInfoTestCases[tCaseNum].pInfo );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                pstr = strstr (ModelId, "MC77");
                if (pstr != NULL)
                {
                    fprintf(fp, "SLQSGetFirmwareInfo Successful\n");
                    fprintf(fp, "Model Id: %s\n",\
                            ImageInfo.dev.s.modelid_str);
                    fprintf(fp, "Boot Version: %s\n",\
                            ImageInfo.dev.s.bootversion_str);
                    fprintf(fp, "Application Version: %s\n",\
                            ImageInfo.dev.s.appversion_str);
                    fprintf(fp, "SKU Id: %s\n",\
                            ImageInfo.dev.s.sku_str);
                    fprintf(fp, "Package Id: %s\n",\
                            ImageInfo.dev.s.packageid_str);
                    fprintf(fp, "Carrier: %s\n",\
                            ImageInfo.dev.s.carrier_str);
                    fprintf(fp, "PRI Version: %s\n",\
                            ImageInfo.dev.s.priversion_str);
                }
                else
                {
                    fprintf(fp, "Firmware id: %lx\n",ImageInfo.dev.g.FirmwareID);
                    fprintf(fp, "technology: %lu\n",ImageInfo.dev.g.Technology);
                    fprintf(fp, "Carrier : %lu\n",ImageInfo.dev.g.Carrier);
                    fprintf(fp, "region: %lu\n",ImageInfo.dev.g.Region);
                    fprintf(fp, "gPSCapability: %lu\n",ImageInfo.dev.g.GPSCapability);
                }
            }
            tCaseNum++;
        }
   }
   if (fp)
       tfclose(fp);
}

void doUpgradeFirmware2k( void )
{
    /* Define the test cases to be executed */
    UpgrdFw2kTC_t UpgrdFw2kTCs[MAX_UPGRADE_FIRMWARE_2K_TESTCASE_NUM] =
    {
        { eQCWWAN_ERR_NONE,
          1,
          "",
          "Empty string firmware upgrade file path" },

        { eQCWWAN_ERR_NONE,
          2,
          "/opt/Qualcomm/Gobi/Images/4000/Generic/",
          "non existent firmware upgrade file path" },

        { eQCWWAN_ERR_NONE,
          3,
          "/opt/Qualcomm/Gobi/Images/3000/Generic/",
          "valid firmware upgrade file path" }
    };

    FILE *fp = tfopen("../../fms/test/results/qafmsupgrdFw2k.txt","a");

    if( fp != NULL )
    {
        fprintf(fp,"\n\n<UpgradeFirmware2k>\n");

        int tstnum = 0;
        int rc;
        int tstcnt = sizeof(UpgrdFw2kTCs)/sizeof(UpgrdFw2kTC_t);
        while ( tstnum < tstcnt )
        {
            fprintf(fp,"\nTest Case %d / %d : %s\n",
                    (tstnum + 1),
                    tstcnt,
                    UpgrdFw2kTCs[tstnum].desc);

            rc = UpgradeFirmware2k( UpgrdFw2kTCs[tstnum].path );

            /* Display result code and text */
            doprintreason (fp, rc);

            tstnum++;
        }
    }

    if( fp != NULL )
       tfclose(fp);
}

void doSLQSGetImageInfo( void )
{
    BYTE   tCaseNum = 0;
    ULONG  nRet;
    FILE   *fp=NULL;
    struct qmifwinfo_s slqsImageInfo;
    BYTE stringsize = 128;
    CHAR ModelId[stringsize];
    ULONG  resultCode;
    CHAR *pstr = NULL;

    resultCode = GetModelID(stringsize,
                            ModelId );
    if( eQCWWAN_ERR_NONE != resultCode )
    {
        fprintf( stderr, "Failed to get device info" );
        doprintreason( stderr, resultCode );
        return;
    }

    /* Define the test cases to be executed */
    SlqsGetImageInfoTestCase_t
        SlqsGetImageInfoTestCases[MAX_SLQS_GET_IMAGE_INFO_TESTCASE_NUM] =
    {
        { eQCWWAN_ERR_INVALID_ARG,  1, "/home/",         NULL,
            "Sending OUT param as NULL" },
        { eQCWWAN_ERR_SWIIM_INVALID_PATH, 2, "/home/unknown/", &slqsImageInfo,\
            "Sending Invalid path" },
        { eQCWWAN_ERR_SWIIM_FILE_NOT_FOUND, 3, "/home/slqs/testim/", &slqsImageInfo,\
            "Sending Invalid path : No Firmware image exist" },
        { eQCWWAN_ERR_SWIIM_CORRUPTED_FW_IMAGE, 4, "/home/slqs/testim_corrupt/",\
             &slqsImageInfo, "Sending path of Corrupted firmware image" },
        { eQCWWAN_ERR_NONE, 5, "./FirmwareImages/",\
             &slqsImageInfo, "All Valid" }
    };

    /* Define the test cases to be executed */
    GetImageInfoTestCase_t
          GetImageInfoTestCases[MAX_GET_IMAGE_INFO_TESTCASE_NUM] =
    {
        { eQCWWAN_ERR_NONE, 0,\
                       "./tstimgs/0", &slqsImageInfo,\
                       "valid image - 0, no trailing forward slash" },
        { eQCWWAN_ERR_NONE, 1,\
                       "./tstimgs/1/", &slqsImageInfo,\
                       "valid image - 1" },
        { eQCWWAN_ERR_NONE, 2,\
                       "./tstimgs/2/", &slqsImageInfo,\
                       "valid image - 2" },
        { eQCWWAN_ERR_NONE, 3,\
                       "./tstimgs/3/", &slqsImageInfo,\
                       "valid image - 3" },
        { eQCWWAN_ERR_NONE, 4,\
                       "./tstimgs/4/", &slqsImageInfo,\
                       "valid image - 4" },
        { eQCWWAN_ERR_NONE, 6,\
                       "./tstimgs/6/", &slqsImageInfo,\
                       "valid image - 6" },
        { eQCWWAN_ERR_NONE, 7,\
                       "./tstimgs/7/", &slqsImageInfo,\
                       "valid image - 7" },
        { eQCWWAN_ERR_NONE, 8,\
                       "./tstimgs/8/", &slqsImageInfo,\
                       "valid image - 8" },
        { eQCWWAN_ERR_NONE, 9,\
                       "./tstimgs/9/", &slqsImageInfo,\
                       "valid image - 9" },
        { eQCWWAN_ERR_SWIIM_CORRUPTED_FW_IMAGE, 10,
                       "./tstimgs/10/", &slqsImageInfo,\
                       "corrupt image - 10" },
        { eQCWWAN_ERR_SWIIM_FILE_NOT_FOUND, 11,
                       "./tstimgs/11/", &slqsImageInfo,\
                       "no uqcn.mbn file" },
        { eQCWWAN_ERR_SWIIM_INVALID_PATH, 12,
                       "./tstimgs/12/", &slqsImageInfo,\
                       "Invalid path" },
    };

    fp = tfopen("../../fms/test/results/slqsGetImageInfo.txt","w");

    if (fp)
    {
        pstr = strstr (ModelId, "MC77");
        if (pstr != NULL)
        {
            while ( tCaseNum < MAX_SLQS_GET_IMAGE_INFO_TESTCASE_NUM )
            {
                fprintf(fp, "\nTest Case %d / %d : %s\n",
                            (tCaseNum+ 1),
                            MAX_SLQS_GET_IMAGE_INFO_TESTCASE_NUM,
                            SlqsGetImageInfoTestCases[tCaseNum].desc);

                if( NULL != SlqsGetImageInfoTestCases[tCaseNum].pInfo )
                {
                    strcpy(SlqsGetImageInfoTestCases[tCaseNum].\
                           pInfo->dev.s.appversion_str, "");
                    strcpy(SlqsGetImageInfoTestCases[tCaseNum].\
                           pInfo->dev.s.bootversion_str, "");
                    strcpy(SlqsGetImageInfoTestCases[tCaseNum].\
                           pInfo->dev.s.carrier_str, "");
                    strcpy(SlqsGetImageInfoTestCases[tCaseNum].\
                           pInfo->dev.s.modelid_str, "");
                    strcpy(SlqsGetImageInfoTestCases[tCaseNum].\
                           pInfo->dev.s.packageid_str, "");
                    strcpy(SlqsGetImageInfoTestCases[tCaseNum].\
                           pInfo->dev.s.priversion_str, "");
                    strcpy(SlqsGetImageInfoTestCases[tCaseNum].\
                           pInfo->dev.s.sku_str, "");
                }

                nRet = SLQSGetImageInfo(
                           SlqsGetImageInfoTestCases[tCaseNum].path,
                           SlqsGetImageInfoTestCases[tCaseNum].pInfo );

                /* Display result code and text */
                doprintreason (fp, nRet);

                if ( !nRet )
                {
                    fprintf(fp, "SlqsGetImageInfo Successful\n");
                    fprintf(fp, "Model Id: %s\n",
                                       slqsImageInfo.dev.s.modelid_str);
                    fprintf(fp, "Boot Version: %s\n",
                                       slqsImageInfo.dev.s.bootversion_str);
                    fprintf(fp, "Application Version: %s\n",
                                       slqsImageInfo.dev.s.appversion_str);
                    fprintf(fp, "SKU Id: %s\n",
                                       slqsImageInfo.dev.s.sku_str);
                    fprintf(fp, "Package Id: %s\n",
                                       slqsImageInfo.dev.s.packageid_str);
                    fprintf(fp, "Carrier: %s\n",
                                       slqsImageInfo.dev.s.carrier_str);
                    fprintf(fp, "PRI Version: %s\n",
                                       slqsImageInfo.dev.s.priversion_str);
                }

                tCaseNum++;
            }
        }
        else
        {
            while ( tCaseNum < MAX_GET_IMAGE_INFO_TESTCASE_NUM )
            {
                fprintf(fp,"\nTest Case %d / %d : %s\n",
                        (tCaseNum+ 1),
                        MAX_GET_IMAGE_INFO_TESTCASE_NUM,
                        GetImageInfoTestCases[tCaseNum].desc);

                slqsImageInfo.dev.g.FirmwareID = 0;
                slqsImageInfo.dev.g.Technology = 0;
                slqsImageInfo.dev.g.Carrier = 0;
                slqsImageInfo.dev.g.Region = 0;
                slqsImageInfo.dev.g.GPSCapability = 0;

                nRet = SLQSGetImageInfo( GetImageInfoTestCases[tCaseNum].path,
                                         GetImageInfoTestCases[tCaseNum].pInfo );

                /* Display result code and text */
                doprintreason (fp, nRet);

                if ( !nRet )
                {
                    fprintf(fp, "Firmware id:   0x%lx\n", slqsImageInfo.dev.g.FirmwareID);
                    fprintf(fp, "technology:    %lu\n", slqsImageInfo.dev.g.Technology);
                    fprintf(fp, "Carrier :      %lu\n", slqsImageInfo.dev.g.Carrier);
                    fprintf(fp, "region:        %lu\n", slqsImageInfo.dev.g.Region);
                    fprintf(fp, "gPSCapability: %lu\n", slqsImageInfo.dev.g.GPSCapability);
                }
                tCaseNum++;
            }
       }
   }
   if (fp)
       tfclose(fp);
}

/**
 * Name:    doGetImagesPreference
 *
 * Purpose: Perform the tests that call the GetImagesPreference() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doGetImagesPreference( void )
{
    BYTE  tCaseNum = 0, idx = 0, i = 0;
    ULONG nRet;
    FILE  *fp = NULL;
    struct PrefImageList *pImageList;
    fp = tfopen("../../fms/test/results/getimagespreference.txt", "w");

    if (fp)
    {
        while ( tCaseNum < MAX_GET_IMAGES_PREFERENCE_TESTCASE_NUM )
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_GET_IMAGES_PREFERENCE_TESTCASE_NUM,
                        GetImagesPrefTestCases[tCaseNum].desc);

            pImageList = GetImagesPrefTestCases[tCaseNum].pImageList;

            nRet = GetImagesPreference(
                         GetImagesPrefTestCases[tCaseNum].pImageListSize,
                         (BYTE *)pImageList );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "GetImagesPreference Successful\n");

                fprintf(fp, "ImageList Size:: %ld\n",
                        *GetImagesPrefTestCases[tCaseNum].pImageListSize);
                fprintf(fp, "Image Element Count:: %d",
                            pImageList->listSize);
                for ( idx = 0;
                      idx < pImageList->listSize;
                      idx++ )
                {
                    fprintf(fp, "\n\n\tImage Type:: %d\n",
                                pImageList->listEntries[idx].imageType);
                    fprintf(fp, "\tImage Id::");
                    for ( i = 0; i < 16; i++ )
                        fprintf(fp, "%x",
                                    pImageList->listEntries[idx].imageId[i]);
                    fprintf(fp, "\n\tBuild Id Length:: %d\n",
                                pImageList->listEntries[idx].buildIdLength);
                    fprintf(fp, "\tBuild Id :: ");
                    for ( i = 0;
                          i < pImageList->listEntries[idx].buildIdLength;

                          i++ )
                        fprintf(fp, "%c",
                                    pImageList->listEntries[idx].buildId[i]);
                }
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doSetImagesPreference
 *
 * Purpose: Perform the tests that call the SetImagesPreference() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSetImagesPreference( void )
{
    BYTE  tCaseNum = 0, idx = 0, i = 0;
    ULONG nRet;
    FILE  *fp = NULL;
    struct PrefImageList imageList;
    BYTE imageId[2][16] = { {0x02, 0x5A, 0xFA, 0xED, 0xB7, 0x0D,
                            0xCF, 0x61, 0x13, 0xB0, 0x2C, 0x22,
                            0x5E, 0x42, 0x71, 0x1C},
                           {0x04, 0x2B, 0xE8, 0x3A, 0x34, 0xD1,
                            0x06, 0xB1, 0x0A, 0xEB, 0x03, 0x67,
                            0xAE, 0xA1, 0x17, 0x62} };
    BYTE buildId[2][18] = { {0x44, 0x33, 0x32, 0x30, 0x30, 0x2D,
                             0x53, 0x54, 0x53, 0x55, 0x47, 0x4E,
                             0x5A, 0x2D, 0x31, 0x35, 0x37, 0x35},
                            {0x44, 0x33, 0x32, 0x30, 0x30, 0x2D,
                             0x53, 0x54, 0x53, 0x55, 0x47, 0x4E,
                             0x5A, 0x2D, 0x31, 0x35, 0x37, 0x35} };

    imageList.listSize = 0x02;

    imageList.listEntries[0].imageType = 0x00;
    for ( idx = 0; idx < 16; idx++ )
        imageList.listEntries[0].imageId[idx] = imageId[0][idx];
    imageList.listEntries[0].buildIdLength = 0x12;
    for ( idx = 0; idx < 18; idx++ )
        imageList.listEntries[0].buildId[idx] = buildId[0][idx];

    imageList.listEntries[1].imageType = 0x01;
    for ( idx = 0; idx < 16; idx++ )
        imageList.listEntries[1].imageId[idx] = imageId[1][idx];
    imageList.listEntries[1].buildIdLength = 0x12;
    for ( idx = 0; idx < 18; idx++ )
        imageList.listEntries[1].buildId[idx] = buildId[1][idx];

    fp = tfopen("../../fms/test/results/setimagespreference.txt", "w");

    if (fp)
    {
        while ( tCaseNum < MAX_SET_IMAGES_PREFERENCE_TESTCASE_NUM )
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SET_IMAGES_PREFERENCE_TESTCASE_NUM,
                        SetImagesPrefTestCases[tCaseNum].desc);
            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Image Element Count:: %d",
                        imageList.listSize);
            for ( idx = 0;
                  idx < imageList.listSize;
                  idx++ )
            {
                fprintf(fp, "\n\n\tImage Type:: %d\n",
                            imageList.listEntries[idx].imageType);
                fprintf(fp, "\tImage Id::");
                for ( i = 0; i < 16; i++ )
                    fprintf(fp, "%x",
                                imageList.listEntries[idx].imageId[i]);
                fprintf(fp, "\n\tBuild Id Length:: %d\n",
                            imageList.listEntries[idx].buildIdLength);
                fprintf(fp, "\tBuild Id :: ");
                for ( i = 0;
                      i < imageList.listEntries[idx].buildIdLength;
                      i++ )
                    fprintf(fp, "%c",
                                imageList.listEntries[idx].buildId[i]);
            }

            nRet = SetImagesPreference(
                             SetImagesPrefTestCases[tCaseNum].imageListSize,
                             (BYTE *)&imageList,
                             SetImagesPrefTestCases[tCaseNum].bForceDownload,
                             SetImagesPrefTestCases[tCaseNum].modemIndex,
                             SetImagesPrefTestCases[tCaseNum].pImageTypesSize,
                             SetImagesPrefTestCases[tCaseNum].pImageTypes );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "SetImagesPreference Successful\n");
                fprintf(fp, "Images Types size:: %ld\n",
                        *SetImagesPrefTestCases[tCaseNum].pImageTypesSize);
                for ( idx = 0;
                      idx < *SetImagesPrefTestCases[tCaseNum].pImageTypesSize;
                      idx++ )
                {
                    fprintf(fp, "Images Type[%d]:: %d\n", idx,
                            SetImagesPrefTestCases[tCaseNum].pImageTypes[idx]);
                }
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

void doGetStoredImages( void )
{
    BYTE                  tCaseNum         = 0;
    BYTE                  idx1             = 0, idx2 =0, idx3 = 0;
    BYTE                  imageIdEntries   = 0;
    ULONG                 nRet, imageListsize;
    FILE                  *fp              = NULL;
    BYTE                  bufImageList[1024];
    struct ImageIdElement *pImageIdElement = NULL;
    struct ImageList      imageList, *pImageList = NULL;

    /* Define the test cases to be executed */
    const GetStoredImagesTestCase_t \
        GetStoredImagesTestCases[ MAX_GET_STORED_IMAGES_TESTCASE_NUM] =
    {
        { eQCWWAN_ERR_INVALID_ARG, 1, NULL,                   10,
                                       "Sending Buffer as NULL" },
        { eQCWWAN_ERR_BUFFER_SZ,   2, bufImageList,          100,
                                       "Sending buffer smaller than required" },
        { eQCWWAN_ERR_NONE,        3, (BYTE *)&imageList,   4308,
                                       "All Valid : Sending Struct ptr" }
    };

   fp = tfopen("../../fms/test/results/getstoredimages.txt","w" );
   if (fp)
   {
        while ( tCaseNum < MAX_GET_STORED_IMAGES_TESTCASE_NUM )
        {
            fprintf( fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum+ 1),
                        MAX_GET_STORED_IMAGES_TESTCASE_NUM,
                        GetStoredImagesTestCases[tCaseNum].desc );

            imageListsize = GetStoredImagesTestCases[tCaseNum].imageListsize;
            fprintf(fp, "Size of the User Buffer: %lu\n", imageListsize );

            /* Call the API */
            nRet =  GetStoredImages( &imageListsize,
                                     (BYTE *)GetStoredImagesTestCases[tCaseNum].
                                             pImageList );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "GetStoredImage Successful\n" );

                pImageList = (struct ImageList *)GetStoredImagesTestCases\
                                                 [tCaseNum].pImageList;

                fprintf(fp, "Total Bytes Copied in Buf: %lu\n", imageListsize );
                fprintf(fp, "Image List Size: %d\n", pImageList->listSize );
                fprintf(fp, " Image List Entries:\n" );
                for( idx1 = 0; idx1 < pImageList->listSize; idx1++ )
                {
                     fprintf(fp, "List Entry %d:\n", idx1 + 1 );
                     fprintf(fp, "  Image Type: %d\n",
                             pImageList->imageIDEntries[idx1].imageType );
                     fprintf(fp, "  Max Images: %d\n",
                             pImageList->imageIDEntries[idx1].maxImages );
                     fprintf(fp, "  Executing Image Index: %x\n",
                             pImageList->imageIDEntries[idx1].executingImage );
                     fprintf(fp, "  No. of Elements in Image ID List: %x\n",
                             pImageList->imageIDEntries[idx1].imageIDSize );
                     imageIdEntries =
                             pImageList->imageIDEntries[idx1].imageIDSize ;

                     fprintf(fp, "\tImage ID List Elements:\n" );
                     for ( idx2 = 0; idx2 < imageIdEntries; idx2++ )
                     {
                          pImageIdElement = ( struct ImageIdElement *)
                                            &pImageList->imageIDEntries[idx1].
                                            imageIDElement[idx2];
                          fprintf(fp, "\tImage ID List Element No.: %d\n",
                                      idx2 + 1 );
                          fprintf(fp, "\tStorage Index: 0x%x\n",
                                      pImageIdElement->storageIndex );
                          fprintf(fp, "\tfailure_count: 0x%x\n",
                                      pImageIdElement->failureCount );
                          fprintf(fp, "\tImage ID: " );
                          for ( idx3 = 0; idx3 < 16; idx3++ )
                          {
                               fprintf(fp, "0x%x ",
                                            pImageIdElement->imageID[idx3] );
                          }
                          fprintf(fp, "\n");
                          fprintf(fp, "\tBuild ID Length: %d\n",
                                      pImageIdElement->buildIDLength );
                          if( pImageIdElement->buildIDLength)
                          {
                              fprintf(fp, "\tBuild ID:%s\n",
                                          pImageIdElement->buildID );
                          }
                          fprintf(fp, "\n");
                     }
                     fprintf(fp, "\n");
                }
            }
            tCaseNum++;
        }
   }
   if(fp)
      tfclose(fp);

}

void doExecuteFmsStubs()
{
     doGetImageStore();
     doSLQSGetFirmwareInfo();

}

/* Define the test cases to be executed */
static SLQSUpgrdfwT_t SlqsUpgrdFwTCs =
{
    0,
    {
        { eQCWWAN_ERR_NONE,
          1,
          "./tstimgs/1",
          "valid firmware upgrade file path: ./tstimgs/1" },
        { eQCWWAN_ERR_NONE,
          2,
          "./tstimgs/2",
          "valid firmware upgrade file path (no AMSS): ./tstimgs/2" },
        { eQCWWAN_ERR_NONE,
          3,
          "./tstimgs/3/",
          "valid firmware upgrade file path: ./tstimgs/3/" },
        { eQCWWAN_ERR_NONE,
          4,
          "./tstimgs/4",
          "valid firmware upgrade file path (no AMSS): ./tstimgs/4" },
        { eQCWWAN_ERR_NONE,
          5,
          "./tstimgs/5",
          "invalid firmware upgrade file path: ./tstimgs/5" },
        { eQCWWAN_ERR_NONE,
          6,
          "./tstimgs/6",
          "valid firmware upgrade file path: ./tstimgs/6" },
        { eQCWWAN_ERR_NONE,
          7,
          "./tstimgs/7",
          "valid firmware upgrade file path (no AMSS): ./tstimgs/7" },
        { eQCWWAN_ERR_NONE,
          8,
          "./tstimgs/8",
          "valid firmware upgrade file path (no AMSS): ./tstimgs/8" },
        { eQCWWAN_ERR_NONE,
          9,
          "./tstimgs/9",
          "valid firmware upgrade file path (no AMSS): ./tstimgs/9" },
        { eQCWWAN_ERR_NONE,
          10,
          "./tstimgs/10",
          "non-existent firmware upgrade file path: ./tstimgs/10" }
    },
    PTHREAD_COND_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    FALSE
};

static void doSLQSUpgradeFwCBErrorHdlr( const ULONG nRet, FILE *fp)
{
    fprintf(fp, "\n\nReturn Code: %lu\n", nRet);
    doprintreason( fp, nRet );
}

static void doSLQSUpgradeFwDisplayFw(FILE *fp)
{
    CHAR amss[128];
    CHAR boot[128];
    CHAR pri[128];

    if( eQCWWAN_ERR_NONE ==
            GetFirmwareRevisions( (BYTE)sizeof(amss),
                                  amss,
                                  (BYTE)sizeof(boot),
                                  boot,
                                  (BYTE)sizeof(pri),
                                  pri ) )
    {
        if( NULL != fp )
            fprintf( fp, "AMSS: %s\nBOOT: %s\nPRI: %s\n", amss, boot, pri);
    }
    else
    {
        fprintf( fp, "AMSS: %s\nBOOT: %s\nPRI: %s\n", "", "", "");
    }
}

static void doSLQSUpgradeFwCB(eDevState devstate)
{
    static const ULONG NumOfTests =
        sizeof(SlqsUpgrdFwTCs.TestCase)/sizeof(SlqsUpgrdFwTCs.TestCase[0]);

    fprintf(stdout, "DevStatChangeCB received: State = %d\n", devstate);

    if( DEVICE_STATE_READY == devstate )
    {
        if( SlqsUpgrdFwTCs.TestCaseIdx < NumOfTests )
        {
            FILE *fp = NULL;
            fp = tfopen("../../fms/test/results/slqsupgradefirmware.txt","a");

            fprintf( fp,
                     "\nTest Case %lu / %d : %s\n",
                     SlqsUpgrdFwTCs.TestCaseIdx + 1,
                     MAX_SLQS_UPGRADE_FIRMWARE_TESTCASE_NUM,
                     SlqsUpgrdFwTCs.TestCase[SlqsUpgrdFwTCs.TestCaseIdx].desc);

            ULONG nRet;
            /* executing firmware before download */
            doSLQSUpgradeFwDisplayFw(fp);
            nRet = UpgradeFirmware2k( SlqsUpgrdFwTCs.TestCase[SlqsUpgrdFwTCs.TestCaseIdx].path );
            doSLQSUpgradeFwCBErrorHdlr(nRet, fp);

            SlqsUpgrdFwTCs.TestCaseIdx++;
            if( !nRet )
            {
                fprintf(fp, "SLQSUpgradeFirmware Successful\n");
            }
            else
            {
                SetPower(DEVICE_RESET);
            }
        }
        else
        {
            /* display firmware information for last test case */
            FILE *fp = NULL;
            fp = tfopen("../../fms/test/results/slqsupgradefirmware.txt","a");
            fprintf( fp,
                     "\nTest Case %lu / %d : %s\n",
                     SlqsUpgrdFwTCs.TestCaseIdx,
                     MAX_SLQS_UPGRADE_FIRMWARE_TESTCASE_NUM,
                     SlqsUpgrdFwTCs.TestCase[SlqsUpgrdFwTCs.TestCaseIdx].desc);

            doSLQSUpgradeFwDisplayFw(fp);

            /* update shared data */
            pthread_mutex_lock(&SlqsUpgrdFwTCs.mutex);
            SlqsUpgrdFwTCs.wait = FALSE;
            pthread_mutex_unlock(&SlqsUpgrdFwTCs.mutex);
            pthread_cond_signal(&SlqsUpgrdFwTCs.cond);

            if( eQCWWAN_ERR_NONE != SetDeviceStateChangeCbk(NULL) )
            {
                fprintf(stderr, "Could not deregister from device state change callback\n");
                pthread_exit(NULL);
            }

        }
    }
}

local void *doSLQSUpgradeFwThread( void *arg )
{
    UNUSEDPARAM( arg );

    if( eQCWWAN_ERR_NONE != SetDeviceStateChangeCbk(doSLQSUpgradeFwCB) )
    {
        fprintf(stderr, "Could not register for device state change callback\n");
        pthread_exit(NULL);
    }
    /* initialize synchronization variables prior to resetting device to
     * prevent any possibility of a race condition whereby a pthread_signal
     * would is executed on uninitialized data
     */
    pthread_mutex_init(&SlqsUpgrdFwTCs.mutex, NULL);
    pthread_cond_init(&SlqsUpgrdFwTCs.cond, NULL);

    /* update share data */
    pthread_mutex_lock(&SlqsUpgrdFwTCs.mutex);
    SlqsUpgrdFwTCs.wait = TRUE;
    pthread_mutex_unlock(&SlqsUpgrdFwTCs.mutex);

    SetPower( DEVICE_RESET ); /* trigger the firmware upgrade process */
    int rc;
    while( SlqsUpgrdFwTCs.wait ){
        rc = pthread_cond_wait(&SlqsUpgrdFwTCs.cond, &SlqsUpgrdFwTCs.mutex );
        if( 0 != rc )
            fprintf(stderr, "pthread_cond_wait rc = %d", rc);
        else
            break;
    }

    SlqsUpgrdFwTCs.TestCaseIdx = 0;

    /* free mutex and condition var. resources */
    pthread_mutex_destroy(&SlqsUpgrdFwTCs.mutex);
    pthread_cond_destroy(&SlqsUpgrdFwTCs.cond);
    pthread_exit(NULL);

    /* Although the return statement below will never be reached, Android fails
     * to compile without an explicit return statement.
     */
    return NULL;
}

void doSLQSUpgradeGobiFw( void )
{
    pthread_t thread;
    void *pvalue;
    int niterations = 10;

    while( niterations--){
        /* initialize and set thread detached attribute */
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        /* create thread */
        pthread_create( &thread, NULL, doSLQSUpgradeFwThread, NULL );
        /* free attribute resources */
        pthread_attr_destroy(&attr);
        /* wait for thread to complete execution */
        pthread_join(thread, &pvalue);
    }
}

enum imaget_type{
    IMAGE_TYPE_MODEM,
    IMAGE_TYPE_PRI,
};

/**
 * Name:    doDeleteStoredImage
 *
 * Purpose: Retrieves the image list from the modem and deletes the image
 *          from the modem which is selected by user.
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doDeleteStoredImage( void )
{
    BYTE idx = 0, idx1 = 0, idx2 = 0, totalImgs = 0, imageIdEntries = 0;
    BYTE imgType = 0;
    int imageIndex = 0;
    ULONG nRet,imageListsize = 0;
    struct ImageElement   imageInfo;
    struct ImageIdElement *pImageIdElement = NULL;
    struct ImageList      imageList;
    char buf[3];

    imageListsize = sizeof( imageList );
    slmemset( (char *)&imageList, 0, imageListsize);

    /* Get the images stored on the device */
    nRet =  GetStoredImages( &imageListsize, (BYTE *)&imageList );

    /* If image retrieving failed, return */
    if ( nRet || ( 0 == imageList.listSize) )
    {
        printf("IMAGE RETREIVING FAILED : %lu\n",nRet );
        doprintreason( stderr, nRet );
        return;
    }

    /* Display the retrieved Image List */
    printf("Image Index\t  Image Type\t   Image ID\n");

    for( idx = 0; idx < imageList.listSize; idx++ )
    {
        imageIdEntries = imageList.imageIDEntries[idx].imageIDSize;

        for ( idx1 = 0; idx1 < imageIdEntries; idx1++ )
        {
            pImageIdElement = ( struct ImageIdElement *)
                                &imageList.imageIDEntries[idx].
                                imageIDElement[idx1];
            printf("%6d", ++totalImgs );
            printf("%18d", imageList.imageIDEntries[idx].imageType );
            printf("%13.2x ", pImageIdElement->imageID[0] );
            for ( idx2 = 1; idx2 < 16; idx2++ )
            {
                 printf("%.2x ", pImageIdElement->imageID[idx2] );
            }
            printf("\n");
        }
    }
    printf("\n");

    while(1) // For recursive menu dsplay
    {
        printf("Enter 0 to EXIT or <Image Index> of Image to be deleted :  ");
        if( fgets(buf, 3, stdin) == 0)
            continue;

        imageIndex = atoi (buf);

        /* Validate the image index specified by user */
        if( 0 == imageIndex )
        {
             printf("\nEXIT selected!! Exiting..\n");
             return;
        }
        else if( ( 0 > imageIndex ) || ( totalImgs < imageIndex) )
        {
            continue;
        }

        /* By default image type is MODEM */
        imgType = IMAGE_TYPE_MODEM;

        /* If the image index is not MODEM image index */
        if( imageIndex > imageList.imageIDEntries[imgType].imageIDSize )
        {
             /* Get the image index in PRI image list */
            imageIndex -= ( imageList.imageIDEntries[imgType].imageIDSize );
            imgType = IMAGE_TYPE_PRI;
        }
        imageIndex--;

        /* Retrieve the information about the image to be deleted */
        imageInfo.imageType = imgType;
        pImageIdElement = ( struct ImageIdElement *)&imageList.\
                           imageIDEntries[imgType].imageIDElement[imageIndex];

        for ( idx = 0; idx < 16; idx++ )
        {
            imageInfo.imageId[idx] = pImageIdElement->imageID[idx];
        }

        imageInfo.buildIdLength = pImageIdElement->buildIDLength;
        slstrncpy( imageInfo.buildId,
                   pImageIdElement->buildID,
                   slstrlen( pImageIdElement->buildID ) + 1 );

        printf("\n\nDeleting Image Info:\n");
        printf("\tImage Type:: %d",imageInfo.imageType);
        if( IMAGE_TYPE_MODEM == imageInfo.imageType)
        {
            printf(" (AMSS)\n");
        }
        else
        {
            printf(" (UQCN)\n");
        }
        printf("\tImage Id::");
        for ( idx = 0; idx < 16; idx++ )
            printf("0x%x ", imageInfo.imageId[idx]);

        printf("\n\tBuild Id Length:: %d\n", imageInfo.buildIdLength);
        printf("\tBuild Id :: %s\n\n\n", imageInfo.buildId);

        /* Delete the selected image , API call */
        nRet = DeleteStoredImage( sizeof( imageInfo ),
                                  (BYTE *)&imageInfo );

       /* If image deleting failed, return */
        if ( eQCWWAN_ERR_NONE != nRet )
        {
            printf("IMAGE DELETING FAILED : %lu\n", nRet );
            doprintreason( stderr, nRet );
            return;
        }

        totalImgs = 0;
        slmemset( (char *)&imageList, 0, imageListsize);

        /* Get the images stored on the device */
        nRet =  GetStoredImages( &imageListsize, (BYTE *)&imageList );

        /* If image retrieving failed, return */
        if ( nRet || ( 0 == imageList.listSize) )
        {
            printf("IMAGE RETREIVING FAILED : %lu\n",nRet );
            doprintreason( stderr, nRet );
            return;
        }

        /* Display the retrieved Image List */
        printf(" Image Index\t  Image Type\t   Image ID\n");

        for( idx = 0; idx < imageList.listSize; idx++ )
        {
            imageIdEntries = imageList.imageIDEntries[idx].imageIDSize;

            for ( idx1 = 0; idx1 < imageIdEntries; idx1++ )
            {
                pImageIdElement = ( struct ImageIdElement *)
                                    &imageList.imageIDEntries[idx].
                                    imageIDElement[idx1];
                printf("%6d", ++totalImgs );
                printf("%18d", imageList.imageIDEntries[idx].imageType );
                printf("%13.2x ", pImageIdElement->imageID[0] );
                for ( idx2 = 1; idx2 < 16; idx2++ )
                {
                     printf("%.2x ", pImageIdElement->imageID[idx2] );
                }
                printf("\n");
            }
        }
        printf("\n");
    }//while(1) end : recursive menu dsplay
}
