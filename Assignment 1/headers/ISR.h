/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Overeem
*
* MODULENAME.: ISR.h
*
* PROJECT....: Assingment 1
*
* DESCRIPTION: Interrupt function, and helper functions to interrupts.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150212  MS    Syntax Fixed.
*
*****************************************************************************/

#ifndef _ISR_H
  #define _ISR_H

/***************************** Include files ********************************/
#include <stdint.h>
#include <iso646.h>
#include <stdbool.h>
#include "inc/emp_type.h"
#include "inc/tm4c123gh6pm.h"

/*****************************   Functions   ********************************/
void sw1_isr();
void systick_timer_isr();
void start_automode();
void stop_automode();

/****************************** End of module *******************************/
#endif
