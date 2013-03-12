PRODUCT_COPY_FILES += \
    hardware/ti/wpan/wl12xx-bluetooth/firmware/TIInit_10.6.15.bts:system/etc/firmware/TIInit_10.6.15.bts \
    hardware/ti/wpan/wl12xx-bluetooth/firmware/TIInit_7.6.15.bts:system/etc/firmware/TIInit_7.6.15.bts \
    hardware/ti/wpan/wl12xx-bluetooth/firmware/TIInit_12.7.27.bts:system/etc/firmware/TIInit_12.7.27.bts \
    hardware/ti/wpan/wl12xx-bluetooth/firmware/TIInit_12.8.32.bts:system/etc/firmware/TIInit_12.8.32.bts

ifeq ($(TARGET_PRODUCT), am335xevm)
PRODUCT_COPY_FILES += \
    hardware/ti/wpan/wl12xx-bluetooth/firmware/am335xevm_TIInit_7.2.31.bts:system/etc/firmware/TIInit_7.2.31.bts
else
PRODUCT_COPY_FILES += \
    hardware/ti/wpan/wl12xx-bluetooth/firmware/omap3evm_TIInit_7.2.31.bts:system/etc/firmware/TIInit_7.2.31.bts
endif

