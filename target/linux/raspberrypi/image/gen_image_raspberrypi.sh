#!/usr/bin/env bash
#
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

set -x 
[ $# == 5 ] || {
    echo "SYNTAX: $0 <file> <fatfs size> <fatfs image> <rootfs size> <rootfs image>"
    exit 1
}

OUTPUT="$1"
FATFSSIZE="$2"
FATFSIMAGE="$3"
ROOTFSSIZE="$4"
ROOTFSIMAGE="$5"

rm -f "$OUTPUT"

head=4
sect=16
blksize=512

# create partition table
set `ptgen -o "$OUTPUT" -h $head -s $sect -t c -p ${FATFSSIZE}m -t 83 -p ${ROOTFSSIZE}m`

FATFSOFFSET="$(($1 / $blksize))"
FATFSSIZE="$(($2 / $blksize))"
ROOTFSOFFSET="$(($3 / $blksize))"
ROOTFSSIZE="$(($4 / $blksize))"

# apply padding, if needed
[ -n "$PADDING" ] && dd if=/dev/zero of="$OUTPUT" bs="$blksize" seek="$ROOTFSOFFSET" conv=notrunc count="$ROOTFSSIZE"
dd if="$ROOTFSIMAGE" of="$OUTPUT" bs="$blksize" seek="$ROOTFSOFFSET" conv=notrunc
dd if="$FATFSIMAGE" of="$OUTPUT" bs="$blksize" seek="$FATFSOFFSET" conv=notrunc

