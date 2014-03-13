#include "is_reg_lang_empty.h"
#include "data_types.h"
#include "common.h"
#include <string.h>

#define UNEXPLORED 0
#define EXPLORING 1
#define EXPLORED 2

int do_dfs(DFA *dfa, int state, int *states)
{
	int result = 0;
	// cycle found.
	if(states[state] == EXPLORING)  {
		result = 1;
	}
	else if (states[state] == UNEXPLORED) {
		states[state] = EXPLORING;
		for (int i = 0; i < dfa->num_char; i++) {
			if (
					do_dfs(dfa, dfa->transition_func[state][i],
						states)
			   ) {
				result = 1;
				break;
			}
		}
		states[state] = EXPLORED;
	}
	return result;
}

// do DFS to search for a cycle.
int has_cycle(DFA *dfa)
{
	int states[dfa->num_states];
	memset(states, UNEXPLORED, sizeof(int) * dfa->num_states);
	return do_dfs(dfa, 0, states);
}

int main(int argc, char **argv)
{
	check_arg_count(argc, 2, "Usage: infnt <file_path>");
	DFA *dfa = parse_dfa(argv[1]);
	int result = !is_reg_lang_empty(dfa);
	if (result) {
		result = has_cycle(dfa);
	}
	printf("Is infinite %d\n", result);
	return 0;
}
