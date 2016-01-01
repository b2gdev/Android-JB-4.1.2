#!/bin/sh

cd ../../
cp -rPp tcbin_misc/recovery/* out/target/product/beagleboard/recovery/root/
mkdir -p out/target/product/beagleboard/recovery/root/system/bin
ln -vs /usr/bin/sh out/target/product/beagleboard/recovery/root/system/bin/sh
echo "The symlink here to sh was added to support adb shell, which expects sh to be present at this location" > out/target/product/beagleboard/recovery/root/system/bin/README
cp -v tcbin_misc/executables/format_emmc out/target/product/beagleboard/recovery/root/sbin/
cp -v tcbin_misc/executables/nuke_emmc out/target/product/beagleboard/recovery/root/sbin/
cp -v tcbin_misc/executables/clean_emmc out/target/product/beagleboard/recovery/root/sbin/
cp -v tcbin_misc/executables/mount_emmc out/target/product/beagleboard/recovery/root/sbin/
cp -v tcbin_misc/executables/getMSP430Version out/target/product/beagleboard/recovery/root/sbin/
cp -v tcbin_misc/executables/getAllFWVersions out/target/product/beagleboard/recovery/root/sbin/
cp -v tcbin_misc/executables/setup_3g_audio out/target/product/beagleboard/recovery/root/sbin/
cp -v kernel/drivers/accessibility/braille/metec/metec_flat20.ko out/target/product/beagleboard/recovery/root/usr/modules/
cp -v kernel/drivers/input/keyboard/cp430_keypad/cp430_keypad.ko out/target/product/beagleboard/recovery/root/usr/modules/
cp -v kernel/drivers/mfd/cp430-core/cp430_core.ko out/target/product/beagleboard/recovery/root/usr/modules/
cp -v kernel/drivers/misc/cp430_charger/cp430_charger.ko out/target/product/beagleboard/recovery/root/usr/modules/
cp -v kernel/drivers/power/tcbin_power/cp430_power/cp430_power.ko out/target/product/beagleboard/recovery/root/usr/modules/
cd tcbin_misc/build_scripts
