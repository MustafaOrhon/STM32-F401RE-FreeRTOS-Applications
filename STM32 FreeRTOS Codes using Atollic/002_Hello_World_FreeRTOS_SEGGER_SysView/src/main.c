/* CREATED 05.05.2023 MUSTAFA ORHON*/
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
char myMessage[100];


bool UART_ACCESS_KEY = 1;


TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;

void Task1_Handler(void *params);
void Task2_Handler(void *params);
void Setup(void);
void GPIO_Config(void);
void USART_Config(void);
void USART_TX(char *msg);
int main(void)
{

	DWT->CTRL |= (1<<0);  // ENABLE TIMESTAMP
   /* CONFIGURE SYSTEM CLOCK AS HSI || HSI ON ||HSE AND PLL OFF || SYSTEM CLOCK 16 MHz*/
	RCC_DeInit();
   /*Update System Clock as 16MHz*/
   SystemCoreClockUpdate();
   Setup();
   /* DO SEGGER SYSVIEW CONFIG AND START SYSVIEW*/
   SEGGER_SYSVIEW_Conf();
   SEGGER_SYSVIEW_Start();

   snprintf(myMessage,sizeof(myMessage),"Hello\r\n");
   USART_TX(myMessage);
   /*TASK1 Create*/
   xTaskCreate(Task1_Handler,"Task 1",configMINIMAL_STACK_SIZE,NULL,3,&myTask1Handle);
   /*TASK2 Create*/
   xTaskCreate(Task2_Handler,"Task 2",configMINIMAL_STACK_SIZE,NULL,3,&myTask2Handle);

   vTaskStartScheduler();

  while (1)
  {


  }

}

void Task1_Handler(void *params)
{
    while(1)
     {
    	   if(UART_ACCESS_KEY ==1)
    	   {
    		 UART_ACCESS_KEY = 0;
    	     snprintf(myMessage,sizeof(myMessage),"This is task 1\r\n");
    	     USART_TX(myMessage);
    	     UART_ACCESS_KEY = 1;
    	     taskYIELD();
    	   }
     }
}

void Task2_Handler(void *params)
{
	while(1)
	 {
		if(UART_ACCESS_KEY == 1)
		{
		UART_ACCESS_KEY = 0;
 	    snprintf(myMessage,sizeof(myMessage),"This is task 2\r\n");
 	    USART_TX(myMessage);
 	    UART_ACCESS_KEY = 1;
 	    taskYIELD();

		}
	 }
}
void Setup(void)
{
  GPIO_Config();
  USART_Config();
}
void GPIO_Config(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct;
	 memset(&GPIO_InitStruct,0,sizeof(GPIO_InitStruct));

	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); // USART2 TX
	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); // USART2 RX

	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	 GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	 GPIO_Init(GPIOA,&GPIO_InitStruct);

}
void USART_Config(void)
{
	USART_InitTypeDef USART_InitStruct;
	memset(&USART_InitStruct,0,sizeof(USART_InitStruct));

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_WordLength =  USART_WordLength_8b;
	USART_InitStruct.USART_Mode = USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;

	USART_Init(USART2,&USART_InitStruct);

	USART_Cmd(USART2,ENABLE);
}
void USART_TX(char *msg)
{

 for(int i = 0; i < strlen(msg);i++)
 {

	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)!= SET);
	USART_SendData(USART2, msg[i]);
 }

}
