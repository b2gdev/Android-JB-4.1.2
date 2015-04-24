/* 
** This source code is "Not a Contribution" under Apache license
**
** Sierra Wireless RIL
**
** Based on reference-ril by The Android Open Source Project
** and U300 RIL by ST-Ericsson.
** Modified by Sierra Wireless, Inc.
**
** Copyright (C) 2011 Sierra Wireless, Inc.
** Copyright (C) ST-Ericsson AB 2008-2009
** Copyright 2006, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
** Based on reference-ril by The Android Open Source Project.
**
** Heavily modified for ST-Ericsson U300 modems.
** Author: Christian Bejram <christian.bejram@stericsson.com>
*/

#include <stdio.h>
#include <telephony/ril.h>
#include <cutils/properties.h>

#define LOG_TAG "RIL"
#include "swiril_log.h"

#include "swiril_misc.h"
#include "swiril_main.h"
#include "swiril_misc_qmi.h"
#include "swiril_device_qmi.h"
#include "swiril_pdp_common.h"
#include "swiril_pdp_qmi.h"
#include "swiril_network_qmi.h"
#include "swiril_main_qmi.h"

/* For QMI */
#include "qmerrno.h"
#include "sludefs.h"
#include "SWIWWANCMAPI.h"

static ULONG s_sessionIDQMI = 0;
static RIL_Data_Call_Response_v6 s_rilPDPContextListRes = { PDP_FAIL_NONE,
                                                            RIL_ACTIVATE_DATA_CALL_RETRY_NO_SUGGESTION,
                                                            0,
                                                            RIL_STATE_INACTIVE,
                                                            NULL,
                                                            NULL,
                                                            NULL,
                                                            NULL,
                                                            NULL };

/**
 *
 * Initialize RIL_Data_Call_Response(_v6) structure
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
void initDataCallResponseList(void)
{
    s_rilPDPContextListRes.status = PDP_FAIL_RADIO_POWER_OFF;

    /* Suggested retry times, default to -1 which means no value is suggested */
    s_rilPDPContextListRes.suggestedRetryTime = RIL_ACTIVATE_DATA_CALL_RETRY_NO_SUGGESTION;
    /* Context ID */
    s_rilPDPContextListRes.cid = 0;
    s_rilPDPContextListRes.active = RIL_STATE_INACTIVE;
    /* One of the PDP_type values in TS 27.007 section 10.1.1. */
    if (s_rilPDPContextListRes.type != NULL) {
        free(s_rilPDPContextListRes.type);
        s_rilPDPContextListRes.type = NULL;
    }
    /* This is done due to bug in Java layer. 
     * because it does not handle NULL for interface name, even when there is no active session 
     */
    s_rilPDPContextListRes.ifname = ril_iface;
    /* IP Addresses */
    if (s_rilPDPContextListRes.addresses != NULL) {
        free(s_rilPDPContextListRes.addresses);
        s_rilPDPContextListRes.addresses = NULL;
    }
    /* DNS */
    if (s_rilPDPContextListRes.dnses != NULL) {
        free(s_rilPDPContextListRes.dnses);
        s_rilPDPContextListRes.dnses = NULL;
    }
    /* Gateways */
    if (s_rilPDPContextListRes.gateways != NULL) {
        free(s_rilPDPContextListRes.gateways);
        s_rilPDPContextListRes.gateways = NULL;
    }
}

/**
 *
 * Sends PDP context information to the RIL based on a solicited
 * request or an unsolicited event
 *
 * @param[in] t 
 *     Pointer to theRIL token
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     Returns the data call list. An entry is added when a
 *     RIL_REQUEST_SETUP_DATA_CALL is issued and removed on a
 *     RIL_REQUEST_DEACTIVATE_DATA_CALL. The list is emptied
 *     when RIL_REQUEST_RADIO_POWER off/on is issued.
 */
void requestOrSendPDPContextListQMI(RIL_Token *t)
{
    /* Network interface name, never change right now */
    s_rilPDPContextListRes.ifname = ril_iface;
    
    /* START MOTO 172,174*/
    if (isDataSessionActive()) {
        s_rilPDPContextListRes.active = RIL_STATE_ACTIVE_LINKUP;
    }
    else {
        /* Clear data call Response List, this will set 
         * s_rilPDPContextListRes.active = RIL_STATE_INACTIVE
         */
            initDataCallResponseList();
            s_rilPDPContextListRes.status = PDP_FAIL_NONE;
        }
    /* Android CDMA onDataStateChanged() will process either NULL or data call list.
     * GSM onDataStateChanged() will only process data call list. For compatibility
     * with both, always return s_rilPDPContextListRes even when session
     * is inactive.
     */
    if (t != NULL) {
            RIL_onRequestComplete(*t, RIL_E_SUCCESS, &s_rilPDPContextListRes,
                                  sizeof(RIL_Data_Call_Response_v6));
        }
    else {
        /* RIL_UNSOL_DATA_CALL_LIST_CHANGED is not expected to be
         * issued because of an RIL_REQUEST_DEACTIVATE_DATA_CALL.
         * And make sure no RIL_UNSOL_DATA_CALL_LIST_CHANGED for power off.
         */
        if (s_rilPDPContextListRes.status != PDP_FAIL_RADIO_POWER_OFF) {
                RIL_onUnsolicitedResponse(RIL_UNSOL_DATA_CALL_LIST_CHANGED,
                                          &s_rilPDPContextListRes,
                                          sizeof(RIL_Data_Call_Response_v6));
            }
        }
    /* STOP MOTO 172,174 */
    return;
            
}

/**
 *
 * Map the IP family preference string to SLQS enum value
 *
 * @param[in] IPPrefp 
 *     Pointer to the IP family preference string
 *
 * @return
 *     enum QMI_IPFamilyPreference value
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     This function performs no pointer validation
 */
static QMI_IPFamilyPreference getIPFamilyPreference(char *IPPrefp)
{
    QMI_IPFamilyPreference IPFamilyPreference = QMI_IP_ADDRESS_UNSPEC;
    
    if ((0 == strcmp(IPPrefp, "IP")) || (0 == strcmp(IPPrefp, "IPV4"))) {
        IPFamilyPreference = QMI_IPV4_ADDRESS;
    }
    else if (0 == strcmp(IPPrefp, "IPV6")) {
        IPFamilyPreference = QMI_IPV6_ADDRESS;
    }
    else if (0 == strcmp(IPPrefp, "IPV4V6")) {
        IPFamilyPreference = QMI_IPV4V6_ADDRESS;
    }
    return IPFamilyPreference;
}

/**
 *
 * RIL_REQUEST_SETUP_DATA_CALL
 * Configure and activate PDP context for default IP connection.
 *
 * @param[in] data 
 *     Pointer to the request data
 * @param datalen 
 *     request data length
 * @param t 
 *     RIL token
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     This function performs no pointer validation
 */
void requestSetupDefaultPDPQMI(void *data, size_t datalen, RIL_Token t)
{
    const char *APN = NULL;
    char *property = NULL;
    int tech_ril;
    ULONG tech_qmi;
    int pidtype;
    char *username = NULL;
    char *password = NULL;
    const char *authtype;
    int authentype;
    int *authentypep;
    int pid;
    int pdp_active = 0;
    
    ULONG nRet;
    ULONG IPAddress = 0;
    ULONG sessionID = 0;
    ULONG FailureReason = 0;
    ULONG PDPType = GOBI_DEFAULT_PDP_TYPE;
    char *pdptype = NULL;
    int i;
    char *ipAddrStrp = NULL;
    USHORT IPAddressV6[8];
    BYTE IPv6prefixlen = 0;
    BYTE IPFamilyPreference = QMI_IPV4_ADDRESS;
    CHAR ProfileName[NUM_MAX_STR_LEN];
    ULONG PdpType;
    CHAR APNName[NUM_MAX_STR_LEN];
    ULONG PrimaryDNSV4;
    ULONG SecondaryDNSV4;
    struct UMTSQoS UMTSGrantedQos;
    struct GPRSQoS GPRSGrantedQos;
    CHAR Username[NUM_MAX_STR_LEN];
    ULONG Authentication;
    ULONG IPAddressV4;
    struct ProfileIdentifier ProfileID;
    ULONG GWAddressV4;
    ULONG SubnetMaskV4;
    BYTE PCSCFAddrPCO;
    struct PCSCFIPv4ServerAddressList ServerAddrList;
    struct PCSCFFQDNAddressList PCSCFFQDNAddrList;
    USHORT PrimDNSV6[8];
    USHORT SecondDNSV6[8];
    ULONG Mtu;
    struct DomainNameList DomainList;
    BYTE IPFamilyPref;
    BYTE IMCNflag;
    WORD Technology;
    struct IPV6AddressInfo IPV6AddrInfo;
    struct IPV6GWAddressInfo IPV6GWAddrInfo;
    struct WdsRunTimeSettings runTimeSettings = {
        &ProfileName[0],
        &PdpType,
        &APNName[0],
        &PrimaryDNSV4,
        &SecondaryDNSV4,
        &UMTSGrantedQos,
        &GPRSGrantedQos,
        &Username[0],
        &Authentication,
        &IPAddressV4,
        &ProfileID,
        &GWAddressV4,
        &SubnetMaskV4,
        &PCSCFAddrPCO,
        &ServerAddrList,
        &PCSCFFQDNAddrList,
        &PrimDNSV6[0],
        &SecondDNSV6[0],
        &Mtu,
        &DomainList,
        &IPFamilyPref,
        &IMCNflag,
        &Technology,
        &IPV6AddrInfo,
        &IPV6GWAddrInfo
    };

    /* Assign parameters. */
    tech_ril = atoi(((const char **)data)[RIL_PDP_PARAM_RADIO_TECH]);
    pidtype = atoi(((const char **)data)[RIL_PDP_PARAM_DATA_PROFILE]);
    APN = ((const char **) data)[RIL_PDP_PARAM_APN];
    username = (char *)(((const char **)data)[RIL_PDP_PARAM_APN_USERNAME]);
    password = (char *)(((const char **)data)[RIL_PDP_PARAM_APN_PASSWORD]);
    authtype = ((const char **)data)[RIL_PDP_PARAM_APN_AUTH_TYPE];
    if ((datalen >= (RIL_REQUEST_SETUP_DATA_CALL_REQUEST_PARAMS_MIN * sizeof(char *))) && 
        (char *)(((const char **)data)[RIL_PDP_PARAM_PDP_TYPE]) != NULL) {
        pdptype = (char *)(((const char **)data)[RIL_PDP_PARAM_PDP_TYPE]);
    }
    else {
        /* default to IPV4 */
        pdptype = "IP";
    }

    /* Network interface name, never change right now */
    s_rilPDPContextListRes.ifname = ril_iface;
    
    /* Check radio state first */
    if ((currentState() == RADIO_STATE_OFF) || (currentState() == RADIO_STATE_UNAVAILABLE)) {
        LOGE("requestSetupDefaultPDPQMI radio is not avaliable\n");
        setLastPdpFailCauseQMI(PDP_FAIL_RADIO_POWER_OFF);
        goto error;
    }
    
    IPFamilyPreference = getIPFamilyPreference(pdptype);
    if (IPFamilyPreference == QMI_IP_ADDRESS_UNSPEC) {
        LOGE("%s IPFamilyPreference unspecified\n", __func__);
        /* update status field for failure cause */
        s_rilPDPContextListRes.status = PDP_FAIL_UNKNOWN_PDP_ADDRESS_TYPE;
        goto finally;
    }

    /* Suggested retry times, default to -1 which means no value is suggested */
    LOGD("%s:: Radio Technology to use: %d\n", __func__, tech_ril);
    s_rilPDPContextListRes.suggestedRetryTime = RIL_ACTIVATE_DATA_CALL_RETRY_NO_SUGGESTION;

    /* SWI_TBD temprory disable this check for handoff */
#if 0
    /* Only support 1 data connection for now */
    if ((s_rilPDPContextListRes.active == RIL_STATE_ACTIVE_LINKUP) && (s_sessionIDQMI != 0)) {
        LOGE("requestSetupDefaultPDPQMI only 1 data connection supported by the modem\n");
        /* update status field for failure cause */
        s_rilPDPContextListRes.status = PDP_FAIL_ONLY_SINGLE_BEARER_ALLOWED;
        goto finally;
    }
#endif
    /* Gobi device only support IPV4 now */
    if (IsGobiDevice()) {
        if (IPFamilyPreference != QMI_IPV4_ADDRESS) {
            LOGE("%s Gobi device only supports IPV4\n", __func__);
            /* update status field for failure cause */
            s_rilPDPContextListRes.status = PDP_FAIL_ONLY_IPV4_ALLOWED;
            goto finally;
        }
    }

    /* Clean up some of the cache */
    /* active */
    s_rilPDPContextListRes.active = RIL_STATE_INACTIVE;
    /* DNS */
    if (s_rilPDPContextListRes.dnses != NULL) {
        free(s_rilPDPContextListRes.dnses);
        s_rilPDPContextListRes.dnses = NULL;
    }
    /* Gateways */
    if (s_rilPDPContextListRes.gateways != NULL) {
        free(s_rilPDPContextListRes.gateways);
        s_rilPDPContextListRes.gateways = NULL;
    }

    /* Stop the DHCP client if already running */
    if (!isDataSessionActive()) {
        checkDHCPnStopService();
    }

    LOGD("requesting data connection to APN '%s' IP preference '%s'", APN, pdptype);

    /* This is work around to get eHRPD from data bearer */
    if (!getPropertyRadioNotifiction() && isDualModeRunningCDMA()) {
        registerDormancyStatusCallbackQMI(true);
    }
    
    /* Start the data session for either UMTS or CDMA */
    if ((tech_ril == RIL_TECH_UMTS) ||
        (tech_ril -2 == RIL_RAT_GPRS) ||
        (tech_ril -2 == RIL_RAT_EDGE) ||
        (tech_ril -2 == RIL_RAT_UMTS) ||
        (tech_ril -2 == RIL_RAT_HSDPA) ||
        (tech_ril -2 == RIL_RAT_HSUPA) ||
        (tech_ril -2 == RIL_RAT_HSPA) ||
        (tech_ril -2 == RIL_RAT_LTE) ||
        (tech_ril -2 == RIL_RAT_HSPAP)) {    
        tech_qmi = QMI_TECH_UMTS;

        /* Determine PDP authentication type, if it was specified */
        authentypep = NULL;
        if (authtype != NULL) {
            authentype = atoi(authtype);
            authentypep = &authentype;

            /* check authentication type, RIL supports 0, 1, 2, 3 */
            if ((authentype < 0) || (authentype > 3)) {
                LOGE("requestSetupDefaultPDPQMI authentication type %d is not supported by modem", authentype);
                goto error;
            }
        }

        /* Start data session for UMTS.

           This relies on there being at least one defined profile on the modem.
           For Sierra devices, this should always be the case for a PRIed modem.
           For Gobi devices, a default profile is created during startup if there
           are no profiles currently defined. 

           SWI_TBD:
           SetDefaultProfile() cannot be used to create a profile if there are
           no currently defined profiles on the modem, because it will return
           an error in this case. Is this a bug in SetDefaultProfile() ?
         */
        if (IsGobiDevice()) {
            nRet = StartDataSession2 (
                        &tech_qmi,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        (char *)APN,
                        NULL,
                        (ULONG *)authentypep,
                        username,
                        password,
                        &sessionID,
                        &FailureReason );
        }
        else {
            nRet = SLQSSetIPFamilyPreference (IPFamilyPreference);
            if (nRet != eQCWWAN_ERR_NONE) {
                LOGE("%s failed to set IPFamilyPreference %d with error=%lu", __func__, IPFamilyPreference, nRet);
                goto error;
            }
            nRet = StartDataSessionLTE (
                        &tech_qmi,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        (char *)APN,
                        NULL,
                        NULL,
                        (ULONG *)authentypep,
                        username,
                        password,
                        &sessionID,
                        &FailureReason, 
                        IPFamilyPreference);
        }
    } else {
        /* CDMA2000 */
        tech_qmi = QMI_TECH_CDMA2000;

        /* start data session with IPV4 for CDMA */
        nRet = StartDataSession2 (
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    &sessionID,
                    &FailureReason );
    }

    /* nRet is the result from either StartDataSession2() or StartDataSessionLTE() */
    if (nRet == eQCWWAN_ERR_NONE) {
        /* Flag the connection is active */
        pdp_active = 1;
        /* Save the new session ID */
        s_sessionIDQMI = sessionID;

        /* call the API, get the address related values */
        nRet = SLQSGetRuntimeSettings( &runTimeSettings );
        
        if (nRet == eQCWWAN_ERR_NONE) {
            if (IPFamilyPreference == QMI_IPV4_ADDRESS) {
                getIPAddressStr(IPAddressV4, &ipAddrStrp);
                getIPAddresses2Str(PrimaryDNSV4, SecondaryDNSV4, &s_rilPDPContextListRes.dnses);
                getIPAddressStr(GWAddressV4, &s_rilPDPContextListRes.gateways);
                LOGI("%s V4 IP='%s' DNS='%s' gateway='%s'\n", 
                     __func__, ipAddrStrp, s_rilPDPContextListRes.dnses, s_rilPDPContextListRes.gateways);
            }
            else if (IPFamilyPreference == QMI_IPV6_ADDRESS) {
                getIPV6AddressStr(&IPV6AddrInfo.IPAddressV6[0], &ipAddrStrp);
                getIPV6Addresses2Str(&PrimDNSV6[0], &SecondDNSV6[0], &s_rilPDPContextListRes.dnses);
                getIPV6AddressStr(&IPV6GWAddrInfo.gwAddressV6[0], &s_rilPDPContextListRes.gateways);
                LOGI("%s V6 IP='%s' DNS='%s' gateway='%s'\n", 
                     __func__, ipAddrStrp, s_rilPDPContextListRes.dnses, s_rilPDPContextListRes.gateways);
            }
            else {
                getIPV4V6AddressesStr(IPAddressV4, &IPV6AddrInfo.IPAddressV6[0], &ipAddrStrp);
                getIPV4V6AddressesStr2(PrimaryDNSV4, SecondaryDNSV4, 
                                       &PrimDNSV6[0], &SecondDNSV6[0], 
                                       &s_rilPDPContextListRes.dnses);
                getIPV4V6AddressesStr(GWAddressV4, &IPV6GWAddrInfo.gwAddressV6[0], &s_rilPDPContextListRes.gateways);
                LOGI("%s V4V6 IP='%s' DNS='%s' gateway='%s'\n", 
                     __func__, ipAddrStrp, s_rilPDPContextListRes.dnses, s_rilPDPContextListRes.gateways);
            }
        }
        else {
            LOGE("%s SLQSGetRuntimeSettings failed %lu", __func__, nRet);
            goto error;
        }
    }
    /* Failure reason can only be translated if 
     * qmerror.h code is eQCWWAN_ERR_QMI_CALL_FAILED 
     */
    else if ( nRet == eQCWWAN_ERR_QMI_CALL_FAILED ) {
        /* An error occurred */
        LOGD("%s connection result: nRet: %d, Reason: %d\n",
             __func__, (int) nRet, (int) FailureReason);

        if ( retrievePDPRejectCauseQMI(FailureReason) < 0) {
            LOGE("Setup data call fail: %d, can't get last fail cause",(int) FailureReason);
        }
        else {
            LOGE("Setup data call fail: FailureReason %d, cause %d",
                 (int) FailureReason, getLastPdpFailCauseQMI());
        }
        goto error;
    }
    else if ( nRet == eQCWWAN_ERR_QMI_NO_EFFECT ) {
        /* check existing data session for handoff */
        if (isDataSessionActive()) {
            LOGI("%s eQCWWAN_ERR_QMI_NO_EFFECT sessionID: %lu\n", __func__, sessionID);
            /* Flag the connection is active */
            pdp_active = 1;
            
            /* compare IP Family Preference */
            if (IPFamilyPreference == getIPFamilyPreference(s_rilPDPContextListRes.type)) {
                /* call the API, get the address related values */
                nRet = SLQSGetRuntimeSettings( &runTimeSettings );
                if (nRet == eQCWWAN_ERR_NONE) {
    
                    if (IPFamilyPreference == QMI_IPV4_ADDRESS) {
                        getIPAddressStr(IPAddressV4, &ipAddrStrp);
                        LOGI("%s eQCWWAN_ERR_QMI_NO_EFFECT IP V4 Address: %lx, '%s'\n", __func__, IPAddressV4, ipAddrStrp);
                    }
                    else if (IPFamilyPreference == QMI_IPV6_ADDRESS) {
                        getIPV6AddressStr(&IPV6AddrInfo.IPAddressV6[0], &ipAddrStrp);
                        LOGI("%s eQCWWAN_ERR_QMI_NO_EFFECT IP V6 Address: '%s'\n", __func__, ipAddrStrp);
                    }
                    else {
                        getIPV4V6AddressesStr(IPAddressV4, &IPV6AddrInfo.IPAddressV6[0], &ipAddrStrp);
                        LOGI("%s eQCWWAN_ERR_QMI_NO_EFFECT IP V4V6 Address: '%s'\n", __func__, ipAddrStrp);
                    }
                    
                    /* check IP address with cache */
                    if (0 == strcmp(ipAddrStrp, s_rilPDPContextListRes.addresses)) {
                        LOGI("%s eQCWWAN_ERR_QMI_NO_EFFECT assume data seesion persist, return ok!", __func__);
                        goto response;
                    }
                    else {
                        LOGE("%s eQCWWAN_ERR_QMI_NO_EFFECT IP address not the same as cache", __func__);
                    }
                }
                else {
                    LOGE("%s eQCWWAN_ERR_QMI_NO_EFFECT Failed to get IP address\n",  __func__);
                }
            }
            else {
                LOGE("%s eQCWWAN_ERR_QMI_NO_EFFECT IPFamilyPreference is not match", __func__);
            }
        }
        else {
            LOGE("%s eQCWWAN_ERR_QMI_NO_EFFECT no active data session",__func__);
        }
        goto error;
    }
    else {
        LOGE("QMI SDK (qmerrno.h) reports error %d\n", (int) nRet );
        goto error;
    }

    /* SWI_TBD ICS dhcpcd doesn't support IPV6, so IPV6 related configuration will be done later */
    if (IPFamilyPreference != QMI_IPV6_ADDRESS) {
        /* Start dhcp client for IPV4 or IPV4V6, IP address and DNS got from SLQSGetRuntimeSettings() already */
        if (property_set("ctl.start", "sierra_dhcpcd")) {
            LOGE("requestSetupDefaultPDP FAILED to set ctl.start sierra_dhcpcd property!");
            goto error;
        }
    
        if (waitDHCPnsetDNS() < 0) {
            LOGE("waitDHCPnsetDNS returned error");
            goto error;
        }
    }
    
response:
    /* update cache of RIL_Data_Call_Response_v6 */
    /* status */
    s_rilPDPContextListRes.status = PDP_FAIL_NONE;
    /* CID */
    s_rilPDPContextListRes.cid = SWI_DEFAULT_CID;
    /* Data call state */
    s_rilPDPContextListRes.active = RIL_STATE_ACTIVE_LINKUP;
    /* PDP type */
    if (pdptype != NULL) {
        if (s_rilPDPContextListRes.type != NULL) {
            free(s_rilPDPContextListRes.type);
        }
        s_rilPDPContextListRes.type = strdup(pdptype);
    }
    /* IP Addresses */
    if (s_rilPDPContextListRes.addresses != NULL) {
        free(s_rilPDPContextListRes.addresses);
        s_rilPDPContextListRes.addresses = NULL;
    }
    if (ipAddrStrp != NULL)
        s_rilPDPContextListRes.addresses = strdup(ipAddrStrp);    
    
finally:

    if (ipAddrStrp != NULL)
        free(ipAddrStrp);
    /* Always return successful response */
    RIL_onRequestComplete(t, RIL_E_SUCCESS, &s_rilPDPContextListRes, sizeof(RIL_Data_Call_Response_v6));
    LOGD("requestSetupDefaultPDP done");
    return;

error:
    /* This is work around to get eHRPD from data bearer */
    if (!getPropertyRadioNotifiction() && isDualModeRunningCDMA()) {
        registerDormancyStatusCallbackQMI(false);
    }
    if ((pdp_active == 1) && (s_sessionIDQMI != 0)) {
        /* PDP_FAIL_ERROR_UNSPECIFIED will kick off retrying of data call */
        if(!isDataSessionActive())
            setLastPdpFailCauseQMI(PDP_FAIL_ERROR_UNSPECIFIED);
        else        
            setLastPdpFailCauseQMI(PDP_FAIL_PROTOCOL_ERRORS);
        isDataSessionCloseByClient(true);
        /* deactivate PDP context */
        nRet = StopDataSession ( s_sessionIDQMI );
        if ((nRet == eQCWWAN_ERR_NONE) || (nRet == eQCWWAN_ERR_QMI_NO_EFFECT)) {
            LOGI("requestSetupDefaultPDPQMI error handle: Stopped the session %lu\n", s_sessionIDQMI);
            s_sessionIDQMI = 0;
            /* stop DHCP client */
            checkDHCPnStopService();
            if (nRet == eQCWWAN_ERR_QMI_NO_EFFECT) {
                isDataSessionCloseByClient(false);
            }
        }
        else
        {
            LOGE("%s:: error handle: Stopped the session %lu failed with %lu \n", __func__, s_sessionIDQMI, nRet);
            isDataSessionCloseByClient(false);
        }
    }
    s_rilPDPContextListRes.status = getLastPdpFailCauseQMI();
    goto finally;
}

/**
 *
 * RIL_REQUEST_DEACTIVATE_DATA_CALL
 * Deactivate PDP context created by RIL_REQUEST_SETUP_DATA_CALL.
 *
 * @param[in] data 
 *     Pointer to the request data
 * @param datalen 
 *     request data length
 * @param t 
 *     RIL token
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     This function performs no pointer validation
 *     See also: RIL_REQUEST_SETUP_DEFAULT_PDP.
 */
void requestDeactivateDefaultPDPQMI(void *data, size_t datalen, RIL_Token t)
{
    const char *pPdpCid = NULL;
    int disconnectReason = RIL_DISCONNECT_REASON_NO_SPECIFIC;
    ULONG nRet;

    pPdpCid = ((const char **) data)[0];
    if ((((datalen/sizeof(char *)) >= RIL_REQUEST_DEACTIVATE_DATA_CALL_REQUEST_PARAMS)) && 
        (((const char **) data)[1] != NULL)) {
        disconnectReason = atoi(((const char **) data)[1]);
    }
    LOGD("requestDeactivateDefaultPDPQMI PdpCid '%s' DisconnectReason '%d'", pPdpCid, disconnectReason);

    /* Regardless of the disconnect reason, stop the data session first, and
     * then do any further actions below.  This way, we always know that the
     * data session was properly stopped.
     */    
    if ((pPdpCid != NULL) && (s_sessionIDQMI != 0)) {
        if (atoi(pPdpCid) == SWI_DEFAULT_CID) {
            isDataSessionCloseByClient(true);
            nRet = StopDataSession ( s_sessionIDQMI );
            if ((nRet != eQCWWAN_ERR_NONE) && (nRet != eQCWWAN_ERR_QMI_NO_EFFECT)) {
                LOGE("%s StopDataSession failed %lu\n", __func__, nRet);
                isDataSessionCloseByClient(false);
                goto error;
            } else if (nRet == eQCWWAN_ERR_QMI_NO_EFFECT) {
                isDataSessionCloseByClient(false);
            }
            LOGI("%s successfully stopped the session %lu\n", __func__, s_sessionIDQMI);
            s_sessionIDQMI = 0;
        }
        else {
            LOGE("requestDeactivateDefaultPDP CID is invalid %s\n", pPdpCid);
            goto error;
        }        
    }
    else {
        LOGE("requestDeactivateDefaultPDP error: CID: %s, s_sessionID: %d",
             pPdpCid, (int) s_sessionIDQMI);
        goto error;
    }

    /* START MOTO 172 */
    /* When turning off the RADIO (e.g., entering airplane mode) Android will first attempt to gracefully
     * close the data call (with a timeout of 30 seconds if data call disconnect is not received) and then
     * send a request to enter low power mode. The RIL should not switch to low power mode here as it'll
     * result in two seperate requests to enter LPM mode which can interfear with the user attempting to 
     * quickly exit from airplane mode.
     */
#if 0
    /* SWI_TBD only observed RIL_DISCONNECT_REASON_RADIO_SHUTDOWN when device is switching off
     * set to airplane mode for now */
    if (disconnectReason == RIL_DISCONNECT_REASON_RADIO_SHUTDOWN) {
        changePowerModeQMI(QMI_POWER_LOW);
        s_sessionIDQMI = 0;
        setRadioState(RADIO_STATE_OFF);
    }
#endif /* 0 */
    /* STOP MOTO 172 */

    /* clean up data call list when data call deactivated */
    initDataCallResponseList();
    s_rilPDPContextListRes.status = PDP_FAIL_NONE;
    /* stop DHCP client */
    checkDHCPnStopService();
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);

    return;
    
error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}
