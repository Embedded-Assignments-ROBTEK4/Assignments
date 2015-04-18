
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
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "status_led.h"
#include "os/system_buffers.h"
#include "drivers/UART.h"
#include "drivers/SSI0.h"
#include "drivers/SSI3.h"
#include "drivers/fpu.h"
#include "drivers/sysclk.h"
#include "drivers/lcd0.h"
#include "drivers/keyboard.h"
#include "os/system_timers.h"
#include "libs/print.h"
#include "drivers/adc.h"

/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void uart_task(void *pvParameters);
void lcd_test(void  *pvParameters);

static void setupHardware(void);

static void setupHardware(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :
*****************************************************************************/
{
  // TODO: Put hardware configuration and initialisation in here

  // Warning: If you do not initialize the hardware clock, the timings will be inaccurate
  set_sysclk(FCPU / 1000);
  enable_fpu();
  sys_ringbuf_uchar_init();
  setup_delay();
  setup_uart0();
  setup_lcd0();
  setup_keyboard();
  InitAdc();
  status_led_init();
}

void lcd_test(void __attribute__((unused)) *pvParameters)
{
  while(1)
  {
    lcd0_write_string("Test");
  	vTaskDelay(300 / portTICK_RATE_MS); // wait 100 ms.
  }
}


void keyboard_test_task(void __attribute__((unused)) *pvParameters);

void keyboard_test_task(void __attribute__((unused)) *pvParameters)
{
  while(true)
  {
    if(keyboard_data_available())
    {
      uart0_out_char(keyboard_in_char());
    }
  }
}
void ADC_test_taskforce(void __attribute__((unused)) *pvParameters);

void ADC_test_taskforce(void __attribute__((unused)) *pvParameters)
{
  while(1)
  {
    INT16U adcstuff = GetADC() * 0.5;
    lcd0_set_cursor(0,0);
    vprintf_(lcd0_write_string, 50, "%d\n", adcstuff);
  	vTaskDelay(5 / portTICK_RATE_MS); // wait 100 ms.
    lcd0_set_cursor(0,0);
    lcd0_write_string("     ");
  }
}

int main(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :
*****************************************************************************/
{
  portBASE_TYPE return_value = pdTRUE;
  setupHardware();
  /*
   Start the tasks defined within this file/specific to this demo.
   */
  return_value &= xTaskCreate( status_led_task, ( signed portCHAR * ) "Status_led", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( check_keyboard, ( signed portCHAR * ) "Keyboard Task", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( timer_task, ( signed portCHAR * ) "Status_led", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( keyboard_test_task, ( signed portCHAR * ) "ForCE1!1", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( ADC_test_taskforce, ( signed portCHAR * ) "ForCE1!1", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );

  if (return_value != pdTRUE)
  {
    GPIO_PORTD_DATA_R &= 0xBF;  // Turn on status LED.
    while(1);  // cold not create one or more tasks.
  }


  /*
   * Start the scheduler.
   */
  vTaskStartScheduler();

  /*
   * Will only get here if there was insufficient memory to create the idle task.
   */
  return 1;
}
