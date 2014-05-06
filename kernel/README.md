The Beryllium src/ Directory Map
====================

A word of warning, this map may not be as up to date as you would want it to. And by that, I mean that this map might reflect changes that haven't happened yet. If you wish to contribute to the sorting, please do! The tutorial for getting started with building this kernel is in the root directory.

src/
---------
Contains configuration files, as well as architecture and enviroment agnostic files, such as the terminal, klog, etc...

src/boot/
---------
Contains architecture agnostic boot-time files. 

Boot files for specific architectures are stored in there respective ./boot directory, such as src/x86/boot/

src/drivers/
---------
Contains configuration and source files for architecture agnostic in-kernel drivers.

Drivers for specific architectures are stored in there respective ./drivers directory.

src/includes/
---------
Contains all header files. They follow the same path, minus src/ (the driver for architecure-agnostic serial is stored in includes/drivers/serial.h)

src/lib/
---------
Contains any generic components that aren't geared towards one thing.

src/x86/
---------
Contains configuration and source files for the x86 architecture. Follows the formatting of /src.
### Warning
This is compilied for i586-elf! This means that the resulting binary is not for x86_64. See x86_64.

src/x86/x64/
---------
Contains configuration and source files for the x86_64 architecture. Mainly command switches and small bits of custom code for 64-bit support.

src/arm/
---------
Contains configuration and source files for the ARM architecture. Follows the formatting of /src.

src/arm/[platform]
---------
Contains configuration and source files tailored for that specific platform, such as the Raspberry Pi. Inherits and mutes everything in src/arm/