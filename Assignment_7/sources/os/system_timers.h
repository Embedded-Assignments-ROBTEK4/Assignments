#pragma once
#include "../../headers/emp_type.h"
#include <stdbool.h>
#include "FreeRTOS.h"

#define MAX_TIMERS 50
void count_timer(INT8U id);
void start_timer(INT8U id, INT16U timer_val);
void stop_timer(INT8U id);
bool is_timer_finished(INT8U id);
INT16U get_timer_val(INT8U id);
void increase_timer_val(INT8U id, INT16U val);
INT8U request_timer(void);
void release_timer(INT8U id);
void timer_task(void __attribute__((unused)) *pvParameters);


#define MAX_TIMER_VAL  0xFFFF
#define SYS_TIMER_PERIODE 5
