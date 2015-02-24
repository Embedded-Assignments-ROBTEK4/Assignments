#include "emp_type.h"
#include "tm4c123gh6pm.h"
#define FCPU 16000000
#define US_CYCLES FCPU/1000000
#define MS_CYCLES FCPU/1000

void delay_microseconds(INT32U delayus); 
void delay_cycles(INT32U delay); 
void delay_milliseconds(INT32U delayms); 
void setup_delays(); //setup the timer0 to be used in the delay functions
