#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "pwm.h"
#include "control.h"

u8 at_atk[] = {0x41, 0x54, 0x0D, 0x0A} ;

u8 at_cwmode[] = {0x41, 0x54, 0x2B, 0x43, 0x57, 0x4D, 0x4F, 0x44, 0x45, 0x3D, 

0x33, 0x0D, 0x0A};//AT+CWMODE=3(??)

u8 at_rst[] = {0x41, 0x54, 0x2B, 0x52, 0x53, 0x54, 0x0D, 0x0A};//AT+RST(??)
u8 at_cwsap[] = {0x41, 0x54, 0x2B, 0x43, 0x57, 0x53, 0x41, 0x50, 0x3D, 0x22, 
		 0x45, 0x53, 0x50, 0x38, 0x32, 0x36, 0x36, 0x5F, 0x54, 0x45, 
		 0x53, 0x54, 0x22, 0x2C, 0x22, 0x31, 0x32, 0x33, 0x34, 0x35, 
		 0x36, 0x37, 0x38, 0x39, 0x30, 0x22, 0x2C, 0x31, 0x2C, 0x33, 
		 0x0D, 0x0A};//AT+CWSAP="ESP8266_TEST","1234567890",1,3(??)

u8 at_cwjap[] = {0x41, 0x54, 0x2B, 0x43, 0x57, 0x4A, 0x41, 0x50, 0x3D, 0x22, 
	0x33, 0x33, 0x33, 0x42, 0x22, 0x2C, 0x22, 0x79, 0x79, 0x72, 0x64, 0x78, 
	0x69, 0x61, 0x6F, 0x6B, 0x65, 0x61, 0x69, 0x22, 0x0D, 0x0A} ;
//AT+CWJAP="333B","yyrdxiaokeai"\r\n

u8 at_cipstart[] = {0x41, 0x54, 0x2B, 0x43, 0x49, 0x50, 0x53, 0x54, 0x41, 
	0x52, 0x54, 0x3D, 0x22, 0x54, 0x43, 0x50, 0x22, 0x2C, 0x22, 0x31, 0x39, 
0x32, 0x2E, 0x31, 0x36, 0x38, 0x2E, 0x31, 0x2E, 0x31, 0x30, 0x34, 0x22, 
0x2C, 0x32, 0x30, 0x30, 0x30, 0x30, 0x0D, 0x0A} ;
//AT+CIPSTART="TCP","192.168.1.104",20000
	
u8 at_cipmux[] = {0x41, 0x54, 0x2B, 0x43, 0x49, 0x50, 0x4D, 0x55, 0x58, 
		 0x3D, 0x31, 0x0D, 0x0A};//AT+CIPMUX=1(??)

u8 at_sipserver[] = {0x41, 0x54, 0x2B, 0x43, 0x49, 0x50, 0x53, 0x45, 0x52, 0x56, 
		 0x45, 0x52, 0x3D, 0x31, 0x2C, 0x38, 0x30, 0x38, 0x30, 0x0D, 0x0A};//AT+CIPSERVER=1,8080(??

u16 getLen(char *s){
	u16 ret=0 ;
	while(s[ret++]!='\0') ;
	return ret-1 ;
}
		 
void sendStr(char *s, u8 usartNum){
	u16 len=getLen(s), i=0;
	for(i=0;i<len;++i){
		if(usartNum==1){
			USART1->DR = s[i];//"AT\r\n"
			while((USART1->SR & 0x40) == 0);//�ȴ��������
		}
		else if(usartNum==2){
			USART2->DR = s[i];//"AT\r\n"
			while((USART2->SR & 0x40) == 0);//�ȴ��������
		}
	}
}
		 
void myDelay(u8 sec){  //����ԭ����delay���������ף��˴�����ѭ������
	u8 i;
	for(i=0 ; i<sec ; ++i){
		delay_ms(1000) ;
	}
}

void ESP8266_Init(u8 usartNum){	
	while((USART1->SR & 0x40) == 0);//���ԭ�����ݣ���������
	while((USART2->SR & 0x40) == 0);
	sendStr("AT\r\n",usartNum) ;
	myDelay(1);
	sendStr("AT+CWMODE=3\r\n",usartNum) ;
	myDelay(2) ;
	sendStr("AT+RST\r\n",usartNum) ;
	myDelay(3) ;
	sendStr("AT+CWJAP=\"333B\",\"yyrdxiaokeai\"\r\n",usartNum) ;
	myDelay(3) ;
	sendStr("AT+CIPSTART=\"TCP\",\"192.168.1.104\",20000\r\n",usartNum) ;
	myDelay(1) ;
}

void GPIO_init(){
	GPIO_InitTypeDef GPIO_InitStructure ;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure) ;
	GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6) ;
}

int main(void)
 {	
	
	u16 times=0; 
	 
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart_init(115200);	 //���ڳ�ʼ��115200
	GPIO_init() ;
//	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ� 
//	TIM_PWM_Init(899,0);//����Ƶ��PWMƵ��=72000/(7199+1)=10Khz
//	set_pwm(899,2);
	ESP8266_Init(2);
	
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{					   
			/*len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\nreceived data:\r\n");
			for(t=0;t<len;t++)
			{
				USART1->DR=USART_RX_BUF[t];
				while((USART1->SR&0X40)==0);//�ȴ����ͽ���
			}*/
			printf("\r\n\r\n");//���뻻��
			USART_RX_STA=0;
		}
		else
		{
			times++;
	//		if(times%100==0)printf("Please input a character;\r\n");  
	//		if(times%100==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
			delay_ms(10);
		}
	}	 
}
 