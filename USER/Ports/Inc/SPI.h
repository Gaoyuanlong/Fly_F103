#ifndef __SPI_H__
#define __SPI_H__
#include "stm32f10x.h"

#define SPI_RATE_SET 0X01

typedef struct SPI_
{
	void (*Init)(void);
	u8 (*Trans)(u8 Data);
	void (*Ctrl)(u8 Cmd,u16 Value);
}SPI_;
extern SPI_ Spi1,Spi2;


#endif
