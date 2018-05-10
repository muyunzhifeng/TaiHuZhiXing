#include "sys.h"
#include "delay.h"
#include "hstctr.h"

float Anglepp=1.8;  //ÿ������ת���ٶ�
float CurAngle=0;   //��ǰ�Ƕ�
int    CurDir=0;     //��ǰ����Ƕ�����
int    PulseNum=0;     //Ӧ����������

void pulse(int count)//�����������
{
	int t;
	for(t=0;t<count;t++)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_6);
		delay_ms(25);
	    GPIO_ResetBits(GPIOA, GPIO_Pin_6);
		delay_ms(25);
	}
}

void HST_GPIO_Config(void)//HST���ƿ�����
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //�����������������������ƿ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //������������������źſ��ƿ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOA, GPIO_Pin_7);      //��ʼ����0
}
