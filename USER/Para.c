#include "Para.h"

void Para_Init(void);
BOOL Para_Updata(void);
BOOL Para_Save(void);
BOOL Para_Read(void);

FATFS fs;
FRESULT F_RET;
FIL GPS_Data;

struct Para_Data_ Para_Data;

struct Para_ Para = 
{
	&Para_Data,
	False,
	Para_Init,
	Para_Updata,
	Para_Save,
	Para_Read
};

//´æ´¢GPSÊý¾Ý
void Save_GPS(void)
{
	UINT P;	
	char Buff[100];	
	
	f_lseek(&GPS_Data,GPS_Data.fsize);
//		sprintf(Buff,"%14.8f\t%14.8f\t%c\r\n",
//																						 Para_Data.Rtk.Lat,
//																					   Para_Data.Rtk.Lon,
//																						 Para_Data.Rtk.ModeInd);
	sprintf(Buff,"%14.8f\t%14.8f\t%c\t%14.8f\t%14.8f\t%c\t\r\n",
																						 Para_Data.Rtk.Lat,
																					   Para_Data.Rtk.Lon,
																						 Para_Data.Rtk.ModeInd,
																						 Para_Data.Gga.Lat,
																					   Para_Data.Gga.Lon,
																						 Para_Data.Gga.Quality);
	
	f_write(&GPS_Data,Buff,strlen(Buff),&P);	
}

void Set_Title(FIL* Data,const char* Path,const char* Tttle)
{
	UINT P;

	f_open(Data,Path,FA_CREATE_ALWAYS | FA_WRITE);
	f_write(Data,Tttle,strlen(Tttle),&P);	
	f_close(Data);	
}

void Para_Init(void)
{
	F_RET = f_mount(&fs,"0:",1);
	if(F_RET != FR_OK) return;
	Set_Title(&GPS_Data,"0:/GPS_Data.txt","RTK.Lat\tRTK.Lon\tRTK.ModeInd\tGGA.Lat\tGGA.Lon\tGGA.Quality\r\n");
	
	f_open(&GPS_Data,"0:/GPS_Data.txt",FA_OPEN_ALWAYS | FA_WRITE);
}

BOOL Para_Updata(void)
{
	Para.Data->Rtk.Lat = RTK_RMC.Lat;
	Para.Data->Rtk.Lon = RTK_RMC.Lon;
	Para.Data->Rtk.ModeInd = RTK_RMC.ModeInd;
	
	Para.Data->Gga.Lat = RTK_GGA.Lat;
	Para.Data->Gga.Lon = RTK_GGA.Lon;
	Para.Data->Gga.Quality = RTK_GGA.Quality;
	
	if(Para.Data->Rtk.ModeInd == 'A'|| Para.Data->Rtk.ModeInd == 'D')
	{
		Para.IsNeedSave = True;
		return True;
	}
	return False;
}

BOOL Para_Save(void)
{
	static u8 Close_Flage = 0;
	
	if(Close_Flage ==0)
	{
		if(Para.IsNeedSave == False) return False;
		if(F_RET != FR_OK) return False;
		
		Save_GPS();
		
		if(Para.IsNeedSave == True && Para.Data->Rtk.ModeInd == 'N')
		{
			f_close(&GPS_Data);		
			Close_Flage = 1;
			return False;
		} 
		return True;
	}
	return False;
}

BOOL Para_Read(void)
{
	return True;
}


/*

*/
