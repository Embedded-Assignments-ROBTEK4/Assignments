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
    fuel_max = max_fuel * PULSES_PER_LITER * PULSE_MULTIPLICATION;
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
}

void activate_shunt()
{
  shunt = true;
  //vprintf_(uart0_out_string, 200, "activated shunt\n");
}

void deactivate_shunt()
{
  shunt = false;
  //vprintf_(uart0_out_string, 200, "deactivated shunt\n");
}

void fuel_task(void __attribute__((unused)) *pvParameters)
{
  portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  double goal_time = xLastWakeTime;
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
        if(fuel_max - fuel_level <= SHUNT_LIMIT * PULSES_PER_LITER * PULSE_MULTIPLICATION)
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

      if(fuel_max - fuel_level > SHUNT_LIMIT * PULSES_PER_LITER * PULSE_MULTIPLICATION &&
          fuel_level >= SHUNT_LIMIT * PULSES_PER_LITER * PULSE_MULTIPLICATION  )
      {
        deactivate_shunt();
      }
    }

    if(shunt)
    {
      goal_time += SHUNT_WAIT_TIME;
      vTaskDelayUntil(&xLastWakeTime, ((portTickType)goal_time - xLastWakeTime)   / portTICK_RATE_MS );

    }
    else
    {
      goal_time += WAIT_TIME;
      vTaskDelayUntil(&xLastWakeTime, ((portTickType)goal_time - xLastWakeTime)   / portTICK_RATE_MS );
    }
  }
}
