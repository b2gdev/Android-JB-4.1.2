/*
 * \ingroup pds
 *
 * \file qaPdsForceXtraDownload.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaPdsForceXtraDownload.c
 *
 * Copyright: © 2010 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_FORCE_XTRA_DOWNLOAD_H__
#define __PDS_FORCE_XTRA_DOWNLOAD_H__

/*
 * This structure contains the ForceXTRADownload response parameters
 *
 */
struct QmiPdsForceXtraDownloadResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 *  prototypes
 */
extern enum eQCWWANError PkQmiPdsForceXtraDownload(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiPdsForceXtraDownload(
    BYTE                               *pMdmResp,
    struct QmiPdsForceXtraDownloadResp *pApiResp );

#endif /* __PDS_FORCE_XTRA_DOWNLOAD_H__ */
