#!/bin/sh

cd ../build_scripts/
./backupfs_build.sh
cd ../../
mkdir -p out/target/product/beagleboard/recovery/root/lib/modules/2.6.37/
cp -v tcbin_misc/syscheck/tests/syscheck out/target/product/beagleboard/recovery/root/system/bin/
cp -v tcbin_misc/syscheck/tests/gps_test out/target/product/beagleboard/recovery/root/system/bin/
cp -v tcbin_misc/syscheck/tests/twl3040_vib_test out/target/product/beagleboard/recovery/root/system/bin/
cp -v tcbin_misc/syscheck/configs/fstab.omap3beagleboard out/target/product/beagleboard/recovery/root/
cp -v tcbin_misc/syscheck/configs/init.rc out/target/product/beagleboard/recovery/root/
cp -v tcbin_misc/non_destructive_nandtest out/target/product/beagleboard/recovery/root/system/bin/
cp -v tcbin_misc/destructive_nandtest out/target/product/beagleboard/recovery/root/system/bin/
cp -v kernel/drivers/mtd/tests/mtd_subpagetest.ko out/target/product/beagleboard/recovery/root/usr/modules/
cp -v kernel/drivers/mtd/tests/mtd_readtest.ko out/target/product/beagleboard/recovery/root/usr/modules/
cd out/target/product/beagleboard/recovery
echo "Creating Testing filesystem"
../../../../../build/tools/mktarball.sh ../../../../host/linux-x86/bin/fs_get_stats root . testing testing.tar.bz2
cd ../../../../../
echo "Copying tar to image_folder"
mv ./out/target/product/beagleboard/recovery/testing.tar.bz2 ./image_folder/Filesystem/
echo "Creating Testing filesystem ... done."
cd tcbin_misc/syscheck/
