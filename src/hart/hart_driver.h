#ifndef _HART_DRIVER_H
#define _HART_DRIVER_H

#include "stm32f10x.h"

#define TRUE  1
#define FALSE 0

#define SYSCLK_FREQUENCY (72000000)
#define TICKS_SECOND     (10000)

typedef enum
{
	HT_SERICAL_NONE,
	HT_SERICAL_EVEN,
	HT_SERICAL_ODD,
}parity_type;

/* serical declaration */
extern unsigned char serical_init(unsigned int baud, 
		unsigned char data_bit, 
		parity_type parity, 
		unsigned char stop_bit);
extern void serical_enable(unsigned char rx_enable, unsigned char tx_enable);
extern unsigned char serical_put_byte(unsigned char byte);
extern unsigned char serical_get_byte(unsigned char *byte);

/* timer declaration */
extern unsigned char timer_init(unsigned int time_100us);
extern void timer_enable(unsigned int time_limit);
extern void timer_disable(void);



/* critical function declaration */
extern void enter_critical_section(void);
extern void exit_critical_section(void);

#endif
