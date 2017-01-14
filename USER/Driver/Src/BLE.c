#include "BLE.h"


void BLE_Init(BLE_* Aim);
BOOL BLE_Ctrl(u8 Cmd,BOOL IsWait,BLE_* Aim);
BOOL BLE_Send(u8* BUF,u8 Cnt,BLE_* Aim);
BOOL BLE_Receive(u8* BUF,u8 Offset,u8 Cnt,BOOL IsWait,BLE_* Aim);

BLE_OPS_ BLE_OPS = 
{
	BLE_Init,
	BLE_Ctrl,
	BLE_Send,
	BLE_Receive
};

void BLE_Init(BLE_* Aim)
{

}

BOOL Strcmp(const char* S1,const char* S2)
{

	return True;
}	

BOOL BLE_Check(const char* S1,BOOL IsWait,BLE_* Aim)
{

	u8 StrLength = strlen(S1);
	
	if(IsWait == False)
	{
		u8 Tmp[24];
		u8 Cnt = 0;
		Tmp[23] = '\0';
		if(StrLength > 24) return False;
		Cnt = Aim->Usart->Recv(Tmp,Cnt,StrLength,False);
		if(Cnt < StrLength)
		{
			OSTimeDly(200);
			Cnt = Aim->Usart->Recv(Tmp,Cnt,StrLength,False);
		}
		if(Cnt < StrLength)
		{
			return False;
		}
		else
		{
			if(strstr((const char*)Tmp,S1) == NULL)
			{
				return False;
			}
			else
			{
				return True;
			}
			
		}			
	}
	else
	{
		u8 Tmp = 0;
		const char* P = S1;
		while(StrLength > 0)
		{
			Aim->Usart->Recv(&Tmp,0,1,True);
			if(Tmp == *P) 
			{
				P++;
				StrLength--;
			}
			else
			{
				P = S1;
				StrLength = strlen(S1);
			}
		}

	return True;
	}
	
	
}

BOOL BLE_Ctrl(u8 Cmd,BOOL IsWait,BLE_* Aim)
{
	BOOL Ret = False;
	switch (Cmd)
	{
		case BLE_WAIT_CONNECT:
			Ret = BLE_Check("AT-AB ConnectionUp",IsWait,Aim);
			break;
		case BLE_CONNECT_DOWN:
			Ret = BLE_Check("AT-AB ConnectionDown",IsWait,Aim);
			break;		
		case BLE_ERR_FORMAT:
			Ret = BLE_Check("AT-AB ErrFormat",IsWait,Aim);
			break;
		case BLE_RESET:
			printf("AT+AB Reset\r\n");
			Ret = BLE_Check("AT-AB ResetPending",IsWait,Aim);
			break;		
		case BLE_CFG_BAUD_115200:
			printf("at+ab config Var07 = 115200\r\n");
			Ret = BLE_Check("AT-AB ResetPending",IsWait,Aim);
			break;
		case BLE_CFG_BAUD_921600:
			printf("at+ab config Var07 = 921600\r\n");	
			Ret = BLE_Check("AT-AB ConfigOk",IsWait,Aim);
			break;
		case BLE_CFG_BAUD_460800:
			printf("at+ab config Var07 = 460800\r\n");	
			Ret = BLE_Check("AT-AB ConfigOk",IsWait,Aim);
			break;		
		case BLE_CFG_RTS_CTS_EN:
			printf("at+ab config Var05 = False\r\n");
			Ret = BLE_Check("AT-AB ConfigOk",IsWait,Aim);
			break;
		case BLE_CFG_CPU_32_MHZ:
			printf("at+ab config Var24 = 32\r\n");
			Ret = BLE_Check("AT-AB ConfigOk",IsWait,Aim);
			break;
		case BLE_CFG_CPU_64_MHZ:
			printf("at+ab config Var24 = 64\r\n");
			Ret = BLE_Check("AT-AB ConfigOk",IsWait,Aim);
			break;
		case BLE_CFG_HCIB_921600:
			printf("at+ab config Var26 = 921600\r\n");
			Ret = BLE_Check("AT-AB ConfigOk",IsWait,Aim);
			break;
		case BLE_CFG_HCIB_2MHZ:
			printf("at+ab config Var26 = 2000000\r\n");
			Ret = BLE_Check("AT-AB ConfigOk",IsWait,Aim);
			break;
		default:
			break;
	}

	
	return Ret;
}

BOOL BLE_Receive(u8* BUF,u8 Offset,u8 Cnt,BOOL IsWait,BLE_* Aim)
{
	Aim->Usart->Recv(BUF,Offset,Cnt,IsWait);
	return True;
}

BOOL BLE_Send(u8* BUF,u8 Cnt,BLE_* Aim)
{
	Aim->Usart->Send(BUF,Cnt);
	return True;
}
