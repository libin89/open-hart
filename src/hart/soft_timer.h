#ifndef _SOFT_TIMER_H
#define _SOFT_TIMER_H

#define TMR_CNT     3

typedef struct soft_timer
{
	unsigned int cnt;
	unsigned char flg;
}soft_timer;

#if 0
typedef enum 
{
	HRT_GAP_TIMEOUT,     /* inter-character gap time */
	HRT_SLAVE_TIMEOUT,  /* the max amount of time permitted for a field device to respond to an incoming message */
	HRT_BT,
}time_type;
#endif

extern void soft_timer_init(void);
extern void set_delay_time(unsigned char id,unsigned int cnt);
extern unsigned char is_timeout_id(unsigned char id);

#endif
