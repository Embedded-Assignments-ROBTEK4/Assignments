#ifndef _COUNTER_
	#define _COUNTER_

#include "button_events.h"
#include "emp_type.h"
#include "GLOBAL_DEFINITIONS.h"
#include <iso646.h>

#define MAX_VAL 7
#define AUTO_COUNT_TIME 200 //time in ms between counting in automode

typedef enum {
	UP,
	DOWN,
	AUTO_UP,
	AUTO_DOWN,
} counter_state;

INT8U count(event button_event);
void count_up(INT8U *counter);
void count_down(INT8U *counter);

#endif
