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
* DESCRIPTION: functions for getting button presses.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150217  StefanRvo    Created file.
*
*****************************************************************************/

#ifndef _BUTTON_EVENT_
	#define _BUTTON_EVENT_


/***************************** Include files ********************************/
#include "emp_type.h"
#include "GLOBAL_DEFINITIONS.h"
#include <stdbool.h>
#include <stdint.h>
#include "tm4c123gh6pm.h"

/***************************** Defines **************************************/
#define LONG_PRESS_TIME 2000 //Long press time in ms
#define DOUBLE_PRESS_TIME 100 //Double click time in ms

/***************************** Enum Definitions *****************************/
typedef enum {
	NO_EVENT,
	SINGLE_PRESS,
	DOUBLE_PRESS,
	LONG_PRESS,
} event;

typedef enum {
	IDLE,
	FIRST_PUSH,
	SECOND_PUSH,
	LONG_PUSH,
	FIRST_RELEASE,
} button_state;

/*****************************   Functions   ********************************/

bool button_pressed(uint32_t GPIO_PORT, uint32_t GPIO_PIN);
event get_button_event();

/****************************** End of module *******************************/
#endif

