#ifndef __TIME_H__
#define __TIME_H__
#include "stm32f10x.h"
#include "ucos_ii.h"
//禁止在主函数内使用



typedef struct Os_Time_
{
	void (*Init)(void);
	u32  (*Now_MS)(void);
	u32  (*Now_US)(void);
	void (*WaitMS)(u32 Time);
}Os_Time_;

extern Os_Time_ Os_Time;

#endif
