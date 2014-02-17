#include "stack.h"

#include <stdlib.h>
#include <string.h>

#include "common.h"

/**
 * Create stack with block the size of system
 * page.
 */
Stack* create_stack()
{
	int size = get_page_size()/sizeof(Stack);
	Stack* stack = (Stack*) malloc(sizeof(Stack));
	stack->block = (Elem*) malloc(sizeof(Elem) * size);
	stack->tail = 0;
	stack->size = size;
	return stack;
}

/** free up the stack memory */
void destroy_stack(Stack *stack)
{
	free(stack->block);
	free(stack);
}

/** 
 * Push with/without resizing.
 */
void stack_push(Stack *stack, int elem)
{
	if (stack->tail == stack->size) {
		int old_size = stack->size;
		stack->size *= 2;
		Elem* old_block = stack->block;
		stack->block = (Elem*) malloc(stack->size * sizeof(Elem));
		memcpy(stack->block, old_block, old_size * sizeof(Elem));
		free(old_block);
	}
	stack->block[stack->tail++].num = elem;
}

int stack_pop(Stack *stack)
{
	assert_cond(stack->tail != 0, "Stack underflow");
	return stack->block[--stack->tail].num;
} 

int stack_empty(Stack *stack)
{
	return stack->tail == 0; 
}
