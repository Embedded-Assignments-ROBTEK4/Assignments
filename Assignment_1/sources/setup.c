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
* Function : Setup LEDs  and SW1, interrupts,
* 					 timer0 and systick timer.
**********************************************/
{
	// Enable the GPIO port that is used for the on-board LED.
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;

	// Waste a few cycles.
	__asm("NOP");
	__asm("NOP");

	// Set the direction as output for the LED's.
	GPIO_PORTF_DIR_R |= LED_RED | LED_GREEN | LED_BLUE;

	// Enable the GPIO pins for digital function.
	GPIO_PORTF_DEN_R |= LED_RED | LED_GREEN | LED_BLUE | SW1_PIN;

	GPIO_PORTF_DIR_R   &= ~SW1_PIN;   	// Init SW1 as input.
  GPIO_PORTF_AFSEL_R &= ~SW1_PIN;  		// Disable alt funct on PF4.
  GPIO_PORTF_PCTL_R  &= ~GPIO_PCTL_PF4_M; 	// Configure PF4 as GPIO.
  GPIO_PORTF_AMSEL_R &= ~SW1_PIN;  		// Disable analog functionality on PF4.
  GPIO_PORTF_PUR_R   |= SW1_PIN;     	// Enable weak pull-up on SW1.
}

/****************************** End of module *******************************/

