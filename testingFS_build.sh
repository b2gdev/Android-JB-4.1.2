#!/bin/sh

./backupfs_build.sh
cd out/target/product/beagleboard/recovery
echo "Creating Testing filesystem"
../../../../../build/tools/mktarball.sh ../../../../host/linux-x86/bin/fs_get_stats root . testing testing.tar.bz2
cd ../../../../../
echo "Copying tar to image_folder"
mv ./out/target/product/beagleboard/recovery/testing.tar.bz2 ./image_folder/Filesystem/
echo "Creating Testing filesystem ... done."
