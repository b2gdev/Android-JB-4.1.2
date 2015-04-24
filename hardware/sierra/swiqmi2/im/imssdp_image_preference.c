/*************
 *
 * Filename:    imssdp_image_preference.c
 *
 * Purpose:     Image management Supplementary Streaming Download Protocol
 *              support.
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/*---------------
  Include files
 ---------------*/
#include "aa/aaglobal.h"
#include "pi/piudefs.h"
#include "sl/sludefs.h"
#include "imssdp_image_preference.h"

/*---------------
  Definitions
 ---------------*/

/*---------------
  Enumerations
 ---------------*/

/*---------------
  Structures
 ---------------*/

/*---------------
  Local storage
 ---------------*/

/*---------------
  Functions
 ---------------*/
package int imssdp_get_image_preference_pack(
    void        *psrc,
    swi_uint8   *pdest )
{
    ssdp_get_image_preference_req_t *pin =
        (ssdp_get_image_preference_req_t *)psrc;

    /* parameter validation */
    if( pin == NULL || pdest == NULL )
    {
        return -1;
    }

    swi_uint8 *pbase = pdest;

    /* command */
    *pdest++ = pin->command;

    return (int)(pdest - pbase);
}

package int imssdp_get_image_preference_unpack(
    swi_uint8   *psrc,
    void        *pdest )
{
    swi_uint8 idx = 0;

    ssdp_get_image_preference_rsp_t *pout =
       (ssdp_get_image_preference_rsp_t *)pdest;


    /* parameter validation */
    if( psrc == NULL || pout == NULL )
    {
        return -1;
    }

    /* command */
    pout->command = *psrc++;

    /* number of entries in the image prefrence list */
    pout->total_entries_in_list = *psrc++;

    /* Maximum 12 image prefrences are allowed */
    if( SSDP_MAX_IMAGE_PREF < pout->total_entries_in_list )
    {
        pout->total_entries_in_list = SSDP_MAX_IMAGE_PREF;
    }

    /* get the image type and image identifier for each image prefrence */
    while( idx < pout->total_entries_in_list )
    {
        /* image type */
        pout->ssdp_img_pref[idx].image_type = *psrc++;

        /* image identifier */
        slmemcpy(   (char *)pout->ssdp_img_pref[idx].image_identifier,
                    (char *)psrc,
                    SSDP_IMAGE_ID_LEN );

        psrc += SSDP_IMAGE_ID_LEN;
        idx++;
    }

    return 0;
}
