ifneq ($(WILINK), wl18xx)
PRODUCT_COPY_FILES += \
    hardware/ti/wlan/mac80211/firmware/wl127x-fw-4-mr.bin:system/etc/firmware/ti-connectivity/wl127x-fw-4-mr.bin \
    hardware/ti/wlan/mac80211/firmware/wl127x-fw-4-sr.bin:system/etc/firmware/ti-connectivity/wl127x-fw-4-sr.bin \
    hardware/ti/wlan/mac80211/firmware/wl127x-fw-4-plt.bin:system/etc/firmware/ti-connectivity/wl127x-fw-4-plt.bin \
    hardware/ti/wlan/mac80211/firmware/wl1271-nvs.bin:system/etc/firmware/ti-connectivity/wl1271-nvs.bin \
    hardware/ti/wlan/mac80211/firmware/wl128x-fw-4-mr.bin:system/etc/firmware/ti-connectivity/wl128x-fw-4-mr.bin \
    hardware/ti/wlan/mac80211/firmware/wl128x-fw-4-plt.bin:system/etc/firmware/ti-connectivity/wl128x-fw-4-plt.bin \
    hardware/ti/wlan/mac80211/firmware/wl128x-fw-4-sr.bin:system/etc/firmware/ti-connectivity/wl128x-fw-4-sr.bin \
    hardware/ti/wlan/mac80211/firmware/wl128x-nvs.bin:system/etc/firmware/ti-connectivity/wl128x-nvs.bin
else
PRODUCT_COPY_FILES += \
    hardware/ti/wlan/mac80211/firmware/wl18xx-fw.bin:system/etc/firmware/ti-connectivity/wl18xx-fw.bin \
    hardware/ti/wlan/mac80211/firmware/wl18xx-conf.bin:system/etc/firmware/ti-connectivity/wl18xx-conf.bin
endif
