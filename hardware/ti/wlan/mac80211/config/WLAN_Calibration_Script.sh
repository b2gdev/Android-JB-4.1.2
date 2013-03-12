#! /system/bin/sh
TARGET_FW_DIR=/system/etc/firmware/ti-connectivity
TARGET_NVS_FILE=$TARGET_FW_DIR/wl1271-nvs.bin
TARGET_INI_FILE=/system/etc/wifi/TQS_D_1.7.ini
WL12xx_MODULE=/system/lib/modules/wl12xx_sdio.ko
WLCORE_MODULE=/system/lib/modules/wlcore.ko
SDIO_MODULE=/system/lib/modules/wlcore_sdio.ko
WL7_MODULE=/system/lib/modules/wl7/wl12xx_sdio.ko

if [ -e $WLCORE_MODULE ];
then
    echo "************************************************************"
    echo "Using R8 driver"
    echo "If this is omap5/wl18xx platform, Calibration is not needed"
    echo "************************************************************"
    exit
else
   echo ""
fi

DRIVER_STATE=`getprop wlan.driver.status`

case "$DRIVER_STATE" in
  "ok") echo " ********************************************************"
             echo " * Turn Wi-Fi OFF and launch the script for calibration *"
             echo " ********************************************************"
             exit;;
          *) echo " ******************************"
             echo " * Starting Wi-Fi calibration *"
             echo " ******************************";;
esac

# Remount system partition as rw
mount -o remount rw /system

# Point it to module in wl7 folder if it is not present in /system/lib/modules/
[ ! -f $WL12xx_MODULE ] && WL12xx_MODULE=$WL7_MODULE;

# Remove old NVS file
`mv $TARGET_NVS_FILE $TARGET_FW_DIR/wl1271-nvs.bin.org`

# Actual calibration...
# calibrator plt autocalibrate <dev> <module path> <ini file1> <nvs file> <mac addr>
# Leaving mac address field empty for random mac
calibrator plt autocalibrate wlan0 $WL12xx_MODULE $TARGET_INI_FILE $TARGET_NVS_FILE

echo " ******************************"

if [ -f $TARGET_NVS_FILE ]; then
echo " * Finished Wi-Fi calibration *"
`rm $TARGET_FW_DIR/wl1271-nvs.bin.org`
else
echo " * Wi-Fi calibration Failed  *"
echo " * Restored default nvs file *"
`mv $TARGET_FW_DIR/wl1271-nvs.bin.org $TARGET_NVS_FILE`
fi

echo " ******************************"
