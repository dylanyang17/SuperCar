#ifndef __PWM_H
#define __PWM_H
#include "sys.h"



//以下函数将APB1下的TIM2与TIM3的CH1初始化为PWM输出端

void TIM_PWM_Init(u16 arr,u16 psc);

#endif
