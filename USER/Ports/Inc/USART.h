#ifndef __USART_H__
#define __USART_H__
#include "stm32f10x.h"
#include "Time.h"
#include "Common.h"
#include <stdio.h>
#include "Queue.h"

#define USART_BUF_SIZE 512
#define TX_BUF_NO_FULL  1
#define RX_BUF_NO_EMPTY 2

typedef struct USART_
{
	void (*Init)(u32 Baud,u8 Priority);
	u8 (*Send)(u8* BUF,u8 Cnt);
	u8 (*Recv)(u8* BUF,u8 Offset,u8 Cnt,BOOL IsWait);
	void (*GetRight)(void);
	void (*FreeRight)(void);
}USART_;
extern USART_ Usart1,Usart2,Usart3;

#endif
