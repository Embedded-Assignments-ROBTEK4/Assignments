/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: main.c
*
* PROJECT....: Portfolio_2
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
#include "../headers/tm4c123gh6pm.h"
#include "../headers/emp_type.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "tasks/status_led.h"
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
#include "tasks/pump.h"
#include "tasks/clock.h"
#include "drivers/buttons.h"
#include "drivers/rotary_encoder0.h"
#include "libs/print.h"
#include "tasks/debug_task.h"
#include "tasks/fuel.h"
#include "drivers/leds.h"
#include "tasks/uart_task.h"
#include "tasks/rand_input_task.h"


/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE 200
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

static void setupHardware(void);

static void setupHardware(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :  Setup hardware taskes
*****************************************************************************/
{
  // Warning: If you do not initialize the hardware clock, the timings will be inaccurate
  enable_fpu();
  setup_pump();
  sys_ringbuf_uchar_init();
  setup_delay();
  setup_uart0();
  setup_lcd0();
  setup_leds();
  InitAdc();
  set_sysclk(FCPU / 1000);
  status_led_init();
}

int main(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :  
*****************************************************************************/
{
  portBASE_TYPE return_value = pdPASS;
  
  setupHardware();
<<<<<<< HEAD
  
  return_value &= xTaskCreate( status_led_task,       ( signed portCHAR * ) "Status_led",          USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( check_keyboard,        ( signed portCHAR * ) "Keyboard Task",       USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( timer_task,            ( signed portCHAR * ) "Timers task",         USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( run_clock,             ( signed portCHAR * ) "RTC task",            USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( collect_button_events, ( signed portCHAR * ) "button task",         USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( scan_encoder0,         ( signed portCHAR * ) "Rotary encoder task", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( fuel_task,             ( signed portCHAR * ) "fuel task",           USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( pump_task,             ( signed portCHAR * ) "pump task",           USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( uart_task,             ( signed portCHAR * ) "uart task",           USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  
=======
  return_value &= xTaskCreate( status_led_task, ( signed portCHAR * ) "Status_led", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( check_keyboard, ( signed portCHAR * ) "Keyboard Task", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( timer_task, ( signed portCHAR * ) "Timers task", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( run_clock, ( signed portCHAR * ) "RTC task", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( collect_button_events, ( signed portCHAR * ) "button task", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( scan_encoder0, ( signed portCHAR * ) "Rotary encoder task", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( fuel_task, ( signed portCHAR * ) "fuel task", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( pump_task, ( signed portCHAR * ) "pump task", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_value &= xTaskCreate( uart_task, ( signed portCHAR * ) "uart task", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  //return_value &= xTaskCreate( rand_input, ( signed portCHAR * ) "uart task", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );


>>>>>>> 328b7545371f6c5ddf5d93b68d0de960ec8cbc09
  if (return_value != pdPASS)
  {
    GPIO_PORTD_DATA_R &= 0xBF;  // Turn on status LED.
    while(1);                   // cold not create one or more tasks.
  }
  
  // Start the scheduler.
  vTaskStartScheduler();
  
  // Will only get here if there was insufficient memory to create the idle task.
  return 1;
}

/****************************** End of module *******************************/
