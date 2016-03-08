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
cp ../../../../tcbin_misc/executables/getAllFWVersions system/usr/bin/
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
cp ../../../../kernel/drivers/misc/lm3553/lm3553.ko ./system/usr/modules/

# other bundled apps
#cp ../../../../tcbin_misc/apks/AndroSensor.apk system/app/
#cp ../../../../tcbin_misc/apks/MarineCompass.apk system/app/
cp ../../../../tcbin_misc/apks/EasyVoiceRecorder.apk system/app/
cp ../../../../tcbin_misc/apks/ES_FileExplorer.apk system/app/
cp ../../../../tcbin_misc/apks/B2G_Updater.apk system/app/
cp ../../../../tcbin_misc/apks/NBP_Editor.apk system/app/
cp ../../../../tcbin_misc/apks/NBP_Compass.apk system/app/
cp ../../../../tcbin_misc/apks/PeriodicTable.apk system/app/

# Copy b2g-ui
cp ../../../../tcbin_misc/apks/B2G_UI.apk system/app/
unzip -j -o ../../../../tcbin_misc/apks/B2G_UI.apk lib/armeabi/* -d system/lib/

# Copy OI_FileManager
cp ../../../../tcbin_misc/apks/OI_FileManager.apk system/app/
unzip -j -o ../../../../tcbin_misc/apks/OI_FileManager.apk lib/armeabi/* -d system/lib/

# Copy Chrome
cp ../../../../tcbin_misc/apks/Chrome.apk system/app/
unzip -j -o ../../../../tcbin_misc/apks/Chrome.apk lib/armeabi-v7a/* -d system/lib/

# Copy Tunein Player
cp ../../../../tcbin_misc/apks/TuneinPlayer.apk system/app/
unzip -j -o ../../../../tcbin_misc/apks/TuneinPlayer.apk lib/armeabi-v7a/* -d system/lib/

chmod 755 system/lib/lib*

rm -rf data/nativebenchmark/
rm -rf data/nativetest/
rm data/app/* 2> /dev/null

cd ../../../../tcbin_misc/build_scripts
