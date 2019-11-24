
//������:"����������" � �� STM32F407VET6 WIFI ������ ESP8266 �� �������� ���������. 
//��������� TELNET ����������� �������� ������ � ��
//������ ������������ � �������� ���� � ������ �������� ���� ,�������� ������ TCP,����� �������,
//������������� ������ ����������� � �������, ������������� ����� IP ����������� ������ � ������� DNS �������
//hldns.ru �������������� ����������� � ���� ��,���������� dns ��� ���� ����������� ��� ���������� � �� �� ����������
// IP ������
// ESP8266 ����������� AT-��������� �� UART,����� �� ����� ���� ��,������� ����� ����������� ��� ���� ������
// ��� freeRTOS, ���� ����� �����,��� ������� ��������� �� ��������� ���������

#include "stm32f4xx.h"
#include "uart.h"//������������ ���� ���������� ����� � ��������� ��� ������
#include "delay.h"//������� ��������� ���� � ���������


void init_RCC()//��������� ������������,���������� ��������� CUBE MX
{
RCC->CR|=RCC_CR_HSEON; //��������� ��������� HSE,������� �����
while (!(RCC->CR & RCC_CR_HSERDY)) {}; // ���� ����������
	
FLASH->ACR |= FLASH_ACR_PRFTEN; // Enable Prefetch Buffer.
FLASH->ACR &= ~FLASH_ACR_LATENCY; // �����������.
FLASH->ACR |= FLASH_ACR_LATENCY_2WS; // Two wait states
FLASH->ACR |=FLASH_ACR_ICEN | FLASH_ACR_DCEN;// Instruction cache enable, Data cache enable
	
RCC->CFGR &=~RCC_CFGR_SW; // �������� ���� SW0, SW1
RCC->CFGR |= RCC_CFGR_PPRE1_2;	//100: AHB clock divided by 2 for APB1
RCC->CFGR |= RCC_CFGR_SW_PLL; // ��������� �� PLL SW1=1	---!!!

 //Config PLL,see page 162	 
RCC->PLLCFGR=(4<<24) | (0<<16) | (84<<6) | 4 | RCC_PLLCFGR_PLLSRC_HSE; //   ������������� ��� ��������� ���� 	PLLON  � CR		
RCC->CR |=    RCC_CR_PLLON;
while (!(RCC->CR & RCC_CR_PLLRDY)){}; //wait for PLL ready 	
	
//� ����� �������� ������������ �� �������� ������ 8 ���,APB1=42MGz(������� ���� 82 ���),APB2=84MGz,SYSCLK=84MGz	

}//end init_RCC()
//*****************************************************************************************************************




int main(void)
{
	
init_RCC();






}
	
