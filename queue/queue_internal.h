#ifndef QUEUE_INTERNAL_H
#define QUEUE_INTERNAL_H

#include <stddef.h>

void *_queue_new_chunk(size_t chunk_size, void *prev);

#endif