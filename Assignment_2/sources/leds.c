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
* 150217  StefanRvO    Create file
*
*****************************************************************************/

/***************************** Include files ********************************/
#include "../headers/leds.h"

/*****************************   Functions   ********************************/
void set_leds(INT8U counter_val)
/**********************************************
* Input : Value of the counter.
* Output : None
* Function : Set LEDs accordingly to the input value.
**********************************************/
{
	// Turn off LEDs.
	GPIO_PORTF_DATA_R &= ~(LED_RED | LED_GREEN | LED_BLUE);

	// Set LEDs.
	switch (counter_val)
	{
		case 1:
			GPIO_PORTF_DATA_R |= LED_GREEN;
			break;
		case 2:
			GPIO_PORTF_DATA_R |= LED_BLUE;
			break;
		case 3:
			GPIO_PORTF_DATA_R |= LED_GREEN | LED_BLUE;
			break;
		case 4:
			GPIO_PORTF_DATA_R |= LED_RED;
			break;
		case 5:
			GPIO_PORTF_DATA_R |= LED_RED | LED_GREEN;
			break;
		case 6:
			GPIO_PORTF_DATA_R |= LED_RED | LED_BLUE;
			break;
		case 7:
			GPIO_PORTF_DATA_R |= LED_RED | LED_GREEN | LED_BLUE;
			break;
		default :
			break;
	}
}
/****************************** End of module *******************************/
