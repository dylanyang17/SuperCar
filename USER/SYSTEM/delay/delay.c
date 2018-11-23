#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "pwm.h"

//This two definition may swap some time, so we may have to change it during competition.
#define dark 0			//the returned value of infrared sensor when it detects dark line.
#define light 1			//the returned value of infrared sensor when it does not detect dark line.

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

u8 bits[10] ;

void sendNum(u16 num, u8 usartNum){ //Output in decimal form.
	u8 length=0 , i=0 ;
	bits[1]=0 ;
	while(num){
		bits[++length] = num%10 ;
		num/=10 ;
	}
	if(length==0) length=1 ;
	for(i=length ; i>=1 ; --i){
		if(usartNum==1){
			USART1->DR = bits[i]+'0';
			while((USART1->SR & 0x40) == 0);
		}
		else if(usartNum==2){
			USART2->DR = bits[i]+'0';
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

void clearData(){
	while((USART1->SR & 0x40) == 0);//??????,?????
	while((USART2->SR & 0x40) == 0);
}

void ESP8266_Init(u8 usartNum){	
	clearData() ;
/*	sendStr("AT\r\n",usartNum) ;
	myDelay(1);
	sendStr("AT+CWMODE=3\r\n",usartNum) ;
	myDelay(2) ;
	sendStr("AT+RST\r\n",usartNum) ;
	myDelay(10) ;*/
	myDelay(1) ;
	sendStr("AT+CWJAP=\"333B\",\"yyrdxiaokeai\"\r\n",usartNum) ;
	myDelay(20) ;
	sendStr("AT+CIPSTART=\"TCP\",\"192.168.0.109\",20000\r\n",usartNum) ;
	myDelay(1) ;
}
//The initiatoin of GPIO for the vccs of wifi module.
void GPIO_init(){
	GPIO_InitTypeDef GPIO_InitStructure ;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10 |  GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure) ;
	GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10 |  GPIO_Pin_11) ;
}

typedef struct{
	u16 x, y;
}Position;

u8 sendFlag = 0 ;
u8 startFlag = 0;
u8 sucFlag = 0 ;
u8 beginFlag = 0 ;
u8 message[105] = {0} ;
u8 bufferLen = 0 ;
u8 buffer[205] = {0} ;
u8 outputFlag = 0 ;

// Infomation from host computer :

//!!! Caution !!! In this definition, my car is A and op's car is B !

u8 gameStatus = 0; 				// 0: Not	beginning   1: Running   2: Pause
u16 nowRound = 0;
Position myPosition, opPosition ;
u8 passengerNum = 0; 
u8 passengerStatus[6] = {0}; //Index is between 1 and 5.  0: Not on the car  1: On my car   2: On op's car
												// Here should be modified to figure out who is A.
Position passengerBeginPos[6] , passengerEndPos[6];
u8 myFoul = 0, opFoul = 0;
u16 myScore = 0, opScore = 0;

//////

void swapPosition(Position *a, Position *b){
	Position tmp = *a ;
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

void printInfo(){
	sendStr("\r\ngameStatus: ", 2) , sendNum(gameStatus, 2) ;
	sendStr("\r\nnowRound: ", 2) , sendNum(nowRound, 2) ;
	sendStr("\r\nmyPosition.x: ", 2) , sendNum(myPosition.x, 2) ;
	sendStr("\r\nmyPosition.y: ", 2) , sendNum(myPosition.y, 2) ;
	sendStr("\r\nopPosition.x: ", 2) , sendNum(opPosition.x, 2) ;
	sendStr("\r\nopPosition.y: ", 2) , sendNum(opPosition.y, 2) ;
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
		passengerBeginPos[i].x |= message[11+((i-1)<<2)] ;
		passengerBeginPos[i].y |= message[12+((i-1)<<2)] ;
		passengerEndPos[i].x   |= message[13+((i-1)<<2)] ;
		passengerEndPos[i].y   |= message[14+((i-1)<<2)] ;
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

//The configuration of PWM logic ouput and infrared vcc/gnd
//PC6 PC7 controls left wheel
//PC10 PC11 control right wheel
//PA1 PA2 are vcc/gnd of left infrared sensor
//PA6 PA5 are vcc/gnd of right infrared sensor
void GPIO_Config(void){
	GPIO_InitTypeDef     GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	GPIO_SetBits(GPIOA, GPIO_Pin_6);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_10);
	GPIO_ResetBits(GPIOC, GPIO_Pin_11);
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);
}

//The initiation of infrared sensor. PA3 & PA7 are the D0 port of left and right sensors, respectively.
void HW_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
//KEEP MOVING!
void forward(){
	GPIO_ResetBits(GPIOC, GPIO_Pin_10);//right forward
	GPIO_SetBits(GPIOC, GPIO_Pin_11);
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);//left forward
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
}

//????
void lwheelstop(){
	GPIO_ResetBits(GPIOC, GPIO_Pin_10);
	GPIO_ResetBits(GPIOC, GPIO_Pin_11);
}

//????
void rwheelstop(){
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);
}

//Trace ??
void trace(u8 leftblack, u8 rightblack){
	if(leftblack == dark && rightblack == light){
		rwheelstop();
	}
	else if(rightblack == dark && leftblack == light){
		lwheelstop();
	}
	else{
		forward();
	}
}



int main(void)
 {	
	
	u16 times=0; 
	u8 nowLen=0 , i=0 ;
	u8 leftblack = 0, rightblack = 0;
	u16 pwm_left = 380, pwm_right = 350;		//??pwm???,??????
	 
	delay_init();	    	 //???????	
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// ?????????2
	//uart_init(115200);	 //?????115200
	GPIO_init() ;

	TIM4_PWM_Init(899,0);			//Initiate PWM output
	HW_Init();
	//ESP8266_Init(1);
	startFlag=1;
	//clearData() ;
	
	TIM_SetCompare1(TIM4,pwm_left);		//ch1??pwm
	TIM_SetCompare2(TIM4,pwm_right);	//ch2??pwm
	
	delay_ms(500);
	forward();
	//////////////////////(debug) printf??????,?????????(????sendStr??)
	while(1)
	{
		/*while(nowLen<bufferLen-1){
				USART2->DR = buffer[nowLen++];
				while((USART2->SR & 0x40) == 0);//??????
		}*/
		if(sendFlag){
			USART2->DR = sendFlag;
			while((USART2->SR & 0x40) == 0);//??????
			sendFlag=0 ;
		}
		if(beginFlag){
			sendStr("Beginning...\r\n", 2) ;
			beginFlag=0 ;
		}
		if(outputFlag){
			/*sendStr("output:", 2) ;
			for(i=0; i<outputFlag ; ++i){
				USART2->DR = buffer[i];
				while((USART2->SR & 0x40) == 0);//??????
			}
			sendStr("\r\n", 2) ;*/
			outputFlag=0 ;
		}
		if(sucFlag){
			sucFlag=0 ;
			sendStr("\r\nReceive successfully !!\r\n", 2) ;
			for(i = 0; i < 64; i++)
			{
				message[i] = buffer[i];
				USART2->DR = message[i];
				while((USART2->SR & 0x40) == 0);//??????
			}
			//??  
			decode() ;
			printInfo() ;
		}
		//?????????
		leftblack = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3);
		rightblack = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7);
		trace(leftblack, rightblack);
	}	 
}

