/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: clock.h
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: Real time clock that can be set using SW1 and SW2.
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
#include "GLOBAL_DEFINITIONS.h"
#include "emp_type.h"
#include <stdbool.h>

/*******************************   Defines    *******************************/
#define SECOND_LENGTH 1000 // Length of a second in ms.

/*******************************    Enums     *******************************/
typedef enum {
	NORMAL,
	SET_MIN,
	SET_HOUR,
} clock_state;

/*******************************   Structs    *******************************/
typedef struct {
	INT8U sec;
	INT8U min;
	INT8U hour;
} time;

/*****************************   Functions   ********************************/
void run_clock(void);
time get_clock(void);
bool set_clock(INT8U hour_, INT8U min_, INT8U sec_);
void setup_clock(void);

/****************************** End of module *******************************/
