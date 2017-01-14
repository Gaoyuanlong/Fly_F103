#include "LED.h"

void LED_Init(LED_* Aim);
void LED_On(LED_* Aim);
void LED_Off(LED_* Aim);
void LED_Reverse(LED_* Aim);

LED_ LED1 = {GPIO_Pin_2,GPIOB};

LED_OPS_ LED_OPS = 
{
	LED_Init,
	LED_On,
	LED_Off,
	LED_Reverse
};

void LED_Init(LED_* Aim)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = Aim->PORTX;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(Aim->GPIOX,&GPIO_InitStructure);

	GPIO_ResetBits(Aim->GPIOX,Aim->PORTX);
}
void LED_On(LED_* Aim)
{
	GPIO_ResetBits(Aim->GPIOX,Aim->PORTX);
}

void LED_Off(LED_* Aim)
{
	GPIO_SetBits(Aim->GPIOX,Aim->PORTX);
}

void LED_Reverse(LED_* Aim)
{
	Aim->GPIOX->ODR ^= Aim->PORTX;
}
