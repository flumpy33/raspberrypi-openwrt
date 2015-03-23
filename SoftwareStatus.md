# Status #

## What is working ##
  * The Raspberry Pi architecture has been added as a target to OpenWrt.
  * The toolchain is configured according to the Rpi architecture.
  * It builds a Linux kernel version 3.1.19 **with** the Rpi patches from [here](https://github.com/raspberrypi/linux).
  * A standard root file system is generated (Ext4 image).
  * The SD card image is now being generated (check the git repository).
  * The generated image boots on the Raspberry Pi hardware! (Currently from the "kernel\_rework\_patches" git branch).

## TODO ##
  * Populate the root file system with the required kernel modules.