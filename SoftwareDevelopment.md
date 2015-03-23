# Software development environment #

For developing software you will need some kind of development environment, which contains all the tools for compiling and building the software. [OpenWrt](https://openwrt.org/) can be installed on any machine running Linux, real or virtual.

## Linux distribution ##

Any Linux distribution will do. For example [Linux Mint 12 "Lisa"](http://linuxmint.com/).

But depending on your choice you need to install one or more of the following (Ubuntu based) packages:
  * git
  * g++
  * flex
  * zlib1g-dev
  * libncurses5-dev

`sudo aptitude install <package>`

_Optional:_
  * patchutils

## Sources ##

Get the sources as described on the [checkout](http://code.google.com/p/raspberrypi-openwrt/source/checkout) page.