#include "USART.h"

void USART1_Init(u32 Baud,u8 Priority);
u8 USART1_Send(u8* BUF,u8 Cnt);
u8 USART1_Recv(u8* BUF,u8 Offset,u8 Cnt,BOOL IsWait);

struct USART_ Usart1 = 
{
	USART1_Init,
	USART1_Send,
	USART1_Recv,
};

#define BUF_SIZE 32


OS_EVENT* USART1_IsSendNoFull;
OS_EVENT* USART1_IsRecvNoEmpty;

u8 USART1_TX_BUF[BUF_SIZE];
u8 USART1_RX_BUF[BUF_SIZE];


Queue_ USART1_QueueSend;
Queue_ USART1_QueueRecv;
void USART1_Init(u32 Baud,u8 Priority)
{
	//GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
 
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//USART1_NVIC 
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = Priority;		

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	

	USART_InitStructure.USART_BaudRate = Baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//USART_HardwareFlowControl_CTS
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);                   

	USART1_IsSendNoFull = OSSemCreate (0);
	USART1_IsRecvNoEmpty = OSSemCreate (0);
	
	USART1_QueueSend = Queue_OPS.Init(BUF_SIZE,USART1_TX_BUF);
	USART1_QueueRecv = Queue_OPS.Init(BUF_SIZE,USART1_RX_BUF);
}
// 写缓存,缓存满会阻塞
u8 USART1_Send(u8* BUF,u8 Cnt)
{
	u8 Err; 
	u8 i = 0;

	for(i = 0;i < Cnt;i++)
	{
		if(USART1_QueueSend.IsFull == True)
		{
			USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
			OSSemPend(USART1_IsSendNoFull,0,&Err); //请求信号量
			i--;
		}
		else
		{
			Queue_OPS.Enqueue(BUF[i],&USART1_QueueSend); //非满，写缓存
		}
	}
		
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

	return i;
}
// 读缓存,缓存空会阻塞
u8 USART1_Recv(u8* BUF,u8 Offset,u8 Cnt,BOOL IsWait)
{
	u8 Err; 
	u8 i = 0;
	
	if((IsWait == True) && (USART1_QueueRecv.IsEmpty == True)) OSSemPend(USART1_IsRecvNoEmpty,0,&Err); //请求信号量

	for(i = Offset;i < Cnt;i++)
	{
		if(Queue_OPS.Dequeue(&BUF[i],&USART1_QueueRecv) == False)
		{
			if(IsWait == True)
				OSSemPend(USART1_IsRecvNoEmpty,0,&Err); //请求信号量
			else
				break;
		}			
	}		

	return i;
}


void USART1_IRQHandler(void)                
{

	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();                          
	OSIntNesting++;
	OS_EXIT_CRITICAL();
	
	if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)  
	{  
		USART_ReceiveData(USART1);   
	}  
	//send
	if(USART_GetITStatus(USART1,USART_IT_TXE) == SET)
	{
		u8 Tmp = 0;
		if(USART1_QueueSend.IsFull == True)
		{
			Queue_OPS.Dequeue(&Tmp,&USART1_QueueSend);
			OSSemPost(USART1_IsSendNoFull);			//发出信号量
			USART_SendData(USART1,Tmp);
		}
		else
		{
			if(Queue_OPS.Dequeue(&Tmp,&USART1_QueueSend) == True)
				USART_SendData(USART1,Tmp);
			else
				USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}
	
	}
	//receive
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{	
		u8 Tmp = USART_ReceiveData(USART1);

		if(USART1_QueueRecv.IsEmpty == True) OSSemPost(USART1_IsRecvNoEmpty);			//发出信号量
		Queue_OPS.Enqueue(Tmp,&USART1_QueueRecv);
	} 	
	
	OSIntExit(); 
}

int fputc(int ch,FILE* f)
{
	USART_SendData(USART1,(u8)ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	  
	return ch;
}


