/*************
 *
 * Filename:    imutils.c
 *
 * Purpose:     Image Management Utility functions available within im package
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#include "aa/aaglobal.h"
#include "er/erudefs.h"
#include "sl/sludefs.h"
#include "os/swi_ossdk.h"
#include "pi/piudefs.h"
#include "imidefs.h"
#include <stdio.h>

/*************
 *
 * Name:    imisfile
 *
 * Purpose: Returns file type
 *
 * Parms:   path   - pointer to full path name of file
 *
 * Return:  0  - no match
 *          1  - matched
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local swi_bool imisfile(
    const char *xtension,
    const char *path )
{
    return ( (slstrstr(path, xtension) == NULL) ? FALSE : TRUE );
}

/*************
 *
 * Name:    imgetfiletype
 *
 * Purpose: Returns file type
 *
 * Parms:   path   - pointer to full path name of file
 *
 * Return:  fw_image_type_e value
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
package enum fw_image_type_e imgetfiletype(
    const char *path )
{
    if( imisfile(".mbn", path) )
        return FW_IMG_TYPE_MBN_GOBI;

    if( imisfile(".cwe", path) )
        return FW_IMG_TYPE_CWE;

    return FW_IMG_TYPE_INVLD;
}

/*  return value:
    success: 0,
    failure: non-zero error code
*/
package enum imerrcodes_e
im_cwe_img_info_get(struct im_image_info_s *pin)
{
    UNUSEDPARAM( pin );

    return IMERRUNSUPPORTEDIMGTYPE;
}

/* search for key in buf pointed to by pbuf of size bufsz
 * return - success: pointer to byte proceeding key
 *          failure: NULL
 */

local void *
parse_gobi_img_info_buf(
    swi_uint8 *pbuf,
    swi_uint32 bufsz,
    swi_uint32 key )
{
    swi_uint8 *plocal = pbuf;
    swi_uint8 *pend = pbuf + bufsz - sizeof(swi_uint32);

    while( plocal < pend )
    {
        if( key == piget32(&plocal) )
        {
            return plocal;
        }
    }

    return NULL;
}

/*  return value:
    success:    0,
    failure:    non-zero error code
    assume:     a valid gobi mbn image path argument provided in
                structure pointed to by pimginfo
*/
package enum imerrcodes_e
im_gobi_img_info_get(struct im_image_info_s *pimginfo)
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
            slmemcpy(   pgobi->imgidstr,
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
            slmemcpy(   pgobi->imgvrsnstr,
                        plocal,
                        sizeof(pgobi->imgvrsnstr) );
        }
    }

    /* mem unmap image file */
    if( !swi_ossdk_umapfile(&map) )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d file mem unmapping failure",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0);
    }

    return err;
}
