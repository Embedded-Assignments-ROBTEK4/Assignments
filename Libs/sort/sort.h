#pragma once

#include <stdint.h>
#include <stdbool.h>
#define QSORT_MIN_SIZE 5
#if QSORT_MIN_SIZE < 3
	#error "Minimum size of QSORT can't be smaller than 3"
#endif

#define SORT_TYPE uint8_t
#define SORT_INDEX_TYPE uint32_t


void quick_sort(		SORT_TYPE *buffer, SORT_INDEX_TYPE len, bool (*compare)(SORT_TYPE *, SORT_TYPE *));
void bubble_sort(		SORT_TYPE *buffer, SORT_INDEX_TYPE len, bool (*compare)(SORT_TYPE *, SORT_TYPE *));
void insertion_sort(SORT_TYPE *buffer, SORT_INDEX_TYPE len, bool (*compare)(SORT_TYPE *, SORT_TYPE *));
void heap_sort(			SORT_TYPE *buffer, SORT_INDEX_TYPE len, bool (*compare)(SORT_TYPE *, SORT_TYPE *));
void intro_sort(		SORT_TYPE *buffer, SORT_INDEX_TYPE len, bool (*compare)(SORT_TYPE *, SORT_TYPE *));
