#include "timer.h"

extern double angle;

extern short getGyroZ(void);

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}


void TIM4_PWM_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef     GPIO_InitStrue;
    TIM_OCInitTypeDef     TIM_OCInitStrue;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);        //??TIM3???GPIO??
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);// ??GPIOB??(LED?BP5??),??AFIO??(???3??2??????BP5??)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
    GPIO_InitStrue.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;     // TIM_CH2
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // ????
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //????????
    GPIO_Init(GPIOB,&GPIO_InitStrue);                //GPIO???????
    
    
    TIM_TimeBaseInitStrue.TIM_Period=arr;    //????????
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc;        //?????
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;    //???????
    TIM_TimeBaseInitStrue.TIM_ClockDivision=0;        //???????,???????????,????TIM_CKD_DIV1
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStrue);        //TIM3?????(??PWM???)
    
    TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM2;        // PWM??2:CNT>CCR?????
    TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;// ????-??????
    TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;// ????
		TIM_OC1Init(TIM4,&TIM_OCInitStrue);
    TIM_OC2Init(TIM4,&TIM_OCInitStrue);        //TIM4 PWM 

    TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);        //????????
    
    TIM_Cmd(TIM4,ENABLE);        //??TIM3
    //TIM_CtrlPWMOutputs(TIM4,ENABLE);
}
