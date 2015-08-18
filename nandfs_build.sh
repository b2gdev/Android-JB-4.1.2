#!/bin/sh

echo "Creating NAND rootfs ..."

./copy_prebuilt.sh

echo "Building NAND UBIFS FS"

cd tcbin_misc/mtd-utils

./mkfs.ubifs/mkfs.ubifs -r ../../out/target/product/beagleboard/system/ -m 2048 -e 126976 -c 3991 -o ubifs.img
./ubi-utils/ubinize -o ubi_system.img -O 2048 -m 2048 -p 128KiB -s 2048 ubinize_system.cfg
mv ubi_system.img ../../image_folder/NandFS/

./mkfs.ubifs/mkfs.ubifs -r ../../out/target/product/beagleboard/cache/ -m 2048 -e 126976 -c 3991 -o ubifs.img
./ubi-utils/ubinize -o ubi_cache.img -O 2048 -m 2048 -p 128KiB -s 2048 ubinize_cache.cfg
mv ubi_cache.img ../../image_folder/NandFS/

cd ../../
