#include <mutex.h>
#include <types.h>
void mutex_lock(mutex_t *mutex)
{
	if (mutex)
	{
		while(mutex)
		{
			#if X86
			asm volatile("pause");
			#else

			#endif
		}
	}
	mutex = (mutex_t*)1;
}

void mutex_unlock(mutex_t *mutex)
{
	if (mutex)
	{
		mutex = 0;
	}
	else
	{
		//mutex->value = false;
	}
}

void mutex_init(mutex_t *mutex)
{
	if(mutex != 0)
	{
		if(mutex != NULL)
		{
			mutex = 0;
		}
	}
}
