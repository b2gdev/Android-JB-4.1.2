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

#ifndef U300_RIL_SIM_H
#define U300_RIL_SIM_H 1

/* The SIM's EF File Header size is variable, but 
 * there's no harm in asking for more bytes than 
 * are available. The SIM will never reply with 
 * more than the header size. So the value below 
 * is designed to ensure we always get the maximum
 * number of bytes for the header for proper parsing
 */
#define SIM_HEADER_REQ_SIZE 64

/* The command used to ask the SIM to provide its 
 * file header
 */
#define SIM_GET_RESPONSE 0xC0

void onSimStateChanged(const char *s);

void requestGetSimStatus(void *data, size_t datalen, RIL_Token t);
void requestSIM_IO(void *data, size_t datalen, RIL_Token t);
void requestEnterSimPin(void *data, size_t datalen, RIL_Token t, int request);
void requestChangePassword(void *data, size_t datalen, RIL_Token t,
                           char *facility);
void requestSetFacilityLock(void *data, size_t datalen, RIL_Token t);
void requestQueryFacilityLock(void *data, size_t datalen, RIL_Token t);

void pollSIMState(void *param);


/* Prototypes for functions that have different implementations for QMI and AT
 * RILs, but are used by common code in swicommon/swiril_sim.c
 */
bool useDelayedSIMReady(void);
int getPINRetryCountQMI(void);


#endif
