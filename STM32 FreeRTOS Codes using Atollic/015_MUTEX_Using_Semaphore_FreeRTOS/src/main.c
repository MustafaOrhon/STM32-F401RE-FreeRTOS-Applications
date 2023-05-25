#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "semphr.h"
#include "stdlib.h"

void GPIO_Config(void);
void UART_Config(void);
void printMsg(char *msg);


/**************************************** PRIVATE VARIABLES*****************************************/

char myMSG[50];
SemaphoreHandle_t xBinarySemaphore;
/**************************************** PRIVATE FUNCTIONS*****************************************/
void Task1_Handler(void *params);
void Task2_Handler(void *params);


int main(void)
{

		RCC_DeInit(); //HSI ON  | HSE OFF | PLL OFF | Systemclock 16 Mhz;
		SystemCoreClockUpdate(); // Update SystemClock to 16MHz
		GPIO_Config();
    	UART_Config();

    	sprintf(myMSG,"Demo of Mutex with Binary Semapohore!\r\n");
    	printMsg(myMSG);

    	vSemaphoreCreateBinary(xBinarySemaphore);

    	if(xBinarySemaphore !=NULL)
    	{
        	sprintf(myMSG,"Semaphore and Tasks are created!\r\n");
        	printMsg(myMSG);

        	xTaskCreate(Task1_Handler,"Task 1",300,NULL,2,NULL);
        	xTaskCreate(Task2_Handler,"Task 2",300,NULL,2,NULL);

        	vTaskStartScheduler();
    	}


  while (1)
  {


  }
}

void GPIO_Config(void)
{

     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); // GPIOA ENABLED

     GPIO_InitTypeDef GPIO_InitStruct ={0};


	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); // TX GPIOA PIN 2
	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); // RX GPIOA PIN 3

	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	 GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	  GPIO_Init(GPIOA,&GPIO_InitStruct);


}

void UART_Config(void)
{

	 USART_InitTypeDef USART_InitStruct = {0};

	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); // USART2 ENABLED


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

void Task1_Handler(void *params)
{
   const char *pvTaskName ="Task1 is Running.\r\n";
	while(1)
	{
	   xSemaphoreTake(xBinarySemaphore,portMAX_DELAY);
       sprintf(myMSG,"%s",pvTaskName);
       printMsg(myMSG);
       xSemaphoreGive(xBinarySemaphore);
       vTaskDelay(pdMS_TO_TICKS(500));
	}


}


void Task2_Handler(void *params)
{
	   const char *pvTaskName ="Task2 is Running.\r\n";
		while(1)
		{
		   xSemaphoreTake(xBinarySemaphore,portMAX_DELAY);
	       sprintf(myMSG,"%s",pvTaskName);
	       printMsg(myMSG);
	       xSemaphoreGive(xBinarySemaphore);
	       vTaskDelay(pdMS_TO_TICKS(500));
		}

}
