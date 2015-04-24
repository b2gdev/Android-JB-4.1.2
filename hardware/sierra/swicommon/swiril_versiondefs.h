/*
** This source code is "Not a Contribution" under Apache license
**
** Sierra Wireless RIL
**
** Based on reference-ril by The Android Open Source Project
** Modified by Sierra Wireless, Inc.
**
** Copyright (C) 2011 Sierra Wireless, Inc.
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
*/

#ifndef SWIRIL_VERSIONDEFS_H
#define SWIRIL_VERSIONDEFS_H 

#define RIL_VERSION_3                       3 /**< Android RIL version 3 */
#define RIL_VERSION_6                       6 /**< Android R3.1 and R4.0, RIL version 6 */

#if (RIL_VERSION == RIL_VERSION_3)
#undef SWI_RIL_VERSION_6
#elif (RIL_VERSION >= RIL_VERSION_6)
#define SWI_RIL_VERSION_6 1
#define RIL_SIM_IO RIL_SIM_IO_v5
#define RIL_REQUEST_REGISTRATION_STATE RIL_REQUEST_VOICE_REGISTRATION_STATE
#define RIL_REQUEST_GPRS_REGISTRATION_STATE RIL_REQUEST_DATA_REGISTRATION_STATE
#define RIL_UNSOL_RESPONSE_NETWORK_STATE_CHANGED RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED

#define RIL_REQUEST_CDMA_SET_SUBSCRIPTION RIL_REQUEST_CDMA_SET_SUBSCRIPTION_SOURCE
#else
#error Unsupported RIL version
#endif

#endif /* SWIRIL_VERSIONDEFS_H */
