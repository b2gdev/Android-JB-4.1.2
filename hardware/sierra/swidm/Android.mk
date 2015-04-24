#
# This source code is "Not a Contribution" under Apache license
#
# Sierra Wireless DM Log function
#
# Copyright (C) 2011 Sierra Wireless, Inc.
#

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:=     \
    swidm_main.c      \
    swiserial.c

LOCAL_CFLAGS:= -g -O0

LOCAL_C_INCLUDES := $(KERNEL_HEADERS) \
    $(TOP)/hardware/sierra/swims \
    $(TOP)/hardware/sierra/swicommon

LOCAL_SHARED_LIBRARIES := \
    libcutils libswims

# Disable prelink, or add to build/core/prelink-linux-arm.map
LOCAL_PRELINK_MODULE := false

# Build executable
LOCAL_MODULE:= SierraDMLog
include $(BUILD_EXECUTABLE)

