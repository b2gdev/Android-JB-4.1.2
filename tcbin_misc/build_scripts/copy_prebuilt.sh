#!/bin/sh

echo "Copying precompiled components ..."

cd ../../out/target/product/beagleboard

mkdir -p system/usr/bin/
mkdir -p system/usr/modules/
mkdir -p system/etc/

cp -rfd ../../../../tcbin_misc/busybox/* system/usr/bin/
cp ../../../../tcbin_misc/executables/installotapackage system/usr/bin/
cp ../../../../tcbin_misc/executables/set_wifi_mac system/usr/bin/
cp ../../../../tcbin_misc/executables/format_emmc system/usr/bin/
cp ../../../../tcbin_misc/executables/nuke_emmc system/usr/bin/
cp ../../../../tcbin_misc/executables/getMSP430Version system/usr/bin/
chmod -R +x system/usr/bin/
chmod 777 system/usr/bin/busybox
cp ../../../../tcbin_misc/gps.conf system/etc/

# copy drivers
cp ../../../../kernel/drivers/accessibility/braille/metec/metec_flat20.ko ./system/usr/modules/
cp ../../../../kernel/drivers/input/keyboard/cp430_keypad/cp430_keypad.ko ./system/usr/modules/
cp ../../../../kernel/drivers/mfd/cp430-core/cp430_core.ko ./system/usr/modules/
cp ../../../../kernel/drivers/misc/cp430_charger/cp430_charger.ko ./system/usr/modules/
cp ../../../../kernel/drivers/power/tcbin_power/cp430_power/cp430_power.ko ./system/usr/modules/
cp ../../../../kernel/drivers/net/usb/GobiNet.ko ./system/usr/modules/

# other bundled apps
cp ../../../../tcbin_misc/apks/AndroSensor_1.9.4.4a.apk system/app/
cp ../../../../tcbin_misc/apks/MarineCompass.apk system/app/
cp ../../../../tcbin_misc/apks/Easy_Voice_Recorder.apk system/app/
cp ../../../../tcbin_misc/apks/TalkBack.apk system/app/
cp ../../../../tcbin_misc/apks/ESFileExplorer.apk system/app/
cp ../../../../device/ti/beagleboard/sierra_binaries/system/app/com.sierra.logs.apk system/app/

# Copy b2g-ui
cp ../../../../tcbin_misc/apks/b2g_ui.apk system/app/
unzip -j -o ../../../../tcbin_misc/apks/b2g_ui.apk lib/armeabi/libb2g_ui.so -d system/lib/
chmod 755 system/lib/libb2g_ui.so

rm -rf data/nativebenchmark/
rm -rf data/nativetest/
rm data/app/* 2> /dev/null

cd ../../../../tcbin_misc/build_scripts
