#include "keyboard.h"
#include "../os/system_timers.h"
#include "FreeRTOS.h"
#include "task.h"
#include "../tasks/pump.h"

static bool push_key(char key);

static char lastkey = 0;
static INT8U keyboard_timer;
static bool key_pressed = 0;

void setup_keyboard(void)
{
	//Aquire ringbuffer
	keyboard_timer = request_timer();
	//Activate Port A and port E
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // activate port A
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE; // activate port E

  __asm__("NOP");
  __asm__("NOP");

	//Setup X1-X3 as output
	GPIO_PORTA_DIR_R 	 |= (KEYBOARD_X1 | KEYBOARD_X2 | KEYBOARD_X3);
	GPIO_PORTA_DEN_R 	 |= (KEYBOARD_X1 | KEYBOARD_X2 | KEYBOARD_X3); 							//Digital enable
	GPIO_PORTA_AFSEL_R &= ~(KEYBOARD_X1 | KEYBOARD_X2 | KEYBOARD_X3);							//Disable alt func.
  GPIO_PORTA_PCTL_R  &= ~(GPIO_PCTL_PA2_M | GPIO_PCTL_PA3_M | GPIO_PCTL_PA4_M); //Configure as GPIO.
  GPIO_PORTA_AMSEL_R &= ~(KEYBOARD_X1 | KEYBOARD_X2 | KEYBOARD_X3);  						//Disable analog functionality.

  //Setup Y1-Y4 as input
  GPIO_PORTE_DIR_R 	 &= ~(KEYBOARD_Y1 | KEYBOARD_Y2 | KEYBOARD_Y3 | KEYBOARD_Y4);
	GPIO_PORTE_DEN_R 	 |= (KEYBOARD_Y1 | KEYBOARD_Y2 | KEYBOARD_Y3 | KEYBOARD_Y4); 										//Digital enable
	GPIO_PORTE_AFSEL_R &= ~(KEYBOARD_Y1 | KEYBOARD_Y2 | KEYBOARD_Y3 | KEYBOARD_Y4);										//Disable alt func.
  GPIO_PORTE_PCTL_R  &= ~(GPIO_PCTL_PE0_M | GPIO_PCTL_PE1_M | GPIO_PCTL_PE2_M | GPIO_PCTL_PE3_M); 	//Configure as GPIO.
  GPIO_PORTE_AMSEL_R &= ~(KEYBOARD_Y1 | KEYBOARD_Y2 | KEYBOARD_Y3 | KEYBOARD_Y4);  																//Disable analog functionality.
	GPIO_PORTE_PUR_R   |=  ~(KEYBOARD_Y1 | KEYBOARD_Y2 | KEYBOARD_Y3 | KEYBOARD_Y4);     																									//Disable pull-up.
}





static bool push_key(char key)
{
	bool returnval = false;
	if(lastkey == key && is_timer_finished(keyboard_timer) && !key_pressed)
	{
		start_timer(keyboard_timer, KEYBOARD_REPEAT_TIME / SYS_TIMER_PERIODE);
		queue_item keyboard_item;
		keyboard_item.type = KEYBOARD;
		keyboard_item.value = key;
		xSemaphoreTake(pump_queue_sem, portMAX_DELAY);
		xQueueSendToBack(pump_queue, &keyboard_item, 0);
		xSemaphoreGive(pump_queue_sem);

		returnval = true;
	}
	else if(lastkey != key && !key_pressed)
	{
		lastkey = key;
		start_timer(keyboard_timer, KEYBOARD_REPEAT_TIME / SYS_TIMER_PERIODE);
		queue_item keyboard_item;
		keyboard_item.type = KEYBOARD;
		keyboard_item.value = key;
		xSemaphoreTake(pump_queue_sem, portMAX_DELAY);
		xQueueSendToBack(pump_queue, &keyboard_item, 0);
		xSemaphoreGive(pump_queue_sem);
		returnval = true;
	}
	key_pressed = true;
	return returnval;


}

void check_keyboard(void __attribute__((unused)) *pvParameters)
{	/*The pressed keys are found by putting charge on
		X1-X3 sequentially and for each of them, checking
		Y1-Y4 to see if they are high.
		Each pin will corespond to a key.
		Ghosting is ignored/not takken into account
	*/
	setup_keyboard();
	while(true)
	{
		key_pressed = 0;
		GPIO_PORTA_DATA_R |= KEYBOARD_X1;
		__asm__("NOP"); //Need to wait 1 cycle before checking
		INT8U in_keys = GPIO_PORTE_DATA_R & KEYBOARD_IN_M;
		if(in_keys & KEYBOARD_Y1) push_key('1');
		if(in_keys & KEYBOARD_Y2) push_key('4');
		if(in_keys & KEYBOARD_Y3) push_key('7');
		if(in_keys & KEYBOARD_Y4) push_key('*');
		GPIO_PORTA_DATA_R &= ~KEYBOARD_X1;

		GPIO_PORTA_DATA_R |= KEYBOARD_X2;
		__asm__("NOP");
		in_keys = GPIO_PORTE_DATA_R & KEYBOARD_IN_M;
		if(in_keys & KEYBOARD_Y1) push_key('2');
		if(in_keys & KEYBOARD_Y2) push_key('5');
		if(in_keys & KEYBOARD_Y3) push_key('8');
		if(in_keys & KEYBOARD_Y4) push_key('0');
		GPIO_PORTA_DATA_R &= ~KEYBOARD_X2;

		GPIO_PORTA_DATA_R |= KEYBOARD_X3;
		__asm__("NOP");
		in_keys = GPIO_PORTE_DATA_R & KEYBOARD_IN_M;
		if(in_keys & KEYBOARD_Y1) push_key('3');
		if(in_keys & KEYBOARD_Y2) push_key('6');
		if(in_keys & KEYBOARD_Y3) push_key('9');
		if(in_keys & KEYBOARD_Y4) push_key('#');
		GPIO_PORTA_DATA_R &= ~KEYBOARD_X3;
		if(key_pressed == 0)
		{
			stop_timer(keyboard_timer);
		}
		vTaskDelay(KEYBOARD_CHECK_PERIOD / portTICK_RATE_MS);
	}

}
