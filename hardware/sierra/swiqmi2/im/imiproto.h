/*************
 *
 * Filename:  imiproto.h
 *
 * Purpose:   This file contains internal prototypes for the im package
 *
 * Copyright: © 201i Sierra Wireless Inc., all rights reserved
 *
 **************/

/* include files */
#include "imssdp_unframed_write.h"
#include "imssdp_hello.h"
#include "imssdp_open_unframed.h"
#include "imidefs.h"

/* imtask_sdk.c */
struct imtcb *imgetcbp(void);

void imsendframed (
    swi_uint8 *txbufp,
    swi_uint32 pktsize);

void imbuild_unframed_write_req (
    ssdp_unframed_write_req_s *psUnframedWriteReq,
    USHORT  sequenceNumber,
    ULONG   blockSize,
    BYTE    *pImageBlock );

enum fw_image_type_e imgetfiletype(
    const char *path );

enum imerrcodes_e im_cwe_img_info_get(
    struct im_image_info_s *pin );

enum imerrcodes_e im_gobi_img_info_get(
    struct im_image_info_s *pimginfo );

void imgobitask( void *paramp );

void imdssend (
    swi_uint8 *txbufp,
    swi_uint32 pktsize);

void imbuild_hello_req (
    ssdp_hello_req_s *psHelloReq );

void imgobidsstart(void);

struct imtcb *imgetcbp(void);

void imhandleunframed (
    swi_uint8  *memrelp,
    swi_uint8  *rxbufp,
    swi_uint16 pktsize,
    swi_bool   errorflag);

void imunframe (
    swi_uint8 *rxbufp,
    swi_uint32 pktsize);
