/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: pump.h
*
* PROJECT....: Portfolio_2
*
* DESCRIPTION: Handle paymeent and start/stop fulling
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150225  ALL		Created file.
* 150226	MS 		Fixed syntax.
*****************************************************************************/
#pragma once

/***************************** Include files ********************************/
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "queue.h"
#include "semphr.h"

#include "../../headers/emp_type.h"
#include "../libs/purchase_database.h"

/*******************************   Defines    *******************************/
#define PREPAID_ID 0
#define UPDATE_INTERVAL 1 //in ms
#define DENIED_SHOW_TIME 5000//in ms
#define FINISHED_SHOW_TIME 3000 //in ms
#define VALVE_RELEASED_TIMEOUT 15000 //in ms

/*******************************    Enums     *******************************/
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
} account;

typedef struct
{
  char name[20];
  double price;
  INT8U id;
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
  VALVE_RELEASED,
  STOP
} fueling_state;

/*****************************   Variables   ********************************/
extern xQueueHandle pump_queue;
extern xSemaphoreHandle pump_queue_sem;

void setup_pump(void);
void set_price(INT8U id, double price);
double get_price(INT8U id);
INT8U get_number_of_fuels(void);
purchase_database *get_purchase_db(void);
fuel *get_fuel(INT8U id);
INT8U get_number_of_accounts(void);

/*****************************   Functions   ********************************/
void pump_task(void __attribute__((unused)) *pvParameters);

/****************************** End of module *******************************/
