#pragma once
#include "emp_type.h"
#include <stdbool.h>
#define MAX_TIMERS 30
void count_timer(INT8U id);
void start_timer(INT8U id, INT16U timer_val);
bool is_timer_finished(INT8U id);
INT8U request_timer(void);
void release_timer(INT8U id);
