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
#include "../headers/SWDelay.h"
#include "../headers/lcd.h"
#include "../headers/clock.h"
#include "../headers/display_clock.h"

#define STATUS_BLINK_TIME 500 //blink time for status led in ms
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
	button sw1 = button_init(&GPIO_PORTF_DATA_R, SW1_PIN);
	button sw2 = button_init(&GPIO_PORTF_DATA_R, SW2_PIN);
	event sw1_event;
	event sw2_event;
	
	
	disable_global_int();
	setup_gpio(); //Setup SW1, SW2 and LCD pins
	setup_delay(); //Setup timer0 for delay functions
	setup_timers(); //Setup systick timer
	enable_global_int();
	
	lcd lcd_disp = lcd_init_4bit(LCD_RS, LCD_E, &LCD_RS_E_PORT, LCD_D4, LCD_D5,
										LCD_D6, LCD_D7, &LCD_DATA_PORT);
	lcd_begin(&lcd_disp, 2);
	
	static INT32U led_status_timer = STATUS_BLINK_TIME / TIMEOUT_SYSTICK;
	while (1)
	{
		while( !ticks );
		ticks--;
		/************ Show status LED *********/
		if(led_status_timer > 0)
			led_status_timer--;
		
		if(!led_status_timer)
		{
			LED_STATUS_PORT ^= LED_STATUS;
			led_status_timer = STATUS_BLINK_TIME / TIMEOUT_SYSTICK;
		}	
		/************END STATUS LED************/
		
		sw1_event = get_button_event(&sw1);
		sw2_event = get_button_event(&sw2);
		time time_s = clock(sw1_event, sw2_event);
		display_clock(&lcd_disp, &time_s);
		
	}
	return (0);
}

/****************************** End of module *******************************/
