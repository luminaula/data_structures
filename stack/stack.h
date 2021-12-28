#ifndef STACK_H
#define STACK_H

#include <stddef.h>

typedef struct stack_t{
	void *data;
	void *head;
	size_t size;
	size_t capacity;
	size_t elem_size;
}stack_t;


stack_t *stack_create(size_t elem_size);
void stack_free(stack_t *stack);
void stack_push(stack_t *stack, void *data);
void *stack_pop(stack_t *stack);
void *stack_pop_inplace(stack_t *stack,void *data);


#endif