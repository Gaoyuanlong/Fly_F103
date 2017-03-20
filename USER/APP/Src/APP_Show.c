#include "APP.h"

void Show_UI(void);

void Task_Show(void *p_arg)
{
	(void)p_arg;
	
	while(1)
	{	
		Show_UI();
		OSTimeDly(500);
	}
}

char Show_Test = 0;
void Show_UI(void)
{
	float X = 0.5,Y = 0;
	Point POS = { X,Y};
	
	Show_Test++;
	if(Show_Test > 100)
		Show_Test = 0;


	POS.X = X;POS.Y = Y;Y++;
	TFT.FillFloat(POS,RTK_XYZ_HP.Heading,WHITE,BLACK);	
	POS.X = X;POS.Y = Y;Y++;
	TFT.FillFloat(POS,RTK_XYZ_HP.Pitch ,WHITE,BLACK);	
	POS.X = X;POS.Y = Y;Y++;
	TFT.FillFloat(POS,RTK_XYZ_HP.PX ,WHITE,BLACK);	
	POS.X = X;POS.Y = Y;Y++;
	TFT.FillFloat(POS,RTK_XYZ_HP.PY ,WHITE,BLACK);
	POS.X = X;POS.Y = Y;Y++;
	TFT.FillFloat(POS,RTK_XYZ_HP.PZ ,WHITE,BLACK);		
	POS.X = X;POS.Y = Y;Y++;
	TFT.FillFloat(POS,RTK_XYZ_HP.VX ,WHITE,BLACK);	
	POS.X = X;POS.Y = Y;Y++;
	TFT.FillChar(POS,RTK_XYZ_HP.Quality + '0' ,WHITE,BLACK);	
	POS.X = X;POS.Y = Y;Y++;
//	TFT.FillFloat(POS,RTK_XYZ.VZ ,WHITE,BLACK);	
//	POS.X = X;POS.Y = Y;Y++;
	TFT.FillNumber(POS,Show_Test,WHITE,BLACK);
	
//	POS.X = X;POS.Y = Y;Y++;
//	TFT.FillFloat(POS,RTK_GGA.UTC,WHITE,BLACK);	
//	POS.X = X;POS.Y = Y;Y++;
//	TFT.FillFloat(POS,RTK_GGA.Lat ,WHITE,BLACK);	
//	POS.X = X;POS.Y = Y;Y++;
//	TFT.FillFloat(POS,RTK_GGA.Lon ,WHITE,BLACK);	
//	POS.X = X;POS.Y = Y;Y++;
//	TFT.FillFloat(POS,RTK_RMC.Lat ,WHITE,BLACK);
//	POS.X = X;POS.Y = Y;Y++;
//	TFT.FillFloat(POS,RTK_RMC.Lon ,WHITE,BLACK);		
//	POS.X = X;POS.Y = Y;Y++;
//	TFT.FillFloat(POS,RTK_RMC.Speed ,WHITE,BLACK);	
//	POS.X = X;POS.Y = Y;Y++;
//	TFT.FillChar(POS,RTK_RMC.ModeInd ,WHITE,BLACK);	
//	POS.X = X;POS.Y = Y;Y++;
//	TFT.FillNumber(POS,Show_Test,WHITE,BLACK);
}
