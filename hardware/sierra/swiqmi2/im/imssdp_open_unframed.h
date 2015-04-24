/*************
 *
 * Filename:    imssdp_open_unframed.h
 *
 * Purpose:     Image management Supplementary Streaming Download Protocol
 *              support.
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef IMSSDP_OPEN_UNFRAMED_H
#define IMSSDP_OPEN_UNFRAMED_H

/*---------------
  Include files
 ---------------*/
#include "aa/aaglobal.h"

/*---------------
  Definitions
 ---------------*/
 /* open unframed packet image header maximum size */
#define SSDP_OPEN_UNFRAMED_PKT_HEADER_MAX_SZ    400
#define SSDP_OPEN_UNFRAMED_PKT_CWE_HEADER_SZ    400
#define SSDP_OPEN_UNFRAMED_PKT_COMMAND          0X25
#define SSDP_OPEN_UNFRAMED_STATUS_OK            0X00
#define SSDP_OPEN_UNFRAMED_IMAGE_NT_REQUIRED    0x07

/*---------------
  Enumerations
 ---------------*/

/*---------------
  Structures
 ---------------*/
/*************
 *
 * Name:    ssdp_open_unframed_req_s
 *
 * Purpose: SSDP OPEN_UNFRAMED request parameter structure
 *
 * Notes:
 *
 **************/
typedef struct ssdp_open_unframed_request_s{
    swi_uint8   command;
    swi_uint8   image_mode;
    swi_uint32  image_length;
    swi_uint8   window_size;
    swi_uint32  max_payload_length;
    swi_uint16  feature_flag;
    swi_char    *pimage_header_payload;
}ssdp_open_unframed_req_s;

/*************
 *
 * Name:    ssdp_open_unframed_rsp_s
 *
 * Purpose: SSDP OPEN_UNFRAMED response parameter structure
 *
 * Notes:
 *
 **************/
typedef struct ssdp_open_unframed_response_s{
    swi_uint8   command;
    swi_uint16  status;
    swi_uint8   window_size;
    swi_uint32  max_payload_length;
}ssdp_open_unframed_rsp_s;

/*---------------
  Prototypes
 ---------------*/
/*************
 *
 * Name:    imssdp_open_unframed_pack
 *
 * Purpose: SSDP OPEN_UNFRAMED message packing function
 *
 * Parms:   (IN)psrc     - pointer to ssdp_open_unframed_req_s
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
int imssdp_open_unframed_pack(
    void        *psrc,
    swi_uint8   *pdest );

/*************
 *
 * Name:    imssdp_open_unframed_unpack
 *
 * Purpose: SSDP OPEN_UNFRAMED message unpacking function
 *
 * Parms:   (IN)psrc     - source buffer
 *          (OUT)pdest   - pointer to ssdp_open_unframed_rsp_s
 *
 * Return:  0 on success
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/int imssdp_open_unframed_unpack(
    swi_uint8   *psrc,
    void        *pdest );

#endif /* IMSSDP_OPEN_UNFRAMED_H */
