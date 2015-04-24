/*************
 *
 * Filename:    imssdp_hello.c
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
#include "imssdp_hello.h"

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
package int imssdp_hello_pack(
    void        *psrc,
    swi_uint8   *pdest )
{
    ssdp_hello_req_s *pin = (ssdp_hello_req_s *)psrc;

    /* parameter validation */
    if( pin == NULL || pdest == NULL )
    {
        return -1;
    }

    swi_uint8 *pbase = pdest;

    /* command */
    *pdest++ = pin->command;

    /* magic number */
    swi_uint32 len = slstrlen((char *)pin->magic_num);
    slstrncpy(  (char *)pdest,
                (char *)pin->magic_num,
                len );

    pdest += len;
    slmemset( (char *)pdest, 0, 32 - len ); /* zero padding */
    pdest += 32 - len;

    /* version number */
    *pdest++ = pin->version_num;

    /* compatible version */
    *pdest++ = pin->compatible_version;

    /* feature bits */
    piput32( pin->featurebits, &pdest );

    return (int)(pdest - pbase);
}

package int imssdp_hello_unpack(
    swi_uint8   *psrc,
    void        *pdest )
{
    ssdp_hello_rsp_s *pout = (ssdp_hello_rsp_s *)pdest;
    swi_uint32 len = 0;

    /* parameter validation */
    if( psrc == NULL || pout == NULL )
    {
        return -1;
    }

    /* command */
    pout->command = *psrc++;

    /* magic number */
    len = sizeof(pout->magic_num);

    slstrncpy(  (char *)pout->magic_num,
                (char *)psrc,
                len );

    psrc += len;

    /* reserved */
    pout->reserved = piget32(&psrc);

    /* major boot flash version */
    pout->major_boot_flash_version = piget16(&psrc);

    /* minor boot flash version */
    pout->minor_boot_flash_version = piget16(&psrc);

    /* compatible version */
    pout->compatible_version = *psrc++;

    /* maximum block size */
    pout->max_block_sz = piget32(&psrc);

    /* flash base address */
    pout->flash_base_address = piget32(&psrc);

    /* flash ID length */
    pout->flash_id_len = *psrc++;

    /* flash ID */
    len = MIN( SSDP_HELLO_REQ_PKT_FLASH_ID_MAX_SZ - 1,
               pout->flash_id_len );
                /* -1 to ensure space for null terminated string */

    slstrncpy( (char *)pout->flash_id,
               (char *)psrc,
                len );

    psrc += len;

    /* window size */
    pout->window_sz = piget16(&psrc);

    /* number of sectors */
    pout->num_of_sectors = piget16(&psrc);

    /* Sector Sizes */
    swi_uint32 count = MIN( SSDP_HELLO_REQ_PKT_SECTOR_SIZES_MAX_SZ,
                            pout->num_of_sectors );

    swi_uint32 i;
    for( i = 0 ; i < count ; i++ )
        pout->sector_sizes[i] = piget32( &psrc );

    psrc += len;

    /* feature bits */
    pout->featurebits = piget32(&psrc);

    return 0;
}
