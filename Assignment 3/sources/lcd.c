#include "../headers/lcd.h"

#include <stdio.h>
#include <string.h>
/* This is a port of the arduino library LiquidCrystal.
** The library has been ported to C for use with the ARM Cortex M4
** It should be very easy to port to other architectures
*/



// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the device doesn't reset the LCD, so we
// can't assume that its in that state when powered up.

 lcd lcd_init_8bit(uint32_t rs, uint32_t enable, volatile uint32_t *rs_enable_port,
		uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3,
		uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7, volatile uint32_t *data_port)
{
  return lcd_init(0, rs, enable, rs_enable_port, d0, d1, d2, d3, d4, d5, d6, d7, data_port);
}


 lcd lcd_init_4bit(uint32_t rs, uint32_t enable, volatile uint32_t *rs_enable_port,
		uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t volatile *data_port)
{
  return lcd_init(1, rs, enable, rs_enable_port, d0, d1, d2, d3, 0, 0, 0, 0, data_port);
}

  lcd lcd_init(uint8_t fourbitmode, uint32_t rs, uint32_t enable, volatile uint32_t *	rs_enable_port,
	    uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3,
	    uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7, volatile uint32_t *data_port)
{
	lcd lcd_s;
  lcd_s._rs_pin = rs;
  lcd_s._enable_pin = enable;
  lcd_s._rs_enable_port = rs_enable_port;
  lcd_s._data_pins[0] = d0;
  lcd_s._data_pins[1] = d1;
  lcd_s._data_pins[2] = d2;
  lcd_s._data_pins[3] = d3; 
  lcd_s._data_pins[4] = d4;
  lcd_s._data_pins[5] = d5;
  lcd_s._data_pins[6] = d6;
  lcd_s._data_pins[7] = d7;
  lcd_s._data_port = data_port;

	//Don't setup pins, should be done beforehand
  
  if (fourbitmode)
    lcd_s._displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  else 
    lcd_s._displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
  
  return lcd_s;
}

void lcd_begin(lcd *lcd_s, uint8_t lines) {
	uint8_t dotsize = LCD_5x8DOTS;
  if (lines > 1)
    lcd_s->_displayfunction |= LCD_2LINE;

  lcd_s->_numlines = lines;
  lcd_s->_currline = 0;

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != 0) && (lines == 1))
    lcd_s->_displayfunction |= LCD_5x10DOTS;

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands.
  delay_microseconds(40000); 
  // Now we pull both RS and R/W low to begin commands
  *(lcd_s->_rs_enable_port) &= ~(lcd_s->_rs_pin);
  *(lcd_s->_rs_enable_port) &= ~(lcd_s->_enable_pin);
  
  	//put the LCD into 4 bit or 8 bit mode
    // this is according to the DEM 16216 SYH-PY datasheet 
    // figure 11-2, pg 13

   if (! (lcd_s->_displayfunction & LCD_8BITMODE)) //4 bit setup
   {
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    lcd_write_4_bits(lcd_s, 0x03);
    delay_microseconds(4500); // wait min 4.1ms

    // second try
    lcd_write_4_bits(lcd_s, 0x03);
    delay_microseconds(4500); // wait min 4.1ms
    
    // third go!
    lcd_write_4_bits(lcd_s, 0x03); 
    delay_microseconds(150);

    // finally, set to 4-bit interface
    lcd_write_4_bits(lcd_s, 0x02); 
  } 
  else  //8 bit setup
  {
    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    lcd_command(lcd_s, LCD_FUNCTIONSET | lcd_s->_displayfunction);
    delay_microseconds(4500);  // wait more than 4.1ms

    // second try
    lcd_command(lcd_s, LCD_FUNCTIONSET | lcd_s->_displayfunction);
    delay_microseconds(150);

    // third go
    lcd_command(lcd_s, LCD_FUNCTIONSET | lcd_s->_displayfunction);
  }

  // finally, set # lines, font size, etc.
  lcd_command(lcd_s, LCD_FUNCTIONSET | lcd_s->_displayfunction);  

  // turn the display on with no cursor or blinking default
  lcd_s->_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  lcd_display(lcd_s);

  // clear it off
  lcd_clear(lcd_s);
  delay_microseconds(2000);

  // Initialize to default text direction (for romance languages)
  lcd_s->_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  lcd_command(lcd_s, LCD_ENTRYMODESET | lcd_s->_displaymode);

}

/********** high level commands, for the user! */
void lcd_write_string(lcd *lcd_s, char *string)
{
	INT8U i = 0;
	while(string[i])
	{
		lcd_write(lcd_s,(unsigned char)string[i]);
		i++;
	}
}
void lcd_clear(lcd *lcd_s)
{
  lcd_command(lcd_s, LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
}

void lcd_home(lcd *lcd_s)
{
  lcd_command(lcd_s, LCD_RETURNHOME);  // set cursor position to zero
}

void lcd_set_cursor(lcd *lcd_s, uint8_t col, uint8_t row)
{
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if ( row > lcd_s->_numlines ) {
    row = lcd_s->_numlines-1;    // we count rows starting w/0
  }
  
  lcd_command(lcd_s, LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void lcd_no_display(lcd *lcd_s) {
  lcd_s->_displaycontrol &= ~LCD_DISPLAYON;
  lcd_command(lcd_s, LCD_DISPLAYCONTROL | lcd_s->_displaycontrol);
}
void lcd_display(lcd *lcd_s) {
  lcd_s->_displaycontrol |= LCD_DISPLAYON;
  lcd_command(lcd_s, LCD_DISPLAYCONTROL | lcd_s->_displaycontrol);
}

// Turns the underline cursor on/off
void lcd_no_cursor(lcd *lcd_s) {
  lcd_s->_displaycontrol &= ~LCD_CURSORON;
  lcd_command(lcd_s, LCD_DISPLAYCONTROL | lcd_s->_displaycontrol);
}
void lcd_cursor(lcd *lcd_s) {
  lcd_s->_displaycontrol |= LCD_CURSORON;
  lcd_command(lcd_s, LCD_DISPLAYCONTROL | lcd_s->_displaycontrol);
}

// Turn on and off the blinking cursor
void lcd_no_blink(lcd *lcd_s) {
  lcd_s->_displaycontrol &= ~LCD_BLINKON;
  lcd_command(lcd_s, LCD_DISPLAYCONTROL | lcd_s->_displaycontrol);
}
void lcd_blink(lcd *lcd_s) {
  lcd_s->_displaycontrol |= LCD_BLINKON;
  lcd_command(lcd_s, LCD_DISPLAYCONTROL | lcd_s->_displaycontrol);
}

// These commands scroll the display without changing the RAM
void lcd_scroll_display_left(lcd *lcd_s) {
  lcd_command(lcd_s, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void lcd_scroll_display_right(lcd *lcd_s) {
  lcd_command(lcd_s, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void lcd_left_to_right(lcd *lcd_s) {
  lcd_s->_displaymode |= LCD_ENTRYLEFT;
  lcd_command(lcd_s, LCD_ENTRYMODESET | lcd_s->_displaymode);
}

// This is for text that flows Right to Left
void lcd_right_to_left(lcd *lcd_s) {
  lcd_s->_displaymode &= ~LCD_ENTRYLEFT;
  lcd_command(lcd_s, LCD_ENTRYMODESET | lcd_s->_displaymode);
}

// This will 'right justify' text from the cursor
void lcd_autoscroll(lcd *lcd_s) {
  lcd_s->_displaymode |= LCD_ENTRYSHIFTINCREMENT;
  lcd_command(lcd_s, LCD_ENTRYMODESET | lcd_s->_displaymode);
}

// This will 'left justify' text from the cursor
void lcd_no_autoscroll(lcd *lcd_s) {
  lcd_s->_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  lcd_command(lcd_s, LCD_ENTRYMODESET | lcd_s->_displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void lcd_create_char(lcd *lcd_s, uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  lcd_command(lcd_s, LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    lcd_write(lcd_s, charmap[i]);
  }
}

/*********** mid level commands, for sending data/cmds */

void lcd_command(lcd *lcd_s, uint8_t value) {
  lcd_send(lcd_s, value, 0);
}

void lcd_write(lcd *lcd_s, uint8_t value) {
  lcd_send(lcd_s, value, 1);
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void lcd_send(lcd *lcd_s, uint8_t value, uint8_t mode) {
	if (mode)
		*(lcd_s->_rs_enable_port) |= lcd_s->_rs_pin;
	else
		*(lcd_s->_rs_enable_port) &= ~lcd_s->_rs_pin;
  
  if (lcd_s->_displayfunction & LCD_8BITMODE) {
    lcd_write_8_bits(lcd_s, value); 
   // delay_microseconds(37); //Need to wait 37 µs between commands
  } else {
    lcd_write_4_bits(lcd_s, value>>4); //Send 4 high bits
    lcd_write_4_bits(lcd_s, value);		//Send 4 low bits
   // delay_microseconds(37); //Need to wait 37 µs between commands
  }
}

void lcd_pulse_enable(lcd *lcd_s) {
	*(lcd_s->_rs_enable_port) &= ~lcd_s->_enable_pin;
	*(lcd_s->_rs_enable_port) |= lcd_s->_enable_pin;
	*(lcd_s->_rs_enable_port) &= ~lcd_s->_enable_pin;
	delay_microseconds(37);
}

void lcd_write_4_bits(lcd *lcd_s, uint8_t value) {
  for (int i = 0; i < 4; i++) {
  	if( (value >> i) & 0x01 ) //set pin high
  		*(lcd_s->_data_port) |= lcd_s->_data_pins[i];
  	else //set pin low
  		*(lcd_s->_data_port) &= ~lcd_s->_data_pins[i];
  }

  lcd_pulse_enable(lcd_s);
}

void lcd_write_8_bits(lcd *lcd_s, uint8_t value) {
  for (int i = 0; i < 8; i++) {
  	if( (value >> i) & 0x01 ) //set pin high
  		*(lcd_s->_data_port) |= lcd_s->_data_pins[i];
  	else //set pin low
  		*(lcd_s->_data_port) &= ~lcd_s->_data_pins[i];
  }
  
  lcd_pulse_enable(lcd_s);
}

