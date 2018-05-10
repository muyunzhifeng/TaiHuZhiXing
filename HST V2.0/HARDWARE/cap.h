
#include "sys.h"

extern __IO uint16_t TIM3IC_ReadValue1[4];
extern __IO uint16_t TIM3IC_ReadValue2[4]; //��¼��ͨ���Ĳ���ֵ
extern __IO uint16_t TIM3CaptureNumber[4] ; //�������
extern __IO uint32_t TIM3Capture[4] ; //���ռ��㲶���ֵ

extern __IO uint16_t TIM4IC_ReadValue1[4];
extern __IO uint16_t TIM4IC_ReadValue2[4]; //��¼��ͨ���Ĳ���ֵ
extern __IO uint16_t TIM4CaptureNumber[4] ; //�������
extern __IO uint32_t TIM4Capture[4] ; //���ռ��㲶���ֵ

extern __IO uint16_t TIM3CAPcount[4];

void TIM2_PWM_Init(u16 arr,u16 psc);//��ͨ��PWM
void TIM4_PWM_Init(u16 arr,u16 psc);//��ͨ��PWM

void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void TIM3_Cap_Init(u16 arr,u16 psc);   //TIM3��ͨ�����벶�����
void TIM4_Cap_Init(u16 arr,u16 psc);    //TIM4��ͨ�����벶�����	