#!/bin/sh

echo "Copying precompiled debug components ..."

cd ../../out/target/product/beagleboard

mkdir -p system/usr/bin/

cp -rfd ../../../../tcbin_misc/busybox/* system/usr/bin/
cp ../../../../tcbin_misc/executables/installotapackage system/usr/bin/
cp ../../../../tcbin_misc/executables/format_emmc system/usr/bin/
cp ../../../../tcbin_misc/executables/nuke_emmc system/usr/bin/
chmod -R +x system/usr/bin/
chmod 777 system/usr/bin/busybox

cd ../../../../tcbin_misc/build_scripts
