#ifndef __QUEUE_H__
#define __QUEUE_H__
#include "stm32f10x.h"
#include "Common.h"
typedef struct Queue_{
  u8* BUF;      
  u16  Size;    
  u16  Head;           
  u16  End;     
	u16  Length;
  BOOL IsFull;     
  BOOL IsEmpty;  
}Queue_;

typedef struct Queue_OPS_
{
	Queue_ (*Init)(u16 Size,u8* P);
	BOOL (*Enqueue)(u8  byte,Queue_* Aim);
	BOOL (*Dequeue)(u8* byte,Queue_* Aim);

}Queue_OPS_;

extern Queue_OPS_ Queue_OPS;

#endif
