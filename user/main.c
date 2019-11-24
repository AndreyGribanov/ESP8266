
//Задача:"Прикрутить" к МК STM32F407VET6 WIFI модуль ESP8266 со стоковой прошивкой. 
//удаленнно TELNET приложением передать данные в МК
//Модуль подсоединить к домашней сети в режиме абонента сети ,протокол обмена TCP,режим сервера,
//переадресация портов настраиваем в роутере, периодическую смену IP провайдером решаем с помощью DNS сервера
//hldns.ru периодическими обращениями к нему МК,фактически dns нам дает возможность нам обращаться к МК по постонному
// IP адресу
// ESP8266 управляется AT-командами по UART,имеет на борту свой МК,который можно перепрошить под свои задачи
// без freeRTOS, хотя сразу видно,что решение разделить на отдельные подзадачи

#include "stm32f4xx.h"
#include "uart.h"//подключаемый файл настройкой порта и функциями для работы
#include "delay.h"//функции генерации пауз в программе


void init_RCC()//настройка тактирования,пользуемся картинкой CUBE MX
{
RCC->CR|=RCC_CR_HSEON; //Запускаем генератор HSE,внешний кварц
while (!(RCC->CR & RCC_CR_HSERDY)) {}; // Ждем готовности
	
FLASH->ACR |= FLASH_ACR_PRFTEN; // Enable Prefetch Buffer.
FLASH->ACR &= ~FLASH_ACR_LATENCY; // Предочистка.
FLASH->ACR |= FLASH_ACR_LATENCY_2WS; // Two wait states
FLASH->ACR |=FLASH_ACR_ICEN | FLASH_ACR_DCEN;// Instruction cache enable, Data cache enable
	
RCC->CFGR &=~RCC_CFGR_SW; // Очистить биты SW0, SW1
RCC->CFGR |= RCC_CFGR_PPRE1_2;	//100: AHB clock divided by 2 for APB1
RCC->CFGR |= RCC_CFGR_SW_PLL; // Переходим на PLL SW1=1	---!!!

 //Config PLL,see page 162	 
RCC->PLLCFGR=(4<<24) | (0<<16) | (84<<6) | 4 | RCC_PLLCFGR_PLLSRC_HSE; //   настраевается при сброшеном бите 	PLLON  в CR		
RCC->CR |=    RCC_CR_PLLON;
while (!(RCC->CR & RCC_CR_PLLRDY)){}; //wait for PLL ready 	
	
//в итоге получили ТАКТИРОВАНИЕ ОТ ВНЕШНЕГО КВАРЦА 8 МГц,APB1=42MGz(таймеры шины 82 МГц),APB2=84MGz,SYSCLK=84MGz	

}//end init_RCC()
//*****************************************************************************************************************




int main(void)
{
	
init_RCC();






}
	
