#ifndef AUTOMATA_COMMON_H
#define AUTOMATA_COMMON_H

#include <assert.h>
#include <stdio.h>
#include "data_types.h"

#define assert_cond(__COND__, __MESSAGE__, ...)\
	if(!(__COND__)) {\
		printf((__MESSAGE__), ##__VA_ARGS__);\
		printf("\n");\
		assert((__COND__));\
	}

void check_arg_count(int argc, int count, char* usage);

DFA *parse_dfa(char *file_name);

long get_page_size();

#endif // AUTOMATA_COMMON_H
