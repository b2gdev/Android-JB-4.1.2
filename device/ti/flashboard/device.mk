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
	device/ti/flashboard/init.omap3evm.rc:root/init.omap3evm.rc \
	device/ti/flashboard/TWL4030_Keypad.kcm:system/usr/keylayout/TWL4030_Keypad.kcm \
	device/ti/flashboard/TWL4030_Keypad.kl:system/usr/keylayout/TWL4030_Keypad.kl \
	device/ti/flashboard/vold.fstab:system/etc/vold.fstab \
	device/ti/flashboard/mixer_paths.xml:system/etc/mixer_paths.xml \
	device/ti/flashboard/media_codecs.xml:system/etc/media_codecs.xml \
	device/ti/flashboard/audio_policy.conf:system/etc/audio_policy.conf

# Hardware Features
PRODUCT_COPY_FILES += \
	frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml

# Bluetooth support
PRODUCT_COPY_FILES += \
	frameworks/native/data/etc/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
	system/bluetooth/data/main.nonsmartphone.conf:system/etc/bluetooth/main.conf

# WLAN support
PRODUCT_COPY_FILES +=\
	frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
	frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml

PRODUCT_PROPERTY_OVERRIDES := \
	hwui.render_dirty_regions=false \
	ro.sf.lcd_density=160

# Comment the following to enable USB Mass Storage (UMS) gadget mode
PRODUCT_PROPERTY_OVERRIDES += \
	persist.sys.usb.config=adb

PRODUCT_PROPERTY_OVERRIDES += \
	persist.sys.strictmode.visual=0 \
	persist.sys.strictmode.disable=1

# Uncomment the following to enable USB Mass Storage (UMS) gadget mode
#PRODUCT_PROPERTY_OVERRIDES += \
#	persist.sys.usb.config=mass_storage,adb

PRODUCT_CHARACTERISTICS := tablet,nosdcard

DEVICE_PACKAGE_OVERLAYS := \
	device/ti/flashboard/overlay

PRODUCT_TAGS += dalvik.gc.type-precise

PRODUCT_PACKAGES += \
	librs_jni \
	com.android.future.usb.accessory

PRODUCT_PACKAGES += \
	libaudioutils \
	libsrec_jni

PRODUCT_PACKAGES += \
	audio.primary.flashboard \
	tinycap \
	tinymix \
	tinyplay

# Bluetooth A2DP audio support
PRODUCT_PACKAGES += \
	audio.a2dp.default

PRODUCT_PACKAGES += \
	dhcpcd.conf

# WI-Fi
PRODUCT_PACKAGES += \
	hostapd.conf \
	wifical.sh \
	TQS_D_1.7.ini \
	TQS_D_1.7_127x.ini \
	TQS_S_2.6.ini \
	crda \
	regulatory.bin \
	calibrator

PRODUCT_PROPERTY_OVERRIDES += \
	wifi.interface=wlan0

# libsensors
PRODUCT_PACKAGES += \
	sensors.omap3

# Filesystem management tools
PRODUCT_PACKAGES += \
	make_ext4fs

PRODUCT_PACKAGES += \
	camera.omap3

# BlueZ test tools
PRODUCT_PACKAGES += \
	hciconfig \
	hcitool

# FileManager Application
PRODUCT_PACKAGES += \
	Amazed \
	FileManager-1.1.6

$(call inherit-product, frameworks/native/build/tablet-dalvik-heap.mk)
$(call inherit-product-if-exists, hardware/ti/wpan/wl12xx-bluetooth/wl12xx_bt_products.mk)
$(call inherit-product-if-exists, hardware/ti/wlan/mac80211/firmware/wl12xx_wlan_fw_products.mk)
