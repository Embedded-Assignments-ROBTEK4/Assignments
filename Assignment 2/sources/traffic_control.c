/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Overeem
*
* MODULENAME.: traffic_control.c
*
* PROJECT....: Assingment 2
*
* DESCRIPTION: Traffic light controller.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150219  ALL    Created file.
*
*****************************************************************************/

#define RED 3
#define YELLOW 5
#define YELLOW_RED 1
#define GREEN 6
#define OFF 7

/****************************** Arrays **************************************/

INT8U normal_sequence[] 	 = {RED, YELLOW_RED, GREEN, YELLOW, RED};
INT8U norwegian_sequence[] = {YELLOW, OFF};
INT8U emergency[] 				 = {RED};


/****************************** Functions **********************************/

INT8U traffic_control
/****************************** End of module *******************************/
#endif

