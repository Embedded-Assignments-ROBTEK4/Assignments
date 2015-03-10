/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: setup.c
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: Setup functions.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150212  MS    			Syntax Fixed.
* 150217  StefanRvo		Split setup into multiple functions.
* 150226  MS    			Syntax Fixed.
*****************************************************************************/

/***************************** Include files ********************************/
#include "../headers/setup.h"

/*****************************   Functions   ********************************/

void setup_gpio()
/**********************************************
* Input : None.
* Output : None.
* Function : LCD pins as output., LED's
**********************************************/
{
	// Enable the GPIO ports that is used for the on-board buttons, and for the lcd.
	SYSCTL_RCGC2_R 		|= SYSCTL_RCGC2_GPIOF;
	SYSCTL_RCGC2_R 		|= SYSCTL_RCGC2_GPIOC;
  SYSCTL_RCGC2_R 		|= SYSCTL_RCGC2_GPIOD;
	

	// Enable the GPIO pins for digital function.
	GPIO_PORTF_DEN_R |= LED_RED | LED_GREEN | LED_BLUE;
	
	GPIO_PORTF_DIR_R 	 |= LED_RED | LED_GREEN | LED_BLUE;
  
  //Setup LCD pins as output
  GPIO_PORTC_DIR_R |= LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
  GPIO_PORTD_DIR_R |= LCD_E  | LCD_RS | LED_STATUS;
  
  GPIO_PORTC_DEN_R |= LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
  GPIO_PORTD_DEN_R |= LCD_E  | LCD_RS | LED_STATUS;
}

/****************************** End of module *******************************/

