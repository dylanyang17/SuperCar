#include "timer.h"

extern double angle;

extern short getGyroZ(void);

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
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
