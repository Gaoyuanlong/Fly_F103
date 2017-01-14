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
	DMA_DeInit(DMA1_Channel1);																									//��ʼ��DMA1ͨ��1ΪĬ��ֵ
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);														//����DMAʱ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = ((u32)0x4001244c);								//DMA����Ĵ�������ʼ��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&DataBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;													//DMA���䷽��,����Ϊ�Ĵ������䵽����
	DMA_InitStructure.DMA_BufferSize = CHANNEL;																	//DMA��������С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;						//DMA����Ĵ�����ַ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;											//DMA�����ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//��������Ĵ�������Ϊ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;		
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;															//DMA����Ϊѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;													//�������ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;																//����Ϊ�������
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);				
	DMA_Cmd(DMA1_Channel1, ENABLE);																							//����DMA1ͨ��1
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, DISABLE);														//ʧ��DMA1����1��������ж�Դ
	
	ADC_DeInit(ADC1);		

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE );	    //ʹ��ADC1ͨ��ʱ��
 	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  																							//72M/6=12,ADC���ʱ�䲻�ܳ���14M
																																									 //ģ��ͨ���������� PA0-4                       
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                                   
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	ADC_InitStucture.ADC_Mode = ADC_Mode_Independent;																	//�����ڶ���ģʽ
	ADC_InitStucture.ADC_ScanConvMode = ENABLE;		                                    //ɨ��ģʽ
	ADC_InitStucture.ADC_ContinuousConvMode = DISABLE;	                              //����ת��
	ADC_InitStucture.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		            //�������
	ADC_InitStucture.ADC_DataAlign = ADC_DataAlign_Right;															//ADC�����Ҷ���
	ADC_InitStucture.ADC_NbrOfChannel = CHANNEL;																			//�涨˳����й���ת����ADCͨ����Ŀ
	ADC_Init(ADC1,&ADC_InitStucture);
 
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_7Cycles5);			       //����˳��				
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_7Cycles5);			       			
	ADC_RegularChannelConfig(ADC1,ADC_Channel_12,3,ADC_SampleTime_7Cycles5);		              		
	ADC_RegularChannelConfig(ADC1,ADC_Channel_13,4,ADC_SampleTime_7Cycles5); 		     
	
	ADC_TempSensorVrefintCmd(ENABLE);																								//ʹ���ڲ��¶ȴ������Ĳο���ѹ
	ADC_DMACmd(ADC1,ENABLE);                                                        //ADC1 DMA����
	ADC_Cmd(ADC1,ENABLE);                                                           //ADC1 ����
	ADC_ResetCalibration(ADC1);																											//����ADCУ׼�Ĵ���
	while (ADC_GetResetCalibrationStatus(ADC1));	
	ADC_StartCalibration(ADC1);																											//��ʼADCУ׼
	while (ADC_GetCalibrationStatus(ADC1));																					//�ȴ�ADCУ׼�Ĵ������
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);																					//�������ADCת��


}

void Get_ADCValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	ADC.Data.CH1 = DataBuf[0] * 2.0f * 3.3f / 4095;
	ADC.Data.CH2 = DataBuf[1] * 2.0f * 3.3f / 4095;
	ADC.Data.CH3 = DataBuf[2] * 2.0f * 3.3f / 4095;
	ADC.Data.CH4 = DataBuf[3] * 2.0f * 3.3f / 4095;
}
