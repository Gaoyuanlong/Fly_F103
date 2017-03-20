#include "BSP.h"
void BSP_Init(void);
struct BSP_ BSP =
{
	BSP_Init
};
void BSP_Init(void)
{
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); 		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	
	Os_Time.Init();
	Usart1.Init(115200,3);
	Usart2.Init(115200,2);
	Usart3.Init(38400,1);
	ADC.Init(); 
}

