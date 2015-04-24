#
# This source code is "Not a Contribution" under Apache license
#
# Sierra Wireless GPS Support
#
# Based on Android.mk as presented on Android Forums at the following
# URL: http://source.android.com/porting/gps.html
# Modified by Sierra Wireless, Inc.
#
# Copyright (C) 2011 Sierra Wireless, Inc.
#
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

#
# Use SDK Version to check for which set of source and header files to
# build. One SDK version (so far) represents all variants of a given 
# release. Therefore, if 2.2.x contained more than one value for 'x'
# then only one SDK version check is necessary. We do this even though
# the SDK and GPS aren't formally related
#
# SDK  8 is Android 2.2.x - aka FROYO
# SDK  9 is isolated
# SDK 10 is Android 2.3.x - aka GINGERBREAD
# SDK 11 is Android 3.0   - aka HONEYCOMB (we don't support this)
# SDK 12 is Android 3.1
# SDK 14 is Android 4.0.1 - aka ICE CREAM SANDWICH
# SDK 15 is Android 4.0.x
# SDK 16 is Android 4.1.x - aka JELLY BEAN
SDK_ANDROID_22:= 8
SDK_ANDROID_23_31_40_41:= 10 11 12 14 15 16
ALL_SDK_VERSIONS:=$(SDK_ANDROID_22) $(SDK_ANDROID_23_31_40_41)

# Check 1 - it's an error if none of the SDK Versions match the platform
#           this is running on
ifeq "$(findstring $(PLATFORM_SDK_VERSION),$(ALL_SDK_VERSIONS))" ""
    $(error -- SWIGPS: Unsupported Android version)
endif

# Check 2 - see if this is a Froyo build
ifneq "$(findstring $(PLATFORM_SDK_VERSION),$(SDK_ANDROID_22))" ""
    GPS_PLATFORM_SOURCE:= \
        swigps_froyo.c
    GPS_PLATFORM_HEADER:= \
        hardware/libhardware_legacy/include/hardware_legacy
endif

# Check 3 - see if this is a Gingerbread or Honeycomb build
ifneq "$(findstring $(PLATFORM_SDK_VERSION),$(SDK_ANDROID_23_31_40_41))" ""
    GPS_PLATFORM_SOURCE:= \
        swigps_module.c \
        swigps_gingerplus.c
    GPS_PLATFORM_HEADER:= \
        /hardware/libhardware/include/hardware/
endif

# For QMI GPS support
LOCAL_MODULE:= libswigpsqmi

# Disable prelink, or add to build/core/prelink-linux-arm.map
LOCAL_PRELINK_MODULE := false

LOCAL_SHARED_LIBRARIES := \
    libcutils libutils libnetutils

LOCAL_SRC_FILES := \
    swigps.c \
    swigps_outb.c \
    swigps_common.c \
    swigps_qmi.c \
    $(GPS_PLATFORM_SOURCE)

LOCAL_C_INCLUDES := $(KERNEL_HEADERS) \
                    $(TOP)/$(GPS_PLATFORM_HEADER) \
                    $(TOP)/hardware/sierra/swiril \
                    $(TOP)/hardware/sierra/swicommon

LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)

LOCAL_LDLIBS += -lpthread
include $(BUILD_SHARED_LIBRARY)


# For AT GPS support. NOTE: The AT RIL is not available for
# Honeycomb releases of Android although this makefile will
# build the AT GPS library for honeycomb. This library should
# not be used on honeycomb for that reason.
include $(CLEAR_VARS)

LOCAL_MODULE:= libswigpsat

# Disable prelink, or add to build/core/prelink-linux-arm.map
LOCAL_PRELINK_MODULE := false

LOCAL_SHARED_LIBRARIES := \
    libcutils libutils libnetutils

LOCAL_SRC_FILES := \
    swigps.c \
    swigps_outb.c \
    swigps_common.c \
    swigps_at.c \
    $(GPS_PLATFORM_SOURCE)

LOCAL_C_INCLUDES := $(KERNEL_HEADERS) \
                    $(TOP)/$(GPS_PLATFORM_HEADER) \
                    $(TOP)/hardware/sierra/swiril \
                    $(TOP)/hardware/sierra/swicommon

LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)

LOCAL_LDLIBS += -lpthread
include $(BUILD_SHARED_LIBRARY)

