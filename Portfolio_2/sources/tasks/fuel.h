#pragma once
#define UNLIMITED -1
#define PULSES_PER_LITER (1125)
#define PULSE_MULTIPLICATION 2
#define PUMP_SPEED ((PULSES_PER_LITER * PULSE_MULTIPLICATION) * .20)
#define WAIT_TIME (1000. / PUMP_SPEED)
#define SHUNT_WAIT_TIME (WAIT_TIME * 10.)
#define SHUNT_LIMIT 0.1


#define FUEL_PULSE 0
#define FUEL_DONE  1

void set_max_fuel(double max_fuel);
void start_fuel(void);
void stop_fuel(void);
void activate_shunt(void);
void deactivate_shunt(void);
void fuel_task(void __attribute__((unused)) *pvParameters);
