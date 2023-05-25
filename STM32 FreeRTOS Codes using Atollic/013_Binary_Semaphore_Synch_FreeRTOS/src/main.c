#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stdio.h"
#include "string.h"
#include "semphr.h"
#include "stdlib.h"

void GPIO_Config(void);
void UART_Config(void);
void printMsg(char *msg);
/**************************************** PRIVATE VARIABLES*****************************************/
TaskHandle_t MasterTaskHandle;
TaskHandle_t SlaveTaskHandle;
char myMSG[50];

xQueueHandle xQueue = NULL;
xSemaphoreHandle xSemaphore = NULL;
/**************************************** PRIVATE FUNCTIONS*****************************************/
void MasterTaskHandler(void *params);
void SlaveTaskHandler(void *params);
void SlaveDoWork(unsigned int passId);

int main(void)
{
	RCC_DeInit(); //HSI ON  | HSE OFF | PLL OFF | Systemclock 16 Mhz;
	SystemCoreClockUpdate(); // Update SystemClock to 16MHz
    GPIO_Config();
    UART_Config();
    sprintf(myMSG,"This is Binary Semaphore Synch Application!\r\n");
    printMsg(myMSG);

    xQueue = xQueueCreate(1,sizeof(unsigned int));

    vSemaphoreCreateBinary(xSemaphore);

    if(xSemaphore !=NULL && xQueue != NULL)
    {
          sprintf(myMSG,"\r\nQueue and Semaphore Successfully Created!\r\n");
    	  printMsg(myMSG);
    	  xTaskCreate(MasterTaskHandler,"Random Number Generator",350,NULL,3,&MasterTaskHandle);
    	  xTaskCreate(SlaveTaskHandler,"Random Number Process",350,NULL,2,&SlaveTaskHandle);

    	  vTaskStartScheduler();

    }
    else
    {
    	  sprintf(myMSG,"\r\nQueue and Semaphore Could Not Created!\r\n");
    	  printMsg(myMSG);
    }


  while (1)
  {


  }
}

void GPIO_Config(void)
{
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
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

void MasterTaskHandler(void *params)
{
	unsigned int xMasterPassId;
	portBASE_TYPE xStatus; // This is used for watching port Status
	xSemaphoreGive(xSemaphore); // Give Semaphore First Time To Start Synch

	while(1)
	{
       xMasterPassId = (rand() & 0x1FF);
       xStatus = xQueueSend(xQueue,&xMasterPassId,portMAX_DELAY);
       if(xStatus != pdPASS)
       {
    	    sprintf(myMSG,"Queue Send Task Failed\r\n");
    	    printMsg(myMSG);

       }
       else
       {
    	   xSemaphoreGive(xSemaphore);

    	   taskYIELD();

       }


	}

}

void SlaveDoWork(unsigned int passId)
{
	sprintf(myMSG,"Slave task working on Passed ID: %d\r\n",passId);
	printMsg(myMSG);
	vTaskDelay(pdMS_TO_TICKS(500));

}
void SlaveTaskHandler(void *params)
{
	unsigned int xSlavePassId;
   portBASE_TYPE xStatus; // This is used for watching port Status

  while(1)
  {
	if(xSemaphoreTake(xSemaphore,portMAX_DELAY) == pdTRUE)
	{
       xStatus = xQueueReceive(xQueue,&xSlavePassId,0);

       if(xStatus != pdPASS)
       {
 	    sprintf(myMSG,"Queue Recieve Task Failed.Queue is Empty!\r\n");
 	    printMsg(myMSG);

        }
       else
       {
 	     SlaveDoWork(xSlavePassId);

       }
	}
  }

}
