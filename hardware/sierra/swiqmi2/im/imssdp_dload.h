/*************
 *
 * Filename:    imssdp_dload.h
 *
 * Purpose:     Image management Supplementary Streaming Download Protocol
 *              support.
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef IMSSDP_DLOAD_H
#define IMSSDP_DLOAD_H

/*---------------
  Include files
 ---------------*/
#include "aa/aaglobal.h"

/*---------------
  Definitions
 ---------------*/

/*---------------
  Enumerations
 ---------------*/

/*---------------
  Structures
 ---------------*/
/*************
 *
 * Name:    ssdp_dload_req_s
 *
 * Purpose: SSDP DLOAD request parameter structure
 *
 * Notes:
 *
 **************/
typedef struct ssdp_dload_request_s{
    swi_uint8   command;
}ssdp_dload_req_s;

/*---------------
  Prototypes
 ---------------*/
/*************
 *
 * Name:    imssdp_dload_pack
 *
 * Purpose: SSDP DLOAD message packing function
 *
 * Parms:   (IN)psrc     - pointer to ssdp_dload_req_s
 *          (OUT)pdest   - destination buffer
 *
 * Return:  success: number of bytes packed into destination buffer
 *          failure: -1
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
int imssdp_dload_pack(
    void        *psrc,
    swi_uint8   *pdest );

#endif /* IMSSDP_DLOAD_H */
