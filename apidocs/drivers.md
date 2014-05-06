#Driver Interface
The driver interface allows for initialisation of hardware & software drivers in a consitant manner.

## driver_t
_driver_t_ defines a type that encapsulates a generic driver that can be started or stopped by the kernel. These are technically kernel modifications (kmods), but are built in to the kernel. The data structure defines several things:

* A way to start, stop, and send messages to drivers
* Essential driver data
* Driver Status
* The name of the Driver
* What the driver implements
* The Drivers' BMR (Base Memory Region)

##Avalable Status's

* ONLINE  (Working)
* OFFLINE (Not Loaded)
* ABORTED (Broken)
* DEAD    (Not responding)
* UNSUPPORTED (Driver failed to load because it doesn't work on this version)

##Base Memory Region
Every driver CAN have a "Base Memory Region", which is a pointer to an area of memory for driver usage. The driver does NOT have to use this memory, or even follow this guideline: it's simply a standard option

##Functions
### start()
Starts a driver
### stop()
Stops a driver
### recieve
Sends a driver_msg_t to the driver
