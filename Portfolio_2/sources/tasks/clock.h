/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: clock.h
*
* PROJECT....: Portfolio_2
*
* DESCRIPTION: Real time clock.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150225  ALL		Created file.
* 150226	MS 		Fixed syntax.
*****************************************************************************/
#pragma once

/***************************** Include files ********************************/
#include "../../headers/emp_type.h"
#include <stdbool.h>
#include "../libs/time.h"

/*******************************   Defines    *******************************/
#define SECOND_LENGTH 			1000 // Length of a second in ms.

/*******************************    Enums     *******************************/

/*****************************   Functions   ********************************/
void run_clock(void __attribute__((unused)) *pvParameters);
time get_clock(void);
bool set_clock(INT8U hour_, INT8U min_, INT8U sec_);
void setup_clock(void);
INT32U get_operating_time(void);

/****************************** End of module *******************************/
