/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Overeem
*
* MODULENAME.: setup.h
*
* PROJECT....: Assingment 1
*
* DESCRIPTION: Contains the different function used in setup.c
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150212  MS    Syntax Fixed.
*
*****************************************************************************/

#ifndef _SETUP_H
  #define _SETUP_H

/***************************** Include files ********************************/
#include <stdint.h>
#include "emp_type.h"
#include "tm4c123gh6pm.h"

/*****************************   Functions   ********************************/
void setup();
void enable_global_int();
void disable_global_int();

/****************************** End of module *******************************/
#endif
