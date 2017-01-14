#ifndef __MPU9250_H__
#define __MPU9250_H__
#include "MPU9250_Register.h"
#include "ucos_ii.h"
#include "BSP.h"

#define CLEAR_GYR 0X01
#define CLEAR_ACC 0X02

typedef struct Vector_
{
	int16_t x;
	int16_t y;
	int16_t z;
}Vector_;

typedef struct MPU9250_Data_
{
	int16_t TEP;
	Vector_ ACC_ADC;
	Vector_ GYR_ADC;
	Vector_ MAG_ADC;
	Vector_ MAG_ASA;	
}MPU9250_Data_;

typedef struct MPU9250_
{
	uint16_t PORTX;	
	GPIO_TypeDef* GPIOX;	
	SPI_* SPIX;	
	MPU9250_Data_ Data;
	
}MPU9250_;

typedef struct MPU9250_OPS_
{
	void (*Init)(MPU9250_* Aim);
	void (*Ctrl)(u8 Cmd,u8 Value,MPU9250_* Aim);
	void (*ACC_GYR_Read)(MPU9250_* Aim);
	void (*MAG_Read)(MPU9250_* Aim);
	void (*TEP_Read)(MPU9250_* Aim);
}MPU9250_OPS_;
extern MPU9250_OPS_ MPU9250_OPS;

#endif
