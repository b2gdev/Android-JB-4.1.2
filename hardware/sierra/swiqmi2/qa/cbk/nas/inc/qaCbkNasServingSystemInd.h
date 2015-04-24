/*
 * \ingroup cbk
 *
 * \file    qaCbkNasServingSystemInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkNasServignSystemInd.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SERVING_SYSTEM_IND_H__
#define __NAS_SERVING_SYSTEM_IND_H__
#include "qaGobiApiCbk.h"

/* An enumeration of eQMI_NAS_SERVING_SYSTEM_IND response TLV IDs */
enum eQMI_NAS_SERVING_SYSTEM_IND
{
    eTLV_SERVING_SYSTEM         = 0x01,
    eTLV_ROAMING_IND_VALUE      = 0x10,
    eTLV_CURRENT_PLMN           = 0x12,
    eTLV_DATA_SERVICE_CAPS      = 0x11,
    eTLV_CDMA_SYSTEM_ID         = 0x13,
    eTLV_CDMA_BASE_STATION_INFO = 0x14,
    eTLV_ROAMING_IND_LIST       = 0x15,
    eTLV_DEFAULT_ROAMING_IND    = 0x16,
    eTLV_3GPP2_TIME_ZONE        = 0x17,
    eTLV_CDMA_P_REV_IN_USE      = 0x18
};

/*
 * eQMI_NAS_SS_INFO_IND TLVs defined below
 */

/*
 * Name:    RoamingIndicationTlv
 *

 * Purpose: Structure used to store all Roaming Indication TLV Value.
 *
 * Members: TlvPresent      - Boolean indicating the presence of the TLV
 *                            in the QMI response
 *
 *          roaming_ind    - Roaming Indication Value
 */
struct RoamingIndicationTlv
{
    BYTE TlvPresent;
    BYTE roaming_ind;
};

#define NAS_MAX_ARRAY_LENGTH 255
/*
 * Name:    DataCapabilitiesTlv
 *
 * Purpose: Structure used to store all Data Capabilities TLV Value.
 *
 * Members: TlvPresent      - Boolean indicating the presence of the TLV
 *                            in the QMI response
 *
 *          datacaps_len    - Length of Data Capabilities
 *
 *          datacaps        - Data Capabilities
 */
struct DataCapabilitiesTlv
{
    BYTE TlvPresent;
    BYTE datacaps_len;
    BYTE datacaps[NAS_MAX_ARRAY_LENGTH];
};

/*
 * Name:    ServingSystemTlv
 *
 * Purpose: Structure used to store all Serving System TLV Value.
 *
 * Members: TlvPresent    - Boolean indicating the presence of the TLV
 *                          in the QMI response
 *
 *          ServingSystem - pointer to ServingSystemInfo
 */
struct ServingSystemTlv
{
    BYTE                     TlvPresent;
    struct ServingSystemInfo ServingSystem;
};

/*
 * Name:    QmiCbkNasServingSystemInd
 *
 * Purpose: Structure used to store all QMI Notification parameters.
 *
 * Members: SSTlv               - Serving System Tlv
 *          RITlv               - Roaming Indication Tlv
 *          mobile_country_code - Representation of MCC
 *          mobile_network_code - Representation of MNC
 *          DCTlv               - Data Capabilities Tlv
 *          sid                 - System ID
 *          nid                 - Network ID
 *          base_id             - Base Station Identification Number
 *          base_lat            - Base Station Latitude
 *          base_long           - Base Station Longitude
 *
 * Note:    None
 */
struct QmiCbkNasServingSystemInd{
    struct ServingSystemTlv     SSTlv;
    struct RoamingIndicationTlv RITlv;
    WORD                        mobile_country_code;
    WORD                        mobile_network_code;
    BYTE                        network_desc_len;
    struct DataCapabilitiesTlv  DCTlv;
    WORD                        sid;
    WORD                        nid;
    WORD                        base_id;
    ULONG                       base_lat;
    ULONG                       base_long;
};

/*************
 * Prototypes
 **************/
enum eQCWWANError UpkQmiCbkNasServingSystemInd (
    BYTE                             *pMdmResp,
    struct QmiCbkNasServingSystemInd *pApiResp );

#endif /* __NAS_SERVING_SYSTEM_IND_H__ */

