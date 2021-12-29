#include "heap_internal.h"
#include "heap.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ptr_arim.h"
#include <immintrin.h>

void heap_swap(heap_t heap, void *a, void *b) {
    char buffer[heap.elem_size];
    memcpy(buffer, a, heap.elem_size);
    memcpy(a, b, heap.elem_size);
    memcpy(b, buffer, heap.elem_size);
}

size_t heap_get_elem_level(heap_t heap, void *elem) {
    ptrdiff_t diff = (uintptr_t)elem - (uintptr_t)heap.data;
    size_t idx = diff / heap.elem_size;
    return 64 - _lzcnt_u64(idx);
}

void *heap_get_level_ptr(heap_t heap, size_t level) {
    if (!level) {
        return heap.data;
    }
    return PTR_OFFSET(heap.data, (1 << level - 1) * heap.elem_size);
}

void heap_get_elem_level_index(heap_t heap, void *elem, size_t *level, size_t *index) {
    *level = heap_get_elem_level(heap, elem);
    void *cur_level = heap_get_level_ptr(heap, *level);
    ptrdiff_t diff = (uintptr_t)elem - (uintptr_t)cur_level;
    *index = diff / heap.elem_size;
}

void *heap_get_loc_down(heap_t heap, void *cur) {
    ptrdiff_t idx = (cur - heap.data) / heap.elem_size * 2;
    void *next = PTR_OFFSET(heap.data, idx * heap.elem_size);
    if (next >= heap.head) {
        return NULL;
    }
    return next;
}

void *heap_get_loc_up(heap_t heap, void *cur) {
    if (cur <= heap.data) {
        return NULL;
    }
    ptrdiff_t idx = (cur - heap.data) / heap.elem_size / 2;
    return PTR_OFFSET(heap.data, idx * heap.elem_size);
}

void heap_sift_up(heap_t heap, void *cur) {
    void *up = heap_get_loc_up(heap, cur);
    if (!up) {
        return;
    }
    int cmp = heap.comparator(cur, up);
    if (cmp > 0) {
        heap_swap(heap, cur, up);
        heap_sift_up(heap, up);
    }
}

void heap_sift_down(heap_t heap, void *cur) {
    void *down_left = heap_get_loc_down(heap, cur);
    void *down_right = PTR_OFFSET(down_left, heap.elem_size);
    if (!down_left) {
        return;
    }
    void *next = cur;
    int cmp = heap.comparator(down_left, next);
    if (cmp > 0) {
        next = down_left;
    }
    if (down_right < heap.head) {
        cmp = heap.comparator(down_right, next);
        if (cmp > 0) {
            next = down_right;
        }
    }
    if (next != cur) {
        heap_swap(heap, next, cur);
        heap_sift_down(heap, next);
    }
}