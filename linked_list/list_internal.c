#include "list_internal.h"
#include "ptr_arim.h"
#include <stdlib.h>

void *list_node_get_next(size_t node_size, void *node) {
    node_header_t *header = list_node_get_header(node_size, node);
    return header->next;
}

void *list_node_get_prev(size_t node_size, void *node) {
    node_header_t *header = list_node_get_header(node_size, node);
    return header->prev;
}

node_header_t *list_node_get_header(size_t node_size, void *node) {
    return (node_header_t *)PTR_OFFSET(node, node_size - sizeof(node_header_t));
}

void *list_create_node(size_t node_size, void *prev, void *next) {
    void *node = calloc(1, node_size);
    list_link_node(node_size, node, prev, next);
    return node;
}

void list_link_node(size_t node_size, void *node, void *prev, void *next) {
    node_header_t *node_header = list_node_get_header(node_size, node);

    if (prev) {
        node_header_t *prev_header = list_node_get_header(node_size, prev);
        prev_header->next = node;
        node_header->prev = prev;
    }
    if (next) {
        node_header_t *next_header = list_node_get_header(node_size, next);
        next_header->prev = node;
        node_header->next = next;
    }
}