#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

typedef struct queue_t {
    size_t chunk_size;
    size_t elem_size;
    size_t size;
    size_t chunk_count;

    void *read_chunk;
    void *write_chunk;

    void *write_head;
    void *read_head;
} queue_t;

queue_t *queue_create(size_t elem_size);
void queue_free(queue_t *queue);
void queue_push(queue_t *queue, void *data);
void *queue_pop(queue_t *queue);

#endif