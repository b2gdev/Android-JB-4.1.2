# Default target is omap3evm/AM37xevm
TARGET_PRODUCT ?= omap3evm

# Use rowboat kernel to build against
KLIB ?= $(ANDROID_ROOT_DIR)/kernel
KLIB_BUILD := $(KLIB)

# Use android toolchain
CROSS_COMPILE=$(ANDROID_ROOT_DIR)/$(TOOLS_PREFIX)

# Install the compat drivers to the specified $TARGET_PRODUCT only
DESTDIR?=$(ANDROID_ROOT_DIR)/out/target/product/$(TARGET_PRODUCT)

ifneq ($(KERNELRELEASE),)

NOSTDINC_FLAGS := -I$(M)/include/ \
	-include $(M)/include/linux/compat-2.6.h \
	$(CFLAGS)

obj-y := compat/


ifeq ($(BT),)
obj-$(CONFIG_COMPAT_WIRELESS) += net/wireless/ net/mac80211/
obj-$(CONFIG_COMPAT_WIRELESS_MODULES) += drivers/net/wireless/

endif
else

export PWD :=	$(shell pwd)
CFLAGS += \
        -DCOMPAT_BASE_TREE="\"$(shell cat compat_base_tree)\"" \
        -DCOMPAT_BASE_TREE_VERSION="\"$(shell cat compat_base_tree_version)\"" \
        -DCOMPAT_PROJECT="\"Compat-wireless\"" \
        -DCOMPAT_VERSION="\"$(shell cat compat_version)\""

# These exported as they are used by the scripts
# to check config and compat autoconf
export COMPAT_CONFIG=config.mk
export CONFIG_CHECK=.$(COMPAT_CONFIG)_md5sum.txt
export COMPAT_AUTOCONF=include/linux/compat_autoconf.h
export CREL=$(shell cat $(PWD)/compat_version)
export CREL_PRE:=.compat_autoconf_
export CREL_CHECK:=$(CREL_PRE)$(CREL)

include $(PWD)/$(COMPAT_CONFIG)

all: modules

modules:
	@./scripts/check_config.sh
	$(MAKE) -j1 -C $(KLIB_BUILD) M=$(PWD) modules
	@touch $@

install: modules
	@echo "Installing WL12xx compat driver..."
	@mkdir -p $(DESTDIR)/system/lib/modules
	@install compat/compat.ko                              $(DESTDIR)/system/lib/modules
	@install drivers/net/wireless/ti/wl12xx/wl12xx.ko      $(DESTDIR)/system/lib/modules
	@install drivers/net/wireless/ti/wl18xx/wl18xx.ko      $(DESTDIR)/system/lib/modules
	@install net/mac80211/mac80211.ko                      $(DESTDIR)/system/lib/modules
	@install net/wireless/cfg80211.ko                      $(DESTDIR)/system/lib/modules
	@install drivers/net/wireless/ti/wlcore/wlcore.ko      $(DESTDIR)/system/lib/modules
	@install drivers/net/wireless/ti/wlcore/wlcore_sdio.ko $(DESTDIR)/system/lib/modules

clean:
	@if [ -d net -a -d $(KLIB_BUILD) ]; then \
		$(MAKE) -C $(KLIB_BUILD) M=$(PWD) clean ;\
	fi
	@rm -f $(CREL_PRE)*

.PHONY: all clean install modules

endif
