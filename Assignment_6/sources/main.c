#include "../headers/scheduler.h"
#include "../headers/systick.h"
#include "../headers/setup.h"
#include "../headers/leds.h"
#include "../headers/system_buffers.h"
#include "../headers/UART.h"
#include "../headers/keyboard.h"
#include "../headers/buttons.h"
#include "../headers/clock.h"
#include "../headers/lcd0.h"
#include "../headers/display_clock.h"
#include "../headers/SWDelay.h"
#include "../headers/clock_uart0_controller.h"
#include "../headers/clock_keyboard_controller.h"
#include "../headers/rotary_encoder0.h"
#include "../headers/print.h"
#include "../headers/alarm_clock.h"
void ledblink(void);
int main(void)
{
	disable_global_int();
	setup_leds();
	setup_systick();
	init_scheduler();
	sys_ringbuf_uchar_init();
	setup_uart0();
	setup_keyboard();
	setup_buttons();
	setup_clock();
	setup_delay(); //Used by lcd.
	setup_lcd0();
	setup_encoder0();
	enable_global_int();
	add_task(check_keyboard);
	add_task(collect_button_events);
	add_task(run_clock);
	add_task(display_clock);
	add_task(clock_uart0_controller);
	add_task(clock_keyboard_controller);
	add_task(scan_encoder0);
	add_task(alarm_task);
	start_scheduler();

	return 0;
}
