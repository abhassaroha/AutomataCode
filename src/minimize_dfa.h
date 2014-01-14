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
	// for each char the num of elements
	// which have non-empty inverse transitions 
	int* count;
} Block;

DFA* minimize_dfa(DFA*);

#endif
