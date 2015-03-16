#include "../headers/scheduler.h"
#include "../headers/setup.h"
#include "../headers/leds.h"
#include "../headers/system_buffers.h"
#include "../headers/SWDelay.h"
void blinkred(void);
void blinkgreen(void);
void blinkyellow(void);


void blinkgreen(void)
{
	while(1)
	{
		wait(90);
		LED_RGB_PORT ^= LED_GREEN;
	}
}
void blinkred(void)
{
	while(1)
	{
		wait(95);
		LED_RGB_PORT ^= LED_RED;
	}
}
void blinkyellow(void)
{
	while(1)
	{
		wait(105);
		LED_RGB_PORT ^= LED_BLUE;
	}
}


int main(void)
{
	disable_global_int();
	setup_leds();
	setup_systick();
	init_scheduler();
	sys_ringbuf_uchar_init();
	setup_delay(); //Used by lcd.
	enable_global_int();

	add_task(blinkred);
	add_task(blinkgreen);
	add_task(blinkyellow);

	start_scheduler();

	return 0;
}
