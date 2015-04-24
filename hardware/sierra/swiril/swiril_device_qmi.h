/**
 *
 * @ingroup swiril
 *
 * @file 
 * Prototypes for RIL device related QMI based Sierra functions
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#ifndef SWIRIL_DEVICE_QMI_H
#define SWIRIL_DEVICE_QMI_H 1

#include <stdbool.h>
#include "SwiDataTypes.h"
#include "qaFmsGetFirmwareInfo.h"

typedef enum {
    QMI_POWER_ONLINE = 0,
    QMI_POWER_LOW,
    QMI_POWER_FACTORY_TEST_MODE,
    QMI_POWER_OFFLINE,
    QMI_POWER_RESET,
    QMI_POWER_OFF,
    QMI_POWER_PERSISTENT_LOW_POWER
}QMI_POWER_STATE;

typedef enum {
    SWI_FW_INFO_TYPE_UNKNOWN = 0,
    SWI_FW_INFO_TYPE_GOBI_CDMA,
    SWI_FW_INFO_TYPE_GOBI_UMTS,
    SWI_FW_INFO_TYPE_SWI_UMTS_ONLY,
    SWI_FW_INFO_TYPE_SWI_DUAL_MODE
}SWI_FW_INFO_TYPE;

void initFirmwareInfoQMI(void);
void clearFirmwareInfoQMI(void);
bool getFirmwareInfoTechQMI(SWI_FW_INFO_TYPE *tech);
bool getFirmwareInfoCarrierQMI(enum eGobiImageCarrier *image);
bool isDualModeModule(void);
bool isDualModeRunningCDMA(void);
bool isDualModeRunningLTE(void);
bool IsSierraDevice(void);
bool setPowerOffQMI(void);
void changePowerModeQMI(QMI_POWER_STATE state);
void setPowerOff(void);
char initializeChannelCDMA(void);
void requestRadioPowerCDMA(void *data, size_t datalen, RIL_Token t);
char initializeChannelUMTS(void);
void requestRadioPowerUMTS(void *data, size_t datalen, RIL_Token t);
void requestBasebandVersionQMI(void *data, size_t datalen, RIL_Token t);
void activateServiceCDMA(void);
void registerActivationStatusCallbackCDMA(void);
void registerSetOMADMStateCallbackCDMA(void);
void requestOEMHookStringsPRLUpdate(void *data, size_t count, RIL_Token t);
void requestOEMHookStringsFactoryReset(void *data, size_t count, RIL_Token t);
void requestOEMHookStringsGetRfSarState(const char **data, size_t count, RIL_Token t);
void requestOEMHookStringsSetRfSarState(const char **data, size_t count, RIL_Token t);
void requestOEMHookStringsSetSMSWake(const char **data, size_t count, RIL_Token t);
void requestResetRadioQMI(void *data, size_t count, RIL_Token t);
void requestOEMHookStringsCancelOmaDm(void *data, size_t count, RIL_Token t);
bool IsOMADMSessionPending(void);
#endif
