
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
#include "esp8266.h"
#include "string.h"//����������� ���������� ��� ������ �� ��������(������� strcom � ���)


const char* ssid = "mu-hru";// ��� WiFi-����, � ������� ����� ������������ ESP8266
const char* password = "muhru";//" ����� ������� ������
const char* port = "10200";//����� ����� TCP ������� ��  ESP8266, ��������� ��� ������������� � WIFI ������
const char* time = "300";//�����,����� ������� ESP8266, �������� ���������� ����������� TCP �������,���,7200 max
const char* ip_update_host = "hldns.ru";  // ��������� ���� (����� �����) ������ DynDNS
const char* ip_update_get = "/update/FY4SYU777F2CU5FCN6B44X7GHMHB3X";  // ��������� GET-������ (����� ����������) ������ ������������� � ������ DynDNS
const char on[4]="1111";//������ ��������,��� ������� ����� ���� ��������
const char of[4]="2222";//������ ��������,��� ������� ����� ������ ��������
char copy[4]={0,0,0,0};
extern unsigned  char uartdata[250]; //����� �������� ������ uart
int i=5;//�������� ����������

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
	
//� ����� �������� ������������ �� �������� ������ 8 ���,APB1=42MGz(������� ���� 84 ���),APB2=84MGz,SYSCLK=84MGz	

}//end init_RCC()

//*****************************************************************************************************************
void init_GPIO()//������������� ������ ��� ������������
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
//����� pa6,pa7 ���� � +
  GPIOA->MODER |= GPIO_MODER_MODER6_0;//01-output,00-input(after reset),10-AF,11-analog.
 	GPIOA->OTYPER |= GPIO_OTYPER_OT6;//1-�������� ���������,0-�����������(����� ������)
	GPIOA->OSPEEDR |=GPIO_OSPEEDER_OSPEEDR6_1;//10-������� �������� 10���,01-2���,11-50���
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR6_0;//01-�������� � +PU,10-PD,00-��� ��������,
	GPIOA->BSRR |= GPIO_BSRR_BS6;
	
	 GPIOA->MODER |= GPIO_MODER_MODER7_0;//01-input
 	GPIOA->OTYPER |= GPIO_OTYPER_OT7;//1-�������� ���������
	GPIOA->OSPEEDR |=GPIO_OSPEEDER_OSPEEDR7_1;//10-������� ��������
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR7_0;//01-�������� � +
   GPIOA->BSRR |= GPIO_BSRR_BS7;
	
//������ ������ pe3,pe4 ���������� � ������
GPIOE->MODER &=~ GPIO_MODER_MODER3;//00-����
GPIOE->PUPDR |= GPIO_PUPDR_PUPDR3_0;//01- �������� � +,10-� -
GPIOE->MODER &=~ GPIO_MODER_MODER4;//00-����
GPIOE->PUPDR |= GPIO_PUPDR_PUPDR4_0;//01- �������� � +,10-� -
}//end init_GPIO()
//***********************************************************************************************************

int main(void)
{
static uint32_t t=0;	//������� �������
init_RCC();
init_GPIO();	
init_UART();
NVIC_EnableIRQ (USART2_IRQn);// ������� CMSIS ����������� ���������� � NVIC �� UART2
__enable_irq ();// ��������� ���������� ����������

init_esp8266();//������������� ������
connection_wifi();//����������� � ���� WIFI
TCP_server();//������� TCP ���������� � ������ �������	29
ip_update_DDNS();	

clear_uartdata();

while(1)
{
delay_ms(1);	//������������� ������ ��������� ������ UART
if (!(uartdata[0]==0))   //���� ����� ������� � ������ ��������� ������
{delay_ms(4);//������ ����� ��� ��������� �� ����� 20 ���� ��� �������� 115200 ���~92160 ���/c =217 ���
   memcpy(copy, &uartdata[11],4);  //���������� 4 �������� ������� ,������� � uartdata[9] � ������ copy 
}

clear_uartdata();
t++;
//memset (uartdata, 0, 255);//��������� 255 ��������� 0,����� ������� '',������� � string.h
//n=0;
if (!(strncmp(copy,on,4))){i=1;GPIOA->BSRR |= GPIO_BSRR_BR6;}//���� ��� ������� �� ���������,4�������� ��� ���������,������� �������
if (!(strncmp(copy,of,4))){i=0;GPIOA->BSRR |= GPIO_BSRR_BS6;}

while(t>300000)//300 000 ��= 5���,200000 �� ����� ���� ������ 8 �����
{
connection_wifi();//����������� � ���� WIFI
TCP_server();//������� TCP ���������� � ������ �������
ip_update_DDNS();	
clear_uartdata();
t=0;
}


}	
}
	
