#include "BMP280.h"

#define CSN_H         Aim->GPIOX->BSRR = Aim->PORTX
#define CSN_L         Aim->GPIOX->BRR  = Aim->PORTX
#define BMP280_NOP()  Os_Time.WaitMS(10)
#define FIX_POINT

void BMP280_Init(BMP280_* Aim);
void BMP280_Read_Temp(BMP280_* Aim);
void BMP280_Read_Press(BMP280_* Aim);
void BMP280_Ctrl(u8 Cmd,u8 Value,BMP280_* Aim);

BMP280_OPS_ BMP280_OPS =
{
	BMP280_Init,
	BMP280_Read_Press,	
	BMP280_Read_Temp,
	BMP280_Ctrl

};

u8 BMP280_SPI_Read(u8 Reg,u8* Value, u8 Cnt,BMP280_* Aim)
{
	u8 Ret;
	CSN_L;
	Ret = Spi1.Trans(Reg | 0X80);
	for(u8 i = 0;i < Cnt;i++)
	{
		Value[i] = Spi1.Trans(0XFF);
	}
	CSN_H;
	
	return Ret;
}

u8 BMP280_SPI_Write(u8 Reg,u8 Value,BMP280_* Aim)
{
	u8 Ret;
	CSN_L;
	Ret = Spi1.Trans(Reg & 0X7F);
	Spi1.Trans(Value);
	CSN_H;
	
	return Ret;
}

void BMP280_Init(BMP280_* Aim)
{
	u8 BUF[2];
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = Aim->PORTX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(Aim->GPIOX, &GPIO_InitStructure);	
	GPIO_SetBits(Aim->GPIOX,Aim->PORTX);
	
	BMP280_SPI_Write(RESET,0XB6,Aim);
	
	BMP280_NOP();
	
	BMP280_SPI_Write(BMP280_CTRL_MEAS_REG,0X57,Aim); //OSR_P:16 OSR_T:2 Normal
	BMP280_SPI_Write(BMP280_CONFIG_REG,   0x10,Aim); //²ÉÑù¼ä¸ô0.5MS IIR:16
	
	BMP280_SPI_Read(BMP280_DIG_T1_LSB_REG,BUF,2,Aim);
	Aim->Data.Para.dig_T1 = (u16)BUF[1] << 8 | BUF[0];

	BMP280_SPI_Read(BMP280_DIG_T2_LSB_REG,BUF,2,Aim);
	Aim->Data.Para.dig_T2 = (s16)BUF[1] << 8 | BUF[0];

	BMP280_SPI_Read(BMP280_DIG_T3_LSB_REG,BUF,2,Aim);
	Aim->Data.Para.dig_T3 = (s16)BUF[1] << 8 | BUF[0];

	BMP280_SPI_Read(BMP280_DIG_P1_LSB_REG,BUF,2,Aim);
	Aim->Data.Para.dig_P1 = (u16)BUF[1] << 8 | BUF[0];

	BMP280_SPI_Read(BMP280_DIG_P2_LSB_REG,BUF,2,Aim);
	Aim->Data.Para.dig_P2 = (s16)BUF[1] << 8 | BUF[0];	
	
	BMP280_SPI_Read(BMP280_DIG_P3_LSB_REG,BUF,2,Aim);
	Aim->Data.Para.dig_P3 = (s16)BUF[1] << 8 | BUF[0];	

	BMP280_SPI_Read(BMP280_DIG_P4_LSB_REG,BUF,2,Aim);
	Aim->Data.Para.dig_P4 = (s16)BUF[1] << 8 | BUF[0];	
	
	BMP280_SPI_Read(BMP280_DIG_P5_LSB_REG,BUF,2,Aim);
	Aim->Data.Para.dig_P5 = (s16)BUF[1] << 8 | BUF[0];	

	BMP280_SPI_Read(BMP280_DIG_P6_LSB_REG,BUF,2,Aim);
	Aim->Data.Para.dig_P6 = (s16)BUF[1] << 8 | BUF[0];
	
	BMP280_SPI_Read(BMP280_DIG_P7_LSB_REG,BUF,2,Aim);
	Aim->Data.Para.dig_P7 = (s16)BUF[1] << 8 | BUF[0];

	BMP280_SPI_Read(BMP280_DIG_P8_LSB_REG,BUF,2,Aim);
	Aim->Data.Para.dig_P8 = (s16)BUF[1] << 8 | BUF[0];
	
	BMP280_SPI_Read(BMP280_DIG_P9_LSB_REG,BUF,2,Aim);
	Aim->Data.Para.dig_P9 = (s16)BUF[1] << 8 | BUF[0];	
}

void BMP280_Read_Press(BMP280_* Aim)
{
	u8 BUF[3];
	u32 Press_ADC;	
	BMP280_SPI_Read(BMP280_PRESS_HSB_REG,BUF,3,Aim);
	Press_ADC = (int32_t)BUF[0] << 12 | (int16_t)BUF[1] << 4 | BUF[2] >> 4;
	
#ifdef FIX_POINT	
	
	s32 var1,var2;
	u32 Press;
	
	var1 = (((s32)Aim->Data.Para.t_fine) >> 1) - (s32)64000;
	var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((s32)Aim->Data.Para.dig_P6);
	var2 = var2 + ((var1 * ((s32)Aim->Data.Para.dig_P5)) << 1);
	var2 = (var2 >> 2) + (((s32)Aim->Data.Para.dig_P4) << 16);
	var1 = (((Aim->Data.Para.dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((s32)Aim->Data.Para.dig_P2) * var1) >> 1)) >> 18;
	var1 = ((((32768 + var1)) * ((s32)Aim->Data.Para.dig_P1)) >> 15);
	
	if(var1 == 0) return;
	
	Press = (((u32)(((s32)1048576) - Press_ADC) - (var2 >> 12))) * 3125;
	
	if(Press < 0X80000000) Press = (Press << 1) / (u32)var1;
	else Press = (Press / (u32)var1) * 2;
	
	var1 = (((s32)Aim->Data.Para.dig_P9) * ((s32)(((Press >> 3) * (Press >> 3)) >> 13))) >> 12;
	var2 = (((s32)(Press >> 2)) * ((s32)Aim->Data.Para.dig_P8)) >> 13;
	Press = (u32)((s32)Press + ((var1 + var2 + Aim->Data.Para.dig_P7) >> 4));
	// KPa
#else		
	double var1, var2;
	float Press;
	var1 = ((double)Aim->Data.Para.t_fine / 2.0) - 64000.0;	
	var2 = var1 * var1 * ((double)Aim->Data.Para.dig_P6) / 32768.0;
	var2 = var2 + var1 * ((double)Aim->Data.Para.dig_P5) * 2.0;
	var2 = (var2 / 4.0)+(((double)Aim->Data.Para.dig_P4) * 65536.0);
	var1 = (((double)Aim->Data.Para.dig_P3) * var1 * var1 / 524288.0 + ((double)Aim->Data.Para.dig_P2) * var1) / 524288.0;
	var1 = (1.0f + var1 / 32768.0) * ((double)Aim->Data.Para.dig_P1);
	if (var1 == 0.0) return;
	Press = 1048576.0 - (double)Press_ADC;
	Press = (Press - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double)Aim->Data.Para.dig_P9) * Press * Press / 2147483648.0;
	var2 = Press * ((double)Aim->Data.Para.dig_P8) / 32768.0;
	Press = Press + (var1 + var2 + ((double)Aim->Data.Para.dig_P7)) / 16.0;
#endif		
	Aim->Data.Press = (float)Press / 100.0f;
}

void BMP280_Read_Temp(BMP280_* Aim)
{
	u8 BUF[3];
	u32 Temp_ADC;
	s32 var1,var2;
	
	BMP280_SPI_Read(BMP280_TEMP_HSB_REG,BUF,3,Aim);
	Temp_ADC = (int32_t)BUF[0] << 12 | (int16_t)BUF[1] << 4 | BUF[2] >> 4;
		
	var1 = ((((Temp_ADC >> 3) - ((s32)Aim->Data.Para.dig_T1 << 1))) * ((s32)Aim->Data.Para.dig_T2)) >> 11;
	var2 = (((((Temp_ADC >> 4) - ((s32)Aim->Data.Para.dig_T1)) * ((Temp_ADC >> 4) - ((s32)Aim->Data.Para.dig_T1))) >> 12) * ((s32)Aim->Data.Para.dig_T3)) >> 14;
	Aim->Data.Para.t_fine = var1 + var2;
	
	Aim->Data.Temp = (float)((Aim->Data.Para.t_fine * 5 + 128) >> 8) / 100.0f;
}

void BMP280_Ctrl(u8 Cmd,u8 Value,BMP280_* Aim)
{

}


