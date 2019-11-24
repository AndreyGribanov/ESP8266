#include "stm32f4xx.h"
#include "uart.h"//
#include "delay.h"


void init_esp8266(void)//инициализация модуля
{
	send_UART("AT\r\n");
	delay_ms(200);
 	send_UART("ATE0\r\n");//отключение эха
	delay_ms(200);
	send_UART("AT+RST\r\n");//перезагрузка модуля
	delay_ms(200);
}//end init_esp8266
//************************************************************************************************************

