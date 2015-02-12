#include "ISR.h"
#include "inc/emp_type.h"
#include <stdbool.h>
#include  <iso646.h>
#define FCPU 16000000 //CPU frequency
#define DEBOUNCETIMEUS 100000 //debounce time in µs
#define DEBOUNCEVAL FCPU/1000000*DEBOUNCETIMEUS
#define DOUBLE_CLICK_TIME_US 500000 //time between clicks for doubleclick
#define DOUBLE_CLICK_VAL FCPU/1000000*DOUBLE_CLICK_TIME_US
#define AUTOMODE_HOLD_TIME_US 2000000
#define AUTOMODE_HOLD_VAL FCPU/1000000*AUTOMODE_HOLD_TIME_US
//Defines for LEDs

#define LED_RED 0x02
#define LED_BLUE 0x04
#define LED_GREEN 0x08
#define LED_PORT GPIO_PORTF_DATA_R
#define SW1_PIN 0x10


static bool count_direction = 1; //1 is forward, 0 is backward


void start_automode()
{
  // Enable and start timer
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;
}

void stop_automode()
{
  // stop timer
  NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE;	
}

void set_leds(INT8U val)
{
	GPIO_PORTF_DATA_R &= ~(LED_BLUE | LED_GREEN | LED_RED);
	switch (val)
	{
		case 1:
			GPIO_PORTF_DATA_R |= LED_GREEN;
			break;
		case 2:
			GPIO_PORTF_DATA_R |= LED_BLUE;
			break;
		case 3:
			GPIO_PORTF_DATA_R |= LED_GREEN | LED_BLUE;
			break;
		case 4:
			GPIO_PORTF_DATA_R |= LED_RED;
			break;
		case 5:
			GPIO_PORTF_DATA_R |= LED_RED | LED_GREEN;
			break;
		case 6:
			GPIO_PORTF_DATA_R |= LED_RED | LED_BLUE;
			break;
		case 7:
			GPIO_PORTF_DATA_R |= LED_RED | LED_GREEN | LED_BLUE;
			break;
		default :
			__asm("NOP");
	}
}

void count()
{
		static INT8U counter = 0;
		if(count_direction)
		{
			counter++;
			if(counter >= 8)
				counter = 0;
		}
		else
		{
			if (counter == 0)
				counter = 7;
			else
				counter--;
		}
		set_leds(counter);
}

#pragma GCC optimize "-O0" //Don't optimise the ISR
void systick_timer_isr()
{
	  // Hardware clears systick int reguest
		count();
}
#pragma GCC optimize "-O0" //Don't optimise the ISR
void sw1_isr()
{
	INT32U timerval = TIMER0_TAV_R;
	GPIO_PORTF_ICR_R = SW1_PIN;  // Clear interrupt flag
	static bool edge = 0; //0 is falling, 1 is rising

	if (edge == 0)	//falling edge tasks
	{
		GPIO_PORTF_IEV_R |= SW1_PIN; //next interrupt should be rising edge
		edge = 1;

		if(timerval < DOUBLE_CLICK_VAL and timerval > DEBOUNCEVAL)
		{
			count_direction = !count_direction;	//change direction on doublclick
		}
		if ( timerval > DEBOUNCEVAL)
		{
			count();
			stop_automode();
			TIMER0_TAV_R = 0; //set timer to zero
		}
	}
	else	//rising edge tasks
	{
		GPIO_PORTF_IEV_R &= ~SW1_PIN; //next interrupt should be falling edge
		edge = 0;

		if(timerval > AUTOMODE_HOLD_VAL)
		  start_automode();
			
	}
	return;
}
