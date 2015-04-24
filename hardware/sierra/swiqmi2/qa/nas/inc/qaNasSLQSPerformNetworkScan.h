/*
 * \ingroup nas
 *
 * \file   qaNasSLQSPerformNetworkScan.h
 *
 * \brief  This file contains definitions, enumerations, structures and
 *         forward declarations for qaNasSLQSPerformNetworkScan.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SLQS_PERFORM_NETWORK_SCAN_H__
#define __NAS_SLQS_PERFORM_NETWORK_SCAN_H__

#define QMI_NAS_NETSTATUS_MASK 0x03
#define QMI_NAS_MAX_INSTANCES  20
#define INDEX_ZERO             0
#define ROAMING_INDEX          2
#define FORBIDDEN_INDEX        4
#define PREFERRED_INDEX        6
#define NO_NW_INSTANCE         0

/*
 * \internal
 *
 * An enumeration of NAS_SLQS_PERFORM_NETWORK_SCAN_RESP response TLV IDs
 *
 */
enum eNAS_SLQS_PERFORM_NETWORK_SCAN_RESP
{
    eTLV_3GPP_NETWORK_INFORMATION = 0x10,
    eTLV_3GPP_NETWORK_RAT         = 0x11,
};

/*
 * \internal
 *
 * This structure contains the PerformNetworkScan response parameters.
 *
 * \sa qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasSlqsPerformNetworkScanResp
{
    /*
     * \internal
     *
     * Every response message must have a results structure
     */
    struct qmTlvResult results;

    /*
     * \internal
     *
     * Response parameters
     */
    slqsNetworkScanInfo *pNetworkInfo;
};

/*
 * \internal
 *
 * Prototypes
 */
extern enum eQCWWANError PkQmiNasSlqsPerformNetworkScan (
    WORD *pMlength,
    BYTE *pParamField );

extern enum eQCWWANError UpkQmiNasSlqsPerformNetworkScan(
    BYTE                                      *pMdmResp,
    struct   QmiNasSlqsPerformNetworkScanResp *pApiResp);

#endif /* __NAS_SLQS_PERFORM_NETWORK_SCAN_H__ */
