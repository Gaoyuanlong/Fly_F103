#include "main.h"
#include<stdlib.h>


__align(8) static OS_STK Task_Init_stk[TASK_INIT_STK_SIZE];
__align(8) static OS_STK Task_Gather_stk[TASK_GATHER_STK_SIZE];
__align(8) static OS_STK Task_Show_stk[TASK_SHOW_STK_SIZE];
void Task_Init(void *p_arg);

int main(void)
{
	OSInit();
	OSTaskCreate(Task_Init,(void *)0,&Task_Init_stk[TASK_INIT_STK_SIZE - 1],TASK_INIT_PRIO);	 
	OSStart();
	while(1);
}

void Task_Init(void *p_arg)
{
	(void)p_arg;
	
	BSP.Init();
	OSStatInit();	
	TFT.Init(300);
	LED_OPS.Init(&LED1);
	SD_Init();
	Para.Init();

	OSTaskCreate(Task_Gather,(void *)0,&Task_Gather_stk[TASK_GATHER_STK_SIZE - 1],TASK_GATHER_PRIO);	
	OSTaskCreate(Task_Show,(void *)0,&Task_Show_stk[TASK_SHOW_STK_SIZE - 1],TASK_SHOW_PRIO);		

	while(1)
	{		
		OSTimeDly(1000);
	}
}



