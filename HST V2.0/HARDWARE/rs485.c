#include "sys.h"
#include "delay.h"
#include "rs485.h"

u8 USART1_RX_Buf[USART1_RX_LEN];  //接收缓冲
u8 USART1_TX_Buf[USART1_TX_LEN];  //发送缓冲
u8 USART1_RX_Data_Len = 0;        //实际接收数据字节长度
u8 USART1_TX_Data_Len = 0;        //待发送数据字节长度
u8 USART1_RX_Flag = 0;            //串口1是否接收完数据	  

void USART1_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收到数据
	{
		res =USART_ReceiveData(USART1); //读取接收到的数据
		if(USART1_RX_Data_Len<USART1_RX_LEN)
		{
			USART1_RX_Buf[USART1_RX_Data_Len]=res; //记录接收到的值
			USART1_RX_Data_Len++; //接收数据增加 1
		} 				
	    USART1_RX_Flag=1;   //串口1接收到数据
	}
}

void RS485_Config(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/*********************配置串口2**************************/
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  , ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.02) as alternate function push-pull *///TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.03) as input floating *///RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
	
	/* USART1 mode config */
	
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	/*  USART1 接收中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //使能串口 2 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //先占优先级 3 级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //从优先级 2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//初始化 NVIC 寄存器
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启中断
	USART_Cmd(USART1, ENABLE); //使能串口
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);//清发送完成标志
    
	
	/**********************配置485控制口*********************/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出 ,PA4,485Ctr
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);        //设置为接收模式，默认接收
	
}

void RS485_Send_Data(u8 *buf,u8 len)       //发送完改为接收
{
	u8 t;
	RS485_TX_EN=1;//设置为发送模式
	for(t=0;t<len;t++)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_SendData(USART1,buf[t]);
	}
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	
	USART1_RX_Data_Len=0;
	RS485_TX_EN=0;//设置为接收模式	
}

