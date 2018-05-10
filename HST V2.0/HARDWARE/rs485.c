#include "sys.h"
#include "delay.h"
#include "rs485.h"

u8 USART1_RX_Buf[USART1_RX_LEN];  //���ջ���
u8 USART1_TX_Buf[USART1_TX_LEN];  //���ͻ���
u8 USART1_RX_Data_Len = 0;        //ʵ�ʽ��������ֽڳ���
u8 USART1_TX_Data_Len = 0;        //�����������ֽڳ���
u8 USART1_RX_Flag = 0;            //����1�Ƿ����������	  

void USART1_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //���յ�����
	{
		res =USART_ReceiveData(USART1); //��ȡ���յ�������
		if(USART1_RX_Data_Len<USART1_RX_LEN)
		{
			USART1_RX_Buf[USART1_RX_Data_Len]=res; //��¼���յ���ֵ
			USART1_RX_Data_Len++; //������������ 1
		} 				
	    USART1_RX_Flag=1;   //����1���յ�����
	}
}

void RS485_Config(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/*********************���ô���2**************************/
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  , ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.02) as alternate function push-pull *///TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.03) as input floating *///RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
	
	/* USART1 mode config */
	
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	/*  USART1 �����ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //ʹ�ܴ��� 2 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ� 3 ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //�����ȼ� 2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//��ʼ�� NVIC �Ĵ���
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //�����ж�
	USART_Cmd(USART1, ENABLE); //ʹ�ܴ���
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);//�巢����ɱ�־
    
	
	/**********************����485���ƿ�*********************/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //������� ,PA4,485Ctr
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);        //����Ϊ����ģʽ��Ĭ�Ͻ���
	
}

void RS485_Send_Data(u8 *buf,u8 len)       //�������Ϊ����
{
	u8 t;
	RS485_TX_EN=1;//����Ϊ����ģʽ
	for(t=0;t<len;t++)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_SendData(USART1,buf[t]);
	}
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	
	USART1_RX_Data_Len=0;
	RS485_TX_EN=0;//����Ϊ����ģʽ	
}

