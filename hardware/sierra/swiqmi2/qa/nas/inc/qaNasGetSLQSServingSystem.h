/**
 * \ingroup nas
 *
 * \file qaNasGetSLQSServingSystem.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasGetSLQSServingSystem.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_GET_SLQS_SERVING_SYSTEM_MSG_H__
#define __NAS_GET_SLQS_SERVING_SYSTEM_MSG_H__
#include "qaGobiApiNas.h"

/*
 * An enumeration of eQMI_NAS_GET_CDMA_SERVING_SYSTEM response TLV IDs
 *
 */
enum eQMI_NAS_GET_SLQS_SERVING_SYSTEM_RESP
{
    eTLV_SERV_SYSTEM                     = 0x01,
    eTLV_ROAMING_IND_VALUE               = 0x10,
    eTLV_DATA_SRV_CAPABILITY             = 0x11,
    eTLV_CURR_PLMN                       = 0x12,
    eTLV_CDMA_SYSTEM_ID                  = 0x13,
    eTLV_CDMA_BASE_STATION_INFORMATION   = 0x14,
    eTLV_ROAMING_IND_LIST                = 0x15,
    eTLV_DEFAULT_ROAMING_INDICATOR       = 0x16,
    eTLV_3GPP2_TIME_ZONE                 = 0x17,
    eTLV_CDMA_P_REV_IN_USE               = 0x18,
    eTLV_3GPP_TIME_ZONE                  = 0x1A,
    eTLV_3GPP2_DST_ADJUSTMENT            = 0x1B,
    eTLV_3GPP_LAC                        = 0x1C,
    eTLV_3GPP_CELL_ID                    = 0x1D,
    eTLV_CONCURRENT_SVC_INFO             = 0x1E,
    eTLV_PRL_INDICATOR                   = 0x1F,
    eTLV_DTM_INDICATION                  = 0x20,
    eTLV_DETAILED_SVC_INFO               = 0x21,
    eTLV_CDMA_SYSTEM_INFO_EXT            = 0x22,
    eTLV_HDR_PERSONALITY                 = 0x23,
    eTLV_TAC_INFO_LTE                    = 0x24,
    eTLV_CALL_BARRING_STATUS             = 0x25,
};

/*
 * This structure contains the SLQSGetServingSystem response parameters.
 *
 * \sa  qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasGetSLQSServingSystemResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Response parameters */
    servSystem              *pServingSystem;
    BYTE                    *pRoamIndicatorVal;
    dataSrvCapabilities     *pDataSrvCapabilities;
    currentPLMN             *pCurrentPLMN;
    WORD                    *pSystemID;
    WORD                    *pNetworkID;
    WORD                    *pBasestationID;
    ULONG                   *pBasestationLatitude;
    ULONG                   *pBasestationLongitude;
    roamIndList             *pRoamingIndicatorList;
    BYTE                    *pDefaultRoamInd;
    qaQmi3Gpp2TimeZone      *p3Gpp2TimeZone;
    BYTE                    *pCDMA_P_Rev;
    BYTE                    *p3GppTimeZone;
    BYTE                    *pGppNetworkDSTAdjustment;
    WORD                    *pLac;
    ULONG                   *pCellID;
    BYTE                    *pConcSvcInfo;
    BYTE                    *pPRLInd;
    BYTE                    *pDTMInd;
    detailSvcInfo           *pDetailedSvcInfo;
    CDMASysInfoExt          *pCDMASystemInfoExt;
    BYTE                    *pHdrPersonality;
    WORD                    *pTrackAreaCode;
    callBarStatus           *pCallBarStatus;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiNasGetSLQSServingSystem(
    WORD *pMlength,
    BYTE *pParamField );

extern enum eQCWWANError UpkQmiNasGetSLQSServingSystem(
    BYTE                                  *pMdmResp,
    struct QmiNasGetSLQSServingSystemResp *pApiResp );

#endif /* __NAS_GET_SLQS_SERVING_SYSTEM_MSG_H__ */
