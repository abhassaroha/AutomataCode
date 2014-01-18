#ifndef AUTOMATA_QUEUE_H
#define AUTOMATA_QUEUE_H

typedef struct __QueueElem {
	int num;
} QueueElem;

typedef struct __Queue {
	int head;
	int tail;
	int size;
	QueueElem* block;
} Queue;

Queue* create_queue();
void queue_push(Queue* queue, int elem);
int queue_pop(Queue* queue);
int queue_empty(Queue* queue);
#endif
