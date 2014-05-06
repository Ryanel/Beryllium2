[Beryllium](id:btop)
=========

An actual serious attempt at a *NIX kernel; with a couple of tweaks to the old formula of course. And only the kernel.

This kernel is unique in the fact that it is a macrokernel that is transitioning into a microkernel. Well, I say unique, more like "special snowflake".

Unique Features
---------
* IO Interrupt Messaging System
* 

Branches ([top](#btop))
---------
+ master: Stable production safe code is stored here.
+ dev: The latest and most up to date code is stored here. May not be working or stable
+ arm: ARM code and features are put here and tested, before being merged into dev.
+ staging: (Sometimes) holds code that is being prepared for merging into master.
+ *: Feature branches, implementing different features. They get merged into dev and deleted.

The release process generally is:

feature-branch -> dev/arm -> staging -> master (release)

This kernel's source has organisation. To understand this organisation to effectively navigate the code, please look at src/README.md

Building ([top](#btop))
---------
* Prerequisites: clang, make, nasm
* Obtain the source code (conviently, you can get the latest code with git clone "https://github.com/Lionel07/Beryllium.git"!)

Now, Beryllium comes with it's own cross compiler (well, binutils, but you can also build clang too), which resides in the toolkit directory.
This is local to beryllium, so it won't mess with the users setup. There are two options, makebinutils.sh or makebinutils-pi.sh. 
Don't worry about the other files, they are there for legacy reasons.
Use makebinutils.sh if you are just trying to the compile the kernel for x86, or makebinutils-pi.sh for building for Raspberry Pi (arm).

* Run the appropreate makebinutils.sh . It does not require root privilages (and keeps to the toolkit directory). It automates the building of binutils for you. Might take a couple of minutes.
* `cd ../`
* `make all`
* You now have made the kernel!
* Optionally, run the kernel in qemu with `make run`

Kernel Options ([top](#btop))
---------
In the makefile, there is a section labeled "COMPILE_OPTIONS :=". This section is for inputting switches that can change some parts of what is compiled or not.

An example is the defaults: `-D DEBUG -D ENABLE_SERIAL`

DEBUG enables debug features, while ENABLE_SERIAL, well, enables serial. Each must be prefixed with `-D`, because these options are passed raw into `${CC}`

In progress...
Will be on the wiki soon!
###Speaking of wiki's? ([top](#btop))
[There infact is a wiki](https://beryllium.simplyelementary.x10.mx). Please contribute!
