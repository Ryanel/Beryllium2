#ifndef WATCHDOG_H
#define WATCHDOG_H

#define WD_HU_LIMIT           0x500 //2048 hang ups

#define WD_NOTIFY_KMAIN       0x10
#define WD_HU_LIMIT_KMAIN     200

void wd_notify(int source);
int wd_get_kmain_hangups();
int wd_get_hangups();
void wd_disable();
void wd_enable();
void wd_init();
#endif
