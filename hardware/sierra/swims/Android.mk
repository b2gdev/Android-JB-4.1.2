#
# This source code is "Not a Contribution" under Apache license
#
# Sierra Wireless RIL
#
# Based on reference-ril by The Android Open Source Project
# Modified by Sierra Wireless, Inc.
#
# Copyright (C) 2011 Sierra Wireless, Inc.
# Copyright 2006 The Android Open Source Project

# For shared lib
# =======================
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:=       \
    swims_ossdkscanmain.c \
    swims_ossdk.c         \
    swims_ossdkcheck.c    
    
LOCAL_C_INCLUDES := $(KERNEL_HEADERS) \
    $(TOP)/hardware/sierra/swicommon

LOCAL_SHARED_LIBRARIES := \
    libcutils
    
LOCAL_LDLIBS += -lpthread 

# Disable prelink, or add to build/core/prelink-linux-arm.map
LOCAL_PRELINK_MODULE := false

#build shared lib
LOCAL_MODULE:= libswims
include $(BUILD_SHARED_LIBRARY)


