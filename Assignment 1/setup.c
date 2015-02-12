#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "headers/setup.h"
#include "inc/emp_type.h"

#define FCPU                 16000000 	// Hz
#define TIMEOUT              200        // mS
#define SYSTICK_RELOAD_VALUE FCPU / 1000 * TIMEOUT-1

// Missing definitions in tm4c123gh6pm.h file
#define NVIC_INT_CTRL_PEND_SYST   0x04000000  // Pend a systick int
#define NVIC_INT_CTRL_UNPEND_SYST 0x02000000  // Unpend a systick int

#if (SYSTICK_RELOAD_VALUE > 0xFFFFFF)
	#error "SYSTICK_RELOAD_VALUE is too high"
#endif

#define LED_RED 0x02
#define LED_BLUE 0x04
#define LED_GREEN 0x08
#define SW1_PIN 0x10

void enable_global_int()
{
  // enable interrupts.
  __asm("cpsie i");
}

void disable_global_int()
{
  // disable interrupts.
  __asm("cpsid i");
}

void setup()
{

	//Enable the GPIO port that is used for the on-board LED.
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;

	//Waste a few cycles
	INT8U i;
	for(i = 0; i < 5; i++ );

	//Set the direction as output (PF1,2,3).
	GPIO_PORTF_DIR_R |= LED_RED | LED_GREEN | LED_BLUE;

	//Enable the GPIO pins for digital function (PF1,2,3,4).
	
	GPIO_PORTF_DEN_R |= LED_RED | LED_GREEN | LED_BLUE | SW1_PIN;

	GPIO_PORTF_DIR_R &= ~SW1_PIN;   // Init SW1 as input

  GPIO_PORTF_AFSEL_R &= ~SW1_PIN;  //     disable alt funct on PF4

  GPIO_PORTF_PCTL_R &= ~0x000F0000; //  configure PF4 as GPIO

  GPIO_PORTF_AMSEL_R &= ~SW1_PIN;  //    disable analog functionality on PF4
  GPIO_PORTF_PUR_R |= SW1_PIN;     //     enable weak pull-up on PF4

  GPIO_PORTF_IS_R &= ~SW1_PIN;     //  	PF4 is edge-sensitive

  GPIO_PORTF_IBE_R &= ~SW1_PIN;    //     PF4 is not both edges

  GPIO_PORTF_IEV_R &= ~SW1_PIN;    //     PF4 falling edge event

  GPIO_PORTF_ICR_R = SW1_PIN;      //  	clear flag4
	GPIO_PORTF_IM_R |= SW1_PIN;			// arm interrupt on PF4

	//NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | 0x00A00000; // priority 5
	NVIC_EN0_R = 0x40000000;      //	 enable interrupt 30 in NVIC

	//setup timer0 for debouncing and push PF4 timing.

	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0; //Enable the timer hardware
  TIMER0_CTL_R &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN); //disable the timer
	TIMER0_CFG_R = 0; // 32 bit mode (no RTC)
	TIMER0_TAMR_R = TIMER_TAMR_TACDIR ;// up, periodic
	TIMER0_CTL_R = TIMER_CTL_TAEN; // enable Timer0_A

	TIMER0_TAV_R = 0; //Set timer to zero

	//setup systick timer for automode

  // Disable systick timer
  NVIC_ST_CTRL_R &= ~(NVIC_ST_CTRL_ENABLE);

  // Set current systick counter to reload value
  NVIC_ST_CURRENT_R = SYSTICK_RELOAD_VALUE;

  // Set Reload value, Systick reload register
  NVIC_ST_RELOAD_R = SYSTICK_RELOAD_VALUE;

  // NVIC systick setup, vector number 15
  // Clear pending systick interrupt request
  NVIC_INT_CTRL_R |= NVIC_INT_CTRL_UNPEND_SYST;

  // Select systick clock source, Use core clock
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC;
	
  // Enable systick interrupt
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_INTEN;

	//Don't start timer. This will be done when enabling automode


	enable_global_int();
	
}
