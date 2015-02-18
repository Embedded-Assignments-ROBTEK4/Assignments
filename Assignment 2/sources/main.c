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
* DESCRIPTION: Main loop
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150212  MS    Syntax Fixed.
*
*****************************************************************************/

/***************************** Include files ********************************/
#include <stdint.h>
#include "../headers/setup.h"
#include "../headers/tm4c123gh6pm.h"
#include "../headers/GLOBAL_DEFINITIONS.h"
#include "../headers/globals.h"
#include "../headers/button_events.h"
#include "../headers/traffic_light.h"
#include "../headers/leds.h"

/*****************************   Functions   ********************************/
int main(void)
/**********************************************
* Input : None
* Output : None
* Function : Call the setup functions
*						 enter mainloop which runs once per 5 seconds
* 					 
**********************************************/
{
	INT8U traffic_val;
	event button_event;
	
	disable_global_int();
	setup_gpio();
	setup_timers();
	enable_global_int();

	while (1)
	{
		while( !ticks )
			;
		ticks--;
		button_event = get_button_event();
		traffic_val = traffic_control(button_event);
		
		set_leds(traffic_val);
	}
	return (0);
}

/****************************** End of module *******************************/
