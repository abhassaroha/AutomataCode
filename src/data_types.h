#ifndef AUTOMATA__DATA__TYPES__H
#define AUTOMATA__DATA__TYPES__H

// The upper limit on states*alphabet_size 
// equals MAX_INT. 
// States are zero indexed
// By convention zero is the start state.

#include "stdio.h"

typedef struct __DFA
{
	int** transition_func;
	int num_char;
	int num_states;
	int* final_states;
	int num_final_states;
} DFA;

// generic elem with a number field.
typedef struct __Elem {
	int num;
} Elem;

#endif
