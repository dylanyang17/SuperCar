
#include "usart.h"
#include "sys.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//????ucos,???????????.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ??	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//?????????,??????,??????????
//ALIENTEK STM32???
//??1???		   
//????@ALIENTEK
//????:www.openedv.com
//????:2012/8/18
//??:V1.5
//????,?????
//Copyright(C) ????????????? 2009-2019
//All rights reserved
//********************************************************************************
//V1.3???? 
//?????????????????.
//????printf???
//???????????.
//???printf????????bug
//V1.4????
//1,???????IO?bug
//2,???USART_RX_STA,????????????2?14??
//3,???USART_REC_LEN,????????????????(???2?14??)
//4,???EN_USART1_RX?????
//V1.5????
//1,????UCOSII???
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//??????,??printf??,??????use MicroLIB	  

#if 1
#pragma import(__use_no_semihosting)             
//??????????                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//??_sys_exit()??????????    
_sys_exit(int x) 
{ 
	x = x; 
} 
//???fputc?? 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//????,??????   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 
 
#if EN_USART1_RX   //???????
//??1??????
//??,??USARTx->SR??????????   	
u8 USART_RX_BUF[USART_REC_LEN];     //????,??USART_REC_LEN???.
//????
//bit15,	??????
//bit14,	???0x0d
//bit13~0,	??????????
u16 USART_RX_STA=0;       //??????	  
  
extern void uart_init(u32 bound){
  //GPIO????
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	//??USART1,GPIOA??
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_AFIO, ENABLE);
	
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//??????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.9
   
  //USART1_RX	  GPIOA.10???
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.10  

	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//??????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.2
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.3  
	
  //Usart1 NVIC ??
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
  
	//Usart2 NVIC ??
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	
  
  
   //USART ?????

	USART_InitStructure.USART_BaudRate = bound;//?????
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
	USART_InitStructure.USART_Parity = USART_Parity_No;//??????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????

  USART_Init(USART1, &USART_InitStructure); //?????1
  USART_Init(USART2, &USART_InitStructure); //?????2
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//????????
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART1, ENABLE);                    //????1 
	USART_Cmd(USART2, ENABLE); 										//????2
}

extern u8 sendFlag ;
extern u8 startFlag ;
extern u8 message[105] ;
extern u8 beginFlag ;
extern u8 sucFlag ;
extern u8 bufferLen ;
extern u8 buffer[205] ;
extern u8 outputFlag ;

void USART2_IRQHandler(void)                	//??2??????
	{
	u8 Res = 0;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		Res =USART_ReceiveData(USART2);	//???????? 
	//		printf("\r\nreceived data:%2x\r\n", Res);
		if(Res == 0x21)
		{
		}
		else if(Res == 0x3F)
		{
		}
		else if(Res == 0x2A)
		{
		}
		/*if((USART_RX_STA&0x8000)==0)//?????
			{
			if(USART_RX_STA&0x4000)//????0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//????,????
				else USART_RX_STA|=0x8000;	//????? 
				}
			else //????0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//??????,??????	  
					}		 
				}
			} */  		 
     } 

} 

u8 checkBeginning(void)
{
	//???"+IPD," ??Socket?????????
	if(bufferLen >= 8
		&& buffer[bufferLen - 8] == 0x2B
		&& buffer[bufferLen - 7] == 0x49
		&& buffer[bufferLen - 6] == 0x50
		&& buffer[bufferLen - 5] == 0x44
		&& buffer[bufferLen - 4] == 0x2C
		/*&& buffer[bufferLen - 3] == 0x36
		&& buffer[bufferLen - 2] == 0x34
		&& buffer[bufferLen - 1] == 0x3A*/)
		return 1;
	else return 0;
}


void USART1_IRQHandler(void)                	//??1??????
{
	u8 Res = 0;
	u8 i=0 ;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Res =USART_ReceiveData(USART1);	//????????
			//printf("\r\nUSART1 received data: %c\r\n", Res);
		if(!startFlag) { sendFlag=Res ; return ; }
	//	else printf("%02X", Res);
		buffer[bufferLen++] = Res ;
	//	printf("%c", Res);
		if(checkBeginning() == 1){
			bufferLen = 0 ;
			beginFlag = 1 ;
		//	printf("\nBeginning..\n") ;
		}
		if(bufferLen >= 2 
				&& bufferLen < 64 
				&& buffer[bufferLen-2]==0x0D 
				&& buffer[bufferLen-1]==0x0A)
		{
			outputFlag = bufferLen;
			bufferLen = 0;
		//	printf("\nFalse Data\n") ;
		}
		
		if(bufferLen >= 64){
			if(buffer[62]==0x0D
				&& buffer[63]==0x0A)
			{
				sucFlag = 1 ;
			}
			bufferLen = 0;
		}
  } 

} 
	
void UART_PutChar(USART_TypeDef* USARTx, uint8_t Data)  
{  
    USART_SendData(USARTx, Data);  
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET){}  
}  

void UART_PutStr (USART_TypeDef* USARTx, uint8_t *str)    
{    
    while (0 != *str)    
    {    
        UART_PutChar(USARTx, *str);    
        str++;      
    }    
}

#endif	
