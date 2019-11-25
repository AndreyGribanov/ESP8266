#include "stm32f4xx.h"
#include "uart.h"//
#include "delay.h"
extern  char* ssid ;
extern  char*password;
void init_esp8266(void)//������������� ������
{
	send_UART("AT\r\n");
	delay_ms(200);
 	send_UART("ATE0\r\n");//���������� ���
	delay_ms(200);
	send_UART("AT+RST\r\n");//������������ ������
	delay_ms(2000);
}//end init_esp8266
//************************************************************************************************************
void connection_wifi(void)//����������� � ���� WIFI
{
send_UART("AT+CWMODE=1\r\n"); // ����� station 
	delay_ms(20);
send_UART("AT+CWJAP=\"");
send_UART(ssid); // SSID 
send_UART("\",\"");
send_UART(password);//������ ������� � ����
send_UART("\"");	
send_UART("\r\n");	
  delay_ms(200);
	
}
