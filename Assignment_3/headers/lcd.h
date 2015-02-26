#ifndef _LCD_H_
#define _LCD_H_

#include <stdint.h>
#include "SWDelay.h"

/* This is a port of the arduino library LiquidCrystal.
** The library has been ported to C for use with the ARM Cortex M4
** It should be very easy to port to other architectures
*/

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

typedef struct {
  uint32_t _rs_pin; // LOW: command.  HIGH: character.
  uint32_t _enable_pin; // activated by a HIGH pulse.
  volatile uint32_t *_rs_enable_port;
  uint32_t _data_pins[8];
  volatile uint32_t *_data_port;

  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;

  uint8_t _initialized;

  uint8_t _numlines,_currline;
  } lcd;

	lcd lcd_init_8bit(uint32_t rs, uint32_t enable, volatile uint32_t *rs_enable_port,
		uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3,
		uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7, volatile uint32_t *data_port);
		
	lcd lcd_init_4bit(uint32_t rs, uint32_t enable, volatile uint32_t *rs_enable_port,
		uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t volatile *data_port);

  lcd lcd_init(uint8_t fourbitmode, uint32_t rs, uint32_t enable, volatile uint32_t *	rs_enable_port,
	    uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3,
	    uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7, volatile uint32_t *data_port);
    
	void lcd_begin(lcd *lcd_s, uint8_t lines);

	void lcd_home(lcd *lcd_s); //This takes a long time. Needs 1,6 ms before next command
	void lcd_clear(lcd *lcd_s); //This takes a long time. Needs 1,6 ms before next command
	
	void lcd_write_string(lcd *lcd_s, char *string);
	
	void lcd_set_cursor(lcd *lcd_s, uint8_t col, uint8_t row);
	void lcd_no_display(lcd *lcd_s);
	void lcd_display(lcd *lcd_s);
	void lcd_no_blink(lcd *lcd_s);
	void lcd_blink(lcd *lcd_s);
	void lcd_no_cursor(lcd *lcd_s);
	void lcd_cursor(lcd *lcd_s);
	void lcd_scroll_display_left(lcd *lcd_s);
	void lcd_scroll_display_right(lcd *lcd_s);
	void lcd_left_to_right(lcd *lcd_s);
	void lcd_right_to_left(lcd *lcd_s);
	void lcd_autoscroll(lcd *lcd_s);
	void lcd_no_autoscroll(lcd *lcd_s);

	void lcd_create_char(lcd *lcd_s, uint8_t location, uint8_t charmap[]);
	void lcd_write(lcd *lcd_s, uint8_t value);
	void lcd_command(lcd *lcd_s, uint8_t value);

	void lcd_send(lcd *lcd_s, uint8_t value, uint8_t mode);
	void lcd_write_4_bits(lcd *lcd_s, uint8_t value);
	void lcd_write_8_bits(lcd *lcd_s, uint8_t value);
	void lcd_pulse_enable(lcd *lcd_s);

#endif

