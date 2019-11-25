#include "stm32f4xx.h"
#include "uart.h"//
#include "delay.h"
extern  char* ssid ;
extern  char*password;
extern char* port;
extern char* time;
void init_esp8266(void)//������������� ������
{
	clear_uartdata();
	send_UART("AT\r\n");
	delay_ms(200);
 	send_UART("AT+RST\r\n");//������������ ������
	delay_ms(4000);
	send_UART("ATE0\r\n");//���������� ���
	delay_ms(200);
		
}//end init_esp8266
//************************************************************************************************************
void connection_wifi(void)//����������� � ���� WIFI
{
	clear_uartdata();
send_UART("AT+CWMODE=1\r\n"); // ����� station 
	delay_ms(20);
send_UART("AT+CWJAP=\"");// \"-���� " � ��������� ������
send_UART(ssid); // SSID 
send_UART("\",\"");
send_UART(password);//������ ������� � ����
send_UART("\"");	
send_UART("\r\n");	
  delay_ms(10000);// disconnect,connect,got ip
	}//end connection_wifi
//*************************************************************************************************************
 void TCP_server(void)//������� ���������� �� ��������� TCP � ������ �������,N ����� ��������� � main.c
 {
	 clear_uartdata();
 send_UART("AT+CIPMUX=1\r\n"); //1-��������� �������������� ����������
	delay_ms(20); 
	 
	 send_UART("AT+CIPSERVER=0\r\n");//0-�������� ������� ����� �������
	 delay_ms(20);
	 
	
	 	 
  send_UART("AT+CIPSERVER=1,");//1-�������� �������
  send_UART(port);
  send_UART("\r\n");
	delay_ms(20); 
	
 send_UART("AT+CIPSTO=");//0-�������� ������� ����� �������
	 send_UART(time);//�����,����� ������� ESP8266, �������� ���������� ����������� TCP ������� 
	 send_UART("\r\n");
	 delay_ms(20);


 }
	
 