#ifndef _PRINT_H_
#define _PRINT_H_

#include <stdarg.h>
#include "emp_type.h"

#define MAX_LINE_LENGHT 4000
//Have to return int as vprintf is reserverd
int vprintf_(void (*destfunc)(unsigned char *), unsigned char *string, ...);

#endif
