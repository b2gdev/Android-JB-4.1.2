/**
 *
 * @ingroup swiril
 *
 * @file 
 * Prototypes for PDP context related QMI Sierra functions
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#ifndef RIL_PDP_QMI_H
#define RIL_PDP_QMI_H 1

#include <stdbool.h>

#define GOBI_DEFAULT_PROFILE_TYPE 0 /**< UMTS */
#define GOBI_DEFAULT_PDP_TYPE 0 /**< IP V4 */
#define GOBI_DATA_SESSION_TECHNOLOGY_UMTS 1 /**< UMTS */
#define GOBI_DATA_SESSION_TECHNOLOGY_CDMA 2 /**< CDMA */
#define GOBI_CALL_END_REASONS_CLIENT_END  2 /**< Client side end the data call */

/* 
 * From QCOM document "GobiConnectionMgmt.pdf, 80-VF219-10B", section 2.5.1
 */
typedef enum {
    PACKET_STATE_DISCONNECTED = 1,
    PACKET_STATE_CONNECTED,
    PACKET_STATE_SUSPENDED,
    PACKET_STATE_AUTHENTICATING
} Packet_SessionState;


typedef enum {
    QMI_TECH_UMTS = 1,
    QMI_TECH_CDMA2000
} QMI_PDPTech;

typedef enum {
    QMI_TRAFFIC_CHANNEL_DORMANT = 1,
    QMI_TRAFFIC_CHANNEL_ACTIVE
} QMI_DormancyState;

typedef enum {
    QMI_IPV4_ADDRESS = 4,
    QMI_IPV6_ADDRESS = 6,
    QMI_IPV4V6_ADDRESS = 7, /* this value is defined by SLQS not QCT */
    QMI_IP_ADDRESS_UNSPEC = 8
} QMI_IPFamilyPreference;

void registerSessionStateCallbackQMI(void);
void registerByteTotalsCallbackQMI(void);
void requestOrSendPDPContextListQMI(RIL_Token *t);
void onPDPContextListChangedQMI(void *param);
void requestPDPContextListQMI(void *data, size_t datalen, RIL_Token t);
void requestSetupDefaultPDPQMI(void *data, size_t datalen, RIL_Token t);
void requestDeactivateDefaultPDPQMI(void *data, size_t datalen, RIL_Token t);
void requestLastPDPFailCauseQMI(void *data, size_t datalen, RIL_Token t);

int getLastPdpFailCauseQMI(void);
void setLastPdpFailCauseQMI(int errCode);
int retrievePDPRejectCauseQMI(ULONG qmicode);
void requestOrSendPDPContextListQMI(RIL_Token *t);
bool isDataSessionActive(void);
void initDataCallResponseList(void);
bool isDataSessionCloseByClient(bool state);

#endif
