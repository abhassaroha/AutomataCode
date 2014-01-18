CC=gcc
CFLAGS=-c -Wall
IFLAGS=-I ./src
DFLAGS=-DHOPCROFT_ALGO
#DFLAGS=

all:
	echo "There is no all"

minimize_dfa_sources=src/minimize_dfa.c src/list.c src/double_list.c
minimize_dfa_objects=$(minimize_dfa_sources:.c=.o)
minimize_dfa: $(minimize_dfa_objects)
	$(CC) $(IFLAGS) $(DFLAGS) $(minimize_dfa_objects) -o $@ 
.c.o:
	$(CC) $(CFLAGS) $(DFLAGS) $(IFLAGS) $< -o $@

.PHONY: clean
clean :
	rm -rf ./src/*.o minimize_dfa
