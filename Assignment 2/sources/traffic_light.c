#include "../headers/traffic_light.h"

INT8U normal_sequence[] = {3, 1, 6, 5, 3}; //Sequence the light shows in in normal operation.
INT8U norwegian_sequence[] = {7,5}; //Sequence the light shows in normal operation.
INT8U emergency_val = 3; //value for the light in emergency operation

INT8U traffic_control(event button_event)
{
	static INT8U normal_sequence_counter = 0;
	static INT8U norwegian_sequence_counter = 0;
	static INT16U timer = SWITCH_TIME / TIMEOUT_SYSTICK;
	static traffic_state state = NORMAL;
	INT8U returnval = 0;
	
	if (timer > 0)
		timer--;
		
	switch(state)
	{
		case NORMAL:
			if(button_event == SINGLE_PRESS)
			{
				state = NORWEGIAN_NIGHT;
				timer = NORWEGIAN_BLINK_TIME/ TIMEOUT_SYSTICK;
			}
			else if(button_event == DOUBLE_PRESS)
			{
				state = EMERGENCY;
			}
			else if(timer == 0)
			{
				normal_sequence_counter = 
					(normal_sequence_counter + 1) % ( sizeof(normal_sequence) / sizeof(normal_sequence[0]) );
					
				timer = SWITCH_TIME / TIMEOUT_SYSTICK;
			}
			returnval = normal_sequence[normal_sequence_counter];
			break;
			
		case NORWEGIAN_NIGHT:
			if(button_event == DOUBLE_PRESS)
			{
				state = EMERGENCY;
			}
			else if (button_event == LONG_PRESS)
			{
				state = NORMAL;
				timer = SWITCH_TIME / TIMEOUT_SYSTICK;
			}
			else if(timer == 0)
			{
				norwegian_sequence_counter = 
					(norwegian_sequence_counter + 1) % ( sizeof(norwegian_sequence) / sizeof(norwegian_sequence[0]) );
					
				timer = NORWEGIAN_BLINK_TIME / TIMEOUT_SYSTICK;
			}
			returnval = norwegian_sequence[norwegian_sequence_counter];
			break;
		case EMERGENCY:
			if(button_event == SINGLE_PRESS)
			{
				state = NORWEGIAN_NIGHT;
				timer = NORWEGIAN_BLINK_TIME/ TIMEOUT_SYSTICK;
			}
			else if (button_event == LONG_PRESS)
			{
				state = NORMAL;
				timer = SWITCH_TIME / TIMEOUT_SYSTICK;
			}
			returnval = emergency_val;
			break;
		default:
			break;
	}
	return returnval;
}
