# Introduction #

This overview will contain some information about the hardware which is used on the
Raspberry Pi (R-Pi for short) board. The R-Pi board is a credit card sized board with PC like capabilities.
There will be 2 models brought on the market Model A ($25,-) and B ($35,-). See the following list
for the difference between the two Models.

**Model A**

  1. ~~128Mb RAM~~
  1. 256Mb RAM
  1. one USB
  1. _**No** Ethernet port_
**Model B**

  1. ~~256Mb RAM~~
  1. 512Mb RAM (**new)
  1. two USB
  1. _Ethernet port_**




# Details #

The R-Pi is build around the Broadcom BCM2835 [SoC](http://en.wikipedia.org/wiki/System_on_a_chip). It's an ARM 11 architecture running at 700MHz.
For more information about the ARM 11 architecture see [ARM 11 page](http://www.arm.com/products/processors/classic/arm11/arm1176.php).
The SoC will have a Videocore 4 GPU, which should be capable of H.264 Playback (Blu-Ray). OpenGL ES2.0 and OpenVG support will be made available through libraries.

**List of hardware
  1. Broadcom BCM2835 SoC
  1. USB 2.0
  1. HDMI
  1. Ethernet port (Model B only)
  1. Audio Jack
  1. RCA output
  1. SD card connector (up to 32GB)
  1. GPIO  (not mounted)
  1. Status LEDs
  1. DSI & CSI connector (not mounted)
  1. JTAG interface (not mounted)**

## Booting ##
The boot order is predefined on the SoC, this code is executed from ROM and cannot be changed. The
GPU will open the bootcode.bin which is stored on the SD card within a FAT partition (FAT32). This will
setup the SDRAM and loads loader.bin. The loader will read the start.elf file which contains the GPU
firmware. Then start.elf will read the config and cmdline text files and starts the kernel (kernel.img).

After that Linux is in control.

For more information about raspberry pi booting see: https://github.com/dwelch67/raspberrypi