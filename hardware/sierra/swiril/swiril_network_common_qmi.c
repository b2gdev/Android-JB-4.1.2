/**
 *
 * @ingroup swiril
 *
 * @file 
 * Provides network related QMI common functions
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#include <telephony/ril.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
/* Needed for getting network time */
#include <at_channel.h>
#include <at_tok.h>
#include <at_misc.h>
#include <limits.h>
#include <cutils/properties.h>

#include "swiril_main.h"
#include "swiril_cache.h"
#include "swiril_misc_qmi.h"
#include "swiril_network_qmi.h"
#include "swiril_device_qmi.h"
#include "swiril_pdp_qmi.h"
#include "swiril_sim_qmi.h"

#include "SWIWWANCMAPI.h"
#include "qmerrno.h"
#include "qmudefs.h"

#define LOG_TAG "RIL"
#include "swiril_log.h"

#define SIGNAL_STRENGTH_THRESHOLDS_SIZE 5
#define REPOLL_REGSTATUS_SELECTED      30   /* 30 * 2 = 1M = ok? */
#define VOICE_NUMBER_STRING_LENGTH 16       /**< Voice number string length including NULL */
#define MIN_STRING_LENGTH 16                /**< MIN string length including NULL */
#define INSTANCESIZE      10                /* Assume up to 10 networks can be scanned */
#define GPS_LEAP_TIME     14                /* GPS time is ahead of UTC by fourteen (14) seconds */
#define QMI_ROAMING_INDICATOR_ROAMING 0
#define QMI_ROAMING_INDICATOR_HOME 1
#define RIL_REGISTRATION_REGISTERED_ROAMING 5
#define RIL_CDMA_SIGNAL_STRENGTH_DBM_DEFAULT 110
#define RIL_CDMA_SIGNAL_STRENGTH_SENSIBLE_DBM 105

/* Encoding scheme used for operator name */
#define ENCODING_SCHEME_GSM  0x00

typedef enum {
    QMI_AUTOMATIC = 0,
    QMI_HOME_ONLY,
    QMI_AFFILIATED_ROMING_ONLY,
    QMI_HOME_AND_AFFILIATED_ROAMING
} QMI_CDMA_Roaming_preference;

typedef enum {
    RIL_HOME_ONLY = 0,
    RIL_AFFILIATED_ROAMING,
    RIL_ANY_NETWORK
} RIL_CDMA_Roaming_preference;

/* structure declaration for function requestGetCDMARoamingPreferenceCDMA*/
typedef struct 
{
    BYTE SCI;
    BYTE SCM;
    BYTE RegHomeSID;
    BYTE RegForeignSID;
    BYTE RegForeignNID;
    BYTE ForceRev0;
    BYTE CustomSCP;
    ULONG Protocol;
    ULONG Broadcast;
    ULONG Application;
    ULONG Roaming;
} STRUCT_GetCDMANetworkParameters;

/* structure declaration for function requestGetPreferredNetworkTypeQMI*/
typedef struct 
{
    ULONG   TechnologyPreference; 
    ULONG   Duration; 
    ULONG   PersistentTechnologyPref;
} STRUCT_GetNetworkPreference;

/* structure declaration for function requestSetNetworkSelectionManualUMTS */
typedef struct 
{
    ULONG   regType; 
    WORD    mcc; 
    WORD    mnc;
    ULONG   rat;
} STRUCT_InitiateNetworkRegistration;

/* structure declaration for function requestQueryAvailableNetworksUMTS*/
typedef struct 
{
    WORD    mMcc; 
    WORD    mMnc; 
    ULONG   mInUse;
    ULONG   mRoaming;
    ULONG   mForbidden;
    ULONG   mPreferred;
    CHAR    mDescription[NUM_MAX_STR_LEN];
} STRUCT_PerformNetworkScan;

/* structure declaration for function requestSetPreferredNetworkTypeQMI*/
typedef struct 
{
    ULONG   TechnologyPreference; 
    ULONG   Duration; 
} STRUCT_SetNetworkPreference;

/* structure declaration for function registration status poll function */
typedef struct{
    RIL_Token t;
    int loopcount;
    ULONG nRegType; /*if registration succeed, set g_regType*/
}STRUCT_RegPollParams;

/* enum declaration for function Get/SetPreferredNetworkTypeQMI*/
typedef enum {
    SWI_RAT_3GPP2       =1<<0, /* Bit 0 Technology selected is 3GPP2 */
    SWI_RAT_3GPP        =1<<1, /* Bit 1 Technology selected is 3GPP */
    SWI_RAT_ANALOG      =1<<2, /* Bit 2 Analog AMPS if 3GPP2, GSM if 3GPP*/
    SWI_RAT_DIGITAL     =1<<3, /* Bit 3 Digital CDMA if 3GPP2, WCDMA if 3GPP */
    SWI_RAT_HDR         =1<<4, /* Bit 4 HDR */
    SWI_RAT_LTE         =1<<5  /* Bit 5 LTE */
} QMI_RatBitSelection;

/* enum declaration for function requestGetPreferredNetworkTypeQMI*/
typedef enum {
    QMI_RAT_AUTO        = 0, /* Automatic */
    QMI_RAT_GSM         = SWI_RAT_3GPP | SWI_RAT_ANALOG, /*6 GSM only */    
    QMI_RAT_WCDMA       = SWI_RAT_3GPP | SWI_RAT_DIGITAL,/*10(0xa) WCDMA */    
    QMI_RAT_GSM_WCDMA   = SWI_RAT_3GPP | SWI_RAT_ANALOG | SWI_RAT_DIGITAL, /*14(0xe) GSM and WCDMA*/
    QMI_RAT_CDMA        = SWI_RAT_3GPP2 | SWI_RAT_DIGITAL, /*9 CDMA only */    
    QMI_RAT_EVDO        = SWI_RAT_3GPP2 | SWI_RAT_HDR ,/*17(0x11) CDMA 1xEV-DO only*/    
    QMI_RAT_CDMA_EVDO   = SWI_RAT_3GPP2 | SWI_RAT_DIGITAL | SWI_RAT_HDR, /*25(0x19) CDMA and EvDo */    
    QMI_RAT_LTE_GSM_WCDMA = SWI_RAT_3GPP | SWI_RAT_ANALOG | SWI_RAT_DIGITAL |SWI_RAT_LTE, /* 46(0x2e)LTE, GSM/WCDMA */
    /* QMI_RAT_LTE_ONLY works for both MC7700 and MC7750 */
    QMI_RAT_LTE_ONLY      = SWI_RAT_3GPP | SWI_RAT_LTE /* 34(0x22) LTE only (3GPP)*/
} QMI_Rat_Technology; /* accroding to QMI_RatBitSelection*/

/* enum declaration for function requestSetPreferredNetworkTypeQMI*/
typedef enum {
    QMI_DURATION_PERSISTENT             = 0, /* Persistent */
    QMI_DURATION_POWER_CYCLE            = 1  /* Power cycle */
} QMI_Duration;

/* enum declaration for function requestSetNetworkSelectionAutomaticUMTS*/
typedef enum {
    RIL_REG_TYPE_AUTOMATIC             = 1, /* Automatic register type */
    RIL_REG_TYPE_MANUAL                = 2  /* Manual register type */
} RIL_RegType;

/* enum declaration for function requestSetNetworkSelectionAutomaticUMTS*/
typedef enum {
    QMI_REG_TYPE_AUTOMATIC             = 0, /* Automatic register type */
    QMI_REG_TYPE_MANUAL                = 1  /* Manual register type */
} QMI_RegType;

/* enum declaration for function requestSetPreferredNetworkTypeQMI*/
typedef enum {
    RIL_RAT_WCDMA_PREPERRED            = 0, /* GSM/WCDMA (WCDMA preferred) */
    RIL_RAT_GSM_ONLY                   = 1, /* GSM only */
    RIL_RAT_WCDMA_ONLY                 = 2, /* WCDMA only */
    RIL_RAT_GSM_WCDMA_AUTO             = 3, /* GSM/WCDMA (auto mode, according to PRL) */
    RIL_RAT_CDMA_EVDO_AUTO             = 4, /* CDMA and EvDo (auto mode, according to PRL) */
    RIL_RAT_CDMA_ONLY                  = 5, /* CDMA only */
    RIL_RAT_EVDO_ONLY                  = 6, /* EVDO only */
    RIL_RAT_GSM_WCDMA_CDMA_EVDO_AUTO   = 7, /* GSM/WCDMA, CDMA, and EvDo (auto mode, according to PRL) */
    RIL_RAT_LTE_CDMA_EVDO              = 8, /* LTE, CDMA and EvDo */
    RIL_RAT_LTE_GSM_WCDMA              = 9, /* LTE, GSM/WCDMA */
    RIL_RAT_LTE_CMDA_EVDO_GSM_WCDMA    = 10,/* LTE, CDMA, EvDo, GSM/WCDMA */
    RIL_RAT_LTE_ONLY                   = 11 /* LTE only */
} RIL_RatSelection;

/* structure declaration for function requestSetNetworkSelectionManualUMTS/requestSetNetworkSelectionAutomaticUMTS */
typedef struct 
{
    ULONG   regType; 
    WORD    mcc; 
    WORD    mnc;
} STRUCT_RegStatus;

struct ServingNWCaps
{
    BYTE    dataCapsSize;
    ULONG   dataCaps[RAT_MAX];
    bool    valid;
};

typedef struct nw_capabilities_lookup
{
  QMI_SERVING_NW_CAPABILITY nw_cap;     /**< QMI network capabilities */
  int order;                            /**< the order we prefer to select */
} STRUCT_nw_capabilities_lookup_type;

/** 
 * GSM/UMTS/LTE network capabilities lookup table
 */
static const STRUCT_nw_capabilities_lookup_type qmi_nw_caps_umts_table[] =
{
    {QMI_SERVING_NW_CAPABILITY_GPRS,                    2},
    {QMI_SERVING_NW_CAPABILITY_EDGE,                    3},
    {QMI_SERVING_NW_CAPABILITY_HSDPA,                   5},
    {QMI_SERVING_NW_CAPABILITY_HSUPA,                   6},
    {QMI_SERVING_NW_CAPABILITY_WCDMA,                   4},
    {QMI_SERVING_NW_CAPABILITY_CDMA_1XRTT,              0},
    {QMI_SERVING_NW_CAPABILITY_CDMA_1XEVDO_REV0,        0},
    {QMI_SERVING_NW_CAPABILITY_CDMA_1XEVDO_REVA,        0},
    {QMI_SERVING_NW_CAPABILITY_GSM,                     1},
    {QMI_SERVING_NW_CAPABILITY_CDMA_1XEVDO_REVB,        0},
    {QMI_SERVING_NW_CAPABILITY_LTE,                     9},
    {QMI_SERVING_NW_CAPABILITY_HSPA_PLUS,               7},
    {QMI_SERVING_NW_CAPABILITY_DUAL_CARRIER_HSPA_PLUS,  8},
};

/** 
 * CDMA network capabilities lookup table
 */
static const STRUCT_nw_capabilities_lookup_type qmi_nw_caps_cdma_table[] =
{
    {QMI_SERVING_NW_CAPABILITY_GPRS,                    0},
    {QMI_SERVING_NW_CAPABILITY_EDGE,                    0},
    {QMI_SERVING_NW_CAPABILITY_HSDPA,                   0},
    {QMI_SERVING_NW_CAPABILITY_HSUPA,                   0},
    {QMI_SERVING_NW_CAPABILITY_WCDMA,                   0},
    {QMI_SERVING_NW_CAPABILITY_CDMA_1XRTT,              1},
    {QMI_SERVING_NW_CAPABILITY_CDMA_1XEVDO_REV0,        2},
    {QMI_SERVING_NW_CAPABILITY_CDMA_1XEVDO_REVA,        3},
    {QMI_SERVING_NW_CAPABILITY_GSM,                     0},
    {QMI_SERVING_NW_CAPABILITY_CDMA_1XEVDO_REVB,        4},
    {QMI_SERVING_NW_CAPABILITY_LTE,                     0},
    {QMI_SERVING_NW_CAPABILITY_HSPA_PLUS,               0},
    {QMI_SERVING_NW_CAPABILITY_DUAL_CARRIER_HSPA_PLUS,  0},
};

/* structure declaration for function requestSetBandModeQMI */
typedef struct 
{
    BYTE   radioInterface;
    WORD   activeBandClass;
    WORD   activeChannel;
} STRUCT_RadioInfor;

/* enum declaration for function requestQueryAvailableBandModeQMI/requestSetBandModeQMI */
typedef enum {
    RIL_AUTOMATIC                      = 0, /* "unspecified" (selected by baseband automatically) */
    RIL_EURO_GSM_900_1800_WCDMA_2000   = 1, /* "EURO band"(GSM-900/DCS-1800/WCDMA-IMT-2000) */
    RIL_US_WCDMA_850_1900_GSM_850_1900 = 2, /* "US band"(GSM-850/PCS-1900/WCDMA-850/WCDMA-PCS-1900) */
    RIL_JPN_WCDMA_800_2000             = 3, /* "JPN band" (WCDMA-800 / WCDMA-IMT-2000) */
    RIL_AUS_GSM_900_1800_WCDMA_850_2000= 4, /* "AUS band"(GSM-900/DCS-1800/WCDMA-850/WCDMA-IMT-2000) */
    RIL_AUS2_GSM_900_1800_WCDMA_850    = 5, /* "AUS band 2" (GSM-900 / DCS-1800 / WCDMA-850) */
    RIL_CELLULAR_800                   = 6, /* "Cellular (800-MHz Band)" */
    RIL_PCS_1900                       = 7, /* "PCS (1900-MHz Band)" */
    RIL_BAND_CLASS_3                   = 8, /* "Band Class 3 (JTACS Band)" */    
    RIL_BAND_CLASS_4                   = 9, /* "Band Class 4 (Korean PCS Band)" */
    RIL_BAND_CLASS_5                   = 10,/* "Band Class 5 (450-MHz Band)" */
    RIL_BAND_CLASS_6                   = 11,/* "Band Class 6 (2-GMHz IMT2000 Band)" */
    RIL_BAND_CLASS_7                   = 12,/* "Band Class 7 (Upper 700-MHz Band)" */
    RIL_BAND_CLASS_8                   = 13,/* "Band Class 8 (1800-MHz Band)" */
    RIL_BAND_CLASS_9                   = 14,/* "Band Class 9 (900-MHz Band)" */
    RIL_BAND_CLASS_10                  = 15,/* "Band Class 10 (Secondary 800-MHz Band)" */
    RIL_BAND_CLASS_11                  = 16,/* "Band Class 11 (400-MHz European PAMR Band)" */
    RIL_BAND_CLASS_15                  = 17,/* "Band Class 15 (AWS Band)" */
    RIL_BAND_CLASS_16                  = 18,/* "Band Class 16 (US 2.5-GHz Band)" */
    RIL_BAND_MAX                       = 19
} RIL_BandSelection;

/* enum declaration for function requestQueryAvailableBandModeQMI/requestSetBandModeQMI */
typedef enum {
    QMI_BAND_CLASS_0A              = 0, /* CDMA Band Class 0 A-system */
    QMI_BAND_CLASS_0B              = 1, /* CDMA Band Class 0 B-system */
    QMI_BAND_CLASS_1               = 2, /* CDMA Band Class 1 */
    QMI_BAND_CLASS_2               = 3, /* CDMA Band Class 2 */
    QMI_BAND_CLASS_3               = 4, /* CDMA Band Class 3 */
    QMI_BAND_CLASS_4               = 5, /* CDMA Band Class 4 */
    QMI_BAND_CLASS_5               = 6, /* CDMA Band Class 5 */
    QMI_GSM_DCS_1800               = 7, /* GSM DCS 1800 */
    QMI_GSM_900_EXTEND             = 8, /* GSM 900 (Extended) */
    QMI_GSM_900_PRIMARY            = 9, /* GSM 900 (Primary) */
    QMI_BAND_CLASS_6               = 10, /* CDMA Band Class 6 */
    QMI_BAND_CLASS_7               = 11, /* CDMA Band Class 7 */
    QMI_BAND_CLASS_8               = 12, /* CDMA Band Class 8 */
    QMI_BAND_CLASS_9               = 13, /* CDMA Band Class 9 */
    QMI_BAND_CLASS_10              = 14, /* CDMA Band Class 10 */
    QMI_BAND_CLASS_11              = 15, /* CDMA Band Class 11 */
    QMI_GSM_450                    = 16, /* GSM 450 */
    QMI_GSM_480                    = 17, /* GSM 480 */
    QMI_GSM_750                    = 18, /* GSM 750 */
    QMI_GSM_850                    = 19, /* GSM 850 */
    QMI_GSM_900_RAILWAYS           = 20, /* GSM 900 (Railways) */
    QMI_GSM_PCS_1900               = 21, /* GSM 1900 */
    QMI_WCDMA_I_2100               = 22, /* WCDMA EUROPE JAPAN and CHINA IMT 2100 band */
    QMI_WCDMA_II_PCS_1900          = 23, /* WCDMA US PCS 1900 */
    QMI_WCDMA_III_1800             = 24, /* WCDMA EUROPE and CHINA DCS 1800 */
    QMI_WCDMA_IV_1700              = 25, /* WCDMA 1700 (U.S.) */
    QMI_WCDMA_V_850                = 26, /* WCDMA 850 US */
    QMI_WCDMA_VI_800               = 27, /* WCDMA 800 */
    QMI_BAND_CLASS_12              = 28, /* CDMA Band Class 12 */
    QMI_BAND_CLASS_14              = 29, /* CDMA Band Class 14 */
    QMI_BAND_CLASS_15              = 31, /* CDMA Band Class 15 */
    QMI_WCDMA_VII_2600             = 48, /* WCDMA EUROPE 2600 */
    QMI_WCDMA_VIII_900             = 49, /* WCDMA EUROPE and JAPAN 900 */
    QMI_WCDMA_IX_1700              = 50, /* WCDMA JAPAN 1700 band */
    QMI_BAND_CLASS_16              = 56, /* CDMA Band Class 16 */
    QMI_BAND_CLASS_17              = 57, /* CDMA Band Class 17 */
    QMI_BAND_CLASS_18              = 58, /* CDMA Band Class 18 */
    QMI_BAND_CLASS_19              = 59, /* CDMA Band Class 19 */
    QMI_WCDMA_X_850                = 60, /* WCDMA JAPAN 850 band */
    QMI_WCDMA_XI_1500              = 61  /* WCDMA JAPAN 1500 band */
} QMI_BandSelection;                      
                                          
static ULONGLONG g_CurrentBand; /* Current band */
static STRUCT_RegStatus g_regStatus = {RIL_REG_TYPE_AUTOMATIC, 0, 0}; /* Default Set to Automatic registration */
static ULONG g_TechnoledgeType = RIL_RAT_GSM_WCDMA_CDMA_EVDO_AUTO; /*GSM/WCDMA/CDMA/EvDo*/
static struct getServingNetworkPara s_servingNetworkParaOld; /**< local cache */
static struct ServingNWCaps s_servingNetworkCapabilitiesOld;
static const struct timeval TIMEVAL_REGISTER_STATUS_POLL = { 2, 0 };
static int TimeZone = 255;
/** cache for CDMA RAT, it allows RIL_RAT_UNKNOWN or RIL_RAT_EHRPD */
static RIL_RAT s_ratCDMA = RIL_RAT_UNKNOWN; 

/** cache for RAT for MC8355 in UMTS mode */
static RIL_RAT s_ratUMTS = RIL_RAT_UNKNOWN;
static bool s_ratUMTS_UpdateEnabled = false;

/* This can be used to block auto/man registration attempts until an initial
   network scan has been performed.
 */
static bool s_initialScanPerformed = false;

/* SWI_TBD
 * the following 2 variables need to be revisited */
extern pthread_mutex_t s_screen_state_mutex;
extern int s_screenState;

void networkServiceInit(void)
{
    memset(&s_servingNetworkParaOld, 0, sizeof(struct getServingNetworkPara));
    s_servingNetworkParaOld.registrationState = QMI_REG_STATE_SEARCHING_NOT_REGISTERED;
    memset(&s_servingNetworkCapabilitiesOld, 0, sizeof(struct ServingNWCaps));
}

struct getServingNetworkPara getServingNetworkParameters(void)
{
    return s_servingNetworkParaOld;
}

SWI_RunningTech getRealtimeTechnology(void)
{
    SWI_RunningTech tech = SWI_RUNNING_TECH_UNKNOWN;
    
    if (s_servingNetworkParaOld.registrationState == QMI_REG_STATE_REGISTERED) {
        /* it only needs to determine GSM or CDMA */
        switch(s_servingNetworkParaOld.radioIfaces[0]) {
            case RAT_CDMA_1XRTT:
            case RAT_CDMA_1XEVDO:
                tech = SWI_RUNNING_TECH_CDMA;
                break;
                            
            case RAT_GSM:
            case RAT_UMTS:
                tech = SWI_RUNNING_TECH_GSM;
                break; 

            case RAT_LTE:    
                tech = SWI_RUNNING_TECH_LTE;
                break; 
                
            case RAT_NO_SERVICE:
            case RAT_AMPS:
            default:
                break;
        }
    }
    return tech;
}

/**
 *
 * Update the cache s_ratCDMA value
 * 
 * @param rat
 *     rat value to be set
 *
 * @return
 *     None
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
void setRatCDMA(RIL_RAT rat)
{
    s_ratCDMA = rat;
}

/**
 *
 * Return the cache of s_ratCDMA
 *
 * @return
 *     RIL_RAT_EHRPD
 *     RIL_RAT_UNKNOWN
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
RIL_RAT getRatCDMA(void)
{
      return s_ratCDMA;
}

/**
 *
 * When data bearer is eHRPD triggering an unsolicited registration state message
 *
 * @return
 *     true  - if domancy state is active
 *     false - otherwise
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
bool getEHRPDFromDataBearer(void)
{
    ULONG nRet;
    QmiWDSDataBearers dataBearerInfo;
    QmiWDSDataBearerTechnology sCurDataBearerTechnology;
    ULONG dormancyState;

    dataBearerInfo.pCurDataBearerTechnology = &sCurDataBearerTechnology;
    dataBearerInfo.pLastCallDataBearerTechnology = NULL;

    /* assume data bearer info valid when dormancy state is active(2) */
    if (isDataSessionActive()) {
        nRet = GetDormancyState(&dormancyState);
        if ((nRet == eQCWWAN_ERR_NONE) && (dormancyState == QMI_TRAFFIC_CHANNEL_ACTIVE)) {
            nRet = SLQSGetDataBearerTechnology( &dataBearerInfo );

            if ((nRet == eQCWWAN_ERR_NONE) && 
                (dataBearerInfo.dataBearerMask & QMI_WDS_CURRENT_CALL_DB_MASK )) {
                QmiWDSDataBearerTechnology *pResp =
                         dataBearerInfo.pCurDataBearerTechnology;

                LOGD("%s Databearer %x\n", __func__, pResp->currentNetwork );
                LOGD("ratMask %lu\n", pResp->ratMask );
                LOGD("soMask %lu\n", pResp->soMask );

                if ((pResp->currentNetwork == QMI_CURRENT_DATA_BEARER_CURRENT_NETWORK_3GPP2) &&
                    (pResp->ratMask == QMI_CURRENT_DATA_BEARER_RAT_MASK_CDMA_EVDO_REVA) &&
                    (pResp->soMask == QMI_CURRENT_DATA_BEARER_SO_MASK_CDMA_EVDO_REVA_EMPA_EHRPD)) {
                    LOGD("%s found eHRPD", __func__);
                    s_ratCDMA = RIL_RAT_EHRPD;
                    sendUnsolicitedCDMARegState();
                }
                return true;
            }
            else
                LOGE("%s: SLQSGetDataBearerTechnology() failed %lu\n", __func__, nRet );
        }
    }
    return false;
}
    
void getServingNetworkErrHandle(void)
{
    LOGE("RegistrationState must never return an error when radio is on.");
    /* SWI_TBD 
     * assume it only happens on startup before modem gets network info,
     * clean cache and let it go as normal */
    networkServiceInit();
}

static bool isUSCarrier(void)
{
    WORD curMCC = s_servingNetworkParaOld.MCC;
    WORD curMNC = s_servingNetworkParaOld.MNC;
    bool ret = false;

    if((curMCC >= 310) && (curMCC <= 316 ))
    {
        if((curMCC == 310) && (curMNC == 110))
            ret = false;
        else if((curMCC == 310) && (curMNC == 140))
            ret = false;
        else if((curMCC == 310) && (curMNC == 400))
            ret = false;
        else if((curMCC == 310) && (curMNC == 470))
            ret = false;
        else if((curMCC == 311) && (curMNC == 170))
            ret = false;
        else
            ret = true;
    }
    return ret;
}

/**
 * <CDR-NWS-550> Definition of International Roaming of ATT
 *
 * Summary: For the purposes of this feature only, the UE shall consider
 *
 * itself to be roaming internationally under the following conditions:
 *
 * • When the MCC of the serving PLMN is not in the range 310 to 316
 *
 * • When the serving PLMN code is one of the following:
 *
 * – 310-110 - Pacific Telecom
 *
 * – 310-140 - Pulse Mobile
 *
 * – 310-400 - Guam Wireless/WaveRunner
 *
 * – 310-470 - Northern Marianna Islands
 *
 * – 311-170 - Petrocom
 *
 * Note: These PLMNs use a US-based MCC, but operate
 *
 * outside of mainland USA and are to be considered as
 *
 * international roaming carriers by AT&T.
 *
 */

int getRegistrationState(void)
{
    int state = (int) s_servingNetworkParaOld.registrationState;
    SWI_FW_INFO_TYPE tech;
    enum eGobiImageCarrier image;
    
    /* MOTO issue #188, added USA carrier checking for ATT */
    /* ATT doesn't like to show roaming indicator in US base carrier */
    if(getFirmwareInfoTechQMI(&tech) && (tech == SWI_FW_INFO_TYPE_GOBI_UMTS) && 
       getFirmwareInfoCarrierQMI(&image) && (image == eGOBI_IMG_CAR_ATT))
    {
        if(isUSCarrier())
            return state;
    }

    if ((s_servingNetworkParaOld.registrationState == QMI_REG_STATE_REGISTERED) &&
        (s_servingNetworkParaOld.roamingInd == QMI_ROAMING_INDICATOR_ROAMING)) {
        state = RIL_REGISTRATION_REGISTERED_ROAMING;
    }
    return state;
}
/**
 *
 * Notify Java layer to switch between GSM and CDMA Object 
 * based on module's realtime technology
 *
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     This function intend to notify Java layer
 *     when there is no active data call.
 */
void switchTechnologyDualMode(void)
{
    if (isDualModeRunningCDMA() && (currentState() != RADIO_STATE_RUIM_READY)) {
        /* notify Java layer switching to CDMA */
        LOGI("%s: set RADIO_STATE_RUIM_READY for CDMA", __func__);
        /* module registed to the network already, so no delay required */
        setRadioState(RADIO_STATE_RUIM_READY);
    }
    else if (isDualModeRunningLTE() && (currentState() != RADIO_STATE_SIM_READY)) {
        /* notify Java layer switching to GSM */
        LOGI("%s: set RADIO_STATE_SIM_READY for GSM", __func__);
        setRadioState(RADIO_STATE_SIM_READY);
    }
    else
        LOGD("%s no need to switch", __func__);
}
                
ULONG getServingNetworkQMI(BYTE *updatedp)
{
    ULONG nRet;
    BYTE  count;
    struct getServingNetworkPara servingNetworkPara;
    BYTE  pDataCapsSize = RAT_MAX;
    ULONG pDataCaps[RAT_MAX]; 

    nRet = GetServingNetworkCapabilities( &pDataCapsSize,
                                          (BYTE *)&pDataCaps[0] );
    if (nRet == eQCWWAN_ERR_NONE) {
        if ((pDataCapsSize != s_servingNetworkCapabilitiesOld.dataCapsSize) ||  
            (memcmp(&pDataCaps[0], &s_servingNetworkCapabilitiesOld.dataCaps[0], pDataCapsSize*sizeof(pDataCaps[0])) != 0)) {
            s_servingNetworkCapabilitiesOld.dataCapsSize = pDataCapsSize;
            memcpy(&s_servingNetworkCapabilitiesOld.dataCaps[0], &pDataCaps[0], pDataCapsSize*sizeof(pDataCaps[0]));
            s_servingNetworkCapabilitiesOld.valid = true;
            *updatedp = SWI_SERVICE_STATUS_UPDATED;
            /* print out when updated */ 
            for(count = 0; count < pDataCapsSize; count++)
                LOGD("%s pDataCaps[%d]  : %lu\n", __func__, count, pDataCaps[count]);
            /* Serving Network Capabilities have changed, so clear the network cache data */
            swiril_cache_clearnetworkdata();
        }
    }
    else {
        LOGE("%s GetServingNetworkCapabilities() error: %lu", __func__, nRet);
        /* Query failed clear cache, set flag for update */
        memset(&s_servingNetworkCapabilitiesOld, 0, sizeof(struct ServingNWCaps));
        *updatedp = SWI_SERVICE_STATUS_UPDATED;
    }
    
    memset(&servingNetworkPara, 0, sizeof(struct getServingNetworkPara));
    servingNetworkPara.radioIfacesSize = RAT_MAX;
    
    nRet = GetServingNetwork(&servingNetworkPara.registrationState,
                             &servingNetworkPara.CSDomain,
                             &servingNetworkPara.PSDomain,
                             &servingNetworkPara.RAN,
                             &servingNetworkPara.radioIfacesSize,
                             &servingNetworkPara.radioIfaces[0],
                             &servingNetworkPara.roamingInd,
                             &servingNetworkPara.MCC,
                             &servingNetworkPara.MNC,
                             sizeof(servingNetworkPara.name),
                             &servingNetworkPara.name[0]);

    if (nRet == eQCWWAN_ERR_NONE) {
        if (memcmp(&servingNetworkPara, &s_servingNetworkParaOld, sizeof(struct getServingNetworkPara)) != 0) {
            /* check whether only registration status changed, network time will be sent only when it changed */
            if((s_servingNetworkParaOld.registrationState != QMI_REG_STATE_REGISTERED) && 
                (servingNetworkPara.registrationState == QMI_REG_STATE_REGISTERED)) {
                *updatedp = SWI_REGISTRATION_STATUS_UPDATED; 

                /* now that we are registered, send SIM_READY if it is pending */
                sendPendingSIMREADY();
            }
            else
                *updatedp = SWI_SERVICE_STATUS_UPDATED;

            /* Serving Network Parameters have changed, so clear the network cache data */
            swiril_cache_clearnetworkdata();
            memcpy(&s_servingNetworkParaOld, &servingNetworkPara, sizeof(struct getServingNetworkPara));
            
            /* print out when updated */ 
            LOGD("%s registrationState  : %lu\n", __func__, servingNetworkPara.registrationState);
            LOGD("CSDomain  : %lu\n", servingNetworkPara.CSDomain);
            LOGD("PSDomain : %lu\n", servingNetworkPara.PSDomain);
            LOGD("RAN  : %lu\n", servingNetworkPara.RAN);
            LOGD("radioIfacesSize  : %d\n", servingNetworkPara.radioIfacesSize);
            for(count = 0; count < servingNetworkPara.radioIfacesSize; count++)
                LOGD("radioIfaces[%d]  : %d\n", count, servingNetworkPara.radioIfaces[count]);
            LOGD("roamingInd  : %lu\n", servingNetworkPara.roamingInd);
            LOGD("MCC  : %d\n", servingNetworkPara.MCC);
            LOGD("MNC  : %d\n", servingNetworkPara.MNC);
            LOGD("Name  : %s\n", servingNetworkPara.name);
            /* set g_regStatus when registration succeed  */
            g_regStatus.mcc = servingNetworkPara.MCC;
            g_regStatus.mnc = servingNetworkPara.MNC;  

            /* SWI_TBD MC7750-like modules notify technology switch */
            /* switchTechnologyDualMode(); */
        }
    }
    else {
        LOGE("%s GetServingNetwork() error: %lu", __func__, nRet);
    }
    
    return nRet;
}

void printSLQSGetServingSystem(void *tqaQmiServingSystemParamp)
{
    qaQmiServingSystemParam *qaQmiServingSystemParamp = (qaQmiServingSystemParam *)tqaQmiServingSystemParamp;
    
    LOGD("SLQSGetServingSystem info:\n");
    LOGD("systemID %d:\n", qaQmiServingSystemParamp->SystemID);
    LOGD("networkID %d:\n", qaQmiServingSystemParamp->NetworkID);
    LOGD("basestationID %d:\n", qaQmiServingSystemParamp->BasestationID);
    LOGD("basestationLatitude %lu:\n", qaQmiServingSystemParamp->BasestationLatitude);
    LOGD("basestationLongitude %lu:\n", qaQmiServingSystemParamp->BasestationLongitude);
    LOGD("Lac %d:\n", qaQmiServingSystemParamp->Lac);
    LOGD("cellID %lu:\n", qaQmiServingSystemParamp->CellID);
    LOGD("3GppTimeZone %d:\n", qaQmiServingSystemParamp->GppTimeZone);
    LOGD("3Gpp2TimeZone leapSeconds %d:\n", qaQmiServingSystemParamp->Gpp2TimeZone.leapSeconds);
    LOGD("3Gpp2TimeZone localTimeOffset %d:\n", qaQmiServingSystemParamp->Gpp2TimeZone.localTimeOffset);
    LOGD("3Gpp2TimeZone daylightSavings %d:\n", qaQmiServingSystemParamp->Gpp2TimeZone.daylightSavings);    
}

/**
 * RIL_UNSOL_NITZ_TIME_RECEIVED
 *
 *  function to indicate network time 
 */
void sendNITZtimeAT(bool always_send)
{
    ATResponse *atresponse = NULL;
    int err;
    char *line;
    char *response;
    char *datetimep;
    int tz=0;
    int year = 0;
    char chYear[3] = "";

    err = at_send_command_singleline("AT+CCLK?", "+CCLK:", &atresponse);
    if (err < 0 || atresponse->success == 0) {
        LOGE("%s: 'AT+CCLK?' failed \n", __func__);
        goto error;
    }

    /*
     * Parse the AT command output
     */
    line=atresponse->p_intermediates->line;
    err = at_tok_start(&line);
    if (err < 0){
        LOGE("%s: at_tok_start failed \n", __func__);
        goto error;
    }

    err = at_tok_nextstr(&line, &datetimep);
    if (err < 0){
        LOGE("%s: at_tok_nextstr for date &time failed \n", __func__);
        goto error;
    }

    strncpy(chYear, datetimep, 2);
    year = atoi(chYear);
    if(year < 11 || year >= 80) {
        LOGE("%s: invalid year: %d\n", __func__, year);
        goto error;
    }
    
    /* The +CCLK has non-standard output with one doublequote character between
       the time and tz info. This is not handled properly by at_tok_nextstr() 
       above, so it stops the string token at the doublequote (by replacing it 
       with '\0') and ignores everything after it. Treat this doublequote as a 
       delimiter and start the next token right after it.
     */
    line = datetimep + strlen(datetimep) + 1;
    err = at_tok_nextint(&line, &tz);
    if (err < 0) {
        LOGE("%s: at_tok_nextstr for time zone failed \n", __func__);
        goto error;
    }

    /* Compose the unsolicited response data and send it */
    asprintf(&response, "%s%+i", datetimep, tz);
    if (always_send || TimeZone != tz){
        RIL_onUnsolicitedResponse(RIL_UNSOL_NITZ_TIME_RECEIVED,
                              response,
                              sizeof(char *));
        TimeZone = tz;
    }
    
    free(response);

error:
    at_response_free(atresponse);
}

/**
 * RIL_UNSOL_NITZ_TIME_RECEIVED
 *
 *  function to indicate network time 
 */
void sendNITZtimeATMC77xx(bool always_send)
{
    ATResponse *atresponse = NULL;
    int err;
    char *line;
    char *response;
    int count;
    ATLine *atlinep = NULL;
    char *datep;
    char *timep;
    int tz;
    int dst;

    err = at_send_command_multivalue("AT!UTCTIME?", "!UTCTIME:", &atresponse);

    if (err < 0 || atresponse->success == 0) {
        goto error;
    }

    /* Make sure there are exactly 5 intermediate response lines */
    for (count=0, atlinep=atresponse->p_intermediates;
            atlinep!=NULL; count++, atlinep=atlinep->p_next);

    if (count != 5) {
        LOGE("sendNITZtime: %i lines in AT!UTCTIME? output ", count);
        goto error;
    }

    /*
     * Parse the AT command output
     */

    /* Get date, discarding line containing prefix.
       The line does not use standard format; the whole line is the value,
       but skip over the first two digits of the year, and only use the 
       last two digits.
     */
    atlinep=atresponse->p_intermediates->p_next;
    datep = atlinep->line+2;
    
    /* Get UTC time
       The line does not use standard format; the whole line is the value.
     */
    atlinep=atlinep->p_next;
    timep = atlinep->line;

    /* Get TZ */
    atlinep=atlinep->p_next;
    line = atlinep->line;

    err = at_tok_start(&line);
    if (err < 0)
        goto error;

    err = at_tok_nextint(&line, &tz);
    if (err < 0)
        goto error;

    /* Get DST */
    atlinep=atlinep->p_next;
    line = atlinep->line;

    err = at_tok_start(&line);
    if (err < 0)
        goto error;

    err = at_tok_nextint(&line, &dst);
    if (err < 0)
        goto error;
          
    /* Compose the unsolicited response data and send it */
    asprintf(&response, "%s,%s%+i,%i", datep, timep, tz, dst);
    if (always_send || TimeZone != tz){
        RIL_onUnsolicitedResponse(RIL_UNSOL_NITZ_TIME_RECEIVED,
                                  response,
                                  strlen(response)+1);
        TimeZone = tz;
    }
    free(response);

error:
    at_response_free(atresponse);
}

void sendNITZtimeQMI(QMI_REGISTERED_RAN regTechnology)
{
    char      *response = NULL;
    ULONGLONG TimeStamp;
    ULONG     TimeSource;
    ULONGLONG ElapseSecsGpsEpoch;
    struct tm tmGpsEpoch, utc;
    time_t    unixsec  = 0;
    time_t    SecondsDiffGpsAndUnix = 0;
    int       gmtoff = 0;
    int       daylightsaving = 0;
    ULONG     nRet = 0;
    qaQmiServingSystemParam tqaQmiServingSystemParam;

    /* clear structure */
    memset(&tmGpsEpoch, 0, sizeof(struct tm));
    memset(&utc, 0, sizeof(struct tm));
    memset(&tqaQmiServingSystemParam, 0, sizeof(qaQmiServingSystemParam));

    /* call Sierra QMI interface */
    nRet = GetNetworkTime(&TimeStamp, &TimeSource);
    if (nRet != eQCWWAN_ERR_NONE ){
        LOGE("sendNITZtimeQMI : GetNetworkTime failed, nRet = %lu\n", nRet);
        return;
    }
        
    LOGD("sendNITZtimeQMI : GetNetworkTime Succeed, time: %llu, time source %lu\n", TimeStamp, TimeSource);
    ElapseSecsGpsEpoch = TimeStamp *1.25/1000; /* convert TimeStamp from 1.25ms to seconds */

    /*set GPS epoch time to 1980/01/06/00:00:00 */
    tmGpsEpoch.tm_year = 80;
    tmGpsEpoch.tm_mon = 0; /* month start at 0*/
    tmGpsEpoch.tm_mday = 6;
    
    /* Get the seconds difference of GPS epoch and unix epoch time */
    SecondsDiffGpsAndUnix = mktime(&tmGpsEpoch);
    SecondsDiffGpsAndUnix += tmGpsEpoch.tm_gmtoff; /* adjust time to UTC time*/

    /* Get UTC time*/
    unixsec = ElapseSecsGpsEpoch + SecondsDiffGpsAndUnix - GPS_LEAP_TIME;
    gmtime_r(&unixsec, &utc); 

    /* if time is before 2011, regard it as invalid, return without sending anything */
    if(utc.tm_year < 111) {
        LOGE("%s: invalid year: %d\n", __func__, utc.tm_year);
        return; 
    }

    /* Get Time Zone and day light saving info from QMI interface */
    nRet = SLQSGetServingSystem(&tqaQmiServingSystemParam);
    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGE("SLQSGetServingSystem: failed %lu\n", nRet );
        gmtoff = utc.tm_gmtoff/60/15;
        if(utc.tm_isdst == 1)
            daylightsaving = utc.tm_isdst;
    } else {
        if(regTechnology == QMI_REGISTERED_RAN_UMTS) {
            if(tqaQmiServingSystemParam.GppTimeZone != 0xff)
                /* difference between local time and Universal time, in increments of 15 min. (signed value)*/
                gmtoff = (signed char)tqaQmiServingSystemParam.GppTimeZone;
            else 
                gmtoff = utc.tm_gmtoff/60/15;
            /* SWI_TBD currently day light saving adjustment cannot be get from QMI interface */
            if(utc.tm_isdst == 1)
                daylightsaving = utc.tm_isdst;
        }
        else if(regTechnology == QMI_REGISTERED_RAN_CDMA2000) {
            /* START MOTO 167 */
           /* Check if the device is actually registered to a network.
            * If not registered, ignore NITZ Time as it is invalid */
           if( tqaQmiServingSystemParam.SystemID == 0xffff ||
               tqaQmiServingSystemParam.NetworkID == 0xffff )
           {
               LOGE("%s: CDMA Device not registered. Ignoring device time\n", __func__ );
               return;
           }            
            /* STOP MOTO 167 */
            if(tqaQmiServingSystemParam.Gpp2TimeZone.localTimeOffset != 0xff){
                /* Offset of local time from system time in units of 30 min */
                gmtoff = ((signed char)tqaQmiServingSystemParam.Gpp2TimeZone.localTimeOffset)/2*4;
            } else  
                gmtoff = utc.tm_gmtoff/60/15;
            /* SWI_TBD currently day light saving adjustment cannot be get from QMI interface */
            if(tqaQmiServingSystemParam.Gpp2TimeZone.daylightSavings != 0xff) {
                if(tqaQmiServingSystemParam.Gpp2TimeZone.daylightSavings == 1)
                    daylightsaving = 1;
            }
            else if(utc.tm_isdst == 1)
                daylightsaving = utc.tm_isdst;
        }
    }
        
    /* adjustment for year, in unix, the year cannot exceed 100 year*/
    if(utc.tm_year >= 100)
        utc.tm_year -= 100;

    utc.tm_mon += 1; /* month start at 0 */

    /* Compose the unsolicited response data in the form "yy/mm/dd,hh:mm:ss(+/-)tz,dt"
         and send it out */
    asprintf(&response, "%02d/%02d/%02d,%02d:%02d:%02d%s%d,%i", 
                utc.tm_year,
                utc.tm_mon,
                utc.tm_mday,
                utc.tm_hour,
                utc.tm_min,
                utc.tm_sec,
                gmtoff >= 0 ? "+" : "-",
                (int)abs(gmtoff), 
                daylightsaving);

    LOGD("sendNITZtimeQMI: %s\n", response);
    RIL_onUnsolicitedResponse(RIL_UNSOL_NITZ_TIME_RECEIVED,
                              response,
                              strlen(response)+1);
    free(response);
}


/**
 * The following data bearer related functions are for the MC8355 in UMTS 
 * mode, to identify network technologies above UMTS.  This is needed because
 * on some networks, the data capability is not above UMTS, so this alternative
 * method is needed.
 */

/**
 * Map from select data bearer values to RIL_RAT values.
 */
static RIL_RAT getRadioTechFromDataBearerTech(ULONG dataBearerTech)
{
    RIL_RAT networkType = RIL_RAT_UNKNOWN;

    switch(dataBearerTech) {
        /* START MOTO 157 */
        case DATA_BEARER_TECH_HSDPA_WCDMA:
            networkType = RIL_RAT_HSDPA;
            break; 
        case DATA_BEARER_TECH_WCDMA_HSUPA:
            networkType = RIL_RAT_HSUPA;
            break; 
        case DATA_BEARER_TECH_HSDPA_HSUPA:
            networkType = RIL_RAT_HSPA;
            break; 
        case DATA_BEARER_TECH_LTE:
            networkType = RIL_RAT_LTE;
        case DATA_BEARER_TECH_HSDPAPLUS_WCDMA:
        case DATA_BEARER_TECH_HSDPAPLUS_HSUPA:
        case DATA_BEARER_TECH_DCHSDPAPLUS_WCDMA:
        case DATA_BEARER_TECH_DCHSDPAPLUS_HSUPA:
        case DATA_BEARER_TECH_HSDPAPLUS_64QAM:
        case DATA_BEARER_TECH_HSDPAPLUS_64QAM_HSUPA:
            networkType = RIL_RAT_HSPAP;
            break; 
        /* STOP MOTO 157 */
        
        #ifdef REPORT_TECH_BY_DATA_BEARER
        /* START MOTO 179 */
        case DATA_BEARER_TECH_WCDMA:
            networkType = RIL_RAT_UMTS;
            break;
        /* STOP MOTO 179 */
        #endif
        
        default:
            networkType = RIL_RAT_UNKNOWN;
            break; 
    }
    LOGD("%s:: DataBearerTech = 0x%lx, networkType =%d", __func__, dataBearerTech, networkType);

    return networkType;
}


/**
 *
 * Poll data bearer which was called from databearer call back function
 *
 * @param[in] params 
 *     Pointer to the data bearer technology
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     None
 */
static void pollDataBearer(void *params)
{
    RIL_RAT *rilRat = NULL;

    assert(params != NULL);

    rilRat = (RIL_RAT *)params;
    
    LOGD("%s:: s_ratUMTS_UpdateEnabled = %d, s_ratUMTS =%d" " *rilRat =%d \n" , __func__, s_ratUMTS_UpdateEnabled, s_ratUMTS, *rilRat);
    
    /* Update stored data bearer if enabled */
    if (s_ratUMTS_UpdateEnabled) {
        
        /* START MOTO 179 */
        #ifdef REPORT_TECH_BY_DATA_BEARER
             s_ratUMTS = *rilRat;
            /* Send unsolicited registration Data State message */
            sendUnsolicitedUMTSRegState();
        #else
        if ((s_ratUMTS == RIL_RAT_UNKNOWN) || (*rilRat > s_ratUMTS)) 
        {
             s_ratUMTS = *rilRat;
            /* Send unsolicited registration Data State message */
            sendUnsolicitedUMTSRegState();
        }        
        #endif
        /* STOP MOTO 179 */
    }
    free(params);
}

static void setDataBearerCB(ULONG dataBearer)
{
    ULONG *poll_params = NULL;
    RIL_RAT rilRat;
    SWI_FW_INFO_TYPE fwtype;

    LOGD("%s:: dataBearer: %lu\n", __func__, dataBearer );

    rilRat = getRadioTechFromDataBearerTech(dataBearer);
    if (rilRat != RIL_RAT_UNKNOWN) {       
        poll_params = malloc(sizeof(rilRat));
        *poll_params = rilRat;

        enqueueRILEvent(RIL_EVENT_QUEUE_NORMAL, pollDataBearer,
                        poll_params, NULL);
    }
}

void registerDataBearerCallback(void)
{
    ULONG nRet;

    nRet = SetDataBearerCallback(&setDataBearerCB);

    if( nRet != eQCWWAN_ERR_NONE ) {
        LOGE("registerDataBearerCallback: failed to enable callback %lu\n", nRet);
    }
}

/** 
 * MC8355 modems in UMTS mode don't always report technology above UMTS using
 * other methods, and so we must rely on the data bearer. rilRatp will only be
 * updated if the necessary conditions are met.
 *
 * @param [in/out] rilRatp
 *     Data bearer technology 
 *
 * @return
 *      none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     None
 */
void updateWithDataBearer(RIL_RAT *rilRatp)
{
    SWI_FW_INFO_TYPE fwtype;

    /* Is it the correct type of modem */
    if (getFirmwareInfoTechQMI(&fwtype) && (fwtype==SWI_FW_INFO_TYPE_GOBI_UMTS)) {
        
        LOGD("%s:: *rilRatp = 0x%lx, fwtype = 0x%lx \n", __func__, *rilRatp,fwtype);

        /* Is it valid to use the data bearer */
         /* START MOTO 179 */
        #ifdef REPORT_TECH_BY_DATA_BEARER 
        if((*rilRatp == RIL_RAT_UMTS)||(*rilRatp == RIL_RAT_HSDPA)||
            (*rilRatp == RIL_RAT_HSUPA)||(*rilRatp == RIL_RAT_HSPA)||
            (*rilRatp ==RIL_RAT_HSPAP))
        #else
        if (*rilRatp == RIL_RAT_UMTS)
        #endif
        /* STOP MOTO 179 */
        {
            /* If not enabled, then enable data bearer updates */
            if (!s_ratUMTS_UpdateEnabled) {
                s_ratUMTS_UpdateEnabled = true;
                s_ratUMTS = RIL_RAT_UNKNOWN;
            } else {
                if (s_ratUMTS != RIL_RAT_UNKNOWN) {
                    *rilRatp = s_ratUMTS;
                }
            }
        } else {
            /* Don't need data bearer info, so stop updates */
            s_ratUMTS_UpdateEnabled = false;
        }
    }
}    


void getMobileIPSetting(void)
{
    ULONG nRet;
    ULONG pMode;
    
    nRet = GetMobileIP(&pMode);
    if ( nRet == eQCWWAN_ERR_NONE ) {
        LOGD("getMobileIPSetting mode: %lu", pMode);
    }
    else {
        LOGE("getMobileIPSetting: failed to enable callback %lu\n", nRet );
    }
}

BYTE getActiveMobileIPProfile(void)
{
    ULONG nRet;
    BYTE pIndex = 0xFF;
    
    nRet = GetActiveMobileIPProfile(&pIndex);
    if ( nRet == eQCWWAN_ERR_NONE ) {
        LOGD("getActiveMobileIPProfile index: %d", pIndex);
    }
    else {
        LOGE("getActiveMobileIPProfile failed: %lu\n", nRet );
    }
    
    return pIndex;
}

/**
 *
 * Look for the biggest radio interface number from radio interface list
 * as the serving radio interface
 *
 * @return
 *     biggest radio interface number found
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     Only observed CDMA mode returning more than one radio interface 
 *     in radio interface list 
 */
BYTE getServingRadioIface(void)
{
    BYTE radioIface;
    BYTE count;
    
    radioIface = s_servingNetworkParaOld.radioIfaces[0];
    for (count = 0; count < s_servingNetworkParaOld.radioIfacesSize; count++) {
        if (radioIface < s_servingNetworkParaOld.radioIfaces[count])
            radioIface = s_servingNetworkParaOld.radioIfaces[count];
    }
    return radioIface;
}

/**
 *
 * Look for the preferred data capability number from data capability list
 * as the serving data capability
 *
 * @param[out] capp 
 *     Pointer to preferred data capability number found
 * @param isCdma 
 *     true  - when look for CDMA data capability
 *     false - otherwise
 * @return
 *     ture - if data capability valid
 *     false - otherwise
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     None 
 */
bool getServingDataCapability(BYTE *capp, bool isCdma)
{
    BYTE cap;
    BYTE count;
    BYTE index;
    bool iRet = false; 

    if (s_servingNetworkCapabilitiesOld.valid && (s_servingNetworkCapabilitiesOld.dataCapsSize > 0)) {
        cap = s_servingNetworkCapabilitiesOld.dataCaps[0];
        for (count = 1; count < s_servingNetworkCapabilitiesOld.dataCapsSize; count++) {
            /* QMI_SERVING_NW_CAPABILITY starts from 1 and qmi_nw_caps_cdma_table/qmi_nw_caps_umts_table starts from 0 */
            index = s_servingNetworkCapabilitiesOld.dataCaps[count] - 1;
            if (isCdma) {
                if ((s_servingNetworkCapabilitiesOld.dataCaps[count] == qmi_nw_caps_cdma_table[index].nw_cap) &&
                    (qmi_nw_caps_cdma_table[cap-1].order < qmi_nw_caps_cdma_table[index].order)) {
                    cap = s_servingNetworkCapabilitiesOld.dataCaps[count];
                }
            }
            else {
                if ((s_servingNetworkCapabilitiesOld.dataCaps[count] == qmi_nw_caps_umts_table[index].nw_cap) &&
                    (qmi_nw_caps_umts_table[cap-1].order < qmi_nw_caps_umts_table[index].order)) {
                    cap = s_servingNetworkCapabilitiesOld.dataCaps[count];
                }
            }
        }
        *capp = cap;
        iRet = true;
    }
    
    return iRet;
}
        
void getMobileIPProfile(BYTE index)
{
    ULONG nRet;
    BYTE                       pEnabled;
    ULONG                      pAddress;
    ULONG                      pPrimaryHA;
    ULONG                      pSecondaryHA;
    BYTE                       pRevTunneling;
    BYTE                       naiSize = 100;
    CHAR                       pNAI[100];
    ULONG                      pHASPI;
    ULONG                      pAAASPI;
    ULONG                      pHAState;
    ULONG                      pAAAState;
    
    nRet = GetMobileIPProfile(  index,
                                &pEnabled,
                                &pAddress,
                                &pPrimaryHA,
                                &pSecondaryHA,
                                &pRevTunneling,
                                naiSize,
                                &pNAI[0],
                                &pHASPI,
                                &pAAASPI,
                                &pHAState,
                                &pAAAState);
                        
    if ( nRet == eQCWWAN_ERR_NONE ) {
        LOGD("getMobileIPProfile pEnabled: %d", pEnabled);
        LOGD("getMobileIPProfile pAddress: %lu", pAddress);
        LOGD("getMobileIPProfile pPrimaryHA: %lu", pPrimaryHA);
        LOGD("getMobileIPProfile pRevTunneling: %lu", pSecondaryHA);
        LOGD("getMobileIPProfile pSecondaryHA: %d", pRevTunneling);
        LOGD("getMobileIPProfile pNAI: %s", pNAI);
        LOGD("getMobileIPProfile pHASPI: %lu", pHASPI);
        LOGD("getMobileIPProfile pAAASPI: %lu", pAAASPI);
        LOGD("getMobileIPProfile pHAState: %lu", pHAState);
        LOGD("getMobileIPProfile pAAAState: %lu", pAAAState);
    }
    else {
        LOGE("getMobileIPProfile failed: %lu\n", nRet );
    }
}

/**
 *
 * Covert GSM/UMTS/LTE rssi value to signal strength defined in TS 27.007 8.5
 *
 * @param radioSignalStrength 
 *     rssi value
 *
 * @return
 *     convertion result of signal strength
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     None
 */
static int getGSMSignalValue(int radioSignalStrength)
{
    int signalValue = 0;
    
    /* TS 27.007 8.5
     * <rssi>:
     * 0    ‑113 dBm or less
     * 1    ‑111 dBm
     * 2...30                ‑109... ‑53 dBm
     * 31  ‑51 dBm or greater
     * 99  not known or not detectable
     */
    if ((-radioSignalStrength < -113)) {
        signalValue = 0;
    }
    else if (-radioSignalStrength > -51) {
        signalValue = 31;
    }
    else {
        signalValue = 30 + ((53 - radioSignalStrength) / 2);
    }
    return signalValue;
}

void requestSignalStrengthQMI(void *data, size_t datalen, RIL_Token t)
{
#ifndef SWI_RIL_VERSION_6
    RIL_SignalStrength signalStrength;
#else
    RIL_SignalStrength_v6 signalStrength;
    ATResponse *atresponse = NULL;
    int err;
    char *line;
    int rsrp = -1;
#endif

    ULONG nRet;
    ULONG pArraySizes = RAT_MAX;
    INT8  pSignalStrengths[RAT_MAX];
    ULONG pRadioInterfaces[RAT_MAX];
    int radioSignalStrength;
    int i;
  
    /*initialize first, Java layer cares about some fields */
    signalStrength.CDMA_SignalStrength.dbm = RIL_CDMA_SIGNAL_STRENGTH_DBM_DEFAULT;
    signalStrength.EVDO_SignalStrength.dbm = RIL_CDMA_SIGNAL_STRENGTH_DBM_DEFAULT;
    signalStrength.GW_SignalStrength.signalStrength = RIL_GSM_SIGNAL_STRENGTH_DEFAULT;
    signalStrength.GW_SignalStrength.bitErrorRate = RIL_GSM_SIGNAL_BIT_ERROR_RATE_DEFAULT;
    /* hardcode these 3 vaules, set to the value higher than full bars threshold */
    signalStrength.CDMA_SignalStrength.ecio = 85;
    signalStrength.EVDO_SignalStrength.ecio = 85;
    signalStrength.EVDO_SignalStrength.signalNoiseRatio = 8;

#ifdef SWI_RIL_VERSION_6
    /* set all LTE fields to -1 */
    signalStrength.LTE_SignalStrength.signalStrength = RIL_LTE_SIGNAL_STRENGTH_UNUSED;
    signalStrength.LTE_SignalStrength.rsrp = RIL_LTE_SIGNAL_STRENGTH_UNUSED;
    signalStrength.LTE_SignalStrength.rsrq = RIL_LTE_SIGNAL_STRENGTH_UNUSED;
    signalStrength.LTE_SignalStrength.rssnr = RIL_LTE_SIGNAL_STRENGTH_UNUSED;
    signalStrength.LTE_SignalStrength.cqi = RIL_LTE_SIGNAL_STRENGTH_UNUSED;
#endif

    nRet = GetSignalStrengths(&pArraySizes,
                              &pSignalStrengths[0],
                              &pRadioInterfaces[0]);

    if (nRet == eQCWWAN_ERR_NONE) {
        for (i = 0; i < (int)pArraySizes; i++) {
            LOGD("%s pRadioInterfaces[%d]=%lu pSignalStrengths[%d]=%d\n", 
            __func__, i, pRadioInterfaces[i], i, pSignalStrengths[i]);
            /* RIL uses postive value */
            radioSignalStrength = -pSignalStrengths[i];
            
            /* SWI_TBD Don't know how to get bitErrorRate, ecio and signalNoiseRatio, hardcode for now */
            switch(pRadioInterfaces[i]) {
                case RAT_CDMA_1XRTT:
                    signalStrength.CDMA_SignalStrength.dbm = radioSignalStrength;
                    break;
                    
                case RAT_CDMA_1XEVDO:
                    signalStrength.EVDO_SignalStrength.dbm = radioSignalStrength;
                    break;
        
                case RAT_GSM:
                case RAT_UMTS:
#ifndef SWI_RIL_VERSION_6
                case RAT_LTE:
#endif
                    signalStrength.GW_SignalStrength.signalStrength = getGSMSignalValue(radioSignalStrength);
                    break;

#ifdef SWI_RIL_VERSION_6
                case RAT_LTE: {
                    /* signal strength */
                    signalStrength.LTE_SignalStrength.signalStrength = getGSMSignalValue(radioSignalStrength);
                    /* RSRQ */
                    /* Hardcode to high range value first in case query failure */
                    signalStrength.LTE_SignalStrength.rsrq = 3;
                    struct slqsSignalStrengthInfo signalInfo;
                    /* Mask all the bits of "Request Mask" to retrieve complete info */
                    signalInfo.signalStrengthReqMask = 0xFF;
                    nRet = SLQSGetSignalStrength( &signalInfo );
                    if (nRet == eQCWWAN_ERR_NONE) {
                        LOGD("RSRQ            : %d\n", signalInfo.rsrqInfo.rsrq );
                        LOGD("Radio Interface : %d\n", signalInfo.rsrqInfo.radioIf );
                        if ((signalInfo.rsrqInfo.radioIf == RAT_LTE) && 
                            (signalInfo.rsrqInfo.rsrq >= -20) && (signalInfo.rsrqInfo.rsrq <= -3)) {
                            signalStrength.LTE_SignalStrength.rsrq = -signalInfo.rsrqInfo.rsrq;
                        }
                    }
                    do {
                        /* initialize rsrp to INT_MAX */
                        signalStrength.LTE_SignalStrength.rsrp = INT_MAX;

                        /* Get rsrp from AT command ""AT!GSTATUS?" */
                        err = at_send_command_singleline("AT!GSTATUS?", "RSRP (dBm):", &atresponse);

                        if (err < 0 || atresponse->success == 0) 
                            break;

                        line = atresponse->p_intermediates->line;
                        err = at_tok_start(&line);
                        if (err < 0) 
                            break;

                        err = at_tok_nextint(&line, &rsrp);
                        if (err < 0) 
                            break;

                        LOGI("%s: rsrp=%i", __func__, rsrp);
                        signalStrength.LTE_SignalStrength.rsrp = rsrp;
                    } while (0);
                    at_response_free(atresponse);
                    
                    /* SWI_TBD Don't know how to get rssnr and cqi, hardcode to use INT_MAX according to ril.h */
                    signalStrength.LTE_SignalStrength.rssnr = INT_MAX;
                    signalStrength.LTE_SignalStrength.cqi = INT_MAX;
                    break;
                }
#endif
                case RAT_NO_SERVICE:
                case RAT_AMPS:
                default:
                    break;
            }
        }
        /* fill in signal strength for both 1xRtt and 1xEvDo with the same value 
         * if only one interface available or the value lower than CDMA sensitivity.
         * Android uses positive number for signal strength, it has to use reverse logic.
         */
        if ((signalStrength.CDMA_SignalStrength.dbm >= RIL_CDMA_SIGNAL_STRENGTH_DBM_DEFAULT) &&
            (signalStrength.EVDO_SignalStrength.dbm <= RIL_CDMA_SIGNAL_STRENGTH_SENSIBLE_DBM)) {
            signalStrength.CDMA_SignalStrength.dbm = signalStrength.EVDO_SignalStrength.dbm;
        }
        else if ((signalStrength.CDMA_SignalStrength.dbm <= RIL_CDMA_SIGNAL_STRENGTH_SENSIBLE_DBM) &&
                 (signalStrength.EVDO_SignalStrength.dbm >= RIL_CDMA_SIGNAL_STRENGTH_DBM_DEFAULT)) {
            signalStrength.EVDO_SignalStrength.dbm = signalStrength.CDMA_SignalStrength.dbm;
        }
    }
    else {
        /* SWI_TBD 
           According to ril.h, RIL_E_GENERIC_FAILURE is not a valid error for
           this command.  Return default value and won't update cache.
         */
        if (t != NULL) {
            LOGE("%s must never return an error when radio is on %lu", __func__, nRet);
            RIL_onRequestComplete(t, RIL_E_SUCCESS, &signalStrength, sizeof(signalStrength));
        }
        return;
    }

    if (t != NULL) {
        swiril_cache_RIL_onRequestComplete(t, RIL_E_SUCCESS,
                              &signalStrength,
                              sizeof(signalStrength),
                              RIL_REQUEST_SIGNAL_STRENGTH);
    }
    else {
        void *responsep;
        int responselen;
        
        swiril_cache_get(RIL_REQUEST_SIGNAL_STRENGTH, &responsep, &responselen);
        
        /* Send notification when any field changed compared with cache */
        if ((responsep == NULL) ||
            ((responsep != NULL) && 
             (memcmp((char *)&signalStrength, (char *)responsep, responselen) != 0))) {
            swiril_cache_set(RIL_REQUEST_SIGNAL_STRENGTH,
                            &signalStrength,
                            sizeof(signalStrength));
                                
            RIL_onUnsolicitedResponse(RIL_UNSOL_SIGNAL_STRENGTH,
                                      &signalStrength,
                                      sizeof(signalStrength));
        }
    }
}

void pollSignalStrength(void *params)
{
    void *data = NULL;
    
    requestSignalStrengthQMI(data, 0, NULL);
}

void signalStrengthCB(
    INT8  signalStrength,
    ULONG radioInterface )
{

    LOGD("signalStrengthCB Signal Strength: %d\n", signalStrength );
    LOGD("signalStrengthCB Radio Interface: %lu\n", radioInterface );
    
    /* poll signal strength once to get signal strength of all available interfaces, 
     * sounds like no way to call QMI API in callback 
     */
    enqueueRILEvent(RIL_EVENT_QUEUE_NORMAL, pollSignalStrength,
                    NULL, NULL);
}

/*************
 *
 * Name:    registerSignalStrengthCallbackQMI
 *
 * Purpose: SetSignalStrengthCallback API driver function for RIL_UNSOL_SIGNAL_STRENGTH
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
void registerSignalStrengthCallbackQMI(void)
{
    ULONG nRet;
    /* SWI_TDB UMTS and CDMA thresholds may be different, but Windows RIL is using the same values now */
    INT8 thresholds[SIGNAL_STRENGTH_THRESHOLDS_SIZE]  = {-106,-100,-93,-86,-80};

    nRet = SetSignalStrengthCallback(&signalStrengthCB,
                                    SIGNAL_STRENGTH_THRESHOLDS_SIZE,thresholds);

    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGE("setSignalStrengthCallback: failed to enable callback %lu\n", nRet );
    }
}

static void setRFInfoCallbackCB(
    ULONG radioInterface,
    ULONG activeBandClass,
    ULONG activeChannel )
{
    LOGD("Radio Interface: %lx\n", radioInterface );
    LOGD("Active Band Class: %lx\n", activeBandClass );
    LOGD("Active Channel: %lx\n", activeChannel );
}

void registerRFInfoCallbackQMI(void)
{
    ULONG nRet;

    nRet = SetRFInfoCallback(&setRFInfoCallbackCB );

    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGE("doSetRFInfoCallback: failed to enable callback %lu\n", nRet );
    }
}


/** RIL_REQUEST_CDMA_SET_SUBSCRIPTION
 *
 * Request to set the location where the CDMA subscription shall
 * be retrieved
 */
void requestCDMASetSubscriptionQMI(void *data, size_t datalen, RIL_Token t)
{
    /* always return success now */
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
}

/** RIL_REQUEST_CDMA_SUBSCRIPTION
 *
 * Request the device MDN / H_SID / H_NID.
 */
void requestCDMASubscriptionCDMA(void *data, size_t datalen, RIL_Token t)
{
    ULONG nRet;
    ULONG nRet1;
    ULONG nRet2;
    CHAR pVoiceNumber[VOICE_NUMBER_STRING_LENGTH];
    CHAR pMIN[MIN_STRING_LENGTH];
    char *responseStr[5];
    WORD  prlVersion = 0;
    WORD  pMCC = 0;
    WORD  pMNC = 0;
    CHAR  pName[NUM_MAX_STR_LEN];
    WORD  pSID = 0;
    WORD  pNID = 0;

    memset(responseStr, 0, sizeof(responseStr));
    
    nRet = GetVoiceNumber( sizeof(pVoiceNumber),
                           pVoiceNumber,
                           sizeof(pMIN),
                           pMIN );

    nRet1 = GetPRLVersion(&prlVersion);

    nRet2 = GetHomeNetwork(&pMCC,
                          &pMNC,
                          sizeof(pName),
                          &pName[0],
                          &pSID,
                          &pNID);
    
    LOGD("%s:: nRet: %lu nRet1: %lu nRet2: %lu", __func__, nRet, nRet1, nRet2);
 
    if (nRet == eQCWWAN_ERR_NONE) {
        LOGD(   "Voice Number : %s\n", pVoiceNumber );
        LOGD(   "Home SID : %d\n", pSID );
        LOGD(   "Home NID : %d\n", pNID );
        LOGD(   "MIN: %s\n", pMIN );
        LOGD(   "PRL: %d\n", prlVersion );
        
        responseStr[0] = pVoiceNumber;
        asprintf(&responseStr[1], "%d", pSID);
        asprintf(&responseStr[2], "%d", pNID);
        responseStr[3] = pMIN;
        asprintf(&responseStr[4], "%d", prlVersion);
        /* Return the successful response */
        RIL_onRequestComplete(t, RIL_E_SUCCESS, responseStr,
                                           5 * sizeof(char *));
    }
    else {
        LOGE("%s:: GetVoiceNumber fail %lu.", __func__, nRet);
        RIL_onRequestComplete(t, RIL_E_SUBSCRIPTION_NOT_AVAILABLE, NULL, 0);
    }

    if (responseStr[1])
        free(responseStr[1]);    
    if (responseStr[2])
        free(responseStr[2]);
    if (responseStr[4])
        free(responseStr[4]);
}

/**
 * RIL_REQUEST_OPERATOR
 *
 * Request current operator ONS or EONS.
 */
void requestOperatorCDMA(void *data, size_t datalen, RIL_Token t)
{
    char *response[3];
    SWI_FW_INFO_TYPE tech;
    enum eGobiImageCarrier image;

    memset(response, 0, sizeof(response));
    /* Determine the MC8355 CDMA operator name by carrier image */ 
    if (getFirmwareInfoTechQMI(&tech) && 
        (tech == SWI_FW_INFO_TYPE_GOBI_CDMA) && 
        getFirmwareInfoCarrierQMI(&image)) {
        if (image == eGOBI_IMG_CAR_SPRINT) {
            response[0] = "Sprint";
            response[1] = "Sprint";
        }
        else if (image == eGOBI_IMG_CAR_VERIZON) {
            response[0] = "Verizon";
            response[1] = "Verizon";
        }
    }
    else if (isDualModeRunningCDMA()) {
        /* MC7750 CDMA operator name sticks to "Verizon" */
        response[0] = "Verizon";
        response[1] = "Verizon";
    }
    
    swiril_cache_RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(response),
                                       RIL_REQUEST_OPERATOR);
}


/**
 * RIL_REQUEST_OPERATOR
 *
 * Request current operator ONS or EONS.
 *
 * SWI_TBD: This is mostly a copy of requestOperator() in swiril_network.c,
 * but with different handling in case of empty names.  It would be good, if
 * possible, to merge the two implementations somehow.
 */
void requestOperatorAT(void *data, size_t datalen, RIL_Token t)
{
    int err;
    int i;
    int skip;
    ATLine *cursor;
    char *response[3];
    ATResponse *atresponse = NULL;

    memset(response, 0, sizeof(response));

    err = at_send_command_multiline
        ("AT+COPS=3,0;+COPS?;+COPS=3,1;+COPS?;+COPS=3,2;+COPS?", "+COPS:",
         &atresponse);

    /* We expect 3 lines here:
     * +COPS: 0,0,"T - Mobile"
     * +COPS: 0,1,"TMO"
     * +COPS: 0,2,"310170"
     */

    if (err < 0)
        goto error;

    for (i = 0, cursor = atresponse->p_intermediates; cursor != NULL;
         cursor = cursor->p_next, i++) {
        char *line = cursor->line;

        err = at_tok_start(&line);

        if (err < 0)
            goto error;

        err = at_tok_nextint(&line, &skip);

        if (err < 0)
            goto error;

        /* If we're unregistered, we may just get
           a "+COPS: 0" response. */
        if (!at_tok_hasmore(&line)) {
            response[i] = NULL;
            continue;
        }

        err = at_tok_nextint(&line, &skip);

        if (err < 0)
            goto error;

        /* A "+COPS: 0, n" response is also possible. */
        if (!at_tok_hasmore(&line)) {
            response[i] = NULL;
            continue;
        }

        err = at_tok_nextstr(&line, &(response[i]));

        if (err < 0)
            goto error;
    }

    if (i != 3)
        goto error;


    /* 
     * Check if modem returned an empty string, and fill it with the
     * name returned by QMI, if it is valid.  Otherwise fill it with
     * the numeric name.
     */
    if (response[0] && strlen(response[0]) == 0) {
        if (strlen(s_servingNetworkParaOld.name) == 0) {
            response[0] = alloca(strlen(response[2]) + 1);
            strcpy(response[0], response[2]);
        } else
            asprintf(&response[0], "%s", &s_servingNetworkParaOld.name[0]);
    }

    if (response[1] && strlen(response[1]) == 0) {
        if (strlen(s_servingNetworkParaOld.name) == 0) {
            response[1] = alloca(strlen(response[2]) + 1);
            strcpy(response[1], response[2]);
        } else
            asprintf(&response[1], "%s", &s_servingNetworkParaOld.name[0]);
    }

    /* Return the successful response and cache the value for next time */
    swiril_cache_RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(response),
                                       RIL_REQUEST_OPERATOR);

finally:
    at_response_free(atresponse);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    goto finally;
}


/**
 * Fetch the operator name, MCC and MNC
 *
 * @param[in] data
 *     Pointer to the request data
 * @param datalen 
 *     request data length
 * @param t
 *     token passed to the RIL command request
 *
 * @return
 *     none
 *
 * @note
 *     RIL_REQUEST_OPERATOR
 */
void requestOperatorUMTS(void *data, size_t datalen, RIL_Token t)
{
    char *response[3];
    ULONG nRet;
    int i;
    nasPLMNNameReq plmnReq;
    nasPLMNNameResp plmnResp;

    memset(response, 0, sizeof(response));
    memset(&plmnResp, 0, sizeof(plmnResp));

    /* Invoke the SLQS API to know the SPN status on the SIM */
    plmnReq.mcc = s_servingNetworkParaOld.MCC;
    plmnReq.mnc = s_servingNetworkParaOld.MNC;
    nRet = SLQSGetPLMNName( &plmnReq, &plmnResp );

    /* Handle the error scenario */
    if (nRet != eQCWWAN_ERR_NONE) {
        goto execute_cops_command;
    }

    /**
     * If the SPN is not present then invoke the AT+COPS implementation
     * If the SPN is present and encoded as ascii use AT+COPS implementation
     */
    if ((plmnResp.spnLength == 0) || 
        ((plmnResp.spnLength != 0) && 
         (plmnResp.spnEncoding == ENCODING_SCHEME_GSM))) {
        goto execute_cops_command;
    }

    if (strlen(&s_servingNetworkParaOld.name[0]) > 0) {
        /* SWI_TBD use the same string for both long and short name */
        asprintf(&response[0], "%s", &s_servingNetworkParaOld.name[0]);
        asprintf(&response[1], "%s", &s_servingNetworkParaOld.name[0]);
    }
    if ((s_servingNetworkParaOld.MCC != 0) && (s_servingNetworkParaOld.MNC != 0)) {
        asprintf(&response[2], "%d%d", s_servingNetworkParaOld.MCC, s_servingNetworkParaOld.MNC);
    }

    swiril_cache_RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(response),
                                       RIL_REQUEST_OPERATOR);
    for(i = 0; i < 3; i++) {
        if (response[i] != NULL) {
            free(response[i]);
        }
    }
    return;

execute_cops_command:
    requestOperatorAT (data, datalen, t);
}

void requestCurrentCallsQMI(void *data, size_t datalen, RIL_Token t)
{
    /* SWI_TBD 
       Even though this RIL command is for voice calls, a response is still
       required, because the upper layers will keep calling it until a 
       response is received.  Return a hard-coded response here, rather
       than querying the modem.  Fix this when voice calls are supported.
     */
    LOGW("RIL_REQUEST_GET_CURRENT_CALLS: returning no current calls");
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
}

static void SLQSSetServingSystemCB(struct ServingSystemInfo *pSessionState)
{
    int i;
        
    LOGD("%s Registration State: %d\n", __func__, pSessionState->registrationState );
    LOGD("CS Attach State: %d\n", pSessionState->csAttachState );
    LOGD("PS Attach State: %d\n", pSessionState->psAttachState );
    LOGD("Selected Network: %d\n", pSessionState->selectedNetwork );
    for (i = 0; i < pSessionState->radioInterfaceNo; i++) {
        LOGD("radioInterfaceList[%d]: %d\n", i, pSessionState->radioInterfaceList[i] );
    }

    /* poll once anyway for registration change, 
     * sounds like no way to call QMI API in callback 
     */
    enqueueRILEvent(RIL_EVENT_QUEUE_NORMAL, pollRegistration,
                    NULL, NULL);
}

void registerSLQSSetServingSystemCallbackQMI(void)
{
    ULONG nRet;

    nRet = SLQSSetServingSystemCallback(&SLQSSetServingSystemCB );

    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGE("registerSLQSSetServingSystemCallbackQMI: failed to enable callback %lu\n", nRet );
    }
}

/**
* RIL_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE
*
* Requests to get the roaming preference status.
*/
void requestGetCDMARoamingPreferenceCDMA(void *data, size_t datalen,
                                       RIL_Token t)
{
    int   RoamingOutPut;
    ULONG nRet = 0;
    
    STRUCT_GetCDMANetworkParameters sParam;
    memset(&sParam, 0, sizeof(STRUCT_GetCDMANetworkParameters));

    /* call Qualcomm NAS interface */
    nRet = GetCDMANetworkParameters(
        &sParam.SCI, 
        &sParam.SCM, 
        &sParam.RegHomeSID, 
        &sParam.RegForeignSID, 
        &sParam.RegForeignNID, 
        &sParam.ForceRev0, 
        &sParam.CustomSCP, 
        &sParam.Protocol, 
        &sParam.Broadcast, 
        &sParam.Application, 
        &sParam.Roaming );

    if (nRet == eQCWWAN_ERR_NONE) {
        switch(sParam.Roaming)
        {
            case QMI_AUTOMATIC:
                RoamingOutPut = RIL_ANY_NETWORK; 
                break;
            case QMI_HOME_ONLY:
                RoamingOutPut = RIL_HOME_ONLY;
                break;
            case QMI_HOME_AND_AFFILIATED_ROAMING:
                RoamingOutPut = RIL_AFFILIATED_ROAMING;
                break;
            default:
                RoamingOutPut = RIL_ANY_NETWORK; 
                break;
        }
        
        LOGD("requestGetCDMARoamingPreferenceQMI sParam.Roaming : %lu, RoamingOutput: %d \n",
            sParam.Roaming, RoamingOutPut);
        RIL_onRequestComplete(t, RIL_E_SUCCESS,  &RoamingOutPut, sizeof(RoamingOutPut));
    }
    else {
        LOGE("GetCDMANetworkParameters error  %lu", nRet);
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    }

}

/**
* RIL_REQUEST_CDMA_SET_ROAMING_PREFERENCE
*
* Requests to set the roaming preference type
*              0 - Home Only
*              1 - Roaming on Affiliated networks
*              2 - Automatic
*/
void requestSetCDMARoamingPreferenceCDMA(void *data, size_t datalen,
                                       RIL_Token t)
{
    int   RoamingInput;
    ULONG RoamingSetting;
    ULONG nRet = 0;

    RoamingInput = ((int *)data)[0];
    switch(RoamingInput)
    {
        case RIL_HOME_ONLY:
            RoamingSetting = QMI_HOME_ONLY; 
            break;
        case RIL_AFFILIATED_ROAMING:
            RoamingSetting = QMI_HOME_AND_AFFILIATED_ROAMING;
            break;
        case RIL_ANY_NETWORK:
            RoamingSetting = QMI_AUTOMATIC; 
            break;
        default:
            RoamingSetting = QMI_AUTOMATIC; 
            break;
    }
    LOGD("requestSetCDMARoamingPreferenceQMI RoamingInput = %d datalen = %d, RoamingSetting = %lu ", 
        RoamingInput, datalen, RoamingSetting);
    
    /* call Qualcomm NAS interface, The modified setting will only be used after the modem has been reset*/
    nRet = SetCDMANetworkParameters(NULL, NULL, NULL, NULL, NULL, NULL, &RoamingSetting );

    if (nRet == eQCWWAN_ERR_NONE) {
        LOGD("requestSetCDMARoamingPreferenceQMI succeed");
        RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);

        /* The modem must be power-cycled after setting the roaming preference type */
        changePowerModeQMI(QMI_POWER_OFF);        
    }
    else {
        LOGE("GetCDMANetworkParameters error  %lu", nRet);
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    }
}

/**
 * Poll GetServingNetwork and return a success, or if the loop counter reaches
 * REPOLL_REGSTATUS_SELECTED, return generic failure.
 */
static void pollRegStatus(void *params)
{
    ULONG nRet = 0;
    BYTE  count;
    struct getServingNetworkPara servingNetworkPara;

    STRUCT_RegPollParams *poll_params = NULL;
    RIL_Token t;

    assert(params != NULL);

    poll_params = (STRUCT_RegPollParams *) params;
    t = poll_params->t;

    if (poll_params->loopcount >= REPOLL_REGSTATUS_SELECTED) {
        goto error;
    }
    
    memset(&servingNetworkPara, 0, sizeof(struct getServingNetworkPara));
    servingNetworkPara.radioIfacesSize = RAT_MAX;
    
    nRet = GetServingNetwork(&servingNetworkPara.registrationState,
                             &servingNetworkPara.CSDomain,
                             &servingNetworkPara.PSDomain,
                             &servingNetworkPara.RAN,
                             &servingNetworkPara.radioIfacesSize,
                             &servingNetworkPara.radioIfaces[0],
                             &servingNetworkPara.roamingInd,
                             &servingNetworkPara.MCC,
                             &servingNetworkPara.MNC,
                             sizeof(servingNetworkPara.name),
                             &servingNetworkPara.name[0]);

    if (nRet == eQCWWAN_ERR_NONE) {
        /* print out when updated */ 
        LOGD("%s registrationState  : %lu\n", __func__, servingNetworkPara.registrationState);
        LOGD("CSDomain  : %lu\n", servingNetworkPara.CSDomain);
        LOGD("PSDomain : %lu\n", servingNetworkPara.PSDomain);
        LOGD("RAN  : %lu\n", servingNetworkPara.RAN);
        LOGD("radioIfacesSize  : %d\n", servingNetworkPara.radioIfacesSize);
        for(count = 0; count < servingNetworkPara.radioIfacesSize; count++)
            LOGD("radioIfaces[%d]  : %d\n", count, servingNetworkPara.radioIfaces[count]);
        LOGD("roamingInd  : %lu\n", servingNetworkPara.roamingInd);
        LOGD("MCC  : %d\n", servingNetworkPara.MCC);
        LOGD("MNC  : %d\n", servingNetworkPara.MNC);
        LOGD("Name  : %s\n", servingNetworkPara.name);
    }
    else {
        LOGE("GetServingNetwork error: %lu", nRet);
        goto error;
    }
    
    if(servingNetworkPara.registrationState == QMI_REG_STATE_REGISTERED){
        /* set g_regStatus when registration succeed */
        g_regStatus.regType = poll_params->nRegType;
        g_regStatus.mcc = servingNetworkPara.MCC;
        g_regStatus.mnc = servingNetworkPara.MNC;  

        /* registration details may have changed; clear network cache data */
        swiril_cache_clearnetworkdata();

        /* now that we are registered, send SIM_READY if it is pending */
        sendPendingSIMREADY();

        RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
        free(poll_params);
        return;
    } else if ( (servingNetworkPara.registrationState == QMI_REG_STATE_REGISTRATION_DENIED)
                    && (poll_params->nRegType == RIL_REG_TYPE_MANUAL) ) {

        /* SWI_TBD
         * We should only get here if we tried to do manual registration
         * with a de-activated SIM and we got CME ERROR 30. Add explicit
         * check for CME ERROR 30 later, if needed.
         */

        /* registration details may have changed; clear network cache data */
        swiril_cache_clearnetworkdata();
        
        /* now that we have failed registration, send SIM_READY if it is pending */
        sendPendingSIMREADY();
        
        RIL_onRequestComplete(t, RIL_E_ILLEGAL_SIM_OR_ME, NULL, 0);
        free(poll_params);
        return;
    } else {
        poll_params->loopcount++;
        enqueueRILEvent(RIL_EVENT_QUEUE_NORMAL, pollRegStatus,
                        poll_params, &TIMEVAL_REGISTER_STATUS_POLL);
        return;
    }

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    free(poll_params);
}

/**
 * Returns true iff persist.sierra.block_init_reg property is set
 * and its value is greater than 0.
 *
 * SWI_TBD: Implementation of this mechanism needs to be revisited to see
 *          if it can or needs to be cleaned up.
 */
bool isBlockInitialRegistration(void)
{
    CHAR propValue[PROPERTY_VALUE_MAX];
    int intpropvalue=0;

    if (property_get("persist.sierra.block_init_reg", propValue, NULL) > 0) {
        intpropvalue = atoi(propValue);
        LOGI("%s: block_init_reg=%i", __func__, intpropvalue);
    }

    return (intpropvalue > 0);
}

/**
 * RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC
 *
 * Specify that the network should be selected automatically.
 */
void requestSetNetworkSelectionAutomaticUMTS(void *data, size_t datalen, RIL_Token t)
{
    ULONG nRegType = RIL_REG_TYPE_AUTOMATIC; /*Automatic registration*/
    ULONG nRet = 0;
    RIL_Errno errno = RIL_E_GENERIC_FAILURE;
    STRUCT_RegPollParams *poll_params = NULL;


    if ( !s_initialScanPerformed && isBlockInitialRegistration() ) {
        /* Do nothing but pretend the command was successful to keep application happy */
        RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
        return;
    } else {
        /* Not blocking initial registration, so pretend an initial scan has been performed */
        s_initialScanPerformed = true;
    }

#if 0
    /* SWI_TBD: Always allow a registration attempt, even if already registered.
     *          The worst that will happen is we get eQCWWAN_ERR_QMI_NO_EFFECT.
     */

    /* if already in automatic selection, directly poll the registration status.*/
    LOGD("%s: g_regStatus.regType %lu, mcc =%d, mnc =%d", __func__, g_regStatus.regType, g_regStatus.mcc, g_regStatus.mnc);
    if((g_regStatus.regType == RIL_REG_TYPE_AUTOMATIC)&&((g_regStatus.mnc != 0)||(g_regStatus.mcc != 0)))
        goto pollregstatus;
#endif
        
    /* call Sierra QMI interface*/
    /* if registration type is automatic, other parameter could be ignored */
    nRet = InitiateNetworkRegistration(nRegType, 0, 0, 0);
    if(nRet == eQCWWAN_ERR_QMI_NO_EFFECT){
        LOGE("InitiateNetworkRegistration for %lu result  %lu", nRegType, nRet);
        goto pollregstatus;
    }
    else if(nRet == eQCWWAN_ERR_QMI_SIM_NOT_INITIALIZED){
        LOGE("InitiateNetworkRegistration for %lu error  %lu", nRegType, nRet);
        errno =  RIL_E_ILLEGAL_SIM_OR_ME;
        goto error;
    }
    else if (nRet != eQCWWAN_ERR_NONE){
        LOGE("InitiateNetworkRegistration for %lu error  %lu", nRegType, nRet);
        errno = RIL_E_GENERIC_FAILURE;
        goto error;
    }

    LOGD("InitiateNetworkRegistration for %lu succeed", nRegType);
    
pollregstatus:
    poll_params = malloc(sizeof(STRUCT_RegPollParams));

    poll_params->loopcount = 0;
    poll_params->t = t;
    poll_params->nRegType = nRegType;

    enqueueRILEvent(RIL_EVENT_QUEUE_NORMAL, pollRegStatus,
                    poll_params, &TIMEVAL_REGISTER_STATUS_POLL);

    return;

error:
    RIL_onRequestComplete(t, errno, NULL, 0);
}

/* SWI_TBD:
 *   Both the AT and QMI versions of this function are here.
 *   This should be cleaned up.
 */
/**
 * RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL
 *
 * Manually select a specified network.
 *
 * The radio baseband/RIL implementation is expected to fall back to 
 * automatic selection mode if the manually selected network should go
 * out of range in the future.
 */
void requestSetNetworkSelectionManual(void *data, size_t datalen,
                                      RIL_Token t)
{
    /* 
     * AT+COPS=[<mode>[,<format>[,<oper>[,<AcT>]]]]
     *    <mode>   = 4 = Manual (<oper> field shall be present and AcT optionally) with fallback to automatic if manual fails.
     *    <format> = 2 = Numeric <oper>, the number has structure:
     *                   (country code digit 3)(country code digit 2)(country code digit 1)
     *                   (network code digit 2)(network code digit 1) 
     */

    int err = 0;
    char *cmd = NULL;
    ATResponse *atresponse = NULL;
    const char *mccMnc = (const char *) data;
    STRUCT_RegPollParams *poll_params = NULL;

    /* Check inparameter. */
    if (mccMnc == NULL) {
        goto error;
    }

    if ( !s_initialScanPerformed && isBlockInitialRegistration() ) {
        /* Do nothing but pretend the command was successful to keep application happy */
        RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
        return;
    } else {
        /* Not blocking initial registration, so pretend an initial scan has been performed */
        s_initialScanPerformed = true;
    }

    /* Build and send command. */
    asprintf(&cmd, "AT+COPS=1,2,\"%s\"", mccMnc);
    err = at_send_command(cmd, &atresponse);
    if (err < 0)
        goto error;

    switch (at_get_cme_error(atresponse)) {
        case CME_SUCCESS:
        case CME_NO_NETWORK_SERVICE:
            break;

        default:
            goto error;
    }

    /* Make sure registration succeeds before returning RIL response.
       The RIL response will be returned in pollOperatorSelected().
     */
    poll_params = malloc(sizeof(STRUCT_RegPollParams));

    poll_params->loopcount = 0;
    poll_params->t = t;
    poll_params->nRegType = RIL_REG_TYPE_MANUAL;

    enqueueRILEvent(RIL_EVENT_QUEUE_NORMAL, pollRegStatus,
                    poll_params, &TIMEVAL_REGISTER_STATUS_POLL);

finally:
    at_response_free(atresponse);

    if (cmd != NULL)
        free(cmd);

    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    goto finally;
}


/**
 * RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL
 *
 * Manually select a specified network.
 *
 * The radio baseband/RIL implementation is expected to fall back to 
 * automatic selection mode if the manually selected network should go
 * out of range in the future.
 */
void requestSetNetworkSelectionManualUMTS(void *data, size_t datalen, RIL_Token t)
{
    ULONG nRet = 0;
    const char *mccMnc = (const char *) data;
    /*
     *    <format> = 2 = Numeric <oper>, the number has structure:
     *                   (country code digit 3)(country code digit 2)(country code digit 1)
     *                   (network code digit 2)(network code digit 1) 
    */
    RIL_Errno errno = RIL_E_GENERIC_FAILURE;
    STRUCT_RegPollParams *poll_params = NULL;

    STRUCT_InitiateNetworkRegistration sParam;
    /* clear structure*/
    memset(&sParam, 0, sizeof(STRUCT_InitiateNetworkRegistration));

    sParam.regType = RIL_REG_TYPE_MANUAL;
    sParam.mcc = (mccMnc[0] - '0' )* 100 + (mccMnc[1] - '0' )* 10 + mccMnc[2] - '0' ;
    if(strlen(mccMnc) == 6)
        sParam.mnc = (mccMnc[3] - '0' )* 100 + (mccMnc[4] - '0' )* 10 + mccMnc[5] - '0' ; 
    else if(strlen(mccMnc) == 5)
        sParam.mnc = (mccMnc[3] - '0' )* 10 + (mccMnc[4] - '0' );

    /* if already in manual selection, mcc and mnc are same, directly poll the status.*/
    LOGD("%s: g_TechnoledgeType %lu, g_regStatus.regType %lu, mcc =%d, mnc =%d", __func__, g_TechnoledgeType, g_regStatus.regType,          g_regStatus.mcc, g_regStatus.mnc);
    if((g_regStatus.regType == RIL_REG_TYPE_MANUAL)&&(g_regStatus.mcc == sParam.mcc)&&(g_regStatus.mnc == sParam.mnc))
        goto pollregstatus;

    /* set rat accroding to requestSetPreferredNetworkTypeQMI */ 
    switch(g_TechnoledgeType) 
    {
        case RIL_RAT_GSM_ONLY:
            sParam.rat = RAT_GSM;
            break;
        case RIL_RAT_WCDMA_PREPERRED:
        case RIL_RAT_WCDMA_ONLY:
        case RIL_RAT_GSM_WCDMA_AUTO:
        case RIL_RAT_GSM_WCDMA_CDMA_EVDO_AUTO: /*default using UMTS*/
            sParam.rat = RAT_UMTS;
            break;
        case RIL_RAT_LTE_CDMA_EVDO:
        case RIL_RAT_LTE_GSM_WCDMA:
        case RIL_RAT_LTE_CMDA_EVDO_GSM_WCDMA:
        case RIL_RAT_LTE_ONLY:
            sParam.rat = RAT_LTE;
            break;
        default:
            LOGE("%s:: g_TechnoledgeType %lu not supported", __func__, g_TechnoledgeType);
            errno = RIL_E_MODE_NOT_SUPPORTED;
            goto error;
    }
    
    /* call Sierra QMI interface */
    LOGD("%s:: Registration type: %lu, mcc: %hu, mnc:%hu, rat:%lu ", __func__,
                                                sParam.regType, 
                                                sParam.mcc, 
                                                sParam.mnc, 
                                                sParam.rat );
    nRet = InitiateNetworkRegistration(
                                        sParam.regType, 
                                        sParam.mcc, 
                                        sParam.mnc, 
                                        sParam.rat );
    if(nRet == eQCWWAN_ERR_QMI_SIM_NOT_INITIALIZED){
        LOGE("%s:: InitiateNetworkRegistration for %lu error  %lu", __func__, sParam.regType, nRet);
        RIL_onRequestComplete(t, RIL_E_ILLEGAL_SIM_OR_ME, NULL, 0);
    }
    else if (nRet != eQCWWAN_ERR_NONE){
        LOGE("%s:: InitiateNetworkRegistration for %lu error  %lu", __func__, sParam.regType, nRet);
        errno = RIL_E_GENERIC_FAILURE;
        goto error;
    }

pollregstatus:    
    poll_params = malloc(sizeof(STRUCT_RegPollParams));

    poll_params->loopcount = 0;
    poll_params->t = t;
    poll_params->nRegType = sParam.regType;

    enqueueRILEvent(RIL_EVENT_QUEUE_NORMAL, pollRegStatus,
                    poll_params, &TIMEVAL_REGISTER_STATUS_POLL);
    return;

error:
    RIL_onRequestComplete(t, errno, NULL, 0);
    
}

/* SWI_TBD:
 *   Both the AT and QMI versions of this function are here.
 *   This should be cleaned up.
 */
/**
 * RIL_REQUEST_QUERY_AVAILABLE_NETWORKS
 *
 * Scans for available networks.
*/
void requestQueryAvailableNetworks(void *data, size_t datalen, RIL_Token t)
{
    /* 
     * AT+COPS=?
     *   +COPS: [list of supported (<stat>,long alphanumeric <oper>
     *           ,short alphanumeric <oper>,numeric <oper>[,<AcT>])s]
     *          [,,(list of supported <mode>s),(list of supported <format>s)]
     *
     *   <stat>
     *     0 = unknown
     *     1 = available
     *     2 = current
     *     3 = forbidden 
     */

    int err = 0;
    ATResponse *atresponse = NULL;
    const char *statusTable[] =
        { "unknown", "available", "current", "forbidden" };
    char **responseArray = NULL;
    char *p;
    int n = 0;
    int i = 0;

    /* We are about to start a scan; easier to always set flag to true rather than
     * determine its current value.
     */
    s_initialScanPerformed = true;

    err = at_send_command_multiline("AT+COPS=?", "+COPS:", &atresponse);
    if (err < 0 || 
        atresponse->success == 0 || 
        atresponse->p_intermediates == NULL)
        goto error;

    p = atresponse->p_intermediates->line;

    /* Determine the number of operators by counting the number of opening
       brackets. However, stop after getting two consecutive commas, because
       this signals the end of the operator list.  
     */
    while (*p != '\0') {
        if (*p == '(') {
            n++;
        } else if ( (*p == ',') && (*(p+1) == ',') ) {
            /* this test will work even if the last char is comma, because the
               next char will always be '\0' in that case.
             */
            break;
        }
        p++;
    }

    /* Allocate array of strings, blocks of 4 strings. */
    responseArray = alloca(n * 4 * sizeof(char *));

    p = atresponse->p_intermediates->line;

    /* Loop and collect response information into the response array. */
    for (i = 0; i < n; i++) {
        int status = 0;
        char *line = NULL;
        char *s = NULL;
        char *longAlphaNumeric = NULL;
        char *shortAlphaNumeric = NULL;
        char *numeric = NULL;
        char *remaining = NULL;

        s = line = getFirstElementValue(p, "(", ")", &remaining);
        p = remaining;

        if (line == NULL) {
            LOGE("Null pointer while parsing COPS response. This should not happen.");
            break;
        }
        /* <stat> */
        err = at_tok_nextint(&line, &status);
        if (err < 0)
            goto error;

        /* long alphanumeric <oper> */
        err = at_tok_nextstr(&line, &longAlphaNumeric);
        if (err < 0)
            goto error;

        /* short alphanumeric <oper> */            
        err = at_tok_nextstr(&line, &shortAlphaNumeric);
        if (err < 0)
            goto error;

        /* numeric <oper> */
        err = at_tok_nextstr(&line, &numeric);
        if (err < 0)
            goto error;

        responseArray[i * 4 + 0] = alloca(strlen(longAlphaNumeric) + 1);
        strcpy(responseArray[i * 4 + 0], longAlphaNumeric);

        responseArray[i * 4 + 1] = alloca(strlen(shortAlphaNumeric) + 1);
        strcpy(responseArray[i * 4 + 1], shortAlphaNumeric);

        responseArray[i * 4 + 2] = alloca(strlen(numeric) + 1);
        strcpy(responseArray[i * 4 + 2], numeric);

        free(s);

        /* 
         * Check if modem returned an empty string, and fill it with MNC/MMC 
         * if that's the case.
         */
        if (responseArray[i * 4 + 0] && strlen(responseArray[i * 4 + 0]) == 0) {
            responseArray[i * 4 + 0] = alloca(strlen(responseArray[i * 4 + 2])
                                              + 1);
            strcpy(responseArray[i * 4 + 0], responseArray[i * 4 + 2]);
        }

        if (responseArray[i * 4 + 1] && strlen(responseArray[i * 4 + 1]) == 0) {
            responseArray[i * 4 + 1] = alloca(strlen(responseArray[i * 4 + 2])
                                              + 1);
            strcpy(responseArray[i * 4 + 1], responseArray[i * 4 + 2]);
        }

        responseArray[i * 4 + 3] = alloca(strlen(statusTable[status]) + 1);
        sprintf(responseArray[i * 4 + 3], "%s", statusTable[status]);
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, responseArray,
                          i * 4 * sizeof(char *));

finally:
    at_response_free(atresponse);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    goto finally;
}

/**
 * RIL_REQUEST_QUERY_AVAILABLE_NETWORKS
 *
 * Scans for available networks.
 */
void requestQueryAvailableNetworksUMTS(void *data, size_t datalen, RIL_Token t)
{
    ULONG nRet = 0;
    int   i = 0;
    const char *statusTable[] =
        { "unknown", "current",  "available", "forbidden" };
    BYTE  InstanceSize = INSTANCESIZE;
    char  **responseArray = NULL;
    STRUCT_PerformNetworkScan sParam[INSTANCESIZE];

    if (isDataSessionActive()) {
        /* if there is an active data session, and will return a GENERIC_FAILURE error. */
        LOGE("%s:: Data Session is active, return error.\n", __func__);
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
        return; 
    }

    /* clear structure */
    memset(&sParam[0], 0, sizeof(sParam));

    /* call Sierra QMI interface */
    nRet = PerformNetworkScan(
                           &InstanceSize,
                           (BYTE*) &sParam[0]);

    if (nRet == eQCWWAN_ERR_NONE) 
    {
        LOGD("%s:: PerformNetworkScan succeed  InstanceSize %d", __func__, (int)InstanceSize);
        /* Allocate array of strings, blocks of 4 strings. */
        responseArray = alloca(InstanceSize * 4 * sizeof(char *));

        /* Loop and collect response information into the response array. */
        for (i = 0; i < InstanceSize; i++) 
        {
            char numeric[7] = "";
            LOGD("%s:: result for operator: %s, mcc: %hu, mnc:%hu, inUse:%lu ", __func__, 
                                                            sParam[i].mDescription, 
                                                            sParam[i].mMcc, 
                                                            sParam[i].mMnc, 
                                                            sParam[i].mInUse );
            responseArray[i * 4 + 0] = alloca(strlen(sParam[i].mDescription) + 1);
            strcpy(responseArray[i * 4 + 0], sParam[i].mDescription);

            responseArray[i * 4 + 1] = alloca(strlen(sParam[i].mDescription) + 1);
            strcpy(responseArray[i * 4 + 1], sParam[i].mDescription);

            int mccmnc = (int)sParam[i].mMcc * 1000 + sParam[i].mMnc;
            snprintf(numeric, sizeof(numeric), "%d", mccmnc);
            LOGD("%s:: location string: %s, mccmnc = %d", __func__, numeric, mccmnc);
            responseArray[i * 4 + 2] = alloca(strlen(numeric)+1);
            strcpy(responseArray[i * 4 + 2], numeric);

            /* 
             * Check if modem returned an empty string, and fill it with MNC/MMC 
             * if that's the case.
             */
            if (responseArray[i * 4 + 0] && strlen(responseArray[i * 4 + 0]) == 0) {
                responseArray[i * 4 + 0] = alloca(strlen(responseArray[i * 4 + 2])
                                                  + 1);
                strcpy(responseArray[i * 4 + 0], responseArray[i * 4 + 2]);
            }

            if (responseArray[i * 4 + 1] && strlen(responseArray[i * 4 + 1]) == 0) {
                responseArray[i * 4 + 1] = alloca(strlen(responseArray[i * 4 + 2])
                                                  + 1);
                strcpy(responseArray[i * 4 + 1], responseArray[i * 4 + 2]);
            }

            responseArray[i * 4 + 3] = alloca(strlen(statusTable[sParam[i].mInUse]) + 1);
            sprintf(responseArray[i * 4 + 3], "%s", statusTable[sParam[i].mInUse]);
        }

        RIL_onRequestComplete(t, RIL_E_SUCCESS, responseArray,
                              i * 4 * sizeof(char *));
        
    }
    else {
        LOGE("%s:: PerformNetworkScan failed error %lu", __func__, nRet);
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    }
}

/**
 * RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE
 *
 * Requests to set the preferred network type for searching and registering
 * (CS/PS domain, RAT, and operation mode).
 */
void requestSetPreferredNetworkTypeQMI(void *data, size_t datalen, RIL_Token t)
{
    int   rat;
    BYTE  bit1 = 1;
    ULONG nRet = 0;
    RIL_Errno errno = RIL_E_GENERIC_FAILURE;

    rat = ((int *) data)[0];

    STRUCT_SetNetworkPreference sParam;
    /* clear structure */
    memset(&sParam, 0, sizeof(STRUCT_SetNetworkPreference));

    switch (rat) {
    case RIL_RAT_GSM_ONLY:
        sParam.TechnologyPreference = QMI_RAT_GSM;
        break;
    case RIL_RAT_WCDMA_ONLY:
        sParam.TechnologyPreference = QMI_RAT_WCDMA;
        break;
    case RIL_RAT_WCDMA_PREPERRED: /*SWI_TBD not sure about WCDMA preferred */
    case RIL_RAT_GSM_WCDMA_AUTO:
        sParam.TechnologyPreference = QMI_RAT_GSM_WCDMA;
        break;
    case RIL_RAT_CDMA_ONLY:
        sParam.TechnologyPreference = QMI_RAT_CDMA;
        break;
    case RIL_RAT_EVDO_ONLY:
        sParam.TechnologyPreference = QMI_RAT_EVDO;
        break;
    case RIL_RAT_CDMA_EVDO_AUTO: 
        sParam.TechnologyPreference = QMI_RAT_CDMA_EVDO;
        break;
    case RIL_RAT_GSM_WCDMA_CDMA_EVDO_AUTO:
        sParam.TechnologyPreference = QMI_RAT_AUTO;  
        break;                                                         
    case RIL_RAT_LTE_CDMA_EVDO:
    case RIL_RAT_LTE_CMDA_EVDO_GSM_WCDMA:
        /* SWI_TBD hard-coded to QMI_RAT_AUTO for now. */
        sParam.TechnologyPreference = QMI_RAT_AUTO; 
        break;                                                         
    case RIL_RAT_LTE_GSM_WCDMA:
        sParam.TechnologyPreference = QMI_RAT_LTE_GSM_WCDMA;  
        break;                                                         
    case RIL_RAT_LTE_ONLY:
        sParam.TechnologyPreference = QMI_RAT_LTE_ONLY;  
        break;                                                         
    default:
        errno = RIL_E_MODE_NOT_SUPPORTED;
        goto error;
    }

    sParam.Duration = QMI_DURATION_PERSISTENT; /* hard code to set the duration to permanently */

    LOGD("%s:: Rat: %d, Technology Preference: 0x%x, Duration:%lu ", 
         __func__, rat,
        (int)sParam.TechnologyPreference, 
        sParam.Duration );
 
    /* call Sierra QMI interface */
    nRet = SetNetworkPreference(sParam.TechnologyPreference, 
                                        sParam.Duration);
    if ((nRet == eQCWWAN_ERR_NONE) || (nRet == eQCWWAN_ERR_QMI_NO_EFFECT)) {
        LOGD("%s:: SetNetworkPreference succeed %lu", __func__ , nRet);
        g_TechnoledgeType = rat;
        RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    }
    else if(nRet == eQCWWAN_ERR_QMI_OP_DEVICE_UNSUPPORTED){
        LOGE("%s:: SetNetworkPreference failed, error  %lu", __func__, nRet);
        RIL_onRequestComplete(t, RIL_E_MODE_NOT_SUPPORTED, NULL, 0);
    }
    else{
        LOGE("%s:: SetNetworkPreference failed, error  %lu", __func__, nRet);
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    }
    return;

error:
    RIL_onRequestComplete(t, errno, NULL, 0);
}

/**
 * RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE
 *
 * Query the preferred network type (CS/PS domain, RAT, and operation mode)
 * for searching and registering.
 */
void requestGetPreferredNetworkTypeQMI(void *data, size_t datalen, RIL_Token t)
{
    int   response = 0;
    ULONG ul1 = 1;
    ULONG nRet = 0;
    ULONG TechnologyPrefer = -1;
    SWI_FW_INFO_TYPE tech = SWI_FW_INFO_TYPE_UNKNOWN;
    
    STRUCT_GetNetworkPreference sParam;
    /* clear structure */
    memset(&sParam, 0, sizeof(STRUCT_GetNetworkPreference));

    /* call Sierra QMI interface */
    nRet = GetNetworkPreference(&sParam.TechnologyPreference, 
                                &sParam.Duration, 
                                &sParam.PersistentTechnologyPref);

    if (nRet != eQCWWAN_ERR_NONE) {
        LOGE("%s:: GetNetworkPreference failed, error: %lu", __func__, nRet);
        goto error;
    }
    
    LOGD("%s:: Tech Pref: 0x%x, Duration: %lu, Persistent Pref 0x%x", __func__,
        (unsigned int)sParam.TechnologyPreference, sParam.Duration, 
        (unsigned int)sParam.PersistentTechnologyPref);

    if(sParam.Duration == QMI_DURATION_POWER_CYCLE) 
        TechnologyPrefer = sParam.TechnologyPreference;
    else if(sParam.Duration == QMI_DURATION_PERSISTENT)
        TechnologyPrefer = sParam.TechnologyPreference; 
    /* SWI_TBD nomatter what duration is, the PersistentTechnologyPref is always 0*/
    else{
        LOGE("%s:: GetNetworkPreference unsported duration type %lu", __func__, sParam.Duration);
        goto error;
    }
    
    switch (TechnologyPrefer) {
        case QMI_RAT_AUTO:
            /* SWI_TBD if MC7750, hard-coded to RIL_RAT_LTE_CDMA_EVDO for now. It is always targeted at Verizon*/
            if(getFirmwareInfoTechQMI(&tech) && (tech == SWI_FW_INFO_TYPE_SWI_DUAL_MODE) ) 
                response = RIL_RAT_LTE_CDMA_EVDO ;
            else {    
                if(getFirmwareInfoTechQMI(&tech) && (tech == SWI_FW_INFO_TYPE_GOBI_UMTS || tech == SWI_FW_INFO_TYPE_GOBI_CDMA) ) 
                    /* MC8355 */
                    response = RIL_RAT_GSM_WCDMA_CDMA_EVDO_AUTO;
                else
                    /* MC7700*/
                    response = RIL_RAT_LTE_GSM_WCDMA;
            }
            break;    
        case QMI_RAT_GSM: 
            response = RIL_RAT_GSM_ONLY;
            break;
        case QMI_RAT_WCDMA:
            response = RIL_RAT_WCDMA_ONLY;
            break;
        case QMI_RAT_GSM_WCDMA: 
            response = RIL_RAT_GSM_WCDMA_AUTO;
            break;
        case QMI_RAT_CDMA_EVDO: 
            response = RIL_RAT_CDMA_EVDO_AUTO;
            break;
        case QMI_RAT_CDMA: 
            response = RIL_RAT_CDMA_ONLY;
            break;
        case QMI_RAT_EVDO: 
            response = RIL_RAT_EVDO_ONLY;
            break;
        case QMI_RAT_LTE_GSM_WCDMA:
            response = RIL_RAT_LTE_GSM_WCDMA;
            break;
        case QMI_RAT_LTE_ONLY:
            response = RIL_RAT_LTE_ONLY;
            break;
        default:
            LOGE("%s:: got a rat type %lu", __func__, sParam.TechnologyPreference);
            goto error;
            break;
    }
    LOGD("%s:: requestGetPreferredNetworkTypeQMI got a rat type %d", __func__, response);
    RIL_onRequestComplete(t, RIL_E_SUCCESS, &response, sizeof(int));
    return; 
    
error:
    LOGE("%s:: getpreferrednwtype error", __func__);
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 * RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE
 *
 * Query current network selectin mode.
 */
void requestQueryNetworkSelectionModeUMTS(void *data, size_t datalen, RIL_Token t)
{
    int   mode = -1;

    switch (g_regStatus.regType) {
    case RIL_REG_TYPE_AUTOMATIC:
        mode = QMI_REG_TYPE_AUTOMATIC; 
        break;    
    case RIL_REG_TYPE_MANUAL:
        mode = QMI_REG_TYPE_MANUAL;   
        break;
    default:
        goto error;
        break;
    }

    swiril_cache_RIL_onRequestComplete(t, RIL_E_SUCCESS, &mode, sizeof(int),
                                       RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE);
    LOGD("requestQueryNetworkSelectionModeQMI succeed, mode = %d", mode);
    return; 
    
error:
    LOGE("requestQueryNetworkSelectionModeQMI error when g_regStatus.regType = %lu", g_regStatus.regType);
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 * RIL_REQUEST_SET_LOCATION_UPDATES
 *
 * Enables/disables network state change notifications due to changes in
 * LAC and/or CID.  
 *
 * Note:  The RIL implementation should default to "updates enabled"
 * when the screen is on and "updates disabled" when the screen is off.
 *
 * See also: RIL_REQUEST_SCREEN_STATE, RIL_UNSOL_RESPONSE_NETWORK_STATE_CHANGED.
 */
void requestSetLocationUpdatesQMI(void *data, size_t datalen, RIL_Token t)
{
    ULONG resultCode;
    ULONG enable = 0;

    enable = ((int *) data)[0];
    if(enable != 0 && enable != 1) {
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
        return;
    }

    /*
     * SWI_TBD: Currently we cannot validate the LAC and CID in QMI, to add
     * valid code once the interfaces are available
     */
     RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
}


/**
 *
 * Turn on/off QMI notifications controlled by RIL ScreenState command.
 *
 * @param on_off
 *     if TRUE, screen is on and notifications should be enabled.
 *
 * @return
 *     zero     : success
 *     non-zero : failure
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
static int setQMINotificationState(int on_off)
{
    ULONG resultCode;
    CHAR propValue[PROPERTY_VALUE_MAX];
    int intpropvalue=0;

    if (on_off) {
        /* 
         * Screen is on - enable all unsolicited notifications
         * Subscribe to network registration events. Currently we cannot
         * validate the LAC and CID in QMI, hence only updating the registration
         * state callback
         */
        /* Subscribe to NAS broadcast indications */
        resultCode = SLQSNasIndicationRegister (1, 1, 1);
        if (resultCode != eQCWWAN_ERR_NONE) {
            LOGE("SLQSNasIndicationRegister: enable failed %lu\n", resultCode );
            return resultCode;
        }

        /* Subscribe to serving system indication */
        resultCode = SLQSSetServingSystemCallback( &SLQSSetServingSystemCB );
        if (resultCode != eQCWWAN_ERR_NONE) {
            LOGE("SLQSSetServingSystemCallback: failed to enable %lu\n", resultCode );
            return resultCode;
        }

        /* 
         * Subscribe to Data Bearer callback, to indicate change in Data Bearer
         */
        resultCode = SetDataBearerCallback(&setDataBearerCB);
        if (resultCode != eQCWWAN_ERR_NONE) {
            LOGE("SetDataBearerCallback: failed to enable %lu\n", resultCode );
            return resultCode;
        }

        /* Register for the Signal Strength Indication */
        registerSignalStrengthCallbackQMI();

    } else {
        /* will not turn off network indication if property value != 0 */
        if (property_get("persist.sierra.lcd_off_net_ind", propValue, NULL) > 0) 
        {
            intpropvalue = atoi(propValue);
            LOGD("persist.sierra.lcd_off_net_ind =  %d\n", intpropvalue );
            if(intpropvalue>0)
            {
                resultCode = SetDataBearerCallback( NULL );
                if (resultCode != eQCWWAN_ERR_NONE) {
                    LOGE("SetDataBearerCallback: failed to disable %lu\n", resultCode );
                    return resultCode;
                }

                INT8 thresholds[SIGNAL_STRENGTH_THRESHOLDS_SIZE] = {0,0,0,0,0};
                resultCode = SetSignalStrengthCallback( NULL,
                                                        SIGNAL_STRENGTH_THRESHOLDS_SIZE,
                                                        thresholds );
                if (resultCode != eQCWWAN_ERR_NONE) {
                    LOGE("SetSignalStrengthCallback: failed to disable %lu\n", resultCode );
                    return resultCode;
                }
                /* Success */
                return 0;
            }
        }
        /* Screen is off - disable all NAS broadcast indications */
        resultCode = SLQSNasIndicationRegister (0, 0, 0);
        if (resultCode != eQCWWAN_ERR_NONE) {
            LOGE("SLQSSetServingSystemCallback: failed to disable %lu\n", resultCode );
            return resultCode;
        }

        /* Screen is off - disable all unsolicited notifications. */
        resultCode = SLQSSetServingSystemCallback( NULL );
        if (resultCode != eQCWWAN_ERR_NONE) {
            LOGE("SLQSSetServingSystemCallback: failed to disable %lu\n", resultCode );
            return resultCode;
        }

        resultCode = SetDataBearerCallback( NULL );
        if (resultCode != eQCWWAN_ERR_NONE) {
            LOGE("SetDataBearerCallback: failed to disable %lu\n", resultCode );
            return resultCode;
        }

        INT8 thresholds[SIGNAL_STRENGTH_THRESHOLDS_SIZE] = {0,0,0,0,0};
        resultCode = SetSignalStrengthCallback( NULL,
                                                SIGNAL_STRENGTH_THRESHOLDS_SIZE,
                                                thresholds );
        if (resultCode != eQCWWAN_ERR_NONE) {
            LOGE("SetSignalStrengthCallback: failed to disable %lu\n", resultCode );
            return resultCode;
        }
    }
    /* Success */
    return 0;
}

/**
 * RIL_REQUEST_SCREEN_STATE
 *
 * Indicates the current state of the screen.  When the screen is off, the
 * RIL should notify the baseband to suppress certain notifications (eg,
 * signal strength and changes in LAC/CID or BID/SID/NID/latitude/longitude)
 * in an effort to conserve power. These notifications should resume when the
 * screen is on.
 *
 * See also: RIL_UNSOL_RESPONSE_NETWORK_STATE_CHANGED.
 */
void requestScreenStateQMI(void *data, size_t datalen, RIL_Token t)
{
    int err, screenState;

    assert(datalen >= sizeof(int *));

    pthread_mutex_lock(&s_screen_state_mutex);
    screenState = s_screenState = ((int *) data)[0];

    if ((screenState == 1) || (screenState == 0)) {
        err = setQMINotificationState(screenState);
        if (err != 0)
            goto error;
    } else {
        LOGE("ERROR: Not a defined value screen state value");
        goto error;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);

finally:
    /* Trigger a refresh of network cache info and poll signal strength, just to be sure. */
    pthread_mutex_unlock(&s_screen_state_mutex);
    if(screenState == 1) {
        swiril_cache_clearnetworkdata();
        RIL_onUnsolicitedResponse( RIL_UNSOL_RESPONSE_NETWORK_STATE_CHANGED,
                                   NULL,
                                   0 );
        enqueueRILEvent(RIL_EVENT_QUEUE_NORMAL, pollSignalStrength,
                        NULL, NULL);
    }
    return;

error:
    LOGE("ERROR: requestScreenState failed");
    if (t != (RIL_Token) 0)
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);

    goto finally;
}

/**
 *
 * map Band table from RIL band mode to QMI band description.
 *
 * @param ullBandTable
 *     QMI band descprition table.
 * @param iBandSize
 *     Size of the band table.
 *
 * @return
 *     true     : success
 *     false    : failure
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
bool BandMapFromRIL2QMI(ULONGLONG *ullBandTable, int iBandSize)
{
    int i = 0;
    ULONGLONG ullBand[RIL_BAND_MAX] = {
        /* All band, equal to band capability get from modem */
        0,

        /* EU band */
        (ULONGLONG)1<<QMI_GSM_900_PRIMARY | 1<<QMI_GSM_900_EXTEND | 1<<QMI_GSM_DCS_1800 |
                   1<<QMI_WCDMA_I_2100 |  (ULONGLONG)1<<QMI_WCDMA_VIII_900,

        /* NA band */
        (ULONGLONG)1<<QMI_GSM_850 | 1<<QMI_GSM_PCS_1900 | 1<<QMI_WCDMA_V_850 | 
                   1<<QMI_WCDMA_II_PCS_1900,

        (ULONGLONG)1<<QMI_WCDMA_VI_800| 1<<QMI_WCDMA_I_2100,

        (ULONGLONG)1<<QMI_GSM_900_PRIMARY| 1<<QMI_GSM_900_EXTEND | 1<<QMI_GSM_DCS_1800 | 
                   1<<QMI_WCDMA_V_850| 1<<QMI_WCDMA_I_2100,

        (ULONGLONG)1<<QMI_GSM_900_PRIMARY| 1<<QMI_GSM_900_EXTEND | 1<<QMI_GSM_DCS_1800 |
                   1<<QMI_WCDMA_V_850,

        (ULONGLONG)1<<QMI_BAND_CLASS_0A | 1<<QMI_BAND_CLASS_0B,
        (ULONGLONG)1<<QMI_BAND_CLASS_1,
        (ULONGLONG)1<<QMI_BAND_CLASS_3,
        (ULONGLONG)1<<QMI_BAND_CLASS_4,
        (ULONGLONG)1<<QMI_BAND_CLASS_5,
        (ULONGLONG)1<<QMI_BAND_CLASS_6,
        (ULONGLONG)1<<QMI_BAND_CLASS_7,
        (ULONGLONG)1<<QMI_BAND_CLASS_8,
        (ULONGLONG)1<<QMI_BAND_CLASS_9,
        (ULONGLONG)1<<QMI_BAND_CLASS_10,
        (ULONGLONG)1<<QMI_BAND_CLASS_11,
        (ULONGLONG)1<<QMI_BAND_CLASS_15,
        (ULONGLONG)1<<QMI_BAND_CLASS_16
    };

    if((ullBandTable == NULL) || (iBandSize != RIL_BAND_MAX)){
        LOGE("%s:: bad parametere", __func__);
        return false;    
    }
    for (i = 0; i < iBandSize; i++)
        ullBandTable[i] = ullBand[i];

    return true;
}     
/**
 * RIL_REQUEST_QUERY_AVAILABLE_BAND_MODE
 *
 * Query the list of band mode supported by RF.
 *
 * See also: RIL_REQUEST_SET_BAND_MODE
 */
void requestQueryAvailableBandModeQMI(void *data, size_t datalen, RIL_Token t)
{
    int       response[RIL_BAND_MAX+1] = {0};
    int       i = 0;
    int       nSize = 0;
    ULONGLONG BandCapability = 0;
    ULONGLONG ullQMIBandTable[RIL_BAND_MAX] = {0};
    ULONG     nRet = 0;
    RIL_Errno errno = RIL_E_GENERIC_FAILURE;
    SWI_FW_INFO_TYPE tech = SWI_FW_INFO_TYPE_UNKNOWN;

    /* call Sierra QMI interface */
    nRet = SLQSGetBandCapability(&BandCapability);
    if (nRet != eQCWWAN_ERR_NONE) {
        LOGE("%s:: SLQSGetBandCapability failed, error: %lu", __func__, nRet);
        goto error;
    }
    LOGD("%s:: SLQSGetBandCapability BandCapability = 0x%016llx", __func__, BandCapability);

    /* map table from RIL band mode to QMI band */
    if (!BandMapFromRIL2QMI(&ullQMIBandTable[0], RIL_BAND_MAX)) {
        LOGE("%s:: MapFromRIL2QMI failed", __func__);
        goto error;
    }

    /* response points to an array of int's, the int[0] is the size of array, 
       reset is one for each available band mode. */
    /* Always return automatic band in response[1] */
    response[1] = 0;

    nSize = 2;
    for (i = 1; i < RIL_BAND_MAX; i++) {
        if (BandCapability & ullQMIBandTable[i]) {
            LOGD("%s:: %d was selected ", __func__, i);
            response[nSize] = i;
            nSize++;
        }
    }

    response[0] = nSize;
    LOGD("%s:: response size :%d", __func__, nSize);

    RIL_onRequestComplete(t, RIL_E_SUCCESS, response, nSize * sizeof(int));
    return;
error:
    RIL_onRequestComplete(t, errno, NULL, 0);
}

static void SLQSSetBandPreferenceCB(
    ULONGLONG band_pref )
{
    /* this callback communicates the current preferred band selection for the device*/
    LOGD("%s:: band_pref = 0x%016llx", __func__, band_pref);

    /* SWI_TBD: The variable g_CurrentBand is accessed by multiple threads. This 
       assignment will not be atomic because g_CurrentBand is too large, although
       the worst that will happen is that the comparison in wait4BandSet() will 
       return FALSE for one more iteration of the loop.  
       However, this should still be fixed at some point */
    g_CurrentBand = band_pref;
}

void registerSetBandPreferenceCallbackQMI(void)
{
    ULONG nRet;

    nRet = SLQSSetBandPreferenceCbk(&SLQSSetBandPreferenceCB);

    if (nRet != eQCWWAN_ERR_NONE) {
        LOGE("%s:: SLQSSetBandPreferenceCbk failed to enable callback %lu\n", __func__, nRet );
    }
}

/**
 *
 * Wait to check whether setting is successful by comparing band from callback
 *
 *
 * @return
 *     true  - success 
 *     false - otherwise
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
bool wait4BandSet(ULONGLONG BandSelection)
{
    int               i = 0;
    int               iWaitTime = 60; /* wait for 60 seconds for setting */

    for (i = 0; i < iWaitTime; i++) {
        if (g_CurrentBand == BandSelection) {
            LOGD("%s:: Band selection succeed\n", __func__);
            return true;
        }
        sleep(1);
    }

    return false;
}
    
/**
 * RIL_REQUEST_SET_BAND_MODE
 *
 * Assign a specified band for RF configuration.
*/
void requestSetBandModeQMI(void *data, size_t datalen, RIL_Token t)
{
    ULONGLONG         QMIBandSet = 0;
    ULONGLONG         BandCapability = 0;
    ULONGLONG         ullQMIBandTable[RIL_BAND_MAX] = {0};
    RIL_Errno         errno = RIL_E_GENERIC_FAILURE;
    ULONG             nRet = 0;
    RIL_BandSelection bandMode = ((int *)data)[0];

    /* map table from RIL band mode to QMI band */
    if (!BandMapFromRIL2QMI(&ullQMIBandTable[0], RIL_BAND_MAX)) {
        LOGE("%s:: MapFromRIL2QMI failed", __func__);
        goto error;
    }

    /* The logic for setting band mode
     * 1) Get band selection capability
     * 2) Check availability of the band to be set
     * 3) Assign the band preference to firmware
     * 4) Check whether setting is successful by comparing band from callback */ 

    /* Step 1,  get available band selection */
    /* call Sierra QMI interface */
    nRet = SLQSGetBandCapability(&BandCapability);
    if (nRet != eQCWWAN_ERR_NONE) {
        LOGE("%s:: SLQSGetBandCapability failed, error: %lu", __func__, nRet);
        goto error;
    }

    /* set Auto band selection to be band capability */
    ullQMIBandTable[0] = BandCapability;
    LOGD("%s:: BandCapability = 0x%016llx, Band selected 0x%016llx for mode %d", 
        __func__, BandCapability, ullQMIBandTable[bandMode], bandMode);

    /* Step 2, Check availability of band to be set */
    if ((BandCapability & ullQMIBandTable[bandMode]) == 0) {
        LOGE("%s:: unsupported band %d selected  \n", __func__, bandMode);
        goto error;
    }

    /* Check wheter current band is already the mode to be set*/
    if (g_CurrentBand == ullQMIBandTable[bandMode]) {
        LOGD("%s:: Current Band already in mode %d \n", __func__, bandMode);
        goto finally;
    }
    
    /* Step 3, Assign the band preference to firmware */
    /* call Sierra QMI interface */
    nRet = SLQSSetBandPreference(ullQMIBandTable[bandMode]);
    if (nRet == eQCWWAN_ERR_QMI_NO_EFFECT) {
        /* eQCWWAN_ERR_QMI_NO_EFFECT was returned when modem already in such band mode */
        LOGD("%s:: nRet: %lu, Current Band already in mode %d \n", __FUNCTION__, nRet, bandMode);
        goto finally;
    } else if (nRet != eQCWWAN_ERR_NONE) {
        LOGE("%s:: SLQSSetBandPreference to mode: %d, QMI: 0x%016llx failed, error: %lu", 
            __func__, bandMode, ullQMIBandTable[bandMode], nRet);
        goto error;
    }

    /* Step 4, check whether setting is successful by comparing band from callback */
    /* wait 60 seconds to see whether the setting is successful */
    if (!wait4BandSet(ullQMIBandTable[bandMode])) {
        LOGE("%s:: wait4BandSet failed \n", __func__);
        goto error;
    }
    
finally:
    /* send back success response*/
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;

error:
    RIL_onRequestComplete(t, errno, NULL, 0);
}

bool getLacCellID(int *lac, ULONG *cellID)
{
    ULONG nRet;
    qaQmiServingSystemParam tqaQmiServingSystemParam;
    int response[VOICE_REGISTRATION_STATE_PARAMS];
    bool hasAct = false;
    bool isValid = false;
    int i;
    ULONG CID = 0;
    bool use_cgreg;
    
    /* SWI_TBD no CID and LAC info from QMI */
    nRet = SLQSGetServingSystem(&tqaQmiServingSystemParam);
    if (nRet != eQCWWAN_ERR_NONE) {
        LOGE("%s:: SLQSGetServingSystem failed, error: %lu", __func__, nRet);
    } else {
        printSLQSGetServingSystem((void *)&tqaQmiServingSystemParam);
        if ((tqaQmiServingSystemParam.Lac != 0xFFFF) &&
            (tqaQmiServingSystemParam.CellID != 0xFFFFFFFF) ) {
            *lac = tqaQmiServingSystemParam.Lac;
            *cellID = tqaQmiServingSystemParam.CellID;
            isValid = true;
        }
    }

    /* Work around to get CID and LAC info from AT */
    /* Enable location information unsolicited result code temporarily, 
     * ignore the response, not VITAL. */
    if (!isValid) {
        at_send_command("AT+CGREG=2", NULL); 
        at_send_command("AT+CREG=2", NULL); 
        
        /* Although QMI has returned registered, it may only be CS attached or
           PS attached but not both.  Try AT+CGREG the first time through the
           loop; if that fails, try AT+CREG the second time through the loop.
           If both fail then the return values are not valid.
         */
        use_cgreg = true;
        for (i = 0; (i < 2) && !isValid; i++) {
            memset(response, 0, sizeof(response));
            if (queryRegState(use_cgreg, &response[0], &CID, &hasAct) &&
                (response[1] > 0) && 
                (CID > 0)) {
                LOGI("%s update LAC %X Cell ID %lX", __func__, response[1], CID);
                *lac = response[1];
                *cellID = CID;
                isValid = true;
            }
            else {
                use_cgreg = false;
            }
        }
        /* Disable location information unsolicited result code */
        at_send_command("AT+CGREG=0", NULL);
        at_send_command("AT+CREG=0", NULL);
    }
    return isValid;
}
