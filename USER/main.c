#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "pwm.h"
#include "control.h"

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
			while((USART1->SR & 0x40) == 0);//Wait for sending successfully...
		}
		else if(usartNum==2){
			USART2->DR = s[i];//"AT\r\n"
			while((USART2->SR & 0x40) == 0);
		}
	}
}
		 
void myDelay(u8 sec){  //The origin delay function is undependable, so I just use simple loop to delay more time.
	u8 i;
	for(i=0 ; i<sec ; ++i){
		delay_ms(1000) ;
	}
}

void ESP8266_Init(u8 usartNum){	
	while((USART1->SR & 0x40) == 0);//清空原有数据，否则会出错
	while((USART2->SR & 0x40) == 0);
/*	sendStr("AT\r\n",usartNum) ;
	myDelay(1);
	sendStr("AT+CWMODE=3\r\n",usartNum) ;
	myDelay(2) ;
	sendStr("AT+RST\r\n",usartNum) ;
	myDelay(10) ;*/
	myDelay(1) ;
	sendStr("AT+CWJAP=\"333B\",\"yyrdxiaokeai\"\r\n",usartNum) ;
	myDelay(20) ;
	sendStr("AT+CIPSTART=\"TCP\",\"192.168.0.104\",20000\r\n",usartNum) ;
	myDelay(1) ;
}

void GPIO_init(){
	GPIO_InitTypeDef GPIO_InitStructure ;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure) ;
	GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |  GPIO_Pin_7) ;
}

typedef struct{
	u16 x, y;
}position;

u8 startFlag = 0;
u8 sucFlag = 0 ;
u8 message[105] = {0} ;
u8 bufferLen = 0 ;
u8 buffer[205] = {0} ;
u8 outputFlag = 0 ;

// Infomation from host computer :

//!!! Caution !!! In this definition, my car is A and op's car is B !

u8 gameStatus = 0; 				// 0: Not	beginning   1: Running   2: Pause
u16 nowRound = 0;
position myPosition, opPosition ;
u8 passengerNum = 0; 
u8 passengerStatus[6] = {0}; //Index is between 1 and 5.  0: Not on the car  1: On my car   2: On op's car
												// Here should be modified to figure out who is A.
position passengerBeginPos[6] , passengerEndPos[6];
u8 myFoul = 0, opFoul = 0;
u16 myScore = 0, opScore = 0;

//////

void swapPosition(position *a, position *b){
	position tmp = *a ;
	*a = *b ;
	*b = tmp ;
}

void swapU8(u8 *a, u8 *b){
	u8 tmp = *a ;
	*a = *b ;
	*b = tmp ;
}

void swapU16(u16 *a, u16 *b){
	u16 tmp = *a ;
	*a = *b ;
	*b = tmp ;
}

void decode(){
	u8 i, index, bit ;
	gameStatus=message[0]>>6 ;
	nowRound=(((u16)(message[0]&((1<<6)-1)))<<8) | message[1] ;
	
	myPosition.x=(message[2]>>7) << 8 ;
	myPosition.y=(message[2]>>6) << 8 ;
	opPosition.x=(message[2]>>5) << 8 ;
	opPosition.y=(message[2]>>4) << 8 ;
	index=2, bit=3 ;
	for(i=1;i<=5;++i){
		passengerBeginPos[i].x = ((u16)(message[index]>>bit)) << 8 ;
		bit-- ;
		passengerBeginPos[i].y = ((u16)(message[index]>>bit)) << 8 ;
		bit-- ;
		passengerEndPos[i].x   = ((u16)(message[index]>>bit)) << 8 ;
		bit-- ;
		passengerEndPos[i].y   = ((u16)(message[index]>>bit)) << 8 ;
		bit-- ;
		if(bit<0) {
			bit=7;
			index++ ;
		}			
	}
	
	myPosition.x|=message[5], myPosition.y|=message[6] ;
	opPosition.x|=message[7], opPosition.y|=message[8] ;
	passengerNum = message[9] >> 2 ;
	passengerStatus[1] |= ((message[9]			) & 3) ;
	passengerStatus[2] |= ((message[10] >> 6) & 3) ;
	passengerStatus[3] |= ((message[10] >> 4) & 3) ;
	passengerStatus[4] |= ((message[10] >> 2) & 3) ;
	passengerStatus[5] |= ((message[10]     ) & 3) ;
	for(i=1;i<=5;++i){
		passengerBeginPos[i].x = message[11+((i-1)<<2)] ;
		passengerBeginPos[i].y = message[12+((i-1)<<2)] ;
		passengerEndPos[i].x   = message[13+((i-1)<<2)] ;
		passengerEndPos[i].y   = message[14+((i-1)<<2)] ;
	}
	myFoul = message[31], opFoul = message[32] ;
	myScore = (((u16)message[33])<<8)|message[34] ;
	opScore = (((u16)message[35])<<8)|message[36] ;
	/*if() {    // Determine who is A to decide whether swap or not.
		swapPosition(&myPosition, &opPosition) ;
		swapU8(&myFoul, &opFoul) ;
		swapU16(&myScore, &opScore) ;
		for(i=1 ; i<=5 ; i++){
			if(passengerStatus[i]==1)
				passengerStatus[i]=2;
			else if(passengerStatus[i]==2)
				passengerStatus[i]=1 ;
		}
	
		// Maybe not be finished...
	}*/
}

int main(void)
 {	
	
	u16 times=0; 
	u8 nowLen=0 , i=0 ;
	 
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(115200);	 //串口初始化115200
	GPIO_init() ;
//	LED_Init();		  	 //初始化与LED连接的硬件接口 
//	TIM_PWM_Init(899,0);//不分频。PWM频率=72000/(7199+1)=10Khz
//	set_pwm(899,2);
	ESP8266_Init(2);
	startFlag=1;
	
	//////////////////////(debug) printf可能出现问题，最好改用寄存器操作（可以使用sendStr函数）
	while(1)
	{
		/*while(nowLen<bufferLen-1){
				USART1->DR = buffer[nowLen++];
				while((USART1->SR & 0x40) == 0);//等待发送完毕
		}*/
		if(outputFlag){
			sendStr("output:", 1) ;
			for(i=0; i<outputFlag ; ++i){
				USART1->DR = buffer[i];
				while((USART1->SR & 0x40) == 0);//等待发送完毕
			}
			sendStr("\r\n", 1) ;
			outputFlag=0 ;
		}
		if(sucFlag){
			sucFlag=0 ;
			sendStr("\r\nReceive successfully !!\r\n", 1) ;
			for(i = 0; i < 64; i++)
			{
				message[i] = buffer[i];
			}
			//处理  
			//decode() ;
		}
	}	 
}
 
