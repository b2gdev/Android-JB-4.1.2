/*
 * \ingroup wds
 *
 * \file qaWdsSetMipMode.h
 *
 * \brief Header file for QMI_WDS_SET_MIP_MODE interface
 *
 * Copyright: © 2010 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __WDS_SET_MIP_MODE_MSG_H__
#define __WDS_SET_MIP_MODE_MSG_H__

#define QMI_WDS_SET_MIP_OFF       0x00
#define QMI_WDS_SET_MIP_PREFERRED 0x01
#define QMI_WDS_SET_MIP_ONLY      0x02

/*
 * This enumeration contains the TLV IDs for QMI_WDS_SET_MIP_MODE
 * requests
 */
enum eWDS_SET_MIP_MODE_REQ
{
    eTLV_SET_MIP_MODE = 0x01
};

/*
 * This structure contains the SetMobileIP request parameters
 *
 * See SWIQCWWAN.h for parameter descriptions
 *
 */
struct QmiWdsSetMipModeReq
{
    ULONG mode;
};

/*
 * This structure contains the SetMobileIP response parameters
 *
 * See SWIQCWWAN.h for parameter descriptions
 *
 */
struct QmiWdsSetMipModeResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/* Prototypes */
extern enum eQCWWANError PkQmiWdsSetMipMode(
    WORD    *pMlength,
    BYTE    *pBuffer,
    ULONG   mode );

extern enum eQCWWANError UpkQmiWdsSetMipMode(
    BYTE                        *pMdmResp,
    struct QmiWdsSetMipModeResp *pApiResp );

#endif /* __WDS_SET_MIP_MODE_MSG_H__ */

