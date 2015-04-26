#include "fuel.h"
#include "../../headers/emp_type.h"
#include "pump.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include <stdbool.h>
#include "../libs/print.h"
#include "../drivers/UART.h"
#include "../drivers/leds.h"

static INT32S fuel_level = 0;
static INT32S fuel_max   = 0;
static bool shunt = false;
static bool fueling = false;

void set_max_fuel(double max_fuel)
{
  fuel_level = 0;
  if(max_fuel == UNLIMITED)
    fuel_max = UNLIMITED;
  else
  {
    fuel_max = max_fuel * PULSES_PER_LITER * PULSE_MULTIPLICATION;
  }
}

void start_fuel(void)
{
  activate_shunt();
  fueling = true;
}

void stop_fuel(void)
{
  fueling = false;
  fuel_max -= fuel_level;
  fuel_level = 0;
  deactivate_shunt();
}

void activate_shunt()
{
  shunt = true;
  LED_RGB_PORT &= ~LED_YELLOW;
  //uart0_out_string("activated shunt\n");
}

void deactivate_shunt()
{
  shunt = false;
  LED_RGB_PORT |= LED_YELLOW;
  //uart0_out_string("deactivated shunt\n");
}

void fuel_task(void __attribute__((unused)) *pvParameters)
{
  portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  double goal_time = xLastWakeTime;
  //vprintf_(uart0_out_string, 200, "%f\n", PUMP_SPEED);
  //vprintf_(uart0_out_string, 200, "%f\n", WAIT_TIME);
  //vprintf_(uart0_out_string, 200, "%f\n", SHUNT_WAIT_TIME);

  while(true)
  {
    if(fueling)
    {
      fuel_level++;
      if(fuel_level % PULSE_MULTIPLICATION == 0)
      {
        queue_item fuel_item;
        fuel_item.type = FUEL;
        fuel_item.value =  FUEL_PULSE;
  			xSemaphoreTake(pump_queue_sem, portMAX_DELAY);
  			xQueueSendToBack(pump_queue, &fuel_item, 0);
  			xSemaphoreGive(pump_queue_sem);
      }
      if(fuel_max != UNLIMITED)
      {
        if((fuel_max - fuel_level) <= (SHUNT_LIMIT * PULSES_PER_LITER * PULSE_MULTIPLICATION) && !shunt)
        {
          activate_shunt();
        }
        if(fuel_level >= fuel_max)
        {
          stop_fuel();
          queue_item fuel_item;
          fuel_item.type = FUEL;
          fuel_item.value =  FUEL_DONE;
    			xSemaphoreTake(pump_queue_sem, portMAX_DELAY);
    			xQueueSendToBack(pump_queue, &fuel_item, 0);
    			xSemaphoreGive(pump_queue_sem);
        }
      }
      if(fuel_level > (SHUNT_LIMIT * PULSES_PER_LITER * PULSE_MULTIPLICATION))
      {
        if((fuel_max == UNLIMITED || (fuel_max - fuel_level) > (SHUNT_LIMIT * PULSES_PER_LITER * PULSE_MULTIPLICATION)) && shunt )
          deactivate_shunt();
      }
    }

    if(shunt)
    {
      goal_time += SHUNT_WAIT_TIME * portTICK_RATE_MS;
      portTickType tick_to_wait = ((portTickType)goal_time) - xLastWakeTime;
      //vprintf_(uart0_out_string, 200, "%d %d %d\n", (int)goal_time, (int) xLastWakeTime, tick_to_wait);

      vTaskDelayUntil(&xLastWakeTime, tick_to_wait );
    }
    else
    {
      goal_time += WAIT_TIME * portTICK_RATE_MS;
      portTickType tick_to_wait = ((portTickType)goal_time) - xLastWakeTime;
      //vprintf_(uart0_out_string, 200, "%d %d %d\n", (int)goal_time, (int) xLastWakeTime, tick_to_wait);
      vTaskDelayUntil(&xLastWakeTime, tick_to_wait );
    }
  }
}
