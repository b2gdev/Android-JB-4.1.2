#!/bin/sh

cp -rPp tcbin_misc/recovery/usr/ out/target/product/beagleboard/recovery/root/
mkdir -p out/target/product/beagleboard/recovery/root/system/bin
ln -vs /usr/bin/sh out/target/product/beagleboard/recovery/root/system/bin/sh
cp -v kernel/drivers/accessibility/braille/metec/metec_flat20.ko out/target/product/beagleboard/recovery/root/usr/modules/
cp -v kernel/drivers/input/keyboard/cp430_keypad/cp430_keypad.ko out/target/product/beagleboard/recovery/root/usr/modules/
cp -v kernel/drivers/mfd/cp430-core/cp430_core.ko out/target/product/beagleboard/recovery/root/usr/modules/
cp -v kernel/drivers/misc/cp430_charger/cp430_charger.ko out/target/product/beagleboard/recovery/root/usr/modules/
