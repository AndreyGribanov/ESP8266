#include "stm32f4xx.h"
#include "uart.h"//
#include "delay.h"
extern  char* ssid ;
extern  char*password;
extern char* port;
extern char* time;
extern char* ip_update_host;
extern char* ip_update_get;


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
	delay_ms(200); 
	
 send_UART("AT+CIPSTO=");//
	 send_UART(time);//�����,����� ������� ESP8266, �������� ���������� ����������� TCP ������� 
	 send_UART("\r\n");
	 delay_ms(2000);

 }//end TCP_server
 //**************************************************************************************************************
void ip_update_DDNS(void)//���������� ������������ IP �� ������� ������� DDNS
{  
	//���������� �������� ������ ��� ������������� ������������� ����������
    clear_uartdata();  
    send_UART("AT+CIPSTART=1,\"TCP\",\"");//������� TCP ���������� 
  	send_UART(ip_update_host);        //� ��������� ������ http:
    send_UART("\",80");	              //���� �� ��������� ��������� http 80,https 443
	  send_UART("\r\n");
		delay_ms(5000); 
	//� ������ ���������� � ������ � [0] CONNECT    OK
	  send_UART("AT+CIPSEND=1,89\r\n");	//�������� 89 ���� �������,������� �����
	  delay_ms(200);
   	clear_uartdata(); 
   
  	send_UART("GET ");             // ������ ����������� ���������� ���� �����	
    send_UART(ip_update_get);      //������,�� ��� ��� ������ �������,�� ��� ������ ����� ������                      
    send_UART( " HTTP/1.1"); 
    send_UART("\r\n");
	
    send_UART("Host:");                                    
    send_UART(ip_update_host); 
  	
    send_UART("\r\n");		
		send_UART("Connection: close");
		 send_UART("\r\n");
		 send_UART("\r\n");
		 
		 		 delay_ms(10000);
// IP ����� � ������,� ������ ����� ������ CLOSED [236]..[241],��-��������,���� �����
//������� �����,�� � IP 0 ����������� �������...���� � ������ �������������� ����������,�� �����
//1,CLOSED,� ���� N1
}//end ip_update_DDNS
//*************************************************************************************************************
//void status(void)//����������� ��������� ����������
//{
//clear_uartdata();	
//send_UART("AT+CIPSTATUS\r\n");//
//delay_ms(1); //��� ���� ����� �������� ������ ������ �� ����� ,��� �������, ��� �������!!!!
//	//���������� �����:STATUS:2    OK  |4 ������� ���� 0d0a0d0a
//  //2-IP �������,3-���������,4-��������
//}

