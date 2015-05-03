/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* Author.....: Martin Steenberg, Niels Hvid, Rasmus Stagsted & Stefan Van Overeem
*
* MODULENAME.: fuel.h
*
* PROJECT....: Portfolio_2
*
* DESCRIPTION: Handle pumping and shunt.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150225  ALL		Created file.
* 150226	MS 		Fixed syntax.
*****************************************************************************/
#pragma once

/***************************** Include files ********************************/

/*******************************   Defines    *******************************/
#define UNLIMITED -1
#define PULSES_PER_LITER (1125)
#define PULSE_MULTIPLICATION 2
#define PUMP_SPEED ((PULSES_PER_LITER * PULSE_MULTIPLICATION) * .20)
#define WAIT_TIME (1000. / PUMP_SPEED)
#define SHUNT_WAIT_TIME (WAIT_TIME * 10.)
#define SHUNT_LIMIT 0.1

#define FUEL_PULSE 0
#define FUEL_DONE  1

/*******************************    Enums     *******************************/

/*****************************   Functions   ********************************/
void set_max_fuel(double max_fuel);
void start_fuel(void);
void stop_fuel(void);
void activate_shunt(void);
void deactivate_shunt(void);
void fuel_task(void __attribute__((unused)) *pvParameters);

/****************************** End of module *******************************/
