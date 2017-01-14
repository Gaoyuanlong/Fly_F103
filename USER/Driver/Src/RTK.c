#include "RTK.h"

void RTK_Init(void);
BOOL RTK_Read(u8 Cmd);

RTK_RMC_ RTK_RMC;
RTK_GGA_ RTK_GGA;
RTK_GPS_ RTK_GPS;

RTK_OPS_ RTK_OPS=
{
	RTK_Init,
	RTK_Read
};


BOOL RTK_Find_Head(const char* Str)
{
	u8 BUF = 0;
	const char* P = Str;
	
	while(*P != '\0')
	{
		Usart2.Recv(&BUF,0,1,True);
		if(BUF == *P)
			P++;
		else
			P = Str;
	}

	return True;
}

u8 RTK_Read_Frame(const char* Str,u8* BUF)
{ 
	u8 i = 0;
	
	BUF[i] = Str[i];i++;
	BUF[i] = Str[i];i++;
	BUF[i] = Str[i];i++;
	BUF[i] = Str[i];i++;
	BUF[i] = Str[i];i++;
	
	while(1)
	{
		Usart2.Recv(&BUF[i++],0,1,True);
	
		if((BUF[i] == '\n') && (BUF[i - 1] == '\r')) 
			break;
		
		if(i == 128)
			break;
	}
	return i + 1;
}


double RTK_Str2Float(const u8* Str_Add)
{ 
	float Float_Int_temp = 0;
	double Float_Float_temp = 0;
	const u8* Str_Add_temp = Str_Add;
	u8 i = 0 , j = 0;
	
	if(Str_Add_temp[i] == '-')
	{
		i++;
	}
	for(; Str_Add_temp[i] != '.' && Str_Add_temp[i] != ',' ; i++)
	{
		Float_Int_temp = (Float_Int_temp + (Str_Add_temp[i] - '0'))*10;
	}
	Float_Int_temp = Float_Int_temp*0.1;
	if(Str_Add_temp[i] == '.')
		i++ ;
	
	for(; Str_Add_temp[i] != ',' ; i++)
	{
		Float_Float_temp = (Float_Float_temp + (Str_Add_temp[i] - '0'))*10;
		j++;
	}
	
	for(j++;j>0;j--)
	{
		Float_Float_temp = Float_Float_temp*0.1;
	}
	
	if(Str_Add_temp[0] == '-')
		return -(Float_Int_temp + Float_Float_temp);
	else
		return (Float_Int_temp + Float_Float_temp);
}


BOOL RTK_Pos2Bool(const u8* Str_Add)
{
	const u8* Str_Add_temp = Str_Add;
	if(*Str_Add_temp == 'A')
		return True;
	else
		return False;
}

void RTK_Extract_RmcData(const u8* BUF)
{
	const u8* Str_Add_temp = BUF;
	u8 i = 0;
	
	while(Str_Add_temp[i++] != ','){}
	RTK_RMC.UTC = RTK_Str2Float(&Str_Add_temp[i]);		
	while(Str_Add_temp[i++] !=  ','){}
	RTK_RMC.IsPosFix  = RTK_Pos2Bool(&Str_Add_temp[i]);
	while(Str_Add_temp[i++] !=  ','){}
	RTK_RMC.Lat = RTK_Str2Float(&Str_Add_temp[i]);
	while(Str_Add_temp[i++] !=  ','){}
	RTK_RMC.LatDir = Str_Add_temp[i];		
	while(Str_Add_temp[i++] !=  ','){}
	RTK_RMC.Lon = RTK_Str2Float(&Str_Add_temp[i]);	
	while(Str_Add_temp[i++] !=  ','){}
	RTK_RMC.LonDir = Str_Add_temp[i];		
	while(Str_Add_temp[i++] !=  ','){}
	RTK_RMC.Speed = RTK_Str2Float(&Str_Add_temp[i]);	
	while(Str_Add_temp[i++] !=  ','){}
	RTK_RMC.TrackAngle = RTK_Str2Float(&Str_Add_temp[i]);	
	while(Str_Add_temp[i++] !=  ','){}
	RTK_RMC.Date = (u32)RTK_Str2Float(&Str_Add_temp[i]);
	while(Str_Add_temp[i++] !=  ','){}
	RTK_RMC.MagAngle = RTK_Str2Float(&Str_Add_temp[i]);
	while(Str_Add_temp[i++] !=  ','){}
	RTK_RMC.MagDir = Str_Add_temp[i];
	while(Str_Add_temp[i++] !=  ','){}
	RTK_RMC.ModeInd = Str_Add_temp[i];
}
void RTK_Extract_GgaData(const u8* BUF)
{
	const u8* Str_Add_temp = BUF;
	u8 i = 0;
	
	while(Str_Add_temp[i++] != ','){}
	RTK_GGA.UTC = RTK_Str2Float(&Str_Add_temp[i]);		
		
	while(Str_Add_temp[i++] !=  ','){}
	RTK_GGA.Lat = RTK_Str2Float(&Str_Add_temp[i]);
		
	while(Str_Add_temp[i++] !=  ','){}
	RTK_GGA.LatDir = Str_Add_temp[i];		
		
	while(Str_Add_temp[i++] !=  ','){}
	RTK_GGA.Lon = RTK_Str2Float(&Str_Add_temp[i]);	
		 
	while(Str_Add_temp[i++] !=  ','){}
	RTK_GGA.LonDir = Str_Add_temp[i];		
		
	while(Str_Add_temp[i++] !=  ','){}
	RTK_GGA.Quality = Str_Add_temp[i];
				
	while(Str_Add_temp[i++] !=  ','){}
	RTK_GGA.Sats = (u8)RTK_Str2Float(&Str_Add_temp[i]);
		
	while(Str_Add_temp[i++] !=  ','){}
	RTK_GGA.Hdop = RTK_Str2Float(&Str_Add_temp[i]);	
		
	while(Str_Add_temp[i++] !=  ','){}
	RTK_GGA.Alt = RTK_Str2Float(&Str_Add_temp[i]);	
		
	while(Str_Add_temp[i++] !=  ','){}
	RTK_GGA.AUnits = Str_Add_temp[i];

	while(Str_Add_temp[i++] !=  ','){}
	RTK_GGA.Undulation = RTK_Str2Float(&Str_Add_temp[i]);

	while(Str_Add_temp[i++] !=  ','){}
	RTK_GGA.UUnits = Str_Add_temp[i];	
		
	while(Str_Add_temp[i++] !=  ','){}
	RTK_GGA.Age = (u8)RTK_Str2Float(&Str_Add_temp[i]);	
		
	while(Str_Add_temp[i++] !=  ','){}
	if(Str_Add_temp[i] != '*')
	{
		RTK_GGA.StnID[0] = Str_Add_temp[i++];	
		RTK_GGA.StnID[1] = Str_Add_temp[i++];
		RTK_GGA.StnID[2] = Str_Add_temp[i++];	
		RTK_GGA.StnID[3] = Str_Add_temp[i++];	
	}
}

BOOL RTK_Read_GPRMC(void)
{
	u8 BUF[128] = "GPRMC,125030.80,A,3853.1375121,N,12131.5780702,E,0.034,105.5,100117,0.0,E,D*3D";
	u8 Sum = 0;
	u8 Length = 0;
	
	RTK_Find_Head("$GPRMC");
	Length = RTK_Read_Frame("GPRMC",BUF);

	for(u8 i = 0;i < Length - 5;i++)
	{
		Sum ^= BUF[i];
	}	

	Sum -= (u8)(BUF[Length - 4] << 4);
	Sum -= (u8)(BUF[Length - 3] & 0X0F);
	
	if(1)
	{
		RTK_Extract_RmcData(BUF);
		return True;
	}
	return False;
}

BOOL RTK_Read_GPGGA(void)
{
	u8 BUF[128] = "GPGGA,125030.80,3853.1375121,N,12131.5780702,E,4,22,0.7,26.043,M,8.02,M,00,0001*5B";
	u8 Sum = 0;
	u8 Length = 0;
	
	RTK_Find_Head("$GPGGA");
	Length = RTK_Read_Frame("GPGGA",BUF);

	for(u8 i = 0;i < Length - 5;i++)
	{
		Sum ^= BUF[i];
	}	

	Sum -= (u8)(BUF[Length - 4] << 4);
	Sum -= (u8)(BUF[Length - 3] & 0X0F);
	
	if(1)
	{
		RTK_Extract_GgaData(BUF);
		return True;
	}
	return False;
}

/*
		���һ��A�ľ� γ��Ϊ(LonA, LatA)���ڶ���B�ľ�γ��Ϊ(LonB, LatB)��
		����0�Ⱦ��ߵĻ�׼������ȡ���ȵ���ֵ(Longitude)������ȡ���ȸ�ֵ(-Longitude)��
		��γȡ90-γ��ֵ(90- Latitude)����γȡ90+γ��ֵ(90+Latitude)��
		�򾭹����������������㱻��Ϊ(MLonA, MLatA)��(MLonB, MLatB)��
		��ô���������Ƶ������Եõ����������������¹�ʽ��
		C = sin(MLatA)*sin(MLatB)*cos(MLonA-MLonB) + cos(MLatA)*cos(MLatB)
		Distance = R*Arccos(C)*Pi/180

		�ȷ�ת����
		���ȷֵ�λ����ת��Ϊ�ȵ�λ���� 
		��=��+��/60
		���磺
		���� = 116��20.12��
		γ�� = 39��12.34�� 
		���� = 116 + 20.12 / 60 = 116.33533�� 
		γ�� = 39 + 12.34 / 60 = 39.20567��

		������ʽ
		l = n��Բ�Ľǣ��� �У�Բ���ʣ��� r���뾶��/180=��(Բ�Ľǻ�����)�� r���뾶��
*/
void RTK_Unit_Convert(void)
{
	#define EARTH_RADIUS 6371004
	#define PI 3.141592653
	#define LAT0 0
	#define LON0 0
	double  Lon_Deg = 0 ,Lat_Deg = 0;
	int Lat_Int_Temp = 0, Lon_Int_Temp = 0;
	
	Lon_Int_Temp = (int)RTK_GGA.Lon/100; 
	Lon_Deg = Lon_Int_Temp + (RTK_GGA.Lon - Lon_Int_Temp*100)/60;
	
	Lat_Int_Temp = (int)RTK_GGA.Lat/100; 
	Lat_Deg = Lat_Int_Temp + (RTK_GGA.Lat - Lat_Int_Temp*100)/60;
	
	if(RTK_GGA.LonDir == 'W')
		Lon_Deg = -Lon_Deg;
	if(RTK_GGA.Lat == 'S')
		Lat_Deg = -Lat_Deg;
	
	RTK_GPS.Lon_M = (Lon_Deg*PI/180)*EARTH_RADIUS;		//���� ��λ��	GGA
	RTK_GPS.Lat_M = (Lat_Deg*PI/180)*EARTH_RADIUS;		//γ�� ��λ��	GGA

	RTK_GPS.Alt_M = RTK_GGA.Alt; 											//��Ժ�ƽ��߶� ��λ��	GGA
	
	if(RTK_RMC.MagDir == 'W')
		RTK_GPS.TrackAngle = -RTK_RMC.TrackAngle;						//������ ��λ��	RMC
	else if(RTK_RMC.MagDir == 'E')
		RTK_GPS.TrackAngle = RTK_RMC.TrackAngle;
	
	RTK_GPS.Quality = RTK_GGA.Quality-'0';						//��λ����	GGA
	RTK_GPS.Speed_M = RTK_RMC.Speed;									//�ƶ��ٶ� ��λ��	RMC
}

void RTK_Init(void)
{

}


BOOL RTK_Read(u8 Cmd)
{
  RTK_Read_GPRMC();
	RTK_Read_GPGGA();
	RTK_Unit_Convert();
	return True;
}

