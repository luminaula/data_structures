#include "list.h"
#include "ptr_arim.h"
#include <stdlib.h>
#include <string.h>

typedef struct _node_header_t {
    void *prev;
    void *next;
} _node_header_t;

void *_list_node_get_next(size_t node_size, void *node);
void *_list_node_get_prev(size_t node_size, void *node);
void *_list_create_node(size_t node_size, void *prev, void *next);
_node_header_t *_list_node_get_header(size_t node_size, void *node);

linked_list_t *list_create(size_t elem_size) {
    linked_list_t *list = malloc(sizeof(linked_list_t));
    list->elem_size = elem_size;
    list->node_size = list->elem_size + 8 - list->elem_size % 8 + sizeof(void *) * 2;
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
}

void list_free(linked_list_t *list){
    if(!list){
        return;
    }
    while(list->head){
        free(list_pop_front(list));
    }
    free(list);
}


void *_list_node_get_next(size_t node_size, void *node) {
    _node_header_t *header = _list_node_get_header(node_size, node);
    return header->next;
}

void *_list_node_get_prev(size_t node_size, void *node) {
    _node_header_t *header = _list_node_get_header(node_size, node);
    return header->prev;
}

_node_header_t *_list_node_get_header(size_t node_size, void *node) {
    return (_node_header_t *)PTR_OFFSET(node, node_size - sizeof(_node_header_t));
}

void *_list_create_node(size_t node_size, void *prev, void *next) {
    void *node = calloc(1, node_size);
    _node_header_t *node_header = _list_node_get_header(node_size, node);

    if (prev) {
        _node_header_t *prev_header = _list_node_get_header(node_size, prev);
        prev_header->next = node;
        node_header->prev = prev;
    }
    if (next) {
        _node_header_t *next_header = _list_node_get_header(node_size, next);
        next_header->prev = node;
        node_header->next = next;
    }
    return node;
}

void list_push_back(linked_list_t *list, void *data) {
    void *node = _list_create_node(list->node_size, list->tail, NULL);
    memcpy(node, data, list->elem_size);

    if (!list->tail) {
        list->tail = node;
        list->head = node;
    } else {
        list->tail = node;
    }

    list->size++;
}

void list_push_front(linked_list_t *list, void *data) {
    void *node = _list_create_node(list->node_size, NULL, list->head);
    memcpy(node, data, list->elem_size);
    if (!list->head) {
        list->head = node;
        list->tail = node;
    } else {
        list->head = node;
    }
    list->size++;
}

void *list_pop_front(linked_list_t *list) {
    if(!list->head){
        return NULL;
    }
    void *node = list->head;
    _node_header_t *header = _list_node_get_header(list->node_size, node);
    void *next = header->next;
    if (next) {
        _node_header_t *next_header = _list_node_get_header(list->node_size, next);
        next_header->prev = NULL;
    }
    list->head = next;
        
    list->size--;
    return node;
}

void *list_pop_back(linked_list_t *list) {
    if(!list->tail){
        return NULL;
    }
    void *node = list->tail;
    _node_header_t *header = _list_node_get_header(list->node_size, node);
    void *prev = header->prev;
    if(prev){
        _node_header_t *prev_header = _list_node_get_header(list->node_size, prev);
        prev_header->next = NULL;
    }
    list->tail = prev;
    list->size--;
    return node;
}