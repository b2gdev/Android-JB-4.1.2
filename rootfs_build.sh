#!/bin/sh

echo "Creating rootfs ..."
cd out/target/product/beagleboard

rm -rf android_rootfs
rm -rf rootfs.tar.bz2

mkdir android_rootfs
mkdir -p android_rootfs/usr/bin/
mkdir -p android_rootfs/usr/modules/
mkdir -p android_rootfs/system/etc/

cp -r root/* android_rootfs
cp -r system android_rootfs
cp -rfd ../../../../tcbin_misc/busybox/* android_rootfs/usr/bin/
chmod -R +x android_rootfs/usr/bin/
cp ../../../../tcbin_misc/gps.conf android_rootfs/system/etc/
cp ../../../../tcbin_misc/init.omap3beagleboard.rc android_rootfs/

# copy drivers
cd android_rootfs
cp ../../../../../kernel/drivers/accessibility/braille/metec/metec_flat20.ko ./usr/modules/
cp ../../../../../kernel/drivers/input/keyboard/cp430_keypad/cp430_keypad.ko ./usr/modules/
cp ../../../../../kernel/drivers/mfd/cp430-core/cp430_core.ko ./usr/modules/
cp ../../../../../kernel/drivers/misc/lm3553/lm3553.ko ./usr/modules/
cp ../../../../../kernel/drivers/misc/cp430_charger/cp430_charger.ko ./usr/modules/
cp ../../../../../kernel/drivers/power/bq27x00_battery.ko ./usr/modules/
# copy TCBIN-Text-Viewer
mkdir -p data/app
cp ../data/app/TCBIN-Text-Viewer.apk data/app/
# other test apps
cp ../../../../../tcbin_misc/AndroSensor_1.9.4.4a.apk data/app/
cp ../../../../../tcbin_misc/com.chartcross.gpstest-1.apk data/app/
cp ../../../../../tcbin_misc/MarineCompass.apk data/app/
cp ../../../../../tcbin_misc/Easy_Voice_Recorder.apk data/app/
cp ../../../../../tcbin_misc/Angry_Birds_2.1.1.apk data/app/
cd ../

../../../../build/tools/mktarball.sh ../../../host/linux-x86/bin/fs_get_stats android_rootfs . rootfs rootfs.tar.bz2

cd ../../../../

echo "copying tar to image_folder"
cp ./out/target/product/beagleboard/rootfs.tar.bz2 ./image_folder/Filesystem/

echo "Creating rootfs ... done."
