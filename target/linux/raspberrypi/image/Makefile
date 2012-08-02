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
KERNEL_IMG_NAME:=kernel.img
KERNEL_IMG:=$(KDIR)/$(KERNEL_IMG_NAME)

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
	rm -rf $(BOOTFS_DIR)
	mkdir -p $(BOOTFS_DIR)
	$(MAKE) -C firmware "BOOTFS_DIR=$(BOOTFS_DIR)" compile
	$(CP) $(KERNEL_IMG) $(BOOTFS_DIR)
	@echo "TODO: generate cmdline.txt"
	$(CP) files/cmdline.txt $(BOOTFS_DIR)
#	$(STAGING_DIR_HOST)/bin/genfatfs -b $(FATSIZE) -d $(BOOTFS_DIR) $(BOOTFS_IMAGE)

	mkdosfs $(BOOTFS_IMAGE) -C $(FATSIZE)
	mcopy -i $(BOOTFS_IMAGE) $(BOOTFS_DIR)/arm128_start.elf ::
	mcopy -i $(BOOTFS_IMAGE) $(BOOTFS_DIR)/arm192_start.elf ::
	mcopy -i $(BOOTFS_IMAGE) $(BOOTFS_DIR)/arm224_start.elf ::
	mcopy -i $(BOOTFS_IMAGE) $(BOOTFS_DIR)/bootcode.bin ::
	mcopy -i $(BOOTFS_IMAGE) $(BOOTFS_DIR)/COPYING.linux ::
	mcopy -i $(BOOTFS_IMAGE) $(BOOTFS_DIR)/LICENCE.broadcom ::
	mcopy -i $(BOOTFS_IMAGE) $(BOOTFS_DIR)/loader.bin ::
	mcopy -i $(BOOTFS_IMAGE) $(BOOTFS_DIR)/arm128_start.elf ::start.elf
	mcopy -i $(BOOTFS_IMAGE) $(BOOTFS_DIR)/cmdline.txt ::
	mcopy -i $(BOOTFS_IMAGE) $(BOOTFS_DIR)/$(KERNEL_IMG_NAME) ::
endef

define Image/Build/sdcard
	./gen_image_raspberrypi.sh $(BIN_DIR)/$(IMG_PREFIX)-sdcard.img $(CONFIG_TARGET_BOOTFS_PARTSIZE) $(BOOTFS_IMAGE) $(CONFIG_TARGET_ROOTFS_PARTSIZE) $(1)
endef

define Image/Build
	$(call Image/Build/fatfs)
	$(call Image/Build/$(1))
	$(call Image/Build/sdcard,$(KDIR)/root.$(1))
	dd if=$(KDIR)/root.$(1) of=$(BIN_DIR)/$(IMG_PREFIX)-root.$(1) bs=128k conv=sync
	dd if=$(BOOTFS_IMAGE) of=$(BIN_DIR)/$(IMG_PREFIX)-boot.fat bs=128k conv=sync
endef

$(eval $(call BuildImage))
