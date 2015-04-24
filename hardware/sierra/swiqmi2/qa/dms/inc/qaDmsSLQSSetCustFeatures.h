/*
 * \ingroup dms
 *
 * \file    qaDmsSLQSSetCustFeatures.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDmsSLQSSetCustFeatures.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_SET_CUST_FEATURES_H__
#define __DMS_SET_CUST_FEATURES_H__

#include "qaGobiApiDms.h"

/* enum declarations */

/*
 * An enumeration of eQMI_DMS_SWI_SET_CUST_FEATURES request TLV IDs
 */
enum eQMI_DMS_SWI_SET_CUST_FEATURES_REQ
{
    eTLV_GPS_SELECT = 0x14,
    eTLV_GPS_SET_ENABLE = 0x10
};

/*
 * This structure contains the SetCustFeatures request parameters.
 */
struct QmiDmsSetCustFeaturesReq
{
    custFeaturesSetting *pCustFeaturesSetting;
};

/*
 * This structure contains the SetCustFeatures response parameters.
 */
struct QmiDmsSetCustFeaturesResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiDmsSetCustFeatures(
    WORD                *pMlength,
    BYTE                *pBuffer,
    custFeaturesSetting *pCustFeaturesSetting );

extern enum eQCWWANError UpkQmiDmsSetCustFeatures(
    BYTE                             *pMdmResp,
    struct QmiDmsSetCustFeaturesResp *pApiResp );

#endif /* __DMS_SET_CUST_FEATURES_H__ */
