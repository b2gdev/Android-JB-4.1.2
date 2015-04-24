/*************
 *
 * Filename:    imssdp_error.c
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
#include "imssdp_error.h"

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
package int imssdp_error_unpack(
    swi_uint8   *psrc,
    void        *pdest )
{
    ssdp_error_rsp_s *pout = (ssdp_error_rsp_s *)pdest;

    /* parameter validation */
    if( psrc == NULL || pout == NULL )
    {
        return -1;
    }

    /* command */
    pout->command = *psrc++;

    /* error reason code */
    pout->error_code = piget32(&psrc);

    /* error description */
    swi_uint32 len = MIN(   slstrlen((char *)psrc),
                            sizeof(pout->error_description) -1  );
                /* -1 to ensure null termination in slstrncpy below */

    slstrncpy(  (char *)pout->error_description,
                (char *)psrc,
                len + 1 );

    return 0;
}
