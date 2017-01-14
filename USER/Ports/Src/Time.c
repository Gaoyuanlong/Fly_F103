#include "Time.h"

u32 usTicks;                    // 每US的Tick数

void Time_Init(void);
u32 TimeNow_US(void);
void Time_WaitMS(u32 Time);

//函数指针初始化
Os_Time_ Os_Time = 
{
	Time_Init,
	OSTimeGet,
	TimeNow_US,
	Time_WaitMS
};

/*
*********************************************************************************************************
*                                          OS_CPU_SysTickInit()
*
* Description: Initialize the SysTick.
*
* Arguments  : none.
*
* Note(s)    : 1) This function MUST be called after OSStart() & after processor initialization.
						 : 2) See it in Time.c
*********************************************************************************************************
*/	
void Time_Init(void)
{
	RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks);
	usTicks = clocks.SYSCLK_Frequency / 1000000;
	SysTick_Config(clocks.SYSCLK_Frequency / 1000);	
	
}


u32 TimeNow_US(void) 
{
	u32 Systick;
	u32 Time1,Time2;
	
	Time1 = OSTimeGet();
	Systick = SysTick->VAL;
	Time2 = OSTimeGet();
	
	if(Time1 != Time2) Systick = SysTick->VAL;
	
	return Time2 * 1000 - Systick / usTicks + 1000;
}

void Time_WaitMS(u32 Time)
{
	u32 Time_Now;

	Time_Now = OSTimeGet();

	while(OSTimeGet() < Time_Now + Time);
}

/*
*********************************************************************************************************
*                                         OS_CPU_SysTickHandler()
*
* Description: Handle the system tick (SysTick) interrupt, which is used to generate the uC/OS-II tick
*              interrupt.
*
* Arguments  : none.
*
* Note(s)    : 1) This function MUST be placed on entry 15 of the Cortex-M3 vector table.
							 2) See it in Time.c
*********************************************************************************************************
*/

void  SysTick_Handler (void)
{
	 OS_CPU_SR  cpu_sr;
	
	 OS_ENTER_CRITICAL();                          /* Tell uC/OS-II that we are starting an ISR          */
	 OSIntNesting++;
	 OS_EXIT_CRITICAL();

	 OSTimeTick();                                /* Call uC/OS-II's OSTimeTick()                       */

	 OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
}
