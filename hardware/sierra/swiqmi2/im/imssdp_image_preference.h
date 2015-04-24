/*************
 *
 * Filename:    imssdp_image_preference.h
 *
 * Purpose:     Image management Supplementary Streaming Download Protocol
 *              support.
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef IMSSDP_IMAGE_PREFERENCES_H
#define IMSSDP_IMAGE_PREFERENCES_H

/*---------------
  Include files
 ---------------*/
#include "aa/aaglobal.h"

/*---------------
  Definitions
 ---------------*/
/* QDL Get firmware prefrence Request packet definitions */
#define SSDP_GETFWPREF_REQ_PKT_COMMAND 46

/* QDL Get firmware prefrence Response packet definitions */
#define SSDP_GETFWPREF_RSP_PKT_COMMAND 47
#define SSDP_MAX_IMAGE_PREF            12
#define SSDP_IMAGE_ID_LEN              16

/*---------------
  Enumerations
 ---------------*/

/*---------------
  Structures
 ---------------*/
/*************
 *
 * Name:    ssdp_get_image_preference_req_s
 *
 * Purpose: SSDP GET IMAGE_PREFERENCE request parameter structure
 *
 * Notes:
 *
 **************/
typedef struct ssdp_get_image_preference_request_s{
    swi_uint8   command;
}ssdp_get_image_preference_req_t;

/*************
 *
 * Name:    ssdp_image_preference_s
 *
 * Purpose: SSDP CURRENT IMAGE_PREFERENCE Image ID structure
 *
 * Notes:
 *
 **************/
typedef struct ssdp_image_preference_s{
    swi_uint8   image_type;
    swi_uint8   image_identifier[SSDP_IMAGE_ID_LEN];
}ssdp_img_pref_t;

/*************
 *
 * Name:    ssdp_current_image_preference_rsp_s
 *
 * Purpose: SSDP CURRENT IMAGE_PREFERENCE response parameter structure
 *
 * Notes:
 *
 **************/
typedef struct ssdp_current_image_preference_response_s{
    swi_uint8       command;
    swi_uint8       total_entries_in_list;
    ssdp_img_pref_t ssdp_img_pref[SSDP_MAX_IMAGE_PREF];
}ssdp_get_image_preference_rsp_t;

/*---------------
  Prototypes
 ---------------*/
/*************
 *
 * Name:    imssdp_get_image_preference_pack
 *
 * Purpose: SSDP GET_IMAGE_PREFERENCE message packing function
 *
 * Parms:   (IN)psrc     - pointer to ssdp_image_preference_req_s
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
int imssdp_get_image_preference_pack(
    void        *psrc,
    swi_uint8   *pdest );

/*************
 *
 * Name:    imssdp_get_image_preference_unpack
 *
 * Purpose: SSDP CURRENT_IMAGE_PREFERENCE message unpacking function
 *
 * Parms:   (IN)psrc     - source buffer
 *          (OUT)pdest   - pointer to ssdp_image_preference_rsp_s
 *
 * Return:  0 on success
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
int imssdp_get_image_preference_unpack(
    swi_uint8   *psrc,
    void        *pdest );

#endif /* IMSSDP_IMAGE_PREFERENCES_H */
