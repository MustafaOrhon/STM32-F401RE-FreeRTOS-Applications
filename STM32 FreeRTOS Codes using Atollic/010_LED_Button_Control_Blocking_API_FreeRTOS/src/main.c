#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "string.h"

void Task1Handler(void *params);
void Task2Handler(void *params);
void GPIO_Config(void);
void UART_Config(void);
void printMsg(char *msg);
char myMessage[50];
TaskHandle_t Task1Handle = NULL ;
TaskHandle_t Task2Handle = NULL ;

int main(void)
{
  RCC_DeInit();   // HSE OFF | HSI ON |	PLL OFF |
  SystemCoreClockUpdate(); // SYSTEM CLOCK 16 MHz

  GPIO_Config();
  UART_Config();
  xTaskCreate(Task1Handler,"Task 1",configMINIMAL_STACK_SIZE,NULL,2,&Task1Handle);
  xTaskCreate(Task2Handler,"Task 2",configMINIMAL_STACK_SIZE,NULL,3,&Task2Handle);

  vTaskStartScheduler(); // START SCHEDULAR
  while (1)
  {


  }
}

void Task1Handler(void *params)
{
	   while(1)
	 {
		   if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3) == Bit_SET)
		   {
			   printMsg("GPIOC PINS ARE ON\r\n");
		   }
		   else
		   {
			   printMsg("GPIOC PINS ARE OFF\r\n");
		   }
		   vTaskDelay(1000);


	 }
}

void Task2Handler(void *params)
{


	   while(1)
	 {
         GPIO_ToggleBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
         vTaskDelay(pdMS_TO_TICKS(1000));

	 }
}
void GPIO_Config(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);


  GPIO_InitTypeDef GPIO_InitStruct ={0};

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOC,&GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOC,&GPIO_InitStruct);


}
void UART_Config(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct = {0} ;
	 USART_InitTypeDef USART_InitStruct = {0};

	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); // USART2 ENABLED
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); // GPIOA ENABLED

	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); // TX GPIOA PIN 2
	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); // RX GPIOA PIN 3

	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	 GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	 GPIO_Init(GPIOA,&GPIO_InitStruct);

	 USART_InitStruct.USART_BaudRate = 9600;
	 USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	 USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	 USART_InitStruct.USART_Parity = USART_Parity_No;
	 USART_InitStruct.USART_StopBits = USART_StopBits_1;
	 USART_InitStruct.USART_WordLength = USART_WordLength_8b;

	 USART_Init(USART2,&USART_InitStruct);

	 USART_Cmd(USART2,ENABLE);



}


void printMsg(char *msg)
{
   for(int i = 0 ; i< strlen(msg);i++)
   {
	   while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)!=SET);
	   USART_SendData(USART2,msg[i]);

   }

}
