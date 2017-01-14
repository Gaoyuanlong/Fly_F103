#include "SPI.h"

void SPI2_Init(void);
u8 SPI2_Translate(u8 Data);
void SPI2_Ctrl(u8 Cmd,u16 Value);



struct SPI_ Spi2 = 
{
	SPI2_Init,
	SPI2_Translate,
	SPI2_Ctrl
};

void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI2_InitStructure;	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ�� 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2ʱ��ʹ�� 	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SPI2_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI����Ϊ˫��˫��ȫ˫��
	SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;		                    //����Ϊ��SPI
	SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                //SPI���ͽ���8λ֡�ṹ
	SPI2_InitStructure.SPI_CPOL = SPI_CPOL_High;		                      //ʱ�����ո�
	SPI2_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                      //���ݲ����ڵڶ���ʱ����
	SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;		                        //NSS�������:�ڲ�NSS�ź���SSIλ����
	SPI2_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_8;		//������Ԥ��ƵֵΪ256
	SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //���ݴ����MSBλ��ʼ
	SPI2_InitStructure.SPI_CRCPolynomial = 7;	                          //CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI2_InitStructure);                                

	SPI_Cmd(SPI2, ENABLE); 


}

u8 SPI2_Translate(u8 Data)
{
	while((SPI2->SR&SPI_I2S_FLAG_TXE) == RESET);		//�ȴ���������	  
	SPI2->DR = Data;	 	                            //����һ��byte   
	while((SPI2->SR&SPI_I2S_FLAG_RXNE) == RESET);   //�ȴ�������һ��byte  
	return SPI2->DR;          	            
}


void SPI2_Ctrl(u8 Cmd,u16 Value)
{ 
	SPI_InitTypeDef  SPI2_InitStructure;		
	
	SPI2_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI����Ϊ˫��˫��ȫ˫��
	SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;		                    //����Ϊ��SPI
	SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                //SPI���ͽ���8λ֡�ṹ
	SPI2_InitStructure.SPI_CPOL = SPI_CPOL_High;		                      //ʱ�����ո�
	SPI2_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                      //���ݲ����ڵڶ���ʱ����
	SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;		                        //NSS�������:�ڲ�NSS�ź���SSIλ����
	SPI2_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_8;		//������Ԥ��ƵֵΪ256
	SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //���ݴ����MSBλ��ʼ
	SPI2_InitStructure.SPI_CRCPolynomial = 7;	                          //CRCֵ����Ķ���ʽ
	
	switch (Cmd)
	{
		case SPI_RATE_SET:
			SPI2_InitStructure.SPI_BaudRatePrescaler = Value;
			break;
		default:
			break;
	} 
	SPI_Init(SPI2, &SPI2_InitStructure);   
}
