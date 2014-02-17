CC=gcc
CFLAGS=-g -c -Wall
IFLAGS=-I ./src
DFLAGS=-DHOPCROFT_ALGO
#DFLAGS=

all: mindfa rlempty

mindfa_src=src/minimize_dfa.c src/list.c src/double_list.c src/queue.c\
src/common.c src/is_reg_lang_empty.c src/stack.c
mindfa_obj=$(patsubst src/%, bin/%, $(mindfa_src:.c=.o))

mindfa: $(mindfa_obj)
	$(CC) $(IFLAGS) $(mindfa_obj) -o bin/$@ 

rlempty_src=src/reg_lang_empty.c src/is_reg_lang_empty.c src/common.c src/stack.c
rlempty_obj=$(patsubst src/%, bin/%, $(rlempty_src:.c=.o))

rlempty: $(rlempty_obj) 
	$(CC) $(IFLAGS) $(rlempty_obj) -o bin/$@

bin/minimize_dfa.o: src/minimize_dfa.c
	$(CC) $(DFLAGS) $(CFLAGS) $(IFLAGS) $< -o $@
bin/%.o: src/%.c
	$(CC) $(CFLAGS) $(IFLAGS) $< -o $@

.PHONY: clean
clean :
	rm -f ./bin/*.o bin/mindfa bin/rlempty
