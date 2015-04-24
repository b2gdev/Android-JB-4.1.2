/**
 *  Filename: qatestroutines.c
 *
 *  Purpose:  Contains functions implementing specific tests called
 *            by the main() routine in qatest.c
 *
 * Copyright: © 2011-12 Sierra Wireless, Inc., all rights reserved
 *
 */
/* Linux definitions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmerrno.h"
#include "qatesthelper.h"
#include "qatestproto.h"

/* Defines */

#define nMaxStrLen 0xFF

/* current line number in the sequence text file */
unsigned int curLineNo = 0;

/* Forward declaration */
extern BOOL blogtofile;
package void qaDisplayTestXref();
package void doDeprecated();

/* Notification Handler Testing */
extern BYTE inbuf[]; /* receive buffer for notification handler
                      * function
                      */
/* Our table of functions */

struct qatesttab
{
    void (*testroutinep)( void );
    char *estrp;   /* enum string pointer */
    char *fstrp;   /* function name string pointer */
};

struct qatesttab ttabs[] =
{
    {(void *)&qaDisplayTestXref,                     "t",   "Display list of available test functions"},
    { NULL,                                          "t0",  "Stop Execution"},
    { NULL,                                          "\n\t==========================", ""},
    { NULL,                                          "\tWireless Data Service APIs", ""},
    { NULL,                                          "\t==========================", ""},
    {(void *)&doStartDataSession2,                   "t1",  "StartDataSession2"},
    {(void *)&doStopDataSession,                     "t2",  "StopDataSession"},
    {(void *)&doStartDataSessionLTE,                 "t3",  "StartDataSessionLTE"},
    {(void *)&doGetDefaultProfile,                   "t4",  "GetDefaultProfile"},
    {(void *)&doGetDefaultProfileLTE,                "t5",  "GetDefaultProfileLTE"},
    {(void *)&doGetAutoconnect,                      "t6",  "GetAutoconnect"},
    {(void *)&doGetSessionState,                     "t7",  "GetSessionState"},
    {(void *)&doGetPacketStatus,                     "t8",  "GetPacketStatus"},
    {(void *)&doGetByteTotals,                       "t9",  "GetByteTotals"},
    {(void *)&doGetDormancyState,                    "t10", "GetDormancyState"},
    {(void *)&doGetDataBearerTechnology,             "t11", "GetDataBearerTechnology"},
    {(void *)&doGetSessionDuration,                  "t12", "GetSessionDuration"},
    {(void *)&doGetIPAddress,                        "t13", "GetIPAddress"},
    {(void *)&doGetIPAddressLTE,                     "t14", "GetIPAddressLTE"},
    {(void *)&doGetConnectionRate,                   "t15", "GetConnectionRate"},
    {(void *)&doSetAutoconnect,                      "t16", "SetAutoconnect"},
    {(void *)&doSetDefaultProfile,                   "t17", "SetDefaultProfile"},
    {(void *)&doSetDefaultProfileLTE,                "t18", "SetDefaultProfileLTE"},
    {(void *)&doSetMobileIP,                         "t19", "SetMobileIP"},
    {(void *)&doGetMobileIP,                         "t20", "GetMobileIP"},
    {(void *)&doGetMobileIPProfile,                  "t21", "GetMobileIPProfile"},
    {(void *)&doGetLastMobileIPError,                "t22", "GetLastMobileIPError"},
    {(void *)&doSLQSGetRuntimeSettings,              "t23", "SLQSGetRuntimeSettings"},
    {(void *)&doSLQSSetProfile,                      "t24", "SLQSSetProfile"},
    {(void *)&doSLQSGetProfile,                      "t25", "SLQSGetProfile"},
    {(void *)&doSLQSStartStopDataSession_start,      "t26", "SLQSStartStopDataSession_start"},
    {(void *)&doSLQSStartStopDataSession_stop,       "t27", "SLQSStartStopDataSession_stop"},
    {(void *)&doSetMobileIPParameters,               "t28", "SetMobileIPParameters"},
    {(void *)&doSLQSSetIPFamilyPreference_v4,        "t29", "SLQSSetIPFamilyPreferencev4"},
    {(void *)&doSLQSSetIPFamilyPreference_v6,        "t30", "SLQSSetIPFamilyPreferencev6"},
    {(void *)&doSLQSSetIPFamilyPreference_v4v6,      "t31", "SLQSSetIPFamilyPreferencev4v6"},
    {(void *)&doSLQSDeleteProfile,                   "t32", "SLQSDeleteProfile"},
    {(void *)&doSLQSCreateProfile,                   "t33", "SLQSCreateProfile"},
    {(void *)&doSLQSAutoConnect,                     "t34", "SLQSAutoConnect"},
    {(void *)&doSLQSGetDataBearerTechnology,         "t35", "SLQSGetDataBearerTechnology"},
    {(void *)&doSLQSModifyProfile,                   "t36", "SLQSModifyProfile"},
    {(void *)&doSLQSSet3GPPConfigItem,               "t37", "SLQSSet3GPPConfigItem"},
    {(void *)&doSLQSGet3GPPConfigItem,               "t38", "SLQSGet3GPPConfigItem"},
    {(void *)&doSLQSGetProfileSettings,              "t39", "SLQSGetProfileSettings"},
    {(void *)&doSetMobileIPProfile,                  "t40", "SetMobileIPProfile"},
    {(void *)&doSLQSWdsSetEventReport,               "t41", "SLQSWdsSetEventReport"},
    {(void *)&doSLQSWdsSwiPDPRuntimeSettings,        "t42", "SLQSWdsSwiPDPRuntimeSettings"},

    { NULL,                                          "\n\t==============================", ""},
    { NULL,                                          "\tDevice Management Service APIs", ""},
    { NULL,                                          "\t==============================", ""},
    {(void *)&doGetManufacturer,                     "t50", "GetManufacturer"},
    {(void *)&doGetModelID,                          "t51", "GetModelID"},
    {(void *)&doGetFirmwareRevision,                 "t52", "GetFirmwareRevision"},
    {(void *)&doGetFirmwareRevisions,                "t53", "GetFirmwareRevisions"},
    {(void *)&doGetPRLVersion,                       "t54", "GetPRLVersion"},
    {(void *)&doGetIMSI,                             "t55", "GetIMSI"},
    {(void *)&doGetSerialNumbers,                    "t56", "GetSerialNumbers"},
    {(void *)&doGetHardwareRevision,                 "t57", "GetHardwareRevision"},
    {(void *)&doGetNetworkTime,                      "t58", "GetNetworkTime"},
    {(void *)&doUIMSetPINProtection,                 "t59", "UIMSetPINProtection"},
    {(void *)&doUIMVerifyPIN,                        "t60", "UIMVerifyPIN"},
    {(void *)&doUIMUnblockPIN,                       "t61", "UIMUnblockPIN"},
    {(void *)&doUIMChangePIN,                        "t62", "UIMChangePIN"},
    {(void *)&doUIMGetPINStatus,                     "t63", "UIMGetPINStatus"},
    {(void *)&doGetDeviceCapabilities,               "t64", "GetDeviceCapabilities"},
    {(void *)&doGetVoiceNumber,                      "t65", "GetVoiceNumber"},
    {(void *)&doSetPower,                            "t66", "SetPower"},
    {(void *)&doGetPower,                            "t67", "GetPower"},
    {(void *)&doUIMGetControlKeyStatus,              "t68", "UIMGetControlKeyStatus"},
    {(void *)&doUIMSetCkProtection,                  "t69", "UIMSetControlKeyProtection"},
    {(void *)&doUIMUnblockCk,                        "t70", "UIMUnblockControlKey"},
    {(void *)&doUIMGetICCID,                         "t71", "UIMGetICCID"},
    {(void *)&doGetOfflineReason,                    "t72", "GetOfflineReason"},
    {(void *)&doResetToFactoryDefaults,              "t73", "ResetToFactoryDefaults"},
    {(void *)&doValidateSPC,                         "t74", "ValidateSPC"},
    {(void *)&doActivateAutomatic,                   "t75", "ActivateAutomatic"},
    {(void *)&doGetActivationState,                  "t76", "GetActivationState"},
    {(void *)&doUIMGetState,                         "t77", "SLQSUIMGetState"},
    {(void *)&doSLQSgetBandCapability,               "t78", "SLQSGetBandCapability"},
    {(void *)&doSLQSGetCustFeatures,                 "t79", "SLQSGetCustFeatures"},
    {(void *)&doSLQSSetCustFeatures,                 "t80", "SLQSSetCustFeatures"},
    {(void *)&doSLQGetCurrentPRLInfo,                "t81", "SLQSGetCurrentPRLInfo"},

    { NULL,                                          "\n\t=========================", ""},
    { NULL,                                          "\tCallback Management APIs", ""},
    { NULL,                                          "\t=========================", ""},
    {(void *)&doSetSessionStateCallback,             "t90", "SetSessionStateCallback"},
    {(void *)&doClearSessionStateCallback,           "t91", "ClearSessionStateCallback"},
    {(void *)&doSetDataBearerCallback,               "t92", "SetDataBearerCallback"},
    {(void *)&doClearDataBearerCallback,             "t93", "ClearDataBearerCallback"},
    {(void *)&doSetDormancyStatusCallback,           "t94", "SetDormancyStatusCallback"},
    {(void *)&doClearDormancyStatusCallback,         "t95", "ClearDormancyStatusCallback"},
    {(void *)&doSetPowerCallback,                    "t96", "SetPowerCallback"},
    {(void *)&doClearPowerCallback,                  "t97", "ClearPowerCallback"},
    {(void *)&doSetByteTotalsCallback,               "t98", "SetByteTotalsCallback"},
    {(void *)&doClearByteTotalsCallback,             "t99", "ClearByteTotalsCallback"},
    {(void *)&doSetActivationStatusCallback,         "t100", "SetActivationStatusCallback"},
    {(void *)&doClearActivationStatusCallback,       "t101", "ClearActivationStatusCallback"},
    {(void *)&doSetMobileIPStatusCallback,           "t102", "SetMobileIPStatusCallback"},
    {(void *)&doClearMobileIPStatusCallback,         "t103", "ClearMobileIPStatusCallback"},
    {(void *)&doSetRoamingIndicatorCallback,         "t104", "SetRoamingIndicatorCallback"},
    {(void *)&doClearRoamingIndicatorCallback,       "t105", "ClearRoamingIndicatorCallback"},
    {(void *)&doSetDataCapabilitiesCallback,         "t106", "SetDataCapabilitiesCallback"},
    {(void *)&doClearDataCapabilitiesCallback,       "t107", "ClearDataCapabilitiesCallback"},
    {(void *)&doSetRFInfoCallback,                   "t108", "SetRFInfoCallback"},
    {(void *)&doClearRFInfoCallback,                 "t109", "ClearRFInfoCallback"},
    {(void *)&doSetSignalStrengthCallback,           "t110", "SetSignalStrengthCallback"},
    {(void *)&doClearSignalStrengthCallback,         "t111", "ClearSignalStrengthCallback"},
    {(void *)&doSetLURejectCallback,                 "t112", "SetLURejectCallback"},
    {(void *)&doClearLURejectCallback,               "t113", "ClearLURejectCallback"},
    {(void *)&doSetNMEACallback,                     "t114", "SetNMEACallback"},
    {(void *)&doClearNMEACallback,                   "t115", "ClearNMEACallback"},
    {(void *)&doSetPdsStateCallback,                 "t116", "SetPdsStateCallback"},
    {(void *)&doClearPdsStateCallback,               "t117", "ClearPdsStateCallback"},
    {(void *)&doSetNewSMSCallback,                   "t118", "SetNewSMSCallback"},
    {(void *)&doClearNewSMSCallback,                 "t119", "ClearNewSMSCallback"},
    {(void *)&doSetCATEventCallback,                 "t120", "SetCatEventCallback"},
    {(void *)&doClearCATEventCallback,               "t121", "ClearCatEventCallback"},
    {(void *)&doSetSLQSOMADMAlertCallback,           "t122", "SetSLQSOMADMAlertCallback"},
    {(void *)&doClearSLQSOMADMAlertCallback,         "t123", "ClearSLQSOMADMAlertCallback"},
    {(void *)&doSetDeviceStateChangeCallback,        "t124", "SetDeviceStateChangeCallback"},
    {(void *)&doClearDeviceStateChangeCallback,      "t125", "ClearDeviceStateChangeCallback"},
    {(void *)&doSetFwDldCompletionCallback,          "t126", "SetFwDldCompletionCallback"},
    {(void *)&doClearFwDldCompletionCallback,        "t127", "ClearFwDldCompletionCallback"},
    {(void *)&doSetSLQSServingSystemCallback,        "t128", "SetSLQSServingSystemCallback"},
    {(void *)&doClearSLQSServingSystemCallback,      "t129", "ClearSLQSServingSystemCallback"},
    {(void *)&doSetOMADMStateCallback,               "t130", "SetOMADMStateCallback"},
    {(void *)&doClearOMADMStateCallback,             "t131", "ClearOMADMStateCallback"},
    {(void *)&doSetSLQSSetBandPrefCallBack,          "t132", "SetSLQSSetBandPrefCallBack"},
    {(void *)&doClearSLQSSetBandPrefCallback,        "t133", "ClearSLQSSetBandPrefCallback"},
    {(void *)&doSetUSSDNotificationCallback,         "t134", "SetUSSDNotificationCallback"},
    {(void *)&doClearUSSDNotificationCallback,       "t135", "ClearUSSDNotificationCallback "},
    {(void *)&doSetUSSDReleaseCallback,              "t136", "SetUSSDReleaseCallback"},
    {(void *)&doClearSetUSSDReleaseCallback,         "t137", "ClearSetUSSDReleaseCallback"},
    {(void *)&doSLQSSetSignalStrengthsCallback,      "t138", "SLQSSetSignalStrengthsCallback"},
    {(void *)&doClearSLQSSetSignalStrengthsCallback, "t139", "ClearSLQSSetSignalStrengthsCallback"},
    {(void *)&doSLQSSetSMSEventCallback,             "t140", "SLQSSetSMSEventCallback"},
    {(void *)&doClearSLQSSetSMSEventCallback,        "t141", "ClearSLQSSetSMSEventCallback"},
    {(void *)&doSLQSVoiceSetSUPSNotificationCallback,"t142", "SLQSVoiceSetSUPSNotificationCallback"},
    {(void *)&doClearSLQSVoiceSetSUPSNotificationCallback,"t143", "ClearSLQSVoiceSetSUPSNotificationCallback"},
    {(void *)&doSetSLQSSDKTerminatedCallback,        "t144", "SetSLQSSDKTerminatedCallback"},
    {(void *)&doClearSLQSSDKTerminatedCallback,      "t145", "ClearSLQSSDKTerminatedCallback"},
    {(void *)&doSetSLQSVoiceSetAllCallStatusCallback, "t146", "SetSLQSVoiceSetAllCallStatusCallback"},
    {(void *)&doClearSLQSVoiceSetAllCallStatusCallback,"t147","ClearSLQSVoiceSetAllCallStatusCallback"},
    {(void *)&doSLQSSetTransLayerInfoCallback,       "t148", "SLQSSetTransLayerInfoCallback"},
    {(void *)&doClearSLQSSetTransLayerInfoCallback,  "t149", "ClearSLQSSetTransLayerInfoCallback"},
    {(void *)&doSLQSSetTransNWRegInfoCallback,       "t150", "SLQSSetTransNWRegInfoCallback"},
    {(void *)&doClearSLQSSetTransNWRegInfoCallback,  "t151", "ClearSLQSSetTransNWRegInfoCallback"},
    {(void *)&doSLQSSetSysSelectionPrefCallBack,     "t152", "SLQSSetSysSelectionPrefCallBack"},
    {(void *)&doClearSLQSSetSysSelectionPrefCallBack,"t153", "ClearSLQSSetSysSelectionPrefCallBack"},
    {(void *)&doSLQSUIMSetStatusChangeCallBack,      "t154", "SLQSUIMSetStatusChangeCallBack"},
    {(void *)&doClearSLQSUIMSetStatusChangeCallBack, "t155", "ClearSLQSUIMSetStatusChangeCallBack"},
    {(void *)&doSLQSUIMSetRefreshCallBack,           "t156", "SLQSUIMSetRefreshCallBack"},
    {(void *)&doClearSLQSUIMSetRefreshCallBack,      "t157", "ClearSLQSUIMSetRefreshCallBack"},
    {(void *)&doSetSLQSVoiceSetPrivacyChangeCallback,  "t158","SetSLQSVoiceSetPrivacyChangeCallback"},
    {(void *)&doClearSLQSVoiceSetPrivacyChangeCallback,"t159","ClearSLQSVoiceSetPrivacyChangeCallback"},
    {(void *)&doSetSLQSVoiceSetDTMFEventCallback,    "t160", "SetSLQSVoiceSetDTMFEventCallback"},
    {(void *)&doClearSLQSVoiceSetDTMFEventCallback,  "t161", "ClearSLQSVoiceSetDTMFEventCallback"},
    {(void *)&doSetSLQSVoiceSetSUPSCallback,         "t162", "SetSLQSVoiceSetSUPSCallback"},
    {(void *)&doClearSLQSVoiceSetSUPSCallback,       "t163", "ClearSLQSVoiceSetSUPSCallback"},
    {(void *)&doSetSLQSNasSysInfoCallBack,           "t164", "SetSLQSNasSysInfoCallBack"},
    {(void *)&doClearSLQSNasSysInfoCallBack,         "t165", "ClearSLQSNasSysInfoCallBack"},
    {(void *)&doSetSLQSNasNetworkTimeCallBack,       "t166", "SetSLQSNasNetworkTimeCallBack"},
    {(void *)&doClearSLQSNasNetworkTimeCallBack,     "t167", "ClearSLQSNasNetworkTimeCallBack"},
    {(void *)&doSetSLQSWmsMemoryFullCallBack,        "t168", "SetSLQSWmsMemoryFullCallBack"},
    {(void *)&doClearSLQSWmsMemoryFullCallBack,      "t169", "ClearSLQSWmsMemoryFullCallBack"},
    {(void *)&doSLQSVoiceSetOTASPStatusCallback,     "t170", "SLQSVoiceSetOTASPStatusCallback"},
    {(void *)&doClearSLQSVoiceSetOTASPStatusCallback,"t171", "ClearSLQSVoiceSetOTASPStatusCallback"},
    {(void *)&doSetSLQSVoiceInfoRecCallback,         "t172", "SetSLQSVoiceInfoRecCallback"},
    {(void *)&doClearSLQSVoiceInfoRecCallback,       "t173", "ClearSLQSVoiceInfoRecCallback"},

    { NULL,                                          "\n\t=========================", ""},
    { NULL,                                          "\tDevice Connectivity APIs", ""},
    { NULL,                                          "\t=========================", ""},
    {(void *)&doDCSGetConnecteDevID,                 "t200", "DCSGetConnectedDevID"},
    {(void *)&doSLQSGetUsbPortNames,                 "t201", "SLQSGetUsbPortNames"},
    {(void *)&doSLQSGetDeviceMode,                   "t202", "SLQSGetDeviceMode"},

    { NULL,                                          "\n\t=========================", ""},
    { NULL,                                          "\tPosition Determination Service APIs", ""},
    { NULL,                                          "\t=========================", ""},
    {(void *)&doGetPDSState,                         "t210", "GetPDSState"},
    {(void *)&doSetPDSState,                         "t211", "SetPDSState"},
    {(void *)&doStartPDSTrackingSessionExt,          "t212", "StartPDSTrackingSessionExt"},
    {(void *)&doStopPDSTrackingSession,              "t213", "StopPDSTrackingSession"},
    {(void *)&doGetPDSDefaults,                      "t215", "GetPDSDefaults"},
    {(void *)&doSetPDSDefaults,                      "t216", "SetPDSDefaults"},
    {(void *)&doGetXTRAAutomaticDownload,            "t217", "GetXTRAAutomaticDownload"},
    {(void *)&doSetXTRAAutomaticDownload,            "t218", "SetXTRAAutomaticDownload"},
    {(void *)&doGetXTRANetwork,                      "t219", "GetXTRANetwork"},
    {(void *)&doSetXTRANetwork,                      "t220", "SetXTRANetwork"},
    {(void *)&doGetXTRAValidity,                     "t221", "GetXTRAValidity"},
    {(void *)&doForceXTRADownload,                   "t222", "ForceXTRADownload"},
    {(void *)&doGetServiceAutomaticTracking,         "t223", "GetServiceAutomaticTracking"},
    {(void *)&doSetServiceAutomaticTracking,         "t224", "SetServiceAutomaticTracking"},
    {(void *)&doGetPortAutomaticTracking,            "t225", "GetPortAutomaticTracking"},
    {(void *)&doSetPortAutomaticTracking,            "t226", "SetPortAutomaticTracking"},
    {(void *)&doResetPDSData,                        "t227", "ResetPDSData"},
    {(void *)&doPDSInjectTimeReference,              "t228", "PDSInjectTimeReference"},
    {(void *)&doSLQSSetAGPSConfig,                   "t229", "SLQSSetAGPSConfig"},
    {(void *)&doSLQSPDSInjectAbsoluteTimeReference,  "t230", "SLQSPDSInjectAbsoluteTimeReference"},
    {(void *)&doSLQSGetAGPSConfig,                   "t231", "SLQSGetAGPSConfig"},
    {(void *)&doSLQSPDSInjectPositionData,           "t232", "SLQSPDSInjectPositionData"},

    { NULL,                                          "\n\t=============================", ""},
    { NULL,                                          "\tCard Application Toolkit APIs", ""},
    { NULL,                                          "\t=============================", ""},
    {(void *)&doCATSendEnvelopeCommand,              "t250", "CATSendEnvelopeCommand"},
    {(void *)&doCATSendTerminalResponse,             "t251", "CATSendTerminalResponse"},

    { NULL,                                          "\n\t=========================", ""},
    { NULL,                                          "\t SMS APIs", ""},
    { NULL,                                          "\t=========================", ""},
    {(void *)&doSaveSMS,                             "t300", "SaveSMS"},
    {(void *)&doSendSMS,                             "t301", "SendSMS"},
    {(void *)&doSetSMSCAddress,                      "t302", "SetSMSCAddress"},
    {(void *)&doGetSMSCAddress,                      "t303", "GetSMSCAddress"},
    {(void *)&doSLQSDeleteSMS,                       "t304", "SLQSDeleteSMS"},
    {(void *)&doSLQSGetSMS,                          "t305", "SLQSGetSMS"},
    {(void *)&doSLQSModifySMSStatus,                 "t306", "SLQSModifySMSStatus"},
    {(void *)&doSLQSGetSMSList,                      "t307", "SLQSGetSMSList"},
    {(void *)&doSLQSGetSMSBroadcastConfig,           "t308", "SLQSGetSMSBroadcastConfig"},
    {(void *)&doSLQSSetSmsBroadcastConfig,           "t309", "SLQSSetSmsBroadcastConfig"},
    {(void *)&doSetSmsBroadcastActivation,           "t310", "SetSmsSLQSBroadcastActivation"},
    {(void *)&doSLQSCDMAEncodeMOTextMsg,             "t311", "SLQSCDMAEncodeMOTextMsg"},
    {(void *)&doSLQSCDMADecodeMTTextMsg,             "t312", "SLQSCDMADecodeMTTextMsg"},
    {(void *)&doSLQSWCDMAEncodeMOTextMsg,            "t313", "SLQSWCDMAEncodeMOTextMsg"},
    {(void *)&doSLQSWCDMADecodeMTTextMsg,            "t314", "SLQSWCDMADecodeMTTextMsg"},
    {(void *)&doSLQSSendSMS,                         "t315", "SLQSSendSMS"},
    {(void *)&doSLQSGetTransLayerInfo,               "t316", "SLQSGetTransLayerInfo"},
    {(void *)&doSLQSGetTransNWRegInfo,               "t317", "SLQSGetTransNWRegInfo"},
    {(void *)&doSLQSGetIndicationRegister,           "t318", "SLQSGetIndicationRegister"},
    {(void *)&doSLQSSetIndicationRegister,           "t319", "SLQSSetIndicationRegister"},
    {(void *)&doSLQSSmsSetRoutes,                    "t320", "SLQSSmsSetRoutes"},
    {(void *)&doSLQSSmsGetMessageProtocol,           "t321", "SLQSSmsGetMessageProtocol"},
    {(void *)&doSLQSSmsGetMaxStorageSize,            "t322", "SLQSSmsGetMaxStorageSize"},

    { NULL,                                          "\n\t===========================", ""},
    { NULL,                                          "\tNetwork Access Service APIs", ""},
    { NULL,                                          "\t===========================", ""},
    {(void *)&doGetHomeNetwork,                      "t350", "GetHomeNetwork"},
    {(void *)&doGetServingSystem,                    "t351", "GetServingNetwork"},
    {(void *)&doGetSignalStrengths,                  "t352", "GetSignalStrengths"},
    {(void *)&doInitNetworkReg,                      "t353", "InitiateNetworkRegistration"},
    {(void *)&doPerformNetworkScan,                  "t354", "PerformNetworkScan"},
    {(void *)&doGetServingNetworkCaps,               "t355", "GetServingNetworkCapabilities"},
    {(void *)&doSetNetworkPreference,                "t356", "SetNetworkPreference"},
    {(void *)&doGetNetworkPreference,                "t357", "GetNetworkPreference"},
    {(void *)&doGetRFInfo,                           "t358", "GetRFInfo"},
    {(void *)&doInitiateDomainAttach,                "t359", "InitiateDomainAttach"},
    {(void *)&doGetACCOLC,                           "t360", "GetACCOLC"},
    {(void *)&doSetACCOLC,                           "t361", "SetACCOLC"},
    {(void *)&doSetCDMANetworkParameters,            "t362", "SetCDMANetworkParameters"},
    {(void *)&doGetCDMANetworkParameters,            "t363", "GetCDMANetworkParameters"},
    {(void *)&doGetANAAAAuthStatus,                  "t364", "GetANAAAAuthenticationStatus"},
    {(void *)&doSLQSGetServingSystem,                "t365", "SLQSGetServingSystem"},
    {(void *)&doSLQSSetBandPreference,               "t366", "SLQSSetBandPreference"},
    {(void *)&doSLQSNasIndicationRegister,           "t367", "SLQSNasIndicationRegister"},
    {(void *)&doSLQSGetSignalStrength,               "t368", "SLQSGetSignalStrength"},
    {(void *)&doSLQSPerformNetworkScan,              "t369", "SLQSPerformNetworkScan"},
    {(void *)&doSLQSSetSysSelectionPref,             "t370", "SLQSSetSysSelectionPref"},
    {(void *)&doSLQSGetSysSelectionPref,             "t371", "SLQSGetSysSelectionPref"},
    {(void *)&doSLQSNasGetSysInfo,                   "t372", "SLQSNasGetSysInfo"},
    {(void *)&doSLQSNasSwiModemStatus,               "t373", "SLQSNasSwiModemStatus"},
    {(void *)&doSLQSNasGetHDRColorCode,              "t374", "SLQSNasGetHDRColorCode"},
    {(void *)&doSLQSNasGetTxRxInfo,                  "t375", "SLQSNasGetTxRxInfo"},
    {(void *)&doSLQSNasGetSigInfo,                   "t376", "SLQSNasGetSigInfo"},
    {(void *)&doSLQSNasIndicationRegisterExt,        "t377", "SLQSNasIndicationRegisterExt"},
    {(void *)&doSLQSGetPLMNName,                     "t378", "SLQSGetPLMNName"},
    {(void *)&doSLQSGetOperatorNameData,             "t379", "SLQSGetOperatorNameData"},
    {(void *)&doSLQSGet3GPP2Subscription,            "t380", "SLQSGet3GPP2Subscription"},
    {(void *)&doSLQSNasGetCellLocationInfo,          "t381", "SLQSNasGetCellLocationInfo"},
    {(void *)&doSLQSInitNetworkReg,                  "t382", "SLQSInitiateNetworkRegistration"},

    { NULL,                                          "\n\t=========================", ""},
    { NULL,                                          "\t SWIOMADMS APIs", ""},
    { NULL,                                          "\t=========================", ""},
    {(void *)&doSLQSOMADMStartSession,               "t400", "SLQSOMADMStartSession"},
    {(void *)&doSLQSOMADMCancelSession,              "t401", "SLQSOMADMCancelSession"},
    {(void *)&doSLQSOMADMGetSessionInfo,             "t402", "SLQSOMADMGetSessionInfo"},
    {(void *)&doSLQSOMADMSendSelection,              "t403", "SLQSOMADMSendSelection"},
    {(void *)&doSLQSOMADMGetSettings,                "t404", "SLQSOMADMGetSettings"},
    {(void *)&doSLQSOMADMSetSettings,                "t405", "SLQSOMADMSetSettings"},
    {(void *)&doSLQSOMADMSetSettings2,               "t406", "SLQSOMADMSetSettings2"},
    {(void *)&doSLQSOMADMGetSettings2,               "t407", "SLQSOMADMGetSettings2"},

    { NULL,                                          "\n\t====================", ""},
    { NULL,                                          "\t RMS APIs", ""},
    { NULL,                                          "\t=======================", ""},
    {(void *)&doGetSMSWake,                          "t500", "GetSMSWake"},
    {(void *)&doSetSMSWake,                          "t501", "SetSMSWake"},

    { NULL,                                          "\n\t=========================", ""},
    { NULL,                                          "\t FMS APIs", ""},
    { NULL,                                          "\t=========================", ""},
    {(void *)&doUpgradeFirmware2k,                   "t600", "UpgradeFirmware2k"},
    {(void *)&doSLQSGetFirmwareInfo,                 "t601", "SLQSGetFirmwareInfo"},
    {(void *)&doSLQSGetImageInfo,                    "t602", "SLQSGetImageInfo"},
    {(void *)&doDeprecated,                          "t603", "GetFirmwareInfo"},
    {(void *)&doDeprecated,                          "t604", "GetImageInfo "},
    {(void *)&doGetImagesPreference,                 "t605", "GetImagesPreference"},
    {(void *)&doSetImagesPreference,                 "t606", "SetImagesPreference"},
    {(void *)&doGetStoredImages,                     "t607", "GetStoredImages"},
    {(void *)&doSLQSUpgradeGobiFw,                   "t608", "SLQSUpgradeGobiFw"},
    {(void *)&doDeleteStoredImage,                   "t609", "DeleteStoredImage"},

    { NULL,                                          "\n\t====================", ""},
    { NULL,                                          "\t OMA DM APIs", ""},
    { NULL,                                          "\t=======================", ""},
    {(void *)&doOMADMSStartSession,                  "t700", "OMADMStartSession"},
    {(void *)&doOMADMSCancelSession,                 "t701", "OMADMCancelSession"},
    {(void *)&doOMADMSGetSessionInfo,                "t702", "OMADMGetSessionInfo"},
    {(void *)&doOMADMSGetPendingNIA,                 "t705", "OMADMGetPendingNIA"},

    { NULL,                                          "\n\t====================", ""},
    { NULL,                                          "\t SAR APIs", ""},
    { NULL,                                          "\t=======================", ""},
    {(void *)&doSLQSSetRfSarState,                   "t800","SLQSSetRFSarState"},
    {(void *)&doSLQSGetRfSarState,                   "t801","SLQSGetRfSarState"},

    { NULL,                                          "\n\t====================", ""},
    { NULL,                                          "\t UIM APIs", ""},
    { NULL,                                          "\t=======================", ""},
    {(void *)&doSLQSUIMReset,                        "t850","SLQSUIMReset"},
    {(void *)&doSLQSUIMPowerDown,                    "t851","SLQSUIMPowerDown"},
    {(void *)&doSLQSUIMGetCardStatus,                "t852","SLQSUIMGetCardStatus"},
    {(void *)&doSLQSUIMSetPinProtection,             "t853", "SLQSUIMSetPinProtection"},
    {(void *)&doSLQSUIMVerifyPin,                    "t854", "SLQSUIMVerifyPin"},
    {(void *)&doSLQSUIMChangePin,                    "t855", "SLQSUIMChangePin"},
    {(void *)&doSLQSUIMUnblockPin,                   "t856", "SLQSUIMUnblockPin"},
    {(void *)&doSLQSUIMRefreshOK,                    "t857","SLQSUIMRefreshOK"},
    {(void *)&doSLQSUIMRefreshRegister,              "t858","SLQSUIMRefreshRegister"},
    {(void *)&doSLQSUIMEventRegister,                "t859","SLQSUIMEventRegister"},
    {(void *)&doSLQSUIMRefreshGetLastEvent,          "t860","SLQSUIMRefreshGetLastEvent"},
    {(void *)&doSLQSUIMRefreshComplete,              "t861","doSLQSUIMRefreshComplete"},
    {(void *)&doSLQSUIMGetFileAttributes,            "t862","SLQSUIMGetFileAttributes"},
    {(void *)&doSLQSUIMDepersonalization,            "t863","SLQSUIMDepersonalization"},
    {(void *)&doSLQSUIMAuthenticate,                 "t864","SLQSUIMAuthenticate"},

    { NULL,                                          "\n\t====================", ""},
    { NULL,                                          "\t VOICE APIs", ""},
    { NULL,                                          "\t=======================", ""},
    {(void *)&doOriginateUSSD,                       "t900", "OriginateUSSD"},
    {(void *)&doAnswerUSSD,                          "t901", "AnswerUSSD"},
    {(void *)&doCancelUSSD,                          "t902", "CancelUSSD"},
    {(void *)&doSLQSVoiceDialCall,                   "t903", "SLQSVoiceDialCall"},
    {(void *)&doSLQSVoiceEndCall,                    "t904", "SLQSVoiceEndCall"},
    {(void *)&doFunctionalDialEndCall,               "t905", "VoiceFunctionalDialEndCall"},
    {(void *)&doSLQSVoiceSetSUPSService,             "t906", "SLQSVoiceSetSUPSService"},
    {(void *)&doSLQSVoiceSetConfig,                  "t907", "SLQSVoiceSetConfig"},
    {(void *)&doSLQSAnswerCall,                      "t908", "SLQSAnswerCall"},
    {(void *)&doSLQSVoiceGetCLIR,                    "t909", "SLQSVoiceGetCLIR"},
    {(void *)&doSLQSVoiceGetCLIP,                    "t910", "SLQSVoiceGetCLIP"},
    {(void *)&doSLQSVoiceGetCallWaiting,             "t911", "SLQSVoiceGetCallWaiting"},
    {(void *)&doSLQSVoiceGetCallBarring,             "t912", "SLQSVoiceGetCallBarring"},
    {(void *)&doSLQSVoiceGetCallForwardingStatus,    "t913", "SLQSVoiceGetCallForwardingStatus"},
    {(void *)&doSLQSVoiceSetCallBarringPassword,     "t914", "SLQSVoiceSetCallBarringPassword"},
    {(void *)&doSLQSVoiceGetCallInfo,                "t915", "SLQSVoiceGetCallInfo"},
    {(void *)&doSLQSVoiceGetAllCallInfo,             "t916", "SLQSVoiceGetAllCallInfo"},
    {(void *)&doSLQSVoiceManageCalls,                "t917", "SLQSVoiceManageCalls"},
    {(void *)&doSLQSVoiceBurstDTMF,                  "t918", "SLQSVoiceBurstDTMF"},
    {(void *)&doSLQSVoiceStartContDTMF,              "t919", "SLQSVoiceStartContDTMF"},
    {(void *)&doSLQSVoiceStopContDTMF,               "t920", "SLQSVoiceStopContDTMF"},
    {(void *)&doSLQSVoiceSendFlash,                  "t921", "SLQSVoiceSendFlash"},
    {(void *)&doSLQSVoiceSetPreferredPrivacy,        "t922", "SLQSVoiceSetPreferredPrivacy"},
    {(void *)&doSLQSVoiceIndicationRegister,         "t923", "SLQSVoiceIndicationRegister"},
    {(void *)&doSLQSVoiceGetConfig,                  "t924", "SLQSVoiceGetConfig"},
    {(void *)&doSLQSVoiceOrigUSSDNoWait,             "t925", "SLQSVoiceOrigUSSDNoWait"},
    {(void *)&doSLQSVoiceBindSubscription,           "t926", "SLQSVoiceBindSubscription"},
    {(void *)&doSLQSVoiceALSSetLineSwitching,        "t927", "SLQSVoiceALSSetLineSwitching"},
    {(void *)&doSLQSVoiceALSSelectLine,              "t928", "SLQSVoiceALSSelectLine"},
    {(void *)&doSLQSVoiceGetCOLP,                    "t929", "SLQSVoiceGetCOLP"},
    {(void *)&doSLQSVoiceGetCOLR,                    "t930", "SLQSVoiceGetCOLR"},
    {(void *)&doSLQSVoiceGetCNAP,                    "t931", "SLQSVoiceGetCNAP"},

    { NULL,                                          "\n\t=========================", ""},
    { NULL,                                          "\tUnimplemented APIs", ""},
    { NULL,                                          "\t=========================", ""},
    {(void *)&doExecuteSmsStubs,                     "t1000", "SmsStubs"},
    {(void *)&doExecuteFmsStubs,                     "t1001", "FmsStubs"},

    { NULL,                                          "\n\t=========================", ""},
    { NULL,                                          "\tSWI APIs", ""},
    { NULL,                                          "\t=========================", ""},
    {(void *)&doSLQSKillSDKProcess,                  "t1100", "SLQSKillSDKProcess"},

};

#define TMAX (sizeof(ttabs)/sizeof(ttabs[0]))

/* Helper functions for testing on ARM platform */
/*
 * Name:     tfopen
 *
 * Purpose:  This function is a wrapper for fopen.
 *
 * Parms:    fnamep - pointer to file (with path if necessary) to be opened
 *           modep  - mode in which to open file
 *
 * Return:   fp     - pointer  to FILE
 *           stderr - FILE is stderr
 *
 * Notes:    For regular testing results are printed to a file. On the ARM
 *           platform, we are using a read-only filesystem and cannot
 *           create results file and hence use the stderr to output our results.
 *
 */
global FILE *tfopen (char *fnamep, char *modep)
{
#ifdef ARM
    UNUSEDPARAM ( fnamep );
    UNUSEDPARAM ( modep );
    return (stderr);
#else
    if (blogtofile)
    {
        return (fopen(fnamep, modep));
    }
    else
    {
        return stderr;
    }
#endif
}

/*
 * Name:     tfclose
 *
 * Purpose:  This function is a wrapper for fclose.
 *
 * Parms:    fp     - pointer  to FILE
 *
 * Return:   None
 *
 * Notes:    On the ARM platform, stderr is used. We do not want to close that.
 *
 */
global void tfclose(FILE *fp)
{
#ifndef ARM
    /* do not close if logging to stderr */
    if (blogtofile)
    {
        if (fp)
            fclose (fp);
    }
#endif
    UNUSEDPARAM ( fp );
}

#define debug 1
// DBG macro
#define DBG( format, arg... ) \
   if (debug == 1)\
   { \
     syslog( LOG_INFO, format, ## arg ); \
   } \


/*
 * Name:     doprintreason
 *
 * Purpose:  This is a wrapper for printing user-friendly error descriptions.
 *
 * Parms:    fp     - pointer  to FILE
 *           nRet   - Return code from SDK/modem
 *
 * Return:   None
 *
 * Notes:    None.
 *
 */
global void doprintreason(FILE *fp, ULONG nRet)
{
    fprintf(fp, "Return Code: %lu (0x%lX)\n", nRet, nRet);

    if ( nRet < eQCWWAN_ERR_ENUM_END )
    {
        fprintf(fp, "Return Reason: %s\n", codemap[nRet+1].desc);
    }
    else if( (nRet > eQCWWAN_ERR_QMI_OFFSET) &&
              (nRet < eQCWWAN_ERR_QMI_MAX) )
    {
         fprintf(fp,
                 "Return Reason: %s\n",
                 qmicodemap[nRet-eQCWWAN_ERR_QMI_OFFSET-1].desc);
    }
    else if( (nRet > eQCWWAN_ERR_SWICM_START) &&
              (nRet < eQCWWAN_ERR_SWICM_END) )
    {
         fprintf(fp,
                 "Return Reason: %s\n",
                 swicmcodemap[nRet-eQCWWAN_ERR_SWICM_START].desc);
    }
    else if( (nRet > eQCWWAN_ERR_SWIIM_START) &&
              (nRet < eQCWWAN_ERR_SWIIM_END) )
    {
         fprintf(fp,
                 "Return Reason: %s\n",
                 swiimcodemap[nRet-eQCWWAN_ERR_SWIIM_START].desc);
    }
    else if( (nRet >eQCWWAN_ERR_SWISMS_START) &&
              (nRet < eQCWWAN_ERR_SWISM_END) )
    {
         fprintf(fp,
                 "Return Reason: %s\n",
                 swismscodemap[nRet-eQCWWAN_ERR_SWISMS_START].desc);
    }
    else if( (nRet >= eQCWWAN_ERR_QMI_CAT_START) &&
              (nRet <= eQCWWAN_ERR_QMI_CAT_END) )
    {
         fprintf(fp,
                 "Return Reason: %s\n",
                 qmicatcodemap[nRet-eQCWWAN_ERR_QMI_CAT_START].desc);
    }
    /*
     * for some reason, arm compiler thinks fp is unused.
     */
    UNUSEDPARAM ( fp );
}

/**
 * Name:     doDeprecated
 *
 * Purpose:  This function is used when tests need to be deprecated. It
 *           informs the user via printf() that the test no longer exists
 *           and returns. Any test that needs to be obsoleted can use this
 *           function
 *
 * Parms:    timeout - ignored
 *
 * Return:   none
 *
 * Notes:    None
 *
 */
package void doDeprecated( void )
{
    printf("\nTest deprecated\n" );
}

/*************
 * Functions below here are control routines for calling the functions in the
 * table  above.
 ************/

/**************
 *
 * Name:     qaErrorInScript
 *
 * Purpose:  Print the error in the test sequence from the file
 *
 * Parms:    locElement    - incorrect element which is present in the sequence
 *           locIndex[OUT] - location of the element, which is incremented after
 *                           exection of this function
 * Return:   none
 *
 **************/
void qaErrorInScript( char locElement, unsigned long *locIndex )
{
    (*locIndex)++;

    if ( (locElement == '\n') ||
         (locElement == '\r') ||
         (locElement == '\0' ) )
    {
        return;
    }

    printf( "\n########################################################\n" );
    printf( "Error in Script: Char \'%c\' present in : Line %d :: Index %lu\n",
            locElement,
            curLineNo,
            *(locIndex)+1 );
    printf( "########################################################\n\n" );
}

/* reverse the string */
static void qaReverse(char string[])
{
    int i, j;
    char character;

    for (i = 0, j = strlen(string)-1; i<j; i++, j--)
    {
     character = string[i];
     string[i] = string[j];
     string[j] = character;
    }
}

/* convert the integer to ascii */
static int qaItoa(int number, char string[])
{
    int i = 0;

    /* generate digits in reverse order */
    do
    {
     string[i++] = number % 10 + '0'; /* get next digit */
    } while ((number /= 10) > 0);     /* delete it */

    string[i] = '\0';
    qaReverse(string);
    return i;
}


/**************
 *
 * Name:     qaRunSelectedTest
 *
 * Purpose:  Runs a selected test, given the test tag
 *
 * Parms:    testtagp  - pointer to test tag
 *           timeout   - timeout value for stop and wait functions as
 *                       passed in on the command line or defaulted.
 * Return:   none
 *
 * Notes:
 *
 **************/
package void qaRunSelectedTest(char * testtagp, unsigned int timeout)
{
    unsigned int i;
    int slen, tslen;

    UNUSEDPARAM( timeout );

    /* Loop through our table and call the API function that matches */
    /* the test tag                                                  */
    for (i = 0; i < TMAX; i++)
    {
        tslen = slstrlen (testtagp);
        slen = slstrncmp(ttabs[i].estrp, testtagp, tslen );
        if (slen == 0)
        {
            /* Invoke the selected API function */
            printf ("Invoking test: %s\n", ttabs[i].fstrp);
            DBG("%s - START", ttabs[i].fstrp);
            (*(ttabs[i].testroutinep))();
            DBG("%s - END", ttabs[i].fstrp);
            break;
        }
    }
}

/**************
 *
 * Name:     qaMakeTestSequence
 *
 * Purpose:  Make the complete test sequence for the case "tn-tm;"
 *
 * Parms:    pTestCase[OUT] - buffer to fill the generated test cases
 *           startTestCase  - initial testcase number
 *           endTestCase    - final testcase number
 *           timeout        - timeout for executing the test case
 *
 * Return:   none
 *
 **************/
void qaMakeTestSequence(
    char         *pTestCase,
    unsigned int startTestCase,
    unsigned int endTestCase )
{
    unsigned int count = 0;
    memset( pTestCase, 0, MAX_CHAR_IN_A_LINE );

    /* generate the sequence of test cases */
    while( startTestCase <= endTestCase )
    {
        char buffer[100];
        int length;
        if( count >=  MAX_CHAR_IN_A_LINE )
            break;
        length = qaItoa(startTestCase, buffer);
        memcpy( pTestCase+count, "t", 1 );
        count++;
        memcpy( pTestCase+count, buffer, length );
        count += length;
        pTestCase[count++] = ',';
        startTestCase++;
    }
    pTestCase[count-1] = '\0';
}

/**************
 *
 * Name:     qaTreatSpecialSequence
 *
 * Purpose:  Make the complete test sequence for the case "tn<delay b>-tm;"
 *
 * Parms:    delay           - delay in execution between testcases
 *           InitialTestCase - initial testcase number
 *           FinalTestCase   - timeout for executing the test case
 *           timeout         - timeout for executing the test case
 *
 * Return:   none
 *
 **************/
void qaTreatSpecialSequence(
    unsigned int InitialTestCase,
    unsigned int delay,
    unsigned int FinalTestCase,
    unsigned int timeout )
{
    /* generate the sequence of test cases with delay */
    while( InitialTestCase <= FinalTestCase )
    {
        unsigned int count = 0;
        char test_case[10];
        char buffer[10];
        int length;

        /* generate the individual test case */
        memset( test_case, 0, 10 );
        memcpy( &test_case[count++], "t", 1 );
        length = qaItoa(InitialTestCase, buffer);
        memcpy( test_case+count, buffer, length );
        count += length;
        test_case[count++] = '\0';

        /* run the individual test */
        qaRunSelectedTest( test_case, timeout );

        /* induce a delay */
        sleep(delay);
        InitialTestCase++;
    }
}


/**************
 *
 * Name:     qaGetCurrentTestCase
 *
 * Purpose:  Obtain the current test case to be executed from the list
 *
 * Parms:    pCurrentTestCase[OUT]    - test case to be executed
 *           pCurrentTestSequence[IN] - current test sequence from the file
 *           curIndex                 - current Index of execution sequence
 *           finalIndex               - final Index of execution sequence
 *           timeout                  - timeout for executing the test case
 *
 * Return:   eRESULT_OK  - Normal termination
 *           eRESULT_NOK - Problem with the execution
 *
 **************/
enum eTestCaseError qaGetCurrentTestCase(
    testCase *currentTestCase,
    char *pCurrentTestSequence,
    unsigned long *curIndex,
    unsigned long finalIndex,
    unsigned int timeout )
{
    enum eTestCaseError eTestResult = eRESULT_OK;
    char testSequence[MAX_CHAR_IN_A_LINE];
    char *testcasebuffer;
    unsigned long offset = 0;
    unsigned int startTestNumber = 0, endTestNumber = 0;

    /* intialize the delay and repeat variables */
    currentTestCase->delay  = 0;
    currentTestCase->repeat = 1;

    if( *(pCurrentTestSequence+(*curIndex)) == COLON )
        (*curIndex)++;

    memcpy( testSequence,
            pCurrentTestSequence+(*curIndex),
            finalIndex - (*curIndex) + 1);
    testcasebuffer = strtok( testSequence, ":<;-");

    offset = (*curIndex) + strlen(testcasebuffer);
    /* need to expand for the item for the case tn-tm */
    if( pCurrentTestSequence[offset] == HYPHEN )
    {
        offset++;
        startTestNumber = atoi( testcasebuffer + 1 );
        testcasebuffer  = strtok( NULL, ":<;");
        endTestNumber = atoi( testcasebuffer + 1 );
        qaMakeTestSequence( currentTestCase->test_case,
                          startTestNumber,
                          endTestNumber );
        offset = offset + strlen(testcasebuffer) + 1;
    }
    /* individual test cases */
    else
    {
        offset++;

        /* remember the test case number for special sequence [qn<delay>-qm] */
        startTestNumber = atoi( testcasebuffer + 1 );
        memcpy( currentTestCase->test_case,
                testcasebuffer,
                strlen(testcasebuffer));
        currentTestCase->test_case[strlen(testcasebuffer)] = '\0';
    }

    /* case for the <delay m> and <repeat m> tags */
    if( !strncmp(pCurrentTestSequence+offset, "delay", 5))
    {
        testcasebuffer = strtok( NULL, " ");
        offset = offset + strlen(testcasebuffer) + 1;
        testcasebuffer = strtok( NULL, ">");
        offset = offset + strlen(testcasebuffer) + 1;
        currentTestCase->delay = atoi(testcasebuffer);

        /* special treatment for format [qn<delay>-qm] */
        if( pCurrentTestSequence[offset] == HYPHEN )
        {
            memcpy( testSequence,
                    pCurrentTestSequence+offset-1,
                    finalIndex - (*curIndex) + 2 );
            testcasebuffer = strtok( testSequence, "-");
            offset++;
            testcasebuffer = strtok( NULL, ";");
            endTestNumber = atoi( testcasebuffer + 1 );
            offset = offset + strlen(testcasebuffer) + 1;

            qaTreatSpecialSequence( startTestNumber,
                                    currentTestCase->delay,
                                    endTestNumber,
                                    timeout );
            currentTestCase->test_case[0] = 0;
            currentTestCase->delay = 0;
            currentTestCase->repeat = 0;
        }
    }
    else if( !strncmp(pCurrentTestSequence+offset, "repeat", 6) )
    {
        testcasebuffer = strtok( NULL, " ");
        offset = offset + strlen(testcasebuffer) + 1;
        testcasebuffer = strtok( NULL, ">");
        offset = offset + strlen(testcasebuffer) + 1;
        currentTestCase->repeat = atoi(testcasebuffer);
    }

    /* update the curent index to the offset and proceed */
    if (testcasebuffer != NULL )
    {
        (*curIndex) = offset;
        if( *curIndex >= finalIndex)
            eTestResult = eRESULT_NOK;
    }
    return eTestResult;
}

/**************
 *
 * Name:     qaExecuteTestCase
 *
 * Purpose:  Execute the test case extracted from the test sequence
 *
 * Parms:    pCurrentTestSequence - test sequence to be executed
 *           curIndex             - current Index of execution sequence
 *           finalIndex           - final Index of execution sequence
 *           timeout              - timeout for executing the test case
 *
 * Return:   eRESULT_OK  - Normal termination
 *           eRESULT_NOK - Problem with the execution
 *
 **************/
enum eTestCaseError qaExecuteTestCase(
    char          *pCurrentTestSequence,
    unsigned long curIndex,
    unsigned long finalIndex,
    unsigned int  timeout )
{
    enum eTestCaseError eTestResult = eRESULT_OK;

    /* execute test case until the end of sequence */
    do
    {
        testCase currentTestCase;
        memset( &currentTestCase, 0, sizeof(testCase) );
        eTestResult = qaGetCurrentTestCase( &currentTestCase,
                                          pCurrentTestSequence,
                                          &curIndex,
                                          finalIndex,
                                          timeout );
        /* repeat tag was identified */
        while(currentTestCase.repeat)
        {
            if(currentTestCase.test_case)
                qaProcessTests( currentTestCase.test_case,
                                NULL,
                                NULL,
                                timeout );
            currentTestCase.repeat--;
        }

        /* delay tag was identified */
        if (currentTestCase.delay > 0 )
            sleep(currentTestCase.delay);

    }while( eTestResult == eRESULT_OK );

    return eRESULT_OK;
}

/**************
 *
 * Name:     qaScanTillEnd
 *
 * Purpose:  Scan the current test sequence till end of sequence
 *
 * Parms:    pCurrentTestCase - line of test sequence
 *           pIndex[OUT]      - index to the end of the current execution test
 *           finalIndex       - final Index of execution sequence
 *
 * Return:   eRESULT_OK  - Normal termination
 *           eRESULT_NOK - Problem with the execution
 *
 **************/
enum eTestCaseError qaScanTillEnd(
    char          *pCurrentTestCase,
    unsigned long *pIndex,
    unsigned long finalIndex )
{
    enum eTestCaseError eTestResult = eRESULT_OK;

    /* scan till the end of square bracket */
    if( pCurrentTestCase[*pIndex] == SQBRACKETOPEN )
        while( pCurrentTestCase[*pIndex] != SQBRACKETCLOSE )
        {
            if ( (*pIndex) > finalIndex )
            {
                qaErrorInScript( SQBRACKETCLOSE, pIndex );
                eTestResult = eRESULT_NOK;
                break;
            }
            (*pIndex)++;
        }
    /* scan till the end of square bracket */
    else
        while( (pCurrentTestCase[*pIndex] != SQBRACKETOPEN) &&
               (pCurrentTestCase[*pIndex] != SEMICOLON) )
        {
            if ( (*pIndex) > finalIndex )
            {
                qaErrorInScript( SEMICOLON, pIndex );
                eTestResult = eRESULT_NOK;
                break;
            }
            (*pIndex)++;
        }
    return eTestResult;
}

/**************
 *
 * Name:     qaExecuteTestSequence
 *
 * Purpose:  Execute the current test sequence from the file
 *
 * Parms:    pCurrentTestCase[IN] - line of test sequence
 *           curIndex             - current Index of execution sequence
 *           finalIndex           - final Index of execution sequence
 *           timeout              - timeout for executing the test case
 *
 * Return:   eRESULT_OK  - Normal termination
 *           eRESULT_NOK - Problem with the execution
 *
 **************/
enum eTestCaseError qaExecuteTestSequence(
    char          *pCurrentTestCase,
    unsigned long curIndex,
    unsigned long finalIndex,
    unsigned int  timeout )
{
    enum eTestCaseError eTestResult = eRESULT_OK;
    unsigned long localIndex = 0;

    /* continue till the end of the execution sequence */
    while (curIndex <= finalIndex)
    {
        switch ( pCurrentTestCase[curIndex] )
        {
            /* reached start of an execution sequence */
            case LETTER_t:
                localIndex = 0;
                /* get the complete test sequence length */
                eTestResult = qaScanTillEnd( &pCurrentTestCase[curIndex],
                                           &localIndex,
                                           finalIndex );
                if( eRESULT_OK != eTestResult )
                    return eTestResult;

                /* execute the test sequence */
                localIndex += curIndex;
                eTestResult = qaExecuteTestCase( pCurrentTestCase,
                                                 curIndex,
                                                 localIndex,
                                                 timeout );
                curIndex = localIndex+1;

                if( eRESULT_OK != eTestResult )
                    return eTestResult;
                break;

            /* reached sequence which is grouped together */
            case SQBRACKETOPEN:
            {
                localIndex = 0;
                eTestResult = qaScanTillEnd( &pCurrentTestCase[curIndex],
                                           &localIndex,
                                           finalIndex );
                localIndex += curIndex;
                if( eRESULT_OK != eTestResult )
                    return eTestResult;

                pCurrentTestCase[localIndex] = SEMICOLON;
                /* recursive call for the test sequence execution */
                eTestResult = qaExecuteTestSequence( pCurrentTestCase,
                                                     curIndex + 1,
                                                     localIndex,
                                                     timeout );
                if( eRESULT_OK != eTestResult )
                    return eTestResult;

                /* end of the current sequence */
                if( pCurrentTestCase[localIndex+1] == COLON)
                    curIndex = localIndex + 2;
                /* end of current line of execution - terminate */
                else if( pCurrentTestCase[localIndex+1] == SEMICOLON )
                    return eTestResult;
                /* <delay m> or <repeat m> tags are present */
                else if( pCurrentTestCase[localIndex+1] == OPENTAG)
                {
                    char buffer[100];
                    char *pBuffer;
                    unsigned curIndexforRepeat = curIndex + 1;
                    curIndex = localIndex + 2;
                    memset(buffer, 0, 100 );
                    memcpy( buffer,
                            pCurrentTestCase+curIndex,
                            finalIndex-curIndex );

                    /* <delay m> tag found - induce the delay */
                    if( !strncmp(buffer, "delay", 5) )
                    {
                        unsigned int delay;
                        pBuffer = strtok( buffer, " ");
                        curIndex = curIndex + strlen(pBuffer) + 1;
                        pBuffer = strtok( NULL, ">");
                        curIndex = curIndex + strlen(pBuffer) + 1;
                        delay = atoi(pBuffer);
                        sleep(delay);
                    }
                    /* <repeat m> tag found - repeat test sequence 'm' times */
                    else if( !strncmp(buffer, "repeat", 6) )
                    {
                        unsigned int repeat;
                        pBuffer = strtok( buffer, " ");
                        curIndex = curIndex + strlen(pBuffer) + 1;
                        pBuffer = strtok( NULL, ">");
                        curIndex = curIndex + strlen(pBuffer) + 1;
                        repeat = atoi(pBuffer);
                        while ( (--repeat) && (eTestResult == eRESULT_OK) )
                        {
                            eTestResult = qaExecuteTestSequence( pCurrentTestCase,
                                                                 curIndexforRepeat,
                                                                 localIndex,
                                                                 timeout );
                        }
                    }
                }
                break;
            }

            /* discasrd the line and move to the next line */
            case HASH:
            case SEMICOLON:
                 return eTestResult;

            /* move to the next index */
            case COLON:
                curIndex++;
                break;

            default:
                qaErrorInScript( pCurrentTestCase[curIndex], &curIndex );
                break;
        }
    }
    return eTestResult;
}

/**************
 *
 * Name:     qaDisplayTestXref
 *
 * Purpose:  Displays the list of available test functions and the
 *           corresponding tag name
 *
 * Parms:    none
 *
 * Return:   none
 *
 * Notes:    none
 *
 **************/
package void qaDisplayTestXref()
{
    unsigned int i;
    printf("Test id API name\n");
    printf("======= ========\n");
    for (i = 0; i < TMAX; i++)
    {
        printf ("%4s\t%s \n", ttabs[i].estrp, ttabs[i].fstrp);
    }
}

/**************
 *
 * Name:     qaProcessTestList
 *
 * Purpose:  Runs a selected test
 *
 * Parms:    testsp  - test or list of to execute
 *           timeout - timeout for calls to API as passed in or defaulted
 *
 * Return:   none
 *
 * Abort:    none
 *
 * Notes:    testsp example: t1 or t1,t3,t6 or x or X
 *
 **************/
package void qaProcessTestList(char * testsp, unsigned int timeout)
{
    char * ltestsp = testsp;
    struct slstrtokblk tokblk;
    char *tokresultp;
    char delimiter[2];

    delimiter[0] = ',';
    delimiter[1] = '\0';

    /* Get the first item in the list */
    tokresultp = slstrtok (ltestsp, &delimiter[0], &tokblk);

    /* Show X-ref list if asked */
    if (tokresultp != NULL && sltoupper(*tokresultp) == 'X')
    {
        qaDisplayTestXref ();
        return;
    }

    /* Proceed to running the sample API calls */
    /* Loop until no more */
    while (tokresultp != NULL)
    {
        qaRunSelectedTest(tokresultp, timeout);

        /* get the next available test */
        tokresultp = slstrtok (NULL, &delimiter[0], &tokblk);
    }
}

/**************
 *
 * Name:     qaProcessTestFile
 *
 * Purpose:  Reads test tags from a file and run them
 *
 * Parms:    testfilenamep - file containing test or list of to execute
 *           timeout       - timeout for calls to API as passed in or defaulted
 *
 * Return:   none
 *
 * Abort:    exits if file cannot be opened
 *
 * Notes:    Format same as test list
 *
 **************/
package void qaProcessTestFile(
    char         *testfilenamep,
    char         *testsequencename,
    unsigned int timeout)
{
    swi_uint32 length;
    char buf[128];
    FILE * fp;
    char * readp;
    enum eTestCaseError eTestResult;

    /* Open file first */
    fp = fopen (testfilenamep, "r");

    /* Make sure file open is OK */
    if (fp == NULL)
    {
        printf ("Error: opening %s for reading\n", testfilenamep);
        exit(1);
    }

    /* Proceed to running the sample API calls */
    /* Loop until no more */
    slmemset (buf, 0, sizeof (buf));
    while ( (readp = fgets (buf, sizeof(buf), fp)) != NULL)
    {
        swi_uint32 i = 0;
        curLineNo++;
        /* check for the test sequence */
        if( testsequencename )
        {
            while( slstrncmp(testsequencename, &buf[1], slstrlen(testsequencename)) )
            {
                curLineNo++;
                slmemset (buf, 0, sizeof (buf));
                if((readp = fgets (buf, sizeof(buf), fp)) == NULL)
                    return;
            }
            printf("\nSequence :: %s", buf+1);
            for(i = 0; i < (slstrlen(buf)+10); i++)
               printf("-");
            printf("\n");

            /* test sequence found - trace the next line */
            slmemset (buf, 0, sizeof (buf));
            if((readp = fgets (buf, sizeof(buf), fp)) == NULL)
                return;
        }

        /* hack - slstrtok doesn't like '\n' in the line read from the */
        /* file, it looks for '\0'                                     */
        for (length=0; length< strlen (buf); length++)
        {
            if (buf[length] == '\n')
                buf[length] = '\0';
        }

        /* found a sequence desription, print and move to the next line */
        if( buf[0] == HASH )
        {
            printf("\nSequence :: %s\n", buf+1);
            for(i = 0; i < (slstrlen(buf)+10); i++)
               printf("-");
            printf("\n");
        }
        else
        {
            /* one line of test string to be executed */
            eTestResult = qaExecuteTestSequence( buf, 0, length, timeout );
            if( eRESULT_OK != eTestResult )
                exit(1);
        }

        slmemset (buf, 0, sizeof (buf));
    }
    fclose(fp);
}

/**************
 *
 * Name:     qaProcessTests
 *
 * Purpose:  Calls the Test list processor depending on what is available,
 *           test list or filename.
 *
 * Parms:    testsp           - test or list of to execute
 *           testsfilenamep   - filename of test list
 * *
 * Return:   none
 *
 * Notes:    By the time this function is called, only one of the first
 *           two parameters should be active, since they cannot co-exist
 *
 **************/
package void qaProcessTests(
    char         *testsp,
    char         *testsfilenamep,
    char         *testsequencename,
    unsigned int timeout)
{
    enum eTestCaseError eTestResult;

    /* Do test tags as given from the command line */
    if ( strlen(testsp) )
    {
        /* can be a test sequence if the input ends with a semicolon */
        if( testsp[strlen(testsp)-1] == SEMICOLON )
        {
            eTestResult = qaExecuteTestSequence( testsp,
                                                 0,
                                                 strlen(testsp),
                                                 timeout );

            if( eRESULT_OK != eTestResult )
            {
                printf( "Error executing test sequence!!");
            }
        }
        else
            qaProcessTestList(testsp, timeout);
    }

    /* Filename supplied */
    if (testsfilenamep != NULL)
    {
         qaProcessTestFile(testsfilenamep, testsequencename, timeout);
    }
    else if ( testsequencename && !strlen(testsp) )
    {
        printf ("Error: No filename specified : Use -f or -s option\n\n\n");
        qaDisplayTestXref();
    }
}

/*===========================================================================
METHOD:
   PrintHex (Public Method)

DESCRIPTION:
   Print Hex data, for debug purposes

PARAMETERS:
   pBuffer       [ I ] - Data buffer
   bufSize       [ I ] - Size of data buffer

RETURN VALUE:
   None
===========================================================================*/
void PrintHex(
   void *       pBuffer,
   unsigned int bufSize )
{
   char PrintBuf[1024];
   char * pPrintBuf=&PrintBuf[0];
   unsigned int pos;
   int status;

   if (pPrintBuf == NULL)
   {
      return;
   }
   memset( pPrintBuf, 0 , bufSize * 3 + 1 );

   for (pos = 0; pos < bufSize; pos++)
   {
      status = snprintf( (pPrintBuf + (pos * 3)),
                         4,
                         "%02X ",
                         *(BYTE *)(pBuffer + pos) );
      if (status != 3)
      {
         return;
      }
   }

   DBG( "   : %s\n", pPrintBuf );

   return;
}

/*===========================================================================
METHOD:
   FlushStdinStream (Public Method)

DESCRIPTION:
   Flush the stdin stream

PARAMETERS:
   None

RETURN VALUE:
   None
===========================================================================*/
void FlushStdinStream( )
{
    int inputChar;

    /* keep on reading until an <New Line> or end of file is received */
    do
    {
        inputChar = getchar();
    }
    while ( ( inputChar != ENTER_KEY ) &&
            ( inputChar != EOF ) );
}
