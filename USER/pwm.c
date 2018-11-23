#include "pwm.h"
#include "stm32f10x_tim.h"
void TIM4_PWM_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef     GPIO_InitStrue;
    TIM_OCInitTypeDef     TIM_OCInitStrue;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);        //??TIM3???GPIO??
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);// ??GPIOB??(LED?BP5??),??AFIO??(???3??2??????BP5??)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
		//GPIO_AFIODeInit();
    //GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
	
    GPIO_InitStrue.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;     // TIM_CH2
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // ????
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //????????
    GPIO_Init(GPIOB,&GPIO_InitStrue);                //GPIO???????
    
    
    TIM_TimeBaseInitStrue.TIM_Period=arr;    //????????
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc;        //?????
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;    //???????
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1;        //???????,???????????,????TIM_CKD_DIV1
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStrue);        //TIM3?????(??PWM???)
    
    TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM2;        // PWM??2:CNT>CCR?????
    TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;// ????-??????
    TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;// ????
		TIM_OC1Init(TIM4,&TIM_OCInitStrue);
    TIM_OC2Init(TIM4,&TIM_OCInitStrue);        //TIM3???2PWM 

    TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);        //????????
    
    TIM_Cmd(TIM4,ENABLE);        //??TIM3
    TIM_CtrlPWMOutputs(TIM4,ENABLE);
}
