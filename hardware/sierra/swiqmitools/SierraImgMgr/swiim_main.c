/**
 *
 * @ingroup swiim
 *
 * @file
 * main fuction of image management 
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

/* include files */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include "SwiDataTypes.h"
#define  LOG_TAG  "swi_imgmgr"
#include "swiril_log.h"
#include "swiim_img.h"
#include <swiqmitool_common.h>

#define ListImages_CMDLINE        "--list"
#define SelectImage_CMDLINE       "--select"
#define ListDeviceImages_CMDLINE  "--listdevice"
#define SelectDeviceImage_CMDLINE "--selectdevice"
#define UpdateImages_CMDLINE      "--update"
#define DownloadImages_CMDLINE    "--download"

char *configFilePath;

static void usage(char *s)
{
    fprintf(stderr,
             "usage: \n%s  --list <image directory> \t\t\tlist all available carrier images \n \
             --select <Id> <image directory> \t\tswitch to new carrier image \n \
             --update <image directory> \t\tupdate all the images currently on the modem with newer image files \n \
             --listdevice \t\t\t\tlist all available carrier images in the device \n \
             --selectdevice <Id> [<config file path>]\tswitch to new carrier image in the device;"
                                                 "NULL chosen if no config file path provided \n", s);
    exit(-1);
}

/* search for key in buf pointed to by pbuf of size bufsz
 * return - success: pointer to byte proceeding key
 *          failure: NULL
 */

static void *parse_gobi_img_info_buf(
    swi_uint8 *pbuf,
    swi_uint32 bufsz,
    swi_uint32 key )
{
    swi_uint8 *plocal = pbuf;

    while( plocal < (pbuf + bufsz) )
    {
        if( key == piget32(&plocal) )
        {
            return plocal;
        }
    }

    return NULL;
}


int im_gobi_img_info_get(struct im_image_info_s *pimginfo)
{
    struct swi_osfilestats map;
    enum imerrcodes_e err = IMERRNONE;

    /* determine file size */
    struct swi_osfile fs;
    if( FALSE == swi_ossdkfileopenread(&fs, pimginfo->pimgpath) )
    {
        return IMERRSYSTEM;
    }

    swi_size_t fsize;
    if( FALSE == swi_ossdkfilesize(&fs, &fsize) )
    {
        return IMERRSYSTEM;
    }

    /* set file offset and file size */
    map.vmapfileoffset = fsize - GOBI_MBN_IMG_INFO_BUF_SIZE;
    map.filesize = GOBI_MBN_IMG_INFO_BUF_SIZE;

    /* memory map image file */
    if( FALSE == swi_ossdk_mmapro(pimginfo->pimgpath, &map) )
    {
        return IMERRSYSTEM;
    }

    /* extract image information */
    swi_uint8 *plocal;
    struct gobi_mbn_img_info_s *pgobi;
    pgobi = (struct gobi_mbn_img_info_s *)&pimginfo->imginfo.gobiinfo;

    /* UQCN file */
    plocal = parse_gobi_img_info_buf(   (swi_uint8 *)map.pvmap,
                                        GOBI_MBN_IMG_INFO_BUF_SIZE,
                                        UQCN_INFO_MAGIC );

    /* UQCN file - yes */
    if( NULL != plocal )
    {
        /* set mbn image type */
        pgobi->type = MBN_IMG_TYPE_UQCN;

        /* extract Gobi MBN version id */
        pgobi->versionid = piget32(&plocal);
    }

    /* UQCN file - no */
    if( NULL == plocal )
    {
        /* AMSS file */
        plocal = parse_gobi_img_info_buf(   (swi_uint8 *)map.pvmap,
                                            GOBI_MBN_IMG_INFO_BUF_SIZE,
                                            MBN_BOOT_MAGIC );
        /* AMSS file - yes */
        if( NULL != plocal )
        {
            /* set mbn image type */
            pgobi->type = MBN_IMG_TYPE_AMSS;

            /* extract Gobi MBN version id */
            swi_uint32 versionid = piget16(&plocal); /* major ver id */
            versionid = piget16(&plocal); /* minor ver id */
            pgobi->versionid = versionid;
        }
        else
        {
            /* Not one of the supported gobi mbn image types (UQCN, AMSS) */
            pgobi->type = MBN_IMG_TYPE_UNSUPPORTED;
            err = IMERRCORRUPTIMAGE;
        }
    }

    if( IMERRNONE == err )
    {
        /* image id string */
        plocal = parse_gobi_img_info_buf(   (swi_uint8 *)map.pvmap,
                                            GOBI_MBN_IMG_INFO_BUF_SIZE,
                                            MBN_HASH_MAGIC );
        if( NULL == plocal )
        {
            err = IMERRCORRUPTIMAGE;
        }
        else
        {
            memcpy( pgobi->imgidstr,
                    plocal,
                    sizeof(pgobi->imgidstr) );
        }
    }

    if( IMERRNONE == err )
    {
        /* (build id) version string */
        plocal = parse_gobi_img_info_buf(   (swi_uint8 *)map.pvmap,
                                            GOBI_MBN_IMG_INFO_BUF_SIZE,
                                            MBN_BUILD_MAGIC );
        if( NULL == plocal )
        {
            err = IMERRCORRUPTIMAGE;
        }
        else
        {
            memcpy( pgobi->imgvrsnstr,
                    plocal,
                    sizeof(pgobi->imgvrsnstr) );
        }
    }

    /* mem unmap image file */
    if( !swi_ossdk_umapfile(&map) )
    {
        exit(-1);
    }

    return err;
}

/**
 *
 * main function for image management
 *
 * @param argc
 *     argument count
 * @param[in] argv 
 *     Pointer to a array of arguments
 *
 * @return
 *     exit code
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */
int main(int argc, char **argv)
{
    swi_bool fListDeviceImage = FALSE;
    swi_bool fSelectDeviceImage = FALSE;
    swi_bool fListImage = FALSE;
    swi_bool fSelectImage = FALSE;
    swi_bool fUpdateImage = FALSE;
    char     chImagePath[MAXPATHSIZE] = "";
    int      iSelectID = 0;


    if (argc == 2)
    {
        if (0 == strcmp(argv[1], ListDeviceImages_CMDLINE))
        {
            fListDeviceImage = TRUE;
        }
        else
        {
            usage(argv[0]);
            return 1;
        }
    }
    else if (argc == 3) 
    {
        if (0 == strcmp(argv[1], ListImages_CMDLINE))
        {
            fListImage = TRUE;
            strcpy((char *)chImagePath, argv[2]);
        }
        else if (0 == strcmp(argv[1], UpdateImages_CMDLINE))
        {
            fUpdateImage = TRUE;
            strcpy((char *)chImagePath, argv[2]);
        }
        else if (0 == strcmp(argv[1], SelectDeviceImage_CMDLINE))
        {
            fSelectDeviceImage = TRUE;
            iSelectID = atoi(argv[2]);
            configFilePath = NULL;
        }
        else
        {
            usage(argv[0]);
            return 1;
        }
    }
    else if (argc == 4) 
    {
        if (0 == strcmp(argv[1], SelectImage_CMDLINE))
        {
            fSelectImage = TRUE;
            iSelectID = atoi(argv[2]);
            strcpy((char *)chImagePath, argv[3]);
        }
        else if (0 == strcmp(argv[1], SelectDeviceImage_CMDLINE))
        {
            fSelectDeviceImage = TRUE;
            iSelectID = atoi(argv[2]);
            configFilePath = argv[3];
        }
        else
        {
            usage(argv[0]);
            return 1;
        }
    }
    else
    {
        usage(argv[0]);
        return 1;
    }

    qmiDeviceConnect();

    if(fListImage == TRUE)
    {
        ListImage(chImagePath);
    }
 
    if(fSelectImage == TRUE)
    {
        if(SelectImage(iSelectID, chImagePath))
            printf("Select Success\r\n");
        else
            printf("Select Failed\r\n");
    }

    if(fUpdateImage == TRUE)
    {
        if(UpdateAllImages(chImagePath))
            printf("Update Success\r\n");
        else
            printf("Update Failed\r\n");
    }

    if(fListDeviceImage == TRUE)
    {
        ListDeviceImages( TRUE );
    }

    if(fSelectDeviceImage == TRUE)
    {
        if (SelectDeviceImage(iSelectID))
            printf("Select Success\n");
        else
            printf("Select Failed\n");
    }
    
    qmiDeviceDisconnect();

    exit(0);
}

