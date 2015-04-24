/*
 * \ingroup fms
 *
 * \file    qaFmsDeleteStoredImage.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaFmsDeleteStoredImage.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __FMS_DELETE_STORED_IMAGE_H__
#define __FMS_DELETE_STORED_IMAGE_H__

/* enum declarations */
/*
 * An enumeration of eQMI_DMS_DELETE_FIRMWARE request TLV IDs
 */
enum eQMI_DMS_DEL_STORED_IMG_REQ
{
    eTLV_STORED_IMAGE_INFO  = 0x01
};

/*
 * This structure contains the DeleteStoredImage request parameters.
 */
struct QmiFmsDeleteStoredImageReq
{
    struct ImageElement *pImageInfo;
};

/*
 * This structure contains the DeleteStoredImage response parameters.
 *
 */
struct QmiFmsDeleteStoredImageResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiFmsDeleteStoredImage(
    WORD  *pMlength,
    BYTE  *pBuffer,
    BYTE  *pImageInfo );

extern enum eQCWWANError UpkQmiFmsDeleteStoredImage(
    BYTE                               *pMdmResp,
    struct QmiFmsDeleteStoredImageResp *pApiResp );

#endif /* __FMS__DELETE_STORED_IMAGE_H__ */
