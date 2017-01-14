#ifndef __APP_H__
#define __APP_H__
#include <stdio.h>
#include "LED.h"
#include "RTK.h"
#include "Queue.h"
#include "BSP.h"
#include "TFT1_44.h"
#include "SDCard.h"
#include "ff.h"	
#include "Para.h"
#include "Communicate.h"

#define TASK_INIT_PRIO      1
#define TASK_GATHER_PRIO    2
#define TASK_SHOW_PRIO      3


#define TASK_INIT_STK_SIZE  80
#define TASK_GATHER_STK_SIZE 512
#define TASK_SHOW_STK_SIZE  512

void Task_Gather(void *p_arg);
void Task_Show(void *p_arg);


#endif

