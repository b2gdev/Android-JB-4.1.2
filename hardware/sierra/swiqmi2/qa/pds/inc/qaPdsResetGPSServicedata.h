/*
 * \ingroup pds
 *
 * \file    qaPdsResetGPSServicedata.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaPdsResetGPSServicedata.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_RESET_GPS_SERVICE_DATA_H__
#define __PDS_RESET_GPS_SERVICE_DATA_H__

/* enum declarations */
/*
 * An enumeration of eQMI_PDS_RESET_GPS_SERVICE_DATA request TLV IDs
 */
enum eQMI_PDS_RESET_GPS_SERVICE_DATA_REQ
{
    eTLV_GPS_SRV_DATA_MASK    = 0x10,
    eTLV_GPS_SRV_CELL_DB_MASK = 0x11
};

/*
 * This structure contains the ResetPDSData request parameters
 */
struct QmiPdsResetGPSServicedataReq
{
    ULONG *pGPSDataMask;
    ULONG *pCellDataMask;
};

/*
 * This structure contains the ResetPDSData response parameters
 */
struct QmiPdsResetGPSServicedataResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiPdsResetGPSServicedata(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG *pGPSDataMask,
    ULONG *pCellDataMask );

extern enum eQCWWANError UpkQmiPdsResetGPSServicedata(
    BYTE                                 *pMdmResp,
    struct QmiPdsResetGPSServicedataResp *pApiResp );

#endif /* __PDS_RESET_GPS_SERVICE_DATA_H__ */
