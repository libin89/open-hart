#ifndef _HART_PROXY_H
#define _HART_PROXY_H

#include "hart_driver.h"
#include "queue.h"

#ifndef NULL
#define NULL 0
#endif

struct hart_proxy {
	struct hart_driver *driver;
	void *link;
	void *tx_appli;
	void *rx_appli;
	unsigned char (*tx_request_notify)(void *tx_appli);	
	unsigned char (*rx_proc_complete_notify)(void *rx_appli);
	unsigned char (*hart_proxy_send_end)(void *link,
		unsigned char *data);
	unsigned char (*hart_proxy_data_received)(void *link,
		unsigned char *data,
		unsigned int cnt);
	common_queue_t *rxb;    //receive buffer
	common_queue_t *txb;    //transmit buffer
};

extern unsigned char hart_proxy_send_rq(struct hart_proxy *proxy,
	unsigned char *data,
	unsigned int cnt);

extern unsigned char hart_proxy_rsp_proc_end(struct hart_proxy *proxy);

#endif
