/*
** This source code is "Not a Contribution" under Apache license
**
** Sierra Wireless RIL
**
** Based on reference-ril by The Android Open Source Project
** and U300 RIL by ST-Ericsson.
** Modified by Sierra Wireless, Inc.
**
** Copyright (C) 2010 Sierra Wireless, Inc.
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

#ifndef U300_RIL_NETWORK_H
#define U300_RIL_NETWORK_H 1

#include <stdbool.h>

#define RIL_GSM_SIGNAL_STRENGTH_DEFAULT         0
#define RIL_GSM_SIGNAL_BIT_ERROR_RATE_DEFAULT   99
#define VOICE_REGISTRATION_STATE_PARAMS         15

#ifndef SWI_RIL_VERSION_6
#define DATA_REGISTRATION_STATE_PARAMS          4
#else
#define RIL_LTE_SIGNAL_STRENGTH_UNUSED          -1
#define RIL_LTE_SIGNAL_STRENGTH_INVALID         0x7FFFFFFF
#define RIL_LTE_RSRP_MAX_LIMIT                  -44
#define RIL_LTE_RSRP_MIN_LIMIT                  -140
#define RIL_LTE_RSRQ_MAX_LIMIT                  -3
#define RIL_LTE_RSRQ_MIN_LIMIT                  -20
#define DATA_REGISTRATION_STATE_PARAMS          6
#endif

/* SWI_TBD there is a inconsistence between ril.h and Java layer 
 * regarding eHRPD and LTE, use Java layer definition for now */
typedef enum {
    RIL_RAT_UNKNOWN = 0,
    RIL_RAT_GPRS,
    RIL_RAT_EDGE,
    RIL_RAT_UMTS,
    RIL_RAT_IS95A,
    RIL_RAT_IS95B,
    RIL_RAT_1xRTT,
    RIL_RAT_EvDo_REV0,
    RIL_RAT_EvDo_REVA,
    RIL_RAT_HSDPA,
    RIL_RAT_HSUPA,
    RIL_RAT_HSPA,
    RIL_RAT_EvDo_REVB = 12,
    RIL_RAT_LTE = 13,
    RIL_RAT_EHRPD = 14,
    RIL_RAT_HSPAP = 15 /* HSPA+ */
} RIL_RAT;

void requestEnterNetworkDepersonalization(void *data, size_t datalen,
                                          RIL_Token t);
void requestOperator(void *data, size_t datalen, RIL_Token t);
bool queryRegState(bool use_cgreg, int *response, unsigned long *cellID, bool *hasAct);
#endif

