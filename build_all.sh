#!/bin/sh


#echo "#############################################################################################"
#echo "Building X-Loader ..."
#echo "#############################################################################################"
#make -C x-loader/ CROSS_COMPILE=arm-eabi- distclean
#make -C x-loader/ CROSS_COMPILE=arm-eabi- omap3530beagle_config
#make -C x-loader/ CROSS_COMPILE=arm-eabi-
#cd x-loader/
#./signGP ./x-load.bin
#mv x-load.bin.ift MLO
#cd ..
#cp ./x-loader/MLO ./image_folder/Boot_Images/

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
make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- omap3_tcbin_android_sdcard_defconfig
export TCBIN_PASS=zonedev
make TARGET_PRODUCT=beagleboard OMAPES=5.x -j8
make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- modules
cp ./kernel/arch/arm/boot/uImage ./image_folder/Boot_Images/uImage

make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- omap3_tcbin_android_normal_defconfig
make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- uImage -j8
cp ./kernel/arch/arm/boot/uImage ./image_folder/NandFS/norm_uImage
./backupfs_build.sh
make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- omap3_tcbin_android_backup_defconfig
make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- uImage -j8
cp ./kernel/arch/arm/boot/uImage ./image_folder/NandFS/bk_uImage
