/*************
 *
 * Filename:    imssdp_dload_to_sdp.h
 *
 * Purpose:     Image management Supplementary Streaming Download Protocol
 *              support.
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef IMSSDP_DLOAD_TO_SDP_H
#define IMSSDP_DLOAD_TO_SDP_H

/*---------------
  Include files
 ---------------*/
#include "aa/aaglobal.h"

/*---------------
  Definitions
 ---------------*/
#define SSDP_DLOAD_TO_SDP 0x70
/*---------------
  Enumerations
 ---------------*/

/*---------------
  Structures
 ---------------*/
/*************
 *
 * Name:    ssdp_dload_to_sdp_req_s
 *
 * Purpose: SSDP switch from DLOAD to SDP protocol request parameter structure
 *
 * Notes:
 *
 **************/
typedef struct ssdp_dload_to_sdp_request_s{
    swi_uint8   command;
    swi_uint16  flags;
}ssdp_dload_to_sdp_req_s;

/*************
 *
 * Name:    ssdp_dload_to_sdp_rsp_s
 *
 * Purpose: SSDP switch from DLOAD to SDP protocol response parameter structure
 *
 * Notes:
 *
 **************/
typedef struct ssdp_dload_to_sdp_response_s{
    swi_uint8   command;
} ssdp_dload_to_sdp_rsp_s;

/*---------------
  Prototypes
 ---------------*/
/*************
 *
 * Name:    imssdp_dload_to_sdp_pack
 *
 * Purpose: SSDP Switch from DLOAD to SDP protocol command packing function
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
int imssdp_dload_to_sdp_pack(
    void        *psrc,
    swi_uint8   *pdest );

int imssdp_dload_to_sdp_unpack(
    swi_uint8   *psrc,
    void        *pdest );

#endif /* IMSSDP_DLOAD_TO_SDP_H */
