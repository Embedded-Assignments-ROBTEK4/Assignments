/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: ISR.h
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: Interrupt functions.
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150212  MS    			Syntax Fixed.
*	150217  StefanRvo 	Changed to only use systick interrupt.
*****************************************************************************/
#ifndef _ISR_H
  #define _ISR_H

/***************************** Include files ********************************/
#include "tm4c123gh6pm.h"
#include "globals.h"

/*****************************   Functions   ********************************/
void systick_timer_isr(void);

/****************************** End of module *******************************/
#endif
