/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Overeem
*
* MODULENAME.: traffic_control.h
*
* PROJECT....: Assingment 2
*
* DESCRIPTION: Traffic light controller.
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150219 ALL Created file
*
*****************************************************************************/

#ifndef _TRAFFIC_CONTROL_H
  #define _TRAFFIC_CONTROL_H

/***************************** Include files ********************************/
#include "tm4c123gh6pm.h"
#include "button_events.h"
#include "GLOBAL_DEFINITIONS.h"
#include "emp_type.h"


/***************************** Defines **************************************/

#define NORMAL_SWITCH_TIME 1000 //switch time in ms
#define NORWEGIAN_SWITCH_TIME 500 //switch time in ms

/******************************** Enums *************************************/

typedef enum {
NORMAL,
NORWEGIAN_NIGHT,
EMERGENCY,
} traffic_state;


/*****************************   Functions   ********************************/

INT8U traffic_control(event button_event);

/****************************** End of module *******************************/
#endif
