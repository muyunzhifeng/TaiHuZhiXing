#include "sys.h"


extern float Anglepp;//每个脉冲转多少度
extern float CurAngle;   //当前角度
extern int CurDir;     //当前电机旋转方向
extern int PulseNum;   //应旋转的角度

void pulse(int count);//脉冲产生函数
void HST_GPIO_Config(void);//HST控制口配置