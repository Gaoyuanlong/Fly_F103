#ifndef __PARA_H__
#define __PARA_H__
#include "stm32f10x.h"
#include "Common.h"
#include "ff.h"
#include <stdio.h>
#include <string.h>
#include "RTK.h"
#include "Communicate.h"

struct Para_Data_
{	
	double Data1;
	double Data2;
	double Data3;
	double Data4;
	double Data5;
	double Data6;
	double Data7;
	double Data8;
};

extern struct Para_ 
{
	struct Para_Data_* Data;
	BOOL IsNeedSave;
	void (*Init)(void);
	BOOL (*Updata)(void);
	BOOL (*Save)(void);
	BOOL (*Read)(void);
}Para;


#endif
