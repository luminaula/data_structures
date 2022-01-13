#ifndef LIST_INTERNAL_H
#define LIST_INTERNAL_H
#include <stddef.h>

typedef struct node_header_t {
    void *prev;
    void *next;
} node_header_t;

void *list_node_get_next(size_t node_size, void *node);
void *list_node_get_prev(size_t node_size, void *node);
void *list_create_node(size_t node_size, void *prev, void *next);
void list_link_node(size_t node_size, void *node, void *prev, void *next);
node_header_t *list_node_get_header(size_t node_size, void *node);
size_t list_header_size();

#endif