CC=gcc
CFLAGS=-g -c -Wall
IFLAGS=-I ./src
DFLAGS=-DHOPCROFT_ALGO
#DFLAGS=

all: mindfa rlempty

mindfa_sources=src/minimize_dfa.c src/list.c src/double_list.c src/queue.c\
src/common.c src/is_reg_lang_empty.c src/stack.c
mindfa_objects=$(mindfa_sources:.c=.o)

mindfa: $(mindfa_objects)
	$(CC) $(IFLAGS) $(mindfa_objects) -o $@ 

rlempty_src=src/reg_lang_empty.c src/is_reg_lang_empty.c src/common.c src/stack.c
rlempty_obj=$(rlempty_src:.c=.o)

rlempty: $(rlempty_obj) 
	$(CC) $(IFLAGS) $(rlempty_obj) -o $@

.c.o:
	$(CC) $(DFLAGS) $(CFLAGS) $(IFLAGS) $< -o $@

.PHONY: clean
clean :
	rm -rf ./src/*.o mindfa rlempty
