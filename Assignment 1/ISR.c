/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Overeem
*
* MODULENAME.: ISR.c
*
* PROJECT....: Assingment 1
*
* DESCRIPTION: Contains the interrupt service routines.
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
#include "headers/ISR.h"

/*****************************    Defines    ********************************/
#define FCPU 									16000000 	// CPU frequency.
#define DEBOUNCETIMEUS 				1500 		// Debounce time in µs.
#define DEBOUNCEVAL 					FCPU / 1000000 * DEBOUNCETIMEUS
#define DOUBLE_CLICK_TIME_US 	200000 		// Time between clicks for doubleclick.
#define DOUBLE_CLICK_VAL 			FCPU / 1000000 * DOUBLE_CLICK_TIME_US
#define AUTOMODE_HOLD_TIME_US 2000000
#define AUTOMODE_HOLD_VAL 		FCPU / 1000000 * AUTOMODE_HOLD_TIME_US

// Defines for I/O.
#define LED_RED 	0x02
#define LED_BLUE 	0x04
#define LED_GREEN 0x08
#define LED_PORT 	GPIO_PORTF_DATA_R
#define SW1_PIN 	0x10

/*****************************   Constants   ********************************/
static bool count_direction = 1; // 1 is forward, 0 is backward.

/*****************************   Functions   *******************************/
void start_automode()
/**********************************************
* Input : None
* Output : None
* Function : Enable and start systick timer used for automode.
**********************************************/
{
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;	// Enable and start timer.
}

void stop_automode()
/**********************************************
* Input : None
* Output : None
* Function : Stop systick timer used for automode.
**********************************************/
{
  NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE;	// Stop timer.
}

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
			__asm("NOP");	// Do nothing.
	}
}

void count()
/**********************************************
* Input : None
* Output : None
* Function : Set LEDs accordingly to the input value.
**********************************************/
{
	static INT8U counter = 0;

	// Check the direction and increment or decrement accordingly.
	if (count_direction)
	{
		counter++;
		if (counter >= 8)
			counter = 0;
	}
	else
	{
		if (counter <= 0)
			counter = 7;
		else
			counter--;
	}
	set_leds(counter);
}

#pragma GCC optimize "-O0" // Don't optimize the ISR.
void systick_timer_isr()
/**********************************************
* Input : None
* Output : None
* Function : When in auto mode the systick
* 					 interrupt is triggered
* 					 and count is called.
**********************************************/
{
	  // Hardware clears systick int reguest.
		count();
}

#pragma GCC optimize "-O0" // Don't optimize the ISR
void sw1_isr()
/**********************************************
* Input : None
* Output : None
* Function : When SW1 is pressed this interrupt
* 					 is triggered. Check if a dobble click
* 					 occurs the direction is changed,
* 					 if SW1 is pressed for 2 sec then
* 					 automode is enabled and the systick timer.
**********************************************/
{
	INT32U timerval  = TIMER0_TAV_R;
	GPIO_PORTF_ICR_R = SW1_PIN;  						// Clear interrupt flag.
	static bool edge = 0; 									// 0 is falling, 1 is rising.

	if (edge == 0)													// Falling edge tasks.
	{
		GPIO_PORTF_IEV_R |= SW1_PIN;				 	// Next interrupt should be rising edge.
		edge = 1;

		if (timerval < DOUBLE_CLICK_VAL and timerval > DEBOUNCEVAL)
		{
			count_direction = !count_direction;	// Change direction on doublclick.
		}

		if (timerval > DEBOUNCEVAL)
		{
			count();
			stop_automode();
		}
	}
	else																		// Rising edge tasks.
	{
		GPIO_PORTF_IEV_R &= ~SW1_PIN; 				// Next interrupt should be falling edge.
		edge = 0;

		if (timerval > AUTOMODE_HOLD_VAL)
		  start_automode();
	}
	TIMER0_TAV_R=0; //zero timer.
	return;
}

/****************************** End of module *******************************/
