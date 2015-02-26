/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Overeem
*
* MODULENAME.: ISR.h
*
* PROJECT....: Assingment 1
*
* DESCRIPTION: definitions which is used for many parts of the application.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150217  StefanRvo    Created file.
*
*****************************************************************************/

#ifndef _GLOBAL_DEFINITIONS_H
	#define _GLOBAL_DEFINITIONS_H

/***************************** Include files ********************************/
#include <stdint.h>
#include <iso646.h>

/***************************** Defines **************************************/
#define FCPU                 			16000000 		// Hz.
#define TIMEOUT_SYSTICK      			5        	 // ms.

#define LED_RED 	0x02
#define LED_BLUE 	0x04
#define LED_GREEN 0x08
#define SW1_PIN 	0x10

/****************************** End of module *******************************/

#endif
