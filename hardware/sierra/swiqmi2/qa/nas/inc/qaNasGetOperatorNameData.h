/*
 * \ingroup nas
 *
 * \file  qaNasGetOperatorNameData.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasGetOperatorNameData.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_GET_OPERATOR_NAME_DATA_H__
#define __NAS_GET_OPERATOR_NAME_DATA_H__

#include "qaGobiApiNas.h"

#define MAX_MCC_MNC_INDEX 3

/* enum declarations */
/*
 * An enumeration of eQMI_NAS_GET_OPERATOR_NAME_DATA response TLV IDs
 */
enum eNAS_GET_OPERATOR_NAME_DATA_RESP
{
    eTLV_SERVICE_PROVIDER_NAME = 0x10,
    eTLV_OPERATOR_PLMN_LIST    = 0x11,
    eTLV_PLMN_NETWORK_NAME     = 0x12,
    eTLV_OPERATOR_NAME_STRING  = 0x13,
    eTLV_NITZ_INFORMATION      = 0x14,
};


/*
 * This structure contains the GetOPeratorNameData response parameters.
 *
 * \sa  qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasGetOperatorNameDataResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Response parameters */
    nasOperatorNameResp  *pOperatorNameData;
};

/*
 * Prototypes
 */
enum eQCWWANError PkQmiNasGetOperatorNameData(
    WORD *pMlength,
    BYTE *pBuffer );

enum eQCWWANError UpkQmiNasGetOperatorNameData(
    BYTE                                 *pMdmresp,
    struct QmiNasGetOperatorNameDataResp *pApiresp);

#endif /* __NAS_GET_OPERATOR_NAME_DATA_H__ */
