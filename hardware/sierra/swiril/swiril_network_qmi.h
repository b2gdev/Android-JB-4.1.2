/**
 *
 * @ingroup swiril
 *
 * @file 
 * Prototypes for network related QMI functions
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#ifndef RIL_NETWORK_QMI_H
#define RIL_NETWORK_QMI_H 1

#include <stdbool.h>
#include "swiril_versiondefs.h"
#include "swiril_main.h"
#include "swiril_network.h"

typedef enum {
    RAT_NO_SERVICE =  0,
    RAT_CDMA_1XRTT =  1,
    RAT_CDMA_1XEVDO = 2,
    RAT_AMPS =        3,   /* Unsupported */
    RAT_GSM =         4,
    RAT_UMTS =        5,
    RAT_LTE =         8,
    RAT_MAX
} Radio_interface;

typedef enum {
    /* START MOTO 157 */
    DATA_BEARER_TECH_CDMA_1XRTT = 1,
    DATA_BEARER_TECH_CDMA_1XEVDO_REV0   = 2,
    DATA_BEARER_TECH_GPRS               = 3,
    DATA_BEARER_TECH_WCDMA              = 4,
    DATA_BEARER_TECH_CDMA_1XEVDO_REVA   = 5,
    DATA_BEARER_TECH_EDGE               = 6,
    DATA_BEARER_TECH_HSDPA_WCDMA        = 7,
    DATA_BEARER_TECH_WCDMA_HSUPA        = 8,
    DATA_BEARER_TECH_HSDPA_HSUPA        = 9,
    DATA_BEARER_TECH_LTE                = 10,
    DATA_BEARER_TECH_CDMA_EVDO_EHRPD    = 11,
    DATA_BEARER_TECH_HSDPAPLUS_WCDMA    = 12,
    DATA_BEARER_TECH_HSDPAPLUS_HSUPA    = 13,
    DATA_BEARER_TECH_DCHSDPAPLUS_WCDMA  = 14,
    DATA_BEARER_TECH_DCHSDPAPLUS_HSUPA  = 15,
    DATA_BEARER_TECH_HSDPAPLUS_64QAM    = 16,
    DATA_BEARER_TECH_HSDPAPLUS_64QAM_HSUPA = 17
    /* STOP MOTO 157 */
}QMI_DATA_BEARER;

typedef enum {
    QMI_REG_STATE_NOT_REGISTERED = 0,
    QMI_REG_STATE_REGISTERED,
    QMI_REG_STATE_SEARCHING_NOT_REGISTERED,
    QMI_REG_STATE_REGISTRATION_DENIED,
    QMI_REG_STATE_UNKNOWN
}QMI_REGISTRATION_STATE;

typedef enum {
    QMI_SERVING_NW_CAPABILITY_GPRS = 1,
    QMI_SERVING_NW_CAPABILITY_EDGE,
    QMI_SERVING_NW_CAPABILITY_HSDPA,
    QMI_SERVING_NW_CAPABILITY_HSUPA,
    QMI_SERVING_NW_CAPABILITY_WCDMA,
    QMI_SERVING_NW_CAPABILITY_CDMA_1XRTT,
    QMI_SERVING_NW_CAPABILITY_CDMA_1XEVDO_REV0,
    QMI_SERVING_NW_CAPABILITY_CDMA_1XEVDO_REVA,
    QMI_SERVING_NW_CAPABILITY_GSM,
    /* SWI extension defined in qaGobiApiNas.h */
    QMI_SERVING_NW_CAPABILITY_CDMA_1XEVDO_REVB = 10,
    QMI_SERVING_NW_CAPABILITY_LTE,
    QMI_SERVING_NW_CAPABILITY_HSPA_PLUS,
    QMI_SERVING_NW_CAPABILITY_DUAL_CARRIER_HSPA_PLUS    
}QMI_SERVING_NW_CAPABILITY;

typedef enum {
    QMI_REGISTERED_RAN_UNKNOWN = 0,
    QMI_REGISTERED_RAN_CDMA2000,
    QMI_REGISTERED_RAN_UMTS
}QMI_REGISTERED_RAN;

typedef enum {
    SWI_NO_UPDATE = 0,
    SWI_SERVICE_STATUS_UPDATED,
    SWI_REGISTRATION_STATUS_UPDATED
} SWI_Service_Network_Status_Update;

typedef enum {
    QMI_CURRENT_DATA_BEARER_CURRENT_NETWORK_UNKNOWN = 0,
    QMI_CURRENT_DATA_BEARER_CURRENT_NETWORK_3GPP2,
    QMI_CURRENT_DATA_BEARER_CURRENT_NETWORK_3GPP
} QMI_CURRENT_DATA_BEARER_CURRENT_NETWORK;

typedef enum {
    QMI_CURRENT_DATA_BEARER_RAT_MASK_CDMA_DONOT_CARE = 0x00,
    QMI_CURRENT_DATA_BEARER_RAT_MASK_CDMA_1X = 0x01,
    QMI_CURRENT_DATA_BEARER_RAT_MASK_CDMA_EVDO_REV0 = 0x02,
    QMI_CURRENT_DATA_BEARER_RAT_MASK_CDMA_EVDO_REVA = 0x04
} QMI_CURRENT_DATA_BEARER_RAT_MASK_CDMA;

typedef enum {
    QMI_CURRENT_DATA_BEARER_RAT_MASK_UMTS_DONOT_CARE = 0x00,
    QMI_CURRENT_DATA_BEARER_RAT_MASK_UMTS_WCDMA = 0x01,
    QMI_CURRENT_DATA_BEARER_RAT_MASK_UMTS_GPRS = 0x02,
    QMI_CURRENT_DATA_BEARER_RAT_MASK_UMTS_HSDPA = 0x04,
    QMI_CURRENT_DATA_BEARER_RAT_MASK_UMTS_HSUPA = 0x08,
    QMI_CURRENT_DATA_BEARER_RAT_MASK_UMTS_EDGE = 0x10,
    QMI_CURRENT_DATA_BEARER_RAT_MASK_UMTS_LTE = 0x20,
} QMI_CURRENT_DATA_BEARER_RAT_MASK_UMTS;

typedef enum {
    QMI_CURRENT_DATA_BEARER_SO_MASK_CDMA_DONOT_CARE = 0x00,
    QMI_CURRENT_DATA_BEARER_SO_MASK_CDMA_1X_IS95 = 0x01,
    QMI_CURRENT_DATA_BEARER_SO_MASK_CDMA_1X_IS2000 = 0x02,
    QMI_CURRENT_DATA_BEARER_SO_MASK_CDMA_1X_IS2000_REL_A = 0x04
} QMI_CURRENT_DATA_BEARER_SO_MASK_CDMA_1X;

typedef enum {
    QMI_CURRENT_DATA_BEARER_SO_MASK_CDMA_EVDO_DONOT_CARE = 0x00,
    QMI_CURRENT_DATA_BEARER_SO_MASK_CDMA_EVDO_REVA_DPA = 0x01,
    QMI_CURRENT_DATA_BEARER_SO_MASK_CDMA_EVDO_REVA_MFPA = 0x02,
    QMI_CURRENT_DATA_BEARER_SO_MASK_CDMA_EVDO_REVA_EMPA = 0x04,
    QMI_CURRENT_DATA_BEARER_SO_MASK_CDMA_EVDO_REVA_EMPA_EHRPD = 0x08
} QMI_CURRENT_DATA_BEARER_SO_MASK_CDMA_EVDO;

struct getServingNetworkPara
{
    ULONG registrationState;
    ULONG CSDomain;
    ULONG PSDomain;
    ULONG RAN;
    BYTE  radioIfacesSize;
    BYTE  radioIfaces[RAT_MAX];
    ULONG roamingInd;
    WORD  MCC;
    WORD  MNC;
    CHAR  name[NUM_MAX_STR_LEN];
};

void registerSignalStrengthCallbackQMI(void);
void registerRFInfoCallbackQMI(void);
void registerDataBearerCallback(void);
void requestSignalStrengthQMI(void *data, size_t datalen, RIL_Token t);
void registerDormancyStatusCallbackQMI(bool enable);
void registerSLQSSetServingSystemCallbackQMI(void);
void registerSetBandPreferenceCallbackQMI(void);
#ifndef SWI_RIL_VERSION_6
void requestRegistrationStateUMTS(void *data, size_t datalen, RIL_Token t);
void requestGPRSRegistrationStateUMTS(void *data, size_t datalen, RIL_Token t);
#else
void requestVoiceRegistrationStateUMTS(void *data, size_t datalen, RIL_Token t);
void requestDataRegistrationStateUMTS(void *data, size_t datalen, RIL_Token t);
#endif
void requestOperatorUMTS(void *data, size_t datalen, RIL_Token t);
#ifndef SWI_RIL_VERSION_6
void requestRegistrationStateCDMA(void *data, size_t datalen, RIL_Token t);
#else
void requestVoiceRegistrationStateCDMA(void *data, size_t datalen, RIL_Token t);
void requestDataRegistrationStateCDMA(void *data, size_t datalen, RIL_Token t);
#endif
void requestOperatorCDMA(void *data, size_t datalen, RIL_Token t);
void requestCDMASetSubscriptionQMI(void *data, size_t datalen, RIL_Token t);
void requestCDMASubscriptionCDMA(void *data, size_t datalen, RIL_Token t);
void requestCurrentCallsQMI(void *data, size_t datalen, RIL_Token t);
void requestGetCDMARoamingPreferenceCDMA(void *data, size_t datalen, RIL_Token t);
void requestSetCDMARoamingPreferenceCDMA(void *data, size_t datalen, RIL_Token t);
void requestSetNetworkSelectionAutomaticUMTS(void *data, size_t datalen, RIL_Token t);
void requestSetNetworkSelectionManual(void *data, size_t datalen, RIL_Token t);
void requestSetNetworkSelectionManualUMTS(void *data, size_t datalen, RIL_Token t);
void requestQueryAvailableNetworks(void *data, size_t datalen, RIL_Token t);
void requestQueryAvailableNetworksUMTS(void *data, size_t datalen, RIL_Token t);
void requestQueryNetworkSelectionModeUMTS(void *data, size_t datalen, RIL_Token t);
void requestSetPreferredNetworkTypeQMI(void *data, size_t datalen, RIL_Token t);
void requestGetPreferredNetworkTypeQMI(void *data, size_t datalen, RIL_Token t);
void requestSetLocationUpdatesQMI(void *data, size_t datalen, RIL_Token t);
void requestScreenStateQMI(void *data, size_t datalen, RIL_Token t);
void requestQueryAvailableBandModeQMI(void *data, size_t datalen, RIL_Token t);
void requestSetBandModeQMI(void *data, size_t datalen, RIL_Token t);

struct getServingNetworkPara getServingNetworkParameters(void);
void printSLQSGetServingSystem(void *tqaQmiServingSystemParamp);
ULONG getServingNetworkQMI(BYTE *updatedp);
void sendNITZtimeATMC77xx(bool always_send);
void sendNITZtimeAT(bool always_send);
void sendNITZtimeQMI(QMI_REGISTERED_RAN regTechnology);
void pollRegistration(void *params);
void getServingNetworkErrHandle(void);
BYTE getServingRadioIface(void);
bool getServingDataCapability(BYTE *capp, bool isCdma);
int getRegistrationState(void);
void networkServiceInit(void);
SWI_RunningTech getRealtimeTechnology(void);
void switchTechnologyDualMode(void);
RIL_RAT getRatCDMA(void);
bool getEHRPDFromDataBearer(void);
void sendUnsolicitedCDMARegState(void);
void setRatCDMA(RIL_RAT rat);
bool getLacCellID(int *lac, ULONG *cellID);
void sendUnsolicitedUMTSRegState();
void updateWithDataBearer(RIL_RAT *rilRatp);

#endif
