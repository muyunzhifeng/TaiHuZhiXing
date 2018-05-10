#include "sys.h"


__IO uint16_t TIM3IC_ReadValue1[4]={0,0,0,0};
__IO uint16_t TIM3IC_ReadValue2[4]={0,0,0,0}; //记录各通道的捕获值
__IO uint16_t TIM3CaptureNumber[4]={0,0,0,0} ; //捕获次数
__IO uint32_t TIM3Capture[4]={0,0,0,0} ; //最终计算捕获差值
//__IO uint32_t TIM3Speed[4]={0,0,0,0} ; //最终捕获差值转化为速度

__IO uint16_t TIM4IC_ReadValue1[4]={0,0,0,0};
__IO uint16_t TIM4IC_ReadValue2[4]={0,0,0,0}; //记录各通道的捕获值
__IO uint16_t TIM4CaptureNumber[4]={0,0,0,0} ; //捕获次数
__IO uint32_t TIM4Capture[4]={0,0,0,0}; //最终计算捕获差值
//__IO uint32_t TIM4Speed[4]={0,0,0,0} ; //最终捕获差值转化为速度

__IO uint16_t TIM3CAPcount[4]={0,0,0,0};//四通道总共捕获到的值

void TIM3_Cap_Init(u16 arr,u16 psc)   //TIM3四通道输入捕获测速
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //使能 TIM3 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能 GPIOA 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能 GPIOB 时钟

	//初始化 GPIOA.6 和GPIOA.7、GPIOB.0、GPIOB.1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PA6 设置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA6下拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化 GPIOA.0
	GPIO_ResetBits(GPIOA,GPIO_Pin_6); //PA0 下拉
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //PA7 设置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA7 下拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化 GPIOA.0
	GPIO_ResetBits(GPIOA,GPIO_Pin_7); //PA0 下拉	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //PB0 设置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB0 下拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化 GPIOB.0
	GPIO_ResetBits(GPIOB,GPIO_Pin_0); //PB0 下拉

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //PB1 设置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB1 下拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化 GPIOB.1
	GPIO_ResetBits(GPIOB,GPIO_Pin_1); //PB1 下拉	
	
	//初始化 TIM3 参数
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //预分频器
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //初始化 TIMx
	
	//初始化 TIM3 输入捕获通道 1234
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到 TI1 上
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //配置输入分频,不分频
	TIM_ICInitStructure.TIM_ICFilter = 0x00; //IC1F=0000 配置输入滤波器 不滤波
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //选择输入端 IC1 映射到 TI1 上
	TIM_ICInit(TIM3, &TIM_ICInitStructure);         //初始化 TIM3 输入捕获通道 1
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	//初始化 NVIC 中断优先级分组
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级 2 级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //从优先级 0 级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道被使能
	NVIC_Init(&NVIC_InitStructure); //初始化 NVIC
	
	//使能中断
	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE); 
	TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);
	
	TIM_Cmd(TIM3,ENABLE ); //⑥使能定时器 5
}

void TIM4_Cap_Init(u16 arr,u16 psc)   //TIM4四通道输入捕获测速
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //使能 TIM4 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能 GPIOB 时钟

	//初始化 GPIOA.6 和GPIOA.7、GPIOB.0、GPIOB.1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PB6 设置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 下拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化 GPIOA.0
	GPIO_ResetBits(GPIOB,GPIO_Pin_6); //PA0 下拉
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //PB7 设置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 下拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化 GPIOA.0
	GPIO_ResetBits(GPIOB,GPIO_Pin_7); //PA0 下拉	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //PB8 设置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 下拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化 GPIOA.0
	GPIO_ResetBits(GPIOB,GPIO_Pin_0); //PA0 下拉

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PB9 设置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 下拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化 GPIOA.0
	GPIO_ResetBits(GPIOB,GPIO_Pin_1); //PA0 下拉	
	
	//初始化 TIM4 参数
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //预分频器
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //初始化 TIMx
	
	//初始化 TIM4 输入捕获通道 1234
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到 TI1 上
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //配置输入分频,不分频
	TIM_ICInitStructure.TIM_ICFilter = 0x00; //IC1F=0000 配置输入滤波器 不滤波
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //选择输入端 IC1 映射到 TI1 上
	TIM_ICInit(TIM4, &TIM_ICInitStructure);         //初始化 TIM3 输入捕获通道 1
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	//初始化 NVIC 中断优先级分组
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //TIM3 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级 2 级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //从优先级 0 级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道被使能
	NVIC_Init(&NVIC_InitStructure); //初始化 NVIC
	
	//使能中断
	TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE); 
	TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);
	
	TIM_Cmd(TIM4,ENABLE ); //使能定时器 4
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		TIM3CAPcount[0]++;
		if(TIM3CaptureNumber[0] == 0)//第一次捕获到
		{
			TIM3IC_ReadValue1[0] = TIM_GetCapture1(TIM3);
			TIM3CaptureNumber[0] = 1;
		}
		else if(TIM3CaptureNumber[0] == 1)//第二次捕获到
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
		if(TIM3CaptureNumber[1] == 0)//第一次捕获到
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM3IC_ReadValue1[1] = TIM_GetCapture2(TIM3);
			TIM3CaptureNumber[1] = 1;
		}
		else if(TIM3CaptureNumber[1] == 1)//第二次捕获到
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
			//Frequency0= 2000000/ TIM2Capture[0];// 计算频率=2MHz/时钟计数
			TIM3CaptureNumber[1] = 0;
		}
	}
	else if(TIM_GetITStatus(TIM3, TIM_IT_CC3) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
		TIM3CAPcount[2]++;
		if(TIM3CaptureNumber[2] == 0)//第一次捕获到
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM3IC_ReadValue1[2] = TIM_GetCapture3(TIM3);
			TIM3CaptureNumber[2] = 1;
		}
		else if(TIM3CaptureNumber[2] == 1)//第二次捕获到
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
			//Frequency0= 2000000/ TIM2Capture[0];// 计算频率=2MHz/时钟计数
			TIM3CaptureNumber[2] = 0;
		}
	}
	else if(TIM_GetITStatus(TIM3, TIM_IT_CC4) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
		TIM3CAPcount[3]++;
		if(TIM3CaptureNumber[3] == 0)//第一次捕获到
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM3IC_ReadValue1[3] = TIM_GetCapture4(TIM3);
			TIM3CaptureNumber[3] = 1;
		}
		else if(TIM3CaptureNumber[3] == 1)//第二次捕获到
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
			//Frequency0= 2000000/ TIM2Capture[0];// 计算频率=2MHz/时钟计数
			TIM3CaptureNumber[3] = 0;
		}
	}
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		if(TIM4CaptureNumber[0] == 0)//第一次捕获到
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM4IC_ReadValue1[0] = TIM_GetCapture1(TIM4);
			TIM4CaptureNumber[0] = 1;
		}
		else if(TIM4CaptureNumber[0] == 1)//第二次捕获到
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
			//Frequency0= 2000000/ TIM2Capture[0];// 计算频率=2MHz/时钟计数
			TIM4CaptureNumber[0] = 0;
		}
	}
	else if(TIM_GetITStatus(TIM4, TIM_IT_CC2) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
		if(TIM4CaptureNumber[1] == 0)//第一次捕获到
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM4IC_ReadValue1[1] = TIM_GetCapture2(TIM4);
			TIM4CaptureNumber[1] = 1;
		}
		else if(TIM4CaptureNumber[1] == 1)//第二次捕获到
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
			//Frequency0= 2000000/ TIM2Capture[0];// 计算频率=2MHz/时钟计数
			TIM4CaptureNumber[1] = 0;
		}		
	}
	else if(TIM_GetITStatus(TIM4, TIM_IT_CC3) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
		if(TIM4CaptureNumber[2] == 0)//第一次捕获到
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM4IC_ReadValue1[2] = TIM_GetCapture3(TIM4);
			TIM4CaptureNumber[2] = 1;
		}
		else if(TIM4CaptureNumber[2] == 1)//第二次捕获到
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
			//Frequency0= 2000000/ TIM2Capture[0];// 计算频率=2MHz/时钟计数
			TIM4CaptureNumber[2] = 0;
		}		
	}
	else if(TIM_GetITStatus(TIM4, TIM_IT_CC4) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
		if(TIM4CaptureNumber[3] == 0)//第一次捕获到
		{
			/* Get the Input TIM2IC_ReadValue value */
			TIM4IC_ReadValue1[3] = TIM_GetCapture4(TIM4);
			TIM4CaptureNumber[3] = 1;
		}
		else if(TIM4CaptureNumber[3] == 1)//第二次捕获到
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
			//Frequency0= 2000000/ TIM2Capture[0];// 计算频率=2MHz/时钟计数
			TIM4CaptureNumber[3] = 0;
		}		
	}
}


 //test
//TIM2 PWM 部分初始化
//PWM 输出初始化
//arr：自动重装值 psc：时钟预分频数
void TIM2_PWM_Init(u16 arr,u16 psc)//四通道PWM
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //①使能定时器 2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //①使能 GPIO 和 AFIO 复用功能时钟

	//设置该引脚为复用输出功能,输出 TIM3 CH2 的 PWM 脉冲波形 PA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //①初始化 GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //①初始化 GPIO
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //①初始化 GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //①初始化 GPIO
	
	//初始化 TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在自动重装载周期值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //③初始化 TIMx
	//初始化 TIM3 Channel2 PWM 模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择 PWM 模式 2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure); //④初始化外设 TIM3 OC1
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能预装载寄存器
	
	TIM_OC2Init(TIM2, &TIM_OCInitStructure); //④初始化外设 TIM3 OC2
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能预装载寄存器
	
	TIM_OC3Init(TIM2, &TIM_OCInitStructure); //④初始化外设 TIM3 OC3
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能预装载寄存器
	
	TIM_OC4Init(TIM2, &TIM_OCInitStructure); //④初始化外设 TIM3 OC4
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能预装载寄存器
	
	TIM_Cmd(TIM2, ENABLE); //⑤使能 TIM3
}

void TIM4_PWM_Init(u16 arr,u16 psc)//四通道PWM
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //①使能定时器 2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //①使能 GPIO 和 AFIO 复用功能时钟

	//设置该引脚为复用输出功能,输出 TIM3 CH2 的 PWM 脉冲波形 PA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //①初始化 GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //①初始化 GPIO
	
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure); //①初始化 GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //①初始化 GPIO
	
	//初始化 TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在自动重装载周期值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //③初始化 TIMx
	//初始化 TIM3 Channel2 PWM 模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择 PWM 模式 2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure); //④初始化外设 TIM3 OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); //使能预装载寄存器
	
	TIM_OC2Init(TIM4, &TIM_OCInitStructure); //④初始化外设 TIM3 OC2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //使能预装载寄存器
	
//	TIM_OC3Init(TIM4, &TIM_OCInitStructure); //④初始化外设 TIM3 OC3
//	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //使能预装载寄存器
	
	TIM_OC4Init(TIM4, &TIM_OCInitStructure); //④初始化外设 TIM3 OC4
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //使能预装载寄存器
	
	TIM_Cmd(TIM4, ENABLE); //⑤使能 TIM3
}
