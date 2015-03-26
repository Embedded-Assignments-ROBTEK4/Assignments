/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: clock.c
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: See header.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150225  ALL		Created file.
* 150226	MS 		Fixed syntax.
*****************************************************************************/

/***************************** Include files ********************************/
#include "../headers/clock.h"
#include "../headers/buttons.h"
#include "../headers/system_timers.h"
#include "../headers/scheduler.h"

static INT8U clock_timer = 0;
static time clock;


static void clock_count_hour(time *time_s, bool direction);
static void clock_count_min(time *time_s, bool direction);
void clock_tick(time *time_s);

/*****************************   Functions   ********************************/
void clock_tick(time *time_s)
/**********************************************
* Input : Struct time.
* Output : None.
* Function : Count timer.
**********************************************/
{
	if(++time_s->sec >= SECONDS_PER_MINUTE)
	{
		time_s->sec = 0;
		if(++time_s->min >= MINUTES_PER_HOUR)
		{
			time_s->min = 0;
			if(++time_s->hour >= HOURS_PER_DAY)
				time_s->hour = 0;
		}
	}
}

static void clock_count_min(time *time_s, bool direction)
/**********************************************
* Input : Struct time, direction of count.
* Output : None.
* Function : Increment time_s->min if direction == true.
* 					 Decrement time_s->min if direction == false.
**********************************************/
{
	if(direction)
		if(time_s->min >= MINUTES_PER_HOUR - 1)
			 time_s->min = 0;
		else
			 time_s->min++;
	else
	{
		if(time_s->min == 0)
			 time_s->min = MINUTES_PER_HOUR - 1;
		else
			 time_s->min--;
	}
}

static void clock_count_hour(time *time_s, bool direction)
/**********************************************
* Input : Struct time, direction of count.
* Output : None.
* Function : Increment time_s->hour if direction == true.
* 					 Decrement time_s->hour if direction == false.
**********************************************/
{
	if(direction)
		if(time_s->hour >= HOURS_PER_DAY - 1)
			 time_s->hour = 0;
		else
			 time_s->hour++;
	else
	{
		if(time_s->hour == 0)
			 time_s->hour = HOURS_PER_DAY - 1;
		else
			 time_s->hour--;
	}
}

void run_clock(void)
/**********************************************
* Input : SW1 event, SW2 event.
* Output : Time.
* Function : Count timer normally if buttons not pressed,
* 					 else the time can be changed by the user
* 					 in SET_MIN / SET_HOUR state.
**********************************************/
{
	clock_state state = NORMAL;
	setup_clock();
	while(1)
	{
		INT8U event_from_button = NO_BUTTON_EVENT;

		if(button_data_avaliable())
		{
			event_from_button = button_in_char();
		}
		switch(state)
		{
			case NORMAL:
				if(event_from_button == SW1_LONG_PRESS or event_from_button == SW2_LONG_PRESS) state = SET_MIN;
				else if(get_timer_val(clock_timer) <= SECOND_LENGTH / TIMEOUT_SYSTICK)
				{
					increase_timer_val(clock_timer, SECOND_LENGTH / TIMEOUT_SYSTICK);
					clock_tick(&clock);
				}
				break;

			case SET_MIN:
				if(event_from_button == SW1_SINGLE_PRESS) clock_count_min(&clock, 1);
				else if(event_from_button == SW2_SINGLE_PRESS) clock_count_min(&clock, 0);
				else if(event_from_button == SW1_LONG_PRESS or event_from_button == SW2_LONG_PRESS)
				{
					state 		 = NORMAL;
					clock.sec = 0;
				}
				else if(event_from_button == SW1_DOUBLE_PRESS or event_from_button == SW2_DOUBLE_PRESS) state = SET_HOUR;
				break;

			case SET_HOUR:
				if(event_from_button == SW1_SINGLE_PRESS) clock_count_hour(&clock, 1);
				else if(event_from_button == SW2_SINGLE_PRESS) clock_count_hour(&clock, 0);
				else if(event_from_button == SW1_LONG_PRESS or event_from_button == SW2_LONG_PRESS)
				{
					state = NORMAL;
					clock.sec = 0;
				}
				if(event_from_button == SW1_DOUBLE_PRESS or event_from_button == SW2_DOUBLE_PRESS) state = SET_MIN;
				break;

			default:
				break;
		}
	wait(SECOND_LENGTH / TIMEOUT_SYSTICK / 2);
	}
}
bool set_clock(INT8U hour_, INT8U min_, INT8U sec_)
{
	if(hour_ < HOURS_PER_DAY && min_ < MINUTES_PER_HOUR && sec_ < SECONDS_PER_MINUTE)
	{
		clock.hour = hour_;
		clock.min = min_;
		clock.sec = sec_;
		return true;
	}
	return false;
}

time get_clock(void)
{
	return clock;
}
void setup_clock(void)
{ //Just accuire a timer for the clock, and set the time to 12:00:00.
	//Start the timer at 2*second lenght
	clock_timer = request_timer();
	start_timer(clock_timer, 2 * SECOND_LENGTH / TIMEOUT_SYSTICK);
	set_clock(12, 0, 0);
}
/****************************** End of module *******************************/
