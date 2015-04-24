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

#ifndef U300_RIL_H
#define U300_RIL_H 1

/* RIL version specific definitions */
#include "swiril_versiondefs.h"

typedef enum {
    SWI_RUNNING_TECH_UNKNOWN                   = 0, /* Modem runs in unknown mode */
    SWI_RUNNING_TECH_GSM                       = 1, /* Modem runs in GSM, GPRS, UMTS mode */
    SWI_RUNNING_TECH_LTE                       = 2, /* Modem runs in LTE mode */
    SWI_RUNNING_TECH_CDMA                      = 3, /* Modem runs in CDMA mode */
    SWI_RUNNING_TECH_EHRPD                     = 4, /* Modem runs in CDMA eHRPD mode */
} SWI_RunningTech;

#define SWI_SMS_FORMAT_GSM		1
#define SWI_SMS_FORMAT_CDMA		2

RIL_RadioState currentState();
void setRadioState(RIL_RadioState newState);
void getScreenStateLock();
int getScreenState();
void releaseScreenStateLock();
int onSupports(int requestCode);
void onCancel(RIL_Token t);
const char *getVersion(void);
int is_modem_ps_only(void);
int getPropertySMSFormat(void);
int getIMEISV(char **datap);

char* ril_iface;

const struct RIL_Env *s_rilenv;

#define RIL_onRequestComplete(t, e, response, responselen) s_rilenv->OnRequestComplete(t,e, response, responselen)
#define RIL_onUnsolicitedResponse(a,b,c) s_rilenv->OnUnsolicitedResponse(a,b,c)

/**
 * RIL_Request Handler Function pointer
 *
 * @param data is pointer to data defined for that RIL_REQUEST_*
 *        data is owned by caller, and should not be modified or freed by callee
 * @param t should be used in subsequent call to RIL_onResponse
 * @param datalen the length of data
 *
 */
typedef void (*RIL_RequestHandlerFunc) (void *data, size_t datalen, RIL_Token t);

void enqueueRILEvent(int isPrio, void (*callback) (void *param), 
                     void *param, const struct timeval *relativeTime);

#define RIL_EVENT_QUEUE_NORMAL 0
#define RIL_EVENT_QUEUE_PRIO 1
#define RIL_EVENT_QUEUE_ALL 2

/** AT Port Opening Delay default value. 
 * The AT port is opened and closed for every transaction. This allows
 * the kernel to suspend the USB connection to the modem when it's not
 * in use. However, some modems require additional delay before they
 * can exchange AT command data. This constant is the default value to
 * use for the delay - ultimately accessed within "at_channel.c". It
 * can be overridden from the command line to the sierra RIL using the
 * "-o <delay in ms>" switch.
 */
#define AT_OPEN_DELAY_MS 10

#endif
