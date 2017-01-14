#ifndef __LED_H__
#define __LED_H__
#include "BSP.h"

typedef struct LED_
{
	uint16_t PORTX;	
	GPIO_TypeDef* GPIOX;	
}LED_;

typedef struct LED_OPS_
{
	void (*Init)(LED_* Aim);
	void (*On)(LED_* Aim);
	void (*Off)(LED_* Aim);
	void (*Reverse)(LED_* Aim);

}LED_OPS_;

extern LED_OPS_ LED_OPS;
extern LED_ LED1;

#endif
