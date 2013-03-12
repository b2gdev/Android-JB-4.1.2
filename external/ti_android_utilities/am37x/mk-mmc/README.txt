README for preparing SD/MMC/Micro-SD CARD for booting Android

Pre-Requesites
--------------
1) Need to have an SD/MMC/Micro-SD card with atleast 2GB of size.
2) The script needs to be invoked from ubuntu linux machine 8.04 or above.
3) User needs to have sudo privileges.

What script will do
-------------------
The mkmmc-android.sh partitions the MMC/SD card into three partiions namely boot, rootfs and data.
The script will then put the boot images on boot partition and extracts the android rootfs-rootfs_*.tar.bz2 to rootfs partition.
Finally the script will copy the Media clips to the data partition and START_HERE folder to boot partition.

How to invoke the script
------------------------
There are three ways of invoking this script.

1)
Command: sudo ./mkmmc-android <device> <MLO> <u-boot.bin> <uImage> <boot.scr> <rootfs tar.bz2 > <Media_Clips> <START_HERE Location>
Example: sudo ./mkmmc-android /dev/sdc MLO u-boot.bin uImage boot.scr rootfs.tar.bz2 Media_Clips
Details: In this case, the script will take Boot Images (MLO,u-boot.bin, uImage and boot.scr) Root Filesystem tarball, Media Clips and START_HERE folder as arguements.
         The script will then put the boot images on boot partition, extract the android rootfs-rootfs_*.tar.bz2 to rootfs partition.
         Finally the script will copy the Media clips to the data partition and START_HERE folder to boot partition.
2)
Command: sudo ./mkmmc-android <device> <MLO> <u-boot.bin> <uImage> <boot.scr> <rootfs tar.bz2 >
Example: sudo ./mkmmc-android /dev/sdc MLO u-boot.bin uImage boot.scr rootfs.tar.bz2
Details: In this case, the script will take Boot Images (MLO,u-boot.bin, uImage and boot.scr) and Root Filesystem tarball as arguements.
         The script will then put the boot images on boot partition, extract the android rootfs-rootfs_*.tar.bz2 to rootfs partition.
         The data partiton will be left empty in this case.
3)
Command: sudo ./mkmmc-android.sh <device>
Example: sudo ./mkmmc-android.sh /dev/sdc
Details: In this case, the script will assume default locations for BootImages, Root Filesystem, Media_Clips and START_HERE. This command is equivalent to the following
         sudo ./mkmmc-android /dev/sdc Boot_Images/MLO Boot_Images/u-boot.bin Boot_Images/uImage Boot_Images/boot.scr Filesystem/rootfs.tar.bz2 Media_Clips START_HERE
         If you are in a particular Board Specific Directory, extracted from DevKit Release, then you can prepare the sd card by executing
         sudo ./mkmmc-android.sh <device>, to prepare sd card.

Warning
-------
Provide the right device name after the script name when you invoke the script. if you get this wrong it can wipe your HDD.
