#!/bin/sh

echo "Creating SD card rootfs ..."

rm -rf ../../out/target/product/beagleboard/android_rootfs
rm -rf ../../out/target/product/beagleboard/rootfs.tar.bz2

./copy_prebuilt.sh

cd ../../out/target/product/beagleboard

mkdir android_rootfs
cp -r root/* android_rootfs
cp -r system android_rootfs
cp -r data android_rootfs

rm android_rootfs/fstab.omap3beagleboard

../../../../build/tools/mktarball.sh ../../../host/linux-x86/bin/fs_get_stats android_rootfs . rootfs rootfs.tar.bz2

cd ../../../../

echo "copying tar to image_folder"
cp ./out/target/product/beagleboard/rootfs.tar.bz2 ./image_folder/Filesystem/
cd tcbin_misc/build_scripts

echo "Creating rootfs ... done."
