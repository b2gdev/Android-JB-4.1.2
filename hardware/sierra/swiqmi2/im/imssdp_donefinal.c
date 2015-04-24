/*************
 *
 * Filename:    imssdp_donefinal.c
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
#include "imssdp_donefinal.h"

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
package int imssdp_donefinal_pack(
    void        *psrc,
    swi_uint8   *pdest )
{
    ssdp_donefinal_req_s *pin = (ssdp_donefinal_req_s *)psrc;

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
