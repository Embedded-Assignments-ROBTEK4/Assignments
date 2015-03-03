#include "emp_type.h"
#include <stdbool.h>
#include <stddef.h>

#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#define BUFFER_SIZE 200
#define BUFFER_NUM 10

#if BUFFER_SIZE<(1<<8-1)
#define RBUF_INDEX_TYPE INT8U
#elif BUFFER_SIZE<(1<<16-1)
#define RBUF_INDEX_TYPE INT16U
#elif BUFFER_SIZE<(1<<32-1)
#define RBUF_INDEX_TYPE INT32U
#else
#define RBUF_INDEX_TYPE INT64U
#endif
typedef struct
{
	RBUF_INDEX_TYPE max;
	RBUF_INDEX_TYPE head;
	RBUF_INDEX_TYPE tail;
	RBUF_INDEX_TYPE size;
	INT8U buffer[BUFFER_SIZE];
} ringbuffer_uchar;

INT8U sys_ringbuf_uchar_pop(INT8U id);

void sys_ringbuf_uchar_push(INT8U id, INT8U val);

void sys_ringbuf_uchar_init(void);

INT8U sys_ringbuf_uchar_request(void); //id 0xFF is none allocated


void sys_ringbuf_uchar_release(INT8U id);

bool sys_ringbuf_uchar_full(INT8U id);

RBUF_INDEX_TYPE sys_ringbuf_uchar_size(INT8U id);
RBUF_INDEX_TYPE sys_ringbuf_uchar_max(INT8U id);

INT8U ringbuffer_uchar_pop(ringbuffer_uchar *buffer);

void ringbuffer_uchar_push(ringbuffer_uchar *buffer, INT8U value);

bool ringbuffer_uchar_full(ringbuffer_uchar *buffer);

void ringbuffer_uchar_init(ringbuffer_uchar *buffer);



#endif
