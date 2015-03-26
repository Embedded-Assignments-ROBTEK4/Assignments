#include "../headers/scheduler.h"
#include "../headers/system_tasks.h"
#include "../headers/tm4c123gh6pm.h"
#include "../headers/assert.h"
#include <stddef.h>
static void init_task(INT8U id, void (*process_func)(void));
static void insert_running(INT8U id);
static void insert_waiting(INT8U id);
static void insert_blocked(INT8U id);
static void remove_from_list(INT8U id, process **list);
static INT8U select_task(void);
static void count_process_timers(void);
static void scheduler_tasks(void);
static void disable_systick_int(void);
static void enable_systick_int(void);
static void kernel_task(void);
static void remove_task_blocking(INT8U id);
static bool in_list(INT8U id); //checks if task is in a list, for debugging
static bool in_this_list(INT8U id, process **list); //checks if task is in a specific list, for debugging

void unset_pendsv(void);

static process tasks[MAX_TASKS];

static INT8U current_task = 0;
static process *running[2] = {NULL, NULL}; //pointers to first and last of this process type
static process *waiting[2] = {NULL, NULL}; //pointers to first and last of this process type
static process *blocked[2] = {NULL, NULL}; //pointers to first and last of this process type


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

static bool in_this_list(INT8U id, process **list)
{
	process *cur = list[0];
	if(list[0] != NULL && list[1] != NULL)
	{
		do
		{
			if(cur->id == id) return true;
			cur = cur-> next;
		}while(cur != list[0]);
	}
	return false;
}


static __attribute__((unused)) bool in_list(INT8U id)
{
	return (in_this_list(id, running) || in_this_list(id, waiting) || in_this_list(id, blocked));
}


static void count_process_timers()
{	//Run throug all process' in waiting and count their timer one down
	if(waiting[0] ==NULL )
		return;

	process *cur =waiting[0];
	do
	{
		if(cur->timer) cur->timer--;
		cur = cur->next;
	}while(cur != waiting[0]);
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
	process *cur = blocked[0];
	while(cur != blocked[1] && !cur->check())
	{
		cur = cur->next;
	}
	if(cur != blocked[1]) //We have found our function, set returnid
	{
		returnid = cur->id;
	}
	else if(blocked[1] != NULL && cur->check())
	{ //We have found our function, set returnid
		returnid = cur->id;
	}
	if(returnid == 0xFF)
	{	//If we didn't find a blocking one, try to find a waiting which is ready.
		//They should be in order, so we actually only need to try the first one
		if(waiting[0] != NULL && waiting[0]->timer == 0)
		{
			returnid = waiting[0]->id;
		}
	}
	if(returnid == 0xFF && running[0] != NULL)
	{//If we still didn't find one, find from running tasks.
			returnid = running[0]->id;
	}
	//remove from current queue and set status to running
	if(returnid != 0xFF)
	{
		switch(tasks[returnid].status)
		{
			case RUNNING:
				remove_from_list(returnid, running);
			break;
			case WAITING:
				remove_from_list(returnid, waiting);
			break;
			case BLOCKED:
				remove_from_list(returnid, blocked);
			break;
			default:
			break;
		}
		tasks[returnid].status = RUNNING;
	}
	return returnid;
}


uint32_t *switch_context(uint32_t *stackptr) //Does the same as systick_switch_context, but don't run system and scheduler tasks
{
  tasks[current_task].process_stack_pointer = stackptr;
  if(tasks[current_task].status == RUNNING && current_task)
    insert_running(current_task);
  current_task = select_task();
  if(current_task == 0xFF)
  {
    current_task = 0; //Start kernel thread
  }
  remove_from_list(current_task, running);
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
	while(true); //Do nothing //We should somehow fix this so we don't waste time here.
}

void start_scheduler()
{
	init_task(0,kernel_task);
	enable_systick_int();
	__asm__ volatile (
  "BL kernel_task\n" //Jump to kernel task
  );
}


void wait_for(bool (*check)(void))
{
	disable_systick_int();

	if(check())
	{
		return;
	}
	else
	{
    disable_systick_int();
		tasks[current_task].check = check;
		tasks[current_task].status = BLOCKED;
		insert_blocked(tasks[current_task].id);
    enable_systick_int();
    request_context_switch();
		return;
	}
}

void wait(INT16U time)
{ //This process wants to wait some time.
	//Put the time in its timer, inset it in the waiting list and request a pendsv interrupt.
	tasks[current_task].timer = time;
  disable_systick_int();
	insert_waiting(current_task);
  tasks[current_task].status = WAITING;
  enable_systick_int();
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
		running[0] = NULL;
		running[1] = NULL;
		waiting[0] = NULL;
		waiting[1] = NULL;
		blocked[0] = NULL;
		blocked[1] = NULL;
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
	insert_running(id);
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
	switch(tasks[id].status)
	{
		case RUNNING:
			remove_from_list(id, running);
		break;
		case WAITING:
			remove_from_list(id, waiting);
		break;
		case BLOCKED:
			remove_from_list(id, blocked);
		break;
		default:
		break;
	}
	tasks[id].status = DEAD;
}

static void remove_task_blocking(INT8U id)
{
	remove_task(id);
	while(1);
}

__attribute__((noinline)) static void insert_running(INT8U id)
{
	assert(!in_this_list(id, running));
	if(running[0] == NULL) //this should only happen if no jobs are running
	{
		//Set prev and next to point to itself, and set running[0:1] to point to it also.
		tasks[id].next = &tasks[id];
		tasks[id].prev = &tasks[id];
		running[0] = &tasks[id];
		running[1] = &tasks[id];
	}
	else
	{
		//Else, we insert at end of queue
		running[1]->next = &tasks[id];
		tasks[id].prev = running[1];
		tasks[id].next = running[0];
		running[0]->prev = &tasks[id];
		running[1] = &tasks[id];
	}
	assert(in_this_list(id, running));
}

__attribute__((noinline)) static void insert_blocked(INT8U id)
{	//this code should be excatly the same as insert running
	//But we don't combine it, as it would make it overly complicated.
	//And it's important that there is not bugs in this code.
	assert(!in_this_list(id, blocked));

	if(blocked[0] == NULL) //this should only happen if no jobs are blocked
	{
		//Set prev and next to point to itself, and set blocked[0:1] to point to it also.
		tasks[id].next = &tasks[id];
		tasks[id].prev = &tasks[id];
		blocked[0] = &tasks[id];
		blocked[1] = &tasks[id];
	}
	else
	{ //Else, we insert at end of queue
		blocked[1]->next = &tasks[id];
		tasks[id].prev = blocked[1];
		tasks[id].next = blocked[0];
		blocked[0]->prev = &tasks[id];
		blocked[1] = &tasks[id];
	}
	assert(in_this_list(id, blocked));
}


__attribute__((noinline)) static void insert_waiting(INT8U id)
{
	assert(!in_this_list(id, waiting));
	if(waiting[0] == NULL) //this should only happen if no jobs are waiting
	{
		//Set prev and next to point to itself, and set waiting[0:1] to point to it also.
		tasks[id].next = &tasks[id];
		tasks[id].prev = &tasks[id];
		waiting[0] = &tasks[id];
		waiting[1] = &tasks[id];
	}
	else
	{ //Here we travel the queue and find where to put the process.
		//The queue should be in a sequence ordered after the timer value.
		//If there are values equal to the process we insert, the inserted process
		//Is placed last.
		process *cur = waiting[0];
		while(cur != waiting[1] && cur->timer <= tasks[id].timer)
		{
			cur = cur->next;
		}
		//eighter, cur is now pointing on the end of the list,
		//the first value with a timer larger than the inserted,
		//or both. Find out which one
		//We also need to check if the timer should be inserted at the start of the queue
		if(cur == waiting[1] && cur->timer <= tasks[id].timer)
		{ //place the task at the end of the list
			waiting[1]->next = &tasks[id];
			tasks[id].prev = waiting[1];
			tasks[id].next = waiting[0];
			waiting[0]->prev = &tasks[id];
			waiting[1] = &tasks[id];
			assert(in_this_list(id, waiting));
			return;
		}
		else if(cur->timer > tasks[id].timer)
		{ //travel one step back
			cur = cur-> prev;
		}
		//check if the task should be inserted at the start of the queue
		if(cur-> next == waiting[0] )
		{	//Insert at queue start
			///BEWARE OF BUGS HERE///
			tasks[id].next = cur->next;
			tasks[id].prev = cur;
			waiting[1]->next = &tasks[id];
			cur->prev = &tasks[id];
			waiting[0] = &tasks[id];
		}
		else
		{ //We just insert after cur
			tasks[id].next = cur->next;
			tasks[id].prev = cur;
			cur -> next = &tasks[id];
		}
		assert(in_this_list(id, waiting));
	}
}

__attribute__((noinline)) static void remove_from_list(INT8U id, process **list) //united function as they are exctely the same
{
	//Travel the list until we reach the id. This may take long time,
	//But most of the time the task will be at the begining.
	assert(in_this_list(id, list));
	//Travel the list until we reach the id. This may take long time,
	//But most of the time the task will be at the begining.
	process *cur = list[0];
	while( cur->id != id && cur != list[1] )
	{//We eighter reach the last or the correct one
		cur = cur -> next;
	}
	assert(cur -> id == id); //we got the correct one!
	if(cur == list[0] && cur == list[1])
	{ //this means we are the only process
		list[0] = NULL;
		list[1] = NULL;
	}
	else if(cur == list[0])
	{ //We are the first process
		list[0] = cur->next;
		list[0]->prev = cur->prev;
		cur->prev->next = list[0];
	}
	else if(cur == list[1])
	{//We are the last process
		list[1] = cur->prev;
		cur->next->prev = cur->prev;
		cur->prev->next = cur->next;
	}
	else
	{//We just remove this process
		cur->next->prev = cur->prev;
		cur->prev->next = cur->next;
	}
	assert(!in_this_list(id, list));
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
