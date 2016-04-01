#!/bin/sh

echo "Copying Google apps ..."

cd ../../out/target/product/beagleboard

rm -f -v system/app/Provision.*
rm -f -v system/app/QuickSearchBox.*

mkdir -p system/tmp
cd system/tmp
unzip -o ../../../../../../tcbin_misc/gapps_for_b2g_revc_update.zip system/* -d .
rsync -a system/* ../
cd -
rm -rf system/tmp

chmod 0755 system/addon.d/70-gapps.sh
find system/app/ -type d -exec chmod 755 {} +
find system/app/ -type f -exec chmod 644 {} +

cd ../../../../tcbin_misc/build_scripts
