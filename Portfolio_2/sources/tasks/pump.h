#pragma once
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "queue.h"
#include "semphr.h"

#include "../../headers/emp_type.h"

#define PREPAID_ID 0
#define UPDATE_INTERVAL 1 //in ms
#define DENIED_SHOW_TIME 5000//in ms
#define VALVE_RELEASED_TIMEOUT 15000 //in ms

typedef enum
{
  KEYBOARD,
  BUTTON,
  FUEL,
  ENCODER,
} queue_type;

typedef struct
{
  queue_type type;
  INT8U value;
} queue_item;

typedef struct
{
  char   number[7];
  char   password[5];
  INT8U   id;
  double  balance;
} account;

typedef struct
{
  char name[20];
  double price;
} fuel;

typedef enum
{
  CHOOSE_PAYMENT,
  PREPAID,
  ACCOUNT_ID,
  ACCOUNT_CODE,
  ACCESS_DENIED,
  SELECT_FUEL_TYPE,
  FUELING,
  COMPLETE,
} pump_state;

typedef enum
{
  START,
  TRUNK_LIFTED,
  VALVE_PRESSED,
  STOP
} fueling_state;

extern xQueueHandle pump_queue;
extern xSemaphoreHandle pump_queue_sem;

void setup_pump(void);
void set_price(INT8U id, double price);
double get_price(INT8U id);

void pump_task(void __attribute__((unused)) *pvParameters);
