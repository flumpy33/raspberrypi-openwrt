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
PKG_REV:=be61355f9e7e28a5fa83e529454d51e90745bb4b
PKG_VERSION:=20120722
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
	$(CP) $(PKG_BUILD_DIR)/boot/arm128_start.elf $(BOOTFS_DIR)
	$(CP) $(PKG_BUILD_DIR)/boot/arm192_start.elf $(BOOTFS_DIR)
	$(CP) $(PKG_BUILD_DIR)/boot/arm224_start.elf $(BOOTFS_DIR)
	$(CP) $(PKG_BUILD_DIR)/boot/bootcode.bin $(BOOTFS_DIR)
	$(CP) $(PKG_BUILD_DIR)/boot/COPYING.linux $(BOOTFS_DIR)
	$(CP) $(PKG_BUILD_DIR)/boot/LICENCE.broadcom $(BOOTFS_DIR)
	$(CP) $(PKG_BUILD_DIR)/boot/loader.bin $(BOOTFS_DIR)
	$(CP) $(PKG_BUILD_DIR)/boot/start.elf $(BOOTFS_DIR)
endef

$(eval $(call Build/DefaultTargets))
