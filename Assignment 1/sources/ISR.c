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
* 150212  MS    		Syntax Fixed.
* 150514  StefanRvO Changed descriptions, deleted optimise pragmas.
* 150517  StefanRvO	Complete rewrite.
*
*****************************************************************************/

/***************************** Include files ********************************/
#include "../headers/ISR.h"


/*****************************   Functions   *******************************/

void systick_timer_isr()
/**********************************************
* Input : None
* Output : None
* Function : Increment systick variable.
**********************************************/
{
	  // Hardware clears systick int reguest.
		ticks++;
}



/****************************** End of module *******************************/
