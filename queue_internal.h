#ifndef QUEUE_INTERNAL_H
#define QUEUE_INTERNAL_H

#include <stddef.h>

void *queue_new_chunk(size_t chunk_size, void *prev);
void *queue_get_next_chunk(size_t chunk_size,void *chunk);


#endif