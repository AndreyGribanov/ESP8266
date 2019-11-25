#include "stm32f4xx.h"
#include "uart.h"//
#include "delay.h"
extern  char* ssid ;
extern  char*password;
extern char* port;
extern char* time;
void init_esp8266(void)//инициализация модуля
{
	clear_uartdata();
	send_UART("AT\r\n");
	delay_ms(200);
 	send_UART("AT+RST\r\n");//перезагрузка модуля
	delay_ms(4000);
	send_UART("ATE0\r\n");//отключение эха
	delay_ms(200);
		
}//end init_esp8266
//************************************************************************************************************
void connection_wifi(void)//подключение к сети WIFI
{
	clear_uartdata();
send_UART("AT+CWMODE=1\r\n"); // режим station 
	delay_ms(20);
send_UART("AT+CWJAP=\"");// \"-знак " в текстовой строке
send_UART(ssid); // SSID 
send_UART("\",\"");
send_UART(password);//пароль доступа к сети
send_UART("\"");	
send_UART("\r\n");	
  delay_ms(10000);// disconnect,connect,got ip
	}//end connection_wifi
//*************************************************************************************************************
 void TCP_server(void)//создать соединение по протоколу TCP в режиме сервера,N порта константа в main.c
 {
	 clear_uartdata();
 send_UART("AT+CIPMUX=1\r\n"); //1-разрешить множественнные соединения
	delay_ms(20); 
	 
	 send_UART("AT+CIPSERVER=0\r\n");//0-удаление сервера перед стартом
	 delay_ms(20);
	 
	
	 	 
  send_UART("AT+CIPSERVER=1,");//1-создание сервера
  send_UART(port);
  send_UART("\r\n");
	delay_ms(20); 
	
 send_UART("AT+CIPSTO=");//0-удаление сервера перед стартом
	 send_UART(time);//время,через которое ESP8266, отключит соединение неактивного TCP клиента 
	 send_UART("\r\n");
	 delay_ms(20);


 }
	
 