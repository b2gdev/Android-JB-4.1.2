/*************
 *
 * Filename:    imssdp_donefinal.h
 *
 * Purpose:     Image management Supplementary Streaming Download Protocol
 *              support.
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef IMSSDP_DONEFINAL_H
#define IMSSDP_DONEFINAL_H

/*---------------
  Include files
 ---------------*/
#include "aa/aaglobal.h"

/*---------------
  Definitions
 ---------------*/
#define SSDP_DONEFINAL_REQ_COMMAND  0x2D
/*---------------
  Enumerations
 ---------------*/

/*---------------
  Structures
 ---------------*/
/*************
 *
 * Name:    ssdp_donefinal_rsp_s
 *
 * Purpose: SSDP DONEFINAL response parameter structure
 *
 * Notes:
 *
 **************/
typedef struct ssdp_donefinal_request_s{
    swi_uint8   command;
}ssdp_donefinal_req_s;

/*---------------
  Prototypes
 ---------------*/
/*************
 *
 * Name:    imssdp_donefinal_upack
 *
 * Purpose: SSDP DONEFINAL message unpacking function
 *
 * Parms:   (IN)psrc    - source buffer
 *          (OUT)pdest   - pointer to ssdp_donefinal_rsp_s
 *
 * Return:  success: number of bytes packed into destination buffer
 *          failure: -1
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
int imssdp_donefinal_pack(
    void        *psrc,
    swi_uint8   *pdest );

#endif /* IMSSDP_DONEFINAL_H */
