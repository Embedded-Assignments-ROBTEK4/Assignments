#ifndef _PRINT_H_
#define _PRINT_H_

#include <stdarg.h>
#include "emp_type.h"
#include <math.h>
#define INT_DIGITS 19 //Max digits of 64 bit int.
#define MAX_LINE_LENGHT 4000
//Have to return int as vprintf is reserverd
int vprintf_(void (*destfunc)(char *), char *string, ...);
INT8U itoa(INT64S number, char * string, INT8U base);
#endif
