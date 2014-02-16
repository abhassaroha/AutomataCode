#include <stdio.h>
#include <stdlib.h>

void check_arg_count(int argc, int count, char* usage)
{
	if (argc < count) {
		printf("Insufficient arguments\n");
		printf("%s", usage);
		printf("\n");
		exit(1);
	}
}
