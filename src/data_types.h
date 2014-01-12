#ifndef AUTOMATA__DATA__TYPES__H
#define AUTOMATA__DATA__TYPES__H

// The upper limit on states*alphabet_size 
// equals MAX_INT. 
// States are zero indexed
// By convention zero is the start state.

#include "assert.h"
#include "stdio.h"

#define assert_cond(__COND__, __MESSAGE__, ...)\
	if(!(__COND__)) {\
		printf((__MESSAGE__), ##__VA_ARGS__);\
		printf("\n");\
		assert((__COND__));\
	}
typedef struct __DFA
{
	int** transition_func;
	int num_char;
	int num_states;
	int* final_states;
	int num_final_states;
} DFA;

typedef struct __List {
	int state_num;
	struct __List *next;
} List;

List* list_insert(List *head, List *node)
{
	node->next = head;
	head = node;
	return head;
}

List* list_remove_from_front(List *head, List *node) 
{
	assert_cond(head, "Empty list");
	head = node->next;
	node->next = NULL;
	return head;
}

List* list_remove(List* head, List* node)
{
	assert_cond(head, "Empty list");
	List* previous = NULL;
	List* current = head;
	while (current != node && current != NULL) {
		previous = current;
		current = current->next;
	} 
	assert_cond(current, "Node not found in list");
	if (previous) previous->next = current->next;
	else head = current->next;
	current->next = NULL;
	return head; 
}

typedef struct __DoubleList {
	int state_num;
	struct __DoubleList *next;
	struct __DoubleList *previous;
} DoubleList;

DoubleList* double_list_insert(DoubleList* head, DoubleList* node) 
{
	node->next = head;
	node->previous = NULL;
	if (head)
		head->previous = node;
	return node;
}

DoubleList* double_list_remove(DoubleList* head, DoubleList* node) 
{
	assert_cond(head, "Empty list");
	node->next->previous = node->previous;
	if (node == head) {
		head = node->next;
	}
	else {
		node->previous->next = node->next;
	}
	return head;
}

#endif
