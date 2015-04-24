/**
 *
 * @ingroup swiril
 *
 * @file 
 * Prototypes for main RIL QMI Sierra functions
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#ifndef SWIRIL_MAIN_QMI_H
#define SWIRIL_MAIN_QMI_H 1

#include "swiril_main.h"
#include "swiril_device_qmi.h"
#include <stdbool.h>

void initRILFunctionHandlerQMI(RIL_RequestHandlerFunc *rilsolreqhandlerp,
                               RIL_RequestHandlerFunc *rilsolreqhandlercdmap);
void qmiDeviceConnect(void);
void qmiDeviceDisconnect(void);
int  qmiGetConnectedDeviceID(void);
void qmiSLQSRestart(void);
bool getPropertyRadioNotifiction(void);
void clearDeviceNodeAndKey(void);

#endif
