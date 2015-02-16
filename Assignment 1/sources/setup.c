/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Overeem
*
* MODULENAME.: setup.c
*
* PROJECT....: Assingment 1
*
* DESCRIPTION: Setup interrupt and timers.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150212  MS    Syntax Fixed.
*
*****************************************************************************/

/***************************** Include files ********************************/
#include "../headers/setup.h"

/*****************************    Defines    ********************************/
// Missing definitions in tm4c123gh6pm.h file.
#define NVIC_INT_CTRL_PEND_SYST   0x04000000  // Pend a systick int.
#define NVIC_INT_CTRL_UNPEND_SYST 0x02000000  // Unpend a systick int.

#define FCPU                 			16000000 		// Hz.
#define TIMEOUT              			200        	// ms.
#define SYSTICK_RELOAD_VALUE 			FCPU / 1000 * TIMEOUT-1

#define LED_RED 	0x02
#define LED_BLUE 	0x04
#define LED_GREEN 0x08
#define SW1_PIN 	0x10

#if (SYSTICK_RELOAD_VALUE > 0xFFFFFF)
	#error "SYSTICK_RELOAD_VALUE is too high"
#endif

/*****************************   Functions   ********************************/
void enable_global_int()
/**********************************************
* Input : None
* Output : None
* Function : Enable global interrupt.
**********************************************/
{
  __asm("cpsie i");	// Enable interrupts.
}

void disable_global_int()
/**********************************************
* Input : None
* Output : None
* Function : Disable global interrupt.
**********************************************/
{
  __asm("cpsid i");	// Disable interrupts.
}

void setup()
/**********************************************
* Input : None
* Output : None
* Function : Setup LEDs  and SW1, interrupts,
* 					 timer0 and systick timer.
**********************************************/
{
	// Enable the GPIO port that is used for the on-board LED.
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;

	// Waste a few cycles.
	INT8U i;
	for (i = 0; i < 5; i++);

	// Set the direction as output (PF1, 2, 3).
	GPIO_PORTF_DIR_R |= LED_RED | LED_GREEN | LED_BLUE;

	// Enable the GPIO pins for digital function (PF1, 2, 3, 4).
	GPIO_PORTF_DEN_R |= LED_RED | LED_GREEN | LED_BLUE | SW1_PIN;

	GPIO_PORTF_DIR_R   &= ~SW1_PIN;   	// Init SW1 as input.
  GPIO_PORTF_AFSEL_R &= ~SW1_PIN;  		// Disable alt funct on PF4.
  GPIO_PORTF_PCTL_R  &= ~0x000F0000; 	// Configure PF4 as GPIO.
  GPIO_PORTF_AMSEL_R &= ~SW1_PIN;  		// Disable analog functionality on PF4.
  GPIO_PORTF_PUR_R   |= SW1_PIN;     	// Enable weak pull-up on PF4.
  GPIO_PORTF_IS_R    &= ~SW1_PIN;     // PF4 is edge-sensitive.
  GPIO_PORTF_IBE_R   &= ~SW1_PIN;    	// PF4 is not both edges.
  GPIO_PORTF_IEV_R   &= ~SW1_PIN;     // PF4 falling edge event.
  GPIO_PORTF_ICR_R    = SW1_PIN;      // Clear flag 4.
	GPIO_PORTF_IM_R    |= SW1_PIN;			// Arm interrupt on PF4.

	NVIC_EN0_R 					= 0x40000000;   // Enable interrupt 30 in NVIC.

	// Setup timer0 for debouncing and push PF4 timing.
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0; 								// Enable the timer hardware.
  TIMER0_CTL_R 	 &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN); // Disable the timer.
	TIMER0_CFG_R 		= 0; 																	// 32 bit mode (no RTC).
	TIMER0_TAMR_R 	= TIMER_TAMR_TACDIR ;									// Up, periodic.
	TIMER0_CTL_R 		= TIMER_CTL_TAEN; 										// Enable Timer0_A.
	TIMER0_TAV_R 		= 0; 																	// Set timer to zero.

	// Setup systick timer for automode.
  NVIC_ST_CTRL_R 	 &= ~(NVIC_ST_CTRL_ENABLE); 					// Disable systick timer.

  // Set current systick counter to reload value.
  NVIC_ST_CURRENT_R = SYSTICK_RELOAD_VALUE;

  // Set Reload value, Systick reload register.
  NVIC_ST_RELOAD_R  = SYSTICK_RELOAD_VALUE;

  // NVIC systick setup, vector number 15.
  // Clear pending systick interrupt request.
  NVIC_INT_CTRL_R  |= NVIC_INT_CTRL_UNPEND_SYST;

  // Select systick clock source, Use core clock.
  NVIC_ST_CTRL_R   |= NVIC_ST_CTRL_CLK_SRC;
	
  // Enable systick interrupt.
  NVIC_ST_CTRL_R   |= NVIC_ST_CTRL_INTEN;

  // Don't start timer. This will be done when enabling automode.
	
  // Enable global interrupt.
  enable_global_int();
}

/****************************** End of module *******************************/
