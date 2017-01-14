#include "ADC.h"

#define CHANNEL 4

void Adc_Init(void);
void Get_ADCValue(void);

u16 DataBuf[CHANNEL];

struct ADC_ ADC = 
{
	Adc_Init,
	Get_ADCValue
};

void Adc_Init(void)
{
	ADC_InitTypeDef	ADC_InitStucture;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	DMA_InitTypeDef	DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);																									//初始化DMA1通道1为默认值
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);														//启动DMA时钟
	DMA_InitStructure.DMA_PeripheralBaseAddr = ((u32)0x4001244c);								//DMA外设寄存器地起始地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&DataBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;													//DMA传输方向,定议为寄存器传输到缓存
	DMA_InitStructure.DMA_BufferSize = CHANNEL;																	//DMA缓存区大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;						//DMA外设寄存器地址不递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;											//DMA缓存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//设置外设寄存器数据为
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;		
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;															//DMA设置为循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;													//设置优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;																//设置为单向访问
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);				
	DMA_Cmd(DMA1_Channel1, ENABLE);																							//启动DMA1通道1
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, DISABLE);														//失能DMA1道道1传送完毕中断源
	
	ADC_DeInit(ADC1);		

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE );	    //使能ADC1通道时钟
 	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  																							//72M/6=12,ADC最大时间不能超过14M
																																									 //模拟通道输入引脚 PA0-4                       
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                                   
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	ADC_InitStucture.ADC_Mode = ADC_Mode_Independent;																	//工作于独立模式
	ADC_InitStucture.ADC_ScanConvMode = ENABLE;		                                    //扫描模式
	ADC_InitStucture.ADC_ContinuousConvMode = DISABLE;	                              //连续转换
	ADC_InitStucture.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		            //软件触发
	ADC_InitStucture.ADC_DataAlign = ADC_DataAlign_Right;															//ADC数据右对齐
	ADC_InitStucture.ADC_NbrOfChannel = CHANNEL;																			//规定顺序进行规则转换的ADC通道数目
	ADC_Init(ADC1,&ADC_InitStucture);
 
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_7Cycles5);			       //采样顺序				
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_7Cycles5);			       			
	ADC_RegularChannelConfig(ADC1,ADC_Channel_12,3,ADC_SampleTime_7Cycles5);		              		
	ADC_RegularChannelConfig(ADC1,ADC_Channel_13,4,ADC_SampleTime_7Cycles5); 		     
	
	ADC_TempSensorVrefintCmd(ENABLE);																								//使能内部温度传感器的参考电压
	ADC_DMACmd(ADC1,ENABLE);                                                        //ADC1 DMA启动
	ADC_Cmd(ADC1,ENABLE);                                                           //ADC1 启动
	ADC_ResetCalibration(ADC1);																											//重置ADC校准寄存器
	while (ADC_GetResetCalibrationStatus(ADC1));	
	ADC_StartCalibration(ADC1);																											//开始ADC校准
	while (ADC_GetCalibrationStatus(ADC1));																					//等待ADC校准寄存器完毕
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);																					//软件启动ADC转换


}

void Get_ADCValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	ADC.Data.CH1 = DataBuf[0] * 2.0f * 3.3f / 4095;
	ADC.Data.CH2 = DataBuf[1] * 2.0f * 3.3f / 4095;
	ADC.Data.CH3 = DataBuf[2] * 2.0f * 3.3f / 4095;
	ADC.Data.CH4 = DataBuf[3] * 2.0f * 3.3f / 4095;
}
