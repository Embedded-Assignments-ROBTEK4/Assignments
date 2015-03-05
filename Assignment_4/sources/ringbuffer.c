#include "../headers/ringbuffer.h"

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
	buffer->mutex = UNLOCKED;
}
