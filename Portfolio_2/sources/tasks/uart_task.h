/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: uart_task.h
*
* PROJECT....: Portfolio_2
*
* DESCRIPTION: Uart_task that comunicates with uart device.
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

/*******************************   Defines    *******************************/
#define CHECK_INTERVAL 5
#define NUMBER_OF_FUEL_TYPES

/*******************************    Enums     *******************************/
typedef struct
{
  char com[20];
  INT8U id;
  INT8U size;
} command;

/*****************************   Functions   ********************************/
void uart_task(void __attribute__((unused)) *pvParameters);

/****************************** End of module *******************************/
