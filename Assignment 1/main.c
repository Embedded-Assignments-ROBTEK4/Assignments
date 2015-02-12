/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Overeem
*
* MODULENAME.: main.c
*
* PROJECT....: Assingment 1
*
* DESCRIPTION: Main loop
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150212  MS    Syntax Fixed.
*
*****************************************************************************/

/***************************** Include files ********************************/
#include <stdint.h>
#include "headers/setup.h"
#include "inc/tm4c123gh6pm.h"

/*****************************   Functions   ********************************/
int main(void)
/**********************************************
* Input : None
* Output : None
* Function : Call the setup function in which
						 the interrupts and timers is setup.
* 					 Then enter a while loop where
* 					 it will stay forever.
**********************************************/
{
	setup();
	while (1)
	{
	}
	return (0);
}

/****************************** End of module *******************************/
