#include "sys.h"


__IO uint16_t TIM3IC_ReadValue1[4]={0,0,0,0};
__IO uint16_t TIM3IC_ReadValue2[4]={0,0,0,0}; //��¼��ͨ���Ĳ���ֵ
__IO uint16_t TIM3CaptureNumber[4]={0,0,0,0} ; //�������
__IO uint32_t TIM3Capture[4]={0,0,0,0} ; //���ռ��㲶���ֵ
//__IO uint32_t TIM3Speed[4]={0,0,0,0} ; //���ղ����ֵת��Ϊ�ٶ�

__IO uint16_t TIM4IC_ReadValue1[4]={0,0,0,0};
__IO uint16_t TIM4IC_ReadValue2[4]={0,0,0,0}; //��¼��ͨ���Ĳ���ֵ
__IO uint16_t TIM4CaptureNumber[4]={0,0,0,0} ; //�������
__IO uint32_t TIM4Capture[4]={0,0,0,0}; //���ռ��㲶���ֵ
//__IO uint32_t TIM4Speed[4]={0,0,0,0} ; //���ղ����ֵת��Ϊ�ٶ�

__IO uint16_t TIM3CAPcount[4]={0,0,0,0};//��ͨ���ܹ����񵽵�ֵ

void TIM3_Cap_Init(u16 arr,u16 psc)   //TIM3��ͨ�����벶�����
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //ʹ�� TIM3 ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ�� GPIOA ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ�� GPIOB ʱ��

	//��ʼ�� GPIOA.6 ��GPIOA.7��GPIOB.0��GPIOB.1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PA6 ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA6��������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ�� GPIOA.0
	GPIO_ResetBits(GPIOA,GPIO_Pin_6); //PA0 ����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //PA7 ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA7 ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ�� GPIOA.0
	GPIO_ResetBits(GPIOA,GPIO_Pin_7); //PA0 ����	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //PB0 ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB0 ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ�� GPIOB.0
	GPIO_ResetBits(GPIOB,GPIO_Pin_0); //PB0 ����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //PB1 ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB1 ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ�� GPIOB.1
	GPIO_ResetBits(GPIOB,GPIO_Pin_1); //PB1 ����	
	
	//��ʼ�� TIM3 ����
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //Ԥ��Ƶ��
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //��ʼ�� TIMx
	
	//��ʼ�� TIM3 ���벶��ͨ�� 1234
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //�����ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽 TI1 ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //���������Ƶ,����Ƶ
	TIM_ICInitStructure.TIM_ICFilter = 0x00; //IC1F=0000 ���������˲��� ���˲�
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //ѡ������� IC1 ӳ�䵽 TI1 ��
	TIM_ICInit(TIM3, &TIM_ICInitStructure);         //��ʼ�� TIM3 ���벶��ͨ�� 1
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	//��ʼ�� NVIC �ж����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ� 2 ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ� 0 ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure); //��ʼ�� NVIC
	
	//ʹ���ж�
	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE); 
	TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);
	
	TIM_Cmd(TIM3,ENABLE ); //��ʹ�ܶ�ʱ�� 5
}

void TIM4_Cap_Init(u16 arr,u16 psc)   //TIM4��ͨ�����벶�����
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //ʹ�� TIM4 ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ�� GPIOB ʱ��

	//��ʼ�� GPIOA.6 ��GPIOA.7��GPIOB.0��GPIOB.1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PB6 ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ�� GPIOA.0
	GPIO_ResetBits(GPIOB,GPIO_Pin_6); //PA0 ����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //PB7 ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ�� GPIOA.0
	GPIO_ResetBits(GPIOB,GPIO_Pin_7); //PA0 ����	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //PB8 ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ�� GPIOA.0
	GPIO_ResetBits(GPIOB,GPIO_Pin_0); //PA0 ����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PB9 ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ�� GPIOA.0
	GPIO_ResetBits(GPIOB,GPIO_Pin_1); //PA0 ����	
	
	//��ʼ�� TIM4 ����
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //Ԥ��Ƶ��
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //��ʼ�� TIMx
	
	//��ʼ�� TIM4 ���벶��ͨ�� 1234
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //�����ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽 TI1 ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //���������Ƶ,����Ƶ
	TIM_ICInitStructure.TIM_ICFilter = 0x00; //IC1F=0000 ���������˲��� ���˲�
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //ѡ������� IC1 ӳ�䵽 TI1 ��
	TIM_ICInit(TIM4, &TIM_ICInitStructure);         //��ʼ�� TIM3 ���벶��ͨ�� 1
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	//��ʼ�� NVIC �ж����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //TIM3 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ� 2 ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ� 0 ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure); //��ʼ�� NVIC
	
	//ʹ���ж�
	TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE); 
	TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);
	
	TIM_Cmd(TIM4,ENABLE ); //ʹ�ܶ�ʱ�� 4
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		TIM3CAPcount[0]++;
		if(TIM3CaptureNumber[0] == 0)//��һ�β���
		{
			TIM3IC_ReadValue1[0] = TIM_GetCapture1(TIM3);
			TIM3CaptureNumber[0] = 1;
		}
		else if(TIM3CaptureNumber[0] == 1)//�ڶ��β���
		{
			TIM3IC_ReadValue2[0] = TIM_GetCapture1(TIM3);
			if (TIM3IC_ReadValue2[0] > TIM3IC_ReadValue1[0])
			{
				TIM3Capture[0] = (TIM3IC_ReadValue2[0] - TIM3IC_ReadValue1[0]);
			}
			else
			{
				TIM3Capture[0] = ((0xFFFF - TIM3IC_ReadValue1[0]) + TIM3IC_ReadValue2[0]);
			}
			TIM3CaptureNumber[0] = 0;
		}
	}
	else if(TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		TIM3CAPcount[1]++;
		if(TIM3CaptureNumber[1] == 0)//��һ�β���
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM3IC_ReadValue1[1] = TIM_GetCapture2(TIM3);
			TIM3CaptureNumber[1] = 1;
		}
		else if(TIM3CaptureNumber[1] == 1)//�ڶ��β���
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM3IC_ReadValue2[1] = TIM_GetCapture2(TIM3);
			/* TIM2IC_ReadValue computation */
			if (TIM3IC_ReadValue2[1] > TIM3IC_ReadValue1[1])
			{
				TIM3Capture[1] = (TIM3IC_ReadValue2[1] - TIM3IC_ReadValue1[1]);
			}
			else
			{
				TIM3Capture[1] = ((0xFFFF - TIM3IC_ReadValue1[1]) + TIM3IC_ReadValue2[1]);
			}
			/* Frequency computation */
			//Frequency0= 2000000/ TIM2Capture[0];// ����Ƶ��=2MHz/ʱ�Ӽ���
			TIM3CaptureNumber[1] = 0;
		}
	}
	else if(TIM_GetITStatus(TIM3, TIM_IT_CC3) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
		TIM3CAPcount[2]++;
		if(TIM3CaptureNumber[2] == 0)//��һ�β���
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM3IC_ReadValue1[2] = TIM_GetCapture3(TIM3);
			TIM3CaptureNumber[2] = 1;
		}
		else if(TIM3CaptureNumber[2] == 1)//�ڶ��β���
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM3IC_ReadValue2[2] = TIM_GetCapture3(TIM3);
			/* TIM2IC_ReadValue computation */
			if (TIM3IC_ReadValue2[2] > TIM3IC_ReadValue1[2])
			{
				TIM3Capture[2] = (TIM3IC_ReadValue2[2] - TIM3IC_ReadValue1[2]);
			}
			else
			{
				TIM3Capture[2] = ((0xFFFF - TIM3IC_ReadValue1[2]) + TIM3IC_ReadValue2[2]);
			}
			/* Frequency computation */
			//Frequency0= 2000000/ TIM2Capture[0];// ����Ƶ��=2MHz/ʱ�Ӽ���
			TIM3CaptureNumber[2] = 0;
		}
	}
	else if(TIM_GetITStatus(TIM3, TIM_IT_CC4) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
		TIM3CAPcount[3]++;
		if(TIM3CaptureNumber[3] == 0)//��һ�β���
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM3IC_ReadValue1[3] = TIM_GetCapture4(TIM3);
			TIM3CaptureNumber[3] = 1;
		}
		else if(TIM3CaptureNumber[3] == 1)//�ڶ��β���
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM3IC_ReadValue2[3] = TIM_GetCapture4(TIM3);
			/* TIM2IC_ReadValue computation */
			if (TIM3IC_ReadValue2[3] > TIM3IC_ReadValue1[3])
			{
				TIM3Capture[3] = (TIM3IC_ReadValue2[3] - TIM3IC_ReadValue1[3]);
			}
			else
			{
				TIM3Capture[3] = ((0xFFFF - TIM3IC_ReadValue1[3]) + TIM3IC_ReadValue2[3]);
			}
			/* Frequency computation */
			//Frequency0= 2000000/ TIM2Capture[0];// ����Ƶ��=2MHz/ʱ�Ӽ���
			TIM3CaptureNumber[3] = 0;
		}
	}
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		if(TIM4CaptureNumber[0] == 0)//��һ�β���
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM4IC_ReadValue1[0] = TIM_GetCapture1(TIM4);
			TIM4CaptureNumber[0] = 1;
		}
		else if(TIM4CaptureNumber[0] == 1)//�ڶ��β���
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM4IC_ReadValue2[0] = TIM_GetCapture1(TIM4);
			/* TIM2IC_ReadValue computation */
			if (TIM4IC_ReadValue2[0] > TIM4IC_ReadValue1[0])
			{
				TIM4Capture[0] = (TIM4IC_ReadValue2[0] - TIM4IC_ReadValue1[0]);
			}
			else
			{
				TIM4Capture[0] = ((0xFFFF - TIM4IC_ReadValue1[0]) + TIM4IC_ReadValue2[0]);
			}
			/* Frequency computation */
			//Frequency0= 2000000/ TIM2Capture[0];// ����Ƶ��=2MHz/ʱ�Ӽ���
			TIM4CaptureNumber[0] = 0;
		}
	}
	else if(TIM_GetITStatus(TIM4, TIM_IT_CC2) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
		if(TIM4CaptureNumber[1] == 0)//��һ�β���
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM4IC_ReadValue1[1] = TIM_GetCapture2(TIM4);
			TIM4CaptureNumber[1] = 1;
		}
		else if(TIM4CaptureNumber[1] == 1)//�ڶ��β���
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM4IC_ReadValue2[1] = TIM_GetCapture2(TIM4);
			/* TIM2IC_ReadValue computation */
			if (TIM4IC_ReadValue2[1] > TIM4IC_ReadValue1[1])
			{
				TIM4Capture[1] = (TIM4IC_ReadValue2[1] - TIM4IC_ReadValue1[1]);
			}
			else
			{
				TIM4Capture[1] = ((0xFFFF - TIM4IC_ReadValue1[1]) + TIM4IC_ReadValue2[1]);
			}
			/* Frequency computation */
			//Frequency0= 2000000/ TIM2Capture[0];// ����Ƶ��=2MHz/ʱ�Ӽ���
			TIM4CaptureNumber[1] = 0;
		}		
	}
	else if(TIM_GetITStatus(TIM4, TIM_IT_CC3) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
		if(TIM4CaptureNumber[2] == 0)//��һ�β���
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM4IC_ReadValue1[2] = TIM_GetCapture3(TIM4);
			TIM4CaptureNumber[2] = 1;
		}
		else if(TIM4CaptureNumber[2] == 1)//�ڶ��β���
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM4IC_ReadValue2[2] = TIM_GetCapture3(TIM4);
			/* TIM2IC_ReadValue computation */
			if (TIM4IC_ReadValue2[2] > TIM4IC_ReadValue1[2])
			{
				TIM4Capture[2] = (TIM4IC_ReadValue2[2] - TIM4IC_ReadValue1[2]);
			}
			else
			{
				TIM4Capture[2] = ((0xFFFF - TIM4IC_ReadValue1[2]) + TIM4IC_ReadValue2[2]);
			}
			/* Frequency computation */
			//Frequency0= 2000000/ TIM2Capture[0];// ����Ƶ��=2MHz/ʱ�Ӽ���
			TIM4CaptureNumber[2] = 0;
		}		
	}
	else if(TIM_GetITStatus(TIM4, TIM_IT_CC4) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
		if(TIM4CaptureNumber[3] == 0)//��һ�β���
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM4IC_ReadValue1[3] = TIM_GetCapture4(TIM4);
			TIM4CaptureNumber[3] = 1;
		}
		else if(TIM4CaptureNumber[3] == 1)//�ڶ��β���
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM4IC_ReadValue2[3] = TIM_GetCapture4(TIM4);
			/* TIM2IC_ReadValue computation */
			if (TIM4IC_ReadValue2[3] > TIM4IC_ReadValue1[3])
			{
				TIM4Capture[3] = (TIM4IC_ReadValue2[3] - TIM4IC_ReadValue1[3]);
			}
			else
			{
				TIM4Capture[3] = ((0xFFFF - TIM4IC_ReadValue1[3]) + TIM4IC_ReadValue2[3]);
			}
			/* Frequency computation */
			//Frequency0= 2000000/ TIM2Capture[0];// ����Ƶ��=2MHz/ʱ�Ӽ���
			TIM4CaptureNumber[3] = 0;
		}		
	}
}


 //test
//TIM2 PWM ���ֳ�ʼ��
//PWM �����ʼ��
//arr���Զ���װֵ psc��ʱ��Ԥ��Ƶ��
void TIM2_PWM_Init(u16 arr,u16 psc)//��ͨ��PWM
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //��ʹ�ܶ�ʱ�� 2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //��ʹ�� GPIO �� AFIO ���ù���ʱ��

	//���ø�����Ϊ�����������,��� TIM3 CH2 �� PWM ���岨�� PA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO
	
	//��ʼ�� TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //�������Զ���װ������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //�۳�ʼ�� TIMx
	//��ʼ�� TIM3 Channel2 PWM ģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ�� PWM ģʽ 2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure); //�ܳ�ʼ������ TIM3 OC1
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
	
	TIM_OC2Init(TIM2, &TIM_OCInitStructure); //�ܳ�ʼ������ TIM3 OC2
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
	
	TIM_OC3Init(TIM2, &TIM_OCInitStructure); //�ܳ�ʼ������ TIM3 OC3
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
	
	TIM_OC4Init(TIM2, &TIM_OCInitStructure); //�ܳ�ʼ������ TIM3 OC4
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM2, ENABLE); //��ʹ�� TIM3
}

void TIM4_PWM_Init(u16 arr,u16 psc)//��ͨ��PWM
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //��ʹ�ܶ�ʱ�� 2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //��ʹ�� GPIO �� AFIO ���ù���ʱ��

	//���ø�����Ϊ�����������,��� TIM3 CH2 �� PWM ���岨�� PA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO
	
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO
	
	//��ʼ�� TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //�������Զ���װ������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //�۳�ʼ�� TIMx
	//��ʼ�� TIM3 Channel2 PWM ģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ�� PWM ģʽ 2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure); //�ܳ�ʼ������ TIM3 OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
	
	TIM_OC2Init(TIM4, &TIM_OCInitStructure); //�ܳ�ʼ������ TIM3 OC2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
	
//	TIM_OC3Init(TIM4, &TIM_OCInitStructure); //�ܳ�ʼ������ TIM3 OC3
//	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
	
	TIM_OC4Init(TIM4, &TIM_OCInitStructure); //�ܳ�ʼ������ TIM3 OC4
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM4, ENABLE); //��ʹ�� TIM3
}
