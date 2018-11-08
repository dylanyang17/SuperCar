//#include "wifi.h"
//#include "delay.h"




//void ESP8266_Init(void){

//	int i = 0;

//	for(i = 0; i < 13; i++){

//		USART1->DR = at_cwmode[i];//??AT+CWMODE=3(??)

//		while((USART1->SR & 0x40) == 0);//??????

//	}

//	delay_ms(1000);

//	for(i = 0; i < 8; i++){

//		USART1->DR = at_rst[i];//??AT+RST(??)

//		while((USART1->SR & 0x40) == 0);//??????

//	}

//	delay_ms(1000);

//	for(i = 0; i < 42; i++){

//		USART1->DR = at_cwsap[i];//??AT+CWSAP="ESP8266_TEST","1234567890",1,3(??)

//		while((USART1->SR & 0x40) == 0);//??????

//	}

//	delay_ms(1000);

//	for(i = 0; i < 13; i++){

//		USART1->DR = at_cipmux[i];//??AT+CIPMUX=1(??)

//		while((USART1->SR & 0x40) == 0);//??????

//	}

//	delay_ms(1000);

//	for(i = 0; i < 21; i++){

//		USART1->DR = at_sipserver[i];//??AT+CIPSERVER=1,8080(??)

//		while((USART1->SR & 0x40) == 0);//??????

//	}

//}

