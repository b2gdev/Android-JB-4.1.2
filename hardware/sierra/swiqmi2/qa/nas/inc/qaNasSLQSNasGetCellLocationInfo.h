/*
 * \ingroup nas
 *
 * \file    qaNasSLQSNasGetCellLocationInfo.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaNasSLQSNasGetCellLocationInfo.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SLQS_GET_CELL_LOCATION_INFO_H__
#define __NAS_SLQS_GET_CELL_LOCATION_INFO_H__

#include "qaGobiApiNas.h"

/* enum declarations */
/*
 * An enumeration of eNAS_SLQS_GET_CELL_LOCATION_INFO_RESP response TLV IDs
 */
enum eNAS_SLQS_GET_CELL_LOCATION_INFO_RESP
{
    eTLV_GERAN_CELL_INFO            = 0x10,
    eTLV_UMTS_CELL_INFO             = 0x11,
    eTLV_CDMA_CELL_INFO             = 0x12,
    eTLV_LTE_INTRA_FREQ_CELL_INFO   = 0x13,
    eTLV_LTE_INTER_FREQ_CELL_INFO   = 0x14,
    eTLV_LTE_NEIGHB_GSM_CELL_INFO   = 0x15,
    eTLV_LTE_NEIGHB_WCDMA_CELL_INFO = 0x16
};

/*
 * This structure contains the SLQSNasGetCellLocationInfo response parameters.
 */
struct QmiNasSLQSGetCellLocationInfoResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    nasCellLocationInfoResp *pNasCellLocationInfoResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiNasSlqsGetCellLocationInfo(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiNasSlqsGetCellLocationInfo(
    BYTE                                     *pMdmResp,
    struct QmiNasSLQSGetCellLocationInfoResp *pApiResp );

#endif /* __NAS_SLQS_GET_CELL_LOCATION_INFO_H__ */
