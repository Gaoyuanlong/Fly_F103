#include "Communicate.h"

void UpData(void);
Communicate_ Communicate=
{
	UpData
};

void Usart3_Send_Gps(void)
{
	#define RTK_GPS_SIZE 48
	#define GPS_HEAD_SIZE 3
	
	RTK_GPS_ RTK_GPS_Send;
	u8 GPS_Head[GPS_HEAD_SIZE] = {'G','P','S'}; 
	
	RTK_GPS_Send.Alt_M = RTK_GPS.Alt_M;
	RTK_GPS_Send.Lat_M = RTK_GPS.Lat_M;
	RTK_GPS_Send.Lon_M = RTK_GPS.Lon_M;
	RTK_GPS_Send.Quality = RTK_GPS.Quality;
	RTK_GPS_Send.Speed_M = RTK_GPS.Speed_M;
	RTK_GPS_Send.TrackAngle = RTK_GPS.TrackAngle;
	
	Usart2.Send(GPS_Head,GPS_HEAD_SIZE);
	Usart2.Send((u8*)&RTK_GPS_Send.Alt_M,RTK_GPS_SIZE);
}

void UpData(void)
{
  Usart3_Send_Gps();
}



