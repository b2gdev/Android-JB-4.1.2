/**
 * \internal
 *
 * \ingroup nas
 *
 * \file qaNasPerformNetworkScan.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasPerformNetworkScan.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_PERFORM_NETWORK_SCAN_MSG_H__
#define __NAS_PERFORM_NETWORK_SCAN_MSG_H__


#define QMI_NAS_NETSTATUS_MASK       0x03
#define QMI_NAS_MAX_INSTANCES        20
#define INDEX_ZERO                   0
#define ROAMING_INDEX                2
#define FORBIDDEN_INDEX              4
#define PREFERRED_INDEX              6
#define MAX_DESCRIPTION_LENGTH       255


/**
 * \internal
 *
 * An enumeration of eQMI_NAS_PERFORM_NETWORK_SCAN response TLV IDs
 *
 */
enum eQMI_NAS_PERFORM_NETWORK_SCAN_RESP
{
    eTLV_3GPP_NETWORK_INFO        = 0x10
};

/**
 * This structure contains the PerformNetworkScan response parameters.
 * This structure will hold the array of the netwok scan information.
 *
 *  \param  pMCC
 *          - Mobile Country Code
 *
 *  \param  pMNC
 *          - Mobile Networ Code
 *
 *  \param  pInUse
 *          - Is the Network the current serving Network
 *              - 0 – Unknown
 *              - 1 – Current serving network
 *              - 2 – Not current serving network, available
 *
 *  \param  pRoaming
 *          - Home/Roam Status of the Network
 *              - 0 – Unknown
 *              - 1 – Home
 *              - 2 – Roam
 *
 *  \param  pForbidden
 *          - Is the Network in the forbidden network list
 *              - 0 – Unknown
 *              - 1 – Forbidden
 *              - 2 – Not Forbidden
 *
 *  \param  pPreferred
 *          - Is the Network in the Preferred network list
 *              - 0 – Unknown
 *              - 1 – Preferred
 *              - 2 – Not Preferred
 *
 *  \param  pDesription
 *          - Network Name/Description
 */
struct QmiNas3GppNetworkInfo
{   
    WORD     pMCC;
    WORD     pMNC; 
    ULONG    pInUse;
    ULONG    pRoaming;
    ULONG    pForbidden;
    ULONG    pPreferred;
    CHAR     pDesription[MAX_DESCRIPTION_LENGTH];
};

/**
 * \internal
 *
 * This structure contains the PerformNetworkScan response parameters.
 *
 * \sa qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasPerformNetworkScanResp
{
    /**
     * \internal
     *
     * Every response message must have a results structure
     */
    struct qmTlvResult results;

    /**
     * \internal
     *
     * Response parameters
     */
    BYTE   *pInstanceSize;
    struct QmiNas3GppNetworkInfo  *pInstances;
};

/**
 * \internal
 *
 * Prototypes
 */
extern enum eQCWWANError PkQmiNasPerformNetworkScan (
    WORD    *pMlength, 
    BYTE    *pParamField );

extern enum eQCWWANError UpkQmiNasPerformNetworkScan(
    BYTE     *pMdmResp,
    struct   QmiNasPerformNetworkScanResp *pAipResp);

#endif /* __NAS_PERFORM_NETWORK_SCAN_MSG_H__ */
