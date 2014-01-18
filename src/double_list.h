#ifndef AUTOMATA_DOUBLE_LIST_H
#define AUTOMATA_DOUBLE_LIST_H

typedef struct __DoubleList {
	int state_num;
	int block_num;
	int to_move;
	struct __DoubleList *next;
	struct __DoubleList *previous;
} DoubleList;

DoubleList* double_list_insert(DoubleList* head, DoubleList* node); 
DoubleList* double_list_remove(DoubleList* head, DoubleList* node); 
#endif
