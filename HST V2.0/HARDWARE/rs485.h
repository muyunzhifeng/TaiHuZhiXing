#include "sys.h"
#include "stdio.h"

#define		USART1_RX_LEN      50           //接收最大字节
#define		USART1_TX_LEN      50           //发送最大字节
#define     RS485_TX_EN        PAout(4)


extern  u8 USART1_RX_Buf[USART1_RX_LEN];  //接收缓冲
extern  u8 USART1_TX_Buf[USART1_TX_LEN];  //发送缓冲
extern  u8 USART1_RX_Data_Len;           //实际接收数据字节长度
extern  u8 USART1_TX_Data_Len;           //待发送数据字节长度
extern  u8 USART1_RX_Flag;               //是否收到数据

void RS485_Config(u32 bound);
void USART1_IRQHandler(void);
void RS485_Send_Data(u8 *buf,u8 len);

