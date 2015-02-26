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
* DESCRIPTION: functions for controlling the LEDS.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150217  StefanRvo    Created file.
*
*****************************************************************************/

#ifndef _LEDS_
	#define _LEDS_

/***************************** Include files ********************************/
#include "GLOBAL_DEFINITIONS.h"
#include "tm4c123gh6pm.h"
#include "emp_type.h"

/*****************************   Functions   ********************************/
void set_leds(INT8U counter_val);

/****************************** End of module *******************************/
#endif

