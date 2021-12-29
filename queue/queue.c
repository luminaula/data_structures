#include "queue.h"
#include "list.h"
#include "list_internal.h"
#include "ptr_arim.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "queue_internal.h"

queue_t *queue_create(size_t elem_size) {
    queue_t *queue = malloc(sizeof(queue_t));
    queue->elem_size = elem_size;
    queue->chunk_size = elem_size * 64;

    queue->read_chunk = _queue_new_chunk(queue->chunk_size,NULL);
    queue->write_chunk = queue->read_chunk;

    queue->write_head = queue->read_chunk;
    queue->read_head = queue->read_chunk;

    queue->size = 0;
    queue->chunk_count = 0;

    return queue;
}



void queue_push(queue_t *queue, void *data) {
    ptrdiff_t write_offset = ptr_diff(queue->write_chunk, queue->write_head);
    if(write_offset + queue->elem_size > queue->chunk_size){
        queue->write_chunk = _queue_new_chunk(queue->chunk_size,queue->write_chunk);
        queue->write_head = queue->write_chunk;
        queue->chunk_count++;
    }
    memcpy(queue->write_head, data, queue->elem_size);
    PTR_ADVANCE(queue->write_head, queue->elem_size);
    queue->size++;
}

void *queue_pop(queue_t *queue) {
    if(queue->write_chunk == queue->read_chunk && queue->read_head >= queue->write_head){
        return NULL;
    }
    if(!queue->read_chunk && queue->write_chunk){
        queue->read_chunk = queue->write_chunk;
        queue->read_head = queue->read_chunk;
    }
    
    ptrdiff_t read_offset = ptr_diff(queue->read_chunk, queue->read_head);
    
    if(read_offset + queue->elem_size > queue->chunk_size){
        void *tmp = queue->read_chunk;
        queue->read_chunk = list_node_get_next(queue->chunk_size + list_header_size(),queue->read_chunk);
        queue->read_head = queue->read_chunk;
        queue->chunk_count--;
        free(tmp);
    }
    if(queue->read_chunk == NULL){
        return NULL;
    }
    void *data = malloc(queue->elem_size);
    memcpy(data,queue->read_head,queue->elem_size);
    PTR_ADVANCE(queue->read_head,queue->elem_size);
    queue->size--;
    return data;
}
