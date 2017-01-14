#include "USART.h"

void USART3_Init(u32 Baud,u8 Priority);
u8 USART3_Send(u8* BUF,u8 Cnt);
u8 USART3_Recv(u8* BUF,u8 Offset,u8 Cnt,BOOL IsWait);

struct USART_ Usart3 = 
{
	USART3_Init,
	USART3_Send,
	USART3_Recv,
};

#define BUF_SIZE 128


OS_EVENT* USART3_IsSendNoFull;
OS_EVENT* USART3_IsRecvNoEmpty;

u8 USART3_TX_BUF[BUF_SIZE];
u8 USART3_RX_BUF[BUF_SIZE];

Queue_ USART3_QueueSend;
Queue_ USART3_QueueRecv;
void USART3_Init(u32 Baud,u8 Priority)
{
	//GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
 
	//USART3_TX   PB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//USART3_RX	  PB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	//USART3_NVIC 
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
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

	USART_Init(USART3, &USART_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART3, ENABLE);                   

	USART3_IsSendNoFull = OSSemCreate (0);
	USART3_IsRecvNoEmpty = OSSemCreate (0);
	
	USART3_QueueSend = Queue_OPS.Init(BUF_SIZE,USART3_TX_BUF);
	USART3_QueueRecv = Queue_OPS.Init(BUF_SIZE,USART3_RX_BUF);
}
// 写缓存,缓存满会阻塞
u8 USART3_Send(u8* BUF,u8 Cnt)
{
	u8 Err; 
	u8 i = 0;

	for(i = 0;i < Cnt;i++)
	{
		if(USART3_QueueSend.IsFull == True)
		{
			USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
			OSSemPend(USART3_IsSendNoFull,0,&Err); //请求信号量
			i--;
		}
		else
		{
			Queue_OPS.Enqueue(BUF[i],&USART3_QueueSend); //非满，写缓存
		}
	}
		
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);

	return i;
}
// 读缓存,缓存空会阻塞
u8 USART3_Recv(u8* BUF,u8 Offset,u8 Cnt,BOOL IsWait)
{
	u8 Err; 
	u8 i = 0;
	
	if((IsWait == True) && (USART3_QueueRecv.IsEmpty == True)) OSSemPend(USART3_IsRecvNoEmpty,0,&Err); //请求信号量

	for(i = Offset;i < Cnt;i++)
	{
		if(Queue_OPS.Dequeue(&BUF[i],&USART3_QueueRecv) == False)
		{
			if(IsWait == True)
				OSSemPend(USART3_IsRecvNoEmpty,0,&Err); //请求信号量
			else
				break;
		}
	}		

	return i;
}


void USART3_IRQHandler(void)                
{

	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();                          
	OSIntNesting++;
	OS_EXIT_CRITICAL();
	
	if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)  
	{  
		USART_ReceiveData(USART3);   
	}  
	//send
	if(USART_GetITStatus(USART3,USART_IT_TXE) == SET)
	{
		u8 Tmp = 0;
		if(USART3_QueueSend.IsFull == True)
		{
			Queue_OPS.Dequeue(&Tmp,&USART3_QueueSend);
			OSSemPost(USART3_IsSendNoFull);			//发出信号量
			USART_SendData(USART3,Tmp);
		}
		else
		{
			if(Queue_OPS.Dequeue(&Tmp,&USART3_QueueSend) == True)
				USART_SendData(USART3,Tmp);
			else
				USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
		}
	
	}
	//receive
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{	
		u8 Tmp = USART_ReceiveData(USART3);

		if(USART3_QueueRecv.IsEmpty == True) OSSemPost(USART3_IsRecvNoEmpty);			//发出信号量
		Queue_OPS.Enqueue(Tmp,&USART3_QueueRecv);
	} 	
	
	OSIntExit(); 
}

