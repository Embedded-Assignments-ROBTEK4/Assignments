#include "pump.h"
#include "debug_task.h"
#include "../drivers/UART.h"
#include "../libs/print.h"
#include "FreeRTOS.h"
#include "task.h"
#include "fuel.h"

void debug_task(void __attribute__((unused)) *pvParameters)
{
  //vprintf_(uart0_out_string, 200, "Debug Task Created");
  set_max_fuel(0.5);
  start_fuel();
  while(true)
  {

    bool got_item = false;
		queue_item item;
    //vprintf_(uart0_out_string, 200, "RECQUEST SEMAPHORE");

    xSemaphoreTake(pump_queue_sem, portMAX_DELAY);
    if(uxQueueMessagesWaiting(pump_queue))
    {
      got_item = true;
      xQueueReceive(pump_queue, &item, 0);
    }
    xSemaphoreGive(pump_queue_sem);
    if(got_item)
    {
      switch(item.type)
      {
        case KEYBOARD:
          vprintf_(uart0_out_string, 200, "KEYBOARD EVENT: %c\n", item.value);
          break;
        case BUTTON:
          vprintf_(uart0_out_string, 200, "BUTTON EVENT: %d\n", (int)item.value);
          break;
        case FUEL:
          vprintf_(uart0_out_string, 200, "FUEL EVENT: %d\n", (int)item.value);
          break;
        case ENCODER:
          vprintf_(uart0_out_string, 200, "ENCODER EVENT: %d\n", (int)item.value);
          break;
        default:
          break;
      }
    }
    vTaskDelay(10 / portTICK_RATE_MS);
  }
}
