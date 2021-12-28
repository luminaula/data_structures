#ifndef LIST_H
#define LIST_H

#include <stddef.h>


typedef struct linked_list_t {
    size_t elem_size;
	size_t node_size;
    size_t size;
    void *head, *tail;
} linked_list_t;

linked_list_t *list_create(size_t elem_size);
void list_push_back(linked_list_t *list, void *data);

#endif