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

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTB时钟使能 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );//SPI2时钟使能
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SPI1_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //SPI设置为双线双向全双工
	SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;		                    //设置为主SPI
	SPI1_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                //SPI发送接收8位帧结构
	SPI1_InitStructure.SPI_CPOL = SPI_CPOL_High;		                      //时钟悬空高
	SPI1_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                        //数据捕获于第二个时钟沿
	SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;		                        //NSS软件管理:内部NSS信号有SSI位控制
	SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //波特率预分频值为256
	SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //数据传输从MSB位开始
	SPI1_InitStructure.SPI_CRCPolynomial = 7;	                          //CRC值计算的多项式
	SPI_Init(SPI1, &SPI1_InitStructure);                                

	SPI_Cmd(SPI1, ENABLE); 


}

u8 SPI1_Translate(u8 Data)
{
	while((SPI1->SR&SPI_I2S_FLAG_TXE) == RESET);		//等待发送区空	  
	SPI1->DR = Data;	 	                            //发送一个byte   
	while((SPI1->SR&SPI_I2S_FLAG_RXNE) == RESET);   //等待接收完一个byte  
	return SPI1->DR;          	            
}

void SPI1_Ctrl(u8 Cmd,u16 Value)
{
	SPI_InitTypeDef  SPI1_InitStructure;
	
	SPI1_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //SPI设置为双线双向全双工
	SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;		                    //设置为主SPI
	SPI1_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                //SPI发送接收8位帧结构
	SPI1_InitStructure.SPI_CPOL = SPI_CPOL_High;		                      //时钟悬空高
	SPI1_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                        //数据捕获于第二个时钟沿
	SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;		                        //NSS软件管理:内部NSS信号有SSI位控制
	SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //波特率预分频值为256
	SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //数据传输从MSB位开始
	SPI1_InitStructure.SPI_CRCPolynomial = 7;	                          //CRC值计算的多项式
	
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

