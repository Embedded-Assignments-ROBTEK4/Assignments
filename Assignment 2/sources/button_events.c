#include "../headers/button_events.h"

bool button_pressed(uint32_t GPIO_PORT, uint32_t GPIO_PIN)
{
	return !(GPIO_PORT & GPIO_PIN);
}
event get_button_event()
{
	static button_state state	= IDLE;
	static INT16U timer 			= 0;
	event button_event 				= NO_EVENT;

	if(timer > 0)
		timer--;
	switch (state)
	{
		case IDLE:
			if( button_pressed(GPIO_PORTF_DATA_R, SW1_PIN) )
			{
				state = FIRST_PUSH;
				timer = LONG_PRESS_TIME / TIMEOUT_SYSTICK;
			}
			break;

		case FIRST_PUSH:
			if(!button_pressed(GPIO_PORTF_DATA_R, SW1_PIN))
			{
				state = FIRST_RELEASE;
				timer = DOUBLE_PRESS_TIME / TIMEOUT_SYSTICK;
			}
			else if(timer == 0)
			{
				state 				= LONG_PUSH;
				button_event 	= LONG_PRESS;
			}
			break;

		case FIRST_RELEASE:
			if (button_pressed(GPIO_PORTF_DATA_R, SW1_PIN))
			{
				state = SECOND_PUSH;
				timer = LONG_PRESS_TIME / TIMEOUT_SYSTICK;
			}
			else if(timer == 0)
			{
				state					= IDLE;
				button_event	= SINGLE_PRESS;
			}
			break;

		case SECOND_PUSH:
			if( !button_pressed(GPIO_PORTF_DATA_R, SW1_PIN) )
			{
				state 				= IDLE;
				button_event 	= DOUBLE_PRESS;
			}
			else if(timer == 0)
			{	
				state 				= LONG_PUSH;
				button_event 	= LONG_PRESS;
			}
			break;
		
		case LONG_PUSH:
			if ( !button_pressed(GPIO_PORTF_DATA_R, SW1_PIN) )
			{
				state = IDLE;
			}
			break;
	}
	return button_event;
}
