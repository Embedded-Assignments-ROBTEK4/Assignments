#include "../headers/scheduler.h"
#include "../headers/setup.h"
#include "../headers/leds.h"
#include "../headers/system_buffers.h"
#include "../headers/SWDelay.h"
void blink(void);

void blink(void)
{
	while(1)
	{
		wait(105);
		LED_RGB_PORT ^= LED_GREEN;
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

	add_task(blink);
	start_scheduler();

	return 0;
}
