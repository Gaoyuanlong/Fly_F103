#include "SPI.h"

void SPI1_Init(void);
u8 SPI1_Translate(u8 Data);
void SPI1_Ctrl(u8 Cmd,u16 Value);



struct SPI_ Spi1 = 
{
	SPI1_Init,
	SPI1_Translate,
	SPI1_Ctrl
};

void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI1_InitStructure;	

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTBʱ��ʹ�� 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );//SPI2ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SPI1_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //SPI����Ϊ˫��˫��ȫ˫��
	SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;		                    //����Ϊ��SPI
	SPI1_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                //SPI���ͽ���8λ֡�ṹ
	SPI1_InitStructure.SPI_CPOL = SPI_CPOL_High;		                      //ʱ�����ո�
	SPI1_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                        //���ݲ����ڵڶ���ʱ����
	SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;		                        //NSS�������:�ڲ�NSS�ź���SSIλ����
	SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //������Ԥ��ƵֵΪ256
	SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //���ݴ����MSBλ��ʼ
	SPI1_InitStructure.SPI_CRCPolynomial = 7;	                          //CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI1_InitStructure);                                

	SPI_Cmd(SPI1, ENABLE); 


}

u8 SPI1_Translate(u8 Data)
{
	while((SPI1->SR&SPI_I2S_FLAG_TXE) == RESET);		//�ȴ���������	  
	SPI1->DR = Data;	 	                            //����һ��byte   
	while((SPI1->SR&SPI_I2S_FLAG_RXNE) == RESET);   //�ȴ�������һ��byte  
	return SPI1->DR;          	            
}

void SPI1_Ctrl(u8 Cmd,u16 Value)
{
	SPI_InitTypeDef  SPI1_InitStructure;
	
	SPI1_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //SPI����Ϊ˫��˫��ȫ˫��
	SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;		                    //����Ϊ��SPI
	SPI1_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                //SPI���ͽ���8λ֡�ṹ
	SPI1_InitStructure.SPI_CPOL = SPI_CPOL_High;		                      //ʱ�����ո�
	SPI1_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                        //���ݲ����ڵڶ���ʱ����
	SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;		                        //NSS�������:�ڲ�NSS�ź���SSIλ����
	SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //������Ԥ��ƵֵΪ256
	SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //���ݴ����MSBλ��ʼ
	SPI1_InitStructure.SPI_CRCPolynomial = 7;	                          //CRCֵ����Ķ���ʽ
	
	switch (Cmd)
	{
		case SPI_RATE_SET:
			SPI1_InitStructure.SPI_BaudRatePrescaler = Value;
			break;
		default:
			break;
	}        
	
	SPI_Init(SPI1, &SPI1_InitStructure); 
}

