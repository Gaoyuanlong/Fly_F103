#include "APP.h"

void Task_Gather(void *p_arg)
{
	(void)p_arg;

	while(1)
	{	
		if(RTK_OPS.Read(0) == True)
		{
			LED_OPS.Reverse(&LED1);
			Communicate.UpData();
		}
		Para.Updata();
		Para.Save();
		OSTimeDly(100);  
	}
}


