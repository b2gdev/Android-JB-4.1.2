LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#LOCAL_CFLAGS += -fvisibility=hidden
#LOCAL_LDLIBS := -llog 
LOCAL_MODULE := libbrailledisplay
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_TAGS := eng
LOCAL_SRC_FILES := BrailleDisplay.c
include $(BUILD_SHARED_LIBRARY)
