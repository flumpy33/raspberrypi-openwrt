# 
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=genfatfs
PKG_VERSION:=0.1
PKG_BUILD_DEPENDS:=cmake

include $(INCLUDE_DIR)/host-build.mk

define Host/Prepare
    mkdir -p $(HOST_BUILD_DIR)
	$(CP) ./src/* $(HOST_BUILD_DIR)/
	$(call Host/Patch)
endef

define Host/Configure
	(cd $(HOST_BUILD_DIR); \
		cmake \
		. \
	)
endef

define Host/Install
	install -m0755 $(HOST_BUILD_DIR)/genfatfs $(STAGING_DIR_HOST)/bin/
endef

define Host/Clean
	rm -f $(STAGING_DIR_HOST)/bin/genfatfs
endef

$(eval $(call HostBuild))
