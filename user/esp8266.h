#ifndef ESP8266_H//��������� ������������� ,����������� �������� �������� ���� ����� #INCLUDE
#define ESP8266_H//
#include "stm32f4xx.h"
#include "stdbool.h"//����������� ���������� ��� ������ � ����� bool
//���������:
void init_esp8266(void);//������������� ������
void connection_wifi(void);//����������� � ���� WIFI
void TCP_server(void);//������� ���������� �� ��������� TCP � ������ �������,N ����� ��������� � main.c
void ip_update_DDNS(void);//���������� ������������ IP �� ������� ������� DDNS
bool status(void);//����������� ��������� ����������
char* wifi_ip(void);//������ ���������� ip ������ �������� ����
void send_Connect(void);
#endif

