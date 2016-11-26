#Rules.make

################# FIELDS MODIFIABLE BY THE USER ###############################
#set home area HOME (relative location for all SDK operations)
HOME=$(ANDROID_ROOT_DIR)

#Current Directory where Graphics SDK is installed
GRAPHICS_INSTALL_DIR=$(ANDROID_ROOT_DIR)/hardware/ti/sgx

#Path of Android Framework
ANDROID_ROOT=$(ANDROID_ROOT_DIR)

#set toolchain root path for arm-eabi
CSTOOL_DIR=$(ANDROID_ROOT_DIR)/prebuilts/gcc/linux-x86/arm/arm-eabi-4.6
CSTOOL_PREFIX=arm-eabi-
CSTOOL_PATH=$(CSTOOL_DIR)/bin


#set the kernel installation path
KERNEL_INSTALL_DIR=$(ANDROID_ROOT_DIR)/kernel

#set filesystem path
TARGETFS_INSTALL_DIR=$(ANDROID_ROOT_DIR)/out/target/product/$(TARGET_PRODUCT)/

#Android 
ANDROID_DISCIMAGE=$(TARGETFS_INSTALL_DIR)/
DISCIMAGE=$(ANDROID_DISCIMAGE)


######################### PRE-DEFINED VARIABLES ###############################
######################## NOT MODIFIABLE BY USER ###############################

EURASIAROOT=$(GRAPHICS_INSTALL_DIR)

#Android Specific

GFX_REL_ES5_LIB_SRCPATH_ANDROID=$(GRAPHICS_INSTALL_DIR)/gfx_rel_es5.x_android
GFX_REL_ES4_LIB_SRCPATH_ANDROID=$(GRAPHICS_INSTALL_DIR)/gfx_rel_es4.x_android

#End Android Specific

GFX_DEVMEM2_SRCPATH = $(GRAPHICS_INSTALL_DIR)/tools/devmem2
GFX_KERNMOD_SRCPATH=$(GRAPHICS_INSTALL_DIR)
