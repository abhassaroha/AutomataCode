CC=gcc
CFLAGS=-c -Wall
IFLAGS=-I ./src
DFLAGS=-DHOPCROFT_ALGO
#DFLAGS=

all:
	echo "There is no all"

minimize_dfa_sources=src/minimize_dfa.c src/list.c src/double_list.c src/queue.c src/parse_dfa.c\
src/check_args.c
minimize_dfa_objects=$(minimize_dfa_sources:.c=.o)

minimize_dfa: $(minimize_dfa_objects)
	$(CC) $(IFLAGS) $(minimize_dfa_objects) -o $@ 

reg_lang_empty_src=src/reg_lang_empty.c src/parse_dfa.c src/check_args.c
reg_lang_empty_obj=$(reg_lang_empty_src:.c=.o)

reg_lang_empty: $(reg_lang_empty_obj) 
	$(CC) $(IFLAGS) $(reg_lang_empty_obj) -o $@

.c.o:
	$(CC) $(CFLAGS) $(IFLAGS) $< -o $@

.PHONY: clean
clean :
	rm -rf ./src/*.o minimize_dfa reg_lang_empty
