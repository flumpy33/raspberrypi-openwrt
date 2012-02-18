#
# Copyright (C) 2010-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=arm
BOARD:=raspberrypi
BOARDNAME:=Raspberry Pi
FEATURES:=fpu ext4 targz usb audio
CFLAGS:=-O2 -pipe -mcpu=arm1176jzf-s -fno-caller-saves
MAINTAINER:=Nathan Huizinga <nathan.huizinga@gmail.com>

LINUX_VERSION:=3.1.9

include $(INCLUDE_DIR)/target.mk

define Target/Description
	Build images for Raspberry Pi (Model B) boards.
endef

KERNELNAME:="zImage"

$(eval $(call BuildTarget))
