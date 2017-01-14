#ifndef __MPU9250_REGISTER_H__
#define __MPU9250_REGISTER_H__



#define	SELF_TEST_X_GYRO		                0X00 
#define	SELF_TEST_Y_GYRO			              0X01
#define	SELF_TEST_Z_GYRO		                0X02
#define	SELF_TEST_X_ACCEL	                  0X0D 
#define	SELF_TEST_Y_ACCEL                   0X0E 
#define SELF_TEST_Z_ACCEL                   0X0F

#define XG_OFFSET_H                         0X13
#define XG_OFFSET_L                        	0X14
#define YG_OFFSET_H                         0X15
#define YG_OFFSET_L                  				0X16
#define ZG_OFFSET_H                         0X17
#define ZG_OFFSET_L  												0X18

#define XA_OFFSET_H                         0X77
#define XA_OFFSET_L                        	0X78
#define YA_OFFSET_H                         0X79
#define YA_OFFSET_L                  				0X80
#define ZA_OFFSET_H                         0X81
#define ZA_OFFSET_L  												0X82

#define SMPLRT_DIV                       		0X19  
#define GYRO_CONFIG_2   										0X1A
#define GYRO_CONFIG_1  											0X1B
#define ACCEL_CONFIG_1   										0X1C
#define ACCEL_CONFIG_2   										0X1D
#define LP_ACCEL_ODR 												0X1E
#define WOM_THR                             0X1F
#define FIFO_EN															0X23

#define I2C_MST_CTRL												0X24
#define I2C_SLV0_ADDR												0X25
#define I2C_SLV0_REG												0X26
#define I2C_SLV0_CTRL												0X27
#define I2C_SLV0_DO                         0x63
#define I2C_MST_STATUS											0X36
#define EXT_SENS_DATA_00                    0X49
#define INT_PIN_CFG                         0X37
#define INT_ENABLE													0X38
#define INT_STATUS                          0X3A

#define	ACCEL_XOUT_H												0X3B
#define	ACCEL_XOUT_L												0X3C
#define	ACCEL_YOUT_H												0X3D
#define	ACCEL_YOUT_L												0X3E
#define	ACCEL_ZOUT_H												0X3F
#define	ACCEL_ZOUT_L												0X40

#define	TEMP_OUT_H													0X41   
#define	TEMP_OUT_L													0X42

#define	GYRO_XOUT_H													0X43
#define	GYRO_XOUT_L													0X44	
#define	GYRO_YOUT_H													0X45
#define	GYRO_YOUT_L													0X46
#define	GYRO_ZOUT_H													0X47
#define	GYRO_ZOUT_L													0X48
#define I2C_MST_DELAY_CTRL                  0X67
#define USER_CTRL                           0X6A
#define	PWR_MGMT_1		                      0X6B
#define	WHO_AM_I		          							0X75	


//--------------------AK8963------------------------//

#define AK8963_ADDR                         0X0C  //AKM addr
#define AK8963_WHOAMI		                    0X00  //AKM ID addr
#define AK8963_ST1		                      0X02  //Data Status1
#define AK8963_ST2		                      0X09  //Data reading end register & check Magnetic sensor overflow occurred
#define AK8963_CNTL1	                      0X0A
#define AK8963_CNTL2		                    0X0B
#define AK8963_ASAX                         0X10  //X-axis sensitivity adjustment value 
#define AK8963_ASAY                         0X11  //Y-axis sensitivity adjustment value
#define AK8963_ASAZ                         0X12  //Z-axis sensitivity adjustment value
		
#define MAG_XOUT_L		                      0X03
#define MAG_XOUT_H		                      0X04
#define MAG_YOUT_L		                      0X05
#define MAG_YOUT_H		                      0X06
#define MAG_ZOUT_L		                      0X07
#define MAG_ZOUT_H		                      0X08
	





#endif
