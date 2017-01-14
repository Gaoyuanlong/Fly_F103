#ifndef __RTK_H__
#define __RTK_H__
#include "USART.h"


#define GPRMC_UTC_STR_ADD					1
#define GPRMC_POS_STA_STR_ADD			2
#define GPRMC_LAT_STR_ADD					3
#define GPRMC_LAT_DIR_STR_ADD			4
#define GPRMC_LON_STR_ADD					5
#define GPRMC_LON_DIR_STR_ADD			6
#define GPRMC_SPEED_STR_ADD				7
#define GPRMC_TRACK_ANGLE_STR_ADD	8
#define GPRMC_DATE_STR_ADD				9
#define GPRMC_MAG_ANGLE_STR_ADD		10
#define GPRMC_MAG_DIR_STR_ADD			11
#define GPRMC_MODE_IND_STR_ADD		12

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

typedef struct RTK_OPS_
{
	void (*Init)(void);
	BOOL (*Read)(u8 Cmd);
}RTK_OPS_;


extern RTK_RMC_ RTK_RMC;
extern RTK_GGA_ RTK_GGA;
extern RTK_GPS_ RTK_GPS;
extern RTK_OPS_ RTK_OPS;
#endif
