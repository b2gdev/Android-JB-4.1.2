/*
 * \ingroup fms
 *
 * \file    qaFmsGetStoredImages.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaFmsStoredImages.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __FMS_GET_STORED_IMAGES_H__
#define __FMS_GET_STORED_IMAGES_H__

#include "qaGobiApiFms.h"

/*
 * This enumeration contains the TLV IDs for QMI_DMS_LIST_STORED_IMAGES
 * response
 *
 */
enum FMS_LIST_STORED_IMAGES
{
    eTLV_STORED_IMAGE_LIST = 0x01
};

/*
 * This structure contains the Get Stored Images response parameters
 */
struct QmiFmsGetStoredImagesResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
    ULONG              *pImagelistSize;
    struct ImageList   *pImageList;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiFmsGetStoredImages(
    WORD *pMlength,
    BYTE *pBuffer);

extern enum eQCWWANError UpkQmiFmsGetStoredImages(
    BYTE                             *pMdmResp,
    struct QmiFmsGetStoredImagesResp *pApiResp);

#endif /* __FMS_GET_STORED_IMAGES_H__ */
