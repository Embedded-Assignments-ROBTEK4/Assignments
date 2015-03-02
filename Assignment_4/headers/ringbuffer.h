#include "emp_type.h"
#include <stdbool.h>
#include <stddef.h>

#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#define BUFFER_SIZE 200
#define BUFFER_NUM 10

typedef struct
{
	INT8U max;
	INT8U head;
	INT8U tail;
	INT8U size;
	INT8U buffer[BUFFER_SIZE];
} ringbuffer_uchar;

INT8U sys_ringbuf_uchar_pop(INT8U id);

void sys_ringbuf_uchar_push(INT8U id, INT8U val);

void sys_ringbuf_uchar_init(void);

INT8U sys_ringbuf_uchar_request(void); //id 0xFF is none allocated

void sys_ringbuf_uchar_release(INT8U id);

bool sys_ringbuf_uchar_full(INT8U id);

INT8U sys_ringbuf_uchar_size(INT8U id);

INT8U ringbuffer_uchar_pop(ringbuffer_uchar *buffer);

void ringbuffer_uchar_push(ringbuffer_uchar *buffer, INT8U value);

bool ringbuffer_uchar_full(ringbuffer_uchar *buffer);

void ringbuffer_uchar_init(ringbuffer_uchar *buffer);

#endif
