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
* DESCRIPTION: see header
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150217  StefanRvo	Created file
*
*****************************************************************************/

/***************************** Include files ********************************/

#include "../headers/button_events.h"

/*****************************   Functions   ********************************/
bool button_pressed(button *button_s)
/**********************************************
* Input : Port, Pin
* Output : outputs false if pin is high, true if low
* Function : check if button is pressed
* 					 
**********************************************/
{
	return !(*(button_s->port) & button_s->pin);
}


event get_button_event(button *button_s)
/**********************************************
* Input : None
* Output : button event
* Function : check if a button event have ocured and returns the event
* 					 
**********************************************/
{
	
	event button_event = NO_EVENT;

	if(button_s->timer > 0)
		button_s->timer--;
	switch (button_s->state)
	{
		case IDLE:
			if( button_pressed(button_s) )
			{
				button_s->state = FIRST_PUSH;
				button_s->timer = LONG_PRESS_TIME / TIMEOUT_SYSTICK;
			}
			break;

		case FIRST_PUSH:
			if(!button_pressed(button_s))
			{
				button_s->state = FIRST_RELEASE;
				button_s->timer = DOUBLE_PRESS_TIME / TIMEOUT_SYSTICK;
			}
			else if(button_s->timer == 0)
			{
				button_s->state 				= LONG_PUSH;
				button_event 	= LONG_PRESS;
			}
			break;

		case FIRST_RELEASE:
			if (button_pressed(button_s))
			{
				button_s->state = SECOND_PUSH;
				button_s->timer = LONG_PRESS_TIME / TIMEOUT_SYSTICK;
			}
			else if(button_s->timer == 0)
			{
				button_s->state					= IDLE;
				button_event	= SINGLE_PRESS;
			}
			break;

		case SECOND_PUSH:
			if( !button_pressed(button_s) )
			{
				button_s->state 				= IDLE;
				button_event 	= DOUBLE_PRESS;
			}
			else if(button_s->timer == 0)
			{	
				button_s->state 				= LONG_PUSH;
				button_event 	= LONG_PRESS;
			}
			break;
		
		case LONG_PUSH:
			if ( !button_pressed(button_s) )
			{
				button_s->state = IDLE;
			}
			break;
	}
	return button_event;
}

button button_init(volatile uint32_t *port, uint32_t pin )
{
	button button_s;
	button_s.port = port;
	button_s.pin = pin;
	button_s.state = IDLE;
	button_s.timer = 0;
	return button_s;
}
