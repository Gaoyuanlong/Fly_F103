#ifndef __BOARD_H__
#define __BOARD_H__
// Driver
#include <stdio.h>
#include "MPU9250.h"
#include "BMP280.h"
#include "Battery.h"
#include "Led.h"
#in

typedef struct Board_
{
	void (*Init)(void);

}Board_;

extern Board_ Board;

extern MPU9250_ MPU_B1;
extern MPU9250_ MPU_B2;
extern MPU9250_ MPU_B3;
extern MPU9250_ MPU_B4;

extern MPU9250_ MPU_T1;
extern MPU9250_ MPU_T2;
extern MPU9250_ MPU_T3;
extern MPU9250_ MPU_T4;

extern LED_ LED1;
extern Battery_ Battery;
extern BMP280_ BMP280;
/*********************************************/
#endif 


