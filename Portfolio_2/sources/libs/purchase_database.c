#include "purchase_database.h"
#include "FreeRTOS.h"
bool add_purchase(purchase_database *purchase_db, purchase this_purchase)
{ //allocates heap for the purchase and add it to the database.
  xSemaphoreTake(purchase_db->sem, portMAX_DELAY);
  purchase *allocated_purchase = pvPortMalloc(sizeof(purchase));
  *allocated_purchase = this_purchase;
  if(allocated_purchase == NULL)
  {
    xSemaphoreGive(purchase_db->sem);
    return false;
  }
  //if puchase database is empty (pointer to start = null), set both pointers to this.
  if(purchase_db->first_purchase == NULL)
  {
    purchase_db->first_purchase = allocated_purchase;
    purchase_db->last_purchase = allocated_purchase;
    allocated_purchase->next_purchase = NULL;
    allocated_purchase->last_purchase = NULL;
  }
  //else, set last purchas in the list's next pointer to point to this.
  else
  {
    purchase_db->last_purchase->next_purchase = allocated_purchase;
    allocated_purchase->last_purchase = purchase_db->last_purchase;
    purchase_db->last_purchase = allocated_purchase;
    allocated_purchase->next_purchase = NULL;
  }
  xSemaphoreGive(purchase_db->sem);
  return true;
}

void delete_purchase(purchase_database *purchase_db) //deletes the last purchase in the database
{
  //check if this is the only one in the list
  xSemaphoreTake(purchase_db->sem, portMAX_DELAY);
  if(purchase_db->first_purchase == purchase_db->last_purchase)
  {
    vPortFree(purchase_db->last_purchase);
    purchase_db->first_purchase = NULL;
    purchase_db->last_purchase = NULL;
  }
  else
  {
    purchase *to_delete = purchase_db->last_purchase;
    purchase_db->last_purchase = to_delete->last_purchase;
    purchase_db->last_purchase->next_purchase = NULL;
    vPortFree(to_delete);
  }
  xSemaphoreGive(purchase_db->sem);
}
void delete_purchase_database(purchase_database *purchase_db) //delete all purchases in the database
{
  while(purchase_db->first_purchase != NULL)
    delete_purchase(purchase_db);
}
double get_total_purchase(purchase_database *purchase_db, INT8U account_id)
{ //return the total amount purchased by this account id.
  xSemaphoreTake(purchase_db->sem, portMAX_DELAY);
  purchase *current_purchase = purchase_db->first_purchase;
  double total = 0;
  while(current_purchase != NULL)
  {
    if(account_id == current_purchase->account_id)
      total += current_purchase->total_price;
    current_purchase = current_purchase->next_purchase;
  }
  xSemaphoreGive(purchase_db->sem);
  return total;
}

double get_total_amount(purchase_database *purchase_db, INT8U puchase_type)
{
  xSemaphoreTake(purchase_db->sem, portMAX_DELAY);
  purchase *current_purchase = purchase_db->first_purchase;
  double total = 0;
  while(current_purchase != NULL)
  {
    if(puchase_type == current_purchase->purchase_type)
      total += current_purchase->amount;
    current_purchase = current_purchase->next_purchase;
  }
  xSemaphoreGive(purchase_db->sem);
  return total;
}

void init_purchase_db(purchase_database *purchase_db)
{
  purchase_db->first_purchase = NULL;
  purchase_db->last_purchase = NULL;
  vSemaphoreCreateBinary(purchase_db->sem);
}
