#include "sys.h"


extern float Anglepp;//ÿ������ת���ٶ�
extern float CurAngle;   //��ǰ�Ƕ�
extern int CurDir;     //��ǰ�����ת����
extern int PulseNum;   //Ӧ��ת�ĽǶ�

void pulse(int count);//�����������
void HST_GPIO_Config(void);//HST���ƿ�����