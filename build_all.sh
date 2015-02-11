#!/bin/sh

echo "#############################################################################################"
echo "Building X-Loader ..."
echo "#############################################################################################"
make -C x-loader/ CROSS_COMPILE=arm-eabi- distclean
make -C x-loader/ CROSS_COMPILE=arm-eabi- omap3530beagle_config
make -C x-loader/ CROSS_COMPILE=arm-eabi-
cd x-loader/
./signGP ./x-load.bin
mv x-load.bin.ift MLO
cd ..
cp ./x-loader/MLO ./image_folder/Boot_Images/

echo "#############################################################################################"
echo "Building U-boot ..."
echo "#############################################################################################"
make -C u-boot/ CROSS_COMPILE=arm-eabi- distclean
make -C u-boot/ CROSS_COMPILE=arm-eabi- omap3_beagle_config
make -C u-boot/ CROSS_COMPILE=arm-eabi-
cp ./u-boot/u-boot.bin ./image_folder/Boot_Images/

echo "#############################################################################################"
echo "Building Kernels and File System ..."
echo "#############################################################################################"
make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- clean

# Run the below line if the Android API was changed
# make update-api TARGET_PRODUCT=beagleboard OMAPES=5.x -j8

# Build SD card kernel
make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- omap3_tcbin_android_sdcard_defconfig

# Build file system
make TARGET_PRODUCT=beagleboard OMAPES=5.x -j8

# Build modules
make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- modules
cp ./kernel/arch/arm/boot/uImage ./image_folder/Boot_Images/uImage

# Build NAND kernel
make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- omap3_tcbin_android_normal_defconfig
make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- uImage -j8
cp ./kernel/arch/arm/boot/uImage ./image_folder/NandFS/norm_uImage

# Build Recovery kernel
./backupfs_build.sh
make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- omap3_tcbin_android_backup_defconfig
make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- uImage -j8
cp ./kernel/arch/arm/boot/uImage ./image_folder/NandFS/bk_uImage
