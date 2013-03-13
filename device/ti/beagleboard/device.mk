#
# Copyright (C) 2011 The Android Open-Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#


PRODUCT_COPY_FILES := \
	device/ti/beagleboard/init.omap3beagleboard.rc:root/init.omap3beagleboard.rc \
	device/ti/beagleboard/vold.fstab:system/etc/vold.fstab \
	device/ti/beagleboard/media_codecs.xml:system/etc/media_codecs.xml \
	device/ti/beagleboard/mixer_paths.xml:system/etc/mixer_paths.xml

PRODUCT_COPY_FILES += \
	frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
	frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml

PRODUCT_COPY_FILES += \
	frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
    frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml

PRODUCT_PROPERTY_OVERRIDES := \
	hwui.render_dirty_regions=false \
	ro.sf.lcd_density=120

PRODUCT_PROPERTY_OVERRIDES += \
	persist.sys.strictmode.visual=0 \
	persist.sys.strictmode.disable=1

PRODUCT_PROPERTY_OVERRIDES += \
	persist.sys.usb.config=mass_storage,adb

PRODUCT_CHARACTERISTICS := tablet

DEVICE_PACKAGE_OVERLAYS := \
    device/ti/beagleboard/overlay

PRODUCT_TAGS += dalvik.gc.type-precise

PRODUCT_PACKAGES += \
	librs_jni \
	com.android.future.usb.accessory

PRODUCT_PACKAGES += \
	libaudioutils \
	libsrec_jni

PRODUCT_PACKAGES += \
        audio.primary.beagleboard \
        tinycap \
        tinymix \
        tinyplay

PRODUCT_PACKAGES += \
	dhcpcd.conf \
	TQS_D_1.7.ini \
	calibrator

# Filesystem management tools
PRODUCT_PACKAGES += \
	make_ext4fs

PRODUCT_PACKAGES += \
	camera.omap3

# libsensors
PRODUCT_PACKAGES += \
	sensors.beagleboard

# FileManager Application
PRODUCT_PACKAGES += \
        FileManager-1.1.6


$(call inherit-product, frameworks/native/build/tablet-dalvik-heap.mk)
