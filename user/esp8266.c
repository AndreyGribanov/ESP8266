#include "stm32f4xx.h"
#include "uart.h"//
#include "delay.h"


void init_esp8266(void)//������������� ������
{
	send_UART("AT\r\n");
	delay_ms(200);
 	send_UART("ATE0\r\n");//���������� ���
	delay_ms(200);
	send_UART("AT+RST\r\n");//������������ ������
	delay_ms(200);
}//end init_esp8266
//************************************************************************************************************

