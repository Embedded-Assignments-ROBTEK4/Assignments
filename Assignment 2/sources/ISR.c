/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Overeem
*
* MODULENAME.: ISR.c
*
* PROJECT....: Assingment 2
*
* DESCRIPTION: Contains the interrupt service routines.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150212  MS    Syntax Fixed.
* 150514  StefanRvO Changed descriptions, deleted optimise pragmas.
*
*****************************************************************************/

/***************************** Include files ********************************/
#include "../headers/ISR.h"

/*****************************    Defines    ********************************/

/*****************************   Variables   ********************************/

/*****************************   Functions   *******************************/

void systick_timer_isr()
/**********************************************
* Input : None
* Output : None
* Function : When in auto mode the systick
* 					 interrupt is triggered
* 					 and the systick timer is incremented.
**********************************************/
{
	  // Hardware clears systick int reguest.
		ticks++;
}



/****************************** End of module *******************************/
