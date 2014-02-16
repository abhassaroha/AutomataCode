#include "parse_dfa.h"
#include "check_args.h"

int is_reg_lang_empty(DFA* in_dfa)
{
	return 0;
}

int main(int argc, char** argv)
{
	check_arg_count(argc, 2, "Usage: min_dfa <DFA_FILE>\n");
	printf("Is Empty %d", is_reg_lang_empty(parse_dfa(argv[1])));
	return 0;
}
