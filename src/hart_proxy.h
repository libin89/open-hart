#ifndef _HART_PROXY_H
#define _HART_PROXY_H

#include "hart_driver.h"

struct hart_proxy {
	struct hart_driver *driver;
	void *link;
	//void *tx_appli;
	//void *rx_appli;
	//unsigned char (*hart_process_completed_notify)(void *rx_appli);
	//unsigned char (*hart_send_request_notify)(void * tx_appli);
	unsigned char (*hart_proxy_send_end)(void *link,
		unsigned char *data);
	unsigned char (*hart_proxy_data_recived)(void *link,
		unsigned char *data,
		unsigned int cnt);
};

extern unsigned char hart_proxy_send_rq(struct hart_proxy *proxy,
	unsigned char *data,
	unsigned int cnt);

extern unsigned char hart_proxy_rsp_proc_end(struct hart_proxy *proxy);

#endif
