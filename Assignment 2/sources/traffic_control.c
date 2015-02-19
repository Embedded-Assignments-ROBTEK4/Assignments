/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Overeem
*
* MODULENAME.: traffic_control.c
*
* PROJECT....: Assingment 2
*
* DESCRIPTION: Traffic light controller.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150219  ALL    Created file.
*
*****************************************************************************/
#include "../headers/traffic_control.h"
#define RED 3
#define YELLOW 5
#define YELLOW_RED 1
#define GREEN 6
#define OFF 7

/****************************** Arrays **************************************/

INT8U normal_sequence[] 	 = {RED, YELLOW_RED, GREEN, YELLOW, RED};
INT8U norwegian_sequence[] = {YELLOW, OFF};
INT8U emergency 				 	 = RED;


/****************************** Functions **********************************/

INT8U traffic_control(event button_event)
{
	static traffic_state state	= NORMAL;
	static INT16U timer 			= NORMAL_SWITCH_TIME / TIMEOUT_SYSTICK;
	static INT8U normal_index = 0;
	static INT8U norwegian_index = 0;
	INT8U returnval = 0;
	
	if (timer > 0)
		timer--;
		
	switch(state)
	{
		case NORMAL:
			if (button_event == SINGLE_PRESS)
			{
				state = NORWEGIAN_NIGHT;
				timer = NORWEGIAN_SWITCH_TIME / TIMEOUT_SYSTICK;
			}
			else if (button_event == DOUBLE_PRESS) state = EMERGENCY;
			else if (timer == 0)
			{
				normal_index = (normal_index + 1) % 
					(sizeof(normal_sequence) / sizeof(normal_sequence[0]));
					
				timer = NORMAL_SWITCH_TIME / TIMEOUT_SYSTICK;
			}
			returnval = normal_sequence[normal_index];
			break;
			
		case NORWEGIAN_NIGHT:
			if (button_event == LONG_PRESS)
			{
				state = NORMAL;
				timer = NORMAL_SWITCH_TIME / TIMEOUT_SYSTICK;
			}
			else if (button_event == DOUBLE_PRESS) state = EMERGENCY;
			else if (timer == 0)
			{
				norwegian_index = (norwegian_index + 1) % 
					(sizeof(norwegian_sequence) / sizeof(norwegian_sequence[0]));
					
				timer = NORWEGIAN_SWITCH_TIME / TIMEOUT_SYSTICK;
			}
			returnval = norwegian_sequence[norwegian_index];
			break;
			
		case EMERGENCY:
			if (button_event == LONG_PRESS)
			{
				state = NORMAL;
				timer = NORMAL_SWITCH_TIME / TIMEOUT_SYSTICK;
			}
			else if (button_event == SINGLE_PRESS)
			{
				state = NORWEGIAN_NIGHT;
				timer = NORWEGIAN_SWITCH_TIME / TIMEOUT_SYSTICK;
			}
			returnval = emergency;
			break;
		default:
			break;
	}
	return returnval;
}
/****************************** End of module *******************************/

