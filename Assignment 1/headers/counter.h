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
* DESCRIPTION: Counter module
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150217  StefanRvo    Created file.
*
*****************************************************************************/

#ifndef _COUNTER_
	#define _COUNTER_

/***************************** Include files ********************************/
#include "button_events.h"
#include "emp_type.h"
#include "GLOBAL_DEFINITIONS.h"
#include <iso646.h>

/***************************** Defines **************************************/
#define MAX_VAL 7
#define AUTO_COUNT_TIME 200 //time in ms between counting in automode

/***************************** Enum Definitions *****************************/

typedef enum {
	UP,
	DOWN,
	AUTO_UP,
	AUTO_DOWN,
} counter_state;

/*****************************   Functions   ********************************/
INT8U count(event button_event);
void count_up(INT8U *counter);
void count_down(INT8U *counter);

/****************************** End of module *******************************/
#endif
