#ifndef __TFT1_44_H__
#define __TFT1_44_H__
#include "stm32f10x_conf.h"
#include "time.h"
#include "Common.h"
#include <math.h>
#include <stdio.h>

#define BLACK  0X0000
#define WHITE  0XFFFF
#define BLUE   0XFF00
#define YELLOW 0X80FF
#define RED    0X00F8
#define GREEN  0XE007

//单位是坐标
typedef struct Point
{
		float X;
		float Y;
}Point;
//单位像素
typedef struct Rectanle
{	
		u16 X_LU;
		u16 Y_LU;
		u16 X_RD;
		u16 Y_RD;
}Rectanle;
struct TFT_Data
{
	char HalfWord;
	Point POS;
	u16 Color;
};
extern struct TFT_
{
	void (*Init)(u16 Level);
	void (*Light)(u16 Level);
	void (*FillString)(Point POS,const char* P,u16 Color,u16 BackGroundColor);
	void (*FillNumber)(Point POS,int Number   ,u16 Color,u16 BackGroundColor);
	void (*FillRectangle)(Rectanle RECT,BOOL IsHollow,u16 Color);
	void (*FillFloat)(Point POS,double Number ,u16 Color,u16 BackGroundColor);
	void (*FillHex)(Point POS,u16 Number,u16 Color,u16 BackGroundColor);
	void (*ClearPixel)(Rectanle RECT);
	void (*FillChar)(Point Pos,u8 Aim,u16 Color,u16 BackGroundColor);

}TFT;

#endif 

