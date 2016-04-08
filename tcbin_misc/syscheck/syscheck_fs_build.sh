#!/bin/sh

cd ../build_scripts/
./backupfs_build.sh
cd ../../
mkdir -p out/target/product/beagleboard/recovery/root/lib/modules/2.6.37/
cp -v tcbin_misc/syscheck/tests/syscheck out/target/product/beagleboard/recovery/root/sbin/
cp -v tcbin_misc/syscheck/tests/gps_test out/target/product/beagleboard/recovery/root/sbin/
cp -v tcbin_misc/syscheck/tests/twl3040_vib_test out/target/product/beagleboard/recovery/root/sbin/
cp -v tcbin_misc/syscheck/tests/lm3553_test out/target/product/beagleboard/recovery/root/sbin/
rm -v out/target/product/beagleboard/recovery/root/fstab.omap3beagleboard
cp -v tcbin_misc/syscheck/configs/init.rc out/target/product/beagleboard/recovery/root/
cp -v tcbin_misc/syscheck/tests/non_destructive_nandtest out/target/product/beagleboard/recovery/root/sbin/
cp -v tcbin_misc/syscheck/tests/destructive_nandtest out/target/product/beagleboard/recovery/root/sbin/
cp -v tcbin_misc/syscheck/tests/fw_printenv out/target/product/beagleboard/recovery/root/sbin/
cp -v tcbin_misc/syscheck/tests/fw_setenv out/target/product/beagleboard/recovery/root/sbin/
cp -v kernel/drivers/mtd/tests/mtd_subpagetest.ko out/target/product/beagleboard/recovery/root/usr/modules/
cp -v kernel/drivers/mtd/tests/mtd_readtest.ko out/target/product/beagleboard/recovery/root/usr/modules/
cp -v kernel/drivers/misc/lm3553/lm3553.ko out/target/product/beagleboard/recovery/root/usr/modules/
mkdir -p out/target/product/beagleboard/recovery/root/system/bin/
cp -r out/target/product/beagleboard/system/bin/* out/target/product/beagleboard/recovery/root/system/bin/
mkdir -p out/target/product/beagleboard/recovery/root/system/lib/
cp -r out/target/product/beagleboard/system/lib/* out/target/product/beagleboard/recovery/root/system/lib/
cd out/target/product/beagleboard/recovery
echo "Creating Testing filesystem"
../../../../../build/tools/mktarball.sh ../../../../host/linux-x86/bin/fs_get_stats root . testing testing.tar.bz2
cd ../../../../../
echo "Copying tar to image_folder"
mv ./out/target/product/beagleboard/recovery/testing.tar.bz2 ./image_folder/Filesystem/
echo "Creating Testing filesystem ... done."
cd tcbin_misc/syscheck/
