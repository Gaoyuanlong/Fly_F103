#ifndef __ZIGBEE_H__
#define __ZIGBEE_H__
#include "BSP.h"

#include "stdio.h"
#define NODE_ROUTER     0X52
#define NODE_CODTER     0X43

#define READ_PAN_ID     0
#define READ_SHORT_ADD  1
#define READ_BAUD       2
#define READ_MAC        3
#define READ_NODE_TYPE  4
#define READ_CHN_NUM    5
#define READ_ROUTER_ADD 6
#define READ_SIG_LEVEL  7

#define SET_RESET       8 
#define SET_AS_ROUTER   9
#define SET_AS_CODTER   10
#define SET_NO_ADD_IN   11
#define SET_CMD_LOSE    12

#define WRITE_PAN_ID    13
#define WRITE_BAUD      14
#define WRITE_CHN_NUM   15
#define WRITE_TRA_TYPE  16
#define WRITE_ROTER_ADD 17
#define WRITE_UART_CHCK 18


typedef struct ZigBee_
{
	USART_* Usart;
	u16 PAN_ID;
	u16 Short_ADD;
	u16 Node_Type;
	u16 CHN_NUM;
	u16 Router_ADD;
	u16 SIG_LEVEL;	

}ZigBee_;


typedef struct ZigBee_OPS_
{
	void (*Init)(ZigBee_* Aim);
	BOOL (*Ctrl)(u8 Cmd,BOOL IsWait,ZigBee_* Aim);
	u8 (*Send)(u8* BUF,u8 Cnt,ZigBee_* Aim);
	u8 (*Recv)(u8* BUF,u8 Offset,u8 Cnt,BOOL IsWait,ZigBee_* Aim);

}ZigBee_OPS_;

extern ZigBee_OPS_ ZigBee_OPS;

#endif

