#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
#include "stdio.h"


//�ֱ�ΪPWM���������
static int pwm_min = 250;
static int pwm_max = 899;
static int flag;


typedef struct
{
	u16 pwm;
	u16 kp;
	u16 ki;
	u16 kd;
	u32 laste;
	u32 preve;
	u32 sume;
}pid_v;


void init_v(pid_v *p);

u16 update_v(pid_v *p, int error);

void stop(void);

void testing(void);

void forward(int pwm);

void set_pwm(int pwm,u8 flag);//����min��max֮������ݽ������ã�flag��012���������Լ�ͬʱ

void tracing(void);//����������Ѳ��

//����ת�亯��
void turn(u8 flag);//ֱ��ת�亯��

	
#endif
