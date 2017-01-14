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
	{0XFC,0X00,0X91,0X03,0XA3,0XB3},//��ȡPAN ID
	{0XFC,0X00,0X91,0X04,0XC4,0XD4},//��ȡ�̵�ַ
	{0XFC,0X00,0X91,0X07,0X97,0XA7},//��ȡ������	
	{0XFC,0X00,0X91,0X08,0XA8,0XB8},//��ȡMAC��ַ
	{0XFC,0X00,0X91,0X0B,0XCB,0XEB},//��ȡ�ڵ�����
	{0XFC,0X00,0X91,0X0D,0X34,0X2B},//��ȡ����Ƶ��	
	{0XFC,0X33,0XD4,0XA1,0XA2,0X01},//��ȡ·�ɵ�ַ	
	{0XFC,0X00,0X92,0XA1,0XB3,0X7D},//��ȡ�ź�ǿ��

	{0XFC,0X00,0X91,0X87,0X6A,0X35},//�����������
	{0XFC,0X00,0X91,0X09,0XA9,0XC9},//����ΪЭ����
	{0XFC,0X00,0X91,0X0A,0XBA,0XDA},//����Ϊ·����
	{0XFC,0X01,0X91,0X8A,0X9D,0X00},//���ý�ֹ����	
	{0XFC,0X01,0X91,0X1A,0X27,0X00},//����ָ��ʧЧ	
	
	{0XFC,0X02,0X91,0X01,0X00,0X00},//д��PAN ID
	{0XFC,0X00,0X91,0X06,0X00,0XF6},//д�벨����
	{0XFC,0X01,0X91,0X0C,0X00,0X1A},//д������Ƶ��	
	{0XFC,0X01,0X91,0X64,0X58,0X00},//д�봫�䷽ʽ
	{0XFC,0X32,0XC3,0X00,0X00,0X01},//д��·�ɵ�ַ
	{0XFC,0X01,0X91,0X9E,0X46,0X00},//д�봮��У��


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




