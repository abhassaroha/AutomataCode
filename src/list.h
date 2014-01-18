#ifndef AUTOMATA_LIST_H
#define AUTOMATA_LIST_H

#include "double_list.h"

typedef struct __List {
	DoubleList* dbl_node;
	union {
		int state_num;
		int block_num;
	};
	int to_move;
	struct __List *next;
} List;

List* list_search(List *head, List *node);
List* list_insert(List *head, List *node);
List* list_remove_from_front(List *head); 
List* list_remove(List *head, List *node);

#endif
