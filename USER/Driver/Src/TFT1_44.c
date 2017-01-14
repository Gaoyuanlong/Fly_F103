#include "TFT1_44.h"
#include "ASCII_Code.h"

#define RS_0() GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define RS_1() GPIO_SetBits(GPIOA,GPIO_Pin_8)

#define CS_0() GPIO_ResetBits(GPIOA,GPIO_Pin_15)
#define CS_1() GPIO_SetBits(GPIOA,GPIO_Pin_15)

#define RST_0() GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define RST_1() GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define LED_0()  GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define LED_1()  GPIO_SetBits(GPIOB,GPIO_Pin_0)

void TFT_Init(u16 Level);
void TFT_Light(u16 Level);
void TFT_ShowString(Point POS,const char* P,u16 Color,u16 BackGroundColor);
void TFT_ShowNumber(Point POS,int Number,u16 Color,u16 BackGroundColor);
void Fill_Rectangle(Rectanle RECT,BOOL IsHollow,u16 Color);
void TFT_ShowFloat(Point POS,double Number,u16 Color,u16 BackGroundColor);
void TFT_Show_Hex(Point POS,u16 Number,u16 Color,u16 BackGroundColor);
void TFT_Clear_Pixel(Rectanle RECT);
void TFT_ShowChar(Point Pos,u8 Aim,u16 Color,u16 BackGroundColor);
struct TFT_ TFT = 
{
	TFT_Init,
	TFT_Light,
	TFT_ShowString,
	TFT_ShowNumber,
	Fill_Rectangle,
	TFT_ShowFloat,
	TFT_Show_Hex,
	TFT_Clear_Pixel,
	TFT_ShowChar
};

void TFT_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTA时钟使能 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTA时钟使能 


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 ;	  //Pin_13 sck  pin15 MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_15);//	CS

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);//	RS

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);//	RST

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

void TFT_SPI_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );//SPI2时钟使能 		
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工  SPI_Direction_1Line_Tx
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
}

void TFT_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 999; 																							 //设置自动重装载寄存器周期
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) (SystemCoreClock / (2000000)) - 1; //设置预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                													 //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  													 //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);          
	
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    																 //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 0;	
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	//使能TIM3重载寄存器ARR
  TIM_ARRPreloadConfig(TIM3, ENABLE);																										
	//使能定时器3
  TIM_Cmd(TIM3, ENABLE);
	
	TIM_SetCompare3(TIM3, 0);
}

u8 SPI_WriteByte(u8 Byte)
{
	while((SPI1->SR&SPI_I2S_FLAG_TXE)==RESET);		//等待发送区空	  
	SPI1->DR=Byte;	 	//发送一个byte   
	//while((SPI1->SR&SPI_I2S_FLAG_RXNE)==RESET);//等待接收完一个byte  
	return SPI1->DR;          	     //返回收到的数据			
} 
void TFT_Write_Command(u8 Data)
{
	RS_0();
	SPI_WriteByte(Data);
}

void  TFT_Write_Data(u8 Data)  
{
	RS_1();
	SPI_WriteByte(Data);
}

void TFT_Light(u16 Level)
{
	if(Level > 999) Level = 999;
	if(Level < 1) Level = 0;
	TIM_SetCompare3(TIM3, Level);
}

void TFT_Single_Colour(u8 TFT_DataH,u8 TFT_DataL)
{
	u8 i,j;
	
	for (i=0;i<160;i++)
		for (j=0;j<128;j++)
		{
			TFT_Write_Data(TFT_DataH);
			TFT_Write_Data(TFT_DataL);
		}
}

void TFT_Init(u16 Level)
{
	TFT_GPIO_Init();
	TFT_SPI_Init();
	TFT_PWM_Init();
	RST_0();
	Os_Time.WaitMS(1);
	RST_1();
	Os_Time.WaitMS(1);
	
	
	TFT_Write_Command(0x11); //Exit Sleep
	Os_Time.WaitMS(200);	
	TFT_Write_Command(0x3a); //Set Color Format
	TFT_Write_Data(0x05);
	TFT_Write_Command(0x36); //RGB
	TFT_Write_Data(0xa8);
	TFT_Write_Command(0x29); // Display On
	
	TFT_Write_Command(0x2A); //Set Column Address
	TFT_Write_Data(0x00);
	TFT_Write_Data(0x00);
	TFT_Write_Data(0x00);
	TFT_Write_Data(0x9F);
	TFT_Write_Command(0x2B); //Set Page Address
	TFT_Write_Data(0x00);
	TFT_Write_Data(0x00);
	TFT_Write_Data(0x00);
	TFT_Write_Data(0x7F);
	
	TFT_Write_Command(0x2C);
	TFT_Single_Colour(0,0);//黑色背景
	TFT_Light(Level);
}


void Address_Set_Word(Point POS)
{
	TFT_Write_Command(0x2a);
	TFT_Write_Data(0);
	TFT_Write_Data((u8)(POS.X * 8) + 32);
	TFT_Write_Data(0);
	TFT_Write_Data((u8)(POS.X * 8) + 32 + 7);

	TFT_Write_Command(0x2b);
	TFT_Write_Data(0);
	TFT_Write_Data((u8)(POS.Y * 16));
	TFT_Write_Data(0);
	TFT_Write_Data((u8)(POS.Y * 16) + 15);

	TFT_Write_Command(0x2C);
}	

void Address_Set_Pixel(Rectanle RECT)
{
	TFT_Write_Command(0x2a);
	TFT_Write_Data(0);
	TFT_Write_Data(RECT.X_LU + 32);
	TFT_Write_Data(0);
	TFT_Write_Data(RECT.X_RD + 32);

	TFT_Write_Command(0x2b);
	TFT_Write_Data(0);
	TFT_Write_Data(RECT.Y_LU);
	TFT_Write_Data(0);
	TFT_Write_Data(RECT.Y_RD);

	TFT_Write_Command(0x2C);
}	

void TFT_ShowChar(Point Pos,u8 Aim,u16 Color,u16 BackGroundColor)
{  							  
	u8 Temp1,Temp2,Temp3;

	Aim = Aim - ' ';                        //得到偏移后的值
	Address_Set_Word(Pos);                      //设置显示更该区域

	for(Temp3 = 0;Temp3 < 16;Temp3 ++)
	{   
		Temp1 = Zk_ASCII8X16[Aim][Temp3];		  //调用1608字体 	                          
		for(Temp2 = 0;Temp2 < 8;Temp2 ++)
		{			    
			if(Temp1 & 0x01)
			{
				TFT_Write_Data((u8)Color);                  //白色字体
				TFT_Write_Data((u8)(Color >> 8));           //白色字体
			}
			else
			{			
				TFT_Write_Data((u8)BackGroundColor);	
				TFT_Write_Data((u8)(BackGroundColor >> 8));
			}
			Temp1 >>= 1;
		}  	 
	}    		
}  

void TFT_ShowString(Point POS,const char* P,u16 Color,u16 BackGroundColor)
{
	while((*P <= '~')&&(*P >= ' ')&&(*P != '\0'))        //判断是不是非法字符!
	{           
		TFT_ShowChar(POS,*P,Color,BackGroundColor);
		POS.X++;
		P++;
	}
}

void TFT_ShowNumber(Point POS,int Number,u16 Color,u16 BackGroundColor)
{
	char BUF[6] = {' ',' ',' ',' ',' ',' '};
	sprintf(BUF,"%d",Number);
	TFT_ShowString(POS,BUF,Color,BackGroundColor);
}

void TFT_ShowFloat(Point POS,double Number,u16 Color,u16 BackGroundColor)
{
	char BUF[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
	sprintf(BUF,"%14.8f",Number);
	TFT_ShowString(POS,BUF,Color,BackGroundColor);
	//if(BUF[3] == '.') BUF[6] = ' ';
}

//画矩形
//Point:和显示文字不同，此处设置的是像素，不是坐标
void Fill_Rectangle(Rectanle RECT,BOOL IsHollow,u16 Color)
{
	Rectanle TMP;
	u8 X_Length = RECT.X_RD - RECT.X_LU;
	u8 Y_Length = RECT.Y_RD - RECT.Y_LU;
	if(IsHollow == True)
	{
		TMP.X_LU = RECT.X_LU;
		TMP.Y_LU = RECT.Y_LU;
		TMP.X_RD = RECT.X_RD;	
		TMP.Y_RD = RECT.Y_LU;		
		Address_Set_Pixel(TMP); 		
		
		for(u8 j = X_Length;j > 0;j--)
		{
			TFT_Write_Data((u8)Color);	
			TFT_Write_Data((u8)(Color >> 8));			
		}

		TMP.X_LU = RECT.X_RD;
		TMP.Y_LU = RECT.Y_LU;
		TMP.X_RD = RECT.X_RD;	
		TMP.Y_RD = RECT.Y_RD;		
		Address_Set_Pixel(TMP); 		
		
		for(u8 j = Y_Length;j > 0;j--)
		{
			TFT_Write_Data((u8)Color);	
			TFT_Write_Data((u8)(Color >> 8));			
		}

		TMP.X_LU = RECT.X_LU;
		TMP.Y_LU = RECT.Y_RD;
		TMP.X_RD = RECT.X_RD;	
		TMP.Y_RD = RECT.Y_RD;		
		Address_Set_Pixel(TMP); 		
		
		for(u8 j = Y_Length + 1;j > 0;j--)
		{
			TFT_Write_Data((u8)Color);	
			TFT_Write_Data((u8)(Color >> 8));			
		}	
		
		TMP.X_LU = RECT.X_LU;
		TMP.Y_LU = RECT.Y_LU;
		TMP.X_RD = RECT.X_LU;	
		TMP.Y_RD = RECT.Y_RD;		
		Address_Set_Pixel(TMP); 		
		
		for(u8 j = Y_Length;j > 0;j--)
		{
			TFT_Write_Data((u8)Color);	
			TFT_Write_Data((u8)(Color >> 8));			
		}
	}
	else
	{
		X_Length++;
		Y_Length++;
		Address_Set_Pixel(RECT); 
		for(u8 i = X_Length;i > 0;i--)
			for(u8 j = Y_Length;j > 0;j--)
			{
				TFT_Write_Data((u8)Color);	
				TFT_Write_Data((u8)(Color >> 8));		
			}
	}
}

void TFT_Clear_Pixel(Rectanle RECT)
{
	u8 X_Length = RECT.X_RD - RECT.X_LU;
	u8 Y_Length = RECT.Y_RD - RECT.Y_LU;
	Address_Set_Pixel(RECT);
	for (u8 i=0;i<X_Length;i++)
		for (u8 j=0;j<Y_Length;j++)
		{
			TFT_Write_Data(0X00);
			TFT_Write_Data(0x00);
		}
	
}

void TFT_Show_Hex(Point POS,u16 Number,u16 Color,u16 BackGroundColor)
{
	char BUF[4];

	sprintf(BUF,"%4X",Number);

	TFT_ShowString(POS,BUF,Color,BackGroundColor);
}
