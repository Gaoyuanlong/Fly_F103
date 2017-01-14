#ifndef __ADC_H__
#define __ADC_H__
#include "stm32f10x.h"
#include "common.h"
struct ADC_Data_
{
	float CH1;
	float CH2;
	float CH3;
	float CH4;
};

extern struct ADC_ 
{
	void (*Init)(void);
	void (*Updata)(void);
	struct ADC_Data_ Data;
}ADC;

#endif
