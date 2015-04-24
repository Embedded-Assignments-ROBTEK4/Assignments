/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: button_events.c
*
* PROJECT....: Assingment 3
*
* DESCRIPTION: See header.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150217  StefanRvo		Created file.
* 150226	MS 					Fixed syntax.
*****************************************************************************/

/***************************** Include files ********************************/
#include "buttons.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "../tasks/pump.h"

/***************************** Enums ****************************************/

typedef enum {
	NO_EVENT,
	PUSHED,
	RELEASED,
} event;

typedef enum {
	IDLE,
	FIRST_PUSH,
	FIRST_RELEASE,
} button_state;

/*****************************   Structs ************************************/
typedef struct {
	volatile uint32_t *port;
	uint32_t pin;
	button_state state;
} button;

/*****************************  Function defs *******************************/
static bool button_pressed(button *button_s);
static event get_button_event(button *button_s);

/******************************	Variables ***********************************/
	static button sw1_button = {&GPIO_PORTF_DATA_R, SW1_PIN, IDLE};
	static button sw2_button = {&GPIO_PORTF_DATA_R, SW2_PIN, IDLE};


/*****************************   Functions   ********************************/
bool button_pressed(button *button_s)
/**********************************************
* Input : Port, pin.
* Output : Output false if pin is high, true if low.
* Function : Check if button is pressed.
**********************************************/
{
	return !(*(button_s->port) & button_s->pin);
}

event get_button_event(button *button_s)
/**********************************************
* Input : Button struct - see header.
* Output : Button event.
* Function : Check if a button event have occurred and returns the event.
**********************************************/
{
	event button_event = NO_EVENT;


	switch(button_s->state)
	{
		case IDLE:
			if(button_pressed(button_s))
			{
				button_s->state = FIRST_PUSH;
				button_event		= PUSHED;
			}
			break;

		case FIRST_PUSH:
			if(!button_pressed(button_s))
			{
				button_s->state = FIRST_RELEASE;
			}
			break;

		case FIRST_RELEASE:
			button_s->state	= IDLE;
			button_event		= RELEASED;
			break;

		default:
			break;
	}
	return button_event;
}


void collect_button_events(void __attribute__((unused)) *pvParameters)
{
	setup_buttons();
	while(true)
	{
		event sw_event = get_button_event(&sw1_button);
		if(sw_event == PUSHED)
		{
			queue_item button_item;
			button_item.type = BUTTON;
			button_item.value = SW1_PRESSED;
			xSemaphoreTake(pump_queue_sem, portMAX_DELAY);
			xQueueSendToBack(pump_queue, &button_item, 0);
			xSemaphoreGive(pump_queue_sem);
		}
		else if(sw_event == RELEASED)
		{
			queue_item button_item;
			button_item.type = BUTTON;
			button_item.value = SW1_RELEASED;
			xSemaphoreTake(pump_queue_sem, portMAX_DELAY);
			xQueueSendToBack(pump_queue, &button_item, 0);
			xSemaphoreGive(pump_queue_sem);
		}

		sw_event = get_button_event(&sw2_button);
		if(sw_event == PUSHED)
		{
			queue_item button_item;
			button_item.type = BUTTON;
			button_item.value = SW2_PRESSED;
			xSemaphoreTake(pump_queue_sem, portMAX_DELAY);
			xQueueSendToBack(pump_queue, &button_item, 0);
			xSemaphoreGive(pump_queue_sem);
		}
		else if(sw_event == RELEASED)
		{
			queue_item button_item;
			button_item.type = BUTTON;
			button_item.value = SW2_RELEASED;
			xSemaphoreTake(pump_queue_sem, portMAX_DELAY);
			xQueueSendToBack(pump_queue, &button_item, 0);
			xSemaphoreGive(pump_queue_sem);
		}
		vTaskDelay(BUTTON_CHECK_PERIOD / portTICK_RATE_MS); // wait 100 ms.
	}
}


void setup_buttons(void)
{ //init pins for SW1 and SW2
	SYSCTL_RCGC2_R 		|= SYSCTL_RCGC2_GPIOF;
	GPIO_PORTF_DIR_R  &= ~(SW1_PIN | SW2_PIN); // Init SW1 and SW2 as input.

	//Unlock PF0
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R  |= 0x01;

	GPIO_PORTF_DEN_R |= SW1_PIN | SW2_PIN;

	GPIO_PORTF_DIR_R   &= ~(SW1_PIN | SW2_PIN);   							// Init SW1 as input.
  GPIO_PORTF_AFSEL_R &= ~(SW1_PIN | SW2_PIN);  								// Disable alt funct
  GPIO_PORTF_PCTL_R  &= ~(GPIO_PCTL_PF4_M | GPIO_PCTL_PF0_M); // Configure as GPIO.
  GPIO_PORTF_AMSEL_R &= ~(SW1_PIN | SW2_PIN);  								// Disable analog functionality
  GPIO_PORTF_PUR_R   |=  (SW1_PIN | SW2_PIN);     						// Enable weak pull-up.

}
/****************************** End of module *******************************/
