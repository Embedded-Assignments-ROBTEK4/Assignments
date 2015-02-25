#ifndef _CLOCK_H_
#define _CLOCK_H_
#include "button_events.h"
#include "GLOBAL_DEFINITIONS.h"

typedef struct {
	INT8U sec;
	INT8U min;
	INT8U hour;
} time;


typedef enum {
	NORMAL,
	SET_MIN,
	SET_HOUR,
} clock_state;


time clock(event button1_ev, event button2_ev);

void clock_count_min(time *time_s, bool direction);
void clock_count_hour(time *time_s, bool direction);

#define SECOND_LENGTH 1000 //length of a second in ms


#endif
