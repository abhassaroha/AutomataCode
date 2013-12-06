/** 
 * Take a DFA and remove indistinguishable states
 * The DFA is represented as a transition table.
 */

#include "minimize_dfa.h"

// This transformation is inverse of itself.
#define REVERSE_INDEX(__NUM__, __INDEX__) (__NUM__) - (__INDEX__) - 1 

#define COND_SWAP(FIRST, SECOND) \
int temp;\
if (FIRST > SECOND) {\
	temp = FIRST;\
	FIRST = SECOND;\
	SECOND = temp;\
}

DFA* remove_unreachable(DFA* in_dfa)
{
	DFA* out_dfa = (DFA*) malloc(sizeof(DFA));
	return out_dfa; 
}

DFA* remove_indistinguishable(DFA* in_dfa)
{
	int i, j;
	int states = in_dfa->num_states;
	int chars = in_dfa->num_char;
	int* final_states = (int*) malloc(sizeof(int)*states);
	memset(final_states, 0, states*sizeof(int));
	// copy over final states info
	for (i = 0; i < in_dfa->num_final_states; i++)
	{
		final_states[in_dfa->final_states[i]] = 1;
	}

	// dist_pairs stores columns in reverse and does not
	// have column for index 0 (state 0)
	// rows are stored from 0 but last state does not have
	// a row.
	// Only the upper triangle is of interest.
	int** dist_pairs = (int**) malloc(sizeof(int*) * (states - 1));	
  // Base case:
	// distinguish final and non-final
	for (i = 0; i < states - 1; i++)
	{
		dist_pairs[i] = (int*) malloc(sizeof(int) * (states - 1));
		for (j = 0; j < REVERSE_INDEX(states, i); j++)
		{
			if (final_states[i] != final_states[REVERSE_INDEX(states, j)])
				dist_pairs[i][j] = 1;
			else
				dist_pairs[i][j] = 0;
		}
	}

  // Induction:
	// All pair which transition to distinguished pair 
	// on some input alphabet are distinguishable
	int added, k, row, col;
	do {
		added = 0;
		for (i = 0; i < states - 1; i++)
		{
			for (j = 0; j < REVERSE_INDEX(states, i); j++)
			{
				if (dist_pairs[i][j] == 1) continue;
				for ( k = 0; k < chars; k++)
				{
					row = in_dfa->transition_func[i][k];
					col = in_dfa->transition_func[REVERSE_INDEX(states, j)][k];
					if (col != row) 
					{
						COND_SWAP(row, col);
						if (dist_pairs[row][REVERSE_INDEX(states, col)] == 1) {
							dist_pairs[i][j] = 1;
							added = 1;
							break;
						}
					}
				} 
			}
		}
	} while (added); // keep doing till last iteration produced a new pair

	int new_index = 0, num_final = 0;
	int* indist_states = (int*) malloc(sizeof(int)*states);
	int* new_final_states = (int*) malloc(sizeof(int)*states);
	for (i = 0; i < states; i++)
		indist_states[i] = -1;

	// re-calculate state indexes
	for (i = 0; i < states - 1; i++)
	{
		if (indist_states[i] == -1) {
			 indist_states[i] = new_index;
			 if (final_states[i])
				 new_final_states[num_final++] = new_index;
			 new_index++;
		}

		for (j = 0; j < REVERSE_INDEX(states, i); j++)
		{
			if (indist_states[REVERSE_INDEX(states, j)] == -1 && dist_pairs[i][j] == 0)
			{
				indist_states[REVERSE_INDEX(states, j)] = indist_states[i];
			}
		}
	}
	// check if last state was considered
	if (indist_states[states - 1] == -1) {
		indist_states[states - 1] = new_index;
		if (final_states[i])
			new_final_states[num_final++] = new_index;
		new_index++;
	}

	DFA* out_dfa = (DFA*) malloc(sizeof(DFA));
	int** transition_function = (int**) malloc(sizeof(int*)*new_index);
	int last_group = -1;
	for (i = 0; i < states; i++)
	{
		if (indist_states[i] < last_group) continue;
		transition_function[++last_group] = (int*) malloc(sizeof(int)*chars);
		assert_cond(last_group == indist_states[i], "Group being considered does not\
				match states new index");
		for (j = 0; j < chars; j++)
		{
			transition_function[last_group][j] = in_dfa->transition_func[i][j];
		}
	}
	assert_cond(last_group == new_index, "Number of rows added to transition \
			function does not match state count");

	out_dfa->transition_func = transition_function;
	out_dfa->num_char = chars;
	out_dfa->num_states = new_index;
	out_dfa->final_states = (int*) malloc(num_final*sizeof(int));
	memcpy(out_dfa->final_states, new_final_states, sizeof(int)*num_final);
	out_dfa->num_final_states = num_final;

	// mem cleanup
	free(final_states);
	for (i = 0; i < states - 1; i++)
		free(dist_pairs[i]);
	free(dist_pairs);
	free(indist_states);
	free(new_final_states);

	return out_dfa; 
}

DFA* minimize_dfa(DFA* in_dfa)
{
	//DFA* out_dfa = remove_unreachable(in_dfa);
	DFA* out_dfa = remove_indistinguishable(in_dfa);
	return out_dfa;
}

DFA* parse_dfa(char* file_name)
{
	int num_char, num_states, num_final_states;
	// TODO: parse dfa file
	int** transition_array = (int**) malloc(sizeof(int*)*num_states); 
	int* final = (int*) malloc(sizeof(int)*num_final_states);
	DFA* in_dfa;
	in_dfa->transition_func = transition_array;
	in_dfa->num_char = num_char;
	in_dfa->num_states = num_states;
	in_dfa->final_states = final;
	in_dfa->num_final_states = num_final_states;
	return in_dfa;
}
 
int main(int argc, char** argv)
{

	DFA* out_dfa = minimize_dfa(parse_dfa("dfa1"));
	printf("Num states %d \n", out_dfa->num_states);
	printf("Num char %d \n", out_dfa->num_char);
	printf("Transition Function:\n");
	printf("0:\t%d\t%d\n", out_dfa->transition_func[0][0],
			out_dfa->transition_func[0][1]);
	printf("0:\t%d\t%d\n", out_dfa->transition_func[1][0],
			out_dfa->transition_func[1][1]);
	printf("Final states: %d\n", out_dfa->num_final_states);
	printf("Final state: %d\n", out_dfa->final_states[0]);
	return 0;
}
