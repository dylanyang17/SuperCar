#include "control.h"
#include "delay.h"


void init_v(pid_v *p)
{
	
}

u16 update_v(pid_v *p, int error)
{
	u32 perror, derror, ierror;
	u32 templ;
	
	perror = error-p->laste;
	ierror = error;
	derror = error-2*(p->laste) +p->preve;
	templ = p->kp*perror + p->ki*ierror + p->kd*derror;
	
	p->preve = p->laste;
	p->laste = error;
	
	return templ;
}

void stop(void)
{
	set_pwm(899,2);
	//printf("\r\nreceived data:%2x\r\n", Res);
	printf("\r\nstopped!\r\n");
	delay_ms(10000);
	printf("\r\nstart!\r\n");
}

void testing()
{
	u8 dir=1;
	int times = 0;
	int pwm = pwm_min;
   	while(times < 100)
	{
 		++times;
		delay_ms(200);
		if(dir)
			pwm+=20;
		else
			pwm-=20;
 		if(pwm>=pwm_max)
			dir=0;
		if(pwm<=pwm_min)
			dir=1;
		set_pwm(pwm, 2);
		printf("\r\npwm:%d\r\n", pwm);
	}
}

void forward(int pwm)
{    
	set_pwm(pwm, 2);
	delay_ms(10000);
}

void set_pwm(int pwm,u8 flag)
{
	
	if(pwm < pwm_min)
		pwm = pwm_min;
	else if(pwm > pwm_max)
		pwm = pwm_max;
	if(flag == 0)
		TIM_SetCompare1(TIM2,pwm);
	else if(flag == 1)
		TIM_SetCompare2(TIM2,pwm);
	else
	{
		TIM_SetCompare1(TIM2,pwm);
		TIM_SetCompare2(TIM2,pwm);
	}
	
}

void tracing(void)
{
	
}

void turn_r(u8 flag)
{
	
}

