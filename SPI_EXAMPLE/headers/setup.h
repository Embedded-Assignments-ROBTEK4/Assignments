/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: setup.h
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: Contains the different functions used in setup.c
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150212  MS    			Syntax Fixed.
* 150217  StefanRvO 	Split setup into multiple functions.
*****************************************************************************/
#ifndef _SETUP_H
  #define _SETUP_H

/***************************** Include files ********************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "GLOBAL_DEFINITIONS.h"

/*****************************    Defines    ********************************/
#define SYSTICK_RELOAD_VALUE 		FCPU / 1000 * TIMEOUT_SYSTICK - 1

#if (SYSTICK_RELOAD_VALUE > 0xFFFFFF)
	#error "SYSTICK_RELOAD_VALUE is too high"
#endif

/*****************************   Functions   ********************************/
void setup_gpio(void);
void enable_global_int(void);
void disable_global_int(void);

/****************************** End of module *******************************/
#endif
