#ifndef DRIVER_H
#define DRIVER_H
typedef struct {
	int (*start)();
	int (*stop)();
} driver_t;
int driver_start(driver_t * driver);
#endif
