#include "../headers/clock.h"
void clock_tick(time *time_s)
{
	if(++time_s->sec == 60)
	{
		time_s->sec = 0;
		if(++time_s->min == 60)
		{
			time_s->min = 0;
			if(++time_s->hour == 24)
				time_s->hour = 0;
		}
	}
}
void clock_count_min(time *time_s, bool direction)
{
	if(direction)
		if(time_s->min == 59)
			time_s->min = 0;
		else
			time_s->min++;
	else
	{
		if(time_s->min == 0)
			time_s->min = 59;
		else
			time_s->min--;
	}
}

void clock_count_hour(time *time_s, bool direction)
{
	if(direction)
		if(time_s->hour == 23)
			time_s->hour = 0;
		else
			time_s->hour++;
	else
	{
		if(time_s->hour == 0)
			time_s->hour = 23;
		else
			time_s->hour--;
	}
}

time clock(event button1_ev, event button2_ev)
{
	static time time_s = {0, 0, 0};
	static INT32U timer = SECOND_LENGTH / TIMEOUT_SYSTICK;
	static clock_state state = NORMAL;
	
	switch(state)
	{
		case NORMAL:
			if(timer > 0)
				timer--;
				
			if(button1_ev == LONG_PRESS or button2_ev == LONG_PRESS) state = SET_MIN;
			else if(timer == 0)
			{
				timer = SECOND_LENGTH / TIMEOUT_SYSTICK;
				clock_tick(&time_s);
			}
		break;
		
		case SET_MIN:
			if(button1_ev == SINGLE_PRESS) clock_count_min(&time_s, 1);
			if(button2_ev == SINGLE_PRESS) clock_count_min(&time_s, 0);
			if(button1_ev == LONG_PRESS or button2_ev == LONG_PRESS)
			{
				state = NORMAL;
				time_s.sec = 0;
			}
			if(button1_ev == DOUBLE_PRESS or button2_ev == DOUBLE_PRESS) state = SET_HOUR;
		break;
		
		case SET_HOUR:
			if(button1_ev == SINGLE_PRESS) clock_count_hour(&time_s, 1);
			if(button2_ev == SINGLE_PRESS) clock_count_hour(&time_s, 0);
			if(button1_ev == LONG_PRESS or button2_ev == LONG_PRESS)
			{
				state = NORMAL;
				time_s.sec = 0;
			}
			if(button1_ev == DOUBLE_PRESS or button2_ev == DOUBLE_PRESS) state = SET_MIN;
		
		break;
		
		default:
		break;
	
	}
	
	
	return time_s;
}

