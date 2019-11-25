#include "stm32f4xx.h"


unsigned  char uartdata[250]; //буфер принятых данных uart
static uint8_t n=0;//номер байта в буфере  uart ,видна только в uart.c
void init_UART(void)//инициализация порта uart c портами uart PB6,PB7
{
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;// Разрешить тактирование портаPORTA
	
	//PB6-USART1 TX ВЫХОД af7->PA3
	 GPIOA->MODER &=~ GPIO_MODER_MODER3;//сброс
	 GPIOA->MODER |= GPIO_MODER_MODER3_1;//01-output,00-input(after reset),10-AF,11-analog.
	 GPIOA->AFR[0]  &=~ GPIO_AFRL_AFSEL3 ;//сброс
	 GPIOA->AFR[0]  |= (GPIO_AFRL_AFSEL3_2 |GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_0);//AF7=	usart
	 GPIOA->OTYPER &=~ GPIO_OTYPER_OT3;//1-открытый коллектор,0-двухтактный(после сброса)
	 GPIOA->OSPEEDR |=GPIO_OSPEEDER_OSPEEDR3_1;//10-высокая скорость 10МГц,01-2МГц,11-50Мгц
	//PB7-USART1 RX ВХОД->PA2
   GPIOA->MODER &=~ GPIO_MODER_MODER2;//сброс
   GPIOA->MODER   |= GPIO_MODER_MODER2_1;//01-output,00-input(after reset),10-AF,11-analog.
	 GPIOA->AFR[0]  &=~ GPIO_AFRL_AFSEL2 ;//сброс
   GPIOA->AFR[0]  |= (GPIO_AFRL_AFSEL2_2 |GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_0);//AF7=	usart
		
   RCC->APB1ENR	|= RCC_APB1ENR_USART2EN;//подключаем тактирование usart
		

	USART2->BRR 	= 0x16D;//настраиваем на скорость 115200 бод
  //42000000/115200/16=22,79  -> 0x16   0,79*16=12,64=13  0xD                        
	
	USART2->CR1 	|= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE |	 USART_CR1_RXNEIE ;
 //USART1->CR3 |=USART_CR2_CLKEN;
	 //ue-включить блок usart,te-вкл передатчик,re-вкл приемник,rxneie-вкл прерывание по приему байта.

}//end init_UART
//*************************************************************************************************************
void send_UART( char* value)// процедура отправки данных в порт uart из массива value
{
	int i=0;
	while (value[i])//пока данные не равны 0
	{while(!(USART2->SR & USART_SR_TXE)) {}  // Ожидаем когда очистится буфер передачи
	 USART2->DR = value[i]; // Помещаем данные в буфер, начинаем передачу
	i++;
	}
}//end send_UART
//***********************************************************************************************************

void clear_uartdata(void)//очистка буфера данных uart
{	
	for(uint8_t i=0;i<250;i++)
	{ uartdata[i] = 0; }
	n=0;//после стирания пишем в начало
}//end clear_uartdata
//*************************************************************************************************************

void USART2_IRQHandler(void)//обработчик прерываний uart
{ 
	while (USART2->SR & USART_SR_RXNE)//если пришли данные(поднят флаг RXNE)..,сброс вручную или после чтения DR
	{ 				uartdata[n]=USART2->DR;
			n++;
  //if 	(uartdata[n-1]==250){n=0;}	//если переполнение буфера,то пишем в начало	
			}	    
		}//end USART1_IRQHandler
//********************************************************************************************************************
		
		
















