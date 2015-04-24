#
# This source code is "Not a Contribution" under Apache license
#
# Sierra Wireless RIL
#
# Based on reference-ril by The Android Open Source Project
# and U300 RIL by ST-Ericsson.
# Modified by Sierra Wireless, Inc.
#
# Copyright (C) 2011 Sierra Wireless, Inc.
# Copyright (C) ST-Ericsson AB 2008-2009
# Copyright 2006 The Android Open Source Project
#
# Based on reference-ril
# Modified for ST-Ericsson U300 modems.
# Author: Christian Bejram <christian.bejram@stericsson.com>
#
# XXX using libutils for simulator build only...
#
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

# SDK  8 is Android 2.2.x - aka FROYO
# SDK  9 is isolated
# SDK 10 is Android 2.3.x - aka GINGERBREAD
# SDK 11 is Android 3.0   - aka HONEYCOMB (we don't support this)
# SDK 12 is Android 3.1
# SDK 14 is Android 4.0.1 - aka ICE CREAM SANDWICH
# SDK 15 is Android 4.0.x
# SDK 16 is Android 4.1.x - aka JELLY BEAN
SDK_ANDROID_22_23:= 8 10
SDK_ANDROID_31:= 12
SDK_ANDROID_40:= 14 15
SDK_ANDROID_41:= 16
RIL_VERSION_6:= $(SDK_ANDROID_31) $(SDK_ANDROID_40) $(SDK_ANDROID_41)
ALL_SDK_VERSIONS:= $(SDK_ANDROID_22_23) $(SDK_ANDROID_31) $(SDK_ANDROID_40) $(SDK_ANDROID_41)
LOCAL_CFLAGS :=

# Check 1 - it's an error if none of the SDK Versions match the platform
#           this is running on
ifeq "$(findstring $(PLATFORM_SDK_VERSION),$(ALL_SDK_VERSIONS))" ""
    $(error -- SWIRIL: Unsupported Android version)
endif

# Check 2 - see if this is a Froyo and Gingerbraed build
ifneq "$(findstring $(PLATFORM_SDK_VERSION), $(SDK_ANDROID_22_23))" ""
    LOCAL_PLATFORM_SOURCE:= \
        swiril_pdp_qmi.c \
        swiril_network_qmi.c
endif

# Check 3 - see if this is a Honeycomb MR1 or Ice Cream Sandwich build
ifneq "$(findstring $(PLATFORM_SDK_VERSION),$(RIL_VERSION_6))" ""
    LOCAL_PLATFORM_SOURCE:= \
        swiril_pdp_qmi_honeycomb.c \
        swiril_network_qmi_honeycomb.c
endif

# Check 4 - see if this is a Ice Cream Sandwitch build
ifneq "$(findstring $(PLATFORM_SDK_VERSION),$(SDK_ANDROID_40))" ""
LOCAL_CFLAGS := -DSWI_RIL_VERSION_3X_ICS
endif

LOCAL_SRC_FILES:= \
    swiril_main.c \
    ../swicommon/swiril_network.c \
    swiril_requestdatahandler.c \
    ../swicommon/swiril_sim.c \
    ../swicommon/swiril_misc.c \
    ../swicommon/swiril_cache.c \
    at_channel.c \
    ../swicommon/at_misc.c \
    ../swicommon/at_tok.c \
    ../swicommon/swiril_oem.c \
    swiril_main_qmi.c \
    swiril_sim_qmi.c \
    swiril_misc_qmi.c \
    swiril_device_qmi.c \
    ../swicommon/swiril_gps.c \
    ../swicommon/swiril_gps_inb.c \
    ../swigps/swigps_common.c \
    swiril_gps_qmi.c \
    swiril_oem_device.c \
    swiril_oem_qmi.c \
    swiril_sms_qmi.c \
    swiril_network_common_qmi.c \
    swiril_pdp_common_qmi.c \
    ../swicommon/fcp_parser.c \
    swiril_callhandling.c \
    swiril_services.c \
    $(LOCAL_PLATFORM_SOURCE)

LOCAL_SHARED_LIBRARIES := \
    libcutils libutils libril libnetutils libswims libswiqmiapi

# For asprinf
LOCAL_CFLAGS += -D_GNU_SOURCE 

LOCAL_C_INCLUDES := $(KERNEL_HEADERS) \
    $(TOP)/hardware/ril/libril/ \
    $(TOP)/hardware/sierra/swigps/ \
    $(TOP)/hardware/sierra/swicommon

# Include the swiqmi2 include directories for RIL
LOCAL_C_INCLUDES += \
    $(TOP)/hardware/sierra/swiqmi2/ \
    $(TOP)/hardware/sierra/swiqmi2/am    \
    $(TOP)/hardware/sierra/swiqmi2/pi    \
    $(TOP)/hardware/sierra/swiqmi2/qa    \
    $(TOP)/hardware/sierra/swiqmi2/qa/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/cat/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/cbk/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/dcs/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/dms/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/fms/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/nas/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/pds/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/rms/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/sms/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/wds/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/swioma/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/omadm/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/sar/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/voice/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/uim/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/swi/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qm    \
    $(TOP)/hardware/sierra/swiqmi2/os    \
    $(TOP)/hardware/sierra/swiqmi2/sl

# To include swims atlast so that SwiDataTypes.h is picked from SLQS 
LOCAL_C_INCLUDES += \
    $(TOP)/hardware/sierra/swims

# Disable prelink, or add to build/core/prelink-linux-arm.map
LOCAL_PRELINK_MODULE := false

# Build shared library
LOCAL_SHARED_LIBRARIES += \
    libcutils libutils
LOCAL_LDLIBS += -lpthread
LOCAL_CFLAGS += -DRIL_SHLIB
LOCAL_MODULE:= libsierra-ril
include $(BUILD_SHARED_LIBRARY)
