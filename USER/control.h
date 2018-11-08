#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
#include "stdio.h"


//分别为PWM输出上下限
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

void set_pwm(int pwm,u8 flag);//对于min到max之间的数据进行设置；flag的012代表左右以及同时

void tracing(void);//基础的自主巡线

//各种转弯函数
void turn(u8 flag);//直角转弯函数

	
#endif
