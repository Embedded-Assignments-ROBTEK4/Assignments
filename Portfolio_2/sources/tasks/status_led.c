/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: leds.c
*
* PROJECT....: ECP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "../../headers/tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "../../headers/emp_type.h"
#include "../../headers/GLOBAL_DEFINITIONS.h"
#include "status_led.h"


/*****************************    Defines    *******************************/
#define PF0		0		// Bit 0

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void status_led_init(void)
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function :
*****************************************************************************/
{
  // Enable the GPIO port that is used for the on-board LED.
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;

  __asm__("NOP");

  GPIO_PORTD_DIR_R |= STATUS_LED_PIN;
  GPIO_PORTD_DEN_R |= STATUS_LED_PIN;
}


void status_led_task(void __attribute__((unused)) *pvParameters)
{

	while(1)
	{
		// Toggle status led
    LED_STATUS_PORT ^= STATUS_LED_PIN;
		vTaskDelay(500 / portTICK_RATE_MS); // wait 500 ms.
	}
}


/****************************** End Of Module *******************************/
