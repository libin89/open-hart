#include "soft_timer.h"

soft_timer g_tmr[TMR_CNT];

void soft_timer_init(void)
{
	unsigned char i;
	
	for(i = 0; i < TMR_CNT; i++)
	{
		g_tmr[i].cnt = 0;
		g_tmr[i].flg = 0;
	}
}

static void soft_timer_dec(soft_timer *tmr)
{
	if(tmr->flg == 0)
	{
		if(tmr->cnt > 0)
		{
			if(--tmr->cnt == 0)
			{
				tmr->flg = 1;
			}
		}
	}
}

void set_delay_time(unsigned char id,unsigned int cnt)
{
	if(id >= TMR_CNT)
	{
		return ;
	}
	g_tmr[id].cnt = cnt;
	if(cnt > 0)
	{
		g_tmr[id].flg = 0;
	}
	else
	{
		g_tmr[id].flg = 1;
	}
}

unsigned char is_timeout_id(unsigned char id)
{
	if(g_tmr[id].flg == 1)
	{
		g_tmr[id].flg = 0;
		return 1;
	}
	return 0;
}

void SysTick_ISR(void)
{
	unsigned char i;
	
	for(i = 0; i < TMR_CNT; i++)
	{
		soft_timer_dec(&g_tmr[i]);
	}
}






