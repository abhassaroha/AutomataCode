CC=gcc -std=c99 -g 
CFLAGS= -c -Wall
IFLAGS=-I ./src
DFLAGS=-DHOPCROFT_ALGO
#DFLAGS=

all: mindfa rlempty rlinf 

# mindfa
mindfa_src=src/minimize_dfa.c src/list.c src/double_list.c src/queue.c\
src/common.c src/is_reg_lang_empty.c src/stack.c
mindfa_obj=$(patsubst src/%, build/%, $(mindfa_src:.c=.o))
mindfa: $(mindfa_obj)
	$(CC) $? -o bin/$@ 
build/minimize_dfa.o: src/minimize_dfa.c mkdir
	$(CC) $(DFLAGS) $(CFLAGS) $(IFLAGS) $< -o $@

# rlempty
rlempty_src=src/reg_lang_empty.c src/is_reg_lang_empty.c src/common.c src/stack.c
rlempty_obj=$(patsubst src/%, build/%, $(rlempty_src:.c=.o))
rlempty: $(rlempty_obj) 
	$(CC) $? -o bin/$@

# rlinf
rlinf_src=src/reg_lang_infiniteness.c src/is_reg_lang_empty.c src/common.c src/stack.c
rlinf_obj=$(patsubst src/%, build/%, $(rlinf_src:.c=.o))
rlinf: $(rlinf_obj)
	$(CC) $? -o bin/$@ 

# generic
build/%.o: src/%.c mkdir
	$(CC) $(CFLAGS) $(IFLAGS) $< -o $@
mkdir: bin build
	mkdir -p $?

.PHONY: clean
clean :
	rm -f ./build/*.o bin/mindfa bin/rlempty
