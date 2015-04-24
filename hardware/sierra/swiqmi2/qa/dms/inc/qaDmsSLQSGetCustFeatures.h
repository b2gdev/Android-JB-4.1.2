/*
 * \ingroup dms
 *
 * \file    qaDmsSLQSGetCustFeatures.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDmsSLQSGetCustFeatures.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_GET_CUST_FEATURES_H__
#define __DMS_GET_CUST_FEATURES_H__

#include "qaGobiApiDms.h"

/* enum declarations */

/*
 * An enumeration of eQMI_DMS_SWI_GET_CUST_FEATURES response TLV IDs
 */
enum eQMI_DMS_SWI_GET_CUST_FEATURES_RESP
{
    eTLV_GPS_ENABLE        = 0x10,
    eTLV_DISABLE_IMSI      = 0x11,
    eTLV_IP_FAMILY_SUPPORT = 0x12,
    eTLV_RM_AUTO_CONNECT   = 0x13,
    eTLV_GPS_SEL           = 0x14,
};

/*
 * This structure contains the GetCustFeatures response parameters.
 */
struct QmiDmsGetCustFeaturesResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    custFeaturesInfo *pCustFeaturesInfo;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiDmsGetCustFeatures(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiDmsGetCustFeatures(
    BYTE                             *pMdmResp,
    struct QmiDmsGetCustFeaturesResp *pApiResp );

#endif /* __DMS_GET_CUST_FEATURES_H__ */
