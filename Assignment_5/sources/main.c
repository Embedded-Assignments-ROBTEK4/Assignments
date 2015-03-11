#include "../headers/scheduler.h"
#include "../headers/systick.h"
#include "../headers/setup.h"
#include "../headers/system_buffers.h"
#include "../headers/UART.h"
#include "../headers/keyboard.h"
#include "../headers/buttons.h"
#include "../headers/clock.h"
#include "../headers/lcd0.h"
#include "../headers/display_clock.h"
#include "../headers/SWDelay.h"
#include "../headers/clock_uart0_controller.h"
int main(void)
{
	disable_global_int();
	setup_gpio(); 				// Setup SW1, SW2 and LCD pins.
	setup_systick(); 			// Setup systick timer.
	init_scheduler();
	sys_ringbuf_uchar_init();
	setup_uart0();
	setup_keyboard();
	setup_buttons();
	setup_clock();
	setup_delay(); //Used by lcd.
	setup_lcd0();
	enable_global_int();
	
	add_task(check_keyboard);
	add_task(collect_button_events);
	add_task(run_clock);
	add_task(display_clock);
	add_task(clock_uart0_controller);
	
	start_scheduler();
	
	return 0;
}
