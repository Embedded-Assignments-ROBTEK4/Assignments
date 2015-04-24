#include "rotary_encoder0.h"
#include <stdbool.h>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "queue.h"
#include "semphr.h"
#include "../libs/print.h"
#include "UART.h"

#include "../tasks/pump.h"
static INT32S encoder0_pos = 0;
static INT8U lastval;


void scan_encoder0(void __attribute__((unused)) *pvParameters)
{
  bool enc_button_last = 1;
  setup_encoder0();
  while(1)
  {
    //Read port status
    INT8U tmp = GPIO_PORTA_DATA_R & 0xFF;
    bool enc_a = tmp & DIGI_A;
    bool enc_b = tmp & DIGI_B;
    bool enc_switch = tmp & DIGI_P2;
    INT8U curval = (enc_a << 1) + enc_b;
    if(!enc_switch && enc_button_last != enc_switch)
    { //Don't do debouncing for now.
  		queue_item encoder_item;
  		encoder_item.type = ENCODER;
  		encoder_item.value = ENC_PUSH;
  		xSemaphoreTake(pump_queue_sem, portMAX_DELAY);
  		xQueueSendToBack(pump_queue, &encoder_item, 0);
  		xSemaphoreGive(pump_queue_sem);
    }

    else if(lastval !=  curval)
    {  //If last values LSB xor this values MSB is 1, we are going CW, else we are going CCW.
      if ( (lastval & 0x1) ^ (curval >> 1))
      {
        if(++encoder0_pos % 2 == 0)
        {
      		queue_item encoder_item;
      		encoder_item.type = ENCODER;
      		encoder_item.value = ENC_CW;
      		xSemaphoreTake(pump_queue_sem, portMAX_DELAY);
      		xQueueSendToBack(pump_queue, &encoder_item, 0);
      		xSemaphoreGive(pump_queue_sem);
        }
      }
      else
      {
        if(--encoder0_pos % 2 == 0)
        {
      		queue_item encoder_item;
      		encoder_item.type = ENCODER;
      		encoder_item.value = ENC_CCW;
      		xSemaphoreTake(pump_queue_sem, portMAX_DELAY);
      		xQueueSendToBack(pump_queue, &encoder_item, 0);
      		xSemaphoreGive(pump_queue_sem);
        }
      }
    }
    lastval = curval;
    enc_button_last = enc_switch;
  }
}

void setup_encoder0(void)
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // activate port A

  __asm__("NOP");
  //Setup pins as input
  GPIO_PORTA_DIR_R 	 &= ~ (DIGI_A | DIGI_B | DIGI_P2);
  GPIO_PORTA_DEN_R 	 |=   (DIGI_A | DIGI_B | DIGI_P2); 	//Digital enable
  GPIO_PORTA_AFSEL_R &= ~ (DIGI_A | DIGI_B | DIGI_P2);	//Disable alt func.
  GPIO_PORTA_PCTL_R  &= ~ (GPIO_PCTL_PA5_M | GPIO_PCTL_PA6_M | GPIO_PCTL_PA7_M); //Configure as GPIO.
  GPIO_PORTA_AMSEL_R &= ~ (DIGI_A | DIGI_B | DIGI_P2);  //Disable analog functionality.

  //Reset counter and read current value in.
  INT8U tmp = GPIO_PORTA_DATA_R & 0xFF;
  bool enc_a = tmp & DIGI_A;
  bool enc_b = tmp & DIGI_B;
  lastval = (enc_a << 1) + enc_b;
}
