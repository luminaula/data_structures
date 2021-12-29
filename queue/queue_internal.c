#include "queue_internal.h"
#include "list_internal.h"

void *_queue_new_chunk(size_t chunk_size, void *prev) {
    return list_create_node(chunk_size + list_header_size(), prev, NULL);
}

