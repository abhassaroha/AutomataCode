#include "is_reg_lang_empty.h"

#include <stdlib.h>
#include <string.h>

#include "stack.h"

/**
 * If final state is reachable from the start state,
 * there is atleast one string which belongs to this language.
 * Otherwise this language is empty.
 */
int is_reg_lang_empty(DFA* in_dfa)
{
	int result = 1;

	int *final = (int*) malloc(in_dfa->num_states * sizeof(int));
	memset(final, 0, sizeof(int) * in_dfa->num_states);

	for (int i = 0; i < in_dfa->num_final_states; i++)
		final[in_dfa->final_states[i]] = 1;

	if (!final[0]) {
		Stack* stack = create_stack();
		// push the start state.
		stack_push(stack, 0);


		int *reachable = (int*) malloc(in_dfa->num_states * sizeof(int));
		memset(reachable + 1, 0, sizeof(int) * in_dfa->num_states - 1);
		reachable[0] = 1;

		// do a DFS to determine if any final state is reachable.
		while (!stack_empty(stack)) {
			int elem, c_elem;
			elem = stack_pop(stack);
			for (int i = 0; i < in_dfa->num_char; i++) {
				c_elem = in_dfa->transition_func[elem][i];
				if (final[c_elem] == 1) {
					result = 0;
					break;
				}
				if (!reachable[c_elem]) {
					reachable[c_elem] = 1;
					stack_push(stack, c_elem);
				}
			}
		} 

		// free memory 
		free(reachable);
		destroy_stack(stack);
	}
	else {
		// start state is final state
		result = 0;
	}

	// free memory 
	free(final);

	return result;
}

