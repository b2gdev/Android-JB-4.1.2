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
** Author: Henrik Persson <henrik.persson@stericsson.com>
*/

#ifndef _U300_RIL_CONFIG_H
#define _U300_RIL_CONFIG_H 1

#include <telephony/ril.h>

/*
 * Requests that will go on the priority queue instead of the normal queue.
 * 
 * If only one queue is configured, the request will be put on the normal
 * queue and sent as a normal request.
 */
static int prioRequests[] = {
    RIL_REQUEST_DIAL,
    RIL_REQUEST_HANGUP,
    RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND,
    RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND,
    RIL_REQUEST_GET_CURRENT_CALLS,
    RIL_REQUEST_ANSWER,
    RIL_REQUEST_LAST_CALL_FAIL_CAUSE,
    RIL_REQUEST_UDUB,
    RIL_REQUEST_SET_MUTE,
    RIL_REQUEST_GET_MUTE,
    RIL_REQUEST_CONFERENCE,
    RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE,
    RIL_REQUEST_SIGNAL_STRENGTH,
    RIL_REQUEST_DTMF,
    RIL_REQUEST_DTMF_START,
    RIL_REQUEST_DTMF_STOP
};
#endif

