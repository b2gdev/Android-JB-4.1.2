/*************
 *
 * Filename:    imssdp_unframed_write.c
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
#include "imcommon.h"
#include "imssdp_unframed_write.h"

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
int imssdp_unframed_write_pack(
    void        *psrc,
    swi_uint8   *pdest )
{
    ssdp_unframed_write_req_s *pin = (ssdp_unframed_write_req_s *)psrc;

    /* parameter validation */
    if( pin == NULL || pdest == NULL )
    {
        return -1;
    }

    swi_uint8 *pbase = pdest;

   /* command */
    *pdest++ = pin->command;

    /* sequence number */
    piput16( pin->sequence_number, &pdest );

    /* address */
    piput32( pin->address, &pdest );

    /* length */
    piput32( pin->length, &pdest );

    /* header's crc */
    swi_uint16 hdrlen = (pdest - pbase);
    swi_uint16 fcs = imCalculate16CRC( pbase, 8*hdrlen );
    piput16( fcs, &pdest );

    /* payload */
    slmemcpy( (char *)pdest,
              (char *)pin->payload,
              pin->length );

    /* advance pointer */
    pdest += pin->length;

    /* payload crc - NOT supported by FW yet !*/
/*   swi_uint16 pfcs = imCalculate16CRC( pdest, 8*pin->length ); */
/*   piput16( pin->payload_crc, &pdest ); */

    return (int)(pdest - pbase);
}

int imssdp_unframed_write_unpack(
    swi_uint8   *psrc,
    void        *pdest )
{
    ssdp_unframed_write_rsp_s *pout = (ssdp_unframed_write_rsp_s *)pdest;

    /* parameter validation */
    if( psrc == NULL || pout == NULL )
    {
        return -1;
    }

    /* command */
    pout->command = *psrc++;

    /* sequence number */
    pout->sequence_number = piget16(&psrc);

    /* address */
    pout->address = piget32(&psrc);

    /* status */
    pout->status = piget16(&psrc);

    return 0;
}
