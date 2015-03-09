#include "../headers/scheduler.h"
#include "../headers/systick.h"
#include "../headers/setup.h"
void green_led_blink(void);

void green_led_blink()
{
	GPIO_PORTF_DATA_R ^= LED_GREEN;
	wait(1000 / TIMEOUT_SYSTICK); //wait 1000 ms.
}
int main(void)
{
	disable_global_int();
	setup_gpio(); 				// Setup SW1, SW2 and LCD pins.
	setup_systick(); 			// Setup systick timer.
	enable_global_int();
	init_scheduler();
	
	add_task(green_led_blink);
	
	start_scheduler();
	
	return 0;
}
