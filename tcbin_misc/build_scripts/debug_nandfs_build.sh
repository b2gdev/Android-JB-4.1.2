#!/bin/sh

echo "Creating NAND rootfs ..."

./copy_prebuilt.sh
./copy_debug_prebuilt.sh
./copy_gapps.sh

echo "Building NAND UBIFS FS"

cd ../mtd-utils

./mkfs.ubifs/mkfs.ubifs -r ../../out/target/product/beagleboard/system/ -m 2048 -e 126976 -c 3991 -o ubifs.img
./ubi-utils/ubinize -o ubi_system.img -O 2048 -m 2048 -p 128KiB -s 2048 ubinize_system.cfg
mv ubi_system.img ../../image_folder/NandFS/

cd ../build_scripts
