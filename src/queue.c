#include "queue.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "data_types.h"

Queue* create_queue()
{
	int size = get_page_size()/sizeof(Elem);
	Queue* queue = (Queue*) malloc(sizeof(Queue));
	queue->block = (Elem*) malloc(size*sizeof(Elem));
	queue->head = 0;
	queue->tail = 0;
	queue->size = size;
	return queue;
}

void destroy_queue(Queue *queue)
{
	free(queue->block);
	free(queue);
}

/**
 * Push the element to the queue.
 * The queue does not overflow, but resizes when
 * it has reached full capacity. When there is space
 * at the beginning of memory block, data is shifted to fill
 * in earlier bytes, otherwise underlying capacity is doubled.
 */
void queue_push(Queue* queue, int elem)
{
	if (queue->tail == queue->size) {
		int old_size = queue->tail - queue->head;
		if (queue->head != 0) {
			memmove(queue->block, queue->block + queue->head, old_size * sizeof(Elem)); 
			queue->head = 0; queue->tail = old_size;
		}
		else {
			queue->size *= 2;
			Elem* old_block = queue->block;
			queue->block = (Elem*) malloc(queue->size * sizeof(Elem));
			memcpy(queue->block, old_block, old_size * sizeof(Elem));
			free(old_block);
		}
	}
	queue->block[queue->tail++].num = elem;
}

/**
 * Pop the element from queue.
 * The queue will underflow if empty queue is popped.
 */
int queue_pop(Queue* queue)
{
	assert_cond(queue->head != queue->tail, "Queue underflow");
	return queue->block[queue->head++].num;
}

int queue_empty(Queue* queue)
{
	return queue->head == queue->tail;
}
