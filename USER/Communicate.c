#include "Communicate.h"

void UpData(void);
Communicate_ Communicate=
{
	UpData
};
RTK_GPS_ GPS_Send_Data;

void Send_Gps_MCU1(void)
{
	#define RTK_GPS_SIZE 48
	#define GPS_HEAD_SIZE 3
	

	u8 GPS_Head[GPS_HEAD_SIZE] = {'G','P','S'}; 
	
	GPS_Send_Data.Alt_M = RTK_GPS.Alt_M;
	GPS_Send_Data.Lat_M = RTK_GPS.Lat_M;
	GPS_Send_Data.Lon_M = RTK_GPS.Lon_M;
	GPS_Send_Data.Quality = RTK_GPS.Quality;
	GPS_Send_Data.Speed_M = RTK_GPS.Speed_M;
	GPS_Send_Data.TrackAngle = RTK_GPS.TrackAngle;
	
	Usart3.Send(GPS_Head,GPS_HEAD_SIZE);
	Usart3.Send((u8*)&GPS_Send_Data.Quality,RTK_GPS_SIZE);
}

void UpData(void)
{
  Send_Gps_MCU1();
}



