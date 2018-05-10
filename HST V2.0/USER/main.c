#include "sys.h"
#include "delay.h"
#include "rs485.h"
#include "hstctr.h"
#include "timer.h"

u8 Send_Buf[20]={0};//ͨ�÷��ͻ���

void Uart1_Rx_Process(void);
//void Uart2_Rx_Process(void);


int main(void)
{	
	delay_init(); //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϷ���2
	HST_GPIO_Config();
    RS485_Config(115200);	//485ͨѶ�� (USART1) ��ʼ��
	
	TIM2_Int_Init(2-1,7200-1);//10KHZƵ�ʼ�����������20Ϊ2ms��TIM2��ʼδʹ��
	
	while(1)
	{		
		if(USART1_RX_Flag)//�������1�յ������ҽ������
		{
			Uart1_Rx_Process();	
			USART1_RX_Flag=0;
		}   
	}		
}

void Uart1_Rx_Process(void)
{	
	if(USART1_RX_Buf[1]==USART1_RX_Data_Len)  //������ݳ�������
	{

		switch(USART1_RX_Buf[0])	
		{
//			case 0xFE:         //�Ǹ�HST�ڵ��ַ
//			    Anglepp=360.0/USART1_RX_Buf[3];//ϸ�ֽ�
//			    if(USART1_RX_Buf[2]==CurDir)         //ͬ��Ƕ�
//				{
//					PulseNum=(USART1_RX_Buf[4]-CurAngle)/Anglepp; 
//                    if(PulseNum>0)
//				    {
//						if(!CurDir)
//						{
//							GPIO_ResetBits(GPIOA, GPIO_Pin_7);//ǰ
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
//							GPIO_ResetBits(GPIOA, GPIO_Pin_7);//ǰ
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
//						GPIO_ResetBits(GPIOA, GPIO_Pin_7);//ǰ
//					}
//					else
//				    {
//						GPIO_SetBits(GPIOA, GPIO_Pin_7);	
//					}
//				}
//				delay_us(1000);
//              CurDir=USART1_RX_Buf[2];	//��ǰ������
//			    CurAngle=USART1_RX_Buf[4];//��ǰ�Ƕ�
//			    pulse(PulseNum);
//				//USART1_Send_Data(USART1_RX_Buf,USART1_RX_Data_Len);
			case 0xFD:
				switch(USART1_RX_Buf[4])
				{
					case 0x01:
						CurDir=USART1_RX_Buf[2];	//����ת
						Anglepp=360.0/USART1_RX_Buf[3];//ϸ�ֽ�
					    if(USART1_RX_Buf[2])
							GPIO_SetBits(GPIOA, GPIO_Pin_7);
						else
							GPIO_ResetBits(GPIOA, GPIO_Pin_7);//ǰ
						TIM_SetCounter(TIM2, 0); //���TIM2����ֵ
						TIM_Cmd(TIM2, ENABLE); //������ʱ��2
					    break;
					case 0x02:
						TIM_Cmd(TIM2,DISABLE);//�رն�ʱ��2
					    break;
				}
				RS485_Send_Data(USART1_RX_Buf,USART1_RX_Data_Len);
                break;      					
			default:		
				break;	
		} 
		USART1_RX_Data_Len=0;
		RS485_TX_EN=0;//����Ϊ����ģʽ
     }
}