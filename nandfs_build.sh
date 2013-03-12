#!/bin/sh

./rootfs_build.sh

echo "#############################################################################################"
echo "Building NAND UBIFS FS"
echo "#############################################################################################"

cd tcbin_misc/mtd-utils
./mkfs.ubifs/mkfs.ubifs -r ../../out/target/product/beagleboard/android_rootfs/ -m 2048 -e 126976 -c 3991 -o ubifs.img
./ubi-utils/ubinize -o ubi.img -O 2048 -m 2048 -p 128KiB -s 2048 ubinize.cfg
mv ubi.img ../../image_folder/NandFS/ubi.img
cd ../../
