#ifndef __COMMUNICATE_H__ 
#define __COMMUNICATE_H__
#include "Common.h"
#include "RTK.h"
#include "Queue.h"
#include "USART.h"

typedef struct Communicate_
{
	void (*UpData)(void);

}Communicate_;

extern Communicate_ Communicate;
extern RTK_GPS_ GPS_Send_Data;
#endif


