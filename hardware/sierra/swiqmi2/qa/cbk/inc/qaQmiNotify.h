/**************
 *
 * Filename: qaQmiNotify.h
 *
 * Purpose:  Header file for QMI Notify interface
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 **************/

#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#include "qmerrno.h"
#include "qaCbkDmsEventReportInd.h"
#include "qaCbkDmsEventReportInd.h"
#include "qaCbkWdsEventReportInd.h"
#include "qaCbkWdsGetPktSrvcStatusInd.h"
#include "qaCbkNasServingSystemInd.h"
#include "qaCbkNasEventReportInd.h"
#include "qaCbkNasSystemSelectionPreferenceInd.h"
#include "qaCbkNasSysInfoInd.h"
#include "qaCbkNasNetworkTimeInd.h"
#include "qaCbkPdsEventReportInd.h"
#include "qaCbkPdsGpsServiceStateInd.h"
#include "qaCbkWmsEventReportInd.h"
#include "qaCbkCatEventReportInd.h"
#include "qaCbkSwiOmaDmEventReportInd.h"
#include "qaCbkDcsEventReportInd.h"
#include "qaCbkFmsEventReportInd.h"
#include "qaCbkOmaDmEventReportInd.h"
#include "qaCbkVoiceUssdInd.h"
#include "qaCbkVoiceSUPSNotificationInd.h"
#include "qaCbkVoiceSLQSVoiceAllCallStatusInd.h"
#include "qaCbkWmsTransLayerInfoInd.h"
#include "qaCbkWmsTransNWRegInfoInd.h"
#include "qaCbkUimSLQSUimSetStatusChangeInd.h"
#include "qaCbkUimSLQSUimSetRefreshInd.h"
#include "qaCbkVoiceSLQSVoicePrivacyInd.h"
#include "qaCbkVoiceSLQSVoiceDTMFInd.h"
#include "qaCbkVoiceSLQSVoiceSUPSInd.h"
#include "qaCbkWmsMemoryFullInd.h"
#include "qaCbkVoiceSLQSVoiceSetOTASPStatusInd.h"
#include "qaCbkVoiceSLQSVoiceInfoRecInd.h"

/*
 * Name:    eQMICallbackIndex
 *
 * Purpose: Enumeration used to list Callback Index in the List. The enum values
 *          has been used in service levels to categorize better.
 */
enum eQMICallbackIndex{
    eQMI_CB_START = 0,

    /*---------------------
      WDS Service Callbacks
     ----------------------*/

    /* WDS_PKT_SRVC_STATUS_IND */
    eQMI_CB_SESSION_STATE = eQMI_CB_START, /* Data Bearer Callback */
    /* WDS_EVENT_REPORT_IND */
    eQMI_CB_WDS_START = eQMI_CB_SESSION_STATE,
    eQMI_CB_DATA_BEARER,                        /* Session State Callback */
    eQMI_CB_DORMANCY_STATUS,                    /* Dormancy Status Callback */
    eQMI_CB_MOBILE_IP,                          /* Mobile IP Callback */
    eQMI_CB_BYTE_TOTALS,                        /* Byte Totals Callback */
    eQMI_CB_WDS_END = eQMI_CB_BYTE_TOTALS,

    /*---------------------
      DMS Service Callbacks
     ----------------------*/

    /* DMS_EVENT_REPORT_IND */
    eQMI_CB_ACTIVATION_STATUS,                  /* Activation Status Callback */
    eQMI_CB_DMS_START =
        eQMI_CB_ACTIVATION_STATUS,
    eQMI_CB_POWER,                              /* Power Callback */
    eQMI_CB_DMS_END = eQMI_CB_POWER,

    /*---------------------
      NAS Service Callbacks
     ----------------------*/

    /* NAS_SERVING_SYSTEM_IND */
    eQMI_CB_ROAMING_INDICATOR,                  /* Roaming Indicator Callback */
    eQMI_CB_NAS_START =
        eQMI_CB_ROAMING_INDICATOR,
    eQMI_CB_DATA_CAPABILITIES,                  /* Data Capabilities Callback */
    /* NAS_EVENT_REPORT_IND */
    eQMI_CB_SIGNAL_STRENGTH,                    /* Signal Strength Callback */
    eQMI_CB_RF_INFO,                            /* RFT Info Callback */
    eQMI_CB_RSSI_INFO,                          /* Received Signal Strength */
    eQMI_CB_LU_REJECT,                          /* LU Reject Callback */
    eQMI_CB_NAS_SERV_SYS,                       /* Serving System Callback */
    eQMI_CB_NAS_BAND_PREF,                      /* Band Preference Callback */
    eQMI_CB_NAS_SYS_SEL_PREFERENCE,             /* System Selection Pref. */
    eQMI_CB_NAS_SYS_INFO_IND,                   /* System Info Indication */
    eQMI_CB_NAS_NETWORK_TIME_IND,               /* Network Time Indication */
    eQMI_CB_NAS_END = eQMI_CB_NAS_NETWORK_TIME_IND,

    /*---------------------
      PDS Service Callbacks
     ----------------------*/

    /* PDS_SERVING_SYSTEM_IND */
    eQMI_CB_NMEA,                               /* NMEA Callback */
    eQMI_CB_PDS_START = eQMI_CB_NMEA,
    eQMI_CB_NMEA_PLUS,                          /* NMEA Plus Callback */
    /* PDS_GPS_SERVICE_STATE_IND */
    eQMI_CB_PDS_STATE,                          /* PDS State Callback */
    eQMI_CB_PDS_END = eQMI_CB_PDS_STATE,

    /*---------------------
      WMS Service Callbacks
     ----------------------*/

    /* WMS_EVENT_REPORT_IND */
    eQMI_CB_NEW_SMS,                         /* New SMS Callback */
    eQMI_CB_WMS_START = eQMI_CB_NEW_SMS,
    eQMI_CB_WMS_TRANS_LAYER_INFO_IND,        /* Trans Layer info callback */
    eQMI_CB_WMS_TRANS_NW_REG_INFO_IND,       /* Trans NW Reg info callback */
    eQMI_CB_SMS_EVENT,                       /* SMS Event Callback */
    eQMI_CB_WMS_MEMORY_FULL,                 /* SMS Memory Full Callback*/
    eQMI_CB_WMS_END = eQMI_CB_WMS_MEMORY_FULL,

    /*---------------------
      CAT Service Callbacks
     ----------------------*/

    /* CAT_EVENT_REPORT_IND */
    eQMI_CB_CAT_EVENT,                          /* CAT Event Callback */
    eQMI_CB_CAT_START = eQMI_CB_CAT_EVENT,
    eQMI_CB_CAT_END = eQMI_CB_CAT_START,

    /*---------------------
      OMA Service Callbacks
     ----------------------*/

    /* OMA_EVENT_REPORT_IND */
    eQMI_CB_OMADM_ALERT,                        /* Roaming Indicator Callback */
    eQMI_CB_OMADM_START = eQMI_CB_OMADM_ALERT,
    eQMI_CB_OMADM_STATE,                        /* Roaming Indicator Callback */
    eQMI_CB_OMADM_END = eQMI_CB_OMADM_STATE,

    /*------------------------
      SWIOMA Service Callbacks
     ------------------------*/

    /* SWIOMA_EVENT_REPORT_IND */
    eQMI_CB_SWIOMADM_ALERT,                    /* OMADM Alert Callback */
    eQMI_CB_SWIOMADM_START = eQMI_CB_SWIOMADM_ALERT,
    eQMI_CB_SWIOMADM_STATE,                    /* OMADM State Callback */
    eQMI_CB_SWIOMADM_END = eQMI_CB_SWIOMADM_STATE,

    /*------------------------
      DCS Service Callbacks
     ------------------------*/

    /* DCS_EVENT_REPORT_IND */
    eQMI_CB_DCS_DEVICE_STATE_CHANGE,            /* Device State Callback */
    eQMI_CB_DCS_START = eQMI_CB_DCS_DEVICE_STATE_CHANGE,
    eQMI_CB_DCS_SDK_TERMINATED,                 /* SDK terminated Callback */
    eQMI_CB_DCS_END = eQMI_CB_DCS_SDK_TERMINATED,
    /*------------------------
      FMS Service Callbacks
     ------------------------*/

    /* FMS_EVENT_REPORT_IND */
    eQMI_CB_FMS_FW_DWLD_STATUS,
    eQMI_CB_FMS_START = eQMI_CB_FMS_FW_DWLD_STATUS,
    eQMI_CB_FMS_END   = eQMI_CB_FMS_FW_DWLD_STATUS,

    /*-----------------------
      Voice Service Callbacks
     ------------------------*/
    /* VOICE_USSD_IND */
    eQMI_CB_VOICE_USSD_RELEASE_IND,          /* USSD Release callback */
    eQMI_CB_VOICE_START = eQMI_CB_VOICE_USSD_RELEASE_IND,
    eQMI_CB_VOICE_USSD_IND,                  /* USSD Notification callback */
    eQMI_CB_VOICE_SUPS_NOTIFICATION_IND,     /* SUPS Notification callback */
    eQMI_CB_VOICE_ALL_CALL_STATUS,           /* Call Status Indication */
    eQMI_CB_VOICE_PRIVACY_IND,               /* Voice Privacy Indication */
    eQMI_CB_VOICE_DTMF_IND,                  /* DTMF Event Indication */
    eQMI_CB_VOICE_SUPS_IND,                  /* SUPS Indication */
    eQMI_CB_VOICE_OTASP_STATUS_IND,          /* OTASP/OTAPA Event Indication */
    eQMI_CB_VOICE_INFO_REC_IND,              /* Info Record Indication */
    eQMI_CB_USSD_END = eQMI_CB_VOICE_INFO_REC_IND,

    /*-----------------------
     UIM Service Callbacks
    ------------------------*/
    eQMI_CB_UIM_STATUS_CHANGE_IND,          /* UIM Status Change Callback */
    eQMI_CB_UIM_REFRESH_IND,                /* UIM Refresh CAllback */
    eQMI_CB_UIM_END = eQMI_CB_UIM_REFRESH_IND,

    eQMI_CB_END                                 /* End of Callback list */
};

/*
 * Name:    eQmiCbkSetStatus
 *
 * Purpose: Enumeration used to identify if the parameters for a callback
 *          function need to be set.
 */
enum eQmiCbkSetStatus{
    QMI_CBK_PARAM_RESET = 0,
    QMI_CBK_PARAM_SET   = 1,
    QMI_CBK_PARAM_NOCHANGE
};

/*
 * Name:    eQMICbkState
 *
 * Purpose: Enumeration used to track the state of Callback service.
 */
enum eQMICbkState{
    eQMI_CBK_INIT,
    eQMI_CBK_LISTENING
};

/* Set<>Callback function prototype */
typedef ULONG (* pCbkType)(void *);

/*************
 *
 * Name:    qaCallbackInfo
 *
 * Purpose: Structure for storing callback function data
 *
 * Members: pCallback      - Callback pointer
 *          pSetCallback   - Function pointer to enable the callback
 *          pCallbackCache - Function pointer to save callback provided by the
 *                           user in case device resets
 *
 * Notes:
 *
 **************/
struct qaCallbackInfo{
    void     *pCallback;
    pCbkType pSetCallback;
    void     *pCallbackCache;
};

/*
 * Name:    QmiNotification
 *
 * Purpose: Structure used to store all QMI Notification parameters.
 *
 * Members: eCbkIndex                            - Index to the Callback List
 *          QmiInd                               - Union Containing the QMI
 *                                                 indications
 *          QmiWdsEventStatusReportInd           - WDS Evevt Indication
 *                                                 Parameters
 *          qaQmiWdsSessionStateInd              - WDS Session State Indication
 *                                                 Parameters
 *          qaQmiCbkDmsEventStatusReportInd      - DMS Event Indication
 *                                                 Parameters
 *          qaQmiCbkNasServingSystemInd          - NAS Serving System Indication
 *                                                 Parameters
 *          qaQmiCbkNasEventStatusReportInd      - NAS Event Indication
 *                                                 Parameters
 *          qaQmiCbkNasSystemSelPrefInd          - NAS System Selection
 *                                                 Preference Indication
 *                                                 parameters
 *          qaQmiCbkPdsEventStatusReportInd      - PDS Event Indication
 *                                                 Parameters
 *          qaQmiCbkPdsGpsServiceStatusInd       - PDS GPS Service State
 *                                                 Indication Parameters
 *          qaQmiCbkWmsEventReportInd            - WMS Event Indication
 *                                                 parameters
 *          qaQmiCbkCatEventStatusReportInd      - CAT Event Indication
 *                                                 parameters
 *          qaQmiCbkSwiOmaDmEventStatusReportInd - SWIOMA Event Indication
 *                                                 parameters
 *          qaQmiCbkDcsEventStatusReportInd      - DCS Event Indication
 *                                                 parameters
 *          qaQmiCbkFmsEventStatusReportInd      - FMS Event Indication
 *                                                 parameters
 *          qaQmiCbkOmaDmEventStatusReportInd    - OMA Event Indication
 *                                                 parameters
 *          qaQmiCbkVoiceUssdInd                 - VOICE Event Indication
 *                                                 parameters
 *          qaQmiCbkVoiceSUPSNotificationInd     - SUPS Notification Indication
 *                                                 parameters
 *          qaQmiCbkVoiceAllCallStatusInd        - VOICE All Call Status
 *                                                 Indication parameters
 *          qaQmiCbkTransLayerNotificationInd    - WMS Transport Layer Info
 *                                                 Indication parameters
 *          qaQmiCbkTransNWRegNotificationInd    - WMS Transport Network Reg
 *                                                 notification indication
 *
 *          qaQmiCbkUIMRefreshInd                - UIM refresh Indication
 *
 *          qaQmiCbkUIMStatusChangeInd           - UIM status Change Indication
 *
 *          qaQmiCbkVoicePrivacyInd              - Voice Privacy Change
 *                                                 Indication parameters
 *
 *          qaQmiCbkVoiceDTMFEventInd            - Voice DTMF Event Indication
 *
 *          qaQmiCbkVoiceSupsInd                 - Voice SUPS Indication
 *
 *          qaQmiCbkNasSysInfo                   - System Info Indication
 *
 *          qaQmiCbkNasNetworkTime               - Network Time Indication
 *
 *          qaQmiCbkWmsMemoryFull                - Memory Full Indication
 *
 *          qaQmiCbkVoiceOTASPStatusInd          - OTASP Status Indication
 *
 *          qaQmiCbkVoiceInfoRecInd              - Info Record Indication
 *
 * Note:    None
 */
struct QmiNotification
{
    enum   eQMICallbackIndex  eCbkIndex;
    union
    {
        struct QmiCbkWdsEventStatusReportInd
                                    qaQmiCbkWdsEventStatusReportInd;

        struct QmiCbkWdsSessionStateInd  qaQmiCbkWdsSessionStateInd;

        struct QmiCbkDmsEventStatusReportInd
                                    qaQmiCbkDmsEventStatusReportInd;

        struct QmiCbkNasServingSystemInd qaQmiCbkNasServingSystemInd;

        struct QmiCbkNasEventStatusReportInd
                                    qaQmiCbkNasEventStatusReportInd;

        struct QmiCbkNasSystemSelPrefInd
                                    qaQmiCbkNasSystemSelPrefInd;

        struct QmiCbkPdsEventStatusReportInd
                                    qaQmiCbkPdsEventStatusReportInd;

        struct QmiCbkPdsGpsServiceStatusInd
                                    qaQmiCbkPdsGpsServiceStatusInd;

        struct QmiCbkWmsEventReportInd
                                    qaQmiCbkWmsEventReportInd;

        struct QmiCbkCatEventStatusReportInd
                                    qaQmiCbkCatEventStatusReportInd;

        struct QmiCbkSwiOmaDmEventStatusReportInd
                                    qaQmiCbkSwiOmaDmEventStatusReportInd;

        struct QmiCbkDcsEventStatusReportInd
                                    qaQmiCbkDcsEventStatusReportInd;

        struct QmiCbkFmsEventStatusReportInd
                                    qaQmiCbkFmsEventStatusReportInd;

        struct QmiCbkOmaDmEventStatusReportInd
                                    qaQmiCbkOmaDmEventStatusReportInd;

        struct QmiCbkVoiceUssdInd   qaQmiCbkVoiceUssdInd;

        voiceSUPSNotification       qaQmiCbkVoiceSUPSNotificationInd;

        voiceSetAllCallStatusCbkInfo
                                    qaQmiCbkVoiceAllCallStatusInd;

        transLayerNotification      qaQmiCbkTransLayerNotificationInd;

        transNWRegInfoNotification  qaQmiCbkTransNWRegNotificationInd;

        UIMRefreshEvent             qaQmiCbkUIMRefreshInd;

        UIMStatusChangeInfo         qaQmiCbkUIMStatusChangeInd;
        voicePrivacyInfo            qaQmiCbkVoicePrivacyInd;

        voiceDTMFEventInfo          qaQmiCbkVoiceDTMFEventInd;

        voiceSUPSInfo               qaQmiCbkVoiceSupsInd;

        nasSysInfo                  qaQmiCbkNasSysInfo;

        nasNetworkTime              qaQmiCbkNasNetworkTime;

        SMSMemoryInfo               qaQmiCbkWmsMemoryFull;

        voiceOTASPStatusInfo        qaQmiCbkVoiceOTASPStatusInd;

        voiceInfoRec                qaQmiCbkVoiceInfoRecInd;
    } QmiInd;
};

package enum eQCWWANError UpkQmiCbkWdsNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkNasNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkDmsNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkPdsNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkCatNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkWmsNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkSwiOmaDmNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkDcsNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkFmsNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkOmaDmNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkVoiceNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

package enum eQCWWANError UpkQmiCbkUIMNotification(
    USHORT                 QmiMsgID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp );

/* Function to get a notification callback */
package void *qaQmiGetCallback(
    enum eQMICallbackIndex eCbkIndex );

/* Function to set a notification callback */
package void qaQmiSetCallback(
    enum eQMICallbackIndex eCbkIndex,
    void                   *pCallback );

void qaNotifyInit(void);

extern void qaQmiRemoveAllCallbacks(void);

/* Function to receive device state change event */
extern void qaQmiDeviceStateChangeCbk(
    BYTE deviceState );

#ifdef __cplusplus
} /* extern "C" { */
#endif
