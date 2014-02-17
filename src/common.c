#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "data_types.h"
#include <string.h>

void check_arg_count(int argc, int count, char* usage)
{
	if (argc < count) {
		printf("Insufficient arguments\n");
		printf("%s", usage);
		printf("\n");
		exit(1);
	}
}

DFA *parse_dfa(char *file_name)
{
	int num_char, num_states, num_final_states;
	FILE *in_file = fopen(file_name, "r");
	if (!in_file) {
		printf("Error opening file: %s", strerror(errno));
		exit(1);
	}

	printf("File Name %s\n\n", file_name);
	printf("In DFA\n\n");
	fscanf(in_file, "%d", &num_char);
	printf("Num char %d \n", num_char);
	fscanf(in_file, "%d", &num_states);
	printf("Num states %d \n", num_states);
	int **transition_array = (int**) malloc(sizeof(int*)*num_states); 
	int i, j;
	printf("Transition Function:\n");
	for (i = 0; i < num_states; i++)
	{
		printf("%d:", i);
		transition_array[i] = (int*) malloc(sizeof(int)*num_char);
		for (j = 0; j < num_char; j++)
		{
			fscanf(in_file, "%d", &transition_array[i][j]);
			printf("\t%d", transition_array[i][j]);
		}
		printf("\n");
	}
	fscanf(in_file, "%d", &num_final_states);
	printf("Final states: %d\n", num_final_states);
	assert_cond(num_states >= num_final_states, " Number of final\
states is greater than number of states, Num Final: %d, Num States: %d",
	num_final_states, num_states);
	printf("Final state:\t"); 
	int *final = (int*) malloc(sizeof(int)*num_final_states);
	for (i = 0; i < num_final_states; i++)
	{
		fscanf(in_file, "%d", &final[i]);
		assert_cond(final[i] < num_states, "final state index is greater\
than number of states. Index: %d, Total States: %d", final[i], num_states);
		printf("%d\t", final[i]);
	}
	printf("\n");

	DFA *in_dfa = (DFA*) malloc(sizeof(DFA));
	in_dfa->transition_func = transition_array;
	in_dfa->num_char = num_char;
	in_dfa->num_states = num_states;
	in_dfa->final_states = final;
	in_dfa->num_final_states = num_final_states;
	return in_dfa;
}

long get_page_size()
{
	return sysconf(_SC_PAGESIZE);
}
