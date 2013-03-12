#
# Copyright (C) 2011-2012 The Android-x86 Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#


PRODUCT_DIR := $(dir $(lastword $(filter-out device/common/%,$(filter device/%,$(ALL_PRODUCTS)))))

PRODUCT_COPY_FILES := \
    $(if $(wildcard $(PRODUCT_DIR)ts.env),$(PRODUCT_DIR),$(LOCAL_PATH)/)ts.env:system/etc/ts.env \
    $(if $(wildcard $(PRODUCT_DIR)ts.conf),$(PRODUCT_DIR),$(LOCAL_PATH)/)ts.conf:system/etc/ts.conf \
    $(if $(wildcard $(PRODUCT_DIR)pointercal),$(PRODUCT_DIR)pointercal:system/etc/pointercal) \

# for touchscreen calibration
PRODUCT_PACKAGES := \
    ts_calibrate \

# plugins
PRODUCT_PACKAGES += \
    arctic2 \
    collie \
    corgi \
    dejitter \
    h3600 \
    input-raw \
    linear \
    linear-h2200 \
    mk712 \
    pthres \
    tatung \
    touchkit \
    ucb1x00 \
    variance
