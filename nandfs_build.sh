#!/bin/sh

echo "Creating rootfs ..."
cd out/target/product/beagleboard

mkdir -p system/usr/bin/
mkdir -p system/usr/modules/
mkdir -p system/etc/

cp -rfd ../../../../tcbin_misc/busybox/* system/usr/bin/
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
cp ../../../../tcbin_misc/TalkBack.apk system/app/
#cp ../../../../tcbin_misc/BrailleBack.apk system/app/
cp ../../../../tcbin_misc/ESFileExplorer.apk system/app/
cp ../../../../tcbin_misc/BIGLauncherFREE.apk system/app/

# move TCBIN-Text-Viewer
#mv ./data/app/TCBIN-Text-Viewer.apk system/app/

cd ../../../../

echo "Building NAND UBIFS FS"

cd tcbin_misc/mtd-utils

./mkfs.ubifs/mkfs.ubifs -r ../../out/target/product/beagleboard/system/ -m 2048 -e 126976 -c 3991 -o ubifs.img
./ubi-utils/ubinize -o ubi_system.img -O 2048 -m 2048 -p 128KiB -s 2048 ubinize_system.cfg
mv ubi_system.img ../../image_folder/NandFS/

./mkfs.ubifs/mkfs.ubifs -r ../../out/target/product/beagleboard/data/ -m 2048 -e 126976 -c 3991 -o ubifs.img
./ubi-utils/ubinize -o ubi_data.img -O 2048 -m 2048 -p 128KiB -s 2048 ubinize_data.cfg
mv ubi_data.img ../../image_folder/NandFS/

./mkfs.ubifs/mkfs.ubifs -r ../../out/target/product/beagleboard/cache/ -m 2048 -e 126976 -c 3991 -o ubifs.img
./ubi-utils/ubinize -o ubi_cache.img -O 2048 -m 2048 -p 128KiB -s 2048 ubinize_cache.cfg
mv ubi_cache.img ../../image_folder/NandFS/

cd ../../
