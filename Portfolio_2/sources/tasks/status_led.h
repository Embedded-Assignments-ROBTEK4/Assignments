/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: status led.h
*
* PROJECT....: EMP
*
* DESCRIPTION: Test.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 100408  KHA    Module created.
*
*****************************************************************************/
#pragma once

#define STATUS_LED_PIN (1 << 6)
#define LED_STATUS_PORT GPIO_PORTD_DATA_R
/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/


void status_led_init(void);

void status_led_task(void *pvParameters);


/****************************** End Of Module *******************************/
