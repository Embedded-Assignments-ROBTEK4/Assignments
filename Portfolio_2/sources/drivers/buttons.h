/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: button_events.h
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: Functions for initialize button and getting button presses.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150217  StefanRvo		Created file.
* 150226	MS 					Fixed syntax.
*****************************************************************************/
#pragma once

/***************************** Include files ********************************/
#include "../../headers/emp_type.h"
#include <stdbool.h>
#include <stdint.h>
#include "../../headers/tm4c123gh6pm.h"
#include "../os/system_buffers.h"

/*******************************   Defines    *******************************/
#define LONG_PRESS_TIME 2000 	// Long press time in ms.
#define DOUBLE_PRESS_TIME 100 // Double click time in ms.

#define SW1_PRESSED       0
#define SW1_RELEASED      1
#define SW2_PRESSED       2
#define SW2_RELEASED      3


#define BUTTON_CHECK_PERIOD 5 //in ms

#define SW1_PIN 		1<<4
#define SW2_PIN			1<<0

/*****************************   Functions   ********************************/

void collect_button_events(void __attribute__((unused)) *pvParameters);
INT8U button_in_char(void);
void setup_buttons(void);

/****************************** End of module *******************************/
