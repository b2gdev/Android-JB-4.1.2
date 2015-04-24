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

LOCAL_SRC_FILES:=     \
	swiim_main.c      \
	swiim_imglist.c    \
	swiim_os.c    \
	swiim_imgselect.c    \
	swiim_imgupdate.c

LOCAL_CFLAGS:=

LOCAL_C_INCLUDES := $(KERNEL_HEADERS) \
    $(TOP)/hardware/ril/libril/ \
    $(TOP)/hardware/sierra/swiqmi2/ \
    $(TOP)/hardware/sierra/swiqmi2/qm \
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
    $(TOP)/hardware/sierra/swiqmitools/common \
    $(TOP)/hardware/sierra/swicommon  

LOCAL_STATIC_LIBRARIES := \
    swiqmitool_common

LOCAL_SHARED_LIBRARIES := \
	libcutils libswiqmiapi

# Disable prelink, or add to build/core/prelink-linux-arm.map
LOCAL_PRELINK_MODULE := false

# Build executable
LOCAL_MODULE:= SierraImgMgr
include $(BUILD_EXECUTABLE)

