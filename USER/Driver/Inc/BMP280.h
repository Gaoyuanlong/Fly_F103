#ifndef __BMP280_H__
#define __BMP280_H__
#include "BSP.h"
#define BMP280_DEVICE_ID_REG 								 0XD0
#define BMP280_RESET_REG     								 0XE0
#define BMP280_STATUS_REG    								 0XF3
#define BMP280_CTRL_MEAS_REG 								 0XF4
#define BMP280_CONFIG_REG    								 0XF5
#define BMP280_PRESS_HSB_REG 							   0XF7
#define BMP280_PRESS_MSB_REG 								 0XF8
#define BMP280_PRESS_LSB_REG 								 0XF9
#define BMP280_TEMP_HSB_REG  								 0XFA
#define BMP280_TEMP_MSB_REG									 0XFB
#define BMP280_TEMP_LSB_REG									 0XFC

#define BMP280_DIG_T1_LSB_REG                0x88  
#define BMP280_DIG_T1_MSB_REG                0x89  
#define BMP280_DIG_T2_LSB_REG                0x8A  
#define BMP280_DIG_T2_MSB_REG                0x8B  
#define BMP280_DIG_T3_LSB_REG                0x8C  
#define BMP280_DIG_T3_MSB_REG                0x8D  
#define BMP280_DIG_P1_LSB_REG                0x8E  
#define BMP280_DIG_P1_MSB_REG                0x8F  
#define BMP280_DIG_P2_LSB_REG                0x90  
#define BMP280_DIG_P2_MSB_REG                0x91  
#define BMP280_DIG_P3_LSB_REG                0x92  
#define BMP280_DIG_P3_MSB_REG                0x93  
#define BMP280_DIG_P4_LSB_REG                0x94  
#define BMP280_DIG_P4_MSB_REG                0x95  
#define BMP280_DIG_P5_LSB_REG                0x96  
#define BMP280_DIG_P5_MSB_REG                0x97  
#define BMP280_DIG_P6_LSB_REG                0x98  
#define BMP280_DIG_P6_MSB_REG                0x99  
#define BMP280_DIG_P7_LSB_REG                0x9A  
#define BMP280_DIG_P7_MSB_REG                0x9B  
#define BMP280_DIG_P8_LSB_REG                0x9C  
#define BMP280_DIG_P8_MSB_REG                0x9D  
#define BMP280_DIG_P9_LSB_REG                0x9E  
#define BMP280_DIG_P9_MSB_REG                0x9F 

typedef struct BMP280_Para_
{
	u16 dig_T1;
	s16 dig_T2;
	s16 dig_T3;
	
	u16 dig_P1;
	s16 dig_P2;
	s16 dig_P3;	
	s16 dig_P4;
	s16 dig_P5;		
	s16 dig_P6;
	s16 dig_P7;	
	s16 dig_P8;
	s16 dig_P9;	
	s32 t_fine;
	
}BMP280_Para_;

typedef struct BMP280_Data_
{
	float Temp;
	float Press;
	BMP280_Para_ Para;
}BMP280_Data_;

typedef struct BMP280_
{
	uint16_t PORTX;	
	GPIO_TypeDef* GPIOX;	
	SPI_* SPIX;	
	BMP280_Data_ Data;
	
}BMP280_;

typedef struct BMP280_OPS_
{
	void (*Init)(BMP280_* Aim);
	void (*ReadPress)(BMP280_* Aim);
	void (*ReadTemp)(BMP280_* Aim);
	void (*Ctrl)(u8 Cmd,u8 Value,BMP280_* Aim);
}BMP280_OPS_;

extern BMP280_OPS_ BMP280_OPS;

#endif


