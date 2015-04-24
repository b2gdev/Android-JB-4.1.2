/*************
 *
 * Filename:    imssdp_open_unframed.c
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
#include "imssdp_open_unframed.h"

/*---------------
  Definitions
 ---------------*/
#define     SIERRA_CWE_IMAGE    0x80

#define MIN(a,b)    a < b ? a : b;

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
package int imssdp_open_unframed_pack(
    void        *psrc,
    swi_uint8   *pdest )
{
    ssdp_open_unframed_req_s *pin = (ssdp_open_unframed_req_s *)psrc;

    /* parameter validation */
    if( pin == NULL || pdest == NULL )
    {
        return -1;
    }

    swi_uint8 *pbase = pdest;

   /* command */
    *pdest++ = pin->command;

    /* image mode in which to open */
    *pdest++ = pin->image_mode;

    /* length of image */
    piput32( pin->image_length, &pdest );

    /* window size */
    *pdest++ = pin->window_size;

    /* max length of payload */
    piput32( pin->max_payload_length, &pdest );

    /* feature flag */
    piput16( pin->feature_flag, &pdest );

    /* image header */
    if(SIERRA_CWE_IMAGE == pin->image_mode)
    {
/* TBD       swi_uint32 len = MIN(   SSDP_OPEN_UNFRAMED_PKT_CWE_HEADER_SZ,
                                sizeof(pin->pimage_header_payload)   ); */
        slmemcpy( (char *)pdest,
                  (char *)pin->pimage_header_payload,
                  SSDP_OPEN_UNFRAMED_PKT_CWE_HEADER_SZ); //len );

        pdest += SSDP_OPEN_UNFRAMED_PKT_CWE_HEADER_SZ;
    }

    return (int)(pdest - pbase);
}

package int imssdp_open_unframed_unpack(
    swi_uint8   *psrc,
    void        *pdest )
{
    ssdp_open_unframed_rsp_s *pout = (ssdp_open_unframed_rsp_s *)pdest;

    /* parameter validation */
    if( psrc == NULL || pout == NULL )
    {
        return -1;
    }

    /* command */
    pout->command = *psrc++;

    /* status */
    pout->status = piget16(&psrc);

    /* window size */
    pout->window_size = *psrc++;

    /* maximum length of payload */
    pout->max_payload_length = piget32(&psrc);

    return 0;
}
