#
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk
include $(INCLUDE_DIR)/host.mk

BOOTFS_DIR:=$(KDIR)/boot
BOOTFS_IMAGE:=$(KDIR)/boot.fat
KERNEL_IMG:=$(KDIR)/kernel.img

define Image/Prepare
	rm -f $(BOOTFS_IMAGE)
	rm -f $(KERNEL_IMG)
endef

define Image/BuildKernel
	$(CP) $(LINUX_DIR)/arch/$(ARCH)/boot/$(subst ",,$(KERNELNAME)) $(KERNEL_IMG)
endef

define Image/Build/squashfs
    $(call prepare_generic_squashfs,$(KDIR)/root.squashfs)
endef

CONFIG_TARGET_BOOTFS_PARTSIZE:=16

FATSIZE=$(shell echo $$(($(CONFIG_TARGET_BOOTFS_PARTSIZE)*1024)))

define Image/Build/fatfs
	$(MAKE) -C firmware "BOOTFS_DIR=$(BOOTFS_DIR)" compile
	$(CP) $(KERNEL_IMG) $(BOOTFS_DIR)
	@echo "TODO: generate cmdline.txt"
	$(CP) files/cmdline.txt $(BOOTFS_DIR)
	$(STAGING_DIR_HOST)/bin/genfatfs -b $(FATSIZE) -d $(BOOTFS_DIR) $(BOOTFS_IMAGE)
endef

define Image/Build/sdcard
	@echo "TODO: image/build/sdcard"
endef

define Image/Build
	$(call Image/Build/fatfs)
	$(call Image/Build/$(1))
	$(call Image/Build/sdcard)
	dd if=$(KDIR)/root.$(1) of=$(BIN_DIR)/$(IMG_PREFIX)-root.$(1) bs=128k conv=sync
	dd if=$(BOOTFS_IMAGE) of=$(BIN_DIR)/$(IMG_PREFIX)-boot.fat bs=128k conv=sync
endef

$(eval $(call BuildImage))
