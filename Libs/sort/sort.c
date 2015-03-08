#include "sort.h"
#include <stdbool.h>
#include <math.h>

static void sift_down(SORT_TYPE *buffer, SORT_INDEX_TYPE start, SORT_INDEX_TYPE end, bool (*compare)(SORT_TYPE *, SORT_TYPE *));
static void heapify(SORT_TYPE *buffer, SORT_INDEX_TYPE len, bool (*compare)(SORT_TYPE *, SORT_TYPE *));
static SORT_INDEX_TYPE partition(SORT_TYPE *buffer, SORT_INDEX_TYPE pivot, SORT_INDEX_TYPE len, bool (*compare)(SORT_TYPE *, SORT_TYPE *) );
static void intro_sort_rec(SORT_TYPE *buffer, SORT_INDEX_TYPE len, bool (*compare)(SORT_TYPE *, SORT_TYPE *), SORT_INDEX_TYPE rec_depth);

uint32_t swaps = 0;

static void swap(SORT_TYPE *a, SORT_TYPE *b)
{
	SORT_TYPE tmp = *a;
	*a = *b;
	*b = tmp;
	swaps++;
}

static SORT_INDEX_TYPE partition(SORT_TYPE *buffer, SORT_INDEX_TYPE pivot, SORT_INDEX_TYPE len, bool (*compare)(SORT_TYPE *, SORT_TYPE *) )
{
		SORT_INDEX_TYPE pivot_final = 0;
		SORT_TYPE p_val = buffer[pivot];
		swap(&buffer[pivot], &buffer[len - 1]);
		for(SORT_INDEX_TYPE i = 0; i < len - 1; i++)
		{
			if(compare(&buffer[i], &p_val))
			{
				swap(&buffer[i], &buffer[pivot_final++]);
			}
		}
		swap(&buffer[pivot_final], &buffer[len-1]);
		return pivot_final;
}

void quick_sort(SORT_TYPE *buffer, SORT_INDEX_TYPE len, bool (*compare)(SORT_TYPE *, SORT_TYPE *)) //Pointer to start of section to sort, and lenght of section
{
	//if lenght is less than minimum size, use another sorting algorithm.
	if(len >= QSORT_MIN_SIZE)
	{
		SORT_INDEX_TYPE pivot = partition(buffer, len / 2, len, compare);
		quick_sort(buffer, pivot, compare);	
		quick_sort(buffer + pivot + 1, len - pivot - 1, compare);
	}
	else
	{
		insertion_sort(buffer, len, compare); //if buffer is small, do a simple insertion sort.
	}
}

void bubble_sort(SORT_TYPE *buffer, SORT_INDEX_TYPE len, bool (*compare)(SORT_TYPE *, SORT_TYPE *))
{	//A simple bubble sort.
	while(len)
	{
		SORT_INDEX_TYPE tmp_len = 0;
		for(SORT_INDEX_TYPE i = 1; i < len; i++)
		{
			if( compare(&buffer[i], &buffer[i-1]) )
			{
				swap(&buffer[i-1], &buffer[i]);
				tmp_len = i;
			}
		}
		len = tmp_len;
	}
}


void intro_sort(SORT_TYPE *buffer, SORT_INDEX_TYPE len, bool (*compare)(SORT_TYPE *, SORT_TYPE *))
{
	SORT_INDEX_TYPE rec_depth = log(len) * 2;
	intro_sort_rec(buffer, len, compare, rec_depth);
}

static void intro_sort_rec(SORT_TYPE *buffer, SORT_INDEX_TYPE len, bool (*compare)(SORT_TYPE *, SORT_TYPE *), SORT_INDEX_TYPE rec_depth)
{
	if(rec_depth == 0)
	{
		heap_sort(buffer, len, compare);
	}
	else if(len >= QSORT_MIN_SIZE)
	{
		SORT_INDEX_TYPE pivot = partition(buffer, len / 2, len, compare);
		intro_sort_rec(buffer, pivot, compare, rec_depth - 1);	
		intro_sort_rec(buffer + pivot + 1, len - pivot - 1, compare, rec_depth - 1);
	}
	else
	{
		insertion_sort(buffer, len, compare); //if buffer is small, do a simple bubble sort.
	}
}


void insertion_sort(SORT_TYPE *buffer, SORT_INDEX_TYPE len, bool (*compare)(SORT_TYPE *, SORT_TYPE *))
{		//A simple insertion sort
	if(len <= 1) return;
	for(SORT_INDEX_TYPE i = 1; i < len ; i++)
	{
		for(SORT_INDEX_TYPE j = i; j > 0 && compare(buffer + j, buffer + j - 1); j--)
		{
			swap(buffer + j - 1, buffer + j);
		}
	}
}

void heap_sort(SORT_TYPE *buffer, SORT_INDEX_TYPE len, bool (*compare)(SORT_TYPE *, SORT_TYPE *))
{
	if(len <= 1) return;
	
	heapify(buffer, len, compare);
	
	for(SORT_INDEX_TYPE end = len - 1; end > 0; end--)
	{
		swap(buffer + end, buffer);
		sift_down(buffer, 0, end - 1, compare);
	}
}

static void heapify(SORT_TYPE *buffer, SORT_INDEX_TYPE len, bool (*compare)(SORT_TYPE *, SORT_TYPE *))
{
	for(SORT_INDEX_TYPE start = len / 2 - 1;; start--)
	{
		sift_down(buffer, start, len - 1, compare);
		if(start == 0) break; //need to do check here to prevent overrun if using unigned index type
	}
}

static void sift_down(SORT_TYPE *buffer, SORT_INDEX_TYPE start, SORT_INDEX_TYPE end, bool (*compare)(SORT_TYPE *, SORT_TYPE *))
{
	SORT_INDEX_TYPE root = start;
	while(root * 2 + 1 <= end)
	{
		SORT_INDEX_TYPE child = root * 2 + 1;
		SORT_INDEX_TYPE to_swap = root;
		if( compare(buffer + to_swap, buffer + child) ) 
		{
			to_swap = child;
		}
		if(child + 1 <= end && compare(buffer + to_swap, buffer + child + 1))
		{
			to_swap = child + 1;
		}
		if(to_swap == root)
			return;
		else
		{
			swap(buffer + root, buffer + to_swap);
			root = to_swap;
		}
	}
	return;
}
