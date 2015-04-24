/*************
 *
 * Filename:    imssdp.h
 *
 * Purpose:     Image management Supplementary Streaming Protocol API prototypes
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef IMSSDP_H
#define IMSSDP_H

/*---------------
  Include files
 ---------------*/

/*---------------
  Definitions
 ---------------*/

/*---------------
  Enumerations
 ---------------*/
/*************
 *
 * Name:    ssdp_request_commands_e
 *
 * Purpose: SSDP request message types
 *
 * Notes:
 *
 **************/
enum e_ssdp_request_commands{
    SSDP_CMD_REQ_MIN,
    DLOAD_TO_SDP_REQ = SSDP_CMD_REQ_MIN,
    HELLO_REQ,
    OPEN_REQ,
    WRITE_REQ,
    DONE_REQ,
    DONE_FINAL_REQ,
    DLOAD_REQ,
    GET_IMAGE_PREFERENCE_REQ,
    SSDP_CMD_REQ_MAX
};


/*************
 *
 * Name:    ssdp_response_commands_e
 *
 * Purpose: SSDP response message types
 *
 * Notes:
 *
 **************/
enum e_ssdp_response_commands{
    SSDP_CMD_RSP_MIN,
    DLOAD_TO_SDP_RSP = SSDP_CMD_RSP_MIN,
    HELLO_RSP,
    OPEN_RSP,
    WRITE_RSP,
    DONE_RSP,
    CURRENT_IMAGE_PREFERENCE_RSP,
    ERROR_PACKET_RSP,
    SSDP_CMD_RSP_MAX
};

enum e_ssdp_actual_commands{
    vDLOAD_TO_SDP_REQ = 0x70,
    vDLOAD_TO_SDP_RSP = 0x2,
    vHELLO_REQ = 0x1,
    vHELLO_RSP = 0x2,
    vOPEN_REQ = 0x25,
    vOPEN_RSP = 0x26,
    vWRITE_REQ = 0x27,
    vWRITE_RSP = 0x28,
    vDONE_REQ = 0x29,
    vDONE_RSP = 0x2A,
    vDONE_FINAL_REQ = 0x2D,
    vDLOAD_REQ = 0x2B,
    vDLOAD_RSP = 0x2,
    vGET_IMAGE_PREFERENCE_REQ = 0x2E,
    vCURRENT_IMAGE_PREFERENCE_RSP = 0x2F,
    vERROR_PACKET_RSP =0x0D
};

/*---------------
  Structures
 ---------------*/
/*************
 *
 * Name:    sdp_packetheader
 *
 * Purpose: SDP packet header structure
 *
 * Notes:
 *
 **************/
struct sdp_packetheader{
    swi_uint8 command;
    void *pparameters;
};

/*************
 *
 * Name:    ssdp_packet
 *
 * Purpose: SDP packet structure
 *
 * Notes:
 *
 **************/
struct sdp_packet{
    struct sdp_packetheader phdr;
    swi_uint8 *pdata;
};

/*---------------
  Prototypes
 ---------------*/
/*************
 *
 * Name:    imssdp_pack_request
 *
 * Purpose: SSDP request message packing function
 *
 * Parms:   (IN)psrc     - structure containing arguments for packing the request
 *          (OUT)pdest   - destination buffer into which the request will be packed
 *
 * Return:  message length on success, -1 otherwise
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
swi_int32 imssdp_pack_request(
    struct sdp_packet   *psrc,
    swi_uint8           *pdest );

/*************
 *
 * Name:    imssdp_pack_response
 *
 * Purpose: SSDP response message unpacking function
 *
 * Parms:   (IN)psrc     - source buffer containing SSDP response
 *          (OUT)pdest   - structure to be populated with the information
 *                         returned in the response.
 *
 * Return:  0 on success, -1 otherwise
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
package swi_int32 imssdp_unpack_response(
    swi_uint8           *psrc,
    struct sdp_packet   *pdest );

package swi_uint32 imssdp_map_cmd (enum e_ssdp_actual_commands cmd);

#endif /* IMSSDP_H */
