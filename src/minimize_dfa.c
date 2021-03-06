/** 
 * Take a DFA and remove indistinguishable states
 * The DFA is represented as a transition table.
 */

#include "minimize_dfa.h"
#include "string.h"
#include "common.h"
#include "queue.h"
#include "is_reg_lang_empty.h"


#ifndef HOPCROFT_ALGO
/** 
 * An implementation of :
 *
 * MR403320 (53 #7132) 68A25 (94A30) 
 * Hopcroft, John An $n$ log $n$ algorithm for minimizing states in a finite automaton.
 * Theory of machines and computations (Proc. Internat. Sympos., Technion, Haifa, 1971), pp. 189–196. Academic Press, New York, 1971.
 */

// This transformation is inverse of itself.
#define REVERSE_INDEX(__NUM__, __INDEX__) (__NUM__) - (__INDEX__) - 1 

#define COND_SWAP(FIRST, SECOND) \
int temp;\
if (FIRST > SECOND) {\
	temp = FIRST;\
	FIRST = SECOND;\
	SECOND = temp;\
}

#endif

int* find_reachable(DFA *in_dfa, DFA *out_dfa)
{
	int* reachable = (int*) malloc(sizeof(int) * in_dfa->num_states);
	memset(reachable + 1, 0, sizeof(int) * in_dfa->num_states - 1);

	Queue* queue = create_queue();
	// push the start state.
	queue_push(queue, 0);
	reachable[0] = 1;
	out_dfa->num_states++;

	// do BFS on the in_dfa transition Directed Graph.
	while(!queue_empty(queue)) {
		int elem, c_elem;
		elem = queue_pop(queue);
		for (int i = 0; i < in_dfa->num_char; i++) {
			c_elem = in_dfa->transition_func[elem][i];
			if (!reachable[c_elem]) {
				queue_push(queue, c_elem);
				reachable[c_elem] = 1;
				out_dfa->num_states++;
			}
		}
	}

	return reachable;
}

void create_transition_function(DFA *in_dfa, DFA *out_dfa, int *state_map)
{
	int* reachable = find_reachable(in_dfa, out_dfa);
	// transition function
	out_dfa->transition_func = (int**) malloc(sizeof(int*) 
			* out_dfa->num_states);
	memset(state_map, -1, sizeof(int)*in_dfa->num_states);
	int count = 0;
	for (int i = 0; i < in_dfa->num_states; i++) {
		if (reachable[i])
			state_map[i] = count++; 
	}
	assert_cond(count == out_dfa->num_states, "element count mismatch");

	for (int i = 0; i < in_dfa->num_states; i++) {
		if (reachable[i]) {
			out_dfa->transition_func[state_map[i]] = (int*) malloc(sizeof(int) 
					* in_dfa->num_char);
			for (int j = 0; j < in_dfa->num_char; j++) { 
				assert_cond(state_map[in_dfa->transition_func[i][j]] != -1,
						"Some reachable state did not get marked");
				out_dfa->transition_func[state_map[i]][j] = 
					state_map[in_dfa->transition_func[i][j]];
			}
		}
	}

	free(reachable);
}

void find_final_states(DFA *in_dfa, DFA *out_dfa, int *state_map)
{
	int* final = (int*) malloc(sizeof(int) * out_dfa->num_states);
	out_dfa->num_final_states = 0;
	memset(final, 0, sizeof(int) * out_dfa->num_states);
	for (int i = 0; i < in_dfa->num_final_states; i++) {
		int state;
		state = state_map[in_dfa->final_states[i]];
		if (state != -1) {
			final[state] = 1;
			out_dfa->num_final_states++;
		}
	}
	assert_cond(out_dfa->num_final_states <= in_dfa->num_final_states,
			"final states in new dfa should be less or equal");

	out_dfa->final_states = (int*) malloc(sizeof(int) * out_dfa->num_final_states);
	int count = 0;
	for (int i = 0; i < out_dfa->num_states; i++) {
		if (final[i]) out_dfa->final_states[count++] = i;
	}

	free(final);
}


DFA *remove_unreachable(DFA *in_dfa)
{
	DFA *out_dfa = (DFA*) malloc(sizeof(DFA));
	out_dfa->num_states = 0;

	int* state_map = (int*) malloc(sizeof(int) * in_dfa->num_states);
	create_transition_function(in_dfa, out_dfa, state_map);
	find_final_states(in_dfa, out_dfa, state_map);
	free(state_map);

	out_dfa->num_char = in_dfa->num_char;
	return out_dfa; 
}

#ifdef HOPCROFT_ALGO

#define DOUBLE_LIST_INSERT(STATE, BLOCK_NUM, HEAD)\
	blocks[BLOCK_NUM]->HEAD = double_list_insert(blocks[BLOCK_NUM]->HEAD,\
			&dbl_lst_nodes[STATE]);\
	dbl_lst_nodes[STATE].block_num = BLOCK_NUM;\
	blocks[BLOCK_NUM]->num_elem++;\
	for (j = 0; j < in_dfa->num_char; j++) {\
		if (inv_trans_func[STATE][j]->count != 0)\
		blocks[BLOCK_NUM]->elem_count[j]++;\
	}

#define DOUBLE_LIST_REMOVE(STATE, BLOCK_NUM, HEAD)\
	blocks[BLOCK_NUM]->HEAD = double_list_remove(blocks[BLOCK_NUM]->HEAD,\
			&dbl_lst_nodes[STATE]);\
	blocks[BLOCK_NUM]->num_elem--;\
	for (j = 0; j < in_dfa->num_char; j++) {\
		if (inv_trans_func[STATE][j]->count != 0)\
		blocks[BLOCK_NUM]->elem_count[j]--;\
	}

DoubleList* create_dbl_lst_nodes(int num_states)
{
	// vector of nodes
	DoubleList *dbl_lst_nodes = (DoubleList*) malloc(sizeof(DoubleList) * num_states);
	for (int i = 0; i < num_states; i++) {
		dbl_lst_nodes[i].state_num = i;
		dbl_lst_nodes[i].block_num = -1;
		dbl_lst_nodes[i].to_move = 0;
		dbl_lst_nodes[i].previous = dbl_lst_nodes[i].next = NULL;
	}
	return dbl_lst_nodes;
}

void destroy_dbl_lst_nodes(DoubleList* dbl_lst_nodes)
{
	free(dbl_lst_nodes);
}

InverseTrans*** crt_inv_transtn_tbl(DFA *in_dfa, DoubleList *nodes)
{
	int i, j;
	// Create inverse transition table.
	InverseTrans ***inv_trans_func = (InverseTrans***) 
		malloc(sizeof(InverseTrans**) * in_dfa->num_states);
	for (i = 0; i < in_dfa->num_states; i++) { 
		inv_trans_func[i] = (InverseTrans**) 
			malloc(sizeof(InverseTrans*) * in_dfa->num_char);
		for (j = 0; j < in_dfa->num_char; j++) {
			inv_trans_func[i][j] = (InverseTrans*) malloc(sizeof(InverseTrans));
			inv_trans_func[i][j]->list = NULL;
			inv_trans_func[i][j]->count = 0;
		}
	}

	List *list_node;
	int target;
	for (i = 0; i < in_dfa->num_states; i++) {
		for (j = 0; j < in_dfa->num_char; j++) {
			list_node = (List*) malloc(sizeof(List));
			list_node->dbl_node = &nodes[i];
			list_node->state_num = i;
			target = in_dfa->transition_func[i][j];
			inv_trans_func[target][j]->count++;
			inv_trans_func[target][j]->list = list_insert(inv_trans_func[target][j]->list,
					list_node);
		}
	}
	return inv_trans_func;
}

void destroy_inv_transtn_tbl(InverseTrans*** inv_trans_func, DFA* in_dfa)
{
	for (int i = 0; i < in_dfa->num_states; i++) {
		for (int j = 0; j < in_dfa->num_char; j++) {
			free(inv_trans_func[i][j]);
		}
		free(inv_trans_func[i]);
	}
	free(inv_trans_func);
}

Block** initialize_blocks(DFA *in_dfa, DoubleList *dbl_lst_nodes, InverseTrans ***inv_trans_func)
{
	int i,j;
	// vector of blocks
	Block **blocks = (Block**) malloc(sizeof(Block*) * in_dfa->num_states);
	for (i = 0; i < in_dfa->num_states; i++) {
		blocks[i] = (Block*) malloc(sizeof(Block));
		blocks[i]->head = NULL;
		blocks[i]->move_head = NULL;
		blocks[i]->elem_count = (int*) malloc(sizeof(int)*in_dfa->num_char);
		memset(blocks[i]->elem_count, 0, in_dfa->num_char*sizeof(int));
		blocks[i]->num_elem = 0;
		blocks[i]->move_count = 0;
	}

	int state;
	for (i = 0; i < in_dfa->num_final_states; i++) {
		state = in_dfa->final_states[i];
		DOUBLE_LIST_INSERT(state, 0, head);
	}

	for (i = 0; i < in_dfa->num_states; i++) {
		if (dbl_lst_nodes[i].block_num == -1) {
			DOUBLE_LIST_INSERT(i, 1, head);
		}
	}

	return blocks;
}

void destroy_blocks(Block** blocks, int num_states)
{
	for (int i = 0; i < num_states; i++) {
		free(blocks[i]);
	}
	free(blocks);
}

#if DEBUG
void print_blocks(Block** blocks, int num_states, int num_char)
{
	DoubleList* dbl_node;
	for (int i = 0; i < num_states; i++) {
		printf("Block %d\n", i);
		for (int j = 0; j < num_char; j++)
			printf("%d ", blocks[i]->elem_count[j]); 
		printf("\n");
		dbl_node = blocks[i]->head;
		if (dbl_node) {
			printf("%d", dbl_node->state_num);
			dbl_node = dbl_node->next;
			while (dbl_node) {
				printf(" -> ");
				printf("%d", dbl_node->state_num);
				dbl_node = dbl_node->next;
			}
			printf("\n");
		}
		else printf("empty!\n");
	}
	printf("\n\n");
}
#endif

List** initialize_l_sets(DFA* in_dfa, Block** blocks)
{
	int i;
	List **l_sets = (List**) malloc(sizeof(List*)*in_dfa->num_char);
	for (i = 0; i < in_dfa->num_char; i++)
		l_sets[i] = NULL;

	// init L sets for each char
	List *node;
	for (i = 0; i < in_dfa->num_char; i++) {
		node = (List*) malloc(sizeof(List));
		if (blocks[0]->elem_count[i] <= blocks[1]->elem_count[i] 
				&& blocks[0]->elem_count[i] != 0) {
			node->state_num = 0;
			l_sets[i] = list_insert(l_sets[i], node);
		}
		else if(blocks[1]->elem_count[i] != 0) {
			node->state_num = 1;
			l_sets[i] = list_insert(l_sets[i], node);
		}
		else if (blocks[0]->elem_count[i] != 0) {
			node->state_num = 0;
			l_sets[i] = list_insert(l_sets[i], node);
		}
	}
	return l_sets;
}

void destroy_l_sets(List** l_sets)
{
	free(l_sets);
}

#if DEBUG 
void print_l_sets(List** l_sets, int num_char)
{
	List *node;
	for (int i = 0; i < num_char; i++) {
		printf("Lset num %d\n", i);
		node = l_sets[i];
		if (node) {
			printf("%d", node->state_num);
			node = node->next;
			while(node) printf(" -> %d", node->state_num);
		}
		printf("\n");
	}
}
#endif

int refine_blocks(DFA *in_dfa, InverseTrans ***inv_trans_func, List **l_sets,
		Block **blocks, DoubleList* dbl_lst_nodes)
{
	int i, j, num_states = 2, ref_block, from_block;
	List *frm_state, *node;
	DoubleList *dbl_node, *to_state;
	List *move_head = NULL;
	List *move_nde_lst = (List*) malloc(sizeof(List) 
			* in_dfa->num_states);
	for (i = 0; i < in_dfa->num_states; i++) {
		move_nde_lst[i].block_num = i;
		move_nde_lst[i].to_move = 0;
		move_nde_lst[i].next = NULL;
	}


	while (1) {
#if DEBUG 
		print_l_sets(l_sets, in_dfa->num_char);
		print_blocks(blocks, num_states, in_dfa->num_char);
#endif
		// pick one set from L and one char
		for (i = 0; i < in_dfa->num_char; i++) { 
			if (l_sets[i] != NULL)
				break;
		}
		if (i != in_dfa->num_char) {
			node = l_sets[i]; 
			l_sets[i] = node->next;
			ref_block = node->state_num;
			free(node);
			// refine using that set and char
			// put blocks on copy list
			to_state = blocks[ref_block]->head; 
			while (to_state != NULL) {
				frm_state = inv_trans_func[to_state->state_num][i]->list;
				while (frm_state != NULL) {
					dbl_node = frm_state->dbl_node;
					from_block = dbl_node->block_num;
					if (from_block != ref_block 
							&& dbl_node->to_move == 0) {
						blocks[from_block]->head = double_list_remove(blocks[from_block]->head,
								dbl_node);
						blocks[from_block]->move_head = double_list_insert(
								blocks[from_block]->move_head,
								dbl_node);
						dbl_node->to_move = 1;
						if (move_nde_lst[from_block].to_move == 0) {
							move_head = list_insert(move_head, &move_nde_lst[from_block]);
							move_nde_lst[from_block].to_move = 1;
						}
						blocks[from_block]->move_count++;
					}
					frm_state = frm_state->next;
				}
				to_state = to_state->next;
			} // put on copy list
			while (move_head) {
				from_block = move_head->block_num;
				if (blocks[from_block]->move_count != blocks[from_block]->num_elem) {
					// copy to new block 
					dbl_node = blocks[from_block]->move_head;
					do {
						DOUBLE_LIST_REMOVE(dbl_node->state_num, from_block, move_head);
						DOUBLE_LIST_INSERT(dbl_node->state_num, num_states, head);
						dbl_node->to_move = 0;
						dbl_node = blocks[from_block]->move_head;
					} while (dbl_node);
					// update l_sets
					for (j = 0; j < in_dfa->num_char; j++) {
						node = (List*) malloc(sizeof(List));
						if (blocks[num_states]->elem_count[j] <= blocks[from_block]->elem_count[j]
								&& blocks[num_states]->elem_count[j] != 0) {
							node->state_num = num_states;
							l_sets[j] = list_insert(l_sets[j], node);
						} 
						else if (blocks[from_block]->elem_count[j] != 0) { 
							node->state_num = from_block;
							if (list_search(l_sets[j], node) == NULL)
								l_sets[j] = list_insert(l_sets[j], node);
						}
						else if (blocks[num_states]->elem_count[j] != 0) {
							node->state_num = num_states;
							l_sets[j] = list_insert(l_sets[j], node);
						}
					}
					num_states++;
				}
				else {
					// copy back from move head to head
					dbl_node = blocks[from_block]->move_head;
					do {
						blocks[from_block]->move_head = double_list_remove(
								blocks[from_block]->move_head,
								dbl_node);
						blocks[from_block]->head = double_list_insert(blocks[from_block]->head,
								dbl_node);
						dbl_node->to_move = 0;
						dbl_node = blocks[from_block]->move_head;
					} while (dbl_node);
				}
				blocks[from_block]->move_count = 0;
				move_head->to_move = 0;
				move_head = list_remove(move_head, move_head);
			}
		} // if i found
		else {
			break;
		}
	}

	free(move_nde_lst);
	return num_states;
}

DFA* create_out_dfa(DFA *in_dfa, DoubleList *dbl_lst_nodes,
		Block **blocks, int num_states )
{
	// Create out DFA
	int state;
	DFA *out_dfa = (DFA*) malloc(sizeof(DFA));
	out_dfa->num_states = num_states;
	out_dfa->num_char = in_dfa->num_char;
	out_dfa->transition_func = (int**) malloc(sizeof(int*) * num_states);
	out_dfa->num_final_states = 0;

	// Calculate old final states.
	int* old_final_states = (int*) malloc(sizeof(int) * in_dfa->num_states);
	int* final_states = (int*) malloc(sizeof(int) * num_states);
	memset(old_final_states, 0, sizeof(int) * in_dfa->num_states);
	memset(final_states, 0, sizeof(int) * num_states);
	for (int i = 0; i < in_dfa->num_final_states; i++) {
		old_final_states[in_dfa->final_states[i]] = 1;
	}

	// Create new transition function and mark new final states.
	for (int i = 0; i < num_states; i++) {
		out_dfa->transition_func[i] = (int*) malloc(sizeof(int) * in_dfa->num_char);
		for (int j = 0; j < in_dfa->num_char; j++) {
			state = in_dfa->transition_func[blocks[i]->head->state_num][j];
			out_dfa->transition_func[i][j] = dbl_lst_nodes[state].block_num; 
		}
		state = blocks[i]->head->state_num;
		if (old_final_states[state]) { 
			final_states[i] = 1;
			out_dfa->num_final_states++;
		}
	}

	// create new final states list.
	out_dfa->final_states = (int*) malloc(sizeof(int) * out_dfa->num_final_states);
	int j = 0;
	for (int i = 0; i < num_states; i++) {
		if (final_states[i]) {
			out_dfa->final_states[j] = i;
			j++;
		}
	}

	// free up the memory
	free(final_states);
	free(old_final_states);

	return out_dfa;
}


// Hopcroft's partitioning into equivalence set
// algorithm.
DFA *remove_indistinguishable(DFA *in_dfa)
{
	DoubleList* dbl_lst_nodes = create_dbl_lst_nodes(in_dfa->num_states);
	InverseTrans ***inv_trans_func = crt_inv_transtn_tbl(in_dfa, dbl_lst_nodes);
	Block **blocks = initialize_blocks(in_dfa, dbl_lst_nodes, inv_trans_func); 
	List **l_sets = initialize_l_sets(in_dfa, blocks);
	int num_states = refine_blocks(in_dfa, inv_trans_func, l_sets, blocks,
			dbl_lst_nodes);
	destroy_l_sets(l_sets);
	destroy_inv_transtn_tbl(inv_trans_func, in_dfa);
	DFA *out_dfa = create_out_dfa(in_dfa, dbl_lst_nodes, blocks, num_states);

	// Free up memory
	destroy_blocks(blocks, in_dfa->num_states);
	destroy_dbl_lst_nodes(dbl_lst_nodes);
	// Free up the in_dfa and associated data. 
	for (int i = 0; i < in_dfa->num_states; i++)
		free(in_dfa->transition_func[i]);
	free(in_dfa->transition_func);
	free(in_dfa->final_states);
	free(in_dfa);

	return out_dfa;
}
#else
// The inductive discovery algorithm for 
// finding distinguishable pairs
DFA *remove_indistinguishable(DFA *in_dfa)
{
	int i, j;
	int states = in_dfa->num_states;
	int chars = in_dfa->num_char;
	int *final_states = (int*) malloc(sizeof(int)*states);
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
	int **dist_pairs = (int**) malloc(sizeof(int*) * (states - 1));	
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
	int *indist_states = (int*) malloc(sizeof(int)*states);
	int *new_final_states = (int*) malloc(sizeof(int)*states);
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

	DFA *out_dfa = (DFA*) malloc(sizeof(DFA));
	int **transition_function = (int**) malloc(sizeof(int*)*new_index);
	int last_group = -1;
	for (i = 0; i < states; i++)
	{
		if (indist_states[i] <= last_group) continue;
		transition_function[++last_group] = (int*) malloc(sizeof(int)*chars);
		assert_cond(last_group == indist_states[i], "Group being considered does not\
match states new index");
		for (j = 0; j < chars; j++)
		{
			transition_function[last_group][j] =
				indist_states[in_dfa->transition_func[i][j]];
		}
	}
	assert_cond(last_group + 1 == new_index, "Number of rows added to transition \
function does not match new state count. LG: %d, NI: %d", last_group, new_index);

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
#endif

DFA *minimize_dfa(DFA *in_dfa)
{
	if (is_reg_lang_empty(in_dfa)) {
		printf("Input regular language is empty, can't be minimized!\n");
		exit(EXIT_FAILURE);
	}
	DFA* out_dfa = remove_unreachable(in_dfa);
	printf("Reachable states %d\n\n", out_dfa->num_states);
	out_dfa = remove_indistinguishable(out_dfa);
	return out_dfa;
}

int main(int argc, char **argv)
{
	int i, j;
	check_arg_count(argc, 2, "Usage: mindfa <DFA_FILE>\n");
	DFA *out_dfa = minimize_dfa(parse_dfa(argv[1]));
	printf("Out DFA\n\n");
	printf("Num states %d \n", out_dfa->num_states);
	printf("Num char %d \n", out_dfa->num_char);
	printf("Transition Function:\n");
	for (i = 0; i < out_dfa->num_states; i++)
	{
		printf("%d:", i);
		for (j = 0; j < out_dfa->num_char; j++)
		{
			printf("\t%d", out_dfa->transition_func[i][j]);
		}
		printf("\n");
	}
	printf("Final states: %d\n", out_dfa->num_final_states);
	printf("Final state:\t"); 
	for (i = 0; i < out_dfa->num_final_states; i++)
	{
		printf("%d\t", out_dfa->final_states[i]);
	}
	printf("\n\n");
	return 0;
}
