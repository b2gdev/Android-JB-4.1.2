/*************
 *
 * Filename:    imssdp_dload_to_sdp.c
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
#include "imssdp_dload_to_sdp.h"

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
package int imssdp_dload_to_sdp_pack(
    void        *psrc,
    swi_uint8   *pdest )
{
    ssdp_dload_to_sdp_req_s *pin = (ssdp_dload_to_sdp_req_s *)psrc;

     /* parameter validation */
    if( pin == NULL || pdest == NULL )
    {
        return -1;
    }

    swi_uint8 *pbase = pdest;

    /* command */
    *pdest++ = pin->command;

    /* flags */
    piput16( pin->flags, &pdest );

    return (int)(pdest - pbase);
}

package int imssdp_dload_to_sdp_unpack(
    swi_uint8   *psrc,
    void        *pdest )
{
    ssdp_dload_to_sdp_rsp_s *pout = (ssdp_dload_to_sdp_rsp_s *)pdest;

    /* parameter validation */
    if( psrc == NULL || pout == NULL )
    {
        return -1;
    }

    /* command */
    pout->command = *psrc++;

    return 0;
}
