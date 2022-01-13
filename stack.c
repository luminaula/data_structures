#include "stack.h"
#include "ptr_arim.h"
#include <stdlib.h>
#include <string.h>

stack_t *stack_create(size_t elem_size) {
    stack_t *stack = malloc(sizeof(stack_t));
    stack->size = 0;
    stack->capacity = 1;
    stack->data = malloc(stack->capacity * elem_size);
    stack->elem_size = elem_size;
    return stack;
}

void stack_free(stack_t *stack) {
    if (!stack) {
        return;
    }
    if (stack->data) {
        free(stack->data);
    }
    free(stack);
}

void stack_push(stack_t *stack, void *data) {
    if (stack->size == stack->capacity) {
        stack->capacity <<= 1;
        stack->data = realloc(stack->data, stack->elem_size * stack->capacity);
        stack->head = PTR_OFFSET(stack->data, stack->elem_size * stack->size);
    }
    memcpy(stack->head, data, stack->elem_size);
    PTR_ADVANCE(stack->head, stack->elem_size);
    stack->size++;
}

void *stack_pop(stack_t *stack) {
    void *data = malloc(stack->elem_size);
    PTR_ADVANCE(stack->head, -stack->elem_size);
    memcpy(data, stack->head, stack->elem_size);
    stack->size--;
    return data;
}

void *stack_pop_inplace(stack_t *stack, void *data) {
    PTR_ADVANCE(stack->head, -stack->elem_size);
    memcpy(data, stack->head, stack->elem_size);
    stack->size--;
}
