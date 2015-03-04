#ifndef _SPI_H_
#define _SPI_H_

#include "emp_type.h"
#include "tm4c123gh6pm.h"
#define FSPI 16000000
#define SPI_CLOCK_RATE 1000000
#define TOTAL_DIVISION (FSPI / SPI_CLOCK_RATE)

#if TOTAL_DIVISION > 254*256
	#error "SPI CLOCK RATE TOO HIGH"
#endif
#if TOTAL_DIVISION <= 256*2
	#define SSICR0DV (TOTAL_DIVISION / 2)
	#define CPSDVSR 2
#else
	#define SSICR0DV 256
	#define CPSDVSR (TOTAL_DIVISION / 256)  //Need to be an even number between 2 and 254
#endif
//The above may not work corretly with all desired clockrates. Need to be tested throug


void setup_spi(void);
void spi3_write_data(INT16U data); //blocking
INT16U spi3_read_data(void); //blocking

#endif
