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
void EXTI_Config(void);

/**************************************** PRIVATE VARIABLES*****************************************/

char myMSG[50];


xSemaphoreHandle xCountingSemaphore = NULL;
/**************************************** PRIVATE FUNCTIONS*****************************************/
void Handler_Task_Handler(void *params);
void Periodic_Task_Handler(void *params);


int main(void)
{

	RCC_DeInit(); //HSI ON  | HSE OFF | PLL OFF | Systemclock 16 Mhz;
	SystemCoreClockUpdate(); // Update SystemClock to 16MHz
    GPIO_Config();
    UART_Config();
    EXTI_Config();
    sprintf(myMSG,"This is Counting Semaphore Interrupt Application!\r\n");
    printMsg(myMSG);

    xCountingSemaphore = xSemaphoreCreateCounting(10,0); // Create Counting Semaphore with 5 Key

    if(xCountingSemaphore != NULL)
    {
        sprintf(myMSG,"Semaphore and Tasks are Created!\r\n");
        printMsg(myMSG);
        xTaskCreate(Handler_Task_Handler,"Handler Task",300,NULL,1,NULL);
        xTaskCreate(Periodic_Task_Handler,"Periodic Task",300,NULL,3,NULL);
        vTaskStartScheduler();
    }
    else
    {
        sprintf(myMSG,"Semaphore and Tasks Could Not Created!\r\n");
        printMsg(myMSG);
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

	  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_Init(GPIOA,&GPIO_InitStruct);



}
void EXTI_Config(void)
{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE); //ENABLE SYSCFG FOR EXTI LINE

	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource13);//Selecting port and pin for EXTI

	  EXTI_InitTypeDef EXTI_InitStruct = {0};
	  GPIO_InitTypeDef GPIO_InitStruct ={0};

	  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_Init(GPIOC,&GPIO_InitStruct);

	  EXTI_InitStruct.EXTI_Line = EXTI_Line13;
	  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;

	  EXTI_Init(&EXTI_InitStruct);

	  NVIC_SetPriority(EXTI15_10_IRQn,5);
	  NVIC_EnableIRQ(EXTI15_10_IRQn);


}
void EXTI15_10_IRQHandler()
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    sprintf(myMSG,"Interrupt is Active.\r\n");
    printMsg(myMSG);

    xSemaphoreGiveFromISR(xCountingSemaphore,&xHigherPriorityTaskWoken);
    xSemaphoreGiveFromISR(xCountingSemaphore,&xHigherPriorityTaskWoken);
    xSemaphoreGiveFromISR(xCountingSemaphore,&xHigherPriorityTaskWoken);
    xSemaphoreGiveFromISR(xCountingSemaphore,&xHigherPriorityTaskWoken);
    xSemaphoreGiveFromISR(xCountingSemaphore,&xHigherPriorityTaskWoken);


    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);

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

void Handler_Task_Handler(void *params)
{

	while(xSemaphoreTake(xCountingSemaphore,portMAX_DELAY))
	{
	      sprintf(myMSG,"Handler Task Processing Event.\r\nRemaining Keys:%u\r\n",
	    		  uxSemaphoreGetCount(xCountingSemaphore));
	      printMsg(myMSG);

	}



}


void Periodic_Task_Handler(void *params)
{
	while(1)
	{
      vTaskDelay(pdMS_TO_TICKS(500));

      sprintf(myMSG,"Periodic Task: Pending the Interrupt \r\n");
      printMsg(myMSG);

      NVIC_SetPendingIRQ(EXTI15_10_IRQn);

      sprintf(myMSG,"Periodic Task: Resuming \r\n");
      printMsg(myMSG);


	}

}
