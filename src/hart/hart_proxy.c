#include "hart_proxy.h"

common_queue_t txb;
common_queue_t rxb;

unsigned char hart_proxy_create(void)
{
	queue_init(&txb);
	queue_init(&rxb);
	return TRUE;
}

void hart_proxy_dequeue(void)
{
	unsigned char put_byte;
	
	dequeue(&txb,&put_byte);
	serical_put_byte(&put_byte);
}

void hart_proxy_enqueue(void)
{
	unsigned char get_byte;
	
	serical_get_byte(&get_byte);
	enqueue(&rxb,&get_byte);
}

unsigned char hart_proxy_send( unsigned char *data,
				unsigned char (*data_proc_complete_notify)(void) )
{
	if(data_proc_complete_notify())
	{
		dequeue(&rxb,data);
	}
	else
	{
		data = NULL;
	}
	return TRUE;
}

unsigned char hart_proxy_receive(unsigned char *data,
				unsigned int cnt,
				unsigned char (*data_request_send_notify)(void))
{
	unsigned char i;
	
	if(data_request_send_notify())
	{
		for(i = 0; i < cnt; i++)
		{
			enqueue(&txb,data+i);
		}		
	}
	else
	{
		data = NULL;
	}
	return TRUE;
}
				
