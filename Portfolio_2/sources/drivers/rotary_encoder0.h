#pragma once
#include "../../headers/tm4c123gh6pm.h"
#include "../../headers/emp_type.h"
#include <stdbool.h>


#define DIGI_A  (1 << 5)
#define DIGI_B  (1 << 6)
#define DIGI_P2 (1 << 7)
void scan_encoder0(void __attribute__((unused)) *pvParameters);
void setup_encoder0(void);

#define ENC_PUSH 0
#define ENC_CW   1 //clockwise rotation
#define ENC_CCW  2 //counter-clockwise rotation
