/*************
 *
 * Filename:    imssdp_done.c
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
#include "pi/piuproto.h"
#include "sl/sludefs.h"
#include "imssdp_done.h"

/*---------------
  Definitions
 ---------------*/
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
package int imssdp_done_pack(
    void        *psrc,
    swi_uint8   *pdest )
{
    ssdp_done_req_s *pin = (ssdp_done_req_s *)psrc;

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

package int imssdp_done_unpack(
    swi_uint8   *psrc,
    void        *pdest )
{
    ssdp_done_rsp_s *pout = (ssdp_done_rsp_s *)pdest;

    /* parameter validation */
    if( psrc == NULL || pout == NULL )
    {
        return -1;
    }

    /* command */
    pout->command = *psrc++;

    /* session status */
    pout->session_status = piget16(&psrc);

    /* image type that failed */
    pout->failed_image_type = *psrc++;

    /* error description */
    swi_uint32 len = MIN(   slstrlen((char *)psrc),
                            sizeof(pout->error_description) - 1  );
                /* -1 to ensure null termination in slstrncpy below */

    slstrncpy(  (char *)pout->error_description,
                (char *)psrc,
                len + 1 );

    return 0;
}
