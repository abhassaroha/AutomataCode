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
struct DFA
{
	int** transition_func;
	int num_char;
	int num_states;
	int* final_states;
	int num_final_states;
};

typedef struct DFA DFA;

#endif
