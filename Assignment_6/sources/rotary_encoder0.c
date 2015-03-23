#include "../headers/rotary_encoder0.h"
#include <stdbool.h>
static INT32S encoder0_pos = 0;
static bool last[2];

INT32S get_encoder0_pos(void)
{
  return encoder0_pos;
}

void reset_encoder0(void)
{
  encoder0_pos = 0;
}

void scan_encoder0(void)
{
  //Read port status
  INT8U tmp = GPIO_PORTA_DATA_R & 0xFF;
  bool enc_a = tmp & DIGI_A;
  bool enc_b = tmp & DIGI_B;
  bool enc_switch = tmp & DIGI_P2;
  if(!enc_switch)
  { //reset
    encoder0_pos = 0;
  }
  else if (last[0] && last[1]) //11
  {
    if      (!enc_a && enc_b) encoder0_pos++; //count up
    else if (enc_a && !enc_b) encoder0_pos--; //count down
  }
  else if (!last[0] && last[1]) //01
  {
    if      (!enc_a && !enc_b) encoder0_pos++; //count up
    else if (enc_a && enc_b)   encoder0_pos--; //count down
  }
  else if (!last[0] && !last[1]) //00
  {
    if      (enc_a && !enc_b) encoder0_pos++; //count up
    else if (!enc_a && enc_b) encoder0_pos--; //count down
  }
  else if (last[0] && !last[1]) //10
  {
    if      (enc_a && enc_b) encoder0_pos++; //count up
    else if (!enc_a && !enc_b) encoder0_pos--; //count down
  }

  last[0] = enc_a;
  last[1] = enc_b;
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
  reset_encoder0();
  INT8U tmp = GPIO_PORTA_DATA_R & 0xFF;
  last[0] = tmp & DIGI_A;
  last[1] = tmp & DIGI_B;
}
