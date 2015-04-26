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
#include "clock.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "queue.h"
#include "semphr.h"

static time clock;

static INT32U operating_time = 0;

void run_clock(void __attribute__((unused)) *pvParameters)
/**********************************************
* Input : SW1 event, SW2 event.SECOND_LENGTH / TIMEOUT_SYSTICK / 60
* Output : Time.
* Function : Count timer normally if buttons not pressed,
* 					 else the time can be changed by the user
* 					 in SET_MIN / SET_HOUR state.
**********************************************/
{
  portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
	setup_clock();
	while(1)
	{
		time_count_sec(&clock, 1);
    operating_time++;
		vTaskDelayUntil(&xLastWakeTime, SECOND_LENGTH / portTICK_RATE_MS );
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
	set_clock(12, 0, 0);
}

INT32U get_operating_time(void)
{
  return operating_time;
}
/****************************** End of module *******************************/
