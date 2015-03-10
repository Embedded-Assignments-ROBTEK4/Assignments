#include "../headers/scheduler.h"
#include "../headers/systick.h"
#include "../headers/setup.h"
#include "../headers/system_buffers.h"
#include "../headers/UART.h"
#include "../headers/keyboard.h"
void uart_sender(void);
void uart_sender(void)
{
	if(keyboard_data_avaliable())
	{
		uart0_out_char(keyboard_in_char());
	}
}


int main(void)
{
	disable_global_int();
	setup_gpio(); 				// Setup SW1, SW2 and LCD pins.
	setup_systick(); 			// Setup systick timer.
	init_scheduler();
	sys_ringbuf_uchar_init();
	setup_uart0();
	setup_keyboard();
	enable_global_int();
	
	add_task(check_keyboard);
	add_task(uart_sender);
	
	
	start_scheduler();
	
	return 0;
}
