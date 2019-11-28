#include "stm32f4xx.h"
#include "uart.h"//
#include "delay.h"
#include "string.h"//стандартная библиотека для работы со строками
#include "stdbool.h"//стандартная библиотека для работы с типом bool
extern  char* ssid ;
extern  char*password;
extern char* port;
extern char* time;
extern char* ip_update_host;
extern char* ip_update_get;
extern  char uartdata[250]; //буфер принятых данных uart
static char data[15];// с учетом всех точек
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
	delay_ms(200); 
	
 send_UART("AT+CIPSTO=");//
	 send_UART(time);//время,через которое ESP8266, отключит соединение неактивного TCP клиента 
	 send_UART("\r\n");
	 delay_ms(2000);

 }//end TCP_server
 //**************************************************************************************************************
void ip_update_DDNS(void)//обновление фактического IP на сервере сервиса DDNS
{  
	//созданнный накануне сервер уже подразумевает множественные соединения
    clear_uartdata();  
    send_UART("AT+CIPSTART=1,\"TCP\",\"");//создать TCP соединение 
  	send_UART(ip_update_host);        //с удаленным хостом http:
    send_UART("\",80");	              //порт по умолчению протокола http 80,https 443
	  send_UART("\r\n");
		delay_ms(5000); 
	//в случае соединения в бувере с [0] CONNECT    OK
	  send_UART("AT+CIPSEND=1,89\r\n");	//передать 89 байт даннных,считать точно
	  delay_ms(200);
   	clear_uartdata(); 
   
  	send_UART("GET ");             // запрос содержимого указанного пути хоста	
    send_UART(ip_update_get);      //обычно,за нас это делает браузер,мы ему только пишем ссылку                      
    send_UART( " HTTP/1.1"); 
    send_UART("\r\n");
	
    send_UART("Host:");                                    
    send_UART(ip_update_host); 
  	
    send_UART("\r\n");		
		send_UART("Connection: close");
		 send_UART("\r\n");
		 send_UART("\r\n");
		 
		 		 delay_ms(10000);
// IP будет в ответе,в буфере перед словом CLOSED [236]..[241],по-хорошему,надо найти
//сначала слово,тк в IP 0 разрядность плавает...если в режиме множественного соединения,то перед
//1,CLOSED,у меня N1
}//end ip_update_DDNS
//*************************************************************************************************************
 bool status(void)//определение состояния соединения
{
clear_uartdata();	
send_UART("AT+CIPSTATUS\r\n");//
delay_ms(10); //Без нормальных пауз после команды в буфере данных для обработки НЕ БУДЕТ(будут,но нас уже там нет) 
//нормальный ответ:STATUS:2,3    OK  //2-IP получен,3-подключен,4-отключен

if ((uartdata[7]=='2')||(uartdata[7]=='3'))
return true;
else	
return false;
}
//****************************************************************************************************************
char* wifi_ip(void)//вернет указатель на массив,кот содержит присвоеный ip модулю домашней сети
{
 clear_uartdata();
 send_UART("AT+CIFSR\r\n"); //1-разрешить множественнные соединения
	delay_ms(4); //2 мС МАЛО,IP даже не весь успевает прочитаться!!!
	memcpy(data, &uartdata[14],15); 
	delay_ms(10); //заканчиваем запись в буфер,чтоб полученные данные при следующей воманде не перемешивались
	return data;
		
}
//**************************************************************************************************************
void send_Connect(void)
{

send_UART("AT+CIPSEND=0,11\r\n");	//передать 89 байт даннных,считать точно
	  delay_ms(20);
send_UART("Connected\r\n");
	delay_ms(20);
   	clear_uartdata(); 
}


