/*************
 *
 * Filename:    imuser.c
 *
 * Purpose:     Image Management Utility functions available to other packages
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

#include "aa/aaglobal.h"
#include "os/swi_ossdk.h"
#include "imidefs.h"

/*  return value:
    success: 0,
    failure: non-zero error code
*/
global enum imerrcodes_e
imuser_image_info_get(struct im_image_info_s *pin)
{
    /* validate path */
    if( 0 != swi_ossdk_stat(pin->pimgpath) )
    {
        return IMERRINVLDPATH; /* invalid path */
    }

    /* determine image type */
    pin->imgtype = imgetfiletype(pin->pimgpath);

    /* extract image information */
    switch( pin->imgtype )
    {
        case FW_IMG_TYPE_CWE:
            return im_cwe_img_info_get(pin);
            break;

        case FW_IMG_TYPE_MBN_GOBI:
            return im_gobi_img_info_get(pin);
            break;

        case FW_IMG_TYPE_MBN:
        default:
            return IMERRUNSUPPORTEDIMGTYPE; /* unsupported image type */
    }
}
