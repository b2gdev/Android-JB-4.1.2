/*
 * \ingroup dms
 *
 * \file    qaDmsGetBandCapability.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDmsGetBandCapability.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_GET_BAND_CAPABILITY_H__
#define __DMS_GET_BAND_CAPABILITY_H__

/*
 *
 * An enumeration ofeQMI_DMS_GET_BAND_CAPABILITY response TLV IDs
 *
 */
enum eQMI_DMS_GET_BAND_CAPABILITY_RESP
{
    eTLV_BAND_CAPABILITY = 0x01
};

enum eQMI_DMS_GET_BAND_CAPABILITY_LENGTH
{
    eTLV_BAND_CAPABILITY_LENGTH = 0x08
};

/*
 * This structure contains the SLQSGetBandCapability response parameters.
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 */
struct QmiDmsGetBandCapabilityResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
    ULONGLONG          *pBandCapability;
};

/*************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiDmsGetBandCapability (
    WORD *pMlength,
    BYTE *pParamField );

extern enum eQCWWANError UpkQmiDmsGetBandCapability (
    BYTE                               *pMdmResp,
    struct QmiDmsGetBandCapabilityResp *pAipResp );

#endif /* __DMS_GET_BAND_CAPABILITY_H__ */
