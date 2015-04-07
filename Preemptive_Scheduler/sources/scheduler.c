#include "../headers/scheduler.h"
#include "../headers/system_tasks.h"
#include "../headers/tm4c123gh6pm.h"
#include "../headers/assert.h"
#include <stddef.h>
static void init_task(INT8U id, void (*process_func)(void));
static INT8U select_task(void);
static void count_process_timers(void);
static void scheduler_tasks(void);
static void disable_systick_int(void);
static void enable_systick_int(void);
static void kernel_task(void);
static void remove_task_blocking(INT8U id);

void unset_pendsv(void);

static process tasks[MAX_TASKS];

static INT8U current_task = 0;


typedef struct {
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r12;
  uint32_t lr;
  uint32_t pc;
  uint32_t psr;
} hw_stack_frame_t;

//This defines the stack frame that must be saved by the software
typedef struct {
  uint32_t r4;
  uint32_t r5;
  uint32_t r6;
  uint32_t r7;
  uint32_t r8;
  uint32_t r9;
  uint32_t r10;
  uint32_t r11;
} sw_stack_frame_t;

__attribute__((unused)) void request_context_switch()
{
  NVIC_INT_CTRL_R |= NVIC_INT_CTRL_PEND_SV;
}
__attribute__((unused)) void unset_pendsv()
{
  NVIC_INT_CTRL_R &= ~NVIC_INT_CTRL_PEND_SV;
}

static void disable_systick_int()
{
	NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_INTEN;
}
static void enable_systick_int()
{
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_INTEN;
}


static void count_process_timers()
{	//Run throug all process'  and count their timer one down

  for(INT8U i = 1; i < MAX_TASKS; i++)
  {
    if(tasks[i].status == WAITING && tasks[i].timer)  tasks[i].timer--;
  }
}

static void scheduler_tasks()
{
	count_process_timers();
}

static INT8U select_task()
{
	INT8U returnid = 0xFF;
	//We start by trying to find a blocked process, they are priotized highest.
	//Find the first one with a function returning true, and start that.
	for(INT8U i = 1; i < MAX_TASKS && returnid == 0xFF; i++ )
  {
    if(tasks[i].status == BLOCKED && tasks[i].check()) returnid = tasks[i].id;
  }

	if(returnid == 0xFF)
	{	//If we didn't find a blocking one, try to find a waiting which is ready.
    for(INT8U i = 1; i < MAX_TASKS; i++)
    {
      if(tasks[i].status == WAITING && tasks[i].timer == 0)
      {
        returnid = tasks[i].id;
      }
    }
	}

	if(returnid == 0xFF)
	{//If we still didn't find one, find from running tasks.
    INT16U max_turns = 0;
    for(INT8U i = 1; i < MAX_TASKS; i++)
    {
      if(tasks[i].status == RUNNING)
      {
        tasks[i].times_since_run++;
        if(tasks[i].times_since_run > max_turns)
        {
          max_turns = tasks[i].times_since_run;
          returnid = tasks[i].id;
        }
      }
    }
	}
	//remove from current queue and set status to running
  if(returnid != 0xFF)
  {
	  tasks[returnid].status = RUNNING;
    tasks[returnid].times_since_run = 0;
    return returnid;
  }
  else
  {
    return 0; //Start kernel thread
  }
}


uint32_t *switch_context(uint32_t *stackptr) //Does the same as systick_switch_context, but don't run system and scheduler tasks
{
  disable_systick_int();
  tasks[current_task].process_stack_pointer = stackptr;
  current_task = select_task();
  enable_systick_int();
  return tasks[current_task].process_stack_pointer;
}

 void systick_timer_isr(void)
{
  scheduler_tasks();
  system_tasks();
  request_context_switch(); //Request a context switch.
}

__attribute__((naked)) void pendsv_isr()
{
  __asm__ volatile
  (
  " MRS   r0, MSP\n" //Load stackptr into first argument
  " STMDB r0!, {r4-r11}\n" //save registers on stack

  " MOV   r4, lr\n" //Move return adress into r4
  " BL    unset_pendsv\n"
  " BL    switch_context\n" //load stackpointer of next process
  " MOV   lr, r4\n"
  " LDMIA r0!, {r4-r11}\n" //Pop registers
  " MSR   MSP, r0\n"
  " BX    lr\n"
  );

}

__attribute__((unused)) void kernel_task()
{
	while(true)
  {
    //request_context_switch();
  } //Do nothing //We should somehow fix this so we don't waste time here.
}

void start_scheduler()
{
	init_task(0,kernel_task);
	enable_systick_int();
  kernel_task(); //Jump to kernel task
}


void wait_for(bool (*check)(void))
{
	if(check())
	{
		return;
	}
	else
	{
    disable_systick_int();
		tasks[current_task].check = check;
		tasks[current_task].status = BLOCKED;
    enable_systick_int();
    request_context_switch();
		return;
	}
}

void wait(INT16U time)
{ //This process wants to wait some time.
	//Put the time in its timer, inset it in the waiting list and request a pendsv interrupt.
	tasks[current_task].timer = time;
  tasks[current_task].status = WAITING;
  request_context_switch();
}

void init_scheduler(void)
{
	for (INT8U i = 0; i < MAX_TASKS; i += 1)
	{
		tasks[i].id = i;
		tasks[i].check = NULL;
		tasks[i].timer = 0;
		tasks[i].status = DEAD;
    tasks[i].times_since_run = 0;
	}
}

static void init_task(INT8U id, void (*process_func)(void))
{
	hw_stack_frame_t *process_frame;
	process_frame = (hw_stack_frame_t *) (((char *)tasks[id].process_stack) +
										sizeof(tasks[id].process_stack) - sizeof(hw_stack_frame_t));
	process_frame->r0 = 0;
	process_frame->r1 = 0;
	process_frame->r2 = 0;
	process_frame->r3 = 0;
	process_frame->r12 = 0;
	process_frame->pc = ((uint32_t)process_func);
	process_frame->lr = (uint32_t)remove_task_blocking;
  process_frame->psr = 0x21000000; //default PSR value
  tasks[id].process_stack_pointer = (uint32_t *)(((char *)process_frame)- sizeof(sw_stack_frame_t));
	tasks[id].id = id;
	tasks[id].check = NULL;
	tasks[id].timer = 0;
	tasks[id].status = RUNNING;
  tasks[id].times_since_run = 0;
}

INT8U add_task(void (*process_func)(void))
{
	INT8U id = 0xFF;
	for(INT8U i = 1; i < MAX_TASKS && id == 0xFF; i++) //Task 0 is kernel
	{
		if(tasks[i].status == DEAD)
		{
			id = i;
			init_task(id, process_func);
		}
	}
	return id;
}

void remove_task(INT8U id)
{
	tasks[id].status = DEAD;
}

static void remove_task_blocking(INT8U id)
{
	remove_task(id);
	while(1);
}



void setup_systick(void)
/**********************************************
* Input : None.
* Output : None.
* Function : Setup systick timer.
**********************************************/
{
	// Setup systick timer.
  NVIC_ST_CTRL_R 	 &= ~(NVIC_ST_CTRL_ENABLE);	// Disable systick timer.

  // Set current systick counter to reload value.
  NVIC_ST_CURRENT_R = SYSTICK_RELOAD_VALUE;

  // Set Reload value, Systick reload register.
  NVIC_ST_RELOAD_R  = SYSTICK_RELOAD_VALUE;

  // NVIC systick setup, vector number 15.
  // Clear pending systick interrupt request.
  NVIC_INT_CTRL_R  |= NVIC_INT_CTRL_PENDSTCLR;

  // Select systick clock source, Use core clock.
  NVIC_ST_CTRL_R   |= NVIC_ST_CTRL_CLK_SRC;

  NVIC_ST_CTRL_R 	 |= NVIC_ST_CTRL_ENABLE;		// Enable and start timer.
}
