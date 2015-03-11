#pragma once

#include <stdint.h>
#include <stdbool.h>

#define LOCKED		1
#define	UNLOCKED	0

typedef uint32_t mutex_t;

void lock_mutex(volatile mutex_t* mutex);
void unlock_mutex(volatile mutex_t* mutex);
bool is_mutex_unlocked(mutex_t *mutex);
