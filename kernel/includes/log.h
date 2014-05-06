#ifndef LOG_H
#define LOG_H
///A critical error
#define LOG_PANIC 0
///A severe non-recoverable error
#define LOG_SEVERE 1
///A recoverable error
#define LOG_ERROR 2
///A warning
#define LOG_WARN 3
///Normal information
#define LOG_INFO 4
//Debug and non-essential information
#define LOG_DEBUG 10

#define LOG_OK 5
#define LOG_FAIL 6
/**
Writes to the kernel log.
**/
void klog(int mode, const char *title, const char *fmt, ...);
/**
Sets the log mask, aka if mode is lower priority that MODE, then it is not outputted.
**/
void klog_setmask(int mode);
#endif
