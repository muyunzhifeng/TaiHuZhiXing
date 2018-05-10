#include "sys.h"
#include "stdio.h"

#define		USART1_RX_LEN      50           //��������ֽ�
#define		USART1_TX_LEN      50           //��������ֽ�
#define     RS485_TX_EN        PAout(4)


extern  u8 USART1_RX_Buf[USART1_RX_LEN];  //���ջ���
extern  u8 USART1_TX_Buf[USART1_TX_LEN];  //���ͻ���
extern  u8 USART1_RX_Data_Len;           //ʵ�ʽ��������ֽڳ���
extern  u8 USART1_TX_Data_Len;           //�����������ֽڳ���
extern  u8 USART1_RX_Flag;               //�Ƿ��յ�����

void RS485_Config(u32 bound);
void USART1_IRQHandler(void);
void RS485_Send_Data(u8 *buf,u8 len);

