#!/bin/sh
cp ../../out/target/product/beagleboard/system/bin/updater update/META-INF/com/google/android/update-binary
cp ../../out/host/linux-x86/framework/signapk.jar .
cp ../../device/ti/beagleboard/security/release* .
