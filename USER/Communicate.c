#include "Communicate.h"

void UpData(void);
Communicate_ Communicate=
{
	UpData
};
RTK_XYZ_HP_ RTK_XYZ_HP_SendData;

void Send_Gps_MCU1(void)
{
	#define RTK_GPS_SIZE 96
	#define GPS_HEAD_SIZE 4
	

	u8 GPS_Head[GPS_HEAD_SIZE] = {'G','P','S','A'}; 

	RTK_XYZ_HP_SendData.PX = RTK_XYZ_HP.PX;
	RTK_XYZ_HP_SendData.PY = RTK_XYZ_HP.PY;
	RTK_XYZ_HP_SendData.PZ = RTK_XYZ_HP.PZ;
	
	RTK_XYZ_HP_SendData.VX = RTK_XYZ_HP.VX;
	RTK_XYZ_HP_SendData.VY = RTK_XYZ_HP.VY;
	RTK_XYZ_HP_SendData.VZ = RTK_XYZ_HP.VZ;
	
	RTK_XYZ_HP_SendData.Heading = RTK_XYZ_HP.Heading;
	RTK_XYZ_HP_SendData.Pitch = RTK_XYZ_HP.Pitch;
	
	RTK_XYZ_HP_SendData.Lon_M = RTK_XYZ_HP.Lon_M;
	RTK_XYZ_HP_SendData.Lat_M = RTK_XYZ_HP.Lat_M;
	RTK_XYZ_HP_SendData.Alt_M = RTK_XYZ_HP.Alt_M;
	RTK_XYZ_HP_SendData.Quality = RTK_XYZ_HP.Quality;

	Usart3.Send(GPS_Head,GPS_HEAD_SIZE);
	Usart3.Send((u8*)&RTK_XYZ_HP_SendData.PX,RTK_GPS_SIZE);
}

void UpData(void)
{
  Send_Gps_MCU1();
}



