#include "list.h"
#include <stdlib.h>
#include <string.h>

linked_list_t *list_create(size_t elem_size) {
    linked_list_t *list = malloc(sizeof(linked_list_t));
    list->elem_size = elem_size;
    list->node_size = list->elem_size + 8 - list->elem_size % 8 + sizeof(void *) * 2;
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
}

void list_push(linked_list_t *list, void *data) {
    void *node = calloc(1, list->node_size);
    void *prev_node = list->tail;
    memcpy(node, data, list->elem_size);

    if (prev_node) {
    	void **links = &((char *)node)[list->node_size - sizeof(void *) * 2];
        void **prev_links = &((char *)prev_node)[list->node_size - sizeof(void *) * 2];
        prev_links[1] = node;
		links[0] = prev_node;
		list->tail = node;
    }
	else{
		list->head = node;
		list->tail = node;
	}

}