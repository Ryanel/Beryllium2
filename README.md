[Beryllium^2](id:btop)
=========

An actual serious attempt at a *NIX kernel; with a couple of tweaks to the old formula of course. And only the kernel.

This kernel is unique in the fact that it is a macrokernel that is transitioning into a microkernel. Well, I say unique, more like "special snowflake".

Ports (+ = priority)
---------
* x86 +
* ARM +
* Integrator-CP (qemu) +
* Raspberry Pi
* SPARC
* MIPS (Soon)
Features
---------
* IO Interrupt Messaging System
* Dynamic Kernel Level Timers
* Architecture independant
* Devices (try /dev/serial0!)
* Kernel Level Logging (minus the logging)
* Device Tree (along with starting and stopping devices)
* Regioned memory manager (in progress)

On Boot
---------
* Sets up low level stuff
* Enters kmain
* Sets up high level stuff
* Attempts to find /bin/init, /sbin/init, etc...
* Runs init

Branches ([top](#btop))
---------
+ master: Stable production safe code is stored here.

The release process generally is:

feature-branch -> master (release)

The kernels source is stored in the kernel directory, while utilites are stored in util/

Building ([top](#btop))
---------
* Prerequisites: clang, make, nasm
* Obtain the source code (conviently, you can get the latest code with git clone "https://github.com/Lionel07/Beryllium2.git"!)

Beryllium uses Clang as its default compiler, and i586-elf-ld for linking(which resides in the toolkit directory).
This is local to Beryllium^2, so it won't mess with the users setup. Use makebinutils.sh to build binutils.

* Run the appropreate makebinutils.sh . It does not require root privilages (and keeps to the toolkit directory). It automates the building of binutils for you. Might take a couple of minutes.
* `cd ../`
* `make all`

The kernel is stored in build/, along with the .iso generated, the generated symboltable and all build kernel mods.

* Optionally, run the kernel in qemu with `make run`
