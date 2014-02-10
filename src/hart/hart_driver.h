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

/* serial declaration */
unsigned char serial_init(unsigned int baud, 
		unsigned char data_bit, 
		parity_type parity, 
		unsigned char stop_bit);
void serial_enable(unsigned char rx_enable, unsigned char tx_enable);
unsigned char serial_put_byte(unsigned char byte);
unsigned char serial_get_byte(unsigned char *byte);
/* virtual function interface */
extern void (*_SerialReceiveMsg)(void);
extern void (*_SerialSendMsg)(void);

/* timer declaration */
unsigned char timer_init(unsigned int time_100us);
void timer_enable(unsigned int time_limit);
void timer_disable(void);



/* critical function declaration */
void enter_critical_section(void);
void exit_critical_section(void);

#endif
