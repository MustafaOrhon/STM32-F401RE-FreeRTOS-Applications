#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "stdio.h"
#include "string.h"
/****************************PRIVATE DEFINES********************************************/
#define LED_ON            1
#define LED_OFF           2
#define LED_TOGGLE_ON     3
#define LED_TOGGLE_OFF    4
#define LED_STATUS        5
#define EXIT_API          6
/****************************PRIVATE FUNCTIONS********************************************/
void GPIO_Config(void);
void UART_Config(void);
void printMsg(char *msg);
void USART2_IRQHandler(void);
void LED_TOGG_Func(uint16_t toggletime);
void LED_TOGG_OFF(void);
uint8_t getCommandCode(uint8_t *buffer);
uint8_t getArguments(uint8_t *buffer);
void LED_TOGGLE_Func(void);
void LED_ERROR_Func(char *ledStateMsg);
void APP_EXIT_FUNC(void);
/****************************PRIVATE VARIABLES********************************************/
uint8_t command_buffer[20];
uint8_t command_length=0;
/****************************PRIVATE Timers********************************************/
TimerHandle_t ToggleTimerHandle = NULL;
/****************************PRIVATE VARIABLES********************************************/
char myMSG[50];
char Menu_Display[] = {"\r\n LED_ON                          ->1 \
                        \r\n LED_OFF                        ->2 \
	                    \r\n LED_TOGGLE ON      - >3 \
		                 \r\n LED_TOGGLE OFF     ->4 \
                        \r\n LED_STATUS               ->5 \
		                \r\n EXIT_API                         ->6 \
		                \r\n Type Your Option Here : "};
/****************************TASKS********************************************/
TaskHandle_t Task1MenuHandle = NULL ;
TaskHandle_t Task2CommandHandle = NULL;
TaskHandle_t Task3ProcessHandle = NULL ;
TaskHandle_t Task4UARTWriteQueueHandle = NULL ;

void Task1_Menu_Display_Handler(void *params);
void Task2_Command_Handler(void *params);
void Task3_Process_Handler(void *params);
void Task4_UART_Write_Queue_Handler(void *params);
void LED_STATUS_Func(char *ledStateMsg);
typedef struct APP_Command
{
	uint8_t COMMAND_NUM;
	uint8_t COMMAND_ARGS[10];
}APP_Command_t;

/****************************QUEUES********************************************/
QueueHandle_t Uart_Write_Queue = NULL;
QueueHandle_t Command_Queue = NULL;
int main(void)
{
  RCC_DeInit();   // HSE OFF | HSI ON |	PLL OFF |
  SystemCoreClockUpdate(); // SYSTEM CLOCK 16 MHz

  GPIO_Config();
  UART_Config();

  Uart_Write_Queue = xQueueCreate(10,sizeof(char*)); //40 BYTE
  Command_Queue    = xQueueCreate(10,sizeof(APP_Command_t*)); // 40 BYTE

  sprintf(myMSG,"This is Queue Command Processing Application!\r\n");
  printMsg(myMSG);

  if((Uart_Write_Queue != NULL) && (Command_Queue !=NULL))
  {
	  /*TASK CREATION BLOCK */
	  sprintf(myMSG,"Queue Creation Successful!\r\n");
	  printMsg(myMSG);
	  xTaskCreate(Task1_Menu_Display_Handler,"Task 1",500,NULL,1,&Task1MenuHandle);
	  xTaskCreate(Task2_Command_Handler,     "Task 2",500,NULL,2,&Task2CommandHandle);
	  xTaskCreate(Task3_Process_Handler,     "Task 3",500,NULL,2,&Task3ProcessHandle);
	  xTaskCreate(Task4_UART_Write_Queue_Handler,  "Task 4",500,NULL,2,&Task4UARTWriteQueueHandle);

	  vTaskStartScheduler(); // START SCHEDULAR

  }
  else
  {
	  sprintf(myMSG,"Queue Creation Failed\r\n");
	  printMsg(myMSG);
  }

  while (1)
  {


  }
}

void Task1_Menu_Display_Handler(void *params)
{
	char *rxData = Menu_Display;
	while(1)
	{
        // Send Data to UART QUEUE
        xQueueSend(Uart_Write_Queue,&rxData,portMAX_DELAY);

		// Wait Notify
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);




	}
}
void Task2_Command_Handler(void *params)
{
	uint8_t commandCode = 0;
    APP_Command_t *appTxCmd;
	while(1)
	{
         xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
         appTxCmd = (APP_Command_t*)pvPortMalloc(sizeof(APP_Command_t)); // ASSIGN MEMORY FOR OUR STRUCT

         taskENTER_CRITICAL();
         commandCode = getCommandCode(command_buffer);
         appTxCmd->COMMAND_NUM = commandCode;
         getArguments(appTxCmd->COMMAND_ARGS);
         taskEXIT_CRITICAL();
         xQueueSend(Command_Queue,&appTxCmd,portMAX_DELAY);

	}
}
void Task3_Process_Handler(void *params)
{
	APP_Command_t * appRxCmd;
	char cmdTxTaskMessage[50];

	uint16_t toggleTime = pdMS_TO_TICKS(500);

	while(1)
	{
       // Read From Command Queue
      xQueueReceive(Command_Queue,(void*)&appRxCmd,portMAX_DELAY);
	   // Process
      if(appRxCmd->COMMAND_NUM == LED_ON)
      {
    	  GPIO_WriteBit(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3,Bit_SET);
      }
      else if(appRxCmd->COMMAND_NUM == LED_OFF)
      {
    	  GPIO_WriteBit(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3,Bit_RESET);
      }
      else if(appRxCmd->COMMAND_NUM == LED_TOGGLE_ON)
      {
    	  LED_TOGG_Func(toggleTime);
      }
      else if(appRxCmd->COMMAND_NUM == LED_TOGGLE_OFF)
      {
    	  LED_TOGG_OFF();
      }

      else if(appRxCmd->COMMAND_NUM == LED_STATUS)
      {
           LED_STATUS_Func(cmdTxTaskMessage);
      }
      else if(appRxCmd->COMMAND_NUM == EXIT_API)
      {
         APP_EXIT_FUNC();
      }
      else
      {
    	  LED_ERROR_Func(cmdTxTaskMessage);
      }


	}
}
void Task4_UART_Write_Queue_Handler(void *params)
{
	char *txData = NULL;
	while(1)
	{

		// Read From UART WRÝTE QUEUE
		 xQueueReceive(Uart_Write_Queue,&txData,portMAX_DELAY);

		// Send Message
          printMsg(txData);


	}
}
void GPIO_Config(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);


  GPIO_InitTypeDef GPIO_InitStruct ={0};

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
	 GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
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
	 USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

	 NVIC_SetPriority(USART2_IRQn,5);
	 NVIC_EnableIRQ(USART2_IRQn);


}


void USART2_IRQHandler(void)
{
	uint8_t rxCharacter;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
   if(USART_GetFlagStatus(USART2,USART_IT_RXNE))
   {
	   rxCharacter = USART_ReceiveData(USART2);

	   if(rxCharacter != '\n')
	   {

		       command_buffer[command_length++] = rxCharacter;
	   }
       else
       {
           // Null-terminate the command buffer
           command_buffer[command_length] = '\0';
           // Replace the following line with your desired command processing logic
		   xTaskNotifyFromISR(Task2CommandHandle,0,eNoAction,&xHigherPriorityTaskWoken);
		   xTaskNotifyFromISR(Task1MenuHandle,0,eNoAction,&xHigherPriorityTaskWoken);
           // Reset the command buffer and length
           command_length = 0;
       }

	   USART_ClearFlag(USART2,USART_FLAG_RXNE);
	   if(xHigherPriorityTaskWoken == pdTRUE)
	   {
		   taskYIELD();
	   }

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
uint8_t getCommandCode(uint8_t *buffer)
{
   return buffer[0] - 48;

}
uint8_t getArguments(uint8_t *buffer)
{
   return buffer[0];

}
void LED_STATUS_Func(char *ledStateMsg)
{
    	sprintf(ledStateMsg,"\r\nLed Status is : %d \r\n",GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_0));
    	xQueueSend(Uart_Write_Queue,&ledStateMsg,portMAX_DELAY);

}
void LED_ERROR_Func(char *ledStateMsg)
{
    	sprintf(ledStateMsg,"\r\nWRONG INPUT.PLEASE ENTER VALID INPUT!\r\n");
    	xQueueSend(Uart_Write_Queue,&ledStateMsg,portMAX_DELAY);

}
void LED_TOGGLE_Func(void)
{
  GPIO_ToggleBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);

}
void LED_TOGG_Func(uint16_t toggletime)
{
   if(ToggleTimerHandle == NULL)
   {
	   // Software Timer Create
	    ToggleTimerHandle = xTimerCreate("LED TOGGLE TIMER",toggletime,pdTRUE,NULL,LED_TOGGLE_Func);
	  // Software Timer Start
	    xTimerStart(ToggleTimerHandle,portMAX_DELAY);

   }
   else
   {
	   xTimerStart(ToggleTimerHandle,portMAX_DELAY);

   }

}
void LED_TOGG_OFF(void)
{
	 xTimerStop(ToggleTimerHandle,portMAX_DELAY);

}
void APP_EXIT_FUNC(void)
{
	 sprintf(myMSG,"\r\nThe Application Exits.....\r\n");
	 printMsg(myMSG);
    // Delete Tasks
	 vTaskDelete(Task1MenuHandle);
	 vTaskDelete(Task2CommandHandle);
	 vTaskDelete(Task3ProcessHandle);
	 vTaskDelete(Task3ProcessHandle);

	// DISABLE ISR
	 USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);

	//DISABLE NVIC
	 NVIC_DisableIRQ(USART2_IRQn);
}
void vApplicationIdleHook(void)
{
   __WFI();
}
