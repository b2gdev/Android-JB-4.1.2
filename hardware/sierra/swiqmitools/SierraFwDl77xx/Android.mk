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

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES :=    $(KERNEL_HEADERS) \
    $(TOP)/hardware/ril/libril/ \
    $(TOP)/hardware/sierra/swiqmi2 \
    $(TOP)/hardware/sierra/swiqmi2/qm \
    $(TOP)/hardware/sierra/swiqmi2/im \
    $(TOP)/hardware/sierra/swiqmi2/qa \
    $(TOP)/hardware/sierra/swiqmi2/qa/inc \
    $(TOP)/hardware/sierra/swiqmi2/qa/dms/inc  \
    $(TOP)/hardware/sierra/swiqmi2/qa/wds/inc  \
    $(TOP)/hardware/sierra/swiqmi2/qa/nas/inc  \
    $(TOP)/hardware/sierra/swiqmi2/qa/sms/inc  \
    $(TOP)/hardware/sierra/swiqmi2/qa/fms/inc  \
    $(TOP)/hardware/sierra/swiqmi2/qa/pds/inc  \
    $(TOP)/hardware/sierra/swiqmi2/qa/cat/inc  \
    $(TOP)/hardware/sierra/swiqmi2/qa/rms/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/dcs/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/swioma/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/omadm/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/sar/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/voice/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/uim/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/swi/inc    \
    $(TOP)/hardware/sierra/swiqmi2/qa/cbk/inc  \
    $(TOP)/hardware/sierra/swiqmitools/common   \
    $(TOP)/hardware/sierra/swicommon  
	
LOCAL_SRC_FILES:= \
	mc77xximgmgmt.c

LOCAL_STATIC_LIBRARIES := \
    swiqmitool_common

LOCAL_SHARED_LIBRARIES := \
	libcutils libswiqmiapi

LOCAL_CFLAGS := 

LOCAL_MODULE:= SierraFwDl77xx
include $(BUILD_EXECUTABLE)

