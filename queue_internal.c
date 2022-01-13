#include "queue_internal.h"
#include "list_internal.h"

void *queue_new_chunk(size_t chunk_size, void *prev) {
    return list_create_node(chunk_size + list_header_size(), prev, NULL);
}

void *queue_get_next_chunk(size_t chunk_size,void *chunk){
    return list_node_get_next(chunk_size + list_header_size(), chunk);
}