#include "Board.h"






void Board_Init(void);
Board_ Board = 
{
	Board_Init
};

void Board_Init(void)
{	
	LED_OPS.Init(&LED1);
	Battery_OPS.Init(&Battery);
	BMP280_OPS.Init(&BMP280);
	MPU9250_OPS.Init(&MPU_B1);
	
	
//	MPU9250_OPS.Init(&MPU_B2);
//	MPU9250_OPS.Init(&MPU_B3);
//	MPU9250_OPS.Init(&MPU_B4);
//	MPU9250_OPS.Init(&MPU_B1);
//	MPU9250_OPS.Init(&MPU_B2);
//	MPU9250_OPS.Init(&MPU_B3);
//	MPU9250_OPS.Init(&MPU_B4);
}


