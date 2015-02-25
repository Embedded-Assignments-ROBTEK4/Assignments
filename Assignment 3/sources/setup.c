/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Overeem
*
* MODULENAME.: setup.c
*
* PROJECT....: Assingment 1
*
* DESCRIPTION: Setup functions.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150212  MS    		Syntax Fixed.
* 150217  StefanRvo	Split setup into multiple functions
*
*****************************************************************************/

/***************************** Include files ********************************/
#include "../headers/setup.h"

/*****************************    Defines    ********************************/

/*****************************   Functions   ********************************/
void enable_global_int()
/**********************************************
* Input : None
* Output : None
* Function : Enable global interrupt.
**********************************************/
{
  __asm("cpsie i");	// Enable interrupts.
}

void disable_global_int()
/**********************************************
* Input : None
* Output : None
* Function : Disable global interrupt.
**********************************************/
{
  __asm("cpsid i");	// Disable interrupts.
}

void setup_timers()
/**********************************************
* Input : None
* Output : None
* Function : Setup systick timer.
**********************************************/
{

	// Setup systick timer.
  NVIC_ST_CTRL_R 	 &= ~(NVIC_ST_CTRL_ENABLE);	// Disable systick timer.

  // Set current systick counter to reload value.
  NVIC_ST_CURRENT_R = SYSTICK_RELOAD_VALUE;

  // Set Reload value, Systick reload register.
  NVIC_ST_RELOAD_R  = SYSTICK_RELOAD_VALUE;

  // NVIC systick setup, vector number 15.
  // Clear pending systick interrupt request.
  NVIC_INT_CTRL_R  |= NVIC_INT_CTRL_PENDSTCLR;

  // Select systick clock source, Use core clock.
  NVIC_ST_CTRL_R   |= NVIC_ST_CTRL_CLK_SRC;
	
  // Enable systick interrupt.
  NVIC_ST_CTRL_R   |= NVIC_ST_CTRL_INTEN;

  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;	// Enable and start timer.
}

void setup_gpio()
/**********************************************
* Input : None
* Output : None
* Function :  SW1 and SW2 as input, LCD pins as output.
**********************************************/
{
	// Enable the GPIO ports that is used for the on-board buttons, and for the lcd.
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
	GPIO_PORTF_DIR_R   &= ~(SW1_PIN | SW2_PIN);   	// Init SW1 and SW2 as input.
	
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R 			 |= 0x01;

	// Enable the GPIO pins for digital function.
	GPIO_PORTF_DEN_R |= SW1_PIN | SW2_PIN | LED_RED | LED_GREEN | LED_BLUE;
	
	GPIO_PORTF_DIR_R |= LED_RED | LED_GREEN | LED_BLUE;
	GPIO_PORTF_DIR_R   &= ~(SW1_PIN | SW2_PIN);   	// Init SW1 as input.
  GPIO_PORTF_AFSEL_R &= ~(SW1_PIN | SW2_PIN);  		// Disable alt funct on PF4.
  GPIO_PORTF_PCTL_R  &= ~(GPIO_PCTL_PF4_M | GPIO_PCTL_PF0_M); 	// Configure PF4 as GPIO.
  GPIO_PORTF_AMSEL_R &= ~(SW1_PIN | SW2_PIN);  		// Disable analog functionality on PF4.
  GPIO_PORTF_PUR_R   |= 	(SW1_PIN | SW2_PIN);     	// Enable weak pull-up on SW1.
  
  //Setup LCD pins as output
  
  GPIO_PORTC_DIR_R   |= LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
  GPIO_PORTD_DIR_R   |= LCD_E | LCD_RS | LED_STATUS;
  
  GPIO_PORTC_DEN_R |= LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
  GPIO_PORTD_DEN_R |= LCD_E | LCD_RS | LED_STATUS;
  
}

/****************************** End of module *******************************/

