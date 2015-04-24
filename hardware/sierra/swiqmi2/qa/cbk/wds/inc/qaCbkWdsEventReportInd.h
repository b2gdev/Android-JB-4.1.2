/*
 * \ingroup cbk
 *
 * \file    qaCbkWdsEventReportInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkWdsEventReportInd.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_EVENT_REPORT_IND_H__
#define __WDS_EVENT_REPORT_IND_H__

/* An enumeration of eQMI_WDS_EVENT_REPORT_IND response TLV IDs */
enum eQMI_WDS_EVENT_REPORT_IND
{
    eTLV_DATA_BEARER_TECHNOLOGY            = 0x17,
    eTLV_DORMANCY_STATUS                   = 0x18,
    eTLV_MIP_STATUS                        = 0x1B,
    eTLV_TX_TOTAL_BYTES                    = 0x19,
    eTLV_RX_TOTAL_BYTES                    = 0x1A,
    eTLV_CURRENT_DATA_BEARER_TECHNOLOGY    = 0x1D,
};

/* Constants to define the type of current networks that can be returned in
 * current Data Bearer Technology TLV */
#define NETWORK3GPP2   0x01
#define NETWORK3GPP    0x02

/* Constants to check for RAT Mask values */
#define MASKDONOTCARE  0x00
#define MASKNULLBEARER 0x8000
#define MASKWCDMA       0x01
#define MASKGPRS        0x02
#define MASKHSDPA       0x04
#define MASKHSUPA       0x08
#define MASKEDGE        0x10
#define MASKLTE         0x20
#define MASKHSDPAPLUS   0x40
#define MASKDCHSDPAPLUS 0x80
#define MASKCDMA1X      0x01
#define MASKEVDOREV0    0x02
#define MASKEVDOREVA    0x04
#define SOMASKEHRPD     0x08

/*
 * eQMI_WDS_EVENT_IND TLVs defined below
 */

/*
 * Name:    DataBearerTechTlv
 *
 * Purpose: Structure used to store all DataBearerTechnology TLV Value.
 *
 * Members: TlvPresent             - Boolean indicating the presence of the TLV
 *                                   in the QMI response
 *
 *          DataBearerTechnology   - Data Bearer Technology Value
 */
struct DataBearerTechTlv
{
    BYTE    TlvPresent;
    ULONG   DataBearerTechnology;
};

/*
 * Name:    DormancyStatusTlv
 *
 * Purpose: Structure used to store all Dormancy Status TLV Value.
 *
 * Members: TlvPresent      - Boolean indicating the presence of the TLV
 *                                   in the QMI response
 *
 *          DormancyStatus   - Dormancy Status Value
 */
struct DormancyStatusTlv
{
    BYTE    TlvPresent;
    ULONG   DormancyStatus;
};

/*
 * Name:    MobileIPStatusTlv
 *
 * Purpose: Structure used to store all Dormancy Status TLV Value.
 *
 * Members: TlvPresent  - Boolean indicating the presence of the TLV
 *                        in the QMI response
 *
 *          MipStatus   - Mobile IP Status Value
 */
struct MobileIPStatusTlv
{
    BYTE  TlvPresent;
    ULONG MipStatus;
};

/*
 * Name:    ByteTotalsTlv
 *
 * Purpose: Structure used to store all Byte Totals TLV Value.
 *
 * Members: TlvPresent   - Boolean indicating the presence of the TLV
 *                         in the QMI response
 *
 *          TxTotalBytes - Total Transmitted Bytes
 *
 *          RxTotalBytes - Total Received Bytes
 *
 */
struct ByteTotalsTlv
{
    BYTE      TlvPresent;
    ULONGLONG TxTotalBytes;
    ULONGLONG RxTotalBytes;
};

/*
 * Name:    CurrentDataBearerTechTlv
 *
 * Purpose: Structure used to store all Current DataBearer Technology
 *          TLV Values.
 *
 * Members: TlvPresent   - Boolean indicating the presence of the TLV in
 *                         the QMI response
 *
 *          RATMask      - RAT mask to indicate the type of technology
 *
 *          SOMask       - SO mask to indicate the service type or type of
 *                         application
 *
 */
struct CurrentDataBearerTechTlv
{
    BYTE    TlvPresent;
    ULONG   RATMask;
    ULONG   SOMask;
};

/*
 * Name:    QmiWdsServStatusInd
 *
 * Purpose: Structure used to store all Data Bearer Notification Parameters.
 *
 * Members: DataBearerTechTlv        - Structure containing the Data Bearer
 *                                     Technology TLV
 *          DormancyStatusTlv        - Structure containing the Dormancy
 *                                     Status TLV
 *          MobileIPStatusYlv        - Structure containing the Mobile IP
 *                                     Status TLV
 *          ByteTotalsTlv            - Structure containing the Byte Totals
 *                                     TLV
 *          CurrentDataBearerTechTlv - Structure containing the Current Data
 *                                     Bearer Technology TLV
 *
 * Note:    None
 */
struct QmiCbkWdsEventStatusReportInd{
    struct DataBearerTechTlv        DBTechTlv;
    struct DormancyStatusTlv        DSTlv;
    struct MobileIPStatusTlv        MSTlv;
    struct ByteTotalsTlv            BTTlv;
    struct CurrentDataBearerTechTlv CDBTechTlv;
};

/*************
 * Prototypes
 **************/
enum eQCWWANError UpkQmiCbkWdsEventReportInd (
    BYTE   *pMdmResp,
    struct QmiCbkWdsEventStatusReportInd *pAipResp );

#endif /* __WDS_EVENT_REPORT_IND_H__ */

