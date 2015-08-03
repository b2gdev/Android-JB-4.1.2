#!/bin/bash

EXPECTED_ARGS=1

if [ $# == 2 ]
then
	if [ $2 = "syscheck" ]; then
		echo "Flashing Testing file system"
		$0 $1 Boot_Images/MLO Boot_Images/u-boot.bin Boot_Images/uImage Filesystem/testing*
		exit 
	else
		echo "Incorrect usage"
		exit
	fi
	
fi

if [ $# == $EXPECTED_ARGS ]
then
	echo "Assuming Default Locations for Prebuilt Images"
	$0 $1 Boot_Images/MLO Boot_Images/u-boot.bin Boot_Images/uImage Filesystem/rootfs* Media_Clips START_HERE
	exit 
fi

if [[ -z $1 || -z $2 || -z $3 || -z $4 ]]
then
	echo "mkmmc-android Usage:"
	echo "	mkmmc-android <device> <MLO> <u-boot.bin> <uImage> <rootfs tar.bz2 > <Optional Media_Clips> <Optional START_HERE folder>"
	echo "	Example: mkmmc-android /dev/sdc MLO u-boot.bin uImage rootfs.tar.bz2 Media_Clips START_HERE"
	exit
fi

if ! [[ -e $2 ]]
then
	echo "Incorrect MLO location!"
	exit
fi

if ! [[ -e $3 ]]
then
	echo "Incorrect u-boot.bin location!"
	exit
fi

if ! [[ -e $4 ]]
then
	echo "Incorrect uImage location!"
	exit
fi

if ! [[ -e $5 ]]
then
	echo "Incorrect rootfs location!"
	exit
fi

echo "All data on "$1" now will be destroyed! Continue? [y/n]"
read ans
if ! [ $ans == 'y' ]
then
	exit
fi

echo "[Unmounting all existing partitions on the device ]"

umount $1*

echo "[Partitioning $1...]"

DRIVE=$1
dd if=/dev/zero of=$DRIVE bs=1024 count=1024 &>/dev/null
	 
SIZE=`fdisk -l $DRIVE | grep Disk | awk '{print $5}'`
	 
echo DISK SIZE - $SIZE bytes
 
CYLINDERS=`echo $SIZE/255/63/512 | bc`
 
echo CYLINDERS - $CYLINDERS
{
echo ,9,0x0C,*
echo ,$(expr $CYLINDERS / 2),,-
echo ,,0x0C,-
} | sfdisk -D -H 255 -S 63 -C $CYLINDERS $DRIVE &> /dev/null

echo "[Making filesystems...]"

mkfs.vfat -F 32 -n boot "$1"1 &> /dev/null
mkfs.ext3 -L rootfs "$1"2 &> /dev/null
mkfs.vfat -F 32 -n data "$1"3 &> /dev/null

echo "[Copying files...]"

mount "$1"1 /mnt
cp $2 /mnt/MLO
cp $3 /mnt/u-boot.bin
cp $4 /mnt/uImage
if [ "$7" ]
then
        echo "[Copying START_HERE floder to boot partition]"
        cp -r $7 /mnt/START_HERE
fi

umount "$1"1

mount "$1"2 /mnt
tar jxvf $5 -C /mnt --numeric-owner &> /dev/null
chmod 755 /mnt
chmod 777 /mnt/usr/bin/busybox
umount "$1"2

if [ "$6" ]
then
	echo "[Copying all clips to data partition]"
	mount "$1"3 /mnt
	cp -r $6/* /mnt/
	umount "$1"3
fi

echo "[Done]"
