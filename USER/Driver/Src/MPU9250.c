#include "MPU9250.h"


#define CSN_H         Aim->GPIOX->BSRR = Aim->PORTX
#define CSN_L         Aim->GPIOX->BRR  = Aim->PORTX
// 地磁计测量速度低，利用操作系统的调度函数延时
#define MPU9250_NOP()      OSTimeDly(5)

void MPU9250_Init(MPU9250_* Aim);
void MPU9250_Ctrl(u8 Cmd,u8 Value,MPU9250_* Aim);	
void MPU9250_Read_GYR_ACC(MPU9250_* Aim);
void MPU9250_MAG_Read(MPU9250_* Aim);
void MPU9250_TEP_Read(MPU9250_* Aim);

MPU9250_OPS_ MPU9250_OPS =
{
	MPU9250_Init,
	MPU9250_Ctrl,
	MPU9250_Read_GYR_ACC,
	MPU9250_MAG_Read,
	MPU9250_TEP_Read,
};

u8 MPU9250_SPI_Read(u8 Reg,MPU9250_* Aim)
{
	u8 Ret;
	CSN_L;

	Aim->SPIX->Trans(Reg | 0X80);
	Ret = Aim->SPIX->Trans(0XFF);
	
	CSN_H;

	return Ret;
}


u8 MPU9250_SPI_Write(u8 Reg,u8 Value,MPU9250_* Aim)
{
	u8 Ret;
	CSN_L;
	
	Ret = Aim->SPIX->Trans(Reg);
	Aim->SPIX->Trans(Value);

	CSN_H;
	
	return Ret;
}

u8 MPU9250_I2C_Read(u8 Reg,MPU9250_* Aim)
{

	MPU9250_SPI_Write(I2C_SLV0_ADDR,AK8963_ADDR | 0X80,Aim);  
	MPU9250_SPI_Write(I2C_SLV0_REG,Reg,Aim); 
	MPU9250_SPI_Write(I2C_SLV0_DO,0XFF,Aim);
	MPU9250_NOP();
	return MPU9250_SPI_Read(EXT_SENS_DATA_00,Aim);
}

void MPU9250_I2C_Write(u8 Reg,u8 Value,MPU9250_* Aim)
{
	MPU9250_SPI_Write(I2C_SLV0_ADDR,AK8963_ADDR,Aim); 
	MPU9250_SPI_Write(I2C_SLV0_REG,Reg,Aim);                
	MPU9250_SPI_Write(I2C_SLV0_DO,Value,Aim); 
	MPU9250_NOP();	
}

void MPU9250_Init(MPU9250_* Aim)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = Aim->PORTX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(Aim->GPIOX, &GPIO_InitStructure);	
	GPIO_SetBits(Aim->GPIOX,Aim->PORTX);	
	
	MPU9250_SPI_Write(PWR_MGMT_1,        0X81,Aim);	//复位，时钟选择GYRO时钟，百分之1精度
	MPU9250_NOP();
	MPU9250_SPI_Write(SMPLRT_DIV,        0X02,Aim); //内部采样速率：1kHz 2分频
	MPU9250_SPI_Write(GYRO_CONFIG_1,     0X18,Aim); //陀螺仪量程  2000deg/s
	MPU9250_SPI_Write(GYRO_CONFIG_2,     0X04,Aim); //陀螺仪及温度低通滤波频率 20HZ	
	MPU9250_SPI_Write(ACCEL_CONFIG_1,    0X10,Aim); //加速度量程  8G
	MPU9250_SPI_Write(ACCEL_CONFIG_2,    0X00,Aim); //加速计低通滤波频率带宽460Hz

	MPU9250_SPI_Write(FIFO_EN       ,    0X78,Aim); //开启fifo
	
	MPU9250_SPI_Write(I2C_MST_CTRL,      0X4D,Aim); //I2C Master mode and Speed 400 kHz
	MPU9250_SPI_Write(USER_CTRL,         0X20,Aim); //I2C_MST_EN 
	MPU9250_SPI_Write(I2C_MST_DELAY_CTRL,0x01,Aim); //延时使能I2C_SLV0_DLY_EN 	
	MPU9250_SPI_Write(I2C_SLV0_CTRL ,    0x81,Aim); //Enable IICand EXT_SENS_DATA =1 Byte	
	
	
	
	MPU9250_I2C_Write(AK8963_CNTL2,      0x01,Aim); //复位
	
	MPU9250_NOP();
	
	MPU9250_I2C_Write(AK8963_CNTL1,      0X12,Aim); //地磁连续转换模式,16bit,8Hz 
	
	Aim->Data.MAG_ASA.x = MPU9250_I2C_Read(AK8963_ASAX,Aim); 
	Aim->Data.MAG_ASA.y = MPU9250_I2C_Read(AK8963_ASAY,Aim); 
	Aim->Data.MAG_ASA.z = MPU9250_I2C_Read(AK8963_ASAZ,Aim);
}

void MPU9250_ACC_Read(MPU9250_* Aim)
{
	u8 BUF[6];
	BUF[0] = MPU9250_SPI_Read(ACCEL_XOUT_L,Aim);
	BUF[1] = MPU9250_SPI_Read(ACCEL_XOUT_H,Aim);
	
	BUF[2] = MPU9250_SPI_Read(ACCEL_YOUT_L,Aim);
	BUF[3] = MPU9250_SPI_Read(ACCEL_YOUT_H,Aim);
	BUF[4] = MPU9250_SPI_Read(ACCEL_ZOUT_L,Aim);
	BUF[5] = MPU9250_SPI_Read(ACCEL_ZOUT_H,Aim);
	
	Aim->Data.ACC_ADC.x = (int16_t) BUF[1] << 8 | BUF[0]; 
	Aim->Data.ACC_ADC.y = (int16_t) BUF[3] << 8 | BUF[2]; 
	Aim->Data.ACC_ADC.z = (int16_t) BUF[5] << 8 | BUF[4];

}


void MPU9250_GYR_Read(MPU9250_* Aim)
{
	u8 BUF[6];	
	BUF[0] = MPU9250_SPI_Read(GYRO_XOUT_L,Aim);
	BUF[1] = MPU9250_SPI_Read(GYRO_XOUT_H,Aim);
	BUF[2] = MPU9250_SPI_Read(GYRO_YOUT_L,Aim);
	BUF[3] = MPU9250_SPI_Read(GYRO_YOUT_H,Aim);
	BUF[4] = MPU9250_SPI_Read(GYRO_ZOUT_L,Aim);
	BUF[5] = MPU9250_SPI_Read(GYRO_ZOUT_H,Aim);
	
	Aim->Data.GYR_ADC.x = (int16_t) BUF[1] << 8 | BUF[0]; 
	Aim->Data.GYR_ADC.y = (int16_t) BUF[3] << 8 | BUF[2]; 
	Aim->Data.GYR_ADC.z = (int16_t) BUF[5] << 8 | BUF[4]; 	

}

void MPU9250_Read_GYR_ACC(MPU9250_* Aim)
{
	if((MPU9250_SPI_Read(INT_STATUS,Aim) & 0X01) == 0X01)
	{
		MPU9250_ACC_Read(Aim);
		MPU9250_GYR_Read(Aim);
	}
}

void MPU9250_ACC_Clear(MPU9250_* Aim)
{
	MPU9250_SPI_Write(XA_OFFSET_H,0,Aim);
	MPU9250_SPI_Write(XA_OFFSET_L,0,Aim);
	MPU9250_SPI_Write(YA_OFFSET_H,0,Aim);
	MPU9250_SPI_Write(YA_OFFSET_L,0,Aim);
	MPU9250_SPI_Write(ZA_OFFSET_H,0,Aim);
	MPU9250_SPI_Write(ZA_OFFSET_L,0,Aim);
}


void MPU9250_GYR_Clear(MPU9250_* Aim)
{
	MPU9250_SPI_Write(XG_OFFSET_H,0,Aim);
	MPU9250_SPI_Write(XG_OFFSET_L,0,Aim);
	MPU9250_SPI_Write(YG_OFFSET_H,0,Aim);
	MPU9250_SPI_Write(YG_OFFSET_L,0,Aim);
	MPU9250_SPI_Write(ZG_OFFSET_H,0,Aim);
	MPU9250_SPI_Write(ZG_OFFSET_L,0,Aim);
}

void MPU9250_MAG_Read(MPU9250_* Aim)
{
	u8 BUF[6];
	if(MPU9250_I2C_Read(AK8963_ST1,Aim))
	{
		BUF[0] = MPU9250_I2C_Read(MAG_XOUT_L,Aim);
		BUF[1] = MPU9250_I2C_Read(MAG_XOUT_H,Aim);
		BUF[2] = MPU9250_I2C_Read(MAG_YOUT_L,Aim);
		BUF[3] = MPU9250_I2C_Read(MAG_YOUT_H,Aim);
		BUF[4] = MPU9250_I2C_Read(MAG_ZOUT_L,Aim);
		BUF[5] = MPU9250_I2C_Read(MAG_ZOUT_H,Aim);	
		if((MPU9250_I2C_Read(AK8963_ST2,Aim) & 0X08) != 0X80) 
		{
			Aim->Data.MAG_ADC.x = (int16_t) BUF[1] << 8 | BUF[0];		
			Aim->Data.MAG_ADC.y = (int16_t) BUF[3] << 8 | BUF[2];
			Aim->Data.MAG_ADC.z = (int16_t) BUF[5] << 8 | BUF[4];
		}
	}
} 

void MPU9250_TEP_Read(MPU9250_* Aim)
{
	u8 BUF[2];	
	BUF[0] = MPU9250_SPI_Read(TEMP_OUT_L,Aim);
	BUF[1] = MPU9250_SPI_Read(TEMP_OUT_H,Aim);

	Aim->Data.TEP = (int16_t) BUF[1] << 8 | BUF[0]; 
}

void MPU9250_Ctrl(u8 Cmd,u8 Value,MPU9250_* Aim)
{
	switch (Cmd)
	{
		case CLEAR_GYR:
			MPU9250_GYR_Clear(Aim);
			break;
		case CLEAR_ACC:
			MPU9250_ACC_Clear(Aim);
			break;
		default:
			break;
	}
}


