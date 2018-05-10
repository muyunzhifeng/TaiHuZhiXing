#include "sys.h"
#include "delay.h"
#include "rs485.h"
#include "hstctr.h"
#include "timer.h"

u8 Send_Buf[20]={0};//通用发送缓存

void Uart1_Rx_Process(void);
//void Uart2_Rx_Process(void);


int main(void)
{	
	delay_init(); //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断分组2
	HST_GPIO_Config();
    RS485_Config(115200);	//485通讯口 (USART1) 初始化
	
	TIM2_Int_Init(2-1,7200-1);//10KHZ频率计数，计数到20为2ms，TIM2初始未使能
	
	while(1)
	{		
		if(USART1_RX_Flag)//如果串口1收到数据且接收完成
		{
			Uart1_Rx_Process();	
			USART1_RX_Flag=0;
		}   
	}		
}

void Uart1_Rx_Process(void)
{	
	if(USART1_RX_Buf[1]==USART1_RX_Data_Len)  //如果数据长度正常
	{

		switch(USART1_RX_Buf[0])	
		{
//			case 0xFE:         //是该HST节点地址
//			    Anglepp=360.0/USART1_RX_Buf[3];//细分角
//			    if(USART1_RX_Buf[2]==CurDir)         //同向角度
//				{
//					PulseNum=(USART1_RX_Buf[4]-CurAngle)/Anglepp; 
//                    if(PulseNum>0)
//				    {
//						if(!CurDir)
//						{
//							GPIO_ResetBits(GPIOA, GPIO_Pin_7);//前
//						}
//						else
//						{
//							GPIO_SetBits(GPIOA, GPIO_Pin_7);	
//						}					
//				
//				    }
//					else
//					{
//						if(CurDir)
//						{
//							GPIO_ResetBits(GPIOA, GPIO_Pin_7);//前
//						}
//						else
//						{
//							GPIO_SetBits(GPIOA, GPIO_Pin_7);
//						    
//						}
//						PulseNum=-PulseNum;
//					}					
//				}
//				else
//				{
//					PulseNum=(USART1_RX_Buf[4]+CurAngle)/Anglepp;
//					if(!USART1_RX_Buf[2])
//					{
//						GPIO_ResetBits(GPIOA, GPIO_Pin_7);//前
//					}
//					else
//				    {
//						GPIO_SetBits(GPIOA, GPIO_Pin_7);	
//					}
//				}
//				delay_us(1000);
//              CurDir=USART1_RX_Buf[2];	//当前角正负
//			    CurAngle=USART1_RX_Buf[4];//当前角度
//			    pulse(PulseNum);
//				//USART1_Send_Data(USART1_RX_Buf,USART1_RX_Data_Len);
			case 0xFD:
				switch(USART1_RX_Buf[4])
				{
					case 0x01:
						CurDir=USART1_RX_Buf[2];	//正反转
						Anglepp=360.0/USART1_RX_Buf[3];//细分角
					    if(USART1_RX_Buf[2])
							GPIO_SetBits(GPIOA, GPIO_Pin_7);
						else
							GPIO_ResetBits(GPIOA, GPIO_Pin_7);//前
						TIM_SetCounter(TIM2, 0); //清除TIM2计数值
						TIM_Cmd(TIM2, ENABLE); //开启定时器2
					    break;
					case 0x02:
						TIM_Cmd(TIM2,DISABLE);//关闭定时器2
					    break;
				}
				RS485_Send_Data(USART1_RX_Buf,USART1_RX_Data_Len);
                break;      					
			default:		
				break;	
		} 
		USART1_RX_Data_Len=0;
		RS485_TX_EN=0;//设置为接收模式
     }
}