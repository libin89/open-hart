#ifndef _QUEUE_H
#define _QUEUE_H

#define OK       1
#define ERROR    0
#define OVERFLOW 2

typedef unsigned char elem_type;

typedef struct queue_node{
		elem_type data;
		struct queue_node *next;
}queue_node, *queue_node_ptr;

typedef struct {
	queue_node_ptr front;
	queue_node_ptr rear;
}common_queue_t;

extern unsigned char queue_init(common_queue_t *queue);
extern unsigned char enqueue(common_queue_t *queue, elem_type *data);
extern unsigned char dequeue(common_queue_t *queue, elem_type *data);
extern unsigned char destroy_queue(common_queue_t *queue);



#endif

