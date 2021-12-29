#ifndef HEAP_INTERNAL_H
#define HEAP_INTERNAL_H

#include "heap.h"
#include <stddef.h>
void heap_swap(heap_t heap, void *a, void *b);
size_t heap_get_elem_level(heap_t heap, void *elem);
void *heap_get_level_ptr(heap_t heap, size_t level);
void heap_get_elem_level_index(heap_t heap, void *elem, size_t *level, size_t *index);
void *heap_get_loc_down(heap_t heap, void *cur);
void *heap_get_loc_up(heap_t heap, void *cur);
void heap_sift_up(heap_t heap, void *cur);
void heap_sift_down(heap_t heap, void *cur);

#endif