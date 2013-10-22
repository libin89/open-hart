#include <stdio.h>
#include "hart_appli.h"
#include "soft_timer.h"
#include "stm32f10x.h"
#define SystemFrequency 72000000u
#define TicksPerSecond 1000u

int main(void)
{
	SystemInit();
	soft_timer_init();
	hart_appli_init();
	SysTick_Config(SystemFrequency / TicksPerSecond);
	
	while(1)
	{
		hart_poll();
	}
	return 0;
}
