/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Overeem
*
* MODULENAME.: main.c
*
* PROJECT....: Assingment 1
*
* DESCRIPTION: Counter
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150212  StefanRvO    Created file.
*
*****************************************************************************/

/***************************** Include files ********************************/
#include "../headers/counter.h"

/*****************************   Functions   ********************************/

void count_up(INT8U *counter)
{
/**********************************************
* Input : int pointer
* Output : None
* Function : count the int up
* 					 
**********************************************/
	if (*counter == MAX_VAL)
		*counter = 0;
	else
		(*counter)++;
}
void count_down(INT8U *counter)
{
/**********************************************
* Input : int pointer
* Output : None
* Function : count the int down
* 					 
**********************************************/
	if (*counter == 0)
		*counter = MAX_VAL;
	else
		(*counter)--;
}


INT8U count(event button_event)
/**********************************************
* Input : None
* Output : None
* Function : Count according to the recieved event.
**********************************************/
{
	static counter_state state 	= UP;
	static INT8U counter 				= 0;
	static INT32U timer 				= 0;
	
	if(timer > 0)
		timer--;
	
	switch(state)
	{
		case UP:
			if(button_event == SINGLE_PRESS) count_up(&counter);
			else if(button_event == DOUBLE_PRESS) state = DOWN;
			else if(button_event == LONG_PRESS)
			{
				state = AUTO_UP;
				timer = AUTO_COUNT_TIME / TIMEOUT_SYSTICK;
			}
			break;
			
		case DOWN:
			if(button_event == SINGLE_PRESS) count_down(&counter);
			else if(button_event == DOUBLE_PRESS) state = UP;
			else if(button_event == LONG_PRESS)
			{
				state = AUTO_DOWN;
				timer = AUTO_COUNT_TIME / TIMEOUT_SYSTICK;
			}
			break;
			
		case AUTO_UP:
			if (button_event != NO_EVENT) state = UP;
			else if (timer == 0)
			{
				count_up(&counter);
				timer = AUTO_COUNT_TIME / TIMEOUT_SYSTICK;
			}
			break;
		case AUTO_DOWN:
			if (button_event != NO_EVENT) state = DOWN;
			else if (timer == 0)
			{
				count_down(&counter);
				timer = AUTO_COUNT_TIME / TIMEOUT_SYSTICK;
			}
			break;
		default:
			break;
	}
	return counter;
}
