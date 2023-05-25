#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

void Task1Handler(void *params);
void Task2Handler(void *params);
void GPIO_Config(void);
void UART_Config(void);
void printMsg(char *msg);
char myMessage[50];
TaskHandle_t Task1Handle = NULL ;
TaskHandle_t Task2Handle = NULL ;
uint8_t count = 0;
uint8_t buttonstate =0;
int main(void)
{
  RCC_DeInit();   // HSE OFF | HSI ON |	PLL OFF |
  SystemCoreClockUpdate(); // SYSTEM CLOCK 16 MHz

  GPIO_Config();
  UART_Config();
  xTaskCreate(Task1Handler,"Task 1",configMINIMAL_STACK_SIZE,NULL,4,&Task1Handle);
  xTaskCreate(Task2Handler,"Task 2",configMINIMAL_STACK_SIZE,NULL,2,&Task2Handle);

  vTaskStartScheduler(); // START SCHEDULAR
  while (1)
  {


  }
}

void Task1Handler(void *params)
{
	UBaseType_t p1,p2;

	snprintf(myMessage,50,"Task-1 is Running\r\n");
	printMsg(myMessage);

	snprintf(myMessage,50,"Task-1 priority : %d \r\n",(uint8_t)uxTaskPriorityGet(Task1Handle));
	printMsg(myMessage);

	snprintf(myMessage,50,"Task-2 priority : %d \r\n",(uint8_t)uxTaskPriorityGet(Task2Handle));
	printMsg(myMessage);
	   while(1)
	 {
		snprintf(myMessage,50,"Task-1 is Running\r\n");
		printMsg(myMessage);
         if(buttonstate == 1)
         {
        	 buttonstate=0;
        	 p1 = uxTaskPriorityGet(Task1Handle);
             p2 = uxTaskPriorityGet(Task2Handle);
             vTaskPrioritySet(Task1Handle,p2);
             vTaskPrioritySet(Task2Handle,p1);


         }

	 }
}

void Task2Handler(void *params)
{
	UBaseType_t p1,p2;

	snprintf(myMessage,50,"Task-2 is Running\r\n");
	printMsg(myMessage);

	snprintf(myMessage,50,"Task-1 priority : %d \r\n",(uint8_t)uxTaskPriorityGet(Task1Handle));
	printMsg(myMessage);

	snprintf(myMessage,50,"Task-2 priority : %d \r\n",(uint8_t)uxTaskPriorityGet(Task2Handle));
	printMsg(myMessage);

	   while(1)
	 {
			snprintf(myMessage,50,"Task-2 is Running\r\n");
			printMsg(myMessage);
	         if(buttonstate == 1)
	         {
	        	 buttonstate=0;
	        	 p1 = uxTaskPriorityGet(Task1Handle);
	             p2 = uxTaskPriorityGet(Task2Handle);
	             vTaskPrioritySet(Task1Handle,p2);
	             vTaskPrioritySet(Task2Handle,p1);


	         }


	 }
}
void GPIO_Config(void)
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

  NVIC_SetPriority(EXTI15_10_IRQn,15);
  NVIC_EnableIRQ(EXTI15_10_IRQn);

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

void EXTI15_10_IRQHandler()
{
  EXTI_ClearITPendingBit(EXTI_Line13);
  if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13))
  {
		 buttonstate = 1;
  }

}
void printMsg(char *msg)
{
   for(int i = 0 ; i< strlen(msg);i++)
   {
	   while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)!=SET);
	   USART_SendData(USART2,msg[i]);

   }

}
