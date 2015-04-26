#include "pump.h"

#include <stdlib.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

void rand_input(void __attribute__((unused)) *pvParameters);


void rand_input(void __attribute__((unused)) *pvParameters)
{
  while(true)
  {
    queue_item item;
    item.type = rand() % 4;
    item.value = rand()% 10;
    xSemaphoreTake(pump_queue_sem, portMAX_DELAY);
    xQueueSendToBack(pump_queue, &item, 0);
    xSemaphoreGive(pump_queue_sem);
    //vTaskDelay(1 / portTICK_RATE_MS);
  }
}
