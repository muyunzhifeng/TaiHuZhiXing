#include "sys.h"
#include "delay.h"
#include "timer.h"
#include "hstctr.h"

//��ʱ��2��ʱ���Ϳ�������


void TIM2_IRQHandler(void) //TIM2 �ж�
{
	u8 count=0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //��� TIM2 �����жϷ������
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update ); //��� TIM2 �����жϱ�־	
		if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_6))
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_6);
		}
		else
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_6);
		}
		
	}
}

void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //��ʱ�� TIM2 ʹ��
	//��ʱ�� TIM2 ��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���ALIENTEK
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //�ڳ�ʼ�� TIM2
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //��������ж�����λ
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //����������ж�
	//�ж����ȼ� NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //TIM2 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ� 0 ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ� 3 ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure); //�ܳ�ʼ�� NVIC �Ĵ���
	TIM_Cmd(TIM2, DISABLE); //���Ȳ�ʹ��TIM2������Ҫ��ʱ���ٿ���
}
