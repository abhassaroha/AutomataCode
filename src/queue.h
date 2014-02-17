#ifndef AUTOMATA_QUEUE_H
#define AUTOMATA_QUEUE_H

#include "data_types.h"

typedef struct __Queue {
	int head;
	int tail;
	int size;
	Elem* block;
} Queue;

Queue* create_queue();
void destroy_queue(Queue *queue);

void queue_push(Queue* queue, int elem);
int queue_pop(Queue* queue);
int queue_empty(Queue* queue);
#endif
