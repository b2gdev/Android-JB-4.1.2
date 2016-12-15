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
	device/ti/beagleboard/init.b2g.rc:root/init.b2g.rc \
	device/ti/beagleboard/beagleboard.xml:system/etc/sound/beagleboard.xml \
	device/ti/beagleboard/vold.fstab:system/etc/vold.fstab \
	device/ti/beagleboard/fstab.omap3beagleboard:root/fstab.omap3beagleboard \
	device/ti/beagleboard/media_codecs.xml:system/etc/media_codecs.xml \
	device/ti/beagleboard/b2g-init:system/bin/b2g-init \
	device/ti/beagleboard/audio_policy.conf:system/etc/audio_policy.conf \
	device/ti/beagleboard/sierra_binaries/system/bin/init.dhcpcd:system/bin/init.dhcpcd \
	device/ti/beagleboard/sierra_binaries/system/bin/init.ril-daemon:system/bin/init.ril-daemon \
	device/ti/beagleboard/sierra_binaries/system/bin/init.dhcpcd-at:system/bin/init.dhcpcd-at \
	device/ti/beagleboard/sierra_binaries/system/bin/init.ril-daemon-at:system/bin/init.ril-daemon-at \
	device/ti/beagleboard/sierra_binaries/system/bin/init.dhcpcd-qmi:system/bin/init.dhcpcd-qmi \
	device/ti/beagleboard/sierra_binaries/system/bin/init.ril-daemon-qmi:system/bin/init.ril-daemon-qmi \
	device/ti/beagleboard/sierra_binaries/system/bin/ash:system/bin/ash \
	device/ti/beagleboard/sierra_binaries/system/bin/SierraDMLog:system/bin/SierraDMLog \
	device/ti/beagleboard/sierra_binaries/system/bin/slqssdk:system/bin/slqssdk \
	device/ti/beagleboard/sierra_binaries/system/bin/swisdk:system/bin/swisdk \
	device/ti/beagleboard/sierra_binaries/system/lib/libswiqmiapi.so:system/lib/libswiqmiapi.so \
	device/ti/beagleboard/sierra_binaries/system/lib/libswigpsqmi.so:system/lib/libswigpsqmi.so \
	device/ti/beagleboard/sierra_binaries/system/lib/libswigpsat.so:system/lib/libswigpsat.so \
	device/ti/beagleboard/sierra_binaries/system/lib/libsierra-ril.so:system/lib/libsierra-ril.so \
	device/ti/beagleboard/sierra_binaries/system/lib/libsierraat-ril.so:system/lib/libsierraat-ril.so \
	device/ti/beagleboard/sierra_binaries/system/lib/libswims.so:system/lib/libswims.so \
	device/ti/beagleboard/sierra_binaries/system/lib/libswisdkapi.so:system/lib/libswisdkapi.so
	

PRODUCT_COPY_FILES += \
	frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
	frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml

PRODUCT_COPY_FILES += \
	frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
    	frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml

PRODUCT_COPY_FILES += \
	frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
	frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
	frameworks/native/data/etc/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
	frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    	system/bluetooth/data/main.nonsmartphone.conf:system/etc/bluetooth/main.conf
	

PRODUCT_PROPERTY_OVERRIDES := \
	hwui.render_dirty_regions=false \
	ro.sf.lcd_density=120

PRODUCT_PROPERTY_OVERRIDES += \
	persist.sys.strictmode.visual=0 \
	persist.sys.strictmode.disable=1

PRODUCT_PROPERTY_OVERRIDES += \
	persist.sys.usb.config=adb

PRODUCT_PROPERTY_OVERRIDES += \
	ro.sierra.voice=1

PRODUCT_CHARACTERISTICS := tablet

DEVICE_PACKAGE_OVERLAYS := \
    device/ti/beagleboard/overlay

PRODUCT_TAGS += dalvik.gc.type-precise

PRODUCT_PACKAGES += \
	ash

PRODUCT_PACKAGES += \
	librs_jni \
	com.android.future.usb.accessory

PRODUCT_PACKAGES += \
	libaudioutils \
	libsrec_jni

PRODUCT_PACKAGES += \
        audio.primary.beagleboard \
		audio_policy.beagleboard \
        tinycap \
        tinymix \
        tinyplay \
        uim-sysfs \
	media-ctl

PRODUCT_PACKAGES += \
	dhcpcd.conf

# Filesystem management tools
PRODUCT_PACKAGES += \
	make_ext4fs

PRODUCT_PACKAGES += \
	camera.omap3

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
	sensors.beagleboard
		 
# Bluetooth A2DP audio support
PRODUCT_PACKAGES += \
        audio.a2dp.default

# GPS
PRODUCT_PACKAGES += \
        gps.beagleboard

# BlueZ test tools
PRODUCT_PACKAGES += \
	hciconfig \
	hcitool	        

$(call inherit-product, frameworks/native/build/tablet-dalvik-heap.mk)
$(call inherit-product-if-exists, hardware/ti/wpan/wl12xx-bluetooth/wl12xx_bt_products.mk)
$(call inherit-product-if-exists, hardware/ti/wlan/mac80211/firmware/wl12xx_wlan_fw_products.mk)
