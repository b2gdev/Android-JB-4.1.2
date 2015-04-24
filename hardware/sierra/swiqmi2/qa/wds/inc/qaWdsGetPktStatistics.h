/*
 * \ingroup wds
 *
 * \Filename: qaWdsGetPktStatistics.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsGetPktStatistics.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_GET_STATISTICS_MSG_H__
#define __WDS_GET_STATISTICS_MSG_H__

/* enum declarations */
/*
 *
 * An enumeration of eQMI_WDS_GET_STATISTICS request TLV IDs
 *
 */
enum eWDS_GET_STATISTICS_REQ
{
    eTLV_STAT_MASK        = 0x01
};

/*
 * This structure contains the GetPacketStatus request parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */

struct QmiWdsGetPktStatisticsReq
{
    ULONG     statmask;
};

/*
 *
 * An enumeration of eQMI_WDS_GET_STATISTICS response TLV IDs
 *
 */
enum eWDS_GET_STATISTICS_RESP
{
    eTLV_TX_OK_COUNT         = 0x10,
    eTLV_RX_OK_COUNT         = 0x11,
    eTLV_TX_ERR_COUNT        = 0x12,
    eTLV_RX_ERR_COUNT        = 0x13,
    eTLV_TX_OFL_COUNT        = 0x14,
    eTLV_RX_OFL_COUNT        = 0x15,
    eTLV_TX_OK_BYTE_COUNT    = 0x19,
    eTLV_RX_OK_BYTE_COUNT    = 0x1A
};

/*
 * This structure contains the GetPacketStatus response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsGetPktStatisticsResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetPacketStatus response parameters */
    ULONG        *pTXPacketSuccesses;
    ULONG        *pRXPacketSuccesses;
    ULONG        *pTXPacketErrors;
    ULONG        *pRXPacketErrors;
    ULONG        *pTXPacketOverflows;
    ULONG        *pRXPacketOverflows;
    ULONGLONG    *pTXOkBytesCount;
    ULONGLONG    *pRXOkBytesCount;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsGetPktStatistics(
    WORD    *pMlength,
    BYTE    *pBuffer,
    ULONG   statmask);

extern enum eQCWWANError UpkQmiWdsGetPktStatistics(
    BYTE    *pMdmResp,
    struct  QmiWdsGetPktStatisticsResp *pAipResp);

#endif /* __WDS_GET_STATISTICS_MSG_H__ */
