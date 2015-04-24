/*************
 *
 * Filename:    imssdp_unframed_write.h
 *
 * Purpose:     Image management Supplementary Streaming Download Protocol
 *              support.
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef IMSSDP_UNFRAMED_WRITE_H
#define IMSSDP_UNFRAMED_WRITE_H

/*---------------
  Include files
 ---------------*/
#include "aa/aaglobal.h"

/*---------------
  Definitions
 ---------------*/
#define SSDP_UNFRAMED_WRITE_COMMAND    0x27

#define SSDP_UNFRAMED_WRITE_DATA_RECEIVED_OK    0x00
#define SSDP_UNFRAMED_WRITE_DATA_CRC_FAILED     0x01
#define SSDP_UNFRAMED_WRITE_DATA_PARSING_FAILED 0x02

/*---------------
  Enumerations
 ---------------*/

/*---------------
  Structures
 ---------------*/
/*************
 *
 * Name:    ssdp_unframed_write_req_s
 *
 * Purpose: SSDP UNFRAMED_WRITE request parameter structure
 *
 * Notes:
 *
 **************/
typedef struct ssdp_unframed_write_request_s{
    swi_uint8   command;
    swi_uint16  sequence_number;
    swi_uint32  address;
    swi_uint32  length;
    swi_uint16  header_crc;
    swi_char    *payload;
    swi_uint16  payload_crc;
}ssdp_unframed_write_req_s;

/*************
 *
 * Name:    ssdp_unframed_write_rsp_s
 *
 * Purpose: SSDP UNFRAMED_WRITE response parameter structure
 *
 * Notes:
 *
 **************/
typedef struct ssdp_unframed_write_response_s{
    swi_uint8   command;
    swi_uint16  sequence_number;
    swi_uint32  address;
    swi_uint16  status;
}ssdp_unframed_write_rsp_s;

/*---------------
  Prototypes
 ---------------*/
/*************
 *
 * Name:    imssdp_unframed_write_pack
 *
 * Purpose: SSDP UNFRAMED_WRITE message packing function
 *
 * Parms:   (IN)psrc     - pointer to ssdp_unframed_write_req_s
 *          (OUT)pdest   - destination buffer
 *
 * Return:  0 on success
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
int imssdp_unframed_write_pack(
    void        *psrc,
    swi_uint8   *pdest );

/*************
 *
 * Name:    imssdp_unframed_write_unpack
 *
 * Purpose: SSDP UNFRAMED_WRITE message unpacking function
 *
 * Parms:   (IN)psrc     - source buffer
 *          (OUT)pdest   - pointer to ssdp_unframed_write_rsp_s
 *
 * Return:  0 on success
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
int imssdp_unframed_write_unpack(
    swi_uint8   *psrc,
    void        *pdest );

#endif /* IMSSDP_UNFRAMED_WRITE_H */
