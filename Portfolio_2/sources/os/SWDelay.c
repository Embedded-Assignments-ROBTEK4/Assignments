/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: SWDelay.c
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: See header.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150224  StefanRvo		Created file.
* 150226	MS 					Fixed syntax.
*****************************************************************************/

/***************************** Include files ********************************/
#include "SWDelay.h"

/*****************************   Functions   ********************************/
inline void delay_cycles(INT32U delay) // Unable to delay 0.
/**********************************************
* Input : Delay.
* Output : None.
* Function : Create delay according to the input.
**********************************************/
{
	TIMER2_TAV_R = 0; // Set timer to zero.
	while(TIMER2_TAV_R < delay - 1);
}

void delay_milliseconds(INT32U delayms)
/**********************************************
* Input : Delay in ms.
* Output : None.
* Function : Create delay using void delay_cycles.
**********************************************/
{
	delay_cycles(MS_CYCLES * delayms);
}

void delay_microseconds(INT32U delayus)
/**********************************************
* Input : Delay in us.
* Output : None.
* Function : Create delay using void delay_cycles.
**********************************************/
{
	delay_cycles(US_CYCLES * delayus);
}

void setup_delay()
/**********************************************
* Input : None.
* Output : None.
* Function : Setup TIMER2 to be used in the delay functions.
**********************************************/
{
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER2; 								// Enable the timer hardware.
	__asm__("NOP");																				// Waste a few cycles.
	__asm__("NOP");
	TIMER2_CTL_R 	 &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN); // Disable the timer.
	TIMER2_CFG_R 		= 0; 																	// 32 bit mode (no RTC).
	TIMER2_TAMR_R		= TIMER_TAMR_TACDIR;									// Up, periodic.
	TIMER2_CTL_R    = TIMER_CTL_TAEN; 										// Enable TIMER2_A.
	TIMER2_TAV_R 		= 0; 																	// Set timer to zero.
}

/****************************** End of module *******************************/
