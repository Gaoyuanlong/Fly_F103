#ifndef __BATTERY_H__
#define __BATTERY_H__ 
#include "BSP.h"

typedef struct Battery_
{
	u8 ADC_Ch;
	float Volt;
}Battery_;


typedef struct Battery_OPS_
{
	void (*Init)(Battery_* Aim);
	void (*Read)(Battery_* Aim);
}Battery_OPS_;

extern Battery_OPS_ Battery_OPS;

#endif
