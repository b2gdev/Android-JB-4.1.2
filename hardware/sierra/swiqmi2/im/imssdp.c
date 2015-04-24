/*************
 *
 * Filename:    imssdp.c
 *
 * Purpose:     Image management Supplementary Streaming Download Protocol
 *              support.
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
/* Reference: QCT document 80-VF459-1 */

/*-------------
  Include files
 --------------*/
#include "aa/aaglobal.h"
#include "er/erudefs.h"
#include "imssdp_dload.h"
#include "imssdp_dload_to_sdp.h"
#include "imssdp_done.h"
#include "imssdp_donefinal.h"
#include "imssdp_error.h"
#include "imssdp_hello.h"
#include "imssdp_image_preference.h"
#include "imssdp_open_unframed.h"
#include "imssdp_unframed_write.h"
#include "imssdp.h"
#include <stdio.h>

/*---------------
  Definitions
 ---------------*/

/*--------------
  Enumerations
 ---------------*/

/*--------------
  Structures
 --------------*/

/*--------------
  Local storage
 --------------*/
local int (*ssdp_req_vector[])( void *psrc, swi_uint8 *pdest) =
{
    imssdp_dload_to_sdp_pack,
    imssdp_hello_pack,
    imssdp_open_unframed_pack,
    imssdp_unframed_write_pack,
    imssdp_done_pack,
    imssdp_donefinal_pack,
    imssdp_dload_pack,
    imssdp_get_image_preference_pack,
    NULL    /* Terminating entry */
};

local int(*ssdp_rsp_vector[])( swi_uint8 *psrc, void *pdest) =
{
    imssdp_dload_to_sdp_unpack,
    imssdp_hello_unpack,
    imssdp_open_unframed_unpack,
    imssdp_unframed_write_unpack,
    imssdp_done_unpack,
    imssdp_get_image_preference_unpack,
    imssdp_error_unpack,
    NULL    /* Terminating entry */
};

/*--------------
  Functions
 --------------*/
local swi_bool ssdp_req_cmd_supported(
    enum e_ssdp_request_commands cmd )
{
    if( cmd < SSDP_CMD_REQ_MAX )
    {
        return TRUE;
    }
    return  FALSE;
}

local swi_bool ssdp_rsp_cmd_supported(
    enum e_ssdp_response_commands cmd )
{
    if( cmd < SSDP_CMD_RSP_MAX )
    {
        return TRUE;
    }
    return  FALSE;
}

package swi_uint32 imssdp_map_cmd (enum e_ssdp_actual_commands cmd)
{
    switch (cmd)
    {
    case vDLOAD_TO_SDP_REQ: return DLOAD_TO_SDP_REQ;
    case vDLOAD_TO_SDP_RSP: return DLOAD_TO_SDP_RSP;
    case vHELLO_REQ: return HELLO_REQ;
    case vOPEN_REQ: return OPEN_REQ;
    case vOPEN_RSP: return OPEN_RSP;
    case vWRITE_REQ: return WRITE_REQ;
    case vWRITE_RSP:return WRITE_RSP;
    case vDONE_REQ: return DONE_REQ;
    case vDONE_RSP: return DONE_RSP;
    case vDONE_FINAL_REQ: return DONE_FINAL_REQ;
    case vDLOAD_REQ: return DLOAD_REQ;
    case vGET_IMAGE_PREFERENCE_REQ: return GET_IMAGE_PREFERENCE_REQ;
    case vCURRENT_IMAGE_PREFERENCE_RSP: return CURRENT_IMAGE_PREFERENCE_RSP;
    case vERROR_PACKET_RSP: return ERROR_PACKET_RSP;
    default:
        {
            char errmsg[100];
            snprintf(errmsg, sizeof(errmsg),
                     "%s:%d Invalid SSDP command",
                     (char *)__func__, __LINE__);
            erAbort (errmsg, cmd);
        }
        break;
    }
    return -1;
}

package swi_int32 imssdp_pack_request(
    struct sdp_packet   *psrc,
    swi_uint8           *pdest )
{
    enum e_ssdp_request_commands cmd;
    cmd = imssdp_map_cmd(psrc->phdr.command);

    if( TRUE == ssdp_req_cmd_supported(cmd) )
    {
        return ssdp_req_vector[cmd](psrc, pdest);
    }

    return -1;
}

package swi_int32 imssdp_unpack_response(
    swi_uint8           *psrc,
    struct sdp_packet   *pdest )
{
    enum e_ssdp_response_commands cmd;
    cmd = imssdp_map_cmd(*psrc);

    if( TRUE == ssdp_rsp_cmd_supported(cmd) )
    {
        return ssdp_rsp_vector[cmd](psrc, pdest);
    }

    return -1;
}
