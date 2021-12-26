#include "heap.h"
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void _heap_swap(heap_t *heap, void *a, void *b);
size_t _heap_get_elem_level(heap_t *heap, void *elem);
void *_heap_get_level_ptr(heap_t *heap, size_t level);
void _heap_get_elem_level_index(heap_t *heap, void *elem, size_t *level, size_t *index);
void *_heap_get_loc_down(heap_t *heap, void *cur);
void *_heap_get_loc_up(heap_t *heap, void *cur);
void _heap_sift_up(heap_t *heap, void *cur);
void _heap_sift_down(heap_t *heap, void *cur);

heap_t *heap_create(int (*comparator)(const void *, const void *), size_t elem_size) {
    heap_t *heap = (heap_t *)malloc(sizeof(heap_t));
    heap->elem_size = elem_size;
    heap->comparator = comparator;
    heap->size = 0;
    heap->capacity = 1;
    heap->data = calloc(heap->capacity, elem_size);
    heap->head = heap->data;
    return heap;
}

heap_t *heap_construct(int (*comparator)(const void *, const void *), size_t elem_size, size_t count, void *data, int in_place) {
    heap_t *heap = (heap_t *)malloc(sizeof(heap_t));
    heap->elem_size = elem_size;
    heap->comparator = comparator;
    heap->size = count;
    heap->capacity = 1l << (64 - _lzcnt_u64(count));
    if (in_place) {
        heap->data = data;
    } else {
        heap->data = malloc(heap->capacity * elem_size);
        memcpy(heap->data,data,elem_size * heap->size);
    }
    if(!heap->size){
        return heap;
    }
    heap->head = &((char *)heap->data)[heap->elem_size * heap->size];
    size_t max_level = _heap_get_elem_level(heap,heap->head);
    void *iter = _heap_get_loc_up(heap,heap->head);

    while(iter >= heap->data){
        _heap_sift_down(heap,iter);
        iter = &((char*)iter)[-heap->elem_size];
    }
    return heap;
}

void heap_free(heap_t *heap){
    if(!heap){
        return;
    }
    if(heap->data){
        free(heap->data);
    }
    free(heap);
    
}

void _heap_swap(heap_t *heap, void *a, void *b) {
    char buffer[heap->elem_size];
    memcpy(buffer, a, heap->elem_size);
    memcpy(a, b, heap->elem_size);
    memcpy(b, buffer, heap->elem_size);
}

size_t _heap_get_elem_level(heap_t *heap, void *elem) {
    ptrdiff_t diff = (uintptr_t)elem - (uintptr_t)heap->data;
    size_t idx = diff / heap->elem_size;
    return 64 - _lzcnt_u64(idx);
}

void *_heap_get_level_ptr(heap_t *heap, size_t level) {
    if (!level) {
        return heap->data;
    }
    return &((char *)heap->data)[(1 << (level - 1)) * heap->elem_size];
}

void _heap_get_elem_level_index(heap_t *heap, void *elem, size_t *level, size_t *index) {
    *level = _heap_get_elem_level(heap, elem);
    void *cur_level = _heap_get_level_ptr(heap, *level);
    ptrdiff_t diff = (uintptr_t)elem - (uintptr_t)cur_level;
    *index = diff / heap->elem_size;
}

void *_heap_get_loc_down(heap_t *heap, void *cur) {
    ptrdiff_t idx = (cur - heap->data) / heap->elem_size * 2;
    void *next = &((char*)heap->data)[idx * heap->elem_size];
    if(next > heap->head){
        return NULL;
    }
    return next;
}

void *_heap_get_loc_up(heap_t *heap, void *cur) {
    if(cur <= heap->data){
        return NULL;
    }
    ptrdiff_t idx = (cur - heap->data) / heap->elem_size /2;
    return &((char*)heap->data)[idx * heap->elem_size];
}

void _heap_sift_up(heap_t *heap, void *cur) {
    void *up = _heap_get_loc_up(heap, cur);
    if (!up) {
        return;
    }
    int cmp = heap->comparator(cur, up);
    if (cmp > 0) {
        _heap_swap(heap, cur, up);
        _heap_sift_up(heap, up);
    }
}

void _heap_sift_down(heap_t *heap, void *cur) {
    void *down_left = _heap_get_loc_down(heap, cur);
    void *down_right = &((char *)down_left)[heap->elem_size];
    if (!down_left) {
        return;
    }
    void *next = cur;
    int cmp = heap->comparator(down_left, next);
    if (cmp > 0) {
        next = down_left;
    }
    if (down_right <= heap->head) {
        cmp = heap->comparator(down_right, next);
        if (cmp > 0) {
            next = down_right;
        }
    }
    if (next != cur) {
        _heap_swap(heap, next, cur);
        _heap_sift_down(heap, next);
    }
}

void heap_insert(heap_t *heap, void *data) {
    if (heap->size == heap->capacity) {
        heap->capacity <<= 1;
        heap->data = realloc(heap->data, heap->elem_size * heap->capacity);
        heap->head = &((char *)heap->data)[heap->elem_size * heap->size];
    }

    memcpy(heap->head, data, heap->elem_size);
    _heap_sift_up(heap, heap->head);
    heap->head = &((char *)heap->head)[heap->elem_size];
    heap->size++;
}

void *heap_extract(heap_t *heap) {
    if (!heap->size) {
        return NULL;
    }
    heap->head = &((char *)heap->head)[-heap->elem_size];

    void *data = malloc(heap->elem_size);
    memcpy(data, heap->data, heap->elem_size);
    memcpy(heap->data, heap->head, heap->elem_size);

    --heap->size;
    _heap_sift_down(heap, heap->data);
    return data;
}

void *heap_peek(heap_t *heap){
    if(!heap->data){
        return NULL;
    }
    void *data = malloc(heap->elem_size);
    memcpy(data,heap->data,heap->elem_size);
    return data;
}

void heap_sort(int (*comparator)(const void *, const void *),size_t elem_size, void *data, size_t count){
    heap_t *heap = heap_construct(comparator,elem_size,count,data,1);
    while(heap->data != heap->head){
        _heap_swap(heap,heap->head,heap->data);
        heap->head = &((char*)heap->head)[-heap->elem_size];
        _heap_sift_down(heap,heap->data);
    }
    free(heap);
}