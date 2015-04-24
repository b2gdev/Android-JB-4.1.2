/*
 * \ingroup fms
 *
 * \file    qaFmsGetImagesPreference.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaFmsGetImagesPreference.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __FMS_GET_IMAGES_PREFERENCE_H__
#define __FMS_GET_IMAGES_PREFERENCE_H__

/*
 * An enumeration of eQMI_DMS_GET_FIRMWARE_PREF response TLV IDs
 */
enum eQMI_FMS_GET_IMAGES_PREFERENCE_RESP
{
    eTLV_GET_IMAGES_PREF_LIST = 0x01
};

/*
 * This structure contains the GetImagesPreference response parameters
 */
struct QmiFmsGetImagesPreferenceResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetImagesPreference response parameters */
    ULONG                *pImageListSize;
    struct PrefImageList *pImageList;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiFmsGetImagesPreference(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiFmsGetImagesPreference(
    BYTE                                 *pMdmResp,
    struct QmiFmsGetImagesPreferenceResp *pApiResp );

#endif /* __FMS_GET_IMAGES_PREFERENCE_H__ */
