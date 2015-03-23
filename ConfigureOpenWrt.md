# Configure OpenWrt #

Before you can start building the software, you need to configure the OpenWrt build environment. An example configuration can be found in the following directory:
`configs/config-rpi`

Copy this file to the current directory and name it ".config":
```
cp configs/config-rpi .config
```

Now you will have a pre-configured environment for building a bare bone Raspberry Pi kernel and root file system.

## Configuration menu ##

Run:
```
make menuconfig
```
to enter the configuration menus, to change settings or just to have a look around.

# Make World #

Execute the following command to start building:
```
make world
```

or

```
make world V=99
```

to see a detailed progress of the build process.

The downloading and building of the different software packages takes some time, depending on the performance of your Linux system. Remember that almost everything is being built from scratch:
  * toolchain (binutils, eglibc, gcc, etc)
  * Linux kernel
  * busybox and other packages
  * root filesystem

On a decent _real_ Linux machine, a build will take about 2 hours. On a virtual one, the build can take several hours more.

So be patient...