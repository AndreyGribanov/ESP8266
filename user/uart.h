#ifndef UART_H//ДЕРЕКТИВЫ,ПОЗВОЛЯЮЩИЕ ПОВТОРНО ВКЛЮЧАТЬ ФАЙЛ ЧЕРЕЗ #INCLUDE
#define UART_H//НЕ ВЫЗЫВАЯ ПРИ ЭТОМ ОШИБОК
#include "stm32f4xx.h"



//прототипы:
void init_UART();//инициализация порта uart c портами uart PB6,PB7
void send_UART(  char* value);// процедура отправки данных в порт uart






#endif


