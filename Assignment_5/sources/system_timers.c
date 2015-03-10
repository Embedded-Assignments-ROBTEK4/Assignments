#include "../headers/system_timers.h"


static INT16U timers[MAX_TIMERS];
static bool allocated[MAX_TIMERS];

void stop_timer(INT8U id)
{
	timers[id] = 0;
}
void count_timer(INT8U id)
{
	if(timers[id])
		timers[id]--;
}

void start_timer(INT8U id, INT16U timer_val)
{
	timers[id] = timer_val;
}

bool is_timer_finished(INT8U id)
{
	return !timers[id];
}

INT8U request_timer(void)
{
	INT8U returnid = 0xFF;
	for(INT8U i = 0; i < MAX_TIMERS && returnid == 0xFF; i++)
	{
		if(!allocated[i])
		{
			allocated[i] = true;
			returnid = i;
		}
	}
	return returnid;
}

void release_timer(INT8U id)
{
	allocated[id] = false;
}
