#ifndef AUTOMATA_STACK_H
#define AUTOMATA_STACK_H

#include "data_types.h"

typedef struct __Stack {
	int tail;
	int size;
	Elem* block;
} Stack;

/** create a stack */
Stack* create_stack();
/** free up the stack memory */
void destroy_stack(Stack *stack);

/**
 * Push to stack and resize if full.
 */
void stack_push(Stack *stack, int elem);
/**
 * Pop from stack, underflow can occur.
 */
int stack_pop(Stack *stack);
/** test emptiness */
int stack_empty(Stack *stack);

#endif
