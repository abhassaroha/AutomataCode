#ifndef AUTOMATA_STACK_H
#define AUTOMATA_STACK_H

#include "data_types.h"

typedef struct __Stack {
	int tail;
	int size;
	Elem* block;
} Stack;

Stack* create_stack();
void destroy_stack(Stack *stack);

void stack_push(Stack *stack, int elem);
int stack_pop(Stack *stack);
int stack_empty(Stack *stack);

#endif
