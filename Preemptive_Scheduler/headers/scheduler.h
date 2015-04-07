#pragma once

#define MAX_TASKS 25
#define TASK_STACK_SIZE 128

#if MAX_TASKS > 255
	#error "Can't have more than 255 tasks!"
#endif

#define TIMEOUT_SYSTICK	5
#define SYSTICK_RELOAD_VALUE 		FCPU / 1000 * TIMEOUT_SYSTICK - 1

#if (SYSTICK_RELOAD_VALUE > 0xFFFFFF)
	#error "SYSTICK_RELOAD_VALUE is too high"
#endif


#include "emp_type.h"
#include "GLOBAL_DEFINITIONS.h"
#include <stdbool.h>
#include <stdint.h>
typedef enum
{
	DEAD,
	RUNNING,
	BLOCKED,
	WAITING,
} task_status;

typedef struct process
{
	INT8U id;
	//Pointer to the process function.
	//Pointer for eventual check function.
	//This function should be non-blocking.
	//This is NULL at process init, and set when process call the wait func.
	//this is NULL if not waiting for anything
	bool (*check)(void);
  //Pointer to pass to the check function when checking.
  //NB! This can't be stack allocated, as it then won't exist when checking
  //Allocate statically or with malloc!
	INT16U timer; //Software timer for the process
	task_status status;
	struct process *next; //Pointer to next process with same status.
	struct process *prev; //Pointer to previous process with same status.
	uint32_t process_stack[TASK_STACK_SIZE];
	uint32_t *process_stack_pointer;
	INT16U times_since_run;
} process;


//Add the task to the pool. Returns process id. returns 0xff on fail.
void init_scheduler(void);
void start_scheduler(void);
INT8U add_task(void (*process_func)(void));
void remove_task(INT8U id); //Remove the task from the pool
void wait_for(bool (*check)(void));
void wait(INT16U time);
void systick_timer_isr(void);
void setup_systick(void);
void pendsv_isr(void);
uint32_t *switch_context(uint32_t *stackptr);
void request_context_switch(void);
