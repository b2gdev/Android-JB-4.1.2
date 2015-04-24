/*
 * Filename:  imudefs.h
 *
 * Purpose:   User definitions for IM package
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 */

#ifndef IMUDEFS_H
#define IMUDEFS_H

/*---------------
  include files
 ---------------*/
#include "aa/aaglobal.h"

/*---------------
  Defines
 ---------------*/
/* common definitions */
#define IM_IMAGE_PATH_MAX_SZ            256
#define IM_AMSS_GENERIC_UMTS_MBN_DIR    '6'

/* Gobi MBN image defines */
#define MBN_BUILD_ID_STR_LEN        100
#define MBN_IMG_ID_STR_LEN          16
#define GOBI_MBN_IMG_INFO_BUF_SIZE  256

#define MBN_LOCK_MAGIC      0x809b1d80L
#define MBN_BOOT_MAGIC      0xFEDC1234L
#define MBN_BUILD_MAGIC     0xFEDC1235L
#define UQCN_INFO_MAGIC     0xFEDC1236L
#define MBN_HASH_MAGIC      0xFEDC1237L
#define MBN_LOCK_AUTH_MAGIC 0xFEDC1238L

/* CWE Sierra Package String parameter defines */
#define CWE_SKU_SZ           10
#define CWE_PARTNO_SZ        10
#define CWE_MODELID_SZ       10
#define CWE_FWVERSION_SZ     15
#define CWE_BOOTBLK_SZ       2
#define CWE_CARRIER_SZ       15
#define CWE_PRIVERSION_SZ    7
#define CWE_PACKAGEID_SZ     3

/*---------------
  Enums
 ---------------*/
/* IM error codes */
enum imerrcodes_e{
    IMERRNONE,
    IMERRINVLDPATH,
    IMERRCORRUPTIMAGE,
    IMERRUNSUPPORTEDIMGTYPE,
    IMERRSYSTEM
};

/* supported image types */
enum fw_image_type_e
{
    FW_IMG_TYPE_MBN_GOBI,
    FW_IMG_TYPE_MBN,
    FW_IMG_TYPE_CWE,
    FW_IMG_TYPE_INVLD
};

/* mbn image types */
enum mbn_image_type_e
{
    MBN_IMG_TYPE_AMSS = 0x05,
    MBN_IMG_TYPE_UQCN = 0x0D,
    MBN_IMG_TYPE_UNSUPPORTED = 0xFF
};

/* CWE image types */
enum cwe_image_type_e
{
    CWE_IMG_TYPE_SPKG = 0x80
};

/*---------------
  Structures
 ---------------*/
struct cwe_img_info_s{
    swi_char modelid_str    [CWE_MODELID_SZ + 1];
    swi_char fwversion_str  [CWE_FWVERSION_SZ + 1];
    swi_char sku_str        [CWE_SKU_SZ + 1];
    swi_char packageid_str  [CWE_PACKAGEID_SZ + 1];
    swi_char carrier_str    [CWE_CARRIER_SZ + 1];
    swi_char priversion_str [CWE_PRIVERSION_SZ + 1];
};

struct gobi_mbn_img_info_s{
    enum mbn_image_type_e type;
    swi_uint32  versionid;
    swi_char    imgidstr[MBN_IMG_ID_STR_LEN];
    swi_char    imgvrsnstr[MBN_BUILD_ID_STR_LEN];
};

union image_info_s{
    struct cwe_img_info_s cweinfo;
    struct gobi_mbn_img_info_s gobiinfo;
};

struct im_image_info_s{
    char *pimgpath;
    enum fw_image_type_e imgtype;
    union image_info_s imginfo;
};

#include "imuproto.h"
#endif /* IMUDEFS_H */
