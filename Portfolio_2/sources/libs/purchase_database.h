//implement a list of purchases as a linked list
//implemented for purchases of fuel
#pragma once
#include <stdbool.h>
#include "../../headers/emp_type.h"
#include "time.h"

typedef struct purchase purchase;

struct purchase
{
  INT8U purchase_type;
  double amount;
  double total_price;
  time time_of_day;
  struct purchase *next_purchase;
  struct purchase *last_purchase;
  INT8U account_id;
};

typedef struct
{
  purchase *first_purchase;
  purchase *last_purchase;
} purchase_database;

bool add_purchase(purchase_database *puchase_db, purchase this_purchase);
void delete_purchase(purchase_database *purchase_db); //deletes the last purchase in the database
void delete_purchase_database(purchase_database *purchase_db); //delete all purchases in the database
double get_total_purchase(purchase_database *purchase_db, INT8U account_id);
double get_total_amount(purchase_database *purchase_db, INT8U puchase_type);
