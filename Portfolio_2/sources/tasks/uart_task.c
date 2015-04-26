#include "uart_task.h"
#include "../drivers/UART.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "pump.h"
#include <math.h>
#include "../libs/print.h"
#include "../libs/purchase_database.h"
#include "../tasks/clock.h"

static INT8U check_for_command(char *buffer, INT8U index, size_t size);
static void set_price_command(void);
static void send_report(void);
double get_total_account_purchases(void);

double get_total_account_purchases(void)
{
  purchase_database *purchase_db = get_purchase_db();
  INT8U number_of_accounts = get_number_of_accounts();
  double total = 0;
  for(INT8U i = 0; i < number_of_accounts; i++)
  {
    total += get_total_purchase(purchase_db, i + 1);
  }
  return total;
}



static void send_report()
{
  //get the database
  purchase_database *purchase_db = get_purchase_db();
  INT8U number_of_fuels = get_number_of_fuels();
  vprintf_(uart0_out_string, 200, "\n\n\nOPERATION REPORT:\n");

  vprintf_(uart0_out_string, 200, "\nTotal sales of gasoline:\n");

  for(INT8U i = 0; i < number_of_fuels; i++)
  {
    vprintf_(uart0_out_string, 200, "%s:   %f L\n", get_fuel(i)->name,
            get_total_amount(purchase_db, i));
  }
  vprintf_(uart0_out_string, 200, "\nCash purchases:\n%f DKK\n", get_total_purchase(purchase_db, 0));

  vprintf_(uart0_out_string, 200, "\nAccount purchases:\n%f DKK\n", get_total_account_purchases());

  vprintf_(uart0_out_string, 200, "\nTotal operation time:\n%d s\n", (int)get_operating_time());

}





command commands[] =
{
  {"set price ", 1, 10},
  {"report",    2, 6}
};

static void set_price_command()
{
  //first get id for the fuel type
  INT8U recived_count = 0;
  INT8U type = 0;
  double price = 0;
  while(1)
  {
    while(uart0_data_available())
    {
      if(recived_count == 0)
      {
        type = uart0_in_char() - '0';
        if(type >= get_number_of_fuels()) return;
      }
      else if(recived_count == 1)
      {
        if(uart0_in_char() != ' ') return;
      }
      else
      {
        INT8U rec_char = uart0_in_char() - '0';
        if(rec_char >= 10)
        {
          set_price(type, price / 100);
          return;
        }
        else
        {
          price *= 10;
          price += rec_char;
        }
      }

      recived_count++;
    }
    vTaskDelay(CHECK_INTERVAL / portTICK_RATE_MS);
  }
}

static INT8U check_for_command(char *buffer, INT8U index, size_t size)
{
  //create string from buffer
  char * test_str = pvPortMalloc(size);
  if(test_str == NULL) return 0;
  //copy buffer content to string
  for(INT8U i = 0; i < size; i++)
  {
    test_str[i] = buffer[(i + index + 1) % size];
  }
  for(INT8U i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
  {
    if(strcmp(commands[i].com, test_str + size - 1 - commands[i].size) == 0)
    {
        vPortFree(test_str);
        return commands[i].id;
    }
  }
  vPortFree(test_str);
  return 0;
}

void uart_task(void __attribute__((unused)) *pvParameters)
{
  char buffer[20];
  INT8U buffer_index = 0;
  while(true)
  {
    while(uart0_data_available())
    {
      buffer[buffer_index++] = uart0_in_char();
      if(buffer_index >= sizeof(buffer) / sizeof(buffer[0]))
        buffer_index = 0;
      buffer[buffer_index] = '\0';

      INT8U command_id = check_for_command(buffer, buffer_index, sizeof(buffer) / sizeof(buffer[0]) );
      switch (command_id)
      {
        case 1:
          set_price_command();
          break;
        case 2:
          send_report();
          break;
      }
    }
    vTaskDelay(CHECK_INTERVAL / portTICK_RATE_MS);
  }
}
