#pragma once

#include <stdint.h>
#include <stdbool.h>
typedef uint32_t sem_t;

void sem_dec(volatile sem_t* sem);
bool try_sem_dec(volatile sem_t* sem);
void sem_inc(volatile sem_t* sem);
