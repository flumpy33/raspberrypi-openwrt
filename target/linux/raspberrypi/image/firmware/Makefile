# 
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk
include $(INCLUDE_DIR)/host.mk

PKG_NAME:=raspberrypi_gpu_firmware
PKG_REV:=0671d60180c8d10978b442de5ec9d083596a5f3f
PKG_VERSION:=20120701
PKG_RELEASE:=1
PKG_MIRROR:=0

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=git://github.com/raspberrypi/firmware.git
PKG_SOURCE_PROTO:=git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_MIRROR:=1

include $(INCLUDE_DIR)/package.mk

define Build/Compile

endef

define Build/InstallDev
	rm -rf $(BOOTFS_DIR)
	mkdir -p $(BOOTFS_DIR)
	$(CP) $(PKG_BUILD_DIR)/boot/arm128_start.elf $(BOOTFS_DIR)
	$(CP) $(PKG_BUILD_DIR)/boot/arm192_start.elf $(BOOTFS_DIR)
	$(CP) $(PKG_BUILD_DIR)/boot/arm224_start.elf $(BOOTFS_DIR)
	$(CP) $(PKG_BUILD_DIR)/boot/bootcode.bin $(BOOTFS_DIR)
	$(CP) $(PKG_BUILD_DIR)/boot/COPYING.linux $(BOOTFS_DIR)
	$(CP) $(PKG_BUILD_DIR)/boot/LICENCE.broadcom $(BOOTFS_DIR)
	$(CP) $(PKG_BUILD_DIR)/boot/loader.bin $(BOOTFS_DIR)
	$(CP) $(PKG_BUILD_DIR)/boot/start.elf $(BOOTFS_DIR)
endef

define Build/Clean
	rm -f $(BOOTFS_DIR)
endef

$(eval $(call Build/DefaultTargets))
