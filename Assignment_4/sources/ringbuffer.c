#include "../headers/ringbuffer.h"

//Create a ringbuffer to hold type x, and with the size of z.
ringbuffer_uchar system_buffers[BUFFER_NUM];
bool allocated[BUFFER_NUM];


INT8U sys_ringbuf_uchar_pop(INT8U id)
{
	return ringbuffer_uchar_pop(&system_buffers[id]);
}

void sys_ringbuf_uchar_push(INT8U id, INT8U val)
{
	ringbuffer_uchar_push(&system_buffers[id], val);
}

void sys_ringbuf_uchar_init()
{
	for(INT8U i = 0; i < BUFFER_NUM; i++)
	{
		ringbuffer_uchar_init(&system_buffers[i]);
		allocated[i] = false;
	}
}

INT8U sys_ringbuf_uchar_request() //id 0xFF is none allocated
{
	INT8U returnid = 0xFF;
	for(INT8U i = 0; i < BUFFER_NUM && returnid == 0xFF; i++)
	{
		if(!allocated[i])
		{
			allocated[i] = true;
			returnid = i;
		}
	}
	return returnid;
}

void sys_ringbuf_uchar_release(INT8U id)
{
	allocated[id] = false;
	ringbuffer_uchar_init(&system_buffers[id]);
}

bool sys_ringbuf_uchar_full(INT8U id)
{
	return 	ringbuffer_uchar_full(&system_buffers[id]);
}


RBUF_INDEX_TYPE sys_ringbuf_uchar_size(INT8U id)
{
	return system_buffers[id].size;
}

RBUF_INDEX_TYPE sys_ringbuf_uchar_max(INT8U id)
{
	return system_buffers[id].max;
}
INT8U ringbuffer_uchar_pop(ringbuffer_uchar *buffer)
{
	
	INT8U returnval = buffer->buffer[buffer->tail];
	buffer->tail =(buffer->tail + 1) % buffer->max;
	buffer->size--;
	
	return returnval;
}

void ringbuffer_uchar_push(ringbuffer_uchar *buffer, INT8U value)
{
	if(buffer->size != buffer->max)
	{
		buffer->buffer[buffer->head] = value;
		buffer->head = (buffer->head + 1) %  buffer -> max;
		buffer->size++;
	}
}

bool ringbuffer_uchar_full(ringbuffer_uchar *buffer)
{
	if( buffer->size == buffer->max )
	 	return true;
	else
		return false;
}

void ringbuffer_uchar_init(ringbuffer_uchar *buffer)
{
	buffer->max = BUFFER_SIZE;
	buffer->head = 0;
	buffer->tail = 0;
	buffer->size = 0;
}
