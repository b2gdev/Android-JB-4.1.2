#!/bin/sh

# TCBIN

alias cpbkfs='./backupfs_build.sh'

alias loadnormkernel='make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- omap3_tcbin_android_normal_defconfig'
alias loadbackupkernel='make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- omap3_tcbin_android_backup_defconfig'
alias loadsdkernel='make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- omap3_tcbin_android_sdcard_defconfig'

alias cpkernel='cp -v kernel/arch/arm/boot/uImage image_folder/NandFS/norm_uImage'
alias cpbkkernel='cp -v kernel/arch/arm/boot/uImage image_folder/NandFS/bk_uImage'
alias cpub='cp -v ./u-boot/u-boot.bin ./image_folder/Boot_Images/'

alias makeModules='loadnormkernel; make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- modules'
alias makeuImage='loadnormkernel; make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- uImage -j8; cpkernel'
alias makebkuImage='cpbkfs; loadbackupkernel; make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- uImage -j8; cpbkkernel'
alias makesduImage='loadsdkernel; make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- uImage -j8; cp -v kernel/arch/arm/boot/uImage image_folder/Boot_Images/uImage'
alias makeMenuCon='make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- menuconfig'
alias makeFS='loadnormkernel; export TCBIN_PASS=zonedev; make TARGET_PRODUCT=beagleboard OMAPES=5.x -j8'
alias makeub='make -C u-boot/ CROSS_COMPILE=arm-eabi-; cpub'

alias sdfmakebkuImage='makebkuImage; cpsdbkuimage; umf'

alias cluImage='make -C kernel/ ARCH=arm CROSS_COMPILE=arm-eabi- clean'

alias mkf='./mkmmc-android.sh /dev/sdf'
alias mkb='./mkmmc-android.sh /dev/sdb'

alias mksys='cd tcbin_misc/mtd-utils; ./mkfs.ubifs/mkfs.ubifs -r ../../out/target/product/beagleboard/system/ -m 2048 -e 126976 -c 3991 -o ubifs.img; ./ubi-utils/ubinize -o ubi_system.img -O 2048 -m 2048 -p 128KiB -s 2048 ubinize_system.cfg; mv -v ubi_system.img ../../image_folder/NandFS/; cd ../../'

alias umf='umount /dev/sdf*'
alias umb='umount /dev/sdb*'

alias cpsduimage='cp -v image_folder/NandFS/norm_uImage /media/data/'
alias cpsduimagef='cp -v image_folder/NandFS/norm_uImage /media/data/; umf'
alias cpsduimageb='cp -v image_folder/NandFS/norm_uImage /media/data/; umb'
alias cpsdbkuimage='cp -v image_folder/NandFS/bk_uImage /media/data/'
alias cpsdsys='cp -v image_folder/NandFS/ubi_system.img /media/data/'
alias cpsddata='cp -v image_folder/NandFS/ubi_data.img /media/data/'
alias cpsdcache='cp -v image_folder/NandFS/ubi_cache.img /media/data/'
alias cpsdub='cp -v u-boot/u-boot.bin /media/data/'

alias gits='git status'
alias gita='git add .'
alias gitaf='git add -f'
alias gitai='git add'
alias gitdn='git diff --name-only'
alias gitd='git diff'
alias gitcm='git commit -m'
alias gitc='git checkout'
alias gitl='git log'
alias gitb='git branch'

alias bfg='geany kernel/arch/arm/mach-omap2/board-omap3beagle.c &'

PATH=/media/Data/tcbin_jb/rowboat-android/prebuilts/gcc/linux-x86/arm/arm-eabi-4.6/bin:/usr/lib/jvm/jdk1.6.0_38/bin:/sbin:$PATH
export PATH
