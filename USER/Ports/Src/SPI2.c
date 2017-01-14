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
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SPI2_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为双线双向全双工
	SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;		                    //设置为主SPI
	SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                //SPI发送接收8位帧结构
	SPI2_InitStructure.SPI_CPOL = SPI_CPOL_High;		                      //时钟悬空高
	SPI2_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                      //数据捕获于第二个时钟沿
	SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;		                        //NSS软件管理:内部NSS信号有SSI位控制
	SPI2_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_8;		//波特率预分频值为256
	SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //数据传输从MSB位开始
	SPI2_InitStructure.SPI_CRCPolynomial = 7;	                          //CRC值计算的多项式
	SPI_Init(SPI2, &SPI2_InitStructure);                                

	SPI_Cmd(SPI2, ENABLE); 


}

u8 SPI2_Translate(u8 Data)
{
	while((SPI2->SR&SPI_I2S_FLAG_TXE) == RESET);		//等待发送区空	  
	SPI2->DR = Data;	 	                            //发送一个byte   
	while((SPI2->SR&SPI_I2S_FLAG_RXNE) == RESET);   //等待接收完一个byte  
	return SPI2->DR;          	            
}


void SPI2_Ctrl(u8 Cmd,u16 Value)
{ 
	SPI_InitTypeDef  SPI2_InitStructure;		
	
	SPI2_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为双线双向全双工
	SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;		                    //设置为主SPI
	SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                //SPI发送接收8位帧结构
	SPI2_InitStructure.SPI_CPOL = SPI_CPOL_High;		                      //时钟悬空高
	SPI2_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                      //数据捕获于第二个时钟沿
	SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;		                        //NSS软件管理:内部NSS信号有SSI位控制
	SPI2_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_8;		//波特率预分频值为256
	SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //数据传输从MSB位开始
	SPI2_InitStructure.SPI_CRCPolynomial = 7;	                          //CRC值计算的多项式
	
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
