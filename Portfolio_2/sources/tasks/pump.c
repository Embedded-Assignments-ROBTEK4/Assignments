/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: pump.c
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
* 150225  ALL		Created file.
* 150226	MS 		Fixed syntax.
*****************************************************************************/

/***************************** Include files ********************************/
#include "pump.h"
#include "../os/system_timers.h"
#include "../drivers/lcd0.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "../../headers/emp_type.h"
#include "../drivers/rotary_encoder0.h"
#include "../libs/print.h"
#include "../drivers/buttons.h"
#include "../drivers/leds.h"
#include "fuel.h"
#include "../libs/purchase_database.h"
#include "clock.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

static bool get_event(queue_type type, INT8U *value);
static pump_state choose_payment(void);
static INT8U select_account_id(void);
static pump_state check_account_code(INT8U account_id);
static INT8U get_account_id(char *string);
static bool validate_account(INT8U id, char *code);
static void show_denied_dialog(void);
static fuel select_fuel_type(void);
static void do_fueling(INT32U prepaid_amount, INT8U account_id, fuel selected_fuel);
static void display_fueling(fuel selected_fuel, double pumped_amount);
static void do_accounting(INT8U account_id, double pumped_amount, fuel *fuel_type, double paid_amount);
static void display_finished_dialog(INT8U __attribute__((unused)) account_id, double pumped_amount, fuel *fuel_type);

xQueueHandle pump_queue;
xSemaphoreHandle pump_queue_sem;

static purchase_database purchase_db;



//no account should have id 0
static account __attribute__((unused)) accounts[] =
{
	{"000000", "0000", 1},
	{"111111", "1111", 2},
	{"123456", "1234", 3},
	{"654321", "4321", 4},
	{"222222", "2222", 5},
	{"333333", "3333", 6},
	{"444444", "4444", 7},
	{"987654", "9876", 8},
	{"######", "####", 9},
	{"875852", "2574", 10},
	{"******", "****", 11},

};

static fuel fuel_types[] =
{
	{"92 Octane",   12.17, 0},
	{"95 Octane",   11.10, 1},
	{"E10",         18.98, 2},
	{"Rocket Fuel", 56.27, 3}
};

/*****************************   Functions   *******************************/

INT8U get_number_of_accounts()
/**********************************************
* Input : -
* Output : Number of accounts
* Function : Return number of accounts
**********************************************/
{
	return sizeof(accounts) / sizeof(accounts[0]);
}

void set_price(INT8U id, double price)
/**********************************************
* Input : Fuel id and price.
* Output : -
* Function : Set price on fuel.
**********************************************/
{
	fuel_types[id].price = price;
}

fuel *get_fuel(INT8U id)
/**********************************************
* Input : Fuel id.
* Output : Fuel struct.
* Function : Return fuel struct based on id.
**********************************************/
{
	if(id >= get_number_of_fuels()) return NULL;
	else return fuel_types + id;
}

purchase_database *get_purchase_db()
/**********************************************
* Input : -
* Output : Database.
* Function : Return database.
**********************************************/
{
	return &purchase_db;
}

INT8U get_number_of_fuels(void)
/**********************************************
* Input : -
* Output : Number of fuel types.
* Function : Return number of fuel types.
**********************************************/
{
	return sizeof(fuel_types) / sizeof(fuel_types[0]);
}

static bool get_event(queue_type type, INT8U *value)
/**********************************************
* Input : -
* Output : -
* Function : 
**********************************************/
{
	queue_item item;

  while(true)
  {
    bool got_item = false;
    xSemaphoreTake(pump_queue_sem, portMAX_DELAY);
    if(uxQueueMessagesWaiting(pump_queue))
    {
      got_item = true;
      xQueueReceive(pump_queue, &item, 0);
    }
    xSemaphoreGive(pump_queue_sem);

    if(item.type == type)
    {
      *value = item.value;
      return true;
    }
    if(got_item == false) return false;
  }

}

void setup_pump()
/**********************************************
* Input : -
* Output : -
* Function : Setup pump.
**********************************************/
{
  pump_queue = xQueueCreate(128, sizeof(queue_item));
  vSemaphoreCreateBinary(pump_queue_sem);
	init_purchase_db(&purchase_db);
}

static pump_state choose_payment()
/**********************************************
* Input : -
* Output : Payment type.
* Function : Select and return selected payment type.
**********************************************/
{
  bool selected_type = true;
  while(1)
  {
    lcd0_set_cursor(0,0);
    lcd0_write_string("CHOOSE PAYMENT:");
    lcd0_set_cursor(0,1);
    if(selected_type == false)
      lcd0_write_string("Account");
    else
      lcd0_write_string("Cash");

    //Get event stuff
    INT8U event;
    if(get_event(ENCODER, &event))
    {
      if(event == ENC_CW || event == ENC_CCW)
      {
        selected_type ^= 1;
        lcd0_clear();
      }
      else if(event == ENC_PUSH)
      {
        lcd0_clear();
        if(selected_type)
          return PREPAID;
        else
          return ACCOUNT_ID;
      }
    }
    vTaskDelay(UPDATE_INTERVAL / portTICK_RATE_MS);
  }
}

static INT32U insert_cash(void)
/**********************************************
* Input : -
* Output : Inserted amount of chash.
* Function : Return inserted amount of chash.
**********************************************/
{
  INT32U inserted_cash = 0;
  while(true)
  {
    lcd0_set_cursor(0,0);
    lcd0_write_string("INSERT CASH:");
    lcd0_set_cursor(0,1);
    vprintf_(lcd0_write_string, 200, "Amount: %d DKK", (int)inserted_cash);
    INT8U event;
    if(get_event(ENCODER, &event))
    {
      if(event == ENC_CW)
      {
        inserted_cash += 50;
        lcd0_clear();
      }
      else if(event == ENC_CCW && inserted_cash != 0)
      {
        inserted_cash -=50;
        lcd0_clear();
      }
      else if(event == ENC_PUSH && inserted_cash != 0)
      {
        lcd0_clear();
        return inserted_cash;
      }
    }
    vTaskDelay(UPDATE_INTERVAL / portTICK_RATE_MS);
  }
}

INT8U get_account_id(char *string) //find the account id from the given string
/**********************************************
* Input : Account name
* Output : Account id
* Function : Return account id based on account name.
**********************************************/
{
  for(INT8U i = 0; i < sizeof(accounts) /sizeof(accounts[0]); i++)
  {
    if(strcmp(string, accounts[i].number) == 0)
    {
      return accounts[i].id;
    }
  }
  return 0;
}

static INT8U select_account_id()
/**********************************************
* Input : -
* Output : Account id
* Function : Select and return account id.
**********************************************/
{
  char entered_id[7] = "";
  INT8U index = 0;
  while(true)
  {
    lcd0_set_cursor(0,0);
    lcd0_write_string("ENTER ACCOUNT ID");
    lcd0_set_cursor(0,1);
    vprintf_(lcd0_write_string, 200, "ID: %s", entered_id);
    INT8U event;
    if(get_event(KEYBOARD, &event))
    {
      entered_id[index++] = event;
      entered_id[index] = '\0';
      if(index == 6)
      {
        lcd0_clear();
        return get_account_id(entered_id);
      }
    }
    vTaskDelay(UPDATE_INTERVAL / portTICK_RATE_MS);
  }
}

static bool validate_account(INT8U id, char *code)
/**********************************************
* Input : Account id and account code
* Output : Validation
* Function : Return validation of code and id
**********************************************/
{
  if(id && strcmp(code, accounts[id - 1].password) == 0)
    return true;
  else
    return 0;
}

static fuel select_fuel_type()
/**********************************************
* Input : -
* Output : fuel type
* Function : Select and return fuel type
**********************************************/
{
  INT8U selected_type = 0;
  while(1)
  {
    lcd0_set_cursor(0,0);
    lcd0_write_string("SELECT FUEL:");
    lcd0_set_cursor(0,1);
    //int whole_part_price = fuel_types[selected_type].price;
    //int decimal_part_price = (fuel_types[selected_type].price - (int)fuel_types[selected_type].price)*100;
    vprintf_(lcd0_write_string, 200, "%s", fuel_types[selected_type].name);
    //lcd0_set_cursor(11,1);
    //vprintf_(lcd0_write_string, 200, "%d,%d", whole_part_price, decimal_part_price);
    //Get event stuff
    INT8U event;
    if(get_event(ENCODER, &event))
    {
      if(event == ENC_CW)
      {
        if(!(++selected_type < sizeof(fuel_types) / sizeof(fuel_types[0]))) selected_type = 0;
        lcd0_clear();
      }
      else if(event == ENC_CCW)
      {
        if(!(--selected_type < sizeof(fuel_types) / sizeof(fuel_types[0]))) selected_type = sizeof(fuel_types)/sizeof(fuel_types[0]) - 1;
        lcd0_clear();
      }
      else if(event == ENC_PUSH)
      {
        lcd0_clear();
        return fuel_types[selected_type];
      }
    }
    vTaskDelay(UPDATE_INTERVAL / portTICK_RATE_MS);
  }
}

static pump_state check_account_code(INT8U account_id)
/**********************************************
* Input : Account id.
* Output : Fuel type.
* Function : Validata account, select fuel type and
             return the selected fuel type.
**********************************************/
{
  char entered_code[5] = "";
  INT8U index = 0;
  while(true)
  {
    lcd0_set_cursor(0,0);
    lcd0_write_string("ENTER CODE:");
    lcd0_set_cursor(0,1);
    vprintf_(lcd0_write_string, 200, "CODE: %s", entered_code);
    INT8U event;
    if(get_event(KEYBOARD, &event))
    {
      entered_code[index++] = event;
      entered_code[index] = '\0';
      if(index == 4)
      {
        lcd0_clear();
        if(validate_account(account_id, entered_code))
          return SELECT_FUEL_TYPE;
        else
          return ACCESS_DENIED;
      }
    }
    vTaskDelay(UPDATE_INTERVAL / portTICK_RATE_MS);
  }
}

void show_denied_dialog(void)
/**********************************************
* Input : -
* Output : -
* Function : Show denied dialog.
**********************************************/
{
  lcd0_set_cursor(0,0);
  lcd0_write_string("ACCESS DENIED!!!");
  lcd0_set_cursor(0,1);
  lcd0_write_string("WRONG ID OR CODE");
  vTaskDelay(DENIED_SHOW_TIME  / portTICK_RATE_MS);
  lcd0_clear();
}

static void display_finished_dialog(INT8U __attribute__((unused)) account_id, double pumped_amount, fuel *fuel_type)
/**********************************************
* Input : Account id, amount of pumped fuel and fuel type
* Output : -
* Function : Display finish pump status.
**********************************************/
{
  lcd0_set_cursor(0,0);
  lcd0_write_string("FUELING FINISHED");
  lcd0_set_cursor(0,1);
  vprintf_(lcd0_write_string, 200, "%fL %f DKK ", pumped_amount, fuel_type->price * pumped_amount);
  vTaskDelay(FINISHED_SHOW_TIME  / portTICK_RATE_MS);
  lcd0_clear();
}

static void do_accounting(INT8U account_id, double pumped_amount, fuel *fuel_type, double paid_amount)
/**********************************************
* Input : Account id, amount of pumped fuel and paid amount
* Output : -
* Function : Handle account data.
**********************************************/
{
	purchase new_purchase;
	new_purchase.amount = pumped_amount;
	new_purchase.purchase_type = fuel_type->id;
	new_purchase.total_price = paid_amount;
	new_purchase.time_of_day = get_clock();
	new_purchase.account_id = account_id;
	add_purchase(&purchase_db, new_purchase);
}


static void display_fueling(fuel selected_fuel, double pumped_amount)
/**********************************************
* Input : Fuel type, amount of pumped fuel.
* Output : -
* Function : Display fuel type and amount of pumped fuel and fuel price
**********************************************/
{
  lcd0_set_cursor(0, 0);
  vprintf_(lcd0_write_string, 200, "Fueled: %f L", pumped_amount);
  lcd0_set_cursor(0,1);
  vprintf_(lcd0_write_string, 200, "%f", selected_fuel.price);
  lcd0_set_cursor(6,1);
  vprintf_(lcd0_write_string, 200, "%f DKK", pumped_amount * selected_fuel.price);
}

void do_fueling(INT32U prepaid_amount, INT8U account_id, fuel selected_fuel)
/**********************************************
* Input : Prepayed amount of cash, account id and fuel type.
* Output : -
* Function : Handle fuelling and observing fuel price
**********************************************/
{
  INT8U fuel_timer = request_timer();
  double pumped_amount = 0;
  fueling_state state = START;
  INT8U event;
  while(true)
  {
    switch(state)
    {
      case START:
        lcd0_set_cursor(0,0);
        lcd0_write_string("Push SW2\nto lift trunk");
        if(get_event(BUTTON, &event))
        {
          if(event == SW2_RELEASED)
          {
            lcd0_clear();
            state = TRUNK_LIFTED;
            start_timer(fuel_timer, VALVE_RELEASED_TIMEOUT / SYS_TIMER_PERIODE);
            LED_RGB_PORT &= ~LED_RED;
          }
        }
        break;
      case TRUNK_LIFTED:
        display_fueling(selected_fuel, pumped_amount);
        if(is_timer_finished(fuel_timer))
        {
					lcd0_clear();
          state = STOP;
        }
        if(get_event(BUTTON, &event))
        {
          if(event == SW1_PRESSED)
          {
            state = VALVE_PRESSED;
            if(account_id != 0)
              set_max_fuel(UNLIMITED);
            else
            {
              double max_fuel = prepaid_amount / selected_fuel.price;
              set_max_fuel(max_fuel);
            }
            start_fuel();
            LED_RGB_PORT &= ~LED_GREEN;
          }
					else if(event == SW2_RELEASED)
					{
						lcd0_clear();
						state = STOP;
					}
        }

        break;
      case VALVE_PRESSED:
        display_fueling(selected_fuel, pumped_amount);
        while(true)
        {
          bool got_item = false;
          queue_item item;
          xSemaphoreTake(pump_queue_sem, portMAX_DELAY);
          if(uxQueueMessagesWaiting(pump_queue))
          {
            got_item = true;
            xQueueReceive(pump_queue, &item, 0);
          }
          xSemaphoreGive(pump_queue_sem);
          if(!got_item) break;
          if(item.type == BUTTON)
          {
            if(item.value == SW1_RELEASED)
            {
              start_timer(fuel_timer, VALVE_RELEASED_TIMEOUT / SYS_TIMER_PERIODE);
              state = VALVE_RELEASED;
              stop_fuel();
              LED_RGB_PORT |= LED_GREEN;
            }
          }
          else if(item.type == FUEL)
          {
            if(item.value == FUEL_PULSE)
            {
              pumped_amount += 1./PULSES_PER_LITER;
            }
            else if(item.value == FUEL_DONE)
            {
							lcd0_clear();
							LED_RGB_PORT |= LED_GREEN;
              state = STOP;
            }
          }
        }
        break;
			case VALVE_RELEASED:
				display_fueling(selected_fuel, pumped_amount);
        if(is_timer_finished(fuel_timer))
        {
					lcd0_clear();
          state = STOP;
        }
        while(true)
        {
          bool got_item = false;
          queue_item item;
          xSemaphoreTake(pump_queue_sem, portMAX_DELAY);
          if(uxQueueMessagesWaiting(pump_queue))
          {
            got_item = true;
            xQueueReceive(pump_queue, &item, 0);
          }
          xSemaphoreGive(pump_queue_sem);
          if(!got_item) break;
          if(item.type == BUTTON)
          {
            if(item.value == SW1_PRESSED)
            {
              state = VALVE_PRESSED;
							LED_RGB_PORT &= ~LED_GREEN;
              if(account_id != 0)
                set_max_fuel(UNLIMITED);

              start_fuel();
            }
            else if(item.value == SW2_RELEASED)
            {
							lcd0_clear();
              state = STOP;
            }
          }
          else if(item.type == FUEL)
          {
            if(item.value == FUEL_PULSE)
            {
              pumped_amount += 1./PULSES_PER_LITER;
            }
            else if(item.value == FUEL_DONE)
            {
							lcd0_clear();
              state = STOP;
            }
          }
        }
				break;
      case STOP:
        LED_RGB_PORT |= LED_RED;
				if(account_id == 0)
					do_accounting(account_id, pumped_amount, &selected_fuel, prepaid_amount);
				else
					do_accounting(account_id, pumped_amount, &selected_fuel, pumped_amount * selected_fuel.price);
				display_finished_dialog(account_id, pumped_amount, &selected_fuel);
        return;
    }
    vTaskDelay(UPDATE_INTERVAL / portTICK_RATE_MS);
  }
  release_timer(fuel_timer);
}

void pump_task(void __attribute__((unused)) *pvParameters)
/**********************************************
* Input : -
* Output : -
* Function : Handle paymeent and start/stop fulling
**********************************************/
{
  pump_state state = CHOOSE_PAYMENT;
	//INT8U pump_timer = request_timer();
  INT8U account_id = 0;
  INT32U prepaid_amount = 0;
  fuel selected_fuel = {"", 0, 0};
  while(true)
  {
    switch(state)
    {
      case CHOOSE_PAYMENT:
        state = choose_payment();
        break;
      case PREPAID:
        prepaid_amount = insert_cash();
        state = SELECT_FUEL_TYPE;
        account_id = PREPAID_ID;
        break;
      case ACCOUNT_ID:
        account_id = select_account_id();
        state = ACCOUNT_CODE;
        break;
      case ACCOUNT_CODE:
        state = check_account_code(account_id);
        break;
      case ACCESS_DENIED:
        show_denied_dialog();
        state = CHOOSE_PAYMENT;
        break;
      case SELECT_FUEL_TYPE:
        selected_fuel = select_fuel_type();
        state = FUELING;
        break;
      case FUELING:
          do_fueling(prepaid_amount, account_id, selected_fuel);
          state = COMPLETE;
        break;
      case COMPLETE:
				state = CHOOSE_PAYMENT;
        break;
      default:
        break;
    }
    vTaskDelay(UPDATE_INTERVAL / portTICK_RATE_MS);
  }
}

/****************************** End of module *******************************/
