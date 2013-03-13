LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	GravitySensor.cpp \
	LinearAccelerationSensor.cpp \
	RotationVectorSensor.cpp \
	OrientationSensor.cpp \
    SensorService.cpp \
    SensorInterface.cpp \
    SensorDevice.cpp \
    SecondOrderLowPassFilter.cpp


LOCAL_CFLAGS:= -DLOG_TAG=\"SensorService\"

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libhardware \
	libutils \
	libbinder \
	libui \
	libgui



LOCAL_MODULE:= libsensorservice

include $(BUILD_SHARED_LIBRARY)
