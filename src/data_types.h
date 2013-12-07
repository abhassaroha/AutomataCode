// The upper limit on states*alphabet_size 
// equals MAX_INT. 
// States are zero indexed
// By convention zero is the start state.

#include "assert.h"
#include "stdio.h"

#define assert_cond(cond, message, ...)\
	if(!cond) {\
		printf(message, ##__VA_ARGS__);\
		printf("\n");\
		assert(cond);\
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
