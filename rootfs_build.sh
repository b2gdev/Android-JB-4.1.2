#!/bin/sh

echo "Creating rootfs ..."
cd out/target/product/beagleboard

rm -rf android_rootfs
rm -rf rootfs.tar.bz2



mkdir -p system/usr/bin/
mkdir -p system/usr/modules/
mkdir -p system/etc/

cp -rfd ../../../../tcbin_misc/busybox/* system/usr/bin/
cp ../../../../tcbin_misc/format_emmc system/usr/bin/
chmod -R +x system/usr/bin/
chmod 777 system/usr/bin/busybox
cp ../../../../tcbin_misc/gps.conf system/etc/

# copy drivers
cp ../../../../kernel/drivers/accessibility/braille/metec/metec_flat20.ko ./system/usr/modules/
cp ../../../../kernel/drivers/input/keyboard/cp430_keypad/cp430_keypad.ko ./system/usr/modules/
cp ../../../../kernel/drivers/mfd/cp430-core/cp430_core.ko ./system/usr/modules/
cp ../../../../kernel/drivers/misc/cp430_charger/cp430_charger.ko ./system/usr/modules/
cp ../../../../kernel/drivers/power/tcbin_power/cp430_power/cp430_power.ko ./system/usr/modules/
cp ../../../../kernel/drivers/net/usb/sierra_net.ko ./system/usr/modules/
cp ../../../../kernel/drivers/usb/serial/sierra.ko ./system/usr/modules/

# other test apps
cp ../../../../tcbin_misc/AndroSensor_1.9.4.4a.apk system/app/
cp ../../../../tcbin_misc/MarineCompass.apk system/app/
cp ../../../../tcbin_misc/Easy_Voice_Recorder.apk system/app/
cp ../../../../tcbin_misc/BrailleBack.apk system/app/
cp ../../../../tcbin_misc/TalkBack.apk system/app/
#cp ../../../../tcbin_misc/BrailleBack.apk system/app/
cp ../../../../tcbin_misc/ESFileExplorer.apk system/app/
cp ../../../../tcbin_misc/BIGLauncherFREE.apk system/app/

# move TCBIN-Text-Viewer
#mv ./data/app/TCBIN-Text-Viewer.apk system/app/

mkdir android_rootfs
cp -r root/* android_rootfs
cp -r system android_rootfs
cp -r data android_rootfs

rm android_rootfs/fstab.omap3beagleboard

../../../../build/tools/mktarball.sh ../../../host/linux-x86/bin/fs_get_stats android_rootfs . rootfs rootfs.tar.bz2

cd ../../../../

echo "copying tar to image_folder"
cp ./out/target/product/beagleboard/rootfs.tar.bz2 ./image_folder/Filesystem/

echo "Creating rootfs ... done."
