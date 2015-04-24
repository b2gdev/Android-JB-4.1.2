/*
 * \ingroup fms
 *
 * \file    qaFmsSetImagesPreference.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaFmsSetImagesPreference.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __FMS_SET_IMAGES_PREFERENCE_H__
#define __FMS_SET_IMAGES_PREFERENCE_H__

/* enum declarations */
/*
 * An enumeration of eQMI_DMS_SET_FIRMWARE_PREF request TLV IDs
 */
enum eQMI_FMS_GET_IMAGES_PREFERENCE_REQ
{
    eTLV_SET_IMAGES_PREF_LIST  = 0x01,
    eTLV_SET_DOWNLOAD_OVERRIDE = 0x10,
    eTLV_SET_MDM_STORAGE_INDEX = 0x11
};

/*
 * This structure contains the SetImagesPreference request parameters.
 */
struct QmiFmsSetImagesPreferenceReq
{
    ULONG                imageListSize;
    struct PrefImageList *pImageList;
    ULONG                bForceDownload;
    BYTE                 modemIndex;
};

/*
 * An enumeration of eQMI_DMS_SET_FIRMWARE_PREF response TLV IDs
 */
enum eQMI_FMS_SET_IMAGES_PREFERENCE_RESP
{
    eTLV_SET_IMAGE_DWNLD_LIST = 0x01
};
/*
 * This structure contains the SetImagesPreference response parameters.
 */
struct QmiFmsSetImagesPreferenceResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* SetImagesPreference response parameters */
    ULONG *pImageTypesSize;
    BYTE  *pImageTypes;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiFmsSetImagesPreference(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG imageListSize,
    BYTE  *pImageList,
    ULONG bForceDownload,
    BYTE  modemIndex );

extern enum eQCWWANError UpkQmiFmsSetImagesPreference(
    BYTE                                 *pMdmResp,
    struct QmiFmsSetImagesPreferenceResp *pApiResp );

#endif /* __FMS_SET_IMAGES_PREFERENCE_H__ */
