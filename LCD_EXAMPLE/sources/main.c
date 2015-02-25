#include "../headers/setup.h"
#include "../headers/lcd.h"
#include <stdlib.h>
#define LCD_D4 1<<4
#define LCD_D5 1<<5
#define LCD_D6 1<<6
#define LCD_D7 1<<7
#define LCD_RS 1<<2
#define LCD_E  1<<3
#define LCD_RS_E_PORT GPIO_PORTC_DATA_R
#define LCD_DATA_PORT GPIO_PORTD_DATA_R

char* itoa(int i, char b[]){ //Converts an int to a string 
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

int main()
{
	disable_global_int();
	setup_gpio();
	setup_delays();
	enable_global_int();

	lcd lcd_disp = lcd_init_4bit(LCD_RS, LCD_E, &LCD_RS_E_PORT, 
		LCD_D4, LCD_D5, LCD_D6, LCD_D7, &LCD_DATA_PORT);
		
	lcd_begin(&lcd_disp,2);
	
	int time = 0;
	char timestr[10];
	while(1)
	{
		time++;
		//delay_milliseconds(100);
		lcd_set_cursor(&lcd_disp,0,0);
		itoa(time,timestr);
		lcd_write_string(&lcd_disp,timestr);
	}
}
