#
# Copyright (C) 2013
#
# This is free software, Licensed under the GNU General Public License v2.
# See /License for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=brcm2708-gpu-fw
PKG_VERSION:=20131102
PKG_RELEASE:=2
PKG_MAINTAINER:=J.Simons<flumpy33@gmail.com>
PKG_REV:=4c1456944b5f6cc9e5141077ed4e158398811fc1
PKG_MD5SUM:=1206f283f0f3cb3ca2d851cbcca2f72c

PKG_SOURCE_URL:=https://github.com/raspberrypi/firmware/archive
PKG_SOURCE:=$(PKG_REV).tar.gz
PKG_SOURCE_SUBDIR:=firmware-$(PKG_REV)
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)/$(PKG_SOURCE_SUBDIR)

include $(INCLUDE_DIR)/package.mk

define Package/brcm2708-gpu-fw
	SECTION:=boot
	CATEGORY:=Boot Loaders
	DEPENDS:=@TARGET_brcm2708
	TITLE:=brcm2708-gpu-fw
	DEFAULT:=y if (TARGET_brcm2708)
endef

define Package/brcm2708-gpu-fw/description
 GPU and kernel boot firmware for brcm2708.
endef

define Build/Compile
endef

define Build/InstallDev
	$(INSTALL_DIR) $(BUILD_DIR)/brcm2708-gpu-fw-boot
	$(CP) $(PKG_BUILD_DIR)/boot/* $(BUILD_DIR)/brcm2708-gpu-fw-boot
endef

$(eval $(call BuildPackage,brcm2708-gpu-fw))

