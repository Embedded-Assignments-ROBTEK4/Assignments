#include "../headers/display_clock.h"
void display_clock(lcd *lcd_disp, time *time_s)
{
	static time time_last = {99, 99, 99};
	
	if(time_last.hour != time_s->hour) //update hour if changed
	{
		lcd_set_cursor(lcd_disp,0,0);
		lcd_write(lcd_disp, '0' + time_s->hour / 10);
		lcd_write(lcd_disp, '0' + time_s->hour % 10);
	}
	if(time_last.sec != time_s->sec) //update collon if second changed
	{
		lcd_set_cursor(lcd_disp,2,0);
		if(time_s->sec % 2 == 0)
			lcd_write(lcd_disp,':');
		else
			lcd_write(lcd_disp,' ');
	}
	if(time_last.min != time_s->min) //update min if changed
	{
		lcd_set_cursor(lcd_disp,3,0);
		lcd_write(lcd_disp, '0' + time_s->min / 10);
		lcd_write(lcd_disp, '0' + time_s->min % 10);
	}

	time_last = *time_s;
}
