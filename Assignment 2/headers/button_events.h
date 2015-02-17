#ifndef _BUTTON_EVENT_
	#define _BUTTON_EVENT_

#include "emp_type.h"
#include "GLOBAL_DEFINITIONS.h"
#include <stdbool.h>
#include <stdint.h>
#include "tm4c123gh6pm.h"

#define LONG_PRESS_TIME 2000 //Long press time in ms
#define DOUBLE_PRESS_TIME 100 //Double click time in ms
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

bool button_pressed(uint32_t GPIO_PORT, uint32_t GPIO_PIN);
event get_button_event();

#endif

