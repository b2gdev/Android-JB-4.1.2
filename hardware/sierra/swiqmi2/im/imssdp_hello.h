/*************
 *
 * Filename:    imssdp_hello.h
 *
 * Purpose:     Image management Supplementary Streaming Download Protocol
 *              support.
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef SSDP_HELLO_H
#define SSDP_HELLO_H

/*---------------
  Include files
 ---------------*/
#include "aa/aaglobal.h"

/*---------------
  Definitions
 ---------------*/
/* The bits in the feature bits field are set to identify the features to be used
 * for that particular session. To indicate that more than one feature is
 * supported, the target will bitwise "OR" the supported feature values to compute
 * the value of the field. The target’s set of supported feature bits will be
 * bitwise "ANDED" together with the host’s set of requested feature bits, and the
 * result will be used for the session.
 */
#define SSDP_HELLO_PKT_UNCOMPRESSED_FEATURE_BIT             0x00000001
#define SSDP_HELLO_PKT_NAND_BOOTABLE_IMAGE_FEATURE_BIT      0x00000002
#define SSDP_HELLO_PKT_NAND_BOOTLOADER_FEATURE_BIT          0x00000004
#define SSDP_HELLO_PKT_NAND_MULTI_IMAGE_FEATURE_BIT         0x00000008
#define SSDP_HELLO_PKT_UNFRAMED_STREAMING_DATA_FEATURE_BIT  0x00000010
#define SSDP_HELLO_PKT_QDL_HIGH_SPEED_FEATURE_BIT           0x00000020
#define SSDP_HELLO_PKT_QDL_BOOT_AND_RECOVERY_FEATURE_BIT    0x00000040

/* HELLO Request packet definitions */
#define SSDP_HELLO_REQ_PKT_COMMAND              0X01
#define SSDP_HELLO_REQ_PKT_MAGIC_NUMBER         "QCOM high speed protocol hst"
#define SSDP_HELLO_REQ_PKT_VERSION_NUMBER       0X06
#define SSDP_HELLO_REQ_PKT_COMPATIBLE_VERSION   0x06

/* HELLO Response packet definitions */
#define SSDP_HELLO_RSP_PKT_COMMAND              0x02
#define SSDP_HELLO_RSP_PKT_MAGIC_NUMBER         "QCOM high speed protocol"
#define SSDP_HELLO_RSP_PKT_RESERVED             0xFFFFFFFF
#define SSDP_HELLO_RSP_PKT_VERSION_NUMBER       0X06
#define SSDP_HELLO_RSP_PKT_COMPATIBLE_VERSION   0x06
#define SSDP_HELLO_REQ_PKT_FLASH_ID_MAX_SZ      64
#define SSDP_HELLO_REQ_PKT_SECTOR_SIZES_MAX_SZ  200

/*---------------
  Enumerations
 ---------------*/

/*---------------
  Structures
 ---------------*/
/*************
 *
 * Name:    ssdp_hello_req_s
 *
 * Purpose: SSDP HELLO request parameter structure
 *
 * Notes:
 *
 **************/
typedef struct ssdp_hello_request_s{
    swi_uint8   command;
    swi_uint8   magic_num[32];
    swi_uint8   version_num;
    swi_uint8   compatible_version;
    swi_uint32  featurebits;
}ssdp_hello_req_s;

/*************
 *
 * Name:    ssdp_hello_rsp_s
 *
 * Purpose: SSDP HELLO response parameter structure
 *
 * Notes:
 *
 **************/
typedef struct ssdp_hello_response_s{
    swi_uint8   command;
    swi_uint8   magic_num[24];
    swi_uint32  reserved;
    swi_uint16  major_boot_flash_version;
    swi_uint16  minor_boot_flash_version;
    swi_uint8   version_number;
    swi_uint8   compatible_version;
    swi_uint32  max_block_sz;
    swi_uint32  flash_base_address;
    swi_uint8   flash_id_len;
    swi_char    flash_id[SSDP_HELLO_REQ_PKT_FLASH_ID_MAX_SZ];
    swi_uint16  window_sz;
    swi_uint16  num_of_sectors;
    swi_uint32  sector_sizes[SSDP_HELLO_REQ_PKT_SECTOR_SIZES_MAX_SZ];
    swi_uint32  featurebits;
}ssdp_hello_rsp_s;

/*---------------
  Prototypes
 ---------------*/
/*************
 *
 * Name:    imssdp_hello_pack
 *
 * Purpose: SSDP HELLO message packing function
 *
 * Parms:   (IN)psrc     - pointer to ssdp_hello_req_s
 *          (OUT)pdest   - destination buffer
 *
 * Return:  0 on success
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
int imssdp_hello_pack(
    void *psrc,
    swi_uint8 *pdest );

/*************
 *
 * Name:    imssdp_hello_unpack
 *
 * Purpose: SSDP HELLO message unpacking function
 *
 * Parms:   (OUT)psrc    - source buffer
 *          (OUT)pdest   - pointer to ssdp_hello_rsp_s
 *
 * Return:  0 on success
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
int imssdp_hello_unpack(
    swi_uint8 *psrc,
    void *pdest );

#endif /* SSDP_HELLO_H */
