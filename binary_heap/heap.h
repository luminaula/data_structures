#ifndef HEAP_H
#define HEAP_H
#include <stddef.h>

typedef struct heap_t {
    int (*comparator)(const void *, const void *);
    void *data;
    void *head;
    size_t size;
    size_t capacity;
    size_t elem_size;
} heap_t;

heap_t *heap_create(int (*comparator)(const void *, const void *), size_t elem_size);
heap_t *heap_construct(int (*comparator)(const void *, const void *), size_t elem_size, size_t count, void *data, int in_place);
void heap_free(heap_t *heap);
void heap_insert(heap_t *heap, void *data);
void *heap_extract(heap_t *heap);
void *heap_peek(heap_t *heap);

void heap_sort(int (*comparator)(const void *, const void *), size_t elem_size, void *data, size_t count);

#endif // HEAP_H