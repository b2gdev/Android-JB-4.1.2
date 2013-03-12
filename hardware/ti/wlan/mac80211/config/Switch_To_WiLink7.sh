#! /system/bin/sh

echo "*********************************************************"
echo "* Switching to use WiLink7 (wl12xx) WLAN driver modules *"
echo "*********************************************************"

MODULES_PATH=/system/lib/modules
WL7_MODULES_PATH=$MODULES_PATH/wl7

if [ ! -d $WL7_MODULES_PATH ]; then
echo "WiLink7 modules folder not present at $WL7_MODULES_PATH"
echo "Failed to switch to use WiLink7"
exit 0
fi
MOD_NOT_PRESENT=0
[ ! -f $WL7_MODULES_PATH/cfg80211.ko ] && MOD_NOT_PRESENT=1;
[ ! -f $WL7_MODULES_PATH/mac80211.ko ] && MOD_NOT_PRESENT=1;
[ ! -f $WL7_MODULES_PATH/wl12xx.ko ] && MOD_NOT_PRESENT=1;
[ ! -f $WL7_MODULES_PATH/wl12xx_sdio.ko ] && MOD_NOT_PRESENT=1;

if [ $MOD_NOT_PRESENT == 1 ]; then
echo "All modules required for WiLink7 are not present in $WL7_MODULES_PATH";
echo "Failed to switch to use WiLink7"
exit 0;
fi

echo ""
echo "Remounting file system..."
echo ""

# Remount system partition as rw
mount -o remount rw /system

echo ""
echo "Trying to make sure Wi-Fi is off..."
echo ""
svc wifi disable
sleep 1
svc wifi disable
sleep 1

echo ""
echo "Removing WL8 (wl18xx) modules..."
echo ""

# Remove WL8 modules
rm $MODULES_PATH/cfg80211.ko
rm $MODULES_PATH/mac80211.ko
rm $MODULES_PATH/wl12xx.ko
rm $MODULES_PATH/wl18xx.ko
rm $MODULES_PATH/wlcore.ko
rm $MODULES_PATH/wlcore_sdio.ko
sleep 2
sync

echo ""
echo "Copying over WL7 (wl12xx) modules..."
echo ""

# Copy WL7 modules
ln -s $WL7_MODULES_PATH/cfg80211.ko $MODULES_PATH/cfg80211.ko
chmod 644 $MODULES_PATH/cfg80211.ko
ln -s $WL7_MODULES_PATH/mac80211.ko $MODULES_PATH/mac80211.ko
chmod 644 $MODULES_PATH/mac80211.ko
ln -s $WL7_MODULES_PATH/wl12xx.ko $MODULES_PATH/wl12xx.ko
chmod 644 $MODULES_PATH/wl12xx.ko
ln -s $WL7_MODULES_PATH/wl12xx_sdio.ko $MODULES_PATH/wlcore_sdio.ko
chmod 644 $MODULES_PATH/wlcore_sdio.ko
sleep 2
sync

echo ""
echo "Rebooting..."
echo ""

# reboot device
reboot

