#pragma once
#include "../../headers/emp_type.h"

#define CHECK_INTERVAL 5
#define NUMBER_OF_FUEL_TYPES
typedef struct
{
  char com[20];
  INT8U id;
  INT8U size;
} command;

void uart_task(void __attribute__((unused)) *pvParameters);
