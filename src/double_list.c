#include "double_list.h"

#include "stdlib.h"

#include "common.h"

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
