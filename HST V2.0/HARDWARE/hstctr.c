#include "sys.h"
#include "delay.h"
#include "hstctr.h"

float Anglepp=1.8;  //每个脉冲转多少度
float CurAngle=0;   //当前角度
int    CurDir=0;     //当前电机角度正负
int    PulseNum=0;     //应给的脉冲数

void pulse(int count)//脉冲产生函数
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

void HST_GPIO_Config(void)//HST控制口配置
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //复用推挽输出，脉冲输出控制口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //复用推挽输出，方向信号控制口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOA, GPIO_Pin_7);      //初始方向0
}
