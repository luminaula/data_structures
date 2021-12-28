#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

typedef struct queue_t{
	size_t chunk_size;
	linked_list_t *chunks;
	
}queue_t;

#endif