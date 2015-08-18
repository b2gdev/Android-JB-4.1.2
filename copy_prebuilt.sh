#!/bin/sh

echo "Copying precompiled components ..."

cd out/target/product/beagleboard

mkdir -p system/usr/bin/
mkdir -p system/usr/modules/
mkdir -p system/etc/

cp -rfd ../../../../tcbin_misc/busybox/* system/usr/bin/
cp ../../../../tcbin_misc/installotapackage system/usr/bin/
cp ../../../../tcbin_misc/set_wifi_mac system/usr/bin/
cp ../../../../tcbin_misc/format_emmc system/usr/bin/
cp ../../../../tcbin_misc/nuke_emmc system/usr/bin/
chmod -R +x system/usr/bin/
chmod 777 system/usr/bin/busybox
cp ../../../../tcbin_misc/gps.conf system/etc/

# copy drivers
cp ../../../../kernel/drivers/accessibility/braille/metec/metec_flat20.ko ./system/usr/modules/
cp ../../../../kernel/drivers/input/keyboard/cp430_keypad/cp430_keypad.ko ./system/usr/modules/
cp ../../../../kernel/drivers/mfd/cp430-core/cp430_core.ko ./system/usr/modules/
cp ../../../../kernel/drivers/misc/cp430_charger/cp430_charger.ko ./system/usr/modules/
cp ../../../../kernel/drivers/power/tcbin_power/cp430_power/cp430_power.ko ./system/usr/modules/

# other bundled apps
cp ../../../../tcbin_misc/AndroSensor_1.9.4.4a.apk system/app/
cp ../../../../tcbin_misc/MarineCompass.apk system/app/
cp ../../../../tcbin_misc/Easy_Voice_Recorder.apk system/app/
cp ../../../../tcbin_misc/TalkBack.apk system/app/
cp ../../../../tcbin_misc/ESFileExplorer.apk system/app/
cp ../../../../tcbin_misc/b2g_ui.apk system/app/

# Copy b2g-ui
unzip -j -o ../../../../tcbin_misc/b2g_ui.apk lib/armeabi/libb2g_ui.so -d system/lib/
chmod 755 system/lib/libb2g_ui.so

rm -rf data/nativebenchmark/
rm -rf data/nativetest/
rm data/app/* 2> /dev/null

cd ../../../../
