#include "USART.h"

void USART2_Init(u32 Baud,u8 Priority);
u8 USART2_Send(u8* BUF,u8 Cnt);
u8 USART2_Recv(u8* BUF,u8 Offset,u8 Cnt,BOOL IsWait);

struct USART_ Usart2 = 
{
	USART2_Init,
	USART2_Send,
	USART2_Recv,
};

#define BUF_SIZE 240


OS_EVENT* USART2_IsSendNoFull;
OS_EVENT* USART2_IsRecvNoEmpty;

u8 USART2_TX_BUF[BUF_SIZE];
u8 USART2_RX_BUF[BUF_SIZE];


Queue_ USART2_QueueSend;
Queue_ USART2_QueueRecv;
void USART2_Init(u32 Baud,u8 Priority)
{
	//GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
 
	//USART2_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART2_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//USART2_NVIC 
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
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

	USART_Init(USART2, &USART_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART2, ENABLE);                   

	USART2_IsSendNoFull = OSSemCreate (0);
	USART2_IsRecvNoEmpty = OSSemCreate (0);
	
	USART2_QueueSend = Queue_OPS.Init(BUF_SIZE,USART2_TX_BUF);
	USART2_QueueRecv = Queue_OPS.Init(BUF_SIZE,USART2_RX_BUF);
}
// 写缓存,缓存满会阻塞
u8 USART2_Send(u8* BUF,u8 Cnt)
{
	u8 Err; 
	u8 i = 0;

	for(i = 0;i < Cnt;i++)
	{
		if(USART2_QueueSend.IsFull == True)
		{
			USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
			OSSemPend(USART2_IsSendNoFull,0,&Err); //请求信号量
			i--;
		}
		else
		{
			Queue_OPS.Enqueue(BUF[i],&USART2_QueueSend); //非满，写缓存
		}
	}
		
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

	return i;
}
// 读缓存,缓存空会阻塞
u8 USART2_Recv(u8* BUF,u8 Offset,u8 Cnt,BOOL IsWait)
{
	u8 Err; 
	u8 i = 0;
	
	if((IsWait == True) && (USART2_QueueRecv.IsEmpty == True)) OSSemPend(USART2_IsRecvNoEmpty,0,&Err); //请求信号量

	for(i = Offset;i < Cnt;i++)
	{
		if(Queue_OPS.Dequeue(&BUF[i],&USART2_QueueRecv) == False)
		{
			if(IsWait == True)
				OSSemPend(USART2_IsRecvNoEmpty,0,&Err); //请求信号量
			else
				break;
		}
	}		

	return i;
}


void USART2_IRQHandler(void)                
{

	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();                          
	OSIntNesting++;
	OS_EXIT_CRITICAL();
	
	if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)  
	{  
		USART_ReceiveData(USART2);   
	}  
	//send
	if(USART_GetITStatus(USART2,USART_IT_TXE) == SET)
	{
		u8 Tmp = 0;
		if(USART2_QueueSend.IsFull == True)
		{
			Queue_OPS.Dequeue(&Tmp,&USART2_QueueSend);
			OSSemPost(USART2_IsSendNoFull);			//发出信号量
			USART_SendData(USART2,Tmp);
		}
		else
		{
			if(Queue_OPS.Dequeue(&Tmp,&USART2_QueueSend) == True)
				USART_SendData(USART2,Tmp);
			else
				USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		}
	
	}
	//receive
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{	
		u8 Tmp = USART_ReceiveData(USART2);

		if(USART2_QueueRecv.IsEmpty == True) OSSemPost(USART2_IsRecvNoEmpty);			//发出信号量
		Queue_OPS.Enqueue(Tmp,&USART2_QueueRecv);
	} 	
	
	OSIntExit(); 
}

