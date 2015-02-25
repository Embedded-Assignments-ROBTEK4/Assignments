#include "../headers/SWDelay.h"

inline void delay_cycles(INT32U delay) //you can not delay 0
{
	TIMER0_TAV_R = 0; //Set timer to zero
	while( TIMER0_TAV_R < delay-1 );
}
void delay_milliseconds(INT32U delayms)
{
	delay_cycles(MS_CYCLES*delayms);
	
}
void delay_microseconds(INT32U delayus)
{
	delay_cycles(US_CYCLES*delayus);
	
}

#pragma GCC optimize "-O0"
void setup_delay() //setup the timer0 to be used in the delay functions
{
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0; //Enable the timer hardware
	TIMER0_CTL_R &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN); //disable the timer
	TIMER0_CFG_R = 0; // 32 bit mode (no RTC)
	TIMER0_TAMR_R = TIMER_TAMR_TACDIR ;// up, periodic
	TIMER0_CTL_R = TIMER_CTL_TAEN; // enable Timer0_A
	TIMER0_TAV_R = 0; //Set timer to zero
}
