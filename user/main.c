
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
#include "esp8266.h"
#include "string.h"//стандартная библиотека для работы со строками(функция strcom в ней)


const char* ssid = "mu-hru";// имя WiFi-сети, к которой будет подключаться ESP8266
const char* password = "muhru";//" здесь признак строки
const char* port = "10200";//номер порта TCP сервера из  ESP8266, прописать при переадресации в WIFI роутер
const char* time = "300";//время,через которое ESP8266, отключит соединение неактивного TCP клиента,сек,7200 max
const char* ip_update_host = "hldns.ru";  // Указываем хост (адрес сайта) службы DynDNS
const char* ip_update_get = "/update/FY4SYU777F2CU5FCN6B44X7GHMHB3X";  // Указываем GET-запрос (набор параметров) адреса идентификации в службе DynDNS
const char on[4]="1111";//массив сранения,при котором будет одно действие
const char of[4]="2222";//массив сранения,при котором будет другое действие
char copy[4]={0,0,0,0};
extern unsigned  char uartdata[250]; //буфер принятых данных uart
int i=5;//условная переменная

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
	
//в итоге получили ТАКТИРОВАНИЕ ОТ ВНЕШНЕГО КВАРЦА 8 МГц,APB1=42MGz(таймеры шины 84 МГц),APB2=84MGz,SYSCLK=84MGz	

}//end init_RCC()

//*****************************************************************************************************************
void init_GPIO()//инициализация портов для визуализации
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
//диоды pa6,pa7 анод к +
  GPIOA->MODER |= GPIO_MODER_MODER6_0;//01-output,00-input(after reset),10-AF,11-analog.
 	GPIOA->OTYPER |= GPIO_OTYPER_OT6;//1-открытый коллектор,0-двухтактный(после сброса)
	GPIOA->OSPEEDR |=GPIO_OSPEEDER_OSPEEDR6_1;//10-высокая скорость 10МГц,01-2МГц,11-50Мгц
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR6_0;//01-подтяжка к +PU,10-PD,00-без подтяжки,
	GPIOA->BSRR |= GPIO_BSRR_BS6;
	
	 GPIOA->MODER |= GPIO_MODER_MODER7_0;//01-input
 	GPIOA->OTYPER |= GPIO_OTYPER_OT7;//1-открытый коллектор
	GPIOA->OSPEEDR |=GPIO_OSPEEDER_OSPEEDR7_1;//10-высокая скорость
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR7_0;//01-подтяжка к +
   GPIOA->BSRR |= GPIO_BSRR_BS7;
	
//кнопки справа pe3,pe4 коммутация к минусу
GPIOE->MODER &=~ GPIO_MODER_MODER3;//00-ВХОД
GPIOE->PUPDR |= GPIO_PUPDR_PUPDR3_0;//01- подтяжка к +,10-к -
GPIOE->MODER &=~ GPIO_MODER_MODER4;//00-ВХОД
GPIOE->PUPDR |= GPIO_PUPDR_PUPDR4_0;//01- подтяжка к +,10-к -
}//end init_GPIO()
//***********************************************************************************************************

int main(void)
{
static uint32_t t=0;	//счетчик времени
init_RCC();
init_GPIO();	
init_UART();
NVIC_EnableIRQ (USART2_IRQn);// Функции CMSIS разрешающие прерывания в NVIC от UART2
__enable_irq ();// Разрешаем глобальные прерывания

init_esp8266();//инициализация модуля
connection_wifi();//подключение к сети WIFI
TCP_server();//создать TCP соединение в режиме сервера	29
ip_update_DDNS();	

clear_uartdata();

while(1)
{
delay_ms(1);	//периодичность опроса приемного буфера UART
if (!(uartdata[0]==0))   //если после очистки в буфере появились данные
{delay_ms(4);//делаем паузу для получения не менее 20 байт при скорости 115200 бод~92160 бит/c =217 мкс
   memcpy(copy, &uartdata[11],4);  //копировать 4 элемента массива ,начиная с uartdata[9] в массив copy 
}

clear_uartdata();
t++;
//memset (uartdata, 0, 255);//заполняет 255 элементов 0,можно символы '',функция в string.h
//n=0;
if (!(strncmp(copy,on,4))){i=1;GPIOA->BSRR |= GPIO_BSRR_BR6;}//если оба совпали по элементам,4элемента для сравнения,включая регистр
if (!(strncmp(copy,of,4))){i=0;GPIOA->BSRR |= GPIO_BSRR_BS6;}

while(t>300000)//300 000 мс= 5мин,200000 по факту чуть больше 8 минут
{
connection_wifi();//подключение к сети WIFI
TCP_server();//создать TCP соединение в режиме сервера
ip_update_DDNS();	
clear_uartdata();
t=0;
}


}	
}
	
