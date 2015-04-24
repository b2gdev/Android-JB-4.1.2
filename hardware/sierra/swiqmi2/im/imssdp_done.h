/*************
 *
 * Filename:    imssdp_done.h
 *
 * Purpose:     Image management Supplementary Streaming Download Protocol
 *              support.
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef IMSSDP_DONE_H
#define IMSSDP_DONE_H

/*---------------
  Include files
 ---------------*/
#include "aa/aaglobal.h"

/*---------------
  Definitions
 ---------------*/
#define SSDP_DONE_RSP_ERR_DESCRIPTION_MAX_SZ    256
#define SSDP_DONE_REQ_COMMAND       0x29
#define SSDP_DONE_SUCCESS           0x00
#define SSDP_DONE_AUTH_FAILED       0x01
#define SSDP_DONE_FLASH_WRITE_ERROR 0x02

/*---------------
  Enumerations
 ---------------*/

/*---------------
  Structures
 ---------------*/
/*************
 *
 * Name:    ssdp_done_rsp_s
 *
 * Purpose: SSDP DONE response parameter structure
 *
 * Notes:
 *
 **************/
typedef struct ssdp_done_request_s{
    swi_uint8   command;
}ssdp_done_req_s;

/*************
 *
 * Name:    ssdp_done_rsp_s
 *
 * Purpose: SSDP DONE response parameter structure
 *
 * Notes:
 *
 **************/
typedef struct ssdp_done_response_s{
    swi_uint8   command;
    swi_uint16  session_status;
    swi_uint8   failed_image_type;
    swi_char    error_description[SSDP_DONE_RSP_ERR_DESCRIPTION_MAX_SZ];
}ssdp_done_rsp_s;

/*---------------
  Prototypes
 ---------------*/
/*************
 *
 * Name:    imssdp_done_pack
 *
 * Purpose: SSDP DONE message packing function
 *
 * Parms:   (IN)psrc     - pointer to ssdp_done_req_s
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
int imssdp_done_pack(
    void        *psrc,
    swi_uint8   *pdest );

/*************
 *
 * Name:    imssdp_done_unpack
 *
 * Purpose: SSDP DONE message unpacking function
 *
 * Parms:   (IN)psrc    - source buffer
 *          (OUT)pdest   - pointer to ssdp_done_rsp_s
 *
 * Return:  0 on success
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
int imssdp_done_unpack(
    swi_uint8   *psrc,
    void        *pdest );

#endif /* IMSSDP_DONE_H */
