#include "list.h"
#include "common.h"

List* list_insert(List *head, List *node)
{
	node->next = head;
	head = node;
	return head;
}

List* list_search(List* head, List* node)
{
	List* result = NULL;
	List* current = head;
	while(current) {
		if (current->state_num == node->state_num) {
			result = current;
			break;
		}
		current = current->next;
	}
	return result;
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
