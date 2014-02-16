#include "queue.h"
#include "common.h"
#include "data_types.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

long get_page_size()
{
	return sysconf(_SC_PAGESIZE);
}

Queue* create_queue()
{
	int page_size = get_page_size();
	int size = page_size/sizeof(Elem);
	Queue* queue = (Queue*) malloc(sizeof(Queue));
	queue->block = (Elem*) malloc(size*sizeof(Elem));
	queue->head = 0;
	queue->tail = 0;
	queue->size = size;
	return queue;
}

void queue_push(Queue* queue, int elem)
{
	int head = queue->head;
	int tail = queue->tail;
	int size = queue->size;
	if (tail == size) {
		if (head != 0) {
			memmove(queue->block, &queue->block[head], (tail - head) * sizeof(Elem)); 
			queue->tail = tail - head; queue->head = 0;
		}
		else {
			size *= 2;
			Elem* new_block = (Elem*) malloc(size*sizeof(Elem));
			memcpy(new_block, queue->block, queue->size*sizeof(Elem));
			free(queue->block);
			queue->block = new_block;
			queue->size = size;
		}
	}
	queue->block[tail].num = elem;
	queue->tail++;
}

int queue_pop(Queue* queue)
{
	assert_cond(queue->head != queue->tail, "Queue underflow");
	Elem* result = &queue->block[queue->head];
	queue->head++;
	return result->num;
}

int queue_empty(Queue* queue)
{
	return queue->head == queue->tail;
}

