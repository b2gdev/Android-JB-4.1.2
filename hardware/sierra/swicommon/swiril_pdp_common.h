/*
** This source code is "Not a Contribution" under Apache license
**
** Sierra Wireless RIL
**
** Based on reference-ril by The Android Open Source Project
** Modified by Sierra Wireless, Inc.
**
** Copyright (C) 2012 Sierra Wireless, Inc.
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
*/

#ifndef SWIRIL_PDP_COMMON_H
#define SWIRIL_PDP_COMMON_H 1

/* This CID is used for both profile and connection ID */
#define SWI_DEFAULT_CID 1

typedef enum {
    RIL_STATE_INACTIVE = 0,
    RIL_STATE_ACTIVE_LINKDOWN,
    RIL_STATE_ACTIVE_LINKUP
} RIL_SessionState;

typedef enum {
    RIL_TECH_CDMA = 0,
    RIL_TECH_UMTS
} RIL_PDPTech;

#ifndef SWI_RIL_VERSION_6
typedef enum {
    RIL_PDP_PARAM_RADIO_TECH = 0,
    RIL_PDP_PARAM_DATA_PROFILE,
    RIL_PDP_PARAM_APN,
    RIL_PDP_PARAM_APN_USERNAME,
    RIL_PDP_PARAM_APN_PASSWORD,
    RIL_PDP_PARAM_APN_AUTH_TYPE,
} RIL_PDPParams;
#else
/* Although RIL version 6 defines 8 parameters for RIL_REQUEST_SETUP_DATA_CALL, 
 * Java only sends 7 due to last parameter optional*/
#define RIL_REQUEST_SETUP_DATA_CALL_REQUEST_PARAMS_MIN 7
#define RIL_REQUEST_DEACTIVATE_DATA_CALL_REQUEST_PARAMS 2
#define RIL_ACTIVATE_DATA_CALL_RETRY_NO_SUGGESTION -1

typedef enum {
    RIL_PDP_PARAM_RADIO_TECH = 0,
    RIL_PDP_PARAM_DATA_PROFILE,
    RIL_PDP_PARAM_APN,
    RIL_PDP_PARAM_APN_USERNAME,
    RIL_PDP_PARAM_APN_PASSWORD,
    RIL_PDP_PARAM_APN_AUTH_TYPE,
    RIL_PDP_PARAM_PDP_TYPE,
    RIL_PDP_PARAM_CONNECTION_PROPERTY /**< Optional, it doesn't send by Java in RIL version 6 */
} RIL_PDPParams;

typedef enum {
    RIL_DISCONNECT_REASON_NO_SPECIFIC = 0,
    RIL_DISCONNECT_REASON_RADIO_SHUTDOWN
} RIL_DataCallDisconnectReason;
#endif /* end of SWI_RIL_VERSION_6 */

#endif /* end of SWIRIL_PDP_COMMON_H */
