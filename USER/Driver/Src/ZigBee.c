#include "ZigBee.h"

void ZigBee_Init(ZigBee_* Aim);
BOOL ZigBee_Ctrl(u8 Cmd,BOOL IsWait,ZigBee_* Aim);
u8 ZigBee_Send(u8* BUF,u8 Cnt,ZigBee_* Aim);
u8 ZigBee_Recv(u8* BUF,u8 Offset,u8 Cnt,BOOL IsWait,ZigBee_* Aim);

ZigBee_OPS_ ZigBee_OPS =
{
	ZigBee_Init,
	ZigBee_Ctrl,
	ZigBee_Send,
	ZigBee_Recv,

};


const u8 ZigBee_Cmd[][6] = 
{
	{0XFC,0X00,0X91,0X03,0XA3,0XB3},//读取PAN ID
	{0XFC,0X00,0X91,0X04,0XC4,0XD4},//读取短地址
	{0XFC,0X00,0X91,0X07,0X97,0XA7},//读取波特率	
	{0XFC,0X00,0X91,0X08,0XA8,0XB8},//读取MAC地址
	{0XFC,0X00,0X91,0X0B,0XCB,0XEB},//读取节点类型
	{0XFC,0X00,0X91,0X0D,0X34,0X2B},//读取无线频道	
	{0XFC,0X33,0XD4,0XA1,0XA2,0X01},//读取路由地址	
	{0XFC,0X00,0X92,0XA1,0XB3,0X7D},//读取信号强度

	{0XFC,0X00,0X91,0X87,0X6A,0X35},//设置软件重启
	{0XFC,0X00,0X91,0X09,0XA9,0XC9},//设置为协调器
	{0XFC,0X00,0X91,0X0A,0XBA,0XDA},//设置为路由器
	{0XFC,0X01,0X91,0X8A,0X9D,0X00},//设置禁止加入	
	{0XFC,0X01,0X91,0X1A,0X27,0X00},//设置指令失效	
	
	{0XFC,0X02,0X91,0X01,0X00,0X00},//写入PAN ID
	{0XFC,0X00,0X91,0X06,0X00,0XF6},//写入波特率
	{0XFC,0X01,0X91,0X0C,0X00,0X1A},//写入无线频道	
	{0XFC,0X01,0X91,0X64,0X58,0X00},//写入传输方式
	{0XFC,0X32,0XC3,0X00,0X00,0X01},//写入路由地址
	{0XFC,0X01,0X91,0X9E,0X46,0X00},//写入串口校验


};



BOOL ZigBee_Ctrl(u8 Cmd,BOOL IsWait,ZigBee_* Aim)
{
	u8 BUF[7];
	u8 Sum = 0;
	BOOL Ret = False;;
	
	for(u8 i = 0;i < 6;i++)
	{
		BUF[i] = ZigBee_Cmd[Cmd][i];
		Sum += ZigBee_Cmd[Cmd][i];
	}
	BUF[6] = Sum;
	
	switch (Cmd)
	{
		case READ_PAN_ID:
			
			Aim->Usart->Send(BUF,7);
			OSTimeDly(50);
			Aim->Usart->Recv(BUF,0,2,True);
			Aim->PAN_ID = BUF[0] << 8 | BUF[1];
			break;
		
		case READ_SHORT_ADD:
			
			Aim->Usart->Send(BUF,7);
			OSTimeDly(50);
			Aim->Usart->Recv(BUF,0,2,True);	
			Aim->Short_ADD = BUF[0] << 8 | BUF[1];
			break;
		
		case READ_NODE_TYPE:
			
			Aim->Usart->Send(BUF,7);
			OSTimeDly(50);
			Aim->Usart->Recv(BUF,0,6,True);	
		
			if(BUF[0] == NODE_ROUTER) Aim->Node_Type = NODE_ROUTER;
			else Aim->Node_Type = NODE_CODTER;

			break;		
		case READ_CHN_NUM:
			
			Aim->Usart->Send(BUF,7);
			OSTimeDly(50);
			Aim->Usart->Recv(BUF,0,6,True);	
			Aim->CHN_NUM = (BUF[5] - 10) * 5 + 2400;
		
			break;
		case READ_ROUTER_ADD:
		
			Aim->Usart->Send(BUF,7);
			OSTimeDly(50);
			Aim->Usart->Recv(BUF,0,2,True);			
		
			Aim->Router_ADD = BUF[0] << 8 | BUF[1];
			break;
		case READ_SIG_LEVEL:
			
			Aim->Usart->Send(BUF,7);
			OSTimeDly(50);
			Aim->Usart->Recv(BUF,0,4,True);	
		
			Aim->SIG_LEVEL = BUF[2];
			break;
		default:
			break;
	}
				
	return Ret;
}
u8 ZigBee_Send(u8* BUF,u8 Cnt,ZigBee_* Aim)
{
	return Aim->Usart->Send(BUF,Cnt);
}
u8 ZigBee_Recv(u8* BUF,u8 Offset,u8 Cnt,BOOL IsWait,ZigBee_* Aim)
{
	return Aim->Usart->Recv(BUF,Offset,Cnt,IsWait);
}



void ZigBee_Init(ZigBee_* Aim)
{

	ZigBee_Ctrl(READ_PAN_ID     ,False,Aim);
	ZigBee_Ctrl(READ_SHORT_ADD  ,False,Aim);
	ZigBee_Ctrl(READ_NODE_TYPE  ,False,Aim);
	ZigBee_Ctrl(READ_CHN_NUM    ,False,Aim);	
	ZigBee_Ctrl(READ_ROUTER_ADD ,False,Aim);	
	ZigBee_Ctrl(READ_SIG_LEVEL  ,False,Aim);	
	
}




