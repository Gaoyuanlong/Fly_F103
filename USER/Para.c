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
	
	sprintf(Buff,"%14.8f\t%14.8f\t%14.8f\t%14.8f\t%14.8f\t%14.8f\t%c\t\r\n",
																						 Para.Data->Data1,
																					   Para.Data->Data2,
																						 Para.Data->Data3,
																						 Para.Data->Data4,
																					   Para.Data->Data5,
																						 Para.Data->Data6,
																						 (char)Para.Data->Data7);
	
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
	static u8 cnt = 0;
	
	Para.Data->Data1 = GPS_Send_Data.Alt_M;
	Para.Data->Data2 = GPS_Send_Data.Lat_M;
	Para.Data->Data3 = GPS_Send_Data.Lon_M;
	Para.Data->Data4 = GPS_Send_Data.TrackAngle;
	Para.Data->Data5 = GPS_Send_Data.Speed_M;
	Para.Data->Data6 = GPS_Send_Data.Quality;
	Para.Data->Data7 =  RTK_RMC.ModeInd;
	
	if(RTK_RMC.ModeInd != 'N')
	{
		cnt++;
		if(cnt > 20)
			 
		Para.IsNeedSave = True;
		return True;
	}
	else
		cnt = 0 ;
	return False;
}

BOOL Para_Save(void)
{
	static u8 cnt = 0;
	static u8 Close_Flage = 0;
	
	if(Close_Flage ==0)
	{
		if(Para.IsNeedSave == False) return False;
		if(F_RET != FR_OK) return False;
		
		Save_GPS();
			
		if(RTK_RMC.ModeInd == 'N')
		{
			cnt ++;
			if(cnt > 20)
			{
				f_close(&GPS_Data);		
				Close_Flage = 1;
				cnt = 0;
				return False;
			}
		} 
		else
				cnt = 0;
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
