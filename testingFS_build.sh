#!/bin/sh

./backupfs_build.sh
mkdir -p out/target/product/beagleboard/recovery/root/lib/modules/2.6.37/
cp -v tcbin_misc/syscheck out/target/product/beagleboard/recovery/root/system/bin/
cp -v tcbin_misc/non_destructive_nandtest out/target/product/beagleboard/recovery/root/system/bin/
cp -v tcbin_misc/destructive_nandtest out/target/product/beagleboard/recovery/root/system/bin/
cp -v tcbin_misc/gps_test out/target/product/beagleboard/recovery/root/system/bin/
cp -v tcbin_misc/twl3040_vib_test out/target/product/beagleboard/recovery/root/system/bin/
cp -v tcbin_misc/testing.fstab.omap3beagleboard out/target/product/beagleboard/recovery/root/fstab.omap3beagleboard
cd out/target/product/beagleboard/recovery
echo "Creating Testing filesystem"
../../../../../build/tools/mktarball.sh ../../../../host/linux-x86/bin/fs_get_stats root . testing testing.tar.bz2
cd ../../../../../
echo "Copying tar to image_folder"
mv ./out/target/product/beagleboard/recovery/testing.tar.bz2 ./image_folder/Filesystem/
echo "Creating Testing filesystem ... done."
