#ifndef TIMER_H
#define TIMER_H
#include <log.h>
#include <types.h>
///Send timer subsystem a tick from device device_id
void timer_recieveTick(uint32_t device_id);
///Gets hi-tick
uint32_t timer_getHi();
///Gets low-tick
uint32_t timer_getLow();
///Gets device id
uint32_t timer_getDevice();
///Gets device name
const char* timer_getDeviceString(uint32_t device);
///Gets current active timer resolution
uint32_t timer_getDeviceRes();
///Gets current uptime in seconds
uint32_t timer_getUptime();
#endif
