#ifndef AUTOMATA_MINIMIZE__DFA__H
#define AUTOMATA_MINIMIZE__DFA__H

#include "data_types.h"
#include "errno.h"
#include "stdlib.h"
#include "string.h"

typedef struct __InverseTrans {
	List * list;
	int count;
} InverseTrans;

typedef struct __Block {
	DoubleList* head;
	DoubleList* move_head;
	// for each char the num of elements
	// which have non-empty inverse transitions 
	int* elem_count;
	// total elem in this block
	int num_elem;
	int move_count;
} Block;

DFA* minimize_dfa(DFA*);

#endif
