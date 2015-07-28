LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES += \
	src/mediactl.c \
	src/v4l2subdev.c \
	src/options.c \
	src/main.c


LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/linux
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := media-ctl

include $(BUILD_EXECUTABLE)

