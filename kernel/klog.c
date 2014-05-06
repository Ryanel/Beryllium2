#include <log.h>
#include <stdarg.h>
#include <stdio.h>
#include <beryllium/video.h>
#include <beryllium/timer.h>
int klog_mask = LOG_DEBUG;
static int background = 0;

void klog(int mode, const char *title, const char *fmt, ...)
{
	if( mode <= klog_mask)
	{
		video_setattributetext(0x0,0xE);
		#ifdef KLOG_TITLE_TIME
		printf("[%08d]:",timer_getHi());
		#else
		if(mode == LOG_OK)
		{
			printf("[OKAY]");
		}
		else if (mode == LOG_FAIL)
		{
			printf("[FAIL]");
		}
		else
		{
			printf("%s: ",title);
		}
		#endif
		video_resetattributetext();
		va_list args;
		va_start(args, fmt);
		vprintf(fmt,args);
		va_end(args);
		
	}
}

void klog_setmask(int mode)
{
	klog_mask = mode;
}
