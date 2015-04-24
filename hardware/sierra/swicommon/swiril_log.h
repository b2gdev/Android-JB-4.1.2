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

#ifndef SWIRIL_LOG_H
#define SWIRIL_LOG_H 

#include "telephony/ril.h"

/* Comment/uncomment following line to disbale/enable RIL debug print */
#define SIERRA_RILDEBUG 1
#ifndef SIERRA_RILDEBUG
/* Undefine debug logging except LOGE */
#define LOGI(...)   ((void)0)
#define LOGD(...)   ((void)0)
#define LOGW(...)   ((void)0)
#endif /* SIERRA_RILDEBUG */

#if (RIL_VERSION == 7)
#define LOGV(...) ALOGV(__VA_ARGS__)
#define LOGI(...) ALOGI(__VA_ARGS__)
#define LOGD(...) ALOGD(__VA_ARGS__)
#define LOGE(...) ALOGE(__VA_ARGS__)
#define LOGW(...) ALOGW(__VA_ARGS__)
#endif

#include <cutils/log.h>

#endif /* SWIRIL_LOG_H */
