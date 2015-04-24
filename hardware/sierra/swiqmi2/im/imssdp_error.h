/*************
 *
 * Filename:    imssdp_error.h
 *
 * Purpose:     Image management Supplementary Streaming Download Protocol
 *              support.
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef IMSSDP_ERROR_H
#define IMSSDP_ERROR_H

/*---------------
  Include files
 ---------------*/
#include "aa/aaglobal.h"

/*---------------
  Definitions
 ---------------*/
#define SSDP_ERROR_RSP_ERR_DESCRIPTION_MAX_SZ   256

/*---------------
  Enumerations
 ---------------*/

/*---------------
  Structures
 ---------------*/
/*************
 *
 * Name:    ssdp_error_rsp_s
 *
 * Purpose: SSDP ERROR response parameter structure
 *
 * Notes:
 *
 **************/
typedef struct ssdp_error_response_s{
    swi_uint8   command;
    swi_uint32  error_code;
    swi_char    error_description[SSDP_ERROR_RSP_ERR_DESCRIPTION_MAX_SZ];
}ssdp_error_rsp_s;

/*---------------
  Prototypes
 ---------------*/
/*************
 *
 * Name:    imssdp_error_unpack
 *
 * Purpose: SSDP ERROR message unpacking function
 *
 * Parms:   (OUT)psrc    - source buffer
 *          (OUT)pdest   - pointer to ssdp_error_rsp_s
 *
 * Return:  0 on success
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
int imssdp_error_unpack(
    swi_uint8   *psrc,
    void        *pdest );

#endif /* IMSSDP_DONE_H */
