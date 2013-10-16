#include "queue.h"
#include <stdlib.h>

unsigned char queue_init(common_queue_t *queue)
{
	queue->front = queue->rear = (queue_node_ptr)malloc(sizeof(queue_node));
	if(!queue->front)
	{
		return OVERFLOW;
	}
	queue->front->next = NULL;
	return OK;
}

unsigned char destroy_queue(common_queue_t *queue)
{
	queue_node_ptr tmp;
	
	while(queue->front)
	{
		tmp = queue->front->next;
		if(NULL == tmp)
		{
			queue->front = queue->rear;
			free(queue->front);
		}
		else
		{
			queue->front->next = tmp->next;
		}
		free(tmp);
	}
	return OK;
}

unsigned char enqueue(common_queue_t *queue, elem_type *data)
{
	queue_node_ptr tmp;
	
	tmp = (queue_node_ptr)malloc(sizeof(queue_node));
	if(!tmp)
	{
		return OVERFLOW;
	}
	tmp->data = *data;
	tmp->next = NULL;
	queue->rear->next = tmp;
	queue->rear = tmp;
	return OK;
}

unsigned char dequeue(common_queue_t *queue, elem_type *data)
{
	queue_node_ptr tmp;
	
	if(queue->front == queue->rear)
	{
		return ERROR;
	}
	tmp = queue->front->next;
	*data = tmp->data;
	queue->front->next = tmp->next;
	if(queue->rear == tmp)
	{
		queue->front = queue->rear;
	}
	free(tmp);
	return OK;
}



