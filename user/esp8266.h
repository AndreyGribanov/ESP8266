#ifndef ESP8266_H//ДЕРЕКТИВЫ ПРЕПРОЦЕССОРА ,ПОЗВОЛЯЮЩИЕ ПОВТОРНО ВКЛЮЧАТЬ ФАЙЛ ЧЕРЕЗ #INCLUDE
#define ESP8266_H//
#include "stm32f4xx.h"

//прототипы:
void init_esp8266(void);//инициализация модуля
void connection_wifi(void);//подключение к сети WIFI
void TCP_server(void);//создать соединение по протоколу TCP в режиме сервера,N порта константа в main.c
void ip_update_DDNS(void);//обновление фактического IP на сервере сервиса DDNS
//void status(void);//определение состояния соединения
#endif

