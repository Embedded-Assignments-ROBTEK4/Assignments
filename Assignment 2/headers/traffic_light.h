#ifndef _TRAFFIC_LIGHT_
	#define _TRAFFIC_LIGHT_

#include "GLOBAL_DEFINITIONS.h"
#include "emp_type.h"
#include "button_events.h"

#define NORWEGIAN_BLINK_TIME 300  //blink time in ms
#define SWITCH_TIME					 1000	//Normal switch time between states in ms

typedef enum {
NORMAL,
NORWEGIAN_NIGHT,
EMERGENCY
} traffic_state;

INT8U traffic_control(event button_event);

#endif
