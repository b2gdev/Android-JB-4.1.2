/**
 *
 * @ingroup swiril
 *
 * @file 
 * Provides OEM device specific Sierra functions
 *
 * @author
 * Copyright: (C) 2012 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#include <telephony/ril.h>
#include <stdbool.h>
#include "swiril_main.h"
#include "swiril_misc_qmi.h"
#include "swiril_sim_qmi.h"
#include "swiril_network_qmi.h"
#include "at_channel.h"
#include "at_tok.h"

/* For QMI */
#include "qmerrno.h"
#include "SWIWWANCMAPI.h"

#define LOG_TAG "RIL"
#include "swiril_log.h"

/**< MIP profile index set to 1 now */
#define SWI_MIP_PROFILE_INDEX 1

/* Both Get and Set parameters use string array and the length in comments below excluding '\0' at the end of string */
typedef enum {
    OEM_PPDATAP_USER_NAI = 0,           /**< NAI string of MIP profile, ASCII string */
    OEM_PPDATAP_REV_TUNNELING_SETTING,  /**< Reverse Tunneling of MIP profile, one byte long decimal string */
    OEM_PPDATAP_HA_SPI,                 /**< HA SPI of MIP profile, up to 4 bytes long hexadecimal value string */
    OEM_PPDATAP_AAA_SPI,                /**< AAA SPI of MIP profile, up to 4 bytes long hexadecimal value string */
    OEM_PPDATAP_HOME_IP,                /**< Home address(IPv4) of MIP profile, up to 4 bytes long hexadecimal value string */
    OEM_PPDATAP_PRIMARY_HA_IP,          /**< HA address(IPv4) of MIP profile, up to 4 bytes long hexadecimal value string */
    OEM_PPDATAP_SECONDARY_HA_IP,        /**< AAA address(IPv4) of MIP profile, up to 4 bytes long hexadecimal value string */
    OEM_PPDATAP_MEID,                   /**< MEID, ASCII string */
    OEM_PPDATAP_P_ESN,                  /**< ESN, ASCII string */
    OEM_PPDATAP_STATION_CLASS_MARK,     /**< Station Class Mark, one byte long decimal string */
    OEM_PPDATAP_SLOTTED_MODE_INDEX,     /**< Slot Cycle Index, one byte long decimal string */
    OEM_PPDATAP_HOME_NID,               /**< Home NID, one byte long decimal string */
    OEM_PPDATAP_HOME_SID,               /**< Home SID, one byte long decimal string */
    OEM_PPDATAP_ACCOLC,                 /**< ACCOLC, one byte long decimal string */
    OEM_PPDATAP_HOME_SYS_REG,           /**< Register on home system, one byte long decimal string */
    OEM_PPDATAP_FSID,                   /**< Register on foreign system, one byte long decimal string */
    OEM_PPDATAP_FNID,                   /**< Register on foreign network, one byte long decimal string */
    OEM_PPDATAP_1X_CHANNEL,             /**< 1xrtt channel number, up to 4 bytes long decimal string */
    OEM_PPDATAP_1X_BAND_CLASS,          /**< 1xrtt band class, up to 4 bytes long decimal string */
    OEM_PPDATAP_1X_SYSTEM_ID,           /**< 1xrtt system ID, up to 4 bytes long decimal string */
    OEM_PPDATAP_1X_NETWORK_ID,          /**< 1xrtt network ID, up to 4 bytes long decimal string */
    OEM_PPDATAP_1X_BASE_ID,             /**< 1xrtt base ID, up to 4 bytes long decimal string */
    OEM_PPDATAP_1X_LATITUDE,            /**< 1xrtt base station latitude, up to 4 bytes long hexadecimal value string */
    OEM_PPDATAP_1X_LONGTITUDE,          /**< 1xrtt base station longtitude, up to 4 bytes long hexadecimal value string */
    OEM_PPDATAP_1X_RX_PWR,              /**< 1xrtt RSSI, up to 4 bytes long decimal string */
    OEM_PPDATAP_1X_RX_ECIO,             /**< 1xrtt ECIO, up to 4 bytes long decimal string */
    OEM_PPDATAP_1X_RX_ERROR_RATE,       /**< 1xrtt error rate, up to 4 bytes long decimal string */
    OEM_PPDATAP_EVDO_CHANNEL,           /**< EVDO channel number, up to 4 bytes long decimal string */
    OEM_PPDATAP_EVDO_RX_PWR,            /**< EVDO RSSI, up to 4 bytes long decimal string */
    OEM_PPDATAP_EVDO_RX_PER,            /**< EVDO Packet Error Rate, up to 4 bytes long decimal string */
    OEM_PPDATAP_EVDO_RX_SINR,           /**< EVDO SINR, up to 4 bytes long decimal string */
    OEM_PPDATAP_EVDO_AN_AAA_STATUS,     /**< EVDO AN-AAA status, up to 4 bytes long decimal string */
    OEM_PPDATAP_EVDO_PRL,               /**< PRL version, up to 4 bytes long decimal string */
    OEM_PPDATAP_AMSS_VERSION,           /**< Firmware version, ASCII string */
    OEM_PPDATAP_P_REV,                  /**< Protocol version, up to 4 bytes long decimal string */
    OEM_PPDATAP_PACKET_DATA_PROFILE,    /**< Active MIP profile number, one byte long decimal string */
    OEM_PPDATAP_MAX
}OEM_HOOK_STRINGS_GET_PPDATAP;

typedef enum {
    OEM_SET_PPDATAP_SPC = 0,                /**< Service Programming Code, expecting up to 6 bytes long decimal value string */
    OEM_SET_PPDATAP_USER_NAI,               /**< NAI string of MIP profile, expecting ASCII string */
    OEM_SET_PPDATAP_REV_TUNNELING_SETTING,  /**< Reverse Tunneling of MIP profile, expecting one byte long decimal string */
    OEM_SET_PPDATAP_HA_SPI,                 /**< HA SPI of MIP profile, expecting up to 4 bytes long hexadecimal value string */
    OEM_SET_PPDATAP_HA_PASSWORD,            /**< HA password of MIP profile, expecting ASCII string */
    OEM_SET_PPDATAP_AAA_SPI,                /**< AAA SPI of MIP profile, expecting up to 4 bytes long hexadecimal value string */
    OEM_SET_PPDATAP_AAA_PASSWORD,           /**< AAA password of MIP profile, expecting ASCII string */
    OEM_SET_PPDATAP_HOME_IP,                /**< Home address(IPv4) of MIP profile, expecting up to 4 bytes long hexadecimal value string */
    OEM_SET_PPDATAP_PRIMARY_HA_IP,          /**< Primary HA address(IPv4) of MIP profile, expecting up to 4 bytes long hexadecimal value string */
    OEM_SET_PPDATAP_SECONDARY_HA_IP,        /**< Secondary HA address(IPv4) of MIP profile, expecting up to 4 bytes long hexadecimal value string */
    OEM_SET_PPDATAP_ACCOLC,                 /**< ACCOLC, expecting one byte long decimal string */
    OEM_SET_PPDATAP_MAX
}OEM_HOOK_STRINGS_SET_PPDATAP;

/**
 *  Device programming command ##DATA# get handler
 *
 *  @param  data [in]
 *          - Pointer to the string pointer array
 *  @param  count [in]
 *          - count of the number of strings pointed to by the data array
 *  @param  t [in]
 *          - A RIL token structure pointer
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */
void requestOEMHookStringsGetppData(const char **data, size_t count, RIL_Token t)
{
    ULONG nRet;
    int i;
    char *responseStrp[OEM_PPDATAP_MAX];
    
    BYTE Enabled;
    ULONG Address;
    ULONG PrimaryHA;
    ULONG SecondaryHA;
    BYTE RevTunneling;
    CHAR NAI[NUM_MAX_STR_LEN];
    ULONG HASPI;
    ULONG AAASPI;
    ULONG HAState;
    ULONG AAAState;
    
    CHAR  ESNString[ESN_STRING_LENGTH];
    CHAR  IMEIString[IMEI_STRING_LENGTH];
    CHAR  MEIDString[MEID_STRING_LENGTH];
    
    int prefType = 0;

    WORD  pMCC;
    WORD  pMNC;
    CHAR  pName[NUM_MAX_STR_LEN];
    WORD  pHomeSID;
    WORD  pHomeNID;

    BYTE  ACCOLC;

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
    
    ATResponse *atresponse = NULL;
    int err;
    char *line;
    int profile;
    
    qaQmiServingSystemParam tqaQmiServingSystemParam;
    
    BYTE  instancesSize = RAT_MAX;
    struct RFBandInfoElements instances[RAT_MAX];

    struct slqsSignalStrengthInfo signalInfo;
    
    ULONG AAAStatus;
    
    WORD  prlVersion;
    
    CHAR fmrString[NUM_MAX_STR_LEN];
    
    /* Subtract OEM hook sub-command */
    count--;

    /* No argument is the only acceptable choice */
    if( count != 0 ) {
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
        LOGE("%s - wrong number of args: %d", __func__, count+1);
        return;
    }

    memset(responseStrp, 0, sizeof(responseStrp));
    
    /* Mobile IP profile */
    nRet = GetMobileIPProfile(SWI_MIP_PROFILE_INDEX,
                              &Enabled,
                              &Address,
                              &PrimaryHA,
                              &SecondaryHA,
                              &RevTunneling,
                              sizeof(NAI),
                              &NAI[0],
                              &HASPI,
                              &AAASPI,
                              &HAState,
                              &AAAState);
    if (nRet == eQCWWAN_ERR_NONE) {
        asprintf(&responseStrp[OEM_PPDATAP_USER_NAI], "%s", NAI);
        asprintf(&responseStrp[OEM_PPDATAP_REV_TUNNELING_SETTING], "%d", RevTunneling);
        asprintf(&responseStrp[OEM_PPDATAP_HA_SPI], "%lx", HASPI);
        asprintf(&responseStrp[OEM_PPDATAP_AAA_SPI], "%lx", AAASPI);
        asprintf(&responseStrp[OEM_PPDATAP_HOME_IP], "%lx", Address);
        asprintf(&responseStrp[OEM_PPDATAP_PRIMARY_HA_IP], "%lx", PrimaryHA);
        asprintf(&responseStrp[OEM_PPDATAP_SECONDARY_HA_IP], "%lx", SecondaryHA);
    }
    else {
        LOGE("%s GetMobileIPProfile error  %lu", __func__, nRet);
        goto error;
    }
    
    /* MEID, ESN */
    nRet = GetSerialNumbers(sizeof(ESNString),
                            &ESNString[0],
                            sizeof(IMEIString),
                            &IMEIString[0],
                            sizeof(MEIDString),
                            &MEIDString[0]); 
    if (nRet == eQCWWAN_ERR_NONE) {
        asprintf(&responseStrp[OEM_PPDATAP_MEID], "%s", MEIDString);
        asprintf(&responseStrp[OEM_PPDATAP_P_ESN], "%s", ESNString);
    }
    else {
        LOGE("%s GetSerialNumbers error  %lu", __func__, nRet);
        goto error;
    }

    /* Home NID/SID */
    nRet = GetHomeNetwork(&pMCC,
                          &pMNC,
                          sizeof(pName),
                          &pName[0],
                          &pHomeSID,
                          &pHomeNID);
    if (nRet == eQCWWAN_ERR_NONE) {
        asprintf(&responseStrp[OEM_PPDATAP_HOME_NID], "%d", pHomeNID);
        asprintf(&responseStrp[OEM_PPDATAP_HOME_SID], "%d", pHomeSID);
    }
    else {
        LOGE("%s GetHomeNetwork error  %lu", __func__, nRet);
        goto error;
    }
    
    /* ACCOLC */
    nRet = GetACCOLC(&ACCOLC);
    if (nRet == eQCWWAN_ERR_NONE) {
        asprintf(&responseStrp[OEM_PPDATAP_ACCOLC], "%d", ACCOLC);
    }
    else {
        LOGE("%s GetACCOLC error  %lu", __func__, nRet);
        goto error;
    }
    
    /* SCM, SCI, Foreign SID/NID */
    nRet = GetCDMANetworkParameters(&SCI, 
                                    &SCM, 
                                    &RegHomeSID, 
                                    &RegForeignSID, 
                                    &RegForeignNID, 
                                    &ForceRev0, 
                                    &CustomSCP, 
                                    &Protocol, 
                                    &Broadcast, 
                                    &Application, 
                                    &Roaming );

    if (nRet == eQCWWAN_ERR_NONE) {
        asprintf(&responseStrp[OEM_PPDATAP_STATION_CLASS_MARK], "%d", SCM);
        asprintf(&responseStrp[OEM_PPDATAP_SLOTTED_MODE_INDEX], "%d", SCI);
        asprintf(&responseStrp[OEM_PPDATAP_HOME_SYS_REG], "%d", RegHomeSID);
        asprintf(&responseStrp[OEM_PPDATAP_FSID], "%d", RegForeignSID);
        asprintf(&responseStrp[OEM_PPDATAP_FNID], "%d", RegForeignNID);
    }
    else {
        LOGE("%s GetCDMANetworkParameters error  %lu", __func__, nRet);
        goto error;
    }
    
    /* SID, NID, base ID, latitude and longitude */
    nRet = SLQSGetServingSystem(&tqaQmiServingSystemParam);
    if (nRet == eQCWWAN_ERR_NONE) {
        asprintf(&responseStrp[OEM_PPDATAP_1X_SYSTEM_ID], "%d", tqaQmiServingSystemParam.SystemID);
        asprintf(&responseStrp[OEM_PPDATAP_1X_NETWORK_ID], "%d", tqaQmiServingSystemParam.NetworkID);
        asprintf(&responseStrp[OEM_PPDATAP_1X_BASE_ID], "%d", tqaQmiServingSystemParam.BasestationID);
        asprintf(&responseStrp[OEM_PPDATAP_1X_LATITUDE], "%lx", tqaQmiServingSystemParam.BasestationLatitude);
        asprintf(&responseStrp[OEM_PPDATAP_1X_LONGTITUDE], "%lx", tqaQmiServingSystemParam.BasestationLongitude);
    }
    else {
        LOGE("%s SLQSGetServingSystem error  %lu", __func__, nRet);
        goto error;
    }
    
    /* channel number, band class */
    nRet = GetRFInfo(&instancesSize, (BYTE *)&instances[0]);
    if (nRet == eQCWWAN_ERR_NONE) {
        for (i = 0; i < instancesSize; i++) {
            if (instances[i].radioInterface == RAT_CDMA_1XRTT) {
                asprintf(&responseStrp[OEM_PPDATAP_1X_CHANNEL], "%lu", instances[i].activeChannel);
                asprintf(&responseStrp[OEM_PPDATAP_1X_BAND_CLASS], "%lu", instances[i].activeBandClass);
            }
            else if (instances[i].radioInterface == RAT_CDMA_1XEVDO) {
                asprintf(&responseStrp[OEM_PPDATAP_EVDO_CHANNEL], "%lu", instances[i].activeChannel);
            }
        }
    }
    else if (nRet == eQCWWAN_ERR_QMI_INFO_UNAVAILABLE) {
        LOGD("%s GetRFInfo return %lu - not register yet", __func__, nRet);
    }
    else {
        LOGE("%s GetRFInfo error  %lu", __func__, nRet);
        goto error;
    }
    
    /* RSSI, ECIO, error rate, and SINR */
    signalInfo.signalStrengthReqMask = 0xFF; /* Mask all the bits of "Request Mask" to retrieve complete info */
    nRet = SLQSGetSignalStrength( &signalInfo );
    if (nRet == eQCWWAN_ERR_NONE) {
        for (i = 0; i < signalInfo.rxSignalStrengthListLen; i++) {
            if (signalInfo.rxSignalStrengthList[i].radioIf == RAT_CDMA_1XRTT) {
                asprintf(&responseStrp[OEM_PPDATAP_1X_RX_PWR], "%d", signalInfo.rxSignalStrengthList[i].rxSignalStrength);
            }
            else if (signalInfo.rxSignalStrengthList[i].radioIf == RAT_CDMA_1XEVDO) {
                asprintf(&responseStrp[OEM_PPDATAP_EVDO_RX_PWR], "%d", signalInfo.rxSignalStrengthList[i].rxSignalStrength);
            }
        }
        for (i = 0; i < signalInfo.ecioListLen; i++) {
            if (signalInfo.ecioList[i].radioIf == RAT_CDMA_1XRTT) {
                asprintf(&responseStrp[OEM_PPDATAP_1X_RX_ECIO], "%d", signalInfo.ecioList[i].ecio);
                break;
            }
        }
        for (i = 0; i < signalInfo.errorRateListLen; i++) {
            if (signalInfo.errorRateList[i].radioIf == RAT_CDMA_1XRTT) {
                asprintf(&responseStrp[OEM_PPDATAP_1X_RX_ERROR_RATE], "%d", signalInfo.errorRateList[i].errorRate);
            }
            else if (signalInfo.errorRateList[i].radioIf == RAT_CDMA_1XEVDO) {
                asprintf(&responseStrp[OEM_PPDATAP_EVDO_RX_PER], "%d", signalInfo.errorRateList[i].errorRate);
            }
        }
        asprintf(&responseStrp[OEM_PPDATAP_EVDO_RX_SINR], "%d", signalInfo.sinr);
    }
    else {
        LOGE("%s SLQSGetSignalStrength error  %lu", __func__, nRet);
        goto error;
    }
    
    /* AN-AAA */
    nRet = GetANAAAAuthenticationStatus(&AAAStatus);
    if (nRet == eQCWWAN_ERR_NONE) {
        asprintf(&responseStrp[OEM_PPDATAP_EVDO_AN_AAA_STATUS], "%lu", AAAStatus);
    }
    else {
        LOGE("%s GetANAAAAuthenticationStatus error  %lu", __func__, nRet);
        goto error;
    }
    
    /* PRL */
    nRet = GetPRLVersion(&prlVersion);
    if (nRet == eQCWWAN_ERR_NONE) {
        asprintf(&responseStrp[OEM_PPDATAP_EVDO_PRL], "%d", prlVersion);
    }
    else {
        LOGE("%s GetPRLVersion error  %lu", __func__, nRet);
        goto error;
    }
    
    /* Firmware version */
    nRet = GetFirmwareRevision(sizeof(fmrString), &fmrString[0]);
    if (nRet == eQCWWAN_ERR_NONE) {
        asprintf(&responseStrp[OEM_PPDATAP_AMSS_VERSION], "%s", fmrString);
    }
    else {
        LOGE("%s GetFirmwareRevision error  %lu", __func__, nRet);
        goto error;
    }

    /* Protocol version */
    err = at_send_command_numeric("AT$QCPREV", &atresponse);

    if (err < 0 || atresponse->success == 0) {
        LOGE("%s AT$QCPREV failed with %d", __func__, err);
        at_response_free(atresponse);
        goto error;
    }
    asprintf(&responseStrp[OEM_PPDATAP_P_REV], "%d", atoi(atresponse->p_intermediates->line));
    at_response_free(atresponse);

    /* Active MIP profile */
    err = at_send_command_singleline("AT$QCMIPP?", "$QCMIPP:", &atresponse);

    if (err < 0 || atresponse->success == 0) {
        LOGE("%s AT$QCMIPP? failed with %d", __func__, err);
        at_response_free(atresponse);
        goto error;
    }
    line = atresponse->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0)
        goto error;

    err = at_tok_nextint(&line, &profile);
    if (err < 0)
        goto error;
    asprintf(&responseStrp[OEM_PPDATAP_PACKET_DATA_PROFILE], "%d", profile);
    at_response_free(atresponse);
    
    LOGD("%s print response", __func__);
    for (i = 0; i < OEM_PPDATAP_MAX; i++) {
        LOGD("%d '%s' ", i, responseStrp[i]);
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, responseStrp, OEM_PPDATAP_MAX * sizeof(char *));
    
finally:
    for (i = 0; i < OEM_PPDATAP_MAX; i++) {
        if (responseStrp[i] != NULL)
            free(responseStrp[i]);
    }
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    goto finally;
}

/**
 *  Device programming command ##DATA# set handler
 *
 *  @param  data [in]
 *          - Pointer to the string pointer array
 *  @param  count [in]
 *          - count of the number of strings pointed to by the data array
 *  @param  t [in]
 *          - A RIL token structure pointer
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */
void requestOEMHookStringsSetppData(const char **data, size_t count, RIL_Token t)
{
    const char **args = data;
    int i;
    ULONG nRet;
    CHAR  *SPCp;
    CHAR  *NAIp;
    ULONG homeIP;
    ULONG *homeIPp = NULL;
    ULONG primaryHA;
    ULONG *primaryHAp = NULL;
    ULONG secondaryHA;
    ULONG *secondaryHAp = NULL;
    BYTE  revTunneling;
    BYTE  *revTunnelingp = NULL;
    ULONG HASPI;
    ULONG *HASPIp = NULL;
    ULONG AAASPI;
    ULONG *AAASPIp = NULL;
    CHAR  *HAPasswordp;
    CHAR  *AAAPasswordp;
    bool hasACCOLC = false;
    BYTE  ACCOLC;
    
    /* Advance to the next argument */
    args++;
    count--;

    /* Argument number has to match required set list number */
    if( count != OEM_SET_PPDATAP_MAX ) {
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
        LOGE("%s - wrong number of args: %d", __func__, count+1);
        return;
    }
    
    LOGD("%s print input", __func__);
    for (i = 0; i < OEM_SET_PPDATAP_MAX; i++) {
        LOGD("%d '%s' ", i, args[i]);
    }
    
    /* Check and assign parameters. */
    SPCp = (char *)args[OEM_SET_PPDATAP_SPC];
    NAIp = (char *)args[OEM_SET_PPDATAP_USER_NAI];
    if (args[OEM_SET_PPDATAP_REV_TUNNELING_SETTING] != NULL) {
        revTunneling = atoi(args[OEM_SET_PPDATAP_REV_TUNNELING_SETTING]);
        revTunnelingp = &revTunneling;
    }
    if (args[OEM_SET_PPDATAP_HA_SPI] != NULL) {
        HASPI = strtoul(args[OEM_SET_PPDATAP_HA_SPI], NULL, 16);
        HASPIp = &HASPI;
    }
    HAPasswordp = (char *)args[OEM_SET_PPDATAP_HA_PASSWORD];
    if (args[OEM_SET_PPDATAP_AAA_SPI] != NULL) {
        AAASPI = strtoul(args[OEM_SET_PPDATAP_AAA_SPI], NULL, 16);
        AAASPIp = &AAASPI;
    }
    AAAPasswordp = (char *)args[OEM_SET_PPDATAP_AAA_PASSWORD];
    if (args[OEM_SET_PPDATAP_HOME_IP] != NULL) {
        homeIP = strtoul(args[OEM_SET_PPDATAP_HOME_IP], NULL, 16);
        homeIPp = &homeIP;
    }
    if (args[OEM_SET_PPDATAP_PRIMARY_HA_IP] != NULL) {
        primaryHA = strtoul(args[OEM_SET_PPDATAP_PRIMARY_HA_IP], NULL, 16);
        primaryHAp = &primaryHA;
    }
    if (args[OEM_SET_PPDATAP_SECONDARY_HA_IP] != NULL) {
        secondaryHA = strtoul(args[OEM_SET_PPDATAP_SECONDARY_HA_IP], NULL, 16);
        secondaryHAp = &secondaryHA;
    }
    if (args[OEM_SET_PPDATAP_ACCOLC] != NULL) {
        ACCOLC = atoi(args[OEM_SET_PPDATAP_ACCOLC]);
        hasACCOLC = true;
    }

    /* Mobile IP profile */
    nRet = SetMobileIPProfile(SPCp,
                              SWI_MIP_PROFILE_INDEX,
                              NULL,
                              homeIPp,
                              primaryHAp,
                              secondaryHAp,
                              revTunnelingp,
                              NAIp,
                              HASPIp,
                              AAASPIp,
                              HAPasswordp,
                              AAAPasswordp);
    if (nRet != eQCWWAN_ERR_NONE) {
        LOGE("%s SetMobileIPProfile error %lu", __func__, nRet);
        goto error;
    }
    
    /* ACCOLC */
    if (hasACCOLC) {
        nRet = SetACCOLC(SPCp, ACCOLC);
        if (nRet != eQCWWAN_ERR_NONE) {
            LOGE("%s SetACCOLC error %lu", __func__, nRet);
            goto error;
        }
    }
    
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 *  Device programming command to validate SPC code
 *
 *  @param  data [in]
 *          - Pointer to the string pointer array
 *  @param  count [in]
 *          - count of the number of strings pointed to by the data array
 *  @param  t [in]
 *          - A RIL token structure pointer
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 *
 */
void requestOEMHookStringsValidateSPC(const char **data, size_t count, RIL_Token t)
{
    const char **args = data;
    char *responseStrp[1];
    ULONG nRet;
    CHAR  *SPCp;
    
    /* Advance to the next argument */
    args++;
    count--;

    /* Argument number has to be 1 */
    if( count != 1 ) {
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
        LOGE("%s - wrong number of args: %d", __func__, count+1);
        return;
    }
    
    SPCp = (char *)args[0];
    nRet = ValidateSPC(SPCp);
    if (nRet == eQCWWAN_ERR_NONE) {
        LOGD("%s SPC ok", __func__);
        responseStrp[0] = "1";
    }
    else {
        LOGE("%s ValidateSPC error  %lu", __func__, nRet);
        responseStrp[0] = "0";
    }
    RIL_onRequestComplete(t, RIL_E_SUCCESS, responseStrp, sizeof(char *));
}
