#ifndef UART_H//���������,����������� �������� �������� ���� ����� #INCLUDE
#define UART_H//�� ������� ��� ���� ������
#include "stm32f4xx.h"



//���������:
void init_UART(void);//������������� ����� uart c ������� uart PB6,PB7
void send_UART( char* value);// ��������� �������� ������ � ���� uart
void USART2_IRQHandler(void);//���������� ���������� uart
void clear_uartdata(void);//������� ������ ������ uart




#endif


