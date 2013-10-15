#include <stdio.h>

int main(void)
{
	hart_appli_init();
	while(1)
	{
		hart_appli_poll();
	}
	return 0;
}
