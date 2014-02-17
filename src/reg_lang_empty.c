#include <stdio.h>
#include "common.h"
#include "is_reg_lang_empty.h"

int main(int argc, char** argv)
{
	check_arg_count(argc, 2, "Usage: rlempty <DFA_FILE>\n");
	printf("Is Empty %d\n\n", is_reg_lang_empty(parse_dfa(argv[1])));
	return 0;
}
