#ifndef AUTOMATA_COMMON_H
#define AUTOMATA_COMMON_H

#include "assert.h"
#include "stdio.h"

#define assert_cond(__COND__, __MESSAGE__, ...)\
	if(!(__COND__)) {\
		printf((__MESSAGE__), ##__VA_ARGS__);\
		printf("\n");\
		assert((__COND__));\
	}

#endif // AUTOMATA_COMMON_H
