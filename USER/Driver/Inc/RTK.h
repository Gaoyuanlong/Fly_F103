#ifndef __RTK_H__
#define __RTK_H__
#include "USART.h"

typedef struct RTK_RMC_
{
	BOOL IsPosFix;
	u32 Date;
	char LatDir;
	char MagDir;
	char LonDir;
	char ModeInd;	
	
	double UTC;
	double Lat;
	double Lon;
	double Speed;
	double TrackAngle;
	double MagAngle;
}RTK_RMC_;

typedef struct RTK_GGA_
{	
	char LatDir;
	char LonDir;
	char Quality;
	u8 Sats;
	char AUnits;	
	char UUnits;
	u8 Age;
	char StnID[4];	
	
	double UTC;
	double Lat;
	double Lon;
	double Hdop;
	double Alt;
	double Undulation;
}RTK_GGA_;

typedef struct RTK_GPS_
{
	u8 Quality;
	double Lat_M;
	double Lon_M;
	double Alt_M;
	double TrackAngle;
	double Speed_M;
}RTK_GPS_;

typedef struct RTK_XYZ_HP_
{
	double PX;
	double PY;
	double PZ;
	
	double VX;
	double VY;
	double VZ;
	
	double Heading;
	double Pitch; 
	
	double Lon_M;
	double Lat_M;
	double Alt_M;
	u8 Quality;
}RTK_XYZ_HP_;

typedef struct RTK_OPS_
{
	void (*Init)(void);
	BOOL (*Read)(u8 Cmd);
}RTK_OPS_;


extern RTK_RMC_ RTK_RMC;
extern RTK_GGA_ RTK_GGA;
//extern RTK_GPS_ RTK_GPS;
extern RTK_OPS_ RTK_OPS;
extern RTK_XYZ_HP_ RTK_XYZ_HP;
#endif
