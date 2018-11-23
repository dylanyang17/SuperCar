#include "movements.h"
#include "stm32f10x.h"

void left_forward(){
	GPIO_SetBits(GPIOC, IN1);
	GPIO_ResetBits(GPIOC, IN2);
}

void left_backward(){
	GPIO_ResetBits(GPIOC, IN1);
	GPIO_SetBits(GPIOC, IN2);
}

void left_stop(){
	GPIO_ResetBits(GPIOC, IN1);
	GPIO_ResetBits(GPIOC, IN2);
}

void right_forward(){
	GPIO_SetBits(GPIOC, IN3);
	GPIO_ResetBits(GPIOC, IN4);
}

void right_backward(){
	GPIO_ResetBits(GPIOC, IN3);
	GPIO_SetBits(GPIOC, IN4);
}

void right_stop(){
	GPIO_ResetBits(GPIOC, IN3);
	GPIO_ResetBits(GPIOC, IN4);
}

void forward1(){
	right_forward();
	left_forward();
}

void backward(){
	right_backward();
	left_backward();
}

void leftturn(){
	right_forward();
	left_stop();
}

void stop1(){
	right_stop();
	left_stop();
}
