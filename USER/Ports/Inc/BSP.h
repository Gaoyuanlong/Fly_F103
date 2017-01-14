#ifndef __BSP_H__
#define __BSP_H__

#include "ADC.h"
#include "Time.h"
#include "USART.h"
#include "SPI.h"
#include "Common.h"
#include "RTK.h"

extern struct BSP_
{
	void (*Init)(void);

}BSP;
extern void BSP_UI(void);

#endif


