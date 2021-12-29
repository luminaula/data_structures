#include "list.h"
#include "ptr_arim.h"
#include <stdlib.h>
#include <string.h>
#include "list_internal.h"


linked_list_t *list_create(size_t elem_size) {
    linked_list_t *list = malloc(sizeof(linked_list_t));
    list->elem_size = elem_size;
    list->node_size = list->elem_size + 8 - list->elem_size % 8 + sizeof(void *) * 2;
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void list_free(linked_list_t *list) {
    if (!list) {
        return;
    }
    while (list->size) {
        free(list_pop_front(list));
    }
    free(list);
}



void list_push_back(linked_list_t *list, void *data) {
    void *node = list_create_node(list->node_size, list->tail, NULL);
    memcpy(node, data, list->elem_size);

    list->tail = node;
    if (!list->head) {
        list->head = node;
    }

    list->size++;
}

void list_move_back(linked_list_t *list, void *data) { list_link_node(list->node_size, data, list->tail, NULL); }

void list_move_front(linked_list_t *list, void *data) { list_link_node(list->node_size, data, NULL, list->head); }

void list_push_front(linked_list_t *list, void *data) {
    void *node = list_create_node(list->node_size, NULL, list->head);
    memcpy(node, data, list->elem_size);

    list->head = node;
    if (!list->tail) {
        list->tail = node;
    }

    list->size++;
}

void *list_pop_front(linked_list_t *list) {
    if (!list->head) {
        return NULL;
    }
    void *node = list->head;
    void *next = list_node_get_next(list->node_size, node);
    if (next) {
        node_header_t *next_header = list_node_get_header(list->node_size, next);
        next_header->prev = NULL;
    } else {
        list->tail = NULL;
    }
    list->head = next;

    list->size--;
    return node;
}

void *list_pop_back(linked_list_t *list) {
    if (!list->tail) {
        return NULL;
    }
    void *node = list->tail;
    void *prev = list_node_get_prev(list->node_size, node);
    if (prev) {
        node_header_t *prev_header = list_node_get_header(list->node_size, prev);
        prev_header->next = NULL;
    } else {
        list->head = NULL;
    }
    list->tail = prev;
    list->size--;
    return node;
}

size_t list_header_size() { return sizeof(node_header_t); }
