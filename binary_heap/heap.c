#include "heap.h"
#include "heap_internal.h"
#include "ptr_arim.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

heap_t heap_create(int (*comparator)(const void *, const void *), size_t elem_size) {
    heap_t heap;
    heap.elem_size = elem_size;
    heap.comparator = comparator;
    heap.size = 0;
    heap.capacity = 1;
    heap.data = calloc(heap.capacity, elem_size);
    heap.head = heap.data;
    return heap;
}

heap_t heap_construct(int (*comparator)(const void *, const void *), size_t elem_size, size_t count, void *data, int in_place) {
    heap_t heap;
    heap.elem_size = elem_size;
    heap.comparator = comparator;
    heap.size = count;
    if (in_place) {
        heap.capacity = count;
        heap.data = data;
    } else {
        heap.capacity = heap.size;
        heap.data = malloc(heap.capacity * elem_size);
        memcpy(heap.data, data, elem_size * heap.size);
    }
    if (!heap.size) {
        return heap;
    }
    heap.head = PTR_OFFSET(heap.data, heap.elem_size * heap.size);
    void *iter = heap_get_loc_up(heap, heap.head);

    while (iter >= heap.data) {
        heap_sift_down(heap, iter);
        PTR_ADVANCE(iter, -heap.elem_size);
    }
    return heap;
}

void heap_free(heap_t heap) {
    if (heap.data) {
        free(heap.data);
    }
}

void heap_insert(heap_t heap, void *data) {
    if (heap.size == heap.capacity) {
        heap.capacity <<= 1;
        heap.data = realloc(heap.data, heap.elem_size * heap.capacity);
        heap.head = PTR_OFFSET(heap.data, heap.elem_size * heap.size);
    }

    memcpy(heap.head, data, heap.elem_size);
    heap_sift_up(heap, heap.head);
    PTR_ADVANCE(heap.head, heap.elem_size);
    heap.size++;
}

void *heap_extract(heap_t heap) {
    if (!heap.size) {
        return NULL;
    }
    PTR_ADVANCE(heap.head, -heap.elem_size);

    void *data = malloc(heap.elem_size);
    memcpy(data, heap.data, heap.elem_size);
    memcpy(heap.data, heap.head, heap.elem_size);

    --heap.size;
    heap_sift_down(heap, heap.data);
    return data;
}

void *heap_peek(heap_t heap) {
    if (!heap.data) {
        return NULL;
    }
    void *data = malloc(heap.elem_size);
    memcpy(data, heap.data, heap.elem_size);
    return data;
}

void heap_sort(int (*comparator)(const void *, const void *), size_t elem_size, void *data, size_t count) {
    heap_t heap = heap_construct(comparator, elem_size, count, data, 1);
    while (heap.data < heap.head) {
        PTR_ADVANCE(heap.head, -heap.elem_size);
        heap_swap(heap, heap.head, heap.data);
        heap_sift_down(heap, heap.data);
    }
}