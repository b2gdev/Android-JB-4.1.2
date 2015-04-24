/* 
 * This source code is "Not a Contribution" under Apache license
 *
 * Sierra Wireless GPS Handler, based on gps_freerunner.c
 *
 * Copyright 2006, The Android Open Source Project
 * Copyright 2009, Michael Trimarchi <michael@panicking.kicks-ass.org>
 * Copyright 2011, Sierra Wireless, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>

#define  LOG_TAG  "gps_mod"

#include "swiril_log.h"
#include <cutils/sockets.h>
#include <cutils/properties.h>
#include "swigps_sgi.h"

#define  GPS_DEBUG  1

#define  DFR(...)   LOGD(__VA_ARGS__)

#if GPS_DEBUG
#  define  D(...)   LOGD(__VA_ARGS__)
#else
#  define  D(...)   ((void)0)
#endif

#define GPS_STATUS_CB(_cb, _s)    \
  if ((_cb).status_cb) {          \
    GpsStatus gps_status;         \
    gps_status.status = (_s);     \
    (_cb).status_cb(&gps_status); \
    DFR("gps status callback: 0x%x", _s); \
  }

extern const GpsInterface* gps_get_swi_hardware_interface(void);

/*******                                   *******
 *******     HARDWARE MODULE INTERFACE     *******
 *******                                   *******/

/**
 * 
 * Wrapper function to return a pointer to the GPS hardware
 * interface.
 *
 * @param [in] dev
 *    Pointer to the device structure, allocated when the gps_open()
 *    function was called. Unused in this call.
 *
 * @return
 *    Pointer to this module's gps_interface structure containing 
 *    function pointers which an upper layer can use to interact 
 *    with the GPS device
 *
 * @Note:
 *    None
 * 
 */
static const GpsInterface *swigps_module_get_gps_interface( struct gps_device_t *dev )
{
    DFR("Started %s", __FUNCTION__ );
    return gps_get_swi_hardware_interface();
}

/**
 * 
 * Hardware interface function to open the GPS device. This function
 * is written following the rules for hardware modules as spelled out 
 * (rather loosely) in hardware.h
 *
 * @param [in] module
 *
 * @param [in] name
 *
 * @param [out] device
 *
 * @return
 *
 * @note
 *
 */
static int open_swigps( const struct hw_module_t *module, 
                     char const *name, 
                     struct hw_device_t **device )
{
    DFR("Started %s", __FUNCTION__);
    struct gps_device_t *dev = malloc(sizeof(struct gps_device_t));
    memset( dev, 0, sizeof(*dev));

    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t *) module;
    dev->get_gps_interface = swigps_module_get_gps_interface;

    *device = (struct hw_device_t *) dev;
    return 0;
}

/** 
 * 
 * Structure containing pointers to functions representing various
 * methods implemented by this hardware module. At this writing
 * there is only one method defined - to open the hardware module
 *
 */
struct hw_module_methods_t swigps_module_methods = {
    .open = open_swigps
};

/**
 * 
 * HAL_MODULE_INFO_SYM is a structure required by all hardware modules
 * in Android as of version 2.3. This structure is used to make the 
 * associated hardware known to the operating system and provides a 
 * standard way of identifying the hardware
 *
 */
struct hw_module_t HAL_MODULE_INFO_SYM = {
    .tag = HARDWARE_MODULE_TAG,
    .version_major = 1,
    .version_minor = 0,
    .id = GPS_HARDWARE_MODULE_ID,
    .name = "Sierra GPS Module",
    .author = "Sierra Wireless, Inc.",
    .methods = &swigps_module_methods
};

