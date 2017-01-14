#ifndef __PARA_H__
#define __PARA_H__
#include "stm32f10x.h"
#include "Common.h"
#include "ff.h"
#include <stdio.h>
#include <string.h>
#include "RTK.h"

struct Para_Data_
{	
	RTK_RMC_ Rtk;
	RTK_GGA_ Gga;
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
