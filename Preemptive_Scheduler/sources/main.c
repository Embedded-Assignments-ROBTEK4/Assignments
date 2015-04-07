#include "../headers/scheduler.h"
#include "../headers/setup.h"
#include "../headers/leds.h"
#include "../headers/system_buffers.h"
#include "../headers/SWDelay.h"
#include "../headers/UART.h"
#include "../headers/clock.h"
#include "../headers/buttons.h"
#include "../headers/lcd0.h"
#include "../headers/display_clock.h"
#include "../headers/clock_uart0_controller.h"

void blinkred(void);
void blinkgreen(void);
void blinkyellow(void);

void blinkgreen()
{
	while(true)
	{
		LED_RGB_PORT ^= LED_GREEN;
		wait(200);
 	}
}

void blinkyellow()
{
	while(true)
	{
		LED_RGB_PORT ^= LED_BLUE;
	}
}

void blinkred()
{
	while(true)
	{
		LED_RGB_PORT ^= LED_RED;
		wait(100);

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
	setup_uart0();
	setup_buttons();
	setup_clock();
	setup_lcd0();

	enable_global_int();

       add_task(collect_button_events);
       add_task(run_clock);
       add_task(display_clock);
       add_task(clock_uart0_controller);


	start_scheduler();

	return 0;
}
