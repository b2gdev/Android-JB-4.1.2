#!/bin/sh
echo "Creating rootfs ..."
cd out/target/product/beagleboard
rm -rf android_rootfs
rm -rf rootfs.tar.bz2
mkdir android_rootfs
cp -r root/* android_rootfs
cp -r system android_rootfs
../../../../build/tools/mktarball.sh ../../../host/linux-x86/bin/fs_get_stats android_rootfs . rootfs rootfs.tar.bz2
cd ../../../../
echo "copying tar to image_folder"
cp ./out/target/product/beagleboard/rootfs.tar.bz2 ./image_folder/Filesystem/
echo "Creating rootfs ... done."
